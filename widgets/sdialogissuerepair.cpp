#include "global.h"
#include "sdialogissuerepair.h"
#include "ui_sdialogissuerepair.h"
#include "tabrepairs.h"
#include "tabrepair.h"

SDialogIssueRepair::SDialogIssueRepair(QList<SRepairModel*> repairs, Qt::WindowFlags flags, QWidget *parent) :
    SModalWidget(parent, flags),
    ui(new Ui::SDialogIssueRepair)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    m_repairsModels = repairs;

    m_clientModel = new SClientModel();
    m_clientModel->load(repairs.at(0)->clientId());
    initPaymentSystems();
    initWidgets();
    collectRepairsData();
    updateWidgets();
    show();

    if (!m_pushButtonIssueEnabled)
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Ошибка данных. Попробуйте закрыть и снова открыть карту ремонта.", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
    }

#ifdef QT_DEBUG
//    ui->checkBoxConfirmGetOut->setChecked(true);
//    ui->checkBoxPaymentCheckout->setChecked(true);
//    if(ui->checkBoxIssueInCredit->isVisible() && m_clientModel->balanceEnabled())
//        ui->checkBoxIssueInCredit->setChecked(true);
//    if(ui->checkBoxPaymentCheckout->isVisible() && ui->checkBoxPaymentCheckout->isEnabled())
//        ui->checkBoxPaymentCheckout->setChecked(true);
//    if(ui->checkBoxConfirmGetOut->isVisible() && ui->checkBoxConfirmGetOut->isEnabled())
//        ui->checkBoxConfirmGetOut->setChecked(true);
//    buttonIssueClicked();
#endif
}

SDialogIssueRepair::SDialogIssueRepair(QList<SRepairModel *> repairs, QWidget *parent) :
    SModalWidget(parent, SModalWidget::State::Hidden)
{
    m_repairsModels = repairs;
    m_clientModel = new SClientModel();
    m_clientModel->load(repairs.at(0)->clientId());
    collectRepairsData();
}

SDialogIssueRepair::~SDialogIssueRepair()
{
    disconnect();
    deleteRepairModels();
    if(ui)
        delete ui;
}

void SDialogIssueRepair::setDefaultStyleSheets()
{
    ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
}

