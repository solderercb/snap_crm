#include "tabsale.h"
#include "ui_tabsale.h"
#include <QWidget>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QScrollBar>
#include <QMessageBox>
#include <ProjectGlobals>
#include <appVer>
#include <SComSettings>
#include <SUserSettings>
#include <SPermissions>
#include <SOfficeModel>
#include <SPhoneModel>
#include <SPhonesModel>
#include <SDocumentModel>
#include <SStandardItemModel>
#include <SSortFilterProxyModel>
#include <SSqlQueryModel>
#include <SSaleTableModel>
#include <SLogRecordModel>
#include <SCashRegisterModel>
#include <SClientModel>
#include <STableViewBOQItemDelegates>
#include <SSaleTabSettingsMenu>
#include <FlashPopup>
#include <STableViewBase>
#include <SStoreItemModel>
#include <QToolTipper>
#include <SLogRecordModel>
#include <SClientInputFormBase>

QMap<int, tabSale*> tabSale::p_instance;

tabSale::tabSale(int doc, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabSale),
    doc_id(doc)
{
    logUserActivity();

    ui->setupUi(this);
    tabSale::guiFontChanged();
    docModel = new SDocumentModel();
    tableModel = new StoreSaleModel(this);
    clientModel = new SClientModel();
    cashRegister = new SCashRegisterModel();

    params = new int;
    *params = 0;
    *params |= comSettings->printOutInvoice()?tabSaleSettingsMenu::PrintDoc:0;
    *params |= comSettings->printPKO()?tabSaleSettingsMenu::PrintCheck:0;
    widgetAction = new tabSaleSettingsMenu(this);
    ui->buttonParams->addAction(widgetAction);
    widgetAction->setParamsModel(params);

    ui->comboBoxPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxMoneyBackAccount->setModel(paymentSystemsModel);
    initPriceColModel();
    ui->comboBoxPriceCol->setModel(m_priceColProxyModel);
    ui->comboBoxCompany->setModel(companiesModel);
    ui->comboBoxClientPhoneType->setModel(clientPhoneTypesModel);
    ui->comboBoxClientPhoneType->setModelColumn(0);
    ui->comboBoxClientPhoneType->setCurrentIndex(0);
    ui->widgetClientMatch->setPhoneMask(0);
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    ui->spinBoxReserve->setMinimum(1);
    ui->lineEditTrack->setButtons("Apply");
    ui->widgetClientMatch->hide();
    connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillClientCreds(int)));
    ui->widgetClientMatch->setClientType(SClientMatch::NameSearchScope::Both);

    ui->tableView->setModel(tableModel);
    ui->tableView->viewport()->installEventFilter(new QToolTipper(ui->tableView));

//    connect(ui->lineEditAddByUID, SIGNAL(returnPressed()), this, SLOT(addItemByUID()));    // подключены в дизайнере
//    connect(ui->buttonReserve, SIGNAL(clicked()), this, SLOT(reserveButtonClicked()));    // подключены в дизайнере
//    connect(ui->buttonSale, SIGNAL(clicked()), this, SLOT(saleButtonClicked()));    // подключены в дизайнере
    connect(ui->comboBoxPriceCol, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPriceCol(int)));    // нужно подключать здесь, иначе возникает глюк с доступом к QMap<> fields
    connect(tableModel, &SSaleTableModel::amountChanged, this, &tabSale::updateTotalSumms);
    if(permissions->viewClients)
    {
        connect(ui->lineEditClientLastName, &QLineEdit::textEdited, ui->widgetClientMatch, &SClientMatch::findByName);
        connect(ui->comboBoxClientPhoneType,SIGNAL(currentIndexChanged(int)),ui->widgetClientMatch,SLOT(setPhoneMask(int)));
        connect(ui->lineEditClientPhone,SIGNAL(textEdited(QString)),this,SLOT(phoneNumberEdited(QString)));
    }


#ifdef QT_DEBUG
    createTestPanel();
#endif

    updateWidgets();
}

tabSale::~tabSale()
{
    // Bug: когда в настройках изменяется размер шрифта и применяется "на лету" к приложению, закрытие этой вкладки в фоне приводит к порче настроек таблицы (visualIndex'ы горизонтальных
    // заголовков равны -1). Чтобы настройки не портились, методом тыка определено, что перед удалением ui нужно как-то взаимодействовать с заголовком или переключиться на вкладку перед закрытием.
    ui->tableView->horizontalHeader()->visualIndex(0);
    delete ui;
    p_instance.remove(doc_id);   // Обязательно блять!
    delete params;
    delete widgetAction;
    delete clientModel;
    delete cashRegister;
    delete docModel;
#ifdef QT_DEBUG
    delete testPushButton;
    delete testBtnAddRandomItem;
    delete testLineEdit;
    delete testPanel;
#endif
}

QString tabSale::tabTitle()
{
    if(doc_id)
        return tr("Расходная накладная %1").arg(doc_id);
    return tr("Продажа");
}

// TODO: идентичный метод есть в классе tabPartRequest; возможно стоит его перенести в tabCommon
void tabSale::updateTabPtr(const int oldId, const int newId)
{
    if(oldId == newId)
        return;

    p_instance.remove(oldId);
    p_instance.insert(newId, this);
}

void tabSale::initPriceColModel()
{
    m_priceColProxyModel = new SSortFilterProxyModel();
    m_priceColProxyModel->setSourceModel(priceColModel);
    m_priceColProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(" + QString::number(SStoreItemModel::PriceOptionWarranty) + ")).*$"));
    m_priceColProxyModel->setFilterKeyColumn(1);
}

void tabSale::setDefaultStyleSheets()
{
    ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientFirstName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientPhone->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditTotal->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditTakeIn->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxClientPhoneType->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxMoneyBackAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPriceCol->setStyleSheet(commonComboBoxStyleSheet);
}

