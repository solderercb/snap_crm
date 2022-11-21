#include "global.h"
#include "tabcashoperation.h"
#include "ui_tabcashoperation.h"

QMap<int, tabCashOperation*> tabCashOperation::p_instance;

tabCashOperation::tabCashOperation(int order, MainWindow *parent) :
    tabCommon(parent),
    m_initialOrderId(order),
    m_orderId(order),
    ui(new Ui::tabCashOperation)
{
    bool nErr = 1;

    ui->setupUi(this);
    ui->labelOfLinkedObject->hide();
    ui->lineEditLinkedObjId->setReadOnly(true);
    ui->lineEditLinkedObjId->hide();
    ui->lineEditLinkedObjCaption->setReadOnly(true);
    ui->lineEditLinkedObjCaption->hide();
    ui->pushButtonOpenLinkedObject->hide();
    ui->lineClientId->setReadOnly(true);
    ui->lineEditBalance->setReadOnly(true);
    ui->checkBoxPrintCheck->setChecked(comSettings->value("print_check").toBool());
    ui->comboBoxCompany->setModel(companiesModel);
    ui->comboBoxCompany->setCurrentIndex(companiesModel->rowByDatabaseID(userDbData->value("company").toInt()));

    paymentSystemsProxyModel = new SSortFilterProxyModel();
    paymentSystemsProxyModel->setSourceModel(paymentSystemsModel);
    paymentSystemsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(" + QString::number(Global::PaymentSystemIds::Balance) + ")).*$"));
    paymentSystemsProxyModel->setFilterKeyColumn(1);
    ui->comboBoxPaymentAccount->setModel(paymentSystemsProxyModel);
    ui->comboBoxPaymentAccount->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(userDbData->value("defaultPaymentSystem", 0).toInt(), "system_id"));
    clientModel = new SClientModel();
    cashRegister = new SCashRegisterModel();
    initCashRegisterModel();

    connect(ui->lineEditClientLastName,SIGNAL(buttonClicked(int)),this,SLOT(lineEditClientLastNameButtonClickHandler(int)));
    connect(ui->lineEditClientLastName,SIGNAL(textEdited(QString)),ui->widgetClientMatch,SLOT(findByLastname(QString)));
    connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillClientCreds(int)));

    userActivityLog->appendRecord("Navigation " + tabTitle());
}

tabCashOperation *tabCashOperation::getInstance(int orderId, MainWindow *parent)
{
    if( !p_instance.contains(orderId) )
      p_instance.insert(orderId, new tabCashOperation(orderId, parent));
    return p_instance.value(orderId);
}

tabCashOperation::~tabCashOperation()
{
    userActivityLog->updateActivityTimestamp();

    delete ui;
    delete clientModel;
    delete cashRegister;
    delete paymentSystemsProxyModel;
    deleteLinkedObjects();
    p_instance.remove(m_orderId);
}

void tabCashOperation::initPKO()
{
    i_tabTitle = tr("Приходный кассовый ордер");
    paymentTypesModel = receiptTypesModel;
    ui->comboBoxOrderType->setModel(paymentTypesModel);
}

void tabCashOperation::initRKO()
{
    i_tabTitle = tr("Расходный кассовый ордер");
    paymentTypesModel = expenditureTypesModel;
    ui->comboBoxOrderType->setModel(paymentTypesModel);
    m_showCheckBoxPrint = 0;
}

void tabCashOperation::clearLinkedObjectFields()
{
    deleteLinkedObjects();
    m_linkedObjId = 0;
    m_linkedObjIdStr.clear();
    method = nullptr;
    ui->lineEditLinkedObjId->clear();
    ui->lineEditLinkedObjCaption->clear();
}

void tabCashOperation::deleteLinkedObjects()
{
    if(repair != nullptr)
    {
        delete repair;
        repair = nullptr;
    }
    if(document != nullptr)
    {
        delete document;
        document = nullptr;
    }
    if(invoice != nullptr)
    {
        delete invoice;
        invoice = nullptr;
    }
}

void tabCashOperation::showLinkedObject()
{
    bool state = 1;
    if(m_linkedObjType == LinkedObjectType::NoLink)
        state = 0;
    else if(m_linkedObjType == LinkedObjectType::Repair)
        ui->labelOfLinkedObject->setText(tr("Ремонт"));
    else if(m_linkedObjType == LinkedObjectType::Invoice)
        ui->labelOfLinkedObject->setText(tr("Счёт"));
    else
        ui->labelOfLinkedObject->setText(tr("Документ"));

    if(m_linkedObjId)
        ui->lineEditLinkedObjId->setText(QString::number(m_linkedObjId));
    else
        ui->lineEditLinkedObjId->clear();

    ui->labelOfLinkedObject->setVisible(state);
    ui->lineEditLinkedObjId->setVisible(state);
    ui->lineEditLinkedObjCaption->setVisible(state);
    ui->pushButtonOpenLinkedObject->setVisible(state);
}

