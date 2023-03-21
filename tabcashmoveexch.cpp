#include "global.h"
#include "tabcashmoveexch.h"
#include "ui_tabcashmoveexch.h"

tabCashMoveExch* tabCashMoveExch::p_instance;

tabCashMoveExch::tabCashMoveExch(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabCashMoveExch)
{
    ui->setupUi(this);
    i_tabTitle = tr("Перемещение денег");
    ui->checkBoxPrintCheck->setChecked(comSettings->value("print_check").toBool());

    paymentSystemsProxyModel = new SSortFilterProxyModel();
    paymentSystemsProxyModel->setSourceModel(paymentSystemsModel);
    paymentSystemsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(-2)|(-1)).*$"));
    paymentSystemsProxyModel->setFilterKeyColumn(1);
    ui->comboBoxSrcPaymentAccount->setModel(paymentSystemsProxyModel);
    ui->comboBoxSrcPaymentAccount->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(userDbData->value("defaultPaymentSystem", 0).toInt(), "system_id"));
    ui->comboBoxDstPaymentAccount->setModel(paymentSystemsProxyModel);
    ui->comboBoxDstPaymentAccount->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(userDbData->value("defaultPaymentSystem", 0).toInt(), "system_id"));
    ui->comboBoxSrcOffice->setModel(officesModel);
    ui->comboBoxSrcOffice->setCurrentIndex(officesModel->rowByDatabaseID(userDbData->value("office").toInt()));
    ui->comboBoxDstOffice->setModel(officesModel);
    ui->comboBoxDstOffice->setCurrentIndex(officesModel->rowByDatabaseID(userDbData->value("office").toInt()));
    cashRegisterSrc = new SCashRegisterModel();
    cashRegisterDst = new SCashRegisterModel();
    initCashRegisterModel();
    operationTypeChanged(0);

    userActivityLog->appendRecord("Navigation " + tabTitle());

#ifdef QT_DEBUG
    test_scheduler_counter = RandomFillerStep::OpType;
    test_scheduler->start(200);
#endif
}

tabCashMoveExch *tabCashMoveExch::getInstance(MainWindow *parent)
{
    if( p_instance == nullptr )
      p_instance = new tabCashMoveExch(parent);
    return p_instance;
}

tabCashMoveExch::~tabCashMoveExch()
{
    userActivityLog->updateActivityTimestamp();

    delete ui;
    delete cashRegisterSrc;
    delete cashRegisterDst;
    delete paymentSystemsProxyModel;
    p_instance = nullptr;
}

void tabCashMoveExch::setDefaultStylesheets()
{
    ui->doubleSpinBoxSrcAmount->setStyleSheet(commonSpinBoxStyleSheet);
    ui->doubleSpinBoxDstAmount->setStyleSheet(commonSpinBoxStyleSheet);
    ui->doubleSpinBoxCharge->setStyleSheet(commonSpinBoxStyleSheet);
    ui->comboBoxSrcPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxDstPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditReason->setStyleSheet(commonLineEditStyleSheet);

}