void tabSale::setBalanceWidgetsVisible(bool state)
{
    ui->labelBalance->setVisible(state);
    ui->lineEditBalance->setVisible(state);
    ui->checkBoxSaleInCredit->setVisible(state);
}

void tabSale::eventResize(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void tabSale::updateWidgets()
{
    QSqlQuery query;

    emit updateTabTitle(this);
    setDefaultStyleSheets();
    if(doc_id)
    {
        int docState = -1;
        tableModel->setState(StoreSaleModel::Sold);
        docModel->load(doc_id);
        if(!docModel->isPrimaryKeyValid())
            return;
        docState = docModel->state();

        ui->labelDocNum->show();
        ui->lineEditDocNum->show();
        ui->lineEditDocNum->setText(QString::number(doc_id));
        ui->labelDate->show();
        ui->lineEditDate->show();
        ui->lineEditDate->setText(docModel->createdStr());
        ui->comboBoxPaymentAccount->setCurrentIndex(docModel->paymentSystemIndex());
        ui->comboBoxPaymentAccount->setEnabled(false);
        ui->comboBoxMoneyBackAccount->setCurrentIndex(docModel->paymentSystemIndex());  // возврат на тот же счет, куда поступала оплата
        ui->comboBoxCompany->setCurrentIndex(docModel->companyIndex());
        ui->spinBoxReserve->setValue(docModel->reserveDays());
        ui->spinBoxReserve->setReadOnly(true);
        ui->lineEditTrack->setText(docModel->trackingNumber());
        ui->buttonLog->show();
        ui->buttonReserve->hide();   // не допускается повтоное нажатие "Резерв"
        if(auto client = docModel->client())
        {
            fillClientCreds(client);
        }
        else
            ui->checkBoxAnonymous->setChecked(true);

        ui->comboBoxPriceCol->setCurrentIndex(docModel->priceOptionIndex());
        ui->comboBoxPriceCol->setEnabled(false);
        ui->checkBoxAnonymous->setEnabled(false);
        ui->lineEditComment->setText(docModel->notes());
        ui->lineEditComment->setReadOnly(true);
        ui->buttonAdd->hide();
        ui->pushButtonClientCredsClearAll->setEnabled(false);
        ui->pushButtonClientFromDB->setEnabled(false);

        if(docState == SDocumentModel::ItemsReserved)
        {
            tableModel->setState(StoreSaleModel::Reserved);
            m_opType = SaleReserved;
            ui->comboBoxPaymentAccount->setEnabled(true);
            ui->comboBoxPriceCol->setEnabled(true);
            ui->lineEditTakeIn->setReadOnly(false);
            ui->lineEditAddByUID->setReadOnly(false);
            ui->labelReserve->hide();
            ui->spinBoxReserve->hide();
            ui->labelReserveProgress->show();
            ui->progressBarReserve->show();
            ui->progressBarReserve->setMaximum(docModel->reserveDays());
            ui->progressBarReserve->setMinimum(0);
            ui->progressBarReserve->setFormat("%v");
            ui->progressBarReserve->setValue( qMax( (int)(docModel->reserveDays() - docModel->createdUtc().daysTo(QDateTime::currentDateTimeUtc())), 0 ));
            ui->groupBoxAdm->hide();
            ui->labelTrack->hide();
            ui->lineEditTrack->hide();
            ui->buttonPrint->hide();
            ui->buttonReserveCancel->show();
            ui->buttonSaleMore->show();
            ui->buttonSale->show();
            tableModel->loadTable(doc_id);
#ifdef QT_DEBUG
            ui->lineEditTakeIn->setText(sysLocale.toString(sysLocale.toDouble(ui->lineEditTotal->text()) + QRandomGenerator::global()->bounded(100), 'f', 2)); // это для отладки
#else
            ui->lineEditTakeIn->setText(sysLocale.toString(0.00, 'f', 2));
#endif
        }
        else if(docState == SDocumentModel::ReserveCancelled || docState == SDocumentModel::OutInvoiceCancelled)   // если открыта РН снятого резерва или распроведённая РН
        {
            ui->lineEditTakeIn->setReadOnly(true);
            ui->lineEditAddByUID->setReadOnly(true);
            ui->labelReserve->hide();
            ui->spinBoxReserve->hide();
            ui->groupBoxAdm->hide();
            ui->labelReserveProgress->hide();
            ui->progressBarReserve->hide();
            ui->groupBoxAdm->hide();
            ui->buttonPrint->hide();
            ui->buttonReserveCancel->hide();
            ui->buttonSaleMore->hide();
            ui->buttonSale->hide();
            ui->labelTrack->hide();
            ui->lineEditTrack->hide();
            tableModel->setState(StoreSaleModel::Cancelled);
            tableModel->loadTable(doc_id);
            ui->lineEditTakeIn->setText(sysLocale.toString(0.00, 'f', 2));
            ui->comboBoxCompany->setEnabled(false);
        }
        else    // если открыта проведённая РН
        {
            ui->lineEditAddByUID->setReadOnly(true);
            ui->labelReserve->hide();
            ui->spinBoxReserve->hide();
            ui->labelReserveProgress->hide();
            ui->progressBarReserve->hide();
            ui->checkBoxSaleInCredit->setEnabled(false);
            ui->labelTrack->show();
            ui->lineEditTrack->show();
            ui->buttonPrint->show();            // будут отображаться только кнопки "Печать", "История" и "Параметры"
            ui->buttonReserveCancel->hide();
            ui->buttonSaleMore->hide();
            ui->buttonSale->hide();
            if(permissions->undoOutInvoice)
            {
                ui->groupBoxAdm->show();
            }
            else
            {
                ui->groupBoxAdm->hide();
            }
            tableModel->setState(StoreSaleModel::Sold);
            tableModel->loadTable(doc_id);
            ui->lineEditTotal->setText(docModel->amountLocal());  // устанавливать суммы нужно только после заполнения таблицы
            ui->lineEditTakeIn->setReadOnly(true);
            ui->lineEditTakeIn->setText(tableModel->amountTotalLocale());  // тут применю хитрость: поля будут заполняться из разных "источников" и, в случае возврата товаров, сравниваться
            ui->comboBoxCompany->setEnabled(false);
        }
    }
    else    // новая расходная накладная
    {
        clearAll();
        setBalanceWidgetsVisible(false);
        m_opType = Sale;    // это флаг для функции sale()
        ui->lineEditTakeIn->setReadOnly(false);
        ui->labelDate->hide();
        ui->lineEditDate->hide();
        ui->labelDocNum->hide();
        ui->lineEditDocNum->hide();
        ui->checkBoxAnonymous->setChecked(false);
        ui->checkBoxAnonymous->setEnabled(true);
        ui->groupBoxAdm->hide();
        ui->labelTrack->hide();
        ui->labelTrack->setText("");
        ui->textEditRevertReason->setText("");
        ui->lineEditTrack->hide();
        ui->labelReserve->show();
        ui->spinBoxReserve->show();
        ui->labelReserveProgress->hide();
        ui->progressBarReserve->hide();
        ui->comboBoxPaymentAccount->setCurrentText(paymentSystemsModel->getDisplayRole(0, 1));
        ui->comboBoxPaymentAccount->setEnabled(true);
        ui->comboBoxPriceCol->setCurrentText(priceColModel->getDisplayRole(2, 1));  // по умолчанию Розница
        ui->comboBoxPriceCol->setEnabled(true);
        ui->comboBoxCompany->setCurrentIndex(SOfficeModel::current()->defaultCompanyIndex());
        if(companiesModel->rowCount() == 1)
        {
            ui->comboBoxCompany->setVisible(false);
            ui->labelCompany->setVisible(false);
        }
        ui->comboBoxCompany->setEnabled(true);
        ui->spinBoxReserve->setValue(comSettings->defaultItemReserveTime());
        ui->spinBoxReserve->setReadOnly(false);
        ui->lineEditAddByUID->setReadOnly(false);
        ui->buttonPrint->hide();            // будут отображаться только кнопки "Параметры", "Добавить", "Резерв", "Продать ещё" и "Продать"
        ui->buttonLog->hide();
        ui->buttonAdd->show();
        ui->buttonReserve->show();
        ui->buttonReserveCancel->hide();
        ui->buttonSaleMore->show();
        ui->buttonSale->show();
        ui->lineEditComment->setReadOnly(false);
        ui->pushButtonClientCredsClearAll->setEnabled(true);
        ui->pushButtonClientFromDB->setEnabled(permissions->viewClients);

        tableModel->setState(StoreSaleModel::New);

#ifdef QT_DEBUG
        test_scheduler_counter = 0;
        test_scheduler->start(200);
#endif
    }
    ui->widgetClientMatch->hide();
}

int tabSale::checkInputNew()
{
    int error = 0;

    // TODO: часть кода взята из класса tabRepairNew; возможно стоит создать общую функцию для проверки введённых данных

    if( !ui->checkBoxAnonymous->isChecked() )
    {
        if (ui->lineEditClientLastName->text() == "" && !clientModel->id())       // проверка фамилии только при создании нового клиента
        {
            ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheetRed);
            error = 1;
        }
        if (ui->lineEditClientFirstName->text() == "")     // если не указано имя
        {
            ui->lineEditClientFirstName->setStyleSheet(commonLineEditStyleSheetRed);
            error = 2;
        }
        if (ui->lineEditClientPatronymic->text() == "" && comSettings->isClientPatronymicRequired() && !m_client)   // если не указано отчество и оно обязятельно
        {
            ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheetRed);
            error = 3;
        }
        if (ui->comboBoxClientAdType->currentIndex() < 0 && comSettings->isVisitSourceRequired() && !m_client)        // если не указан источник обращения, а он обязателен и клиент новый
        {
            ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheetRed);
            error = 4;
        }
        if (!ui->lineEditClientPhone->hasAcceptableInput() && comSettings->isClientPhoneRequired() && clientModel->isNew())   // если не указан телефон и он обязятелен
        {
            ui->lineEditClientPhone->setStyleSheet(commonLineEditStyleSheetRed);
            error = 5;
        }
    }
    if( (paymentSystemsModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id") == -2) && \
            (!(clientModel->isBalanceEnabled()) || ui->checkBoxAnonymous->isChecked()) )
    {
        ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 6;
    }
    if ((m_opType != Reserve) && (sysLocale.toDouble(ui->lineEditTakeIn->text()) < sysLocale.toDouble(ui->lineEditTotal->text())) )   //
    {
        ui->lineEditTakeIn->setStyleSheet(commonLineEditStyleSheetRed);
        error = 7;
    }
    if (tableModel->rowCount() == 0)   //    в таблице нет строк
    {
        // будет слишкм стрёмно, если окрасить tableView в красный, поэтому просто ничего не делаем.
        error = 9;
    }
    if ( sysLocale.toDouble(ui->lineEditTotal->text()) <= 0 )
    {
        ui->lineEditTotal->setStyleSheet(commonLineEditStyleSheetRed);
        error = 10;
    }
    for(int i = 0; i < tableModel->rowCount(); i++) // TODO: Это нужно перенести в SSaleTableModel
    {
        if(!tableModel->state(i))   // только не помеченные на снятие резерва/возврат
        {
            if ( tableModel->price(i) <= 0 ) // цена меньше нуля
            {
                tableModel->setData(tableModel->index(i, SSaleTableModel::Columns::Price), QColor(255, 209, 209), Qt::BackgroundRole);

                error = 11;
            }
            if( tableModel->price(i) < tableModel->unformattedData(i, SSaleTableModel::Columns::InPrice).toDouble() ) // цена меньше закупочной
            {
                QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                            tr("Цена ниже закупочной, продолжить?\n\"%1\"").arg(tableModel->name(i)),
                                                                            QMessageBox::No | QMessageBox::Yes,
                                                                            QMessageBox::No);
                if (resBtn == QMessageBox::No)
                {
                    error = 12;
                }
            }
        }
    }
    if ( ui->comboBoxCompany->currentIndex() < 0)
    {
        ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 13;
    }
//    if ()   //
//    {
//        ui->->setStyleSheet(commonLineEditStyleSheetRed);
//        error = ;
//    }

    if (error)
    {
        qDebug() << "Ошибка создания РН: возможно, не все обязательные поля заполнены (error " << error << ")";
        return 1;
    }

    return 0;
}