void tabCashOperation::showClient()
{
    bool state = 1;
    if(m_showClient == Client::NotVisible)
        state = 0;
    else if(m_showClient == Client::Employee)
        ui->labelClient->setText(tr("Сотрудник"));
    else
        ui->labelClient->setText(tr("Плательщик"));

    ui->labelClient->setVisible(state);
    ui->lineClientId->setVisible(state);
    ui->lineEditClientLastName->setVisible(state);

    ui->labelBalance->setVisible(m_showBalance&state);
    ui->lineEditBalance->setVisible(m_showBalance&state);
}

void tabCashOperation::updateLineEditButtons(int state)
{
    if(state == ClientFieldsAccess::Denied)
    {
        if(permissions->value("Х")) // TODO: разрешение на просмотр карточки клиента
            ui->lineEditClientLastName->setButtons("Open");
        else
            ui->lineEditClientLastName->clearButtons();
        ui->lineEditClientLastName->setReadOnly(true);
    }
    else if(state == ClientFieldsAccess::SelNClr)
    {
        if(m_client)
            ui->lineEditClientLastName->setButtons("Edit,Clear");
        else
            ui->lineEditClientLastName->setButtons("Edit");
        ui->lineEditClientLastName->setReadOnly(true);
    }
    else
    {
        if(m_client)
            ui->lineEditClientLastName->setButtons("Clear");
        else
            ui->lineEditClientLastName->setButtons("Edit");
        ui->lineEditClientLastName->setReadOnly(false);
    }
    ui->lineEditClientLastName->updateGeometry();
}

void tabCashOperation::setAmountReadOnly(bool state)
{
    ui->doubleSpinBoxAmount->setReadOnly(state);
}

void tabCashOperation::setDefaultStylesheets()
{
    ui->lineEditLinkedObjCaption->setStyleSheet(commonLineEditStyleSheet);
    ui->doubleSpinBoxAmount->setStyleSheet(commonSpinBoxStyleSheet);
    ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
}

bool tabCashOperation::commit(bool repeatAfter)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nErr = 1;
    int initial_order_id = m_orderId;

    QUERY_LOG_START(metaObject()->className());

    if(!checkInput())
        return 0;

    cashRegister->setOperationType(m_orderType);
    cashRegister->setCreated(ui->dateEdit->dateTime());
    cashRegister->setSystemId(paymentSystemsProxyModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id"));
    cashRegister->setReason(ui->lineEditReason->text());    // если пользователь отредактировал автоматически сгенерированный комментарий
    cashRegister->setSkipLogRecording(m_skipAutoLogRecord);
    if(m_client)
        cashRegister->setClient(m_client);
    if(method != nullptr)
    {
        (cashRegister->*method)(m_linkedObjId);
    }

    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        cashRegister->commit();
        m_orderId = cashRegister->id();
        switch(m_orderType)
        {
            case SCashRegisterModel::RecptSimple:
            case SCashRegisterModel::ExpSimple: commitSimple(); break;
            case SCashRegisterModel::RecptBalance:
            case SCashRegisterModel::ExpBalance: commitBalance(m_amount); break;
            case SCashRegisterModel::RecptInvoice: commitInvoice(); break;
            case SCashRegisterModel::RecptGoods: commitDocument(); break;
            case SCashRegisterModel::RecptPrepayRepair: commitPrepayRepair(); break;
//            case SCashRegisterModel::ExpRevert: commitRevert(); break;    // см. комметарии к методу
        }

#ifdef QT_DEBUG
//        throw 0; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif

        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (int type)
    {
        nErr = 0;
        cashRegister->setId(0);
        m_orderId = initial_order_id;
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
            p_instance.remove(initial_order_id);   // Если всё ОК, то нужно заменить указатель
            p_instance.insert(m_orderId, this);    // иначе будет падать при попытке создать новую вкладку

            m_orderTypeRO = 1;
            m_showCheckBoxPrint = 0;
            ui->widgetClientMatch->hide();
            updateWidgets();
        }

        if(ui->checkBoxPrintCheck->isChecked())
            print();
    }
    return nErr;
}