bool tabCashMoveExch::commit(bool repeatAfter)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nErr = 1;

    QUERY_LOG_START(metaObject()->className());

    if(!checkInput())
        return 0;

    cashRegisterSrc->setOperationType(SCashRegisterModel::MoveCash);
    cashRegisterDst->setOperationType(SCashRegisterModel::MoveCash);
    cashRegisterSrc->setAmount(-ui->doubleSpinBoxSrcAmount->value());
    cashRegisterDst->setAmount(ui->doubleSpinBoxDstAmount->value());
    cashRegisterSrc->setOfficeIndex(ui->comboBoxSrcOffice->currentIndex());
    cashRegisterDst->setOfficeIndex(ui->comboBoxDstOffice->currentIndex());
    cashRegisterSrc->setCreatedDate(ui->dateEdit->date());
    cashRegisterDst->setCreatedDate(ui->dateEdit->date());
    cashRegisterSrc->setSystemId(paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxSrcPaymentAccount->currentIndex(), "system_id"));
    cashRegisterDst->setSystemId(paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxDstPaymentAccount->currentIndex(), "system_id"));
    cashRegisterSrc->setSkipLogRecording(m_skipAutoLogRecord);
    cashRegisterDst->setSkipLogRecording(m_skipAutoLogRecord);

    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        cashRegisterSrc->setReason(QString());  // поле обязательное, поэтому сначала записывается пустая строка
        cashRegisterDst->setReason(QString());
        cashRegisterSrc->commit();
        cashRegisterDst->commit();
        m_srcOrderId = cashRegisterSrc->id();
        m_dstOrderId = cashRegisterDst->id();
        cashRegisterSrc->setReason(QString("%1; %2").arg(cashRegisterSrc->constructReason(m_dstOrderId)).arg(ui->lineEditReason->text()));    //
        cashRegisterDst->setReason(QString("%1; %2").arg(cashRegisterDst->constructReason(m_srcOrderId)).arg(ui->lineEditReason->text()));
        cashRegisterSrc->commit();
        cashRegisterDst->commit();

#ifdef QT_DEBUG
//        throw 0; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif

        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (int type)
    {
        nErr = 0;
        initCashRegisterModel();
        if(type == 0)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
    }

#ifdef QT_DEBUG
//    nErr = 1; // и это для отладки (чтобы проверить работу дальше)
#endif

    QUERY_LOG_STOP;

    delete query;

    if(nErr)
    {
        if(!repeatAfter)
        {
            emit updateTabTitle(this);
            m_widgetsRO = 1;
            m_reasonRO = 1;
            m_showCheckBoxPrint = 0;
            updateWidgets();
        }

        if(ui->checkBoxPrintCheck->isChecked())
            print();
    }
    return nErr;
}

void tabCashMoveExch::print()
{
    QMap<QString, QVariant> report_vars;
    report_vars.insert("type", Global::Reports::pko);
    report_vars.insert("record_id", m_dstOrderId);
    emit generatePrintout(report_vars);
}

void tabCashMoveExch::updateOrderIdLineEdit()
{
    ui->lineEditOrderNum->setText(QString("%1, %2").arg(cashRegisterSrc->id()).arg(cashRegisterDst->id()));
    ui->lineEditOrderNum->setStyleSheet("QLineEdit { background: #90EE90;}");   // светло-зелёный
}

void tabCashMoveExch::constructReasonPart()
{
    if(m_opType == Type::Exchange)
    {
        m_reason = QString("%1, %2 %3").arg(tr("обмен валют"), chargeType, sysLocale.toString(ui->doubleSpinBoxCharge->value(), 'f', 4));
    }
    else
    {
        m_reason = QString("%1 %2").arg(chargeType, sysLocale.toString(ui->doubleSpinBoxCharge->value(), 'f', 2));
    }
}

void tabCashMoveExch::flipCurrency()
{
    ui->toolButtonFlipCurrency->setFocus(); // Qt 5.15.4 при нажатии toolButton фокус остаётся на doubleSpinBox и сигнал editingFinished не генерируется
    if(currencyFlipped)
        ui->doubleSpinBoxCharge->setValue(prevCurrencyRate);
    else
    {
        prevCurrencyRate = ui->doubleSpinBoxCharge->value();
        ui->doubleSpinBoxCharge->setValue(1/ui->doubleSpinBoxCharge->value());
    }
    currencyFlipped ^= 1;
}

QString tabCashMoveExch::tabTitle()
{
    return i_tabTitle;
}