void tabSale::commitClient()
{
    clientModel->set_firstName(ui->lineEditClientFirstName->text());
    clientModel->set_lastName(ui->lineEditClientLastName->text());
    clientModel->set_patronymicName(ui->lineEditClientPatronymic->text());
    clientModel->appendLogText(tr("Быстрое создание клиента из формы продажи"), "!");
    if(ui->comboBoxClientAdType->currentIndex() >= 0)
        clientModel->set_adType(clientAdTypesList->databaseIDByRow(ui->comboBoxClientAdType->currentIndex()));
    if (clientModel->phones()->isEmpty() && ui->lineEditClientPhone->hasAcceptableInput())
    {
        clientModel->addPhone(ui->lineEditClientPhone->text(),
                              ui->comboBoxClientPhoneType->currentIndex());
    }
    clientModel->commit();

#ifdef QT_DEBUG
//    Global::throwDebug();
#endif
}

int tabSale::checkInput()
{

    setDefaultStyleSheets();
    switch(m_opType)
    {
        case CancelReserve:
        case SetTrack: return 0;
        case Unsale: return checkInputUnsale();
        default: return checkInputNew();
    }
}

void tabSale::updateTotalSumms(const double amountTotal, const double, const double)
{
    double takeInSumm = sysLocale.toDouble(ui->lineEditTakeIn->text());
    ui->lineEditTotal->setText(sysLocale.toString(amountTotal, 'f', 2));
    if(takeInSumm >= amountTotal)
        ui->lineEditCharge->setText(sysLocale.toString(takeInSumm - amountTotal, 'f', 2));
    else
        ui->lineEditCharge->setText(sysLocale.toString(0.00, 'f', 2));
}

