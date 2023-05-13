#include "spagesalarysummary.h"
#include "ui_spagesalarysummary.h"

SPageSalarySummary::SPageSalarySummary(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalarySummary)
{
    ui->setupUi(this);
    ui->textEditDisclaimer->setVisible(ui->toolButtonDisclaimer->isChecked());
    connect(parentTab, &tabSalary::showSubsistanceGroup, this, &SPageSalarySummary::setGroupBoxSubsistanceVisible);
    connect(parentTab, &tabSalary::setFillMonthChargeOnUpdate, this, &SPageSalarySummary::setFillMonthChargeOnUpdate);
    ui->toolButtonApplySummaryMonthCharge->resize(ui->doubleSpinBoxSummaryMonthCharge->height(), ui->doubleSpinBoxSummaryMonthCharge->height());
    ui->dateEditSubsistenceDate->setDate(QDate::currentDate());
    ui->dateEditSalaryDate->setDate(QDate::currentDate());
    ui->doubleSpinBoxBalance->setMinimum(-ui->doubleSpinBoxBalance->maximum());
    ui->doubleSpinBoxSummaryAmountToPay->setMinimum(-ui->doubleSpinBoxSummaryAmountToPay->maximum());

    paymentSystemsProxyModel = new SSortFilterProxyModel();
    paymentSystemsProxyModel->setSourceModel(paymentSystemsModel);
    paymentSystemsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(-2)|(-1)).*$"));
    paymentSystemsProxyModel->setFilterKeyColumn(1);
    ui->comboBoxSalaryPaymentSystem->setModel(paymentSystemsProxyModel);
    ui->comboBoxSalaryPaymentSystem->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(0, "system_id"));
    ui->comboBoxSubsistencePaymentSystem->setModel(paymentSystemsProxyModel);
    ui->comboBoxSubsistencePaymentSystem->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(0, "system_id"));

    connect(ui->toolButtonApplySummaryMonthCharge, &QToolButton::clicked, this, &SPageSalarySummary::setMonthCharge);
    connect(ui->pushButtonPaySubsistence, &QPushButton::clicked, this, &SPageSalarySummary::paySubsistence);
    connect(ui->pushButtonPaySalary, &QPushButton::clicked, this, &SPageSalarySummary::paySalary);
}

SPageSalarySummary::~SPageSalarySummary()
{
    delete ui;
}

void SPageSalarySummary::setFillMonthChargeOnUpdate(const bool state)
{
    m_fillMonthChargeOnUpdate = state;
}

void SPageSalarySummary::setDbRecordModelsData()
{
    QDateTime periodBegin = parentTab->m_periodBegin;
    QDateTime periodEnd = parentTab->m_periodEnd;

    periodBegin.setTimeZone(QTimeZone::utc());  // в АСЦ период за который была сдалеана выплата не учитывает часовой пояс и считается с 00:00:00 до 23:59:59
    periodEnd.setTimeZone(QTimeZone::utc());
    periodEnd = periodEnd.addSecs(-1);

    salaryModel->setPaymentDate(QDateTime::currentDateTime());
    salaryModel->setEmployee(parentTab->m_userModel->id());
    salaryModel->setUser(userDbData->value("id").toInt());
    salaryModel->setPeriodFrom(periodBegin);
    salaryModel->setPeriodTo(periodEnd);

    cashRegister->setEmployee(parentTab->m_userModel->id());
    cashRegister->setUser(userDbData->value("id").toInt());
//    cashRegister->setSkipLogRecording(true);
}

void SPageSalarySummary::markRepairsPayed()
{
    try
    {
        for(int i = 0; i < parentTab->m_repairs->rowCount(); i++)
        {
            if(parentTab->m_repairs->index(i, 10).data().toInt())   // пропуск ранее оплаченных, если они отображаются
                continue;

            salaryRepairsModel = new SSalaryRepairsModel();
//            if(parentTab->m_repairs->index(i, 13).data().toInt())
//                salaryRepairsModel->setCartridge();
//            else
                salaryRepairsModel->setRepair(parentTab->m_repairs->id(i));
            salaryRepairsModel->setUser(parentTab->m_userModel->id());
            salaryRepairsModel->setSumm(parentTab->m_repairs->unformattedData(i, 6).toDouble() + parentTab->m_repairs->unformattedData(i, 7).toDouble());
            salaryRepairsModel->commit();

            delete salaryRepairsModel;
        }
    }
    catch(int)
    {
        delete salaryRepairsModel;
        throw 1;
    }

}

void SPageSalarySummary::paySubsistence()
{
    if(!ui->checkBoxSubsistenceOK->isChecked())
        return;

    double amount = ui->doubleSpinBoxSalarySumm->value();
    QString reason = ui->textEditSubsistenceReason->toPlainText();
    salaryModel = new SSalaryModel();
    cashRegister = new SCashRegisterModel();

    salaryModel->setType(SSalaryModel::Subsistence);
    salaryModel->setNotes(reason);
    salaryModel->setSumm(amount);
    salaryModel->setBalance(parentTab->m_userModel->balance());
    cashRegister->setAmount(-amount);
    cashRegister->setReason(reason);
    cashRegister->setSystemId(paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxSubsistencePaymentSystem->currentIndex(), "system_id"));
    cashRegister->setOperationType(SCashRegisterModel::ExpSubsist);
    setDbRecordModelsData();

    pay();
}