bool tabCashOperation::commitSimple()
{
    return 1;
}

bool tabCashOperation::commitBalance(float amount)
{
    QString note;
    bool nErr = 1;
    if(amount > 0)
    {
        note = tr("Баланс клиента №%1 пополнен на %2").arg(m_client).arg(sysLocale.toCurrencyString(amount));
    }
    else
    {
        note = tr("С баланса клиента №%1 списано %2").arg(m_client).arg(sysLocale.toCurrencyString(-amount));
    }

    nErr = clientModel->updateBalance(amount, note);

    return nErr;
}

/*  Дополнительные действия при проведении операций, связанных с каким-либо документом (РН или ПН)
*/
bool tabCashOperation::commitDocument()
{
    if(document->type() == SDocumentModel::OutInvoice)
        document->setOrderId(m_orderId); // Для РН сумма автозаполняется, поэтому нужно лишь записать номер операции.
    else if(document->type() == SDocumentModel::InInvoice)
    {
        // TODO: Для ПН возможна оплата частями и нужно следить, когда сумма всех платежей будет больше равна суммы накладной
    }
    return document->commit();
}

/*  Дополнительные действия для ПКО оплаты счёта
*/
bool tabCashOperation::commitInvoice()
{
    invoice->setPaid(QDateTime::currentDateTime());
    invoice->setState(SInvoiceModel::Payed);
    return invoice->commit();
}

/*  Дополнительные действия для ПКО предоплаты ремонта
*/
bool tabCashOperation::commitPrepayRepair()
{
    repair->addPrepay(m_amount);
    return repair->commit();
}

/*  Дополнительные действия для возвратного РКО
 *  TODO: В АСЦ v3.7.31.1123 при проведении возвратного РКО соответствующий ПКО не помечается; возможно, это стоит делать (для противодействия тупости или даже умышленного вредительства со стороны пользователя)
 *  Также при этом не производится никаких действий с ремонтом/расходной накладной/счётом, если производится возврат средств, ранее уплоченных по ним.
*/
bool tabCashOperation::commitRevert()
{

}

void tabCashOperation::print()
{
    QMap<QString, QVariant> report_vars;
    report_vars.insert("type", "pko");
    report_vars.insert("record_id", m_orderId);
    emit generatePrintout(report_vars);
}

void tabCashOperation::setAmount(const float amount)
{
    float l_amount = amount;
    if(l_amount < 0)
        l_amount = -l_amount;
    ui->doubleSpinBoxAmount->setValue(l_amount);
    amountChanged(amount);
}

void tabCashOperation::updateOrderIdLineEdit()
{
    ui->lineEditOrderNum->setText(QString::number(cashRegister->id()));
    ui->lineEditOrderNum->setStyleSheet("QLineEdit { background: #90EE90;}");   // светло-зелёный
}

QString tabCashOperation::tabTitle()
{
    if(m_orderId > 0)
        return i_tabTitle + QString(" %1").arg(m_orderId);

    return i_tabTitle;
}

void tabCashOperation::prepareTemplate(QMap<int, QVariant> data)
{
    if(data.isEmpty())
    {
#ifdef QT_DEBUG
        if(m_orderId < 0)
            test_scheduler->start(200);
#endif
        return;
    }

    m_orderTypeRO = 1;
    QMap<int, QVariant>::const_iterator i = data.constBegin();
    while(i != data.constEnd())
    {
        switch(i.key())
        {   // QMap сортирует данные по ключу, поэтому вызов методов будет последовательным
            case TmplType: ui->comboBoxOrderType->setCurrentIndex(paymentTypesModel->rowByDatabaseID(i.value().toInt())); break;
            case TmplObjId: fillLinkedObj(i.value().toInt()); break;
            case TmplClient: fillClientCreds(i.value().toInt()); break;
            case TmplAmount: ui->doubleSpinBoxAmount->setValue(i.value().toDouble()); break;
        }
        ++i;
    }
}