void tabSale::phoneNumberEdited(QString)
{
    if(permissions->viewClients)
        ui->widgetClientMatch->findByPhone(ui->lineEditClientPhone->displayText(), ui->comboBoxClientPhoneType->currentIndex());
}

void tabSale::clearClientCreds(bool hideCoincidence)
{
    setDefaultStyleSheets();
    if(!m_client)
        return;

    if(clientModel->id())
        clientModel->clear();

    m_client = 0;
    bool updatePriceOption = tableModel->state() != StoreSaleModel::New || !docModel->isDirty();
    price_col = 2;
    ui->comboBoxPriceCol->setCurrentText(priceColModel->getDisplayRole(price_col, 1));
    ui->lineEditClientLastName->setReadOnly(false);     // разрешаем ввод текста (вдруг он был запрещён)
    ui->lineEditClientFirstName->setReadOnly(false);
    ui->lineEditClientPatronymic->setReadOnly(false);
    ui->lineEditClientPhone->setReadOnly(false);
    ui->pushButtonCreateTabClient->setEnabled(false);
    ui->pushButtonClientFromDB->setEnabled(permissions->viewClients);
    ui->pushButtonClientCredsClearAll->setEnabled(true);
    ui->lineEditClientLastName->clear();
    ui->lineEditClientFirstName->clear();
    ui->lineEditClientPatronymic->clear();
    ui->lineEditClientPhone->clear();
    ui->lineEditClientPhone->setReadOnly(false);
    ui->comboBoxClientPhoneType->setEnabled(true);
    ui->lineEditBalance->clear();
    setBalanceWidgetsVisible(false);
    ui->comboBoxClientPhoneType->setCurrentIndex(clientPhoneTypesModel->index(0, 0).row());     // устанавливаем первый элемент выпадающего списка
    ui->comboBoxClientAdType->setEnabled(true);
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    ui->checkBoxSaleInCredit->setChecked(false);
    if (hideCoincidence)
        ui->widgetClientMatch->hide();
}

void tabSale::fillClientCreds(int id)
{
    if(m_client == id)
        return;

    clearClientCreds(false);    // очищаем данные клиента, но не прячем таблицу совпадений
    bool updatePriceOption = tableModel->state() != StoreSaleModel::New || !docModel->isDirty();
    m_client = id;                // установка нового значения должна выполняться после очистки
    clientModel->load(m_client);
    docModel->set_client(m_client);
    tableModel->setClient(m_client);
    cashRegister->set_client(m_client);
    ui->lineEditClientFirstName->setText(clientModel->firstName());
    ui->lineEditClientLastName->setText(clientModel->lastName());
    ui->lineEditClientPatronymic->setText(clientModel->patronymicName());
    ui->lineEditClientFirstName->setReadOnly(true);
    ui->lineEditClientLastName->setReadOnly(true);  // запрет на изменение, если клиент из базы
    ui->lineEditClientPatronymic->setReadOnly(true);

    ui->comboBoxClientPhoneType->setCurrentIndex(clientModel->phones()->primary()->maskIndex());
    ui->lineEditClientPhone->setText(clientModel->phones()->primary()->phone());
    ui->lineEditClientPhone->setReadOnly(true);
    ui->comboBoxClientPhoneType->setEnabled(false);

    ui->pushButtonCreateTabClient->setEnabled(permissions->viewClients);
    ui->comboBoxClientAdType->setEnabled(false);
    ui->checkBoxAnonymous->setChecked(false);

    if(updatePriceOption)
        ui->comboBoxPriceCol->setCurrentIndex(clientModel->priceColumnIndex());
    ui->comboBoxClientAdType->setCurrentIndex(clientModel->adTypeIndex());
    if(clientModel->isBalanceEnabled())
    {
        setBalanceWidgetsVisible(true);
        ui->checkBoxSaleInCredit->setEnabled(true);
        ui->lineEditBalance->setText(sysLocale.toString(clientModel->balance(), 'f', 2));
    }
    else
        setBalanceWidgetsVisible(false);
}

