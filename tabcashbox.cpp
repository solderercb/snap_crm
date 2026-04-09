#include "tabcashbox.h"
#include "ui_tabcashbox.h"
#include <QMessageBox>
#include <ProjectGlobals>
#include <Mainwindow>
#include <tabPrintDialog>
#include <SPermissions>
#include <SUserSettings>
#include <SComSettings>
#include <STableCashboxModel>
#include <SPeriodSelector>
#include <STableViewGridLayout>
#include <SStandardItemModel>
#include <SPaymentTypesModel>
#include <SSortFilterProxyModel>
#include <SSqlQueryModel>
#include <SCashRegisterModel>
#include <FlashPopup>

tabCashbox* tabCashbox::p_instance = nullptr;

tabCashbox *tabCashbox::getInstance(MainWindow *parent)
{
    if( !p_instance )
      p_instance = new tabCashbox(parent);
    return p_instance;
}

tabCashbox::tabCashbox(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabCashbox)
{
    logUserActivity();
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    m_allEnabledTypes = SCashRegisterModel::paymentTypesList(SPaymentTypesModel::Type::All);
    m_allEnabledTypes->filterTypes({SCashRegisterModel::ExpCustom});
    ui->comboBoxType->setModel(m_allEnabledTypes.get());
    ui->comboBoxType->setButtons("Clear");
    ui->comboBoxType->setPlaceholderText(tr("Все"));
    ui->comboBoxType->setCurrentIndex(-1);

    m_paymentSystems = new SSortFilterProxyModel();
    m_paymentSystems->setSourceModel(paymentSystemsModel);
    m_paymentSystems->setFilterRegularExpression(QRegularExpression("^(?!(-2)).*$"));
    m_paymentSystems->setFilterKeyColumn(1);
    ui->comboBoxPaymentSystem->setModel(m_paymentSystems);
    ui->comboBoxPaymentSystem->setButtons("Clear");
    ui->comboBoxPaymentSystem->setPlaceholderText(tr("Все"));
    ui->comboBoxPaymentSystem->setCurrentIndex(-1);

    ui->comboBoxCompany->setModel(companiesModel);
    ui->comboBoxCompany->setCurrentIndex(companiesModel->rowByDatabaseID(userDbData->company()));
    ui->comboBoxCompany->setEnabled(false); // В АСЦ кассовые операции не детерминированы по организации
//    connect(ui->comboBoxCompany, qOverload<int>(&SComboBox::currentIndexChanged),
//            this, [=](int index){m_company = index; filterChanged(index);});

    ui->comboBoxOffice->setModel(officesModel);
    ui->comboBoxOffice->setCurrentIndex(officesModel->rowByDatabaseID(userDbData->currentOffice()));
    ui->comboBoxOffice->setEnabled(permissions->viewForeignOfficeCashOrders);

    if(permissions->viewForeignOfficeCashOrders)
    {
        connect(ui->comboBoxOffice, qOverload<int>(&SComboBox::currentIndexChanged), this, &tabCashbox::officeChanged);
    }
    connect(ui->comboBoxType, qOverload<int>(&SComboBox::currentIndexChanged),
            this, [=](int index){m_paymentType = index; filterChanged(index);});
    connect(ui->comboBoxPaymentSystem, qOverload<int>(&SComboBox::currentIndexChanged),
            this, [=](int index){m_paymentSystem = index; filterChanged(index);});

    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &tabCashbox::lineEditSearchTextChanged);

    ui->widgetPeriodSelector->setMovingInterval(SPeriodSelector::Day);
    ui->widgetPeriodSelector->setDefaultPeriod();
    connect(ui->widgetPeriodSelector, &SPeriodSelector::refreshButtonClicked, this, &tabCashbox::refreshManual);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &tabCashbox::refreshManual);
    connect(ui->pushButtonMoveCash, &QPushButton::clicked, this, &tabCashbox::createMoveCashTab);

    ui->pushButtonWithdrawDeposition->setEnabled(false); // TODO: диалог внесения/выемки средств

#ifdef QT_DEBUG
    ui->widgetPeriodSelector->setPeriodBegin(QDate(2026,4,1));
    ui->widgetPeriodSelector->setPeriodEnd(QDate(2026,4,9));
#endif

    refreshManual();
}

tabCashbox::~tabCashbox()
{
    delete ui;
    p_instance = nullptr;
}

// TODO: в методе выполняются аналогичные другим вкладкам действия; возможно стоит их перенести в tabCommon
bool tabCashbox::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier))
            switch(keyEvent->key())
            {
                case Qt::Key_F: setFocusSearchField(); event->accept(); break;
                default: ;
            }
    }
    return tabCommon::event(event);
}

QString tabCashbox::tabTitle()
{
    return tr("Финансы");
}

void tabCashbox::setFocusSearchField()
{
    ui->lineEditSearch->setFocus(Qt::MouseFocusReason);
    ui->lineEditSearch->selectAll();
}