void tabCashOperation::updateWidgets()
{
    setDefaultStylesheets();
    if(m_orderId > 0)
    {
        ui->lineEditDate->setVisible(true);
        ui->lineEditDate->setText(cashRegister->created());
        ui->dateEdit->setVisible(false);
        ui->buttonPrint->show();
        ui->buttonRevert->setVisible(m_buttonRevertVisible);
        ui->buttonSave->hide();
        ui->buttonSaveMore->hide();
        updateOrderIdLineEdit();
        ui->checkBoxAutoReason->setDisabled(true);
        ui->lineEditReason->setReadOnly(true);
        ui->pushButtonOpenLinkedObject->setText(tr("Открыть"));
        ui->comboBoxCompany->setEnabled(false);
        setAmountReadOnly();
        m_clientRO = ClientFieldsAccess::Denied;
        // TODO: разрешение редактировать платёжную систему в проведённом кассовом ордере
//        if(!permissions->value("Х"))
//            ui->comboBoxPaymentAccount->setEnabled(false);
    }
    else
    {
        ui->toolButtonApplyPaymentSystem->hide();
        ui->lineEditDate->setVisible(false);
        ui->dateEdit->setVisible(permissions->value("71"));    // Проводить документы задним числом
        ui->dateEdit->setDate(QDate::currentDate());
        ui->buttonSave->show();
        ui->buttonSaveMore->show();
        ui->buttonPrint->hide();
        ui->buttonRevert->hide();
        ui->checkBoxAutoReason->setEnabled(true);
        ui->lineEditReason->setReadOnly(m_reasonRO);
        m_reason = cashRegister->constructReason(m_linkedObjIdStr);
        ui->lineEditReason->setText(m_reason);
        setAmountReadOnly(m_amountRO);
    }

    ui->checkBoxPrintCheck->setVisible(m_showCheckBoxPrint);
    ui->comboBoxOrderType->setEnabled(!m_orderTypeRO);
    ui->comboBoxPaymentAccount->setEnabled(!m_paymentAccountRO);

    if(m_showBalance)
        ui->lineEditBalance->setText(sysLocale.toString(clientModel->balance(), 'f', 2));
    else
        ui->lineEditBalance->clear();

    ui->toolButtonApplyPaymentSystem->hide();
    showLinkedObject();
    showClient();
    updateLineEditButtons(m_clientRO);
}

bool tabCashOperation::checkInput()
{
    int error = 0;

    setDefaultStylesheets();
    if(ui->doubleSpinBoxAmount->value() == 0)
    {
        error |= 1<<0;
        ui->doubleSpinBoxAmount->setStyleSheet(commonSpinBoxStyleSheetRed);
    }
    if(m_linkedObjType != LinkedObjectType::NoLink)
    {
        if(m_linkedObjId == 0)
        {
            error |= 1<<1;
            ui->lineEditLinkedObjCaption->setStyleSheet(commonLineEditStyleSheetRed);
        }
    }
    // TODO: В АСЦ v3.7.31.1123 только ПКО может быть проведён на анонимного клиента; возможно, это неправильно
    if(m_showClient != Client::NotVisible && (m_orderType != SCashRegisterModel::RecptSimple))
    {
        if( m_client == 0 && m_employee == 0 )
        {
            error |= 1<<2;
            ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheetRed);
        }
    }
    if(ui->comboBoxPaymentAccount->currentIndex() == -1)
    {
        error |= 1<<3;
        ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheetRed);
    }
    if(!m_reasonRO && ui->lineEditReason->text().isEmpty())
    {
        error |= 1<<4;
        ui->lineEditReason->setStyleSheet(commonLineEditStyleSheetRed);
    }
    if(m_client && ((m_orderType == SCashRegisterModel::RecptBalance) || (m_orderType == SCashRegisterModel::ExpBalance)) )
    {
        if(!clientModel->balanceEnabled())
        {
            bool tmp = permissions->value("11");    // Редактировать существующих клиентов
            if(tmp)
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Баланс клиента"));
                msgBox.setText(tr("Баланс клиента не включен. Включить?"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::Yes);

                if(msgBox.exec() == QMessageBox::Yes)
                {
                    tmp = clientModel->setBalanceEnabled();
                }
                else
                    tmp = 0;

                m_showBalance = tmp;
            }
            if(!tmp)
            {
                error |= 1<<5;
                ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheetRed);
            }
        }
    }

    if (error)
    {
        qDebug() << "Ошибка создания кассового ордера: возможно, не все обязательные поля заполнены (error " << error << ")";
        appLog->appendRecord(QString("Error while creating cash register record: probably some obligatory fields empty (error %1)").arg(error));
        return 0;
    }
    return 1;
}