void tabSale::buttonSelectExistingClientHandler()
{
    emit createTabSelectExistingClient(1, this);
}

void tabSale::buttonCreateTabClientHandler()
{
    emit createTabClient(m_client);
}

void tabSale::tableRowDoubleClick(QModelIndex tableIndex)
{
    emit createTabSparePart(tableModel->itemId(tableIndex.row()));
}

void tabSale::hideGroupBoxClient(bool isAnonymousBuyer)
{
    if(isAnonymousBuyer)
    {
        ui->groupBoxClient->hide();
        ui->buttonReserve->setEnabled(false);   // думаю нельзя резервировать товар на анонимного клиента
        cashRegister->unsetClient();
        tableModel->unsetClient();
        docModel->unsetClient();
    }
    else
    {
        ui->groupBoxClient->show();
        ui->buttonReserve->setEnabled(true);
        if(m_client)
        {
            cashRegister->set_client(m_client);
            tableModel->setClient(m_client);
            docModel->set_client(m_client);
        }
    }
}

void tabSale::selectPriceCol(int index)
{
    if(tableModel->state() == StoreSaleModel::New || tableModel->state() == StoreSaleModel::Reserved)
        tableModel->setPriceColumn((SStoreItemModel::PriceOption)m_priceColProxyModel->databaseIDByRow(index));
}

void tabSale::addItemByUID()
{
    if(tableModel->addItemByID(ui->lineEditAddByUID->text().toInt()))
        ui->lineEditAddByUID->setText("");
}

void tabSale::print()
{
    switch(m_opType)
    {
        case Reserve:
        case CancelReserve: return;
        case Unsale: /* TODO: печать РКО */ return;
        case SetTrack: return;
    }

    if(*params & tabSaleSettingsMenu::PrintDoc)     // печать РН
    {
        QMap<QString, QVariant> report_vars;
        report_vars.insert("type", Global::Reports::rn);
        report_vars.insert("doc_id", doc_id);
        emit generatePrintout(report_vars);
    }
    if(*params & tabSaleSettingsMenu::PrintCheck && cashRegister->isPrimaryKeyValid())   // печать ПКО
    {
        QMap<QString, QVariant> report_vars;
        report_vars.insert("type", Global::Reports::pko);
        report_vars.insert("order_id", cashRegister->isPrimaryKeyValid());
        emit generatePrintout(report_vars);
    }

    // TODO: печать чека с пом. РРО
}

int tabSale::commitStages()
{
    switch(m_opType)
    {
        case Unsale:
        case CancelReserve:
        case SetTrack: return 1;
        case Sale:
        case Reserve: return 2;   // 2 этапа: запись данных клиента, запись данных о продаже/резерве
        case SaleReserved: return 2; // в случае продажи ранее зарезервированного товара, первый этап будет пропущен
    }

    return 0;
}

int tabSale::checkInputUnsale()
{
    bool balance = (targetPaymentAccount() == -2);
    if( balance && (!(clientModel->isBalanceEnabled()) || ui->checkBoxAnonymous->isChecked()) )
    {
        ui->comboBoxMoneyBackAccount->setStyleSheet(commonComboBoxStyleSheetRed);
        return 1;
    }

    return 0;
}

void tabSale::commitUnsale()  // распроведение
{
    // TODO: нужно изучить вопрос возврата товара при условии использования РРО, возможно, для этого нужно создавать отдельный документ на возврат
    double amount = 0;
    int paymentAccount = targetPaymentAccount();
    bool isAnonymous = ui->checkBoxAnonymous->isChecked();

    tableModel->backOutItems(StoreSaleModel::OpType::Unsale);
    docModel->commit();

    if(!isAnonymous)
        clientModel->updatePurchases(-tableModel->itemsAffected());
    amount = sysLocale.toDouble(ui->lineEditCharge->text()); // в поле Сдача будет записана сумма товаров, помеченных на удаление (т. е. сумма возврата)
    if(paymentAccount == -2)
    {
        clientModel->updateBalance(amount, QString("Зачисление %1 - возврат за товары по РН №%2").arg(sysLocale.toCurrencyString(amount)).arg(doc_id), SBalanceLogRecordModel::RoyaltyReason::Document, doc_id);
    }
    else
    {
        prepareCashRegisterModel(amount, paymentAccount);
        cashRegister->commit(-amount);
    }

#ifdef QT_DEBUG
//    Global::throwDebug();
#endif
}

void tabSale::clearAll()
{
    clearClientCreds(1);
    ui->checkBoxAnonymous->setChecked(false);
    ui->lineEditTotal->setText(sysLocale.toString(0.00, 'f', 2));
    ui->lineEditTakeIn->setText("");
    ui->lineEditComment->setText("");
    tableModel->clear();
    delete docModel;
    delete clientModel;
    docModel = new SDocumentModel();
    clientModel = new SClientModel();
}

int tabSale::targetPaymentAccount()
{
    int paymentSystemIndex = -1;
    if(m_opType == Unsale)
    {
        paymentSystemIndex = ui->comboBoxMoneyBackAccount->currentIndex();
    }
    else    // Sale, SaleReserved
    {
        if(ui->checkBoxSaleInCredit->isChecked())
            return -2;
        else
            paymentSystemIndex = ui->comboBoxPaymentAccount->currentIndex();
    }

    return paymentSystemsModel->databaseIDByRow(paymentSystemIndex, "system_id");
}