void tabCashMoveExch::updateWidgets()
{
    setDefaultStylesheets();
    if(m_srcOrderId && m_dstOrderId)
    {
        m_widgetsRO = 1;
        ui->lineEditDate->setText(cashRegisterSrc->created());
        ui->buttonSave->hide();
        ui->buttonSaveMore->hide();
        updateOrderIdLineEdit();
    }
    else
    {
        m_widgetsRO = 0;
        ui->dateEdit->setDate(QDate::currentDate());
        ui->buttonSave->show();
        ui->buttonSaveMore->show();
        ui->lineEditReason->setText(m_reason);
    }

    ui->radioButtonMove->setDisabled(m_widgetsRO);
    ui->radioButtonExchange->setDisabled(m_widgetsRO);
    ui->lineEditDate->setVisible(m_widgetsRO);
    ui->dateEdit->setVisible(!m_widgetsRO && permissions->value("71"));    // Проводить документы задним числом
    ui->comboBoxSrcOffice->setEnabled(!m_widgetsRO);
    ui->comboBoxDstOffice->setEnabled(!m_widgetsRO);
    ui->comboBoxSrcPaymentAccount->setEnabled(!m_widgetsRO);
    ui->comboBoxDstPaymentAccount->setEnabled(!m_widgetsRO);
    ui->doubleSpinBoxSrcAmount->setReadOnly(m_widgetsRO);
    ui->doubleSpinBoxDstAmount->setReadOnly(m_widgetsRO);
    ui->doubleSpinBoxCharge->setReadOnly(m_widgetsRO);
    ui->checkBoxPrintCheck->setVisible(!m_widgetsRO);
    ui->checkBoxAutoReason->setDisabled(m_widgetsRO);
    ui->lineEditReason->setReadOnly(m_reasonRO);
    ui->toolButtonFlipCurrency->setVisible(m_showFlipCurrencyButton && !m_widgetsRO);
}

bool tabCashMoveExch::checkInput()
{
    int error = 0;

    setDefaultStylesheets();
    if(ui->doubleSpinBoxSrcAmount->value() == 0)
    {
        error |= 1<<0;
        ui->doubleSpinBoxSrcAmount->setStyleSheet(commonSpinBoxStyleSheetRed);
    }
    if(ui->comboBoxSrcPaymentAccount->currentIndex() == -1)
    {
        error |= 1<<1;
        ui->comboBoxSrcPaymentAccount->setStyleSheet(commonComboBoxStyleSheetRed);
    }
    if(ui->doubleSpinBoxDstAmount->value() == 0)
    {
        error |= 1<<2;
        ui->doubleSpinBoxDstAmount->setStyleSheet(commonSpinBoxStyleSheetRed);
    }
    if(ui->comboBoxDstPaymentAccount->currentIndex() == -1)
    {
        error |= 1<<3;
        ui->comboBoxDstPaymentAccount->setStyleSheet(commonComboBoxStyleSheetRed);
    }
    if(ui->comboBoxSrcPaymentAccount->currentIndex() == ui->comboBoxDstPaymentAccount->currentIndex())
    {
        error |= 1<<4;
        ui->comboBoxDstPaymentAccount->setStyleSheet(commonComboBoxStyleSheetRed);
    }
    if(!m_reasonRO && ui->lineEditReason->text().isEmpty())
    {
        error |= 1<<5;
        ui->lineEditReason->setStyleSheet(commonLineEditStyleSheetRed);
    }
    if(m_opType == Type::Move && ui->doubleSpinBoxCharge->value() < 0)
    {
        error |= 1<<6;
        ui->doubleSpinBoxCharge->setStyleSheet(commonSpinBoxStyleSheetRed);
    }

    if (error)
    {
        appLog->appendRecord(QString("Error while creating cash register records: probably some obligatory fields empty (error %1)").arg(error));
        return 0;
    }
    return 1;
}

void tabCashMoveExch::initCashRegisterModel()
{
    m_srcOrderId = 0;
    m_dstOrderId = 0;
    cashRegisterSrc->setId(0);
    cashRegisterDst->setId(0);
}