void SDialogIssueRepair::initWidgets()
{
    ui->doubleSpinBoxTotalAmount->setMinimum(-9999999.990000);
    setDefaultStyleSheets();

    if(m_repairsModels.first()->cartridge())    // если картридж, то
    {
        m_isCartridgeIssue = 1;
        ui->checkBoxWorksDocPrint->setText(tr("Печать чек-акта(ов)", "", m_repairsModels.size()));
    }
    else
    {
        ui->checkBoxWorksDocPrint->setText(tr("Печать акта(ов) выполненных работ", "", m_repairsModels.size()));
    }

    ui->checkBoxWorksDocPrint->setChecked(comSettings->printWorksList && !(m_clientModel->options() & (SClientModel::BalanceEnabled | SClientModel::Company | SClientModel::Regular)));
    ui->comboBoxRejectReason->setModel(rejectReasonModel);
    ui->comboBoxRejectReason->setCurrentIndex(-1);
    ui->comboBoxPaymentAccount->setModel(m_paymentSystemsProxyModel);
    ui->comboBoxPaymentAccount->setCurrentIndex(m_paymentSystemsProxyModel->rowByDatabaseID(userDbData->defaultPaymentSystem, "system_id"));
    connect(ui->comboBoxRejectReason, SIGNAL(currentTextChanged(QString)), this, SLOT(otheRejectReasonShow(QString)));
    connect(ui->textEditRejectReason, SIGNAL(textChanged()), this, SLOT(textEditTextChanged()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(buttonCancelClicked()));
    connect(ui->pushButtonIssue, SIGNAL(clicked()), this, SLOT(buttonIssueClicked()));
    connect(ui->pushButtonLooseDocPrint, SIGNAL(clicked()), this, SLOT(createLooseDoc()));
    connect(ui->comboBoxPaymentAccount, SIGNAL(currentIndexChanged(int)), this, SLOT(paymentSystemChanged(int)));
    connect(ui->checkBoxPayFromBalance, &QCheckBox::toggled, this, &SDialogIssueRepair::checkBoxInCreditToggled);
    connect(ui->checkBoxIssueInCredit, &QCheckBox::toggled, this, &SDialogIssueRepair::checkBoxIssueInCreditToggled);
}

void SDialogIssueRepair::updateWidgets()
{
    ui->checkBoxDiagDocPrint->setVisible(m_singleRepairWidgetsVisible);
    ui->pushButtonLooseDocPrint->setVisible(m_singleRepairWidgetsVisible);
    ui->comboBoxRejectReason->setVisible(m_singleRepairWidgetsVisible && m_rejectReasonWidgetsVisible);
    ui->textEditRejectReason->setVisible(false);
    ui->labelPrepay->setVisible(m_singleRepairWidgetsVisible);
    ui->doubleSpinBoxPrepay->setVisible(m_singleRepairWidgetsVisible);
    ui->labelAgreedAmount->setVisible(m_singleRepairWidgetsVisible);
    ui->doubleSpinBoxAgreedAmount->setVisible(m_singleRepairWidgetsVisible);
    ui->checkBoxPayFromBalance->setVisible(m_clientModel->balanceEnabled());
    // checkBox "В долг" отображается только если настроен переход "Готово к выдаче"->"Выдано в долг" и включен баланс клиента
    ui->checkBoxIssueInCredit->setVisible(m_clientModel->balanceEnabled() &&
                                                comSettings->repairStatuses[Global::RepStateIds::Ready].Contains.contains(Global::RepStateIds::ReturnedInCredit));
    ui->checkBoxPaymentCheckout->setVisible(m_totalAmountToPay != 0);
    ui->checkBoxConfirmGetOut->setVisible(m_totalAmountToPay == 0);
    ui->labelIssuedMessage->setVisible(m_singleRepairWidgetsVisible);
    if(m_singleRepairWidgetsVisible)
        ui->labelIssuedMessage->setText(m_repairsModels.first()->issuedMsg());

    ui->doubleSpinBoxPrepay->setValue(m_totalPrepayAmount);
    ui->doubleSpinBoxAgreedAmount->setValue(m_totalAgreedAmount);
    ui->doubleSpinBoxAlreadyPayed->setValue(m_totalPaymentsAmount);
    ui->doubleSpinBoxTotalAmount->setValue(m_totalAmount);
    ui->doubleSpinBoxCurrentPaymentAmount->setValue(m_totalAmountToPay);

    ui->pushButtonIssue->setEnabled(m_pushButtonIssueEnabled);
}

void SDialogIssueRepair::initPaymentSystems()
{
    m_paymentSystemsProxyModel = new SSortFilterProxyModel();
    m_paymentSystemsProxyModel->setSourceModel(paymentSystemsModel);
    m_paymentSystemsProxyModel->setFilterKeyColumn(1);
    if(!m_clientModel->balanceEnabled())
    {
        m_paymentSystemsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(" + QString::number(Global::PaymentSystemIds::Balance) + ")).*$"));
    }
}

/*  Проверка введённых пользователем данных и соответствия сумм
 *  Возвращает 0 если всё ОК
*/
bool SDialogIssueRepair::checkInput()
{
    int error = 0;
    QString errMsg;

    setDefaultStyleSheets();
    if(ui->comboBoxRejectReason->isVisible() && ui->comboBoxRejectReason->currentIndex() == -1)
    {
        ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheetRed);
        error |= 1 << 0;
    }
    if(ui->textEditRejectReason->isVisible() && ui->textEditRejectReason->toPlainText() == "")
    {
        ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheetRed);
        error |= 1 << 1;
    }
    if( ui->checkBoxPaymentCheckout->isVisible() && !ui->checkBoxPaymentCheckout->isChecked() && !ui->checkBoxIssueInCredit->isChecked())
    {
        errMsg = tr("Подтвердите правильность ввода данных");
        error |= 1 << 2;
    }
    if( ui->checkBoxConfirmGetOut->isVisible() && !ui->checkBoxConfirmGetOut->isChecked())
    {
        // TODO: подумать над заменой уведомления на окраску чекбокса в красный
        errMsg = tr("Подтвердите выдачу");
        error |= 1 << 3;
    }

    error |= checkAmounts() << 4;

    if(error)
        qDebug().nospace() << "[" << this << "] checkInput() | error: " << error;
    if(!errMsg.isEmpty())
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Ошибка"), errMsg, QColor("#FFC7AD"), QColor("#FFA477"));

    return error;
}

