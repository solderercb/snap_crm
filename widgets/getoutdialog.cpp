#include "global.h"
#include "getoutdialog.h"
#include "ui_getoutdialog.h"
#include "tabrepair.h"

getOutDialog::getOutDialog(QWidget *p, Qt::WindowFlags f) :
    QWidget(p, f),
    ui(new Ui::getOutDialog),
    parent(p)
{
    float worksAndSparePartsSumm;
    ui->setupUi(this);
    setDefaultStyleSheets();
    connect(ui->comboBoxRejectReason, SIGNAL(currentTextChanged(QString)), this, SLOT(otheRejectReasonShow(QString)));
    connect(ui->textEditRejectReason, SIGNAL(textChanged()), this, SLOT(textEditTextChanged()));
    tabRepair *parentTab = static_cast<tabRepair*>(parent);
    setRepairModel(parentTab->repairModel);
    setClientModel(parentTab->clientModel);
    setSaleTableModel(parentTab->worksAndPartsModel);

    m_repairId = repairModel->id();
    repairModel->reload();
    summToPay = repairModel->realRepairCost() - repairModel->prepaidSumm();

    ui->textEditRejectReason->setHidden(true);
    if (repairModel->state() == Global::RepStateIds::Ready)
    {
        newState = Global::RepStateIds::Returned;
        ui->comboBoxRejectReason->setHidden(true);  // если статус "Готово к выдаче", то элементы выбора/ввода причины отказа скрываем
        ui->checkBoxConfirmGetOut->setHidden(true);
    }
    else
    {
        newState = Global::RepStateIds::ReturnedNoRepair;
        ui->comboBoxRejectReason->setModel(rejectReasonModel);
        ui->comboBoxRejectReason->setCurrentIndex(-1);
        ui->checkBoxPaymentCheckout->setHidden(true);
    }
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditPrepay->setText(sysLocale.toString(repairModel->prepaidSumm(), 'f', 2));
    ui->lineEditAgreedAmount->setText(sysLocale.toString(repairModel->repairCost(), 'f', 2));
    ui->lineEditAlreadyPayed->setText(sysLocale.toString(repairModel->prepaidSumm(), 'f', 2));  // назначение данного поля не понятно, оно вроде бы, всегда равно полю Предоплата
    ui->lineEditTotalAmount->setText(sysLocale.toString(repairModel->realRepairCost(), 'f', 2));
    ui->lineEditCurrentPaymentAmount->setText(sysLocale.toString(summToPay, 'f', 2));
    initPaymentSystems();
    if(repairModel->issuedMsg().isEmpty())
        ui->labelIssuedMessage->setHidden(true);
    else
        ui->labelIssuedMessage->setText(repairModel->issuedMsg());
    if(repairModel->repairCost() != saleTableModel->amountTotal()) // если сумма работ и деталей не равна согласованной, отобразится сообщение
        summsNotEq = 1;

    // Деньги это важно, поэтому перепроверяем суммы
    if(repairModel->realPrepaidSumm() != repairModel->prepaidSumm())
    {
        appLog->appendRecord(QString("Rechecking prepaid summ for repair %1: mismatch.").arg(m_repairId));
        ui->pushButtonGetout->setDisabled(true);
    }
    worksAndSparePartsSumm = repairModel->realWorksCost() + repairModel->realPartsCost();
    if(worksAndSparePartsSumm != repairModel->realRepairCost())
    {
        appLog->appendRecord(QString("Rechecking total amount of works and parts for repair %1: mismatch.").arg(m_repairId));
        ui->pushButtonGetout->setDisabled(true);
    }

    if (!ui->pushButtonGetout->isEnabled())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Ошибка данных. Попробуйте закрыть и снова открыть карту ремонта.", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
    }

#ifdef QT_DEBUG
//    ui->checkBoxConfirmGetOut->setChecked(true);
//    ui->checkBoxPaymentCheckout->setChecked(true);
//    getOut();
#endif
}

getOutDialog::~getOutDialog()
{
    delete ui;
    emit close();
}

void getOutDialog::setRepairModel(SRepairModel *model)
{
    repairModel = model;
}

void getOutDialog::setClientModel(SClientModel *model)
{
    clientModel = model;
}