void tabCashMoveExch::operationTypeChanged(bool)
{
    ui->doubleSpinBoxCharge->setMinimum(0);
    if(ui->radioButtonExchange->isChecked())
    {
        m_opType = Type::Exchange;
        m_showFlipCurrencyButton = 1;
        chargeType = tr("курс");
    }
    else
    {
        m_opType = Type::Move;
        m_showFlipCurrencyButton = 0;
        chargeType = tr("комиссия");
    }
    constructReasonPart();
    setWidgetsParams();
}

void tabCashMoveExch::setWidgetsParams()
{
    m_skipAutoLogRecord = 1;
    m_reasonRO = 1;
    ui->labelCharge->setText(chargeType);
    ui->doubleSpinBoxCharge->setValue(0);

    if(m_opType == Type::Exchange)
        ui->doubleSpinBoxCharge->setDecimals(4);
    else
        ui->doubleSpinBoxCharge->setDecimals(2);
    updateWidgets();

#ifdef QT_DEBUG
    if(test_scheduler_counter == RandomFillerStep::End)  // если заполнение случайными данными уже выполнено, а я решил изменить тип операции
    {
        test_scheduler_counter = RandomFillerStep::SrcAccount;
        randomFill();
    }
#endif
}

void tabCashMoveExch::dateChanged(QDate)
{
    if(ui->dateEdit->date() >= QDate::currentDate())
    {
        m_backdate = 0;
        ui->dateEdit->setDateTime(QDateTime::currentDateTime());
        return;
    }
    m_backdate = 1;
}

void tabCashMoveExch::buttonSaveClicked()
{
    commit();
}

void tabCashMoveExch::buttonSaveMoreClicked()
{
    if(!commit(1))
        return;

    shortlivedNotification *newPopup = new shortlivedNotification(this, tabTitle(), tr("Проведёно"), QColor(214,239,220), QColor(229,245,234));

    initCashRegisterModel();
    m_widgetsRO = 0;
    ui->doubleSpinBoxSrcAmount->setValue(0);   // чтобы избежать случайного дабл-клика нужно или выключать на несколько мсек кнопку или сбрасывать значение в 0
    ui->doubleSpinBoxDstAmount->setValue(0);   // чтобы избежать случайного дабл-клика нужно или выключать на несколько мсек кнопку или сбрасывать значение в 0

#ifdef QT_DEBUG
    test_scheduler_counter = RandomFillerStep::OpType;
    randomFill();
#endif
}

void tabCashMoveExch::autoReasonEnabled(int isAuto)
{
//    if(m_reasonManEdited)
        // TODO: если пользователь отредактировал примечание, а потом включил флаг "Автозаполонение основания", нужно предупредить о потере введённых данных
    m_reasonRO = isAuto;

    updateWidgets();
}

void tabCashMoveExch::reasonEdited(QString)
{
    m_reasonManEdited = 1;
}

void tabCashMoveExch::buttonPrintClicked()
{
    print();
}

void tabCashMoveExch::calculateDstAmount()
{
    double dstValue = 0;
    ui->doubleSpinBoxDstAmount->blockSignals(1);
    if(m_opType == Type::Exchange)
    {
        dstValue = ui->doubleSpinBoxSrcAmount->value()*ui->doubleSpinBoxCharge->value();
    }
    else
    {
        dstValue = ui->doubleSpinBoxSrcAmount->value() - ui->doubleSpinBoxCharge->value();
    }
    ui->doubleSpinBoxDstAmount->setValue(dstValue);
    cashRegisterDst->setAmount(dstValue);
    ui->doubleSpinBoxDstAmount->blockSignals(0);
}

/* Слот, вызываемый по сигналу spinBox'а
 * spinBox должен принимать только абсолютное знаение
*/
void tabCashMoveExch::srcAmountChanged(double amountAbs)
{
    m_srcAmount = -amountAbs;
    calculateDstAmount();
}