/* Проверка соответствия согласованной(-ых) суммы и суммы за работы и детали
 * Возвращает 1 если суммы не совпадают или если на балансе клиента недостаточно денег и пользователь прервал выдачу
*/
bool SDialogIssueRepair::checkAmounts()
{
    if(m_summsNotEq)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                    tr("Сумма ремонта не равна согласованной. Всё ОК?\n"),
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::No);
        if (resBtn == QMessageBox::No)
        {
            return 1;
        }
    }

    if(m_isPayFromBalance && !m_isIssuingInCredit)
    {
        return !m_clientModel->balanceEnough(-m_totalAmountToPay);
    }

    return 0;
}

void SDialogIssueRepair::collectRepairsData()
{
    SRepairModel *repairModel;
    SSaleTableModel *BOQModel;
    QList<SRepairModel*>::const_iterator i = m_repairsModels.constBegin();
    int repairId;
    double worksAndSparePartsSumm = 0;
    double prepayAmount = 0;
    double paymentsAmount = 0;
    double repairCost = 0;
    double amountToPay = 0;
    double realRepairCost = 0;
    int singleRepairWidgetsVisible = 0;

    m_rejectReasonWidgetsVisible = 0;
    m_totalPrepayAmount = 0;
    m_totalPaymentsAmount = 0;
    m_totalAgreedAmount = 0;
    m_totalAmount = 0;
    m_totalAmountToPay = 0;
    m_repairsWithPayment.clear();

    while(i != m_repairsModels.constEnd())
    {
        repairModel = (*i);
//        repairModel->reload();
        repairId = repairModel->id();
        BOQModel = repairModel->BOQModel();

        m_singleRepairWidgetsVisible = !singleRepairWidgetsVisible;
        if(repairModel->state() == Global::RepStateIds::ReadyNoRepair)
        {
            m_rejectReasonWidgetsVisible = m_singleRepairWidgetsVisible;
        }

        repairCost = repairModel->repairCost();
        realRepairCost = repairModel->realRepairCost();
        prepayAmount = repairModel->paymentsAmount(SCashRegisterModel::PaymentType::RecptPrepayRepair); // конкретно предоплаты
        paymentsAmount = repairModel->paymentsAmount(); // все поступления
        worksAndSparePartsSumm = BOQModel->amountTotal();
        amountToPay = worksAndSparePartsSumm - paymentsAmount;

        if(amountToPay)
            m_repairsWithPayment.insert(QString::number(repairId), amountToPay);

        if(!repairModel->cartridge() && repairCost != worksAndSparePartsSumm) // если сумма работ и деталей не равна согласованной, отобразится сообщение
            m_summsNotEq = 1;

        // Деньги это важно, поэтому перепроверяем суммы
        if(repairModel->prepaidSumm() != prepayAmount)
        {
            appLog->appendRecord(QString("Rechecking prepaid summ for repair %1: mismatch.").arg(repairId));
            m_pushButtonIssueEnabled &= 0;
        }

        if(worksAndSparePartsSumm != realRepairCost)
        {
            appLog->appendRecord(QString("Rechecking total amount of works and parts for repair %1: mismatch.").arg(repairId));
            m_pushButtonIssueEnabled &= 0;
        }

        m_totalAmount += worksAndSparePartsSumm;
        m_totalPrepayAmount += prepayAmount;
        m_totalPaymentsAmount += paymentsAmount;
        m_totalAgreedAmount += repairCost;
        m_totalAmountToPay += amountToPay;

        i++;
        singleRepairWidgetsVisible++;
    }
}

void SDialogIssueRepair::setRepairReady(SRepairModel *model)
{
    model->setState(Global::RepStateIds::Ready);
}

void SDialogIssueRepair::buttonIssueClicked()
{
    bool nErr = 1;

    getWidgetsValues();

    if(checkInput())
        return;

    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    QUERY_LOG_START(parent()->metaObject()->className());
    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        issueRepairs();
#ifdef QT_DEBUG
//        throw Global::ThrowType::Debug; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);

        // в случае возникновения ошибки запроса нужно обновить данные моделей
        QList<SRepairModel*>::const_iterator i = m_repairsModels.constBegin();
        SRepairModel *repairModel;
        while(i != m_repairsModels.constEnd())
        {
            repairModel = (*i);
            repairModel->reload();
            i++;
        }
        m_clientModel->load(m_repairsModels.at(0)->clientId());

        emit issueFailed();
    }
    QUERY_LOG_STOP;

    if(nErr)
    {
        if(ui->checkBoxWorksDocPrint->isChecked())
        {
            m_isListOwner = 0;  // модели не нужно удалять, т. к. производится печать отчетов
            emit printWorksLists();
        }

        emit issueSuccessfull();

        tabRepairs::refreshIfTabExists();
        this->deleteLater();
    }
}