void tabCashOperation::load(const int orderId)
{
    cashRegister->load(orderId);
    m_showCheckBoxPrint = 0;
    m_orderTypeRO = 1;
    m_orderId = orderId;
    m_orderType = cashRegister->operationType();
    m_amount = cashRegister->amount();
    m_buttonRevertVisible = 0;
    if(m_amount >= 0)
    {
        if(cashRegister->systemId() == 0)   // TODO: в АСЦ возврат доступен только для ПКО в кассу Наличные, возможно, стоит это доработать
            m_buttonRevertVisible = 1;
        initPKO();
    }
    else
    {
        initRKO();
    }
    ui->comboBoxPaymentAccount->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(cashRegister->systemId(), "system_id"));
    ui->comboBoxOrderType->setCurrentIndex(paymentTypesModel->rowByDatabaseID(m_orderType));
    switch(m_linkedObjType)
    {
        case LinkedObjectType::Document: fillDocumentCreds(cashRegister->documentId()); break;
        case LinkedObjectType::Repair: fillRepairCreds(cashRegister->repairId()); break;
        case LinkedObjectType::Invoice: fillInvoiceCreds(cashRegister->invoiceId()); break;
        default: fillClientCreds(cashRegister->client());
    }
    ui->doubleSpinBoxAmount->setValue(cashRegister->amountAbs());
    ui->lineEditReason->setText(cashRegister->reason());
    updateOrderIdLineEdit();
}

void tabCashOperation::initCashRegisterModel()
{
    cashRegister->setId(0);

    switch (m_orderId) {
        case tabCashOperation::PKO: initPKO(); break;
        case tabCashOperation::RKO: initRKO(); break;
        default: load(m_orderId);
    }
}

void tabCashOperation::clearClientCreds(bool)
{
    m_client = 0;
    clientModel->clear();
    m_showBalance = 0;
    ui->lineClientId->clear();
    ui->lineEditClientLastName->clear();
    m_clientRO = ClientFieldsAccess::Full;
    cashRegister->unsetClient();
    updateWidgets();
}

void tabCashOperation::fillClientCreds(int clientId)
{
    clearClientCreds(false);
    m_client = clientId;
    clientModel->load(clientId);
    m_showBalance = clientModel->balanceEnabled();
    ui->lineClientId->setText(QString::number(clientId));
    ui->lineEditClientLastName->setText(clientModel->fullShortName());
    m_clientRO = ClientFieldsAccess::SelNClr;
    cashRegister->setClient(m_client);
    updateWidgets();
    ui->lineEditClientLastName->setFocus();
}

void tabCashOperation::fillRepairCreds(int repairId)
{
    method = &SCashRegisterModel::setRepairId;
    m_linkedObjId = repairId;
    m_linkedObjIdStr = QString::number(repairId);
    repair = new SRepairModel();
    repair->load(m_linkedObjId);
    fillClientCreds(repair->clientId());
    ui->lineEditLinkedObjCaption->setText(repair->title());
    ui->doubleSpinBoxAmount->setValue(repair->realRepairCost());
    updateWidgets();
}

void tabCashOperation::fillDocumentCreds(int documentId)
{
    method = &SCashRegisterModel::setDocumentId;
    document = new SDocumentModel();
    document->load(documentId);
    fillClientCreds(document->client());
    m_linkedObjId = documentId;
    m_linkedObjIdStr = QString::number(documentId);

    ui->lineEditLinkedObjCaption->setText(document->title());
    setAmount(document->amount());
    updateWidgets();
}

void tabCashOperation::fillInvoiceCreds(int invoiceId)
{
    method = &SCashRegisterModel::setInvoiceId;
    invoice = new SInvoiceModel();
    invoice->load(invoiceId);
    m_linkedObjId = invoiceId;
    m_linkedObjIdStr = invoice->num();
    ui->lineEditLinkedObjCaption->setText(m_linkedObjIdStr);
    ui->lineEditClientLastName->setText(invoice->covenantorName());
    m_client = invoice->clientId();
    setAmount(invoice->summ());
    updateWidgets();
}

void tabCashOperation::fillLinkedObj(int objId)
{
    switch (m_linkedObjType)
    {
        case LinkedObjectType::Document: fillDocumentCreds(objId); break;
        case LinkedObjectType::Repair: fillRepairCreds(objId); break;
        case LinkedObjectType::Invoice: fillInvoiceCreds(objId); break;
    }
}

void tabCashOperation::orderTypeChanged(int index)
{
    if(m_orderId < 0)
    {
        m_orderType = paymentTypesModel->databaseIDByRow(index);
        cashRegister->setOperationType(m_orderType);
    }
    setWidgetsParams();
}

