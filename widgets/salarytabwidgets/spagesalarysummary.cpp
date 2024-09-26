#include "spagesalarysummary.h"
#include "ui_spagesalarysummary.h"

SPageSalarySummary::SPageSalarySummary(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalarySummary)
{
    ui->setupUi(this);
    SPageSalarySummary::guiFontChanged();

    ui->textEditDisclaimer->setVisible(ui->toolButtonDisclaimer->isChecked());
    connect(parentTab, &tabSalary::showSubsistanceGroup, this, &SPageSalarySummary::setGroupBoxSubsistanceVisible);
    connect(parentTab, &tabSalary::setFillMonthChargeOnUpdate, this, &SPageSalarySummary::setFillMonthChargeOnUpdate);
    connect(parentTab, &tabSalary::fwdFillClientCreds, this, &SPageSalarySummary::fillClientCreds);
    ui->toolButtonApplySummaryMonthCharge->resize(ui->doubleSpinBoxSummaryMonthCharge->height(), ui->doubleSpinBoxSummaryMonthCharge->height());
    ui->dateEditSubsistenceDate->setDate(QDate::currentDate());
    ui->dateEditSalaryDate->setDate(QDate::currentDate());
    ui->doubleSpinBoxEarning->setMinimum(-ui->doubleSpinBoxEarning->maximum());
    ui->doubleSpinBoxPayed->setMinimum(-ui->doubleSpinBoxPayed->maximum());
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
    ui->pushButtonPaySubsistence->setEnabled(permissions->createCashRegisters);
    ui->pushButtonPaySubsistence->setEnabled(permissions->createCashRegisters && permissions->paySubsistence);

    connect(ui->toolButtonApplySummaryMonthCharge, &QToolButton::clicked, this, &SPageSalarySummary::setMonthCharge);
    connect(ui->pushButtonPaySubsistence, &QPushButton::clicked, this, &SPageSalarySummary::paySubsistence);
    connect(ui->pushButtonPaySalary, &QPushButton::clicked, this, &SPageSalarySummary::paySalary);
}

SPageSalarySummary::~SPageSalarySummary()
{
    delete ui;
}

int SPageSalarySummary::createUserClientCardMsgBox()
{
    if(!parentTab->m_userModel->clientUserId())
    {
        auto btnSel = new QPushButton(tr("Выбрать"));
        auto msgBox = new QMessageBox(this);
        msgBox->setWindowTitle(tr("Баланс сотрудника не включен"));
        msgBox->setText(tr("Для зачисления разницы на баланс сотрудника необходимо указать или создать карточку клиента-сотрудника. Создать автоматически?"));
        auto btnY = msgBox->addButton(QMessageBox::Yes);
        msgBox->addButton(QMessageBox::No);
        msgBox->addButton(btnSel, QMessageBox::ActionRole);
        auto btnC = msgBox->addButton(QMessageBox::Cancel);
        msgBox->exec();
        auto clickedButton = msgBox->clickedButton();
        if (clickedButton == btnC)
        {
            return CancelOperation;
        }
        else if(clickedButton == btnY)
        {
            if(!permissions->editGlobalSettings)  // Изменять служебные настройки
            {
                shortlivedNotification *newPopup = new shortlivedNotification(this,
                                                                              tr("Информация"),
                                                                              tr("У вас недостаточно прав. Обратитесь к администратору."),
                                                                              QColor(255,164,119),
                                                                              QColor(255,199,173));
                return CancelOperation;
            }
            return CreateAutomaticaly;
        }
        else if(clickedButton == btnSel)
        {
            return SelectExist;
        }
        else    // btnN
        {
            shortlivedNotification *newPopup = new shortlivedNotification(this,
                                                                          tr("Информация"),
                                                                          tr("Сумма выплаты не соответствует значению в поле \"Итого к оплате\""),
                                                                          QColor(255,164,119),
                                                                          QColor(255,199,173));
            return DontCreate;
        }
    }

    return ClientExists;
}