void SDialogIssueRepair::issueRepairs()
{
    bool nErr = 1;
    SCashRegisterModel *cashRegister;
    SRepairModel *repairModel;
    SSaleTableModel *BOQModel;
    SWorkshopIssuedModel *workshopIssuedModel;
    QList<SRepairModel*>::const_iterator i = m_repairsModels.constBegin();
    int repairId;
    int newState;
    double amountToPay;

    // TODO: проверка безналичной оплаты

    while(i != m_repairsModels.constEnd() && m_pushButtonIssueEnabled)
    {
        repairModel = (*i);

        if(comSettings->useSimplifiedCartridgeRepair && repairModel->cartridge() && repairModel->state() == Global::RepStateIds::InWork)
            setRepairReady(repairModel);

        repairId = repairModel->id();

        amountToPay = m_repairsWithPayment.value(QString::number(repairId), 0);

        switch ( ((amountToPay == 0) << BitAmountZero) | (m_isIssuingInCredit << BitInCredit) | (repairModel->quickRepair() << BitQuickRepair) | repairModel->state() )
        {
            case Global::RepStateIds::ReadyNoRepair:    // выдача без ремонта с оплатой (например, оплата диагностики)
            case Global::RepStateIds::ReadyNoRepair | AmountZero:
            case Global::RepStateIds::ReadyNoRepair | AmountZero | InCredit: newState = Global::RepStateIds::ReturnedNoRepair; break;
            case Global::RepStateIds::GetIn | QuickRepair:
            case Global::RepStateIds::Ready:
            case Global::RepStateIds::Ready | QuickRepair:  // этот вариант на тот случай, если в каком-то ремонте по ошибке установлена галочка "Быстрый ремонт" (например, из-за косяков предыдущих ревизий)
            case Global::RepStateIds::Ready | AmountZero:
            case Global::RepStateIds::Ready | AmountZero | InCredit: newState = Global::RepStateIds::Returned; break; // при выдаче в долг ранее оплаченных они переключаются в "Выдано клиенту"
            case Global::RepStateIds::GetIn | InCredit | QuickRepair:
            case Global::RepStateIds::Ready | InCredit: newState = Global::RepStateIds::ReturnedInCredit; break;
            default: throw Global::ThrowType::ConditionsError;
        }

        workshopIssuedModel = new SWorkshopIssuedModel();
        workshopIssuedModel->setRepair(repairId);
        BOQModel = repairModel->BOQModel();

        if(amountToPay && m_isPayFromBalance)
        {
            m_clientModel->updateBalance(-amountToPay, tr("Списание %1 за ремонт №%2").arg(sysLocale.toCurrencyString(amountToPay)).arg(repairId));
        }

        repairModel->setBoxIndex(-1);
        repairModel->setState(newState);
        if(!m_rejectReason.isEmpty())
            repairModel->setRejectReason(m_rejectReason);
        BOQModel->repair_saveTables(SSaleTableModel::RepairOpType::Sale);
        repairModel->commit();
        nErr = workshopIssuedModel->commit();

        delete workshopIssuedModel;

        if(!nErr)
            throw Global::ThrowType::QueryError;

        i++;
    }

    // TODO: нужен какой-то идентификатор группы выданных картриджей (просто уникальный номер или, например, запись в таблице docs)
    //       Он будет использоваться для печати кассового чека с детализацией.
    if(m_totalAmountToPay && !m_isPayFromBalance)
    {
        cashRegister = new SCashRegisterModel();

        cashRegister->setOperationType(SCashRegisterModel::RecptRepair);
        if(m_repairsModels.count() == 1)
            cashRegister->setRepairId(repairId);
        cashRegister->setAmount(m_totalAmountToPay);
        cashRegister->setReason(cashRegister->constructReason(m_repairsWithPayment.keys().join(", ")));
        cashRegister->setClient(m_clientModel->id());
        cashRegister->setSystemId(m_paymentSystemId);
        // Выбор компании и офиса пользователем не предусмотрен; используются текущие значения из userDbData
        nErr = cashRegister->commit();

        delete cashRegister;

        if(!nErr)
            throw Global::ThrowType::QueryError;
    }
}