void tabCashOperation::setWidgetsParams()
{
    m_linkedObjType = LinkedObjectType::NoLink;
    m_showClient = Client::Client;
    m_clientRO = ClientFieldsAccess::Full;
    m_amountRO = false;
    m_showBalance = 0;
    m_skipAutoLogRecord = 0;
    m_reasonRO = 1;
    clearLinkedObjectFields();
    // TODO: В АСЦ v3.7.31.1123 в таблице БД docs нет метки, что товар продан в долг.
    //       Наличие типа кассовой операции "Поступление денег в счет продажи товаров (РН)" может привести к
    //       накоплению на балансе клиента долга за товары, которые по факту были оплачены.
    //       Нужно добавить столбец с id записи в таблице balance и при проведении такой операции производить манипуляции с балансом.
    //       Это же касается ремонта, выданного в долг.
    //       Отметка об этой проблеме также оставлена в SPaymentTypesModel::SPaymentTypesModel(const int, QObject*)
    switch (m_orderType)
    {
    case SCashRegisterModel::ExpBalance:
    case SCashRegisterModel::RecptBalance: m_showBalance = 1; m_skipAutoLogRecord = 1; break;                                /* нет полей связанного объекта; поле с балансом */
    case SCashRegisterModel::ExpInvoice:   m_linkedObjType = LinkedObjectType::Document; m_clientRO = ClientFieldsAccess::SelNClr; break; /* выбор ПН; поля клиента RO; сумма редактируемая для "оплаты часятями" */
    case SCashRegisterModel::RecptGoods:   m_linkedObjType = LinkedObjectType::Document; m_clientRO = ClientFieldsAccess::Denied; m_amountRO = true; break;  /* выбор РН; поля клиента RO */
    case SCashRegisterModel::ExpZ:         m_showClient = Client::NotVisible; m_skipAutoLogRecord = 1; break;                /* нет полей связанного объекта; нет полей клиента */
    case SCashRegisterModel::RecptRepair:  m_amountRO = true;                                       /* те же режимы, что и для предоплаты, но сумма не редактируемая */
    case SCashRegisterModel::RecptPrepayRepair: m_linkedObjType = LinkedObjectType::Repair; m_clientRO = ClientFieldsAccess::Denied; m_skipAutoLogRecord = 1; break;    /* выбор ремонта; поля клиента RO */
    case SCashRegisterModel::RecptInvoice: m_linkedObjType = LinkedObjectType::Invoice; m_clientRO = ClientFieldsAccess::Denied; m_amountRO = true; m_skipAutoLogRecord = 1; break;   /* выбор счета; поля клиента RO */
    case SCashRegisterModel::RecptSimple:
    case SCashRegisterModel::ExpSimple:    m_skipAutoLogRecord = 1; break;
    case SCashRegisterModel::ExpSubsist:
    case SCashRegisterModel::ExpSalary:    m_showClient = Client::Employee; break;                  /* только просмотр; в поле клиента пишем данные сотрудника */
    case SCashRegisterModel::AddSubCash:   m_showClient = Client::NotVisible; break;                /* только просмотр; нет полей клиента */
    case SCashRegisterModel::ExpRepair:    m_linkedObjType = LinkedObjectType::Repair; break;       /* только просмотр */
    case SCashRegisterModel::ExpGoods:     m_linkedObjType = LinkedObjectType::Document; break;     /* только просмотр */
    case SCashRegisterModel::ExpDealer:    m_clientRO = ClientFieldsAccess::Denied; break;          /* только просмотр */
    case SCashRegisterModel::ExpRevert:    m_paymentAccountRO = 1; m_skipAutoLogRecord = 1; break;
//    case SCashRegisterModel::MoveCash:     m_showClient = Client::NotVisible; break;              /* TODO: */
    }
    if(m_orderType > SCashRegisterModel::ExpCustom)
    {
        m_skipAutoLogRecord = 1;
        fillClientCreds(expenditureTypesModel->clientByDatabaseId(m_orderType));
        ui->doubleSpinBoxAmount->setValue(expenditureTypesModel->defaultAmountByDatabaseId(m_orderType));
        ui->comboBoxPaymentAccount->setCurrentIndex(paymentSystemsProxyModel->rowByDatabaseID(expenditureTypesModel->paymentSystemByDatabaseId(m_orderType), "system_id"));
    }

    if(m_showClient == Client::NotVisible)
        clearClientCreds(0);

    updateWidgets();

#ifdef QT_DEBUG
    if(test_scheduler_counter == RandomFillerStep::End)  // если заполнение случайными данными уже выполнено, а я решил изменить тип операции
    {
        test_scheduler_counter = RandomFillerStep::LinkedObj;
        randomFill();
    }
#endif
}