void SPageSalarySummary::createUserClientCard()
{
    m_userClient = new SClientModel(0, this);
    SUserModel *user = parentTab->m_userModel;

    m_userClient->setFirstName(user->name());
    m_userClient->setLastName(user->surname());
    m_userClient->setPatronymicName(user->patronymic());
    m_userClient->appendLogText(tr("Быстрое создание клиента при выплате заработной платы"));
    m_userClient->setEmployeeId(user->id());
    m_userClient->setAdTypeIndex(-1);
    parentTab->m_userModel->setClientModel(m_userClient);
    addOldBalanceValue();
    m_commitUserClientModelsPending = 1;
}

void SPageSalarySummary::setFillMonthChargeOnUpdate(const bool state)
{
    m_fillMonthChargeOnUpdate = state;
}

void SPageSalarySummary::setDbRecordModelsData(const int type, const int system, const double amount, const QString &reason, const QDate date)
{
    int employee = parentTab->m_userModel->id();
    int user = userDbData->id;
    QDateTime periodBegin = parentTab->m_periodBegin;
    QDateTime periodEnd = parentTab->m_periodEnd;
    QDateTime created = QDateTime::currentDateTime();

    created.setDate(date);  //  в случае проводки "задним числом" изменяем дату

    // в АСЦ период за который была сделана выплата не учитывает часовой пояс и считается с 00:00:00 до 23:59:59
    periodBegin.setTimeZone(QTimeZone::utc());
    periodEnd.toUTC();
    periodEnd = periodEnd.addSecs(-1);

    salaryModel->setType(type);
    salaryModel->setSumm(amount);
    salaryModel->setNotes(reason);
    salaryModel->setPaymentDate(created);
    salaryModel->setPeriodFrom(periodBegin);
    salaryModel->setPeriodTo(periodEnd);
    salaryModel->setEmployee(employee);
    salaryModel->setUser(user);
    salaryModel->setBalance(0);

    if(type == SSalaryModel::Salary)
        cashRegister->setOperationType(SCashRegisterModel::ExpSalary);
    else
        cashRegister->setOperationType(SCashRegisterModel::ExpSubsist);
    cashRegister->setCompany(userDbData->company);
    cashRegister->setSystemId(system);
    cashRegister->setAmount(-amount);
    cashRegister->setReason(reason);
    cashRegister->setCreated(created);
    cashRegister->setEmployee(employee);
    cashRegister->setUser(user);
    //    cashRegister->setSkipLogRecording(true);
}

void SPageSalarySummary::addOldBalanceValue()
{
    auto msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(tr("Баланс сотрудника"));
    msgBox->setText(tr("Записать старое значение баланса в новом формате?\n"\
                       "Сумма будет прибавлена или вычтена если была выбрана карточка существующего клиента (не рекомендуется, см. справку)."));
    msgBox->addButton(QMessageBox::Yes);
    auto btnN = msgBox->addButton(QMessageBox::No);
    msgBox->setDefaultButton(btnN);

    msgBox->exec();
    auto clickedButton = msgBox->clickedButton();
    if (clickedButton == btnN)
    {
        return;
    }

    QSqlQuery query(QSqlDatabase::database("connMain"));
    query.exec(QString(
               "SELECT `balance` FROM salary WHERE `user_id` = %1 AND `type` = 0 ORDER BY `period_from` DESC LIMIT 1")
               .arg(parentTab->m_userModel->id()));
    query.first();
    m_employeeBalanceToConvert = query.value(0).toDouble();
}

void SPageSalarySummary::markRepairsPayed()
{
    try
    {
        QDateTime periodEnd = parentTab->m_periodEnd;
        periodEnd = periodEnd.addSecs(-1);

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
            salaryRepairsModel->setSumm(parentTab->m_repairs->unformattedData(i, STableSalaryRepairsModel::Columns::EmployeeSalaryWorks).toDouble() + parentTab->m_repairs->unformattedData(i, STableSalaryRepairsModel::Columns::EmployeeSalaryParts).toDouble());
            salaryRepairsModel->setAccountingDate(periodEnd);
            salaryRepairsModel->commit();

            delete salaryRepairsModel;
        }
    }
    catch(Global::ThrowType)
    {
        delete salaryRepairsModel;
        throw Global::ThrowType::QueryError;
    }

}