/* Подготовка модели документа
*/
void tabSale::prepareDocModel()
{
    switch(m_opType)
    {
        case CancelReserve: tableModel->prepareFreeItems(); break;
        case Unsale: tableModel->prepareUnsaleItems(ui->textEditRevertReason->toPlainText()); break;
        case SetTrack: return;
        default: ;
    }

    int reserveDays = 5, priceCol = 2;
    double amount, currency = 0.00 /* TODO: запрос валюты, даже если программа не перезапускалась несколько дней */;

    amount = tableModel->amountTotal();
    priceCol = priceColModel->index(ui->comboBoxPriceCol->currentIndex(), 1).data().toInt();
    reserveDays = ui->spinBoxReserve->value();

    int type = SDocumentModel::OutInvoice;
    int state = docModel->state();
    int paymentAccount = targetPaymentAccount();
    docModel->setCompanyIndex(ui->comboBoxCompany->currentIndex());
    // Выбор офиса пользователем не предусмотрен; используется текущее значение из класса userDbData
    docModel->set_amount(amount);
    docModel->set_notes(ui->lineEditComment->text());
    docModel->set_priceOption(priceCol);
    docModel->set_reserveDays(reserveDays);
    switch (m_opType)
    {
        case Sale: state = SDocumentModel::OutInvoicePayed; break;
        case Reserve: type = SDocumentModel::ReserveInvoice; state = SDocumentModel::ItemsReserved; break;
        case SaleReserved: state = SDocumentModel::OutInvoicePayed; break;
        case Unsale: if(amount == 0) {state = SDocumentModel::OutInvoiceCancelled;}; paymentAccount = docModel->paymentSystem(); break;
        case CancelReserve: if(amount == 0) {state = SDocumentModel::ReserveCancelled;}; break;
        default: break;
    }
    docModel->set_type(type);
    docModel->set_state(state);
    docModel->set_paymentSystem(paymentAccount);
}