void tabCashOperation::dateChanged(QDate)
{
    if(ui->dateEdit->date() >= QDate::currentDate())
    {
        m_backdate = 0;
        ui->dateEdit->setDateTime(QDateTime::currentDateTime());
        return;
    }
    m_backdate = 1;
}

void tabCashOperation::findClientByLastname(QString)
{

}

void tabCashOperation::buttonOpenDocClicked()
{
    if(m_orderId > 0)
    {
        switch(m_linkedObjType)
        {
            case LinkedObjectType::Repair: emit createTabOpenRepair(cashRegister->repairId()); break;
            case LinkedObjectType::Document: emit createTabOpenDocument(cashRegister->documentId()); break;
            case LinkedObjectType::Invoice: emit createTabOpenInvoice(cashRegister->invoiceId());
        }
    }
    else
    {
        switch(m_linkedObjType)
        {
            // TODO: создавать вкладку выбора связанного объекта нужно с предустановленным фильтром
            case LinkedObjectType::Repair: emit createTabSelectRepair(1, this); break;
            case LinkedObjectType::Document: emit createTabSelectDocument(1, this); break;
            case LinkedObjectType::Invoice: emit createTabSelectInvoice(1, this);
        }
        clearLinkedObjectFields();
    }
}

void tabCashOperation::buttonSaveClicked()
{
    commit();
}

void tabCashOperation::buttonSaveMoreClicked()
{
    if(!commit(1))
        return;

    shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Проведён"), tabTitle(), QColor(214,239,220), QColor(229,245,234));
    m_orderId = m_initialOrderId;
//    ui->comboBoxOrderType->setCurrentIndex(-1);
    clearLinkedObjectFields();

    initCashRegisterModel();
    cashRegister->setOperationType(m_orderType);    // тип операции тот же
    ui->doubleSpinBoxAmount->setValue(0);   // чтобы избежать случайного дабл-клика нужно или выключать на несколько мсек кнопку или сбрасывать значение в 0

#ifdef QT_DEBUG
    test_scheduler_counter = RandomFillerStep::OpType;
    randomFill();
#endif
}

void tabCashOperation::autoReasonEnabled(int isAuto)
{
//    if(m_reasonManEdited)
        // TODO: если пользователь отредактировал примечание, а потом включил флаг "Автозаполонение основания", нужно предупредить о потере введённых данных
    m_reasonRO = isAuto;

    updateWidgets();
}

void tabCashOperation::reasonEdited(QString)
{
    m_reasonManEdited = 1;
}

void tabCashOperation::paymentSystemChanged(int index)
{
    if(m_orderId > 0)
        ui->toolButtonApplyPaymentSystem->show();
    if(cashRegister)
        cashRegister->setSystemId(paymentSystemsProxyModel->databaseIDByRow(index, "system_id"));
}

void tabCashOperation::lineEditClientLastNameButtonClickHandler(int buttonId)
{
    switch (buttonId)
    {
        case SLineEdit::Clear: clearClientCreds(0); break;
        case SLineEdit::Edit: emit createTabSelectExistingClient(1, this); break;
        case SLineEdit::Open: emit createTabClient(m_client); break;
    }
}

void tabCashOperation::buttonPrintClicked()
{
    print();
}

/* Создание новой вкладки возвратного РКО
*/
void tabCashOperation::buttonRevertClicked()
{
    QMap<int, QVariant> data;
    data.insert(TmplType, SCashRegisterModel::ExpRevert);
    if(m_client)
        data.insert(TmplClient, m_client);
    data.insert(TmplAmount, m_amount);

    emit createTabUndoOperation(tabCashOperation::RKO, data);
}

/* Слот, вызываемый по сигналу spinBox'а
 * spinBox должен принимать только абсолютное знаение
*/
void tabCashOperation::amountChanged(double amountAbs)
{
    if(m_orderId > 0)
        return;

    m_amount = amountAbs;
    if(m_orderId == Type::RKO)
        m_amount = -m_amount;
    cashRegister->setAmount(m_amount);
    if(ui->lineEditReason->isReadOnly())
    {
        m_reason = cashRegister->constructReason(m_linkedObjIdStr);
        ui->lineEditReason->setText(m_reason);
    }
}

void tabCashOperation::applyPaymentSystem()
{
    QSqlQuery *query;
    bool nErr = 1;
    query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QUERY_EXEC(query,nErr)(QUERY_BEGIN);
    nErr = cashRegister->commit();
    QUERY_COMMIT_ROLLBACK(query,nErr);
    if(nErr)
        ui->toolButtonApplyPaymentSystem->hide();

    delete query;
}