void SPageSalarySummary::paySubsistence()
{
    if(!ui->checkBoxSubsistenceOK->isChecked())
        return;

    double currentAmount = ui->doubleSpinBoxSubsistenceSumm->value();

    salaryModel = new SSalaryModel();
    cashRegister = new SCashRegisterModel();

    setDbRecordModelsData(SSalaryModel::Subsistence,
                          paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxSubsistencePaymentSystem->currentIndex(), "system_id"),
                          currentAmount,
                          ui->textEditSubsistenceReason->toPlainText(),
                          ui->dateEditSubsistenceDate->date());

    pay();
    parentTab->updateModels();
}

void SPageSalarySummary::paySalary()
{
    if(!ui->checkBoxSalaryOK->isChecked())
        return;

    double currentAmount = ui->doubleSpinBoxSalarySumm->value();
    if(currentAmount != m_earningSinceLastPay)
    {
        int tmp = createUserClientCardMsgBox();
        switch(tmp)
        {
            case ClientExists: m_userClient = parentTab->m_userModel->clientModel(); break;
            case CreateAutomaticaly: createUserClientCard(); break;
            case SelectExist: parentTab->createTabSelectExistingClient(); return;   // после создания вкладки выбора сотрудника-клиента прерываем операцию выдачи ЗП; она будет продолжена в методе fillClientCreds()
            case DontCreate:
            case CancelOperation: return;
            default: ;
        }
    }

    QString reason = tr("Выплата заработной платы за период с %1 по %2").arg(parentTab->m_periodBegin.date().toString("dd.MM.yyyy"), parentTab->m_periodEnd.addDays(-1).toString("dd.MM.yyyy"));
    salaryModel = new SSalaryModel();
    cashRegister = new SCashRegisterModel();

    setDbRecordModelsData(SSalaryModel::Salary,
                          paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxSalaryPaymentSystem->currentIndex(), "system_id"),
                          currentAmount,
                          reason,
                          ui->dateEditSalaryDate->date());

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

        if(m_commitUserClientModelsPending)
        {
            // TODO: исправить чехарду с несколькими BEGIN/COMMIT
            QUERY_EXEC(query,nErr)(QUERY_BEGIN);
            m_userClient->commit();
            parentTab->m_userModel->setClientUserId(m_userClient->id());
            parentTab->m_userModel->commit();
            QUERY_COMMIT_ROLLBACK(query,nErr);
            shortlivedNotification *newPopup = new shortlivedNotification(this, parentTab->tabTitle(), tr("Карточка сотрудника-клиента успешно создана или связана"), QColor(214,239,220), QColor(229,245,234));
            m_userClient->setBalanceEnabled();  // TODO: изменить методы в классе SClientModel, связанные с включением, выключением и пополнением баланса; произвести соотв. изменения в этом классе.
            if(m_employeeBalanceToConvert)
            {
                QUERY_EXEC(query,nErr)(QUERY_BEGIN);
                m_userClient->updateBalance(m_employeeBalanceToConvert, tr("Преобразование данных о сумме на балансе сотрудника из формата программы АСЦ"));
                QUERY_COMMIT_ROLLBACK(query,nErr);
            }

            m_commitUserClientModelsPending = 0;
        }

        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        salaryModel->commit();
        cashRegister->commit();
        if(salaryModel->type() == SSalaryModel::Salary)
        {
            markRepairsPayed();

            double amount = cashRegister->amountAbs();
            if(amount != m_earningSinceLastPay && m_userClient)
            {
                m_userClient->setBalanceEnabled();  // TODO: изменить методы в классе SClientModel, связанные с включением, выключением и пополнением баланса; произвести соотв. изменения в этом классе.
                QString reason;
                if(amount > m_earningSinceLastPay)
                    reason = tr("Списание средств с баланса сотрудника-клиента при выплате заработной платы");
                else
                    reason = tr("Зачисление разницы заработка и выплаченной суммы на баланс сотрудника-клиента");
                m_userClient->updateBalance(m_earningSinceLastPay - amount, reason);
                salaryModel->setBalanceRecord(m_userClient->balanceObj()->id());
                salaryModel->commit();
            }
        }


