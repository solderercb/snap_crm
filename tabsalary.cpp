#include "tabsalary.h"
#include "ui_tabsalary.h"
#include "global.h"

tabSalary* tabSalary::p_instance = nullptr;

tabSalary::tabSalary(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabSalary)
{
    // Игициализация моделей до UI
    m_userModel = new SUserModel();
    m_repairs = new STableSalaryRepairsModel();
    m_repairWorks = new STableSalaryRepairWorksModel();
    m_repairParts = new STableSalaryRepairPartsModel();
    m_sales = new STableSalarySalesModel();
    m_saleParts = new STableSalarySalePartsModel();
    m_extraCharges = new STableSalaryExtraModel();
    m_items = new STableSalaryItemsModel();
    m_payments = new STableSalaryPaymentsModel();
    m_recepted = new STableSalaryReceptedIssued();
    m_issued = new STableSalaryReceptedIssued();

    ui->setupUi(this);
    usersModelF = new SSortFilterProxyModel();
    usersModelF->setSourceModel(usersModel);
    usersModelF->setFilterRegularExpression(QRegularExpression("^(?!(admin)).*$")); // без админа
    usersModelF->setFilterKeyColumn(0);
    ui->comboBoxEmployee->setModel(usersModelF);
    ui->comboBoxEmployee->setCurrentIndex(-1);
    tabChanged(ui->tabWidget->currentIndex());

    ui->dateEditPeriod->setDate(QDate::currentDate());
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &tabSalary::tabChanged);

    connect(ui->buttonAddExtraCharge, &QPushButton::clicked, m_extraCharges, &STableSalaryExtraModel::addNewRow);
    connect(ui->buttonSaveExtraChargesList, &QPushButton::clicked, m_extraCharges, &STableSalaryExtraModel::saveTable);
    connect(ui->toolButtonPrevPeriod, &QToolButton::clicked, this, &tabSalary::prevPeriod);
    connect(ui->toolButtonNextPeriod, &QToolButton::clicked, this, &tabSalary::nextPeriod);
    connect(ui->toolButtonLoad, &QToolButton::clicked, this, &tabSalary::loadButtonPressed);

#ifdef QT_DEBUG
    ui->comboBoxEmployee->setCurrentIndex(0);
    ui->dateEditPeriod->setDate(QDate(2022,8,01));
    loadButtonPressed();
#endif
    updateWidgets();
}

tabSalary *tabSalary::getInstance(MainWindow *parent)
{
    if( !p_instance )
      p_instance = new tabSalary(parent);
    return p_instance;
}

tabSalary::~tabSalary()
{
    delete ui;
    delete m_repairs;
    delete m_repairWorks;
    delete m_repairParts;
    delete m_sales;
    delete m_saleParts;
    delete m_extraCharges;
    delete m_items;
    delete m_payments;
    delete m_recepted;
    delete m_issued;
    delete m_userModel;
    p_instance = nullptr;
}

QString tabSalary::tabTitle()
{
    return "Заработная плата";
}

QString tabSalary::periodBegin()
{
    return m_periodBegin.toUTC().toString("yyyy-MM-dd hh:mm:ss");
}

QString tabSalary::periodEnd()
{
    return m_periodEnd.toUTC().toString("yyyy-MM-dd hh:mm:ss");
}

int tabSalary::employeeId()
{
    return m_userModel->id();
}

