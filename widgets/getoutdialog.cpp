#include "global.h"
#include "getoutdialog.h"
#include "ui_getoutdialog.h"
#include "tabrepair.h"

getOutDialog::getOutDialog(QList<SRepairModel*> repairs, Qt::WindowFlags flags, QWidget *parent) :
    SWidget(parent, flags),
    ui(new Ui::getOutDialog)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    ui->doubleSpinBoxTotalAmount->setMinimum(-9999999.990000);
    setDefaultStyleSheets();
    m_repairsModels = repairs;
    m_clientModel = new SClientModel();
    m_clientModel->load(repairs.at(0)->clientId());
    initPaymentSystems();

    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxRejectReason->setModel(rejectReasonModel);
    ui->comboBoxRejectReason->setCurrentIndex(-1);
    connect(ui->comboBoxRejectReason, SIGNAL(currentTextChanged(QString)), this, SLOT(otheRejectReasonShow(QString)));
    connect(ui->textEditRejectReason, SIGNAL(textChanged()), this, SLOT(textEditTextChanged()));

    collectRepairsData();
    ui->checkBoxDiagDocPrint->setVisible(m_singleRepairWidgetsVisible);
    ui->pushButtonLooseDocPrint->setVisible(m_singleRepairWidgetsVisible);
    ui->comboBoxRejectReason->setVisible(m_singleRepairWidgetsVisible && m_rejectReasonWidgetsVisible);
    ui->textEditRejectReason->setVisible(false);
    ui->labelPrepay->setVisible(m_singleRepairWidgetsVisible);
    ui->doubleSpinBoxPrepay->setVisible(m_singleRepairWidgetsVisible);
    ui->labelAgreedAmount->setVisible(m_singleRepairWidgetsVisible);
    ui->doubleSpinBoxAgreedAmount->setVisible(m_singleRepairWidgetsVisible);
    ui->checkBoxPaymentCheckout->setVisible(m_totalAmountToPay != 0);
    ui->checkBoxConfirmGetOut->setVisible(m_totalAmountToPay == 0);
    ui->labelIssuedMessage->setVisible(m_singleRepairWidgetsVisible);
    if(m_singleRepairWidgetsVisible)
        ui->labelIssuedMessage->setText(repairs.at(0)->issuedMsg());

    ui->doubleSpinBoxPrepay->setValue(m_totalPrepayAmount);
    ui->doubleSpinBoxAgreedAmount->setValue(m_totalAgreedAmount);
    ui->doubleSpinBoxAlreadyPayed->setValue(m_totalPrepayAmount);  // назначение данного поля не понятно, оно вроде бы, всегда равно полю Предоплата
    ui->doubleSpinBoxTotalAmount->setValue(m_totalAmount);
    ui->doubleSpinBoxCurrentPaymentAmount->setValue(m_totalAmountToPay);

    ui->pushButtonIssue->setEnabled(m_pushButtonIssueEnabled);

    show();

    if (!m_pushButtonIssueEnabled)
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Ошибка данных. Попробуйте закрыть и снова открыть карту ремонта.", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
    }

#ifdef QT_DEBUG
//    ui->checkBoxConfirmGetOut->setChecked(true);
//    ui->checkBoxPaymentCheckout->setChecked(true);
//    buttonIssueClicked();
#endif
}

getOutDialog::~getOutDialog()
{
    delete ui;
    emit close();
}

void getOutDialog::setDefaultStyleSheets()
{
    ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
}

void getOutDialog::initPaymentSystems()
{
    m_paymentSystemsProxyModel = new SSortFilterProxyModel();
    m_paymentSystemsProxyModel->setSourceModel(paymentSystemsModel);
    if(!m_clientModel->balanceEnabled())
    {
        m_paymentSystemsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(" + QString::number(Global::PaymentSystemIds::Balance) + ")).*$"));
        ui->checkBoxCreditPayment->setHidden(true);
    }
    m_paymentSystemsProxyModel->setFilterKeyColumn(1);
    ui->comboBoxPaymentAccount->setModel(m_paymentSystemsProxyModel);
    ui->comboBoxPaymentAccount->setCurrentIndex(m_paymentSystemsProxyModel->rowByDatabaseID(userDbData->defaultPaymentSystem, "system_id"));
}