/* Установка платёжной системы
 * Данный метод используется при выдаче ремонта без отображения диалогового окна
 * Приоритет метода ниже, чем setPayFromBalance или setIssuingInCredit
*/
void SDialogIssueRepair::setPaymentSystemId(int id)
{
    if(m_isPayFromBalance || m_isIssuingInCredit)
        return;

    m_paymentSystemId = id;
}

void SDialogIssueRepair::setPayFromBalance(bool state)
{
    m_isPayFromBalance = state;
    if(state)
        m_paymentSystemId = Global::PaymentSystemIds::Balance;
}

void SDialogIssueRepair::setIssuingInCredit(bool state)
{
    m_isIssuingInCredit = state;
    if(state)
        setPayFromBalance();
}

/* Сбор параметров выдачи
 * Данный метод используется при выдаче ремонта с отображением диалогового окна
*/
void SDialogIssueRepair::getWidgetsValues()
{
    m_isIssuingInCredit = ui->checkBoxIssueInCredit->isChecked();

    // если установлен checkBox "В долг", то деньги всегда списываются с баланса клиента
    // разница лишь в проверке достаточного остатка на балансе
    if(ui->checkBoxPayFromBalance->isChecked() || m_isIssuingInCredit)
    {
        m_paymentSystemId = Global::PaymentSystemIds::Balance;
        m_isPayFromBalance = 1;
    }
    else
    {
        m_paymentSystemId = m_paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id");
        m_isPayFromBalance = (m_paymentSystemId == Global::PaymentSystemIds::Balance);
    }

    if(ui->comboBoxRejectReason->isVisible())
    {
        if(ui->comboBoxRejectReason->currentIndex() != rejectReasonModel->property("other_reject_reason").toInt())
            m_rejectReason = ui->comboBoxRejectReason->currentText();
        else
            m_rejectReason = ui->textEditRejectReason->toPlainText();
    }
}

/*  Удаление моделей данных ремонтов
 *  Используется при групповой выдаче ремонтов непосредственно с вкладки Ремонты
*/
void SDialogIssueRepair::deleteRepairModels()
{
    if(!m_isListOwner)
        return;

    SRepairModel *repair;
    while(m_repairsModels.count())
    {
        repair = m_repairsModels.last();
        m_repairsModels.removeLast();
        delete repair;
    }
}

void SDialogIssueRepair::setListOwner(bool state)
{
    m_isListOwner = state;
}
void SDialogIssueRepair::buttonCancelClicked()
{
    this->deleteLater();
}

void SDialogIssueRepair::createLooseDoc()
{

}

void SDialogIssueRepair::otheRejectReasonShow(QString comboBoxText)
{
    ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheet);
    if(comboBoxText == rejectReasonModel->getDisplayRole(rejectReasonModel->property("other_reject_reason").toInt(), 1))
    {
        ui->textEditRejectReason->setHidden(false);
        ui->textEditRejectReason->resize(ui->comboBoxRejectReason->width(), 100);
        ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
    }
    else
    {
        ui->textEditRejectReason->setHidden(true);
    }
}

void SDialogIssueRepair::textEditTextChanged()
{
    ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
}

void SDialogIssueRepair::paymentSystemChanged(int index)
{
    ui->checkBoxPayFromBalance->blockSignals(true);
    if(m_paymentSystemsProxyModel->databaseIDByRow(index, "system_id") == Global::PaymentSystemIds::Balance)
        ui->checkBoxPayFromBalance->setChecked(true);
    else
        ui->checkBoxPayFromBalance->setChecked(false);
    ui->checkBoxPayFromBalance->blockSignals(false);
}

void SDialogIssueRepair::checkBoxInCreditToggled(bool state)
{
    ui->comboBoxPaymentAccount->blockSignals(true);
    if(state)
        ui->comboBoxPaymentAccount->setCurrentIndex(m_paymentSystemsProxyModel->rowByDatabaseID(Global::PaymentSystemIds::Balance, "system_id"));
    else
        ui->comboBoxPaymentAccount->setCurrentIndex(m_paymentSystemsProxyModel->rowByDatabaseID(Global::PaymentSystemIds::Cash, "system_id"));
    ui->comboBoxPaymentAccount->blockSignals(false);
}

void SDialogIssueRepair::checkBoxIssueInCreditToggled(bool state)
{
    ui->checkBoxPayFromBalance->setChecked(state);
    ui->checkBoxPayFromBalance->setEnabled(!state);
    ui->comboBoxPaymentAccount->setEnabled(!state);
    ui->checkBoxPaymentCheckout->setChecked(false);
    ui->checkBoxPaymentCheckout->setEnabled(!state);
}