void tabCashbox::constructQueryClause(FilterList &filter)
{
    int index;

    filter.fields.append(STableViewBase::initFilterField("t1.`created`", FilterField::More, ui->widgetPeriodSelector->periodBegin()));
    filter.fields.append(STableViewBase::initFilterField("t1.`created`", FilterField::LessEq, ui->widgetPeriodSelector->periodEnd()));

    index = ui->comboBoxCompany->currentIndex();
    filter.fields.append(STableViewBase::initFilterField("t1.`company`", FilterField::Equals, companiesModel->databaseIDByRow(index)));

    index = ui->comboBoxOffice->currentIndex();
    filter.fields.append(STableViewBase::initFilterField("t1.`office`", FilterField::Equals, officesModel->databaseIDByRow(index)));

    index = ui->comboBoxType->currentIndex();
    if(index >= 0)
    {
        QModelIndex sourceIndex = m_allEnabledTypes->mapToSource(ui->comboBoxType->model()->index(index, 0));
        filter.fields.append(STableViewBase::initFilterField("t1.`type`", FilterField::Equals, sourceIndex.siblingAtColumn(1).data().toInt()));
    }

    index = ui->comboBoxPaymentSystem->currentIndex();
    if(index >= 0)
        filter.fields.append(STableViewBase::initFilterField("t1.`payment_system`", FilterField::Equals, m_paymentSystems->databaseIDByRow(index, "system_id")));

    if(!ui->lineEditSearch->text().isEmpty())
    {
        FilterList search;
        search.op = FilterList::Or;
        FilterField::Op matchFlag;
        if(userDbData->useRegExpSearch())
            matchFlag = FilterField::RegExp;
        else
            matchFlag = FilterField::Contains;

        search.fields.append(STableViewBase::initFilterField("t1.`notes`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));

        FilterList clientName;
        clientName.op = FilterList::Or;
        clientName.fields.append(STableViewBase::initFilterField("t2.`name`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
        clientName.fields.append(STableViewBase::initFilterField("t2.`surname`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
        clientName.fields.append(STableViewBase::initFilterField("t2.`ur_name`", matchFlag, ui->lineEditSearch->text(), Qt::CaseInsensitive));
        search.childs.append(clientName);

        filter.childs.append(search);
    }
}

void tabCashbox::loadBalances()
{
    // настройка шрифта (название, размер, стиль) и выравнивания задаются в дизайнере
    // шрифт полей для настраиваемых счетов копируется из 0-й строки
    QFormLayout* layout = dynamic_cast<QFormLayout*>(ui->groupBoxBalance->layout());
    auto w = dynamic_cast<QLabel*>(layout->itemAt(1)->widget());
    QFont font = w->font();
    Qt::Alignment almnt = w->alignment();
    QPalette green = paletteForValue(0);
    QPalette red = paletteForValue(1);

    auto updateLabelValue = [&](QLabel *l, double value){
        l->setText(sysLocale.toString(value, 'f', comSettings->classicKassa()?2:0));
        if(value > 0)
            l->setPalette(green);
        else if(value < 0)
            l->setPalette(red);
    };

    auto officeId = officesModel->databaseIDByRow(ui->comboBoxOffice->currentIndex());
    QStringList queryParts;
    auto balances = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    queryParts << QString("  SUM(IF(`payment_system` = %1, `summa`, 0)) AS 'balance_cash'").arg(Global::PaymentSystemIds::Cash);
    queryParts << QString("  SUM(IF(`payment_system` = %1, `summa`, 0)) AS 'balance_cashless'").arg(Global::PaymentSystemIds::Cashless);
    queryParts << QString("  SUM(IF(`payment_system` = %1, `summa`, 0)) AS 'balance_card'").arg(Global::PaymentSystemIds::Card);
    balances->exec(QUERY_SEL_CASHBOX_BALANCES(queryParts.join(",\n"), officeId));
    balances->first();

    updateLabelValue(ui->labelBalanceCashValue, balances->value(0).toDouble());
    updateLabelValue(ui->labelBalanceCashlessValue, balances->value(1).toDouble());
    updateLabelValue(ui->labelBalanceCardValue, balances->value(2).toDouble());

    // В БД и в АСЦ CRM Прочие платёжные системы (Настройки -» Финансы) не детерминированы по офису и организации;
    // если в организации несколько офисов, то в группе Баланс будут отображаться "чужие" счета
    ui->groupBoxBalance->setToolTip("");
    while(layout->rowCount() > 4)
    {
        layout->removeRow(3);
    }

    queryParts.clear();
    auto customIds = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    customIds->exec(QUERY_SEL_CASHBOX_CUSTOM_SYSTEMS_IDS_BALANCES(officeId));
    while(customIds->next())
    {
        queryParts << QString("  SUM(IF(`payment_system` = %1, `summa`, 0)) AS 'balance_%1'").arg(customIds->value(0).toInt());
    }

    if(queryParts.isEmpty())
        return;

    balances->exec(QUERY_SEL_CASHBOX_BALANCES(queryParts.join(",\n"), "SELECT `office` FROM `cash_orders` GROUP BY 1"));
    customIds->first();
    balances->first();
    bool addTooltip = 0;
    for(int r = 3, c = 0; c < balances->record().count(); r++, c++)
    {
        addTooltip |= customIds->value(1).isNull();
        QString label = paymentSystemsModel->getDisplayRole(customIds->value(0).toInt(), "system_id") + (customIds->value(1).isNull()?" *":"");
        auto labelValue = new QLabel(this);
        labelValue->setFont(font);
        labelValue->setAlignment(almnt);
        updateLabelValue(labelValue, balances->value(c).toDouble());
        layout->insertRow(r, label, labelValue);
        customIds->next();
    }
    if(addTooltip)
        ui->groupBoxBalance->setToolTip(("Счета, отмеченные *, являются общими для нескольких офисов и их суммарный баланс может отличаться от суммы под таблицей"));
}

void tabCashbox::loadMonthStat()
{
    auto layout = dynamic_cast<QGridLayout*>(ui->groupBoxMonthStat->layout());
    if(!layout)
        return;

    auto monthStat = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));

    auto updateLabelValue = [&](QLabel *l, QVariant value){
        l->setText(sysLocale.toString(value.toDouble(), 'f', comSettings->classicKassa()?2:0));
    };

    int layoutRow = 0;
    std::vector<int> discrSystems = {Global::PaymentSystemIds::Cash, Global::PaymentSystemIds::Cashless, Global::PaymentSystemIds::Card, 254, 255}; // 254 — все, кроме Наличных, Безнала, Карты;  255 — все счета
    QLabel* w = nullptr;

    QString sysClause;
    QString officeClause = QString::number(officesModel->databaseIDByRow(ui->comboBoxOffice->currentIndex()));
    QStringList otherSysList;

    foreach (auto sys, discrSystems)
    {
        switch(sys){
            case 254: sysClause = QString("NOT IN (%1)").arg(otherSysList.join(',')); break; // `payment_system` NOT IN (1, 2, ..., N)
            case 255: sysClause = "IN (SELECT `payment_system` FROM `cash_orders` GROUP BY 1)"; break;  // все
            default: otherSysList << QString::number(sys); sysClause = QString("IN (%1)").arg(sys); // `payment_system` IN (N)
        }
        monthStat->exec(QUERY_SEL_CASHBOX_MONTH_STAT(sysClause, officeClause));
        monthStat->first();
        for(int c = 1; c < 4; c++)
        {
            w = dynamic_cast<QLabel*>(layout->itemAt(layoutRow*4 + c)->widget());
            updateLabelValue(w, monthStat->value(c-1));
        }

        layoutRow++;
    }
}

/* Метод возвращает настройку цвета QPalette:
 *  зелёный для положительных
 *  красный для отрицательных
 * Настройка цвета выполняется в дизайнере для элементов группы "С начала месяца"
 * и копируется из первой строки
*/
QPalette tabCashbox::paletteForValue(bool forNegativeValues)
{
    auto layout = dynamic_cast<QGridLayout*>(ui->groupBoxMonthStat->layout());
    if(!layout)
        return QPalette();

    QLabel* w = nullptr;
    if(forNegativeValues)
        w = dynamic_cast<QLabel*>(layout->itemAt(2)->widget());
    else
        w = dynamic_cast<QLabel*>(layout->itemAt(1)->widget());

    if(!w)
        return QPalette();

    return w->palette();
}

void tabCashbox::constructMainTableQueryFilter()
{
    int id = -1;
    QString query;
    QString clause;
    QVariant value;
    FilterList filter;

    filter.op = FilterList::And;

    constructQueryClause(filter);
    ui->cashboxWidget->setFilter(filter);
}

void tabCashbox::filterChanged(int)
{
    constructMainTableQueryFilter();
    ui->cashboxWidget->refresh();
}

void tabCashbox::createMoveCashTab()
{
    MainWindow::getInstance()->createTabCashMoveExch();
}

void tabCashbox::officeChanged(const int index)
{
    m_office = index;
    refreshManual();
}

void tabCashbox::lineEditSearchTextChanged(QString)
{
    constructMainTableQueryFilter();
    ui->cashboxWidget->delayedRefresh(350);
}

/* Обновление, вызываемое программно
*/
void tabCashbox::refresh()
{
    ui->cashboxWidget->refresh(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionPreserve);
    loadMonthStat();
    loadBalances();
}

/* Обновление, вызываемое действием пользователя
*/
void tabCashbox::refreshManual()
{
    constructMainTableQueryFilter();
    ui->cashboxWidget->refresh(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
    loadMonthStat();
    loadBalances();
}

void tabCashbox::refreshIfTabExists()
{
    tabCashbox *tabInstance = nullptr;

    tabInstance = MainWindow::getInstance()->findChild<tabCashbox*>();
    if(tabInstance)
    {
        tabInstance->refresh();
    }
}