void SPageSalarySummary::paySalary()
{
    if(!ui->checkBoxSalaryOK->isChecked())
        return;

    double amount = ui->doubleSpinBoxSalarySumm->value();
    if(amount != ui->doubleSpinBoxSummaryAmountToPay->value())
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Сумма выплаты не соответствует значению в поле \"Итого к оплате\".\r\n"\
                               "В данной версии работа с балансом сотрудника не реализована"));
        msgBox.exec();
        return;
    }

    QString reason = tr("Выплата заработной платы за период с %1 по %2").arg(parentTab->m_periodBegin.date().toString("dd.MM.yyyy"), parentTab->m_periodEnd.addDays(-1).toString("dd.MM.yyyy"));
    salaryModel = new SSalaryModel();
    cashRegister = new SCashRegisterModel();

    salaryModel->setType(SSalaryModel::Salary);
    salaryModel->setNotes(reason);
    salaryModel->setSumm(amount);
//    salaryModel->setBalance(ui->doubleSpinBoxSummaryAmountToPay->value() - amount);
    salaryModel->setBalance(0);
    cashRegister->setAmount(-amount);
    cashRegister->setReason(reason);
    cashRegister->setSystemId(paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxSalaryPaymentSystem->currentIndex(), "system_id"));
    cashRegister->setOperationType(SCashRegisterModel::ExpSalary);
    setDbRecordModelsData();

    pay();
    parentTab->updateModels();
}

void SPageSalarySummary::pay()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    m_queryLog = new SQueryLog();

    try
    {
        m_queryLog->start(parentTab->metaObject()->className());
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);

        salaryModel->commit();
        if(salaryModel->type() == SSalaryModel::Salary)
            markRepairsPayed();
        cashRegister->commit();


#ifdef QT_DEBUG
//        throw 0; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (int type)
    {
        nErr = 0;
        // TODO всплывающее сообщение
        if(type == 0)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
//            nErr = 1; // это чтобы проверить работу дальше
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }

    m_queryLog->stop();
    if(nErr)
        ;   // TODO: зелёное сообщение

    delete query;
    delete m_queryLog;
    delete salaryModel;
    delete cashRegister;
}

void SPageSalarySummary::setGroupBoxSubsistanceVisible(bool visible)
{
    ui->groupBoxSubsistence->setVisible(visible);
}

void SPageSalarySummary::updateModels()
{
}

void SPageSalarySummary::updateWidgets()
{
    double monthRate = parentTab->m_userModel->salaryRate();
    double monthCharge = 0;
    double earningWorksAndParts = parentTab->m_repairs->total(6, STableSalaryRepairsModel::IncludePayed) + parentTab->m_repairs->total(7, STableSalaryRepairsModel::IncludePayed);   // в АСЦ в поле "Заработок с ремонтов" только сумма за работы; сумма за детали в ремонтах прибавляется к сумме за проданные и отображается в поле "Заработок с продаж"
    double earningReceptedIssued = parentTab->m_recepted->total(4) + parentTab->m_issued->total(4);
    double earningSales = parentTab->m_sales->total(5);
    double extraCharges = parentTab->m_extraCharges->total(2, 1);
    double extraChargesOff = parentTab->m_extraCharges->total(2, -1);
    double earning = 0;
    double paymentsSubsistanceSumm = parentTab->m_payments->total(STableSalaryPaymentsModel::Subsistance);
    double paymentsSalarySumm = parentTab->m_payments->total(STableSalaryPaymentsModel::Salary);
//    double employeeBalance = parentTab->m_userModel->balance();
    if(m_fillMonthChargeOnUpdate)
    {
        monthCharge = monthRate;
        m_fillMonthChargeOnUpdate = 0;
    }

    ui->doubleSpinBoxSummaryEarningsRepairs->setValue(earningWorksAndParts);
    ui->doubleSpinBoxSummaryMonthRate->setValue(monthRate);
    ui->doubleSpinBoxSummaryMonthCharge->setValue(monthCharge);
    ui->doubleSpinBoxSummaryEarningsSales->setValue(earningSales);
    ui->doubleSpinBoxSummaryEarningsReceiptIssue->setValue(earningReceptedIssued);
    ui->doubleSpinBoxSummaryItemsCost->setValue(parentTab->m_items->totalUnusedItemsCost());
    ui->doubleSpinBoxExtraCharges->setValue(extraCharges);
    if(extraCharges)
        ui->doubleSpinBoxExtraCharges->setStyleSheet("background-color: rgb(144,238,144);");
    else
        ui->doubleSpinBoxExtraCharges->setStyleSheet("");
    ui->doubleSpinBoxExtraChargesOff->setValue(-extraChargesOff);
    if(extraChargesOff)
        ui->doubleSpinBoxExtraChargesOff->setStyleSheet("background-color: rgb(234,73,73);");
    else
        ui->doubleSpinBoxExtraChargesOff->setStyleSheet("");
    earning += earningWorksAndParts;
    earning += earningSales;
    earning += earningReceptedIssued;
    earning += monthCharge;
    earning += extraCharges + extraChargesOff;
    ui->doubleSpinBoxEarning->setValue(earning);
    ui->doubleSpinBoxSubsistence->setValue(paymentsSubsistanceSumm);
    ui->doubleSpinBoxPayed->setValue(paymentsSalarySumm);
//    ui->doubleSpinBoxBalance->setValue(employeeBalance);
    ui->doubleSpinBoxSummaryAmountToPay->setValue(/*employeeBalance +*/ earning - paymentsSubsistanceSumm - paymentsSalarySumm);
    ui->doubleSpinBoxSalarySumm->setValue(/*employeeBalance +*/ earning - paymentsSubsistanceSumm - paymentsSalarySumm);
}

/*  Кнопка справа от поля "По ставке начислить" должна выполнять те же действия, что и кнопка "Загрузить" на нижней панели,
 *  кроме автозаполнения поля (поведение аналогично АСЦ).
*/
void SPageSalarySummary::setMonthCharge()
{
    if(parentTab->checkInput())
        return;
    parentTab->updateModels();
    updateWidgets();
}