void getOutDialog::setSaleTableModel(SSaleTableModel *model)
{
    saleTableModel = model;
}

void getOutDialog::setDefaultStyleSheets()
{
    ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheet);
}

void getOutDialog::initPaymentSystems()
{
    paymentSystemsProxyModel = new SSortFilterProxyModel();
    paymentSystemsProxyModel->setSourceModel(paymentSystemsModel);
    if(!clientModel->balanceEnabled())
    {
        paymentSystemsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(" + QString::number(Global::PaymentSystemIds::Balance) + ")).*$"));
        ui->checkBoxCreditPayment->setHidden(true);
    }
    paymentSystemsProxyModel->setFilterKeyColumn(1);
    ui->comboBoxPaymentAccount->setModel(paymentSystemsProxyModel);
    ui->comboBoxPaymentAccount->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(userDbData->value("defaultPaymentSystem", 0).toInt(), "system_id"));
}

bool getOutDialog::checkAmounts()
{
    if(summsNotEq)
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

void getOutDialog::getOut()
{
    setDefaultStyleSheets();
    if(ui->comboBoxRejectReason->isVisible() && ui->comboBoxRejectReason->currentIndex() == -1)
    {
        ui->comboBoxRejectReason->setStyleSheet(commonComboBoxStyleSheetRed);
        return;
    }
    if(ui->textEditRejectReason->isVisible() && ui->textEditRejectReason->toPlainText() == "")
    {
        ui->textEditRejectReason->setStyleSheet(commonTextEditStyleSheetRed);
        return;
    }

    // TODO: проверка безналичной оплаты

    SCashRegisterModel *cashRegister = new SCashRegisterModel();
    SWorkshopIssuedModel *workshopIssuedModel = new SWorkshopIssuedModel();
    SRepairStatusLog *repairStatusLog = new SRepairStatusLog(m_repairId);
#ifdef QT_DEBUG
    SQueryLog *queryLog = new SQueryLog();
#endif
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nErr = 1;

    if( ui->checkBoxPaymentCheckout->isVisible() && !ui->checkBoxPaymentCheckout->isChecked())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Ошибка"), tr("Подтвердите правильность ввода данных"), QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
        return;
    }
    if( ui->checkBoxConfirmGetOut->isVisible() && !ui->checkBoxConfirmGetOut->isChecked())
    {
        // TODO: подумать над заменой уведомления на окраску чекбокса в красный
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Ошибка"), tr("Подтвердите выдачу"), QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
        return;
    }
    if(!checkAmounts())
    {
        return;
    }

    QUERY_LOG_START(metaObject()->className());
    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        if(summToPay)
        {
            if(ui->checkBoxCreditPayment->isChecked())
            {
                clientModel->updateBalance(-summToPay, tr("Списание %1 за ремонт №%2").arg(sysLocale.toCurrencyString(summToPay)).arg(m_repairId));
            }
            else
            {
                cashRegister->setId(0);
                cashRegister->setClient(clientModel->id());
                cashRegister->setOperationType(SCashRegisterModel::RecptRepair);
                cashRegister->setRepairId(m_repairId);
                cashRegister->setAmount(summToPay);
                cashRegister->setReason(cashRegister->constructReason(m_repairId));
//                cashRegister->setSkipLogRecording(true);
                nErr = cashRegister->commit();
            }
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
        saleTableModel->repair_saveTables(SSaleTableModel::RepairOpType::Sale);
        repairModel->commit();
        repairStatusLog->setStatus(newState);
        repairStatusLog->setManager(repairModel->currentManager());
        repairStatusLog->setEngineer(repairModel->engineer());
        repairStatusLog->commit();
        workshopIssuedModel->setRepair(m_repairId);
        workshopIssuedModel->commit();

#ifdef QT_DEBUG
//        throw 0; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (int type)
    {
        nErr = 0;
        if(type == 0)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }
    QUERY_LOG_STOP;

    if(nErr)
    {
        emit getOutOk();
        this->deleteLater();
    }

    delete workshopIssuedModel;
    delete repairStatusLog;
}

void getOutDialog::cancel()
{
    emit close();
//    this->deleteLater();
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