#ifdef QT_DEBUG
//        throw Global::ThrowType::Debug; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;

        shortlivedNotification *newPopup = new shortlivedNotification(this, parentTab->tabTitle(), tr("Ошибка выполнения запроса"), QColor(255,164,119), QColor(255,199,173));
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
//            nErr = 1; // это чтобы проверить работу дальше
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }

    m_queryLog->stop();
    if(nErr)
        shortlivedNotification *newPopup = new shortlivedNotification(this, parentTab->tabTitle(), tr("Проведено"), QColor(214,239,220), QColor(229,245,234));

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
    double earningWorksAndParts = parentTab->m_repairs->total(STableSalaryRepairsModel::Columns::EmployeeSalaryWorks, STableSalaryRepairsModel::IncludePayed) + parentTab->m_repairs->total(STableSalaryRepairsModel::Columns::EmployeeSalaryParts, STableSalaryRepairsModel::IncludePayed);   // в АСЦ в поле "Заработок с ремонтов" только сумма за работы; сумма за детали в ремонтах прибавляется к сумме за проданные и отображается в поле "Заработок с продаж"
    double earningReceptedIssued = parentTab->m_recepted->total(STableSalaryReceptedIssued::Columns::PayDeviceIn) + parentTab->m_issued->total(STableSalaryReceptedIssued::Columns::PayDeviceOut);
    double earningSales = parentTab->m_sales->total(STableSalarySalesModel::Columns::SalaryPart);
    double extraCharges = parentTab->m_extraCharges->total(STableSalaryExtraModel::Columns::Price, 1);
    double extraChargesOff = parentTab->m_extraCharges->total(STableSalaryExtraModel::Columns::Price, -1);
    double earning = 0;
    double paymentsSubsistanceSumm = parentTab->m_payments->total(STableSalaryPaymentsModel::Subsistance);
    double paymentsSalarySumm = parentTab->m_payments->total(STableSalaryPaymentsModel::Salary);
    double employeeBalance = parentTab->m_userModel->balance();
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
    m_earningSinceLastPay = earning - paymentsSubsistanceSumm - paymentsSalarySumm;
    m_earningSinceLastPay = round(m_earningSinceLastPay*100)/100;
    ui->doubleSpinBoxEarning->setValue(earning);
    ui->doubleSpinBoxSubsistence->setValue(paymentsSubsistanceSumm);
    ui->doubleSpinBoxPayed->setValue(paymentsSalarySumm);
    ui->doubleSpinBoxBalance->setValue(employeeBalance);
    ui->doubleSpinBoxSummaryAmountToPay->setValue(employeeBalance + earning - paymentsSubsistanceSumm - paymentsSalarySumm);
    ui->doubleSpinBoxSalarySumm->setValue(employeeBalance + earning - paymentsSubsistanceSumm - paymentsSalarySumm);
    ui->checkBoxSubsistenceOK->setChecked(false);
    ui->checkBoxSalaryOK->setChecked(false);
#ifdef QT_DEBUG
    ui->checkBoxSalaryOK->setChecked(true);
    ui->doubleSpinBoxSalarySumm->setValue(1000);
#endif
}

void SPageSalarySummary::fillClientCreds(int id)
{
    qDebug().nospace() << "[" << this << "] fillClientCreds() | id = " << id;
    parentTab->m_userModel->setClientUserId(id);
    m_userClient = new SClientModel(id, this);
    m_userClient->setEmployeeId(parentTab->m_userModel->id());
    parentTab->m_userModel->setClientModel(m_userClient);
    addOldBalanceValue();
    m_commitUserClientModelsPending = 1;
    paySalary();
}

void SPageSalarySummary::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize);
    font.setKerning(true);

    ui->doubleSpinBoxEarning->setFont(font);
    ui->doubleSpinBoxSubsistence->setFont(font);
    ui->doubleSpinBoxPayed->setFont(font);
    ui->doubleSpinBoxSummaryAmountToPay->setFont(font);
    ui->doubleSpinBoxExtraCharges->setFont(font);
    ui->doubleSpinBoxExtraChargesOff->setFont(font);
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