bool getOutDialog::checkInput()
{
    setDefaultStyleSheets();
    if(ui->comboBoxRejectReason->isVisible() && ui->comboBoxRejectReason->currentIndex() == -1)
    {
        ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheetRed);
        return 0;
    }
    if(ui->textEditRejectReason->isVisible() && ui->textEditRejectReason->toPlainText() == "")
    {
        ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheetRed);
        return 0;
    }

    return 1;
}

bool getOutDialog::checkAmounts()
{
    if(m_summsNotEq)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                    tr("Сумма ремонта не равна согласованной. Всё ОК?\n"),
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::No);
        if (resBtn == QMessageBox::No)
        {
            return 0;
        }
    }
    return 1;
}

void getOutDialog::collectRepairsData()
{
    SRepairModel *repairModel;
    SSaleTableModel *worksAndSparePartsModel;
    QList<SRepairModel*>::const_iterator i = m_repairsModels.constBegin();
    int repairId;
    double worksAndSparePartsSumm = 0;
    double prepayAmount = 0;
    double repairCost = 0;
    double amountToPay = 0;
    double realRepairCost = 0;
    int singleRepairWidgetsVisible = 0;

    m_rejectReasonWidgetsVisible = 0;
    m_totalPrepayAmount = 0;
    m_totalAgreedAmount = 0;
    m_totalAmount = 0;
    m_totalAmountToPay = 0;

    while(i != m_repairsModels.constEnd())
    {
        repairModel = (*i);
//        repairModel->reload();
        repairId = repairModel->id();
        worksAndSparePartsModel = repairModel->worksAndPartsModel();

        m_singleRepairWidgetsVisible = !singleRepairWidgetsVisible;
        if(repairModel->state() == Global::RepStateIds::ReadyNoRepair)
        {
            m_rejectReasonWidgetsVisible = m_singleRepairWidgetsVisible;
        }

        repairCost = repairModel->repairCost();
        realRepairCost = repairModel->realRepairCost();
        prepayAmount = repairModel->realPrepaidSumm();
        worksAndSparePartsSumm = worksAndSparePartsModel->amountTotal();
        amountToPay = worksAndSparePartsSumm - prepayAmount;

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
        m_totalAgreedAmount += repairCost;
        m_totalAmountToPay += amountToPay;

        i++;
        singleRepairWidgetsVisible++;
    }
}

void getOutDialog::buttonIssueClicked()
{
    bool nErr = 1;

    if(!checkInput())
        return;

    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    if( ui->checkBoxPaymentCheckout->isVisible() && !ui->checkBoxPaymentCheckout->isChecked())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Ошибка"), tr("Подтвердите правильность ввода данных"), QColor("#FFC7AD"), QColor("#FFA477"));
        return;
    }
    if( ui->checkBoxConfirmGetOut->isVisible() && !ui->checkBoxConfirmGetOut->isChecked())
    {
        // TODO: подумать над заменой уведомления на окраску чекбокса в красный
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Ошибка"), tr("Подтвердите выдачу"), QColor("#FFC7AD"), QColor("#FFA477"));
        return;
    }
    if(!checkAmounts())
    {
        return;
    }
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

        emit issueFailed();
    }
    QUERY_LOG_STOP;

    if(nErr)
    {
        emit issueSuccessfull();
        this->deleteLater();
    }
}