void tabSale::prepareCashRegisterModel(const double amount, const int account)
{
    if(account == -2)    // баланс
        return;

    switch(m_opType)
    {
        case Reserve:
        case CancelReserve:
        case SetTrack: return;
        default: ;
    }

    cashRegister->set_company(docModel->company());
    cashRegister->set_document(doc_id);
    cashRegister->set_systemId(account);
    cashRegister->set_client(docModel->client());

    if(m_opType == Unsale)
    {
        cashRegister->set_operationType(SCashRegisterModel::ExpGoods);
        cashRegister->set_reason(QString("Расход денег в размере %2 - возврат за товары по РН №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)));
    }
    else
    {
        cashRegister->set_operationType(SCashRegisterModel::RecptGoods);
        cashRegister->set_reason(QString("Поступление денег в размере %2 по расходной накладной №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)));
    }
}

QString tabSale::prepareUnsaleNote()
{
    QStringList logText;
    logText << tr("Расходная накладная №%1 распроведена").arg(doc_id);
    QString reason = ui->textEditRevertReason->toPlainText();
    if(reason.length())
        logText << tr("основание: ", "основание распроведения РН") + reason;

    return logText.join(", ");
}

void tabSale::beginCommit()
{
    prepareDocModel();
}

bool tabSale::skip(const int stage)
{
    switch(m_opType)
    {
        case Sale:
        case Reserve: return ((stage == 0) && (ui->checkBoxAnonymous->isChecked() || !clientModel->isNew()));
        case SaleReserved: return (stage == 0);
        case SetTrack: return (QString::compare(ui->lineEditTrack->text(), docModel->trackingNumber()) == 0);
    }

    return 0;
}

void tabSale::commit(const int stage)
{
    int stg = stage;
    switch(m_opType)
    {
        case Unsale: commitUnsale(); stg = -1; break;   // -1 чтобы пропустить следующий switch
        case CancelReserve: commitCancelReserve(); stg = -1; break;
        case SetTrack: commitTrack(); return;
        default: ;
    }

    switch(stg)
    {
        case 0: commitClient(); return;
        case 1: commitNew(); break;
        default: ;
    }

    commitLogs();
}

void tabSale::commitNew()
{
    double amount = tableModel->amountTotal();
    int paymentAccount = targetPaymentAccount();
    bool isAnonymous = ui->checkBoxAnonymous->isChecked();

    if(paymentAccount == -2)
        if(!clientModel->balanceEnough(-amount))
            Global::throwError(Global::ThrowType::UserCanceled);

    if(m_opType == Sale || m_opType == Reserve)
    {
        docModel->commit();
        doc_id = docModel->id();
        tableModel->setDocumentId(doc_id);
    }

    if(m_opType == Reserve)
    {
        tableModel->commit(StoreSaleModel::OpType::Reserve);
    }
    else
    {
        tableModel->commit(StoreSaleModel::OpType::Sale);

        if(paymentAccount == -2)   // если продажа в долг
        {
            clientModel->updateBalance(-amount, QString("Списание %1 за товары по РН №%2").arg(sysLocale.toCurrencyString(amount)).arg(doc_id), SBalanceLogRecordModel::RoyaltyReason::Document, doc_id);
        }
        else
        {
            prepareCashRegisterModel(amount, paymentAccount);
            cashRegister->commit(amount);
            docModel->set_cashOrder(cashRegister->id());
        }

        if(!isAnonymous)
            clientModel->updatePurchases(tableModel->itemsAffected());
    }
    docModel->commit();

#ifdef QT_DEBUG
//    Global::throwDebug();
#endif
}

void tabSale::throwHandler(int)
{
    clientModel->setFieldsFailed();     // в случаях сбоев при создании нового клиента или изменении баланса
    cashRegister->setFieldsFailed();
    switch(m_opType)
    {
        case Sale:
        case Reserve:
        case SaleReserved:
        case Unsale:
        case CancelReserve: {
            doc_id = 0;
            docModel->setFieldsFailed();
            docModel->setData(SDocumentModel::C_cashOrder, QVariant());
            break;
        }
    }
}

void tabSale::endCommit(const int stage)
{
    switch(m_opType)
    {
        case SaleReserved:
        case Unsale:
        case CancelReserve: return; // при продаже ранее зарезервированного товара, распроведении РН или отмене резерва дополнительных действий не требуется
        case SetTrack: new shortlivedNotification(this, "Успешно", "ТТН задана", QColor(214,239,220), QColor(229,245,234)); return;
        default: ;
    }

    switch(stage)   // Создание нового клиента может быть только при продаже или резервировании товара
    {
        case 0: fillClientCreds(clientModel->id()); break;
        default: ;
    }
}

void tabSale::endCommit()
{
    switch(m_opType)
    {
        case SetTrack: return;  // вызывать updateWidgets() не требуется
        default: ;
    }

    if(!ui->checkBoxAnonymous->isChecked())
        clientModel->SSingleRowModelBase::load();

    print();

    switch(m_endCommitOp)
    {
        case SwitchToViewMode: switchTabToViewMode(); break;
        case PrepareRepeat: prepareForRepeatedOp(); break;
    }

    updateWidgets();
}

void tabSale::prepareForRepeatedOp()
{
    doc_id = 0;
    m_opType = Sale;
    new shortlivedNotification(this, "Успешно", "Расходная накладная проведена", QColor(214,239,220), QColor(229,245,234));
    docModel->clearEverything();
    cashRegister->clearEverything();
}

void tabSale::switchTabToViewMode()
{
    docModel->SSingleRowModelBase::load();
    cashRegister->SSingleRowModelBase::load();
    updateTabPtr(0, doc_id);
}

void tabSale::saleButtonClicked()
{
    m_endCommitOp = SwitchToViewMode;

    manualSubmit();
}

/* На вкладке продажа задумка с кнопками "Продать ещё" и "Продать" несколько другая, чем аналогичные кнопки
 * на вкладке приёма в ремонт: при нажатии "Продать" производится продажа, вкладка остаётся открытой (это для
 * того, чтобы пользователь мог сначала набросать список товаров, а потом собрать товары и выдать покупателю);
 * нажатие "Продать ещё" приведёт к проводке накладной и очистке списка товаров и данных клиента (этот режим
 * будет удобен при большом кол-ве продаж — не придётся вызывать вкладку продажи вручную).
 */
void tabSale::saleMoreButtonClicked()
{
    m_endCommitOp = PrepareRepeat;

    manualSubmit();
}

/* При нажатии "Резерв" вызывается тот же метод, что и при продаже; для изменения набора запросов к БД используется
 * флаг m_opType.
 */
void tabSale::reserveButtonClicked()
{
    m_opType = Reserve;
    m_endCommitOp = SwitchToViewMode;

    manualSubmit();
}

void tabSale::addItemButtonClicked()
{
    emit createTabSelectItem(1, this);
}

void tabSale::paramsButtonClicked()
{

}

void tabSale::updateChargeAmount(QString text)
{
    // TODO: применить стиль по умолчанию, если поле было красным
    double takeInSumm = sysLocale.toDouble(text);
    double amount = sysLocale.toDouble(ui->lineEditTotal->text());
    if(amount <= takeInSumm )
        ui->lineEditCharge->setText(sysLocale.toString(takeInSumm - amount, 'f', 2));
    else
        ui->lineEditCharge->setText(sysLocale.toString(0.00, 'f', 2));

}

void tabSale::comboBoxIndexChanged(int)
{
}

void tabSale::phoneTypeChanged(int index)
{
    ui->lineEditClientPhone->setInputMask("");
    ui->lineEditClientPhone->setInputMask(clientPhoneTypesModel->index(index, 2).data().toString() + ";_");  // Here ";_" for filling blank characters with underscore
}

void tabSale::commitTrack()
{
    docModel->set_trackingNumber(ui->lineEditTrack->text());
    docModel->commit();
}

void tabSale::commitLogs()
{
    auto log = std::make_unique<SLogRecordModel>();
    log->set_type(SLogRecordModel::Doc);
    log->set_document(docModel->id());
    switch (m_opType)
    {
        case Sale: log->commit(QString("Расходная накладная №%1 создана").arg(doc_id)); break;
        case Reserve: log->commit(QString("Расходная накладная №%1 создана (резерв)").arg(doc_id)); break;
        case SaleReserved: log->commit(QString("Расходная накладная №%1 проведена").arg(doc_id)); break;
        case Unsale: if(docModel->state() == SDocumentModel::OutInvoiceCancelled) log->commit(prepareUnsaleNote()); break;
        case CancelReserve: if(docModel->state() == SDocumentModel::ReserveCancelled) log->commit(tr("Резерв товара по РН №%1 снят").arg(doc_id)); break;
        default: break;
    }
}

void tabSale::setTrackNum()
{
    m_opType = SetTrack;
    manualSubmit();
}

void tabSale::setTrackNum(int)
{
    setTrackNum();
}

void tabSale::reserveCancelButtonClicked()
{
    if(docModel->state() != SDocumentModel::ItemsReserved)   // проверка на всякий случай, вдруг накосячу с отображением кнопок в разных режимах
        return;

    m_opType = CancelReserve;
    manualSubmit();
}

void tabSale::commitCancelReserve()
{

    tableModel->backOutItems(StoreSaleModel::OpType::FreeReserved);
    docModel->commit();

#ifdef QT_DEBUG
//    Global::throwDebug();
#endif
}

void tabSale::printButtonClicked()
{

}

void tabSale::logButtonClicked()
{

}

void tabSale::unSaleButtonClicked()
{
    if(docModel->state() != SDocumentModel::OutInvoicePayed)   // проверка на всякий случай, вдруг накосячу с отображением кнопок в разных режимах
        return;

    QMessageBox resBtn( QMessageBox::Question, "SNAP CRM",
                        tr("Распровести расходную накладную и вернуть средства покупателю?"),
                        QMessageBox::No | QMessageBox::Yes);
    QAbstractButton *noButton = resBtn.button(QMessageBox::No);
    resBtn.setDefaultButton(QMessageBox::No);
    resBtn.setWindowModality(Qt::ApplicationModal);
    resBtn.exec();
    if (resBtn.clickedButton() == noButton)
        return;

    m_opType = Unsale;
    manualSubmit();
}

void tabSale::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize());

    ui->lineEditClientLastName->setFont(font);
    ui->lineEditClientFirstName->setFont(font);
    ui->lineEditClientPatronymic->setFont(font);
    ui->lineEditClientPhone->setFont(font);
    ui->lineEditTotal->setFont(font);
    ui->lineEditTakeIn->setFont(font);
    ui->comboBoxClientPhoneType->setFont(font);
    ui->comboBoxClientAdType->setFont(font);
    ui->comboBoxCompany->setFont(font);
    ui->comboBoxPaymentAccount->setFont(font);
    ui->comboBoxMoneyBackAccount->setFont(font);
    ui->comboBoxPriceCol->setFont(font);
}

tabSale* tabSale::getInstance(int doc_id, MainWindow *parent)
{
    if( !p_instance.contains(doc_id) )
      p_instance.insert(doc_id, new tabSale(doc_id, parent));
    return p_instance.value(doc_id);
}

#ifdef QT_DEBUG
void tabSale::randomFill()
{
    int i;

    if (test_scheduler_counter == 1)   // клиент
    {
        int val = QRandomGenerator::global()->bounded(100);
        if (val > 66)  // или выбираем из уже имеющихся клиентов или создаём нового или включаем галочку анонимный
        {
//            fillClientCreds(143);
            int id = SClientInputFormBase::randomClientIdFromDB();
            fillClientCreds(id);
        }
        else if (val > 33)    // ввод данных из тестового списка
        {
            i = QRandomGenerator::global()->bounded(clients4Test->size());
            ui->lineEditClientLastName->setText(clients4Test->value(i)->at(0));
            ui->lineEditClientFirstName->setText(clients4Test->value(i)->at(1));
            ui->lineEditClientPatronymic->setText(clients4Test->value(i)->at(2));
            if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 или телефон мобильный или городской (это для проверки масок)
            {
                ui->comboBoxClientPhoneType->setCurrentIndex(1);
                ui->lineEditClientPhone->setText(clients4Test->value(i)->at(4));
            }
            else
            {
                ui->comboBoxClientPhoneType->setCurrentIndex(0);
                ui->lineEditClientPhone->setText(clients4Test->value(i)->at(3));
            }

            i = QRandomGenerator::global()->bounded(clientAdTypesList->rowCount()); // если клиент новый, то случайно выбираем источник обращения
            ui->comboBoxClientAdType->setCurrentIndex(i);
        }
        else
            ui->checkBoxAnonymous->setChecked(true);
    }
    else if (test_scheduler_counter == 2)   // товары
    {
        tableModel->dbgAddRandomItem();
    }
    else if (test_scheduler_counter == 3)   // продажа в долг или с оплатой
    {
    {
        if(ui->checkBoxSaleInCredit->isVisible())
            if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50
                ui->checkBoxSaleInCredit->setChecked(true);
            }
    }
    else if (test_scheduler_counter == 4)   // поле полученной суммы
    {
        ui->lineEditTakeIn->setText(sysLocale.toString(sysLocale.toDouble(ui->lineEditTotal->text()) + QRandomGenerator::global()->bounded(100), 'f', 2));
    }
    else if (test_scheduler_counter == 5)  // платёжная система
    {
        ui->comboBoxPaymentAccount->setCurrentIndex(QRandomGenerator::global()->bounded(paymentSystemsModel->rowCount()));
    }
    else if (test_scheduler_counter == 6)   // тип цен
    {
        ui->comboBoxPriceCol->setCurrentIndex(QRandomGenerator::global()->bounded(priceColModel->rowCount()));
    }
    else if (test_scheduler_counter == 11)
    {
//        saleButtonClicked();
//        main_window_test_scheduler2->start(500);    // запускаем таймер закрытия вкладки приёма в ремонт
    }
    if (test_scheduler_counter < 11)
    {
        test_scheduler_counter++;
        test_scheduler->start(400);    //  (пере-)запускаем таймер
    }
//    qDebug() << "test_scheduler_handler(), test_scheduler_counter = " << test_scheduler_counter++;

}

void tabSale::createTestPanel()
{
    testPanel = new QWidget(this, Qt::SplashScreen);
    testPanel->setGeometry(0,0,190, 60);
    testLineEdit = new QLineEdit(testPanel);
    testLineEdit->setGeometry(5,5, 90, 20);
    testPushButton = new QPushButton(testPanel);
    testPushButton->setGeometry(5,30, 90, 20);
    testPushButton->setText("updateWidgets()");
//    connect(testPushButton, &QPushButton::clicked, this, &tabSale::test_updateWidgetsWithDocNum);
    connect(testPushButton, SIGNAL(clicked()), this, SLOT(test_updateWidgetsWithDocNum()));
    testBtnAddRandomItem = new QPushButton(testPanel);
    testBtnAddRandomItem->setGeometry(95,30, 90, 20);
    testBtnAddRandomItem->setText("AddRandomItem");
#if defined QT_DEBUG || defined QT_TEST
    connect(testBtnAddRandomItem, &QPushButton::clicked, tableModel, &StoreSaleModel::dbgAddRandomItem);
#endif
    testPanel->show();
}

void tabSale::test_scheduler_handler()  //
{
    if(tableModel->state() == 0)
        randomFill();
}

void tabSale::test_scheduler2_handler()  // обработик таймера закрытия вкладки
{
//    closeTab(ui->tabWidget->indexOf(tabClients::getInstance(0)));
    //    test_scheduler->start(1000);    //  перезапускаем таймер открытия вкладки
}

void tabSale::test_updateWidgetsWithDocNum()
{
    p_instance.remove(doc_id);   // заменить указатель
    doc_id = testLineEdit->text().toInt();
    p_instance.insert(doc_id, this);    // иначе будет падать при попытке создать новую вкладку продажи
    updateWidgets();
}

#endif