bool tabSalary::tabCloseRequest()
{
    if(m_extraCharges->isDirty())
    {
        auto result = QMessageBox::question(this, tr("Данные не сохранены"), tr("Список дополнительных начислений и списаний не сохранён!\nСохранить перед закрытием?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (result == QMessageBox::Cancel)
        {
            return 0;
        }
        else if (result == QMessageBox::No)
        {
            return 1;
        }
        else
        {
            ui->buttonSaveExtraChargesList->clicked();
        }
    }
    return 1;
}

void tabSalary::updateModels()
{
    m_modelUpdatedFlags = 0;
    m_userModel->setSalaryRateStartDate(m_periodBegin.date());
    m_userModel->load(usersModelF->databaseIDByRow(ui->comboBoxEmployee->currentIndex()));
    m_extraCharges->setEmployeeId(m_userModel->id());
    setModelUpdatedFlag(SPageSalaryBase::UserModel);
    emit updateDaughterTabsModels();
}

void tabSalary::modelsUpdated()
{
}

void tabSalary::loadButtonPressed()
{
    if(checkInput())
        return;
    emit setFillMonthChargeOnUpdate(1);
    updateModels();
}

void tabSalary::periodDateChanged(const QDate date)
{
    emit showSubsistanceGroup(date.month() == QDate::currentDate().month());

    if(comSettings->value("classic_salary").toBool())
    {
        m_periodBegin.setDate(date.addDays(-date.day() + 1));
        m_periodEnd = m_periodBegin.addDays(date.daysInMonth());
    }
    else
    {
        m_periodBegin.setDate(date.addDays(-date.dayOfWeek() + 1));
        m_periodEnd = m_periodBegin.addDays(7);
    }
    updateWidgets();
}

void tabSalary::tabChanged(const int index)
{
    bool state = index==2;
    setExtraChargesButtonsVisible(state);
}

void tabSalary::setExtraChargesButtonsVisible(bool state)
{
    bool stateForPeriod = m_periodBegin.date().month() == QDate::currentDate().month();
    ui->buttonAddExtraCharge->setVisible(state && stateForPeriod);
    ui->buttonSaveExtraChargesList->setVisible(state && stateForPeriod);
}

void tabSalary::createTabSelectExistingClient()
{
    emit createTabSelectExistingClient(1, this);
}

void tabSalary::fillClientCreds(int id)
{
    emit fwdFillClientCreds(id);
}

void tabSalary::prevPeriod()
{
    QDate current = ui->dateEditPeriod->date();

    if(comSettings->value("classic_salary").toBool())
    {
        current = current.addMonths(-1);
    }
    else
    {
        current = current.addDays(-7);
    }

    ui->dateEditPeriod->setDate(current);
}

void tabSalary::nextPeriod()
{
    QDate current = ui->dateEditPeriod->date();

    if(comSettings->value("classic_salary").toBool())
    {
        current = current.addMonths(1);
    }
    else
    {
        current = current.addDays(7);
    }

    ui->dateEditPeriod->setDate(current);
}

void tabSalary::updateWidgets()
{
    ui->labelPeriod->setText(m_periodBegin.toString("dd.MM.yyyy") + " − " + m_periodEnd.addDays(-1).toString("dd.MM.yyyy"));
    setExtraChargesButtonsVisible(ui->tabWidget->currentIndex() == 2);
}

void tabSalary::setModelUpdatedFlag(const int pos)
{
    m_modelUpdatedFlags |= 1<<pos;
    if(m_modelUpdatedFlags == (
        1 << SPageSalaryBase::UserModel |
        1 << SPageSalaryBase::RepairsModel |
        1 << SPageSalaryBase::ExtraChargesModel |
        1 << SPageSalaryBase::SalesModel |
        1 << SPageSalaryBase::ItemsModel |
        1 << SPageSalaryBase::PaymentsModel |
        1 << SPageSalaryBase::ReceptedModel |
        1 << SPageSalaryBase::IssuedModel))
    {
        modelsUpdated();
        emit updateDaughterTabsWidgets();
    }
}

/* Проверка данных перед загрузкой сведений о заработках сотрудника
 * Возвращает 1 в случае ошибки
*/
bool tabSalary::checkInput()
{
    if(ui->comboBoxEmployee->currentIndex() == -1)
    {
        ui->comboBoxEmployee->setStyleSheet(commonComboBoxStyleSheetRed);
        return 1;
    }
    ui->comboBoxEmployee->setStyleSheet(commonComboBoxStyleSheet);

    return 0;
}