void getOutDialog::issueRepairs()
{
    bool nErr = 1;
    int paymentAccount = 0;
    bool balance = 0;
    SCashRegisterModel *cashRegister;
    QStringList repairsListForCashRegister;
    SRepairModel *repairModel;
    SSaleTableModel *worksAndSparePartsModel;
    SWorkshopIssuedModel *workshopIssuedModel;
    QList<SRepairModel*>::const_iterator i = m_repairsModels.constBegin();
    int repairId;
    int newState;
    double amountToPay;

    // TODO: проверка безналичной оплаты

    if(ui->checkBoxCreditPayment->isChecked())
    {
        paymentAccount = -2;
        balance = 1;
    }
    else
    {
        paymentAccount = paymentSystemsModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id");
        balance = paymentAccount == -2;
    }

    if(balance)
    {
        if(!m_clientModel->balanceEnough(-m_totalAmountToPay)) // проверка при выдаче нескольких ремонтов за раз с оплатой с баланса
            throw Global::ThrowType::UserCanceled;
    }

    while(i != m_repairsModels.constEnd() && m_pushButtonIssueEnabled)
    {
        repairModel = (*i);

        switch (repairModel->state())
        {
            case Global::RepStateIds::Ready: newState = Global::RepStateIds::Returned; break;
            case Global::RepStateIds::ReadyNoRepair: newState = Global::RepStateIds::ReturnedNoRepair; break;
            default: throw Global::ThrowType::ConditionsError;
        }

        repairId = repairModel->id();
        workshopIssuedModel = new SWorkshopIssuedModel();
        workshopIssuedModel->setRepair(repairId);
        worksAndSparePartsModel = repairModel->worksAndPartsModel();
        repairsListForCashRegister.append(QString::number(repairId));

        amountToPay = repairModel->realRepairCost() - repairModel->prepaidSumm();

        if(amountToPay && balance)
        {
            m_clientModel->updateBalance(-amountToPay, tr("Списание %1 за ремонт №%2").arg(sysLocale.toCurrencyString(amountToPay)).arg(repairId));
        }

        repairModel->setBoxIndex(-1);
        repairModel->setState(newState);
        if(ui->comboBoxRejectReason->isVisible())
        {
            if(ui->comboBoxRejectReason->currentIndex() != rejectReasonModel->property("other_reject_reason").toInt())
                repairModel->setRejectReason(ui->comboBoxRejectReason->currentText());
            else
                repairModel->setRejectReason(ui->textEditRejectReason->toPlainText());
        }
        worksAndSparePartsModel->repair_saveTables(SSaleTableModel::RepairOpType::Sale);
        repairModel->commit();
        nErr = workshopIssuedModel->commit();

        delete workshopIssuedModel;

        if(!nErr)
            throw Global::ThrowType::QueryError;

        i++;
    }

    // TODO: нужен какой-то идентификатор группы выданных картриджей (просто уникальный номер или, например, запись в таблице docs)
    //       Он будет использоваться для печати кассового чека с детализацией.
    if(!balance)
    {
        cashRegister = new SCashRegisterModel();

        cashRegister->setOperationType(SCashRegisterModel::RecptRepair);
        if(repairsListForCashRegister.count() == 1)
            cashRegister->setRepairId(repairId);
        cashRegister->setAmount(m_totalAmountToPay);
        cashRegister->setReason(cashRegister->constructReason(repairsListForCashRegister.join(", ")));
        cashRegister->setClient(m_clientModel->id());
        cashRegister->setSystemId(m_paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id"));
        nErr = cashRegister->commit();

        delete cashRegister;

        if(!nErr)
            throw Global::ThrowType::QueryError;
    }
}

void getOutDialog::buttonCancelClicked()
{
    emit close();
}

void getOutDialog::createLooseDoc()
{

}

void getOutDialog::otheRejectReasonShow(QString comboBoxText)
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

void getOutDialog::textEditTextChanged()
{
    ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
}

void getOutDialog::paymentSystemChanged(int index)
{
    ui->checkBoxCreditPayment->blockSignals(true);
    if(paymentSystemsModel->databaseIDByRow(index, "system_id") == Global::PaymentSystemIds::Balance)
        ui->checkBoxCreditPayment->setChecked(true);
    else
        ui->checkBoxCreditPayment->setChecked(false);
    ui->checkBoxCreditPayment->blockSignals(false);
}

void getOutDialog::checkBoxInCreditToggled(bool state)
{
    ui->comboBoxPaymentAccount->blockSignals(true);
    if(state)
        ui->comboBoxPaymentAccount->setCurrentIndex(paymentSystemsModel->rowByDatabaseID(Global::PaymentSystemIds::Balance, "system_id"));
    else
        ui->comboBoxPaymentAccount->setCurrentIndex(paymentSystemsModel->rowByDatabaseID(Global::PaymentSystemIds::Cash, "system_id"));
    ui->comboBoxPaymentAccount->blockSignals(false);
}