void tabCashMoveExch::dstAmountChanged(double amountAbs)
{
    m_dstAmount = amountAbs;
    ui->doubleSpinBoxCharge->blockSignals(1);
    if(m_opType == Type::Exchange)
    {
        ui->doubleSpinBoxCharge->setMinimum(0);
        ui->doubleSpinBoxCharge->setValue(ui->doubleSpinBoxDstAmount->value()/ui->doubleSpinBoxSrcAmount->value());
    }
    else
    {
        ui->doubleSpinBoxCharge->setMinimum(-ui->doubleSpinBoxCharge->maximum());
        ui->doubleSpinBoxCharge->setValue(ui->doubleSpinBoxSrcAmount->value() - ui->doubleSpinBoxDstAmount->value());
    }
    ui->doubleSpinBoxCharge->blockSignals(0);
    constructReasonPart();
    updateWidgets();
}

void tabCashMoveExch::chargeChanged(double)
{
    ui->doubleSpinBoxCharge->setMinimum(0);
    calculateDstAmount();
    constructReasonPart();
    updateWidgets();
}

void tabCashMoveExch::chargeChanged()
{
    currencyFlipped = 0;
    prevCurrencyRate = 0;
}

#ifdef QT_DEBUG
void tabCashMoveExch::randomFill()
{
    int i, j;

    if (test_scheduler_counter == RandomFillerStep::OpType)
    {
        i = QRandomGenerator::global()->bounded(100);
        if(i > 50)
            ui->radioButtonExchange->setChecked(1);
    }
    else if (test_scheduler_counter == RandomFillerStep::SrcAccount)
    {
        i = QRandomGenerator::global()->bounded(paymentSystemsProxyModel->rowCount());
        ui->comboBoxSrcPaymentAccount->setCurrentIndex(i);
    }
    else if (test_scheduler_counter == RandomFillerStep::DstAccount)
    {
        i = QRandomGenerator::global()->bounded(paymentSystemsProxyModel->rowCount());
        ui->comboBoxDstPaymentAccount->setCurrentIndex(i);
    }
    else if (test_scheduler_counter == RandomFillerStep::Amount)
    {
        i = QRandomGenerator::global()->bounded(20);
        ui->doubleSpinBoxSrcAmount->setValue(i*50);
        j = QRandomGenerator::global()->bounded(100);
        if(j > 50)  // заполняем поле с суммой назначения, поле комиссии или курса заполнится автоматически
        {
            i = QRandomGenerator::global()->bounded(20);
            if(m_opType == Type::Exchange)
            {
                ui->doubleSpinBoxDstAmount->setValue(i*50);
            }
            else
            {
                if(i > ui->doubleSpinBoxSrcAmount->value())
                {
                    i = QRandomGenerator::global()->bounded(ui->doubleSpinBoxSrcAmount->value());
                }
                ui->doubleSpinBoxDstAmount->setValue(i);
            }
        }
        else  // заполняем поле с комиссии или курса, поле с суммой назначения заполнится автоматически
        {
            if(m_opType == Type::Exchange)
            {
                i = QRandomGenerator::global()->bounded(5);
                ui->doubleSpinBoxCharge->setValue(40+i);
            }
            else
            {
                i = QRandomGenerator::global()->bounded(100);
                ui->doubleSpinBoxCharge->setValue(i);
            }

        }
        if(ui->doubleSpinBoxSrcAmount->value() == 0 || ui->doubleSpinBoxDstAmount->value() == 0 )   // если какое-то значение не заполнилось, то нужно повторить.
            test_scheduler_counter = RandomFillerStep::DstAccount;
    }

    if (test_scheduler_counter < RandomFillerStep::End)
    {
        test_scheduler_counter++;
        test_scheduler->start(200);    //  (пере-)запускаем таймер
    }
}

void tabCashMoveExch::test_scheduler_handler()
{
    randomFill();
}

void tabCashMoveExch::test_scheduler2_handler()
{
}
#endif