#ifdef QT_DEBUG
void tabCashOperation::randomFill()
{
    int i;

    if (test_scheduler_counter == RandomFillerStep::OpType)
    {
        i = QRandomGenerator::global()->bounded(paymentTypesModel->rowCount());
        ui->comboBoxOrderType->setCurrentIndex(i);
    }
    else if (test_scheduler_counter == RandomFillerStep::LinkedObj)
    {
        switch (m_linkedObjType)
        {
            case LinkedObjectType::Document: dbgRandomDocument(m_orderId); break;
            case LinkedObjectType::Repair: dbgRandomRepair(); break;
            case LinkedObjectType::Invoice: dbgRandomInvoice(); break;
        }
    }
    else if (test_scheduler_counter == RandomFillerStep::RClient)
    {
        if(m_linkedObjType == LinkedObjectType::NoLink)   // только если нет связанного объекта
        {
//            dbgRandomClient(m_showBalance);
            dbgRandomClient(QRandomGenerator::global()->bounded(2));
        }
    }
    else if (test_scheduler_counter == RandomFillerStep::Amount)
    {
        if(!m_amountRO)
        {
            i = QRandomGenerator::global()->bounded(20);
            setAmount(i*50);
        }
    }
    else if (test_scheduler_counter == RandomFillerStep::PaymentSys)
    {
        i = QRandomGenerator::global()->bounded(paymentSystemsProxyModel->rowCount());
        ui->comboBoxPaymentAccount->setCurrentIndex(i);
    }

    if (test_scheduler_counter < RandomFillerStep::End)
    {
        test_scheduler_counter++;
        test_scheduler->start(200);    //  (пере-)запускаем таймер
    }
}

void tabCashOperation::dbgRandomRepair()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));

    for(int j = 0; j < 3; j++)
    {
        if(m_orderType == SCashRegisterModel::RecptPrepayRepair)
            query->exec(QString("SELECT `id` FROM (SELECT ROUND((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM workshop AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.state IN (0,1,2,3,4,5,6,13,14,15,31,32,33,34,35)) AS t1 ON t1.`num` = `rand`.`rand`;"));
        else
            query->exec(QString("SELECT `id` FROM (SELECT ROUND((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM workshop AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.state IN (6,7,8,12,16)) AS t1 ON t1.`num` = `rand`.`rand`;"));
        if(j<2)   // для mysql 5.6.51 (win) результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их
            continue;

        query->first();
        if(query->isValid())
            fillRepairCreds(query->record().value(0).toInt());
    }
    delete query;
}

void tabCashOperation::dbgRandomDocument(int type)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));

    for(int j = 0; j < 3; j++)
    {
        query->exec(QString("SELECT `id` FROM (SELECT ROUND((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM docs AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.`type` IN (%1) AND t1.`order_id` IS NULL) AS t1 ON t1.`num` = `rand`.`rand`;").arg(-type));
        if(j<2)   // для mysql 5.6.51 (win) результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их
            continue;

        query->first();
        if(query->isValid())
            fillDocumentCreds(query->record().value(0).toInt());
    }
    delete query;
}

void tabCashOperation::dbgRandomInvoice()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));

    for(int j = 0; j < 3; j++)
    {
        query->exec(QString("SELECT `id` FROM (SELECT TRUNCATE((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM invoice AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.state IN (1)) AS t1 ON t1.`num` = `rand`.`rand`;"));
        if(j<2)   // для mysql 5.6.51 (win) результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их
            continue;

        query->first();
        if(query->isValid())
            fillInvoiceCreds(query->record().value(0).toInt());
    }
    delete query;
}

void tabCashOperation::dbgRandomClient(bool balanceEnabled)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));

    for(int j = 0; j < 3; j++)
    {
        query->exec(QString("SELECT `id` FROM (SELECT ROUND((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM clients AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.`balance_enable` IN (%1) AND t1.`state` = 1 AND t1.`type` = 0) AS t1 ON t1.`num` = `rand`.`rand`;").arg(balanceEnabled));
        if(j<2)   // для mysql 5.6.51 (win) результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их
            continue;

        query->first();
        if(query->isValid())
            fillClientCreds(query->record().value(0).toInt());
    }
    delete query;
}

void tabCashOperation::test_scheduler_handler()
{
    randomFill();
}

void tabCashOperation::test_scheduler2_handler()
{
}
#endif

