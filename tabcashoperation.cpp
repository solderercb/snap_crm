#include "global.h"
#include "tabcashoperation.h"
#include "ui_tabcashoperation.h"
#include "models/sofficemodel.h"

QMap<int, tabCashOperation*> tabCashOperation::p_instance;

tabCashOperation::tabCashOperation(int order, MainWindow *parent) :
    tabCommon(parent),
    m_initialOrderId(order),
    m_orderId(order),
    ui(new Ui::tabCashOperation)
{
    logUserActivity();

    initModels();
    initWidgets();

    ;
    // order может быть инициализирован отрицательными значениями в случаях создания новых ордеров
    switch (order)
    {
        case 0:
        case tabCashOperation::PKO: initPKO(); break;
        case tabCashOperation::RKO: initRKO(); break;
        default: load(order);
    }
}

void tabCashOperation::initModels()
{
    m_paymentAccountsProxyModel = new SSortFilterProxyModel();
    m_paymentAccountsProxyModel->setSourceModel(paymentSystemsModel);
    m_paymentAccountsProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(" + QString::number(Global::PaymentSystemIds::Balance) + ")).*$"));
    m_paymentAccountsProxyModel->setFilterKeyColumn(1);

    m_clientModel = new SClientModel();
    connect(m_clientModel, &SClientModel::modelUpdated, this, &tabCashOperation::clientModelUpdated);
    m_cashRegisterModel = new SCashRegisterModel();
    initCashRegisterModel();
}

void tabCashOperation::initWidgets()
{
    ui->setupUi(this);

    tabCashOperation::guiFontChanged();
    ui->labelOfLinkedObject->hide();
    ui->lineEditLinkedObjId->setReadOnly(true);
    ui->lineEditLinkedObjId->hide();
    ui->lineEditLinkedObjCaption->enableAutoSetCursorPositionToBegin(true);
    ui->lineEditLinkedObjCaption->setReadOnly(true);
    ui->lineEditLinkedObjCaption->hide();
    ui->pushButtonOpenLinkedObject->hide();
    ui->checkBoxPrintCheck->setChecked(comSettings->printCheck);
    ui->comboBoxCompany->setModel(companiesModel);
    if(companiesModel->rowCount() == 1)
    {
        ui->comboBoxCompany->setVisible(false); // в АСЦ это поле остаётся видимым даже при одной компании и офисе
        ui->labelCompany->setVisible(false);
    }

    if(permissions->createBackdatedDocuments)
        ui->dateEdit->setAllowBackDate(true);
    ui->dateEdit->setDateTrackingEnabled(true);

    ui->comboBoxPaymentAccount->setModel(m_paymentAccountsProxyModel);
    setPaymentAccountIndex(userDbData->defaultPaymentSystem);

    ui->widgetClientInput->setClientModel(m_clientModel);
    ui->widgetClientInput->setLayoutRefWidget(0, ui->labelDate);
    ui->widgetClientInput->setLayoutRefWidget(1, ui->doubleSpinBoxAmount);
    ui->widgetClientInput->setClientsTabDefaultCategory(SClientModel::Categories::Regulars);

    if(m_orderId == tabCashOperation::RKO)  // Независимо от настроек сумма может быть с копейками
        ui->doubleSpinBoxAmount->setDecimals(2);

    connect(ui->pushButtonOpenLinkedObject, &QPushButton::clicked, this, &tabCashOperation::buttonOpenDocClicked);
    connect(ui->comboBoxPaymentAccount, qOverload<int>(&QComboBox::currentIndexChanged), this, &tabCashOperation::paymentSystemChanged);
    connect(ui->comboBoxOperationType, qOverload<int>(&QComboBox::currentIndexChanged), this, &tabCashOperation::operationTypeChanged);
    connect(ui->checkBoxAutoReason, &QCheckBox::stateChanged, this, &tabCashOperation::autoReasonEnabled);
    connect(ui->doubleSpinBoxAmount, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &tabCashOperation::amountChanged);
    connect(ui->lineEditReason, &QLineEdit::textEdited, this, &tabCashOperation::reasonEdited);
    connect(ui->toolButtonApplyPaymentSystem, &QToolButton::clicked, this, &tabCashOperation::applyPaymentSystem);
    connect(ui->buttonSave, &QPushButton::clicked, this, &tabCashOperation::buttonSaveClicked);
    connect(ui->buttonSaveMore, &QPushButton::clicked, this, &tabCashOperation::buttonSaveMoreClicked);
    connect(ui->buttonPrint, &QPushButton::clicked, this, &tabCashOperation::buttonPrintClicked);
    connect(ui->buttonRevert, &QPushButton::clicked, this, &tabCashOperation::buttonRevertClicked);
}

tabCashOperation *tabCashOperation::getInstance(int orderId, MainWindow *parent)
{
    if( !p_instance.contains(orderId) )
      p_instance.insert(orderId, new tabCashOperation(orderId, parent));
    return p_instance.value(orderId);
}

tabCashOperation::~tabCashOperation()
{
    delete ui;
    delete m_clientModel;
    delete m_cashRegisterModel;
    delete m_paymentAccountsProxyModel;
    deleteLinkObject();
    p_instance.remove(m_orderId);
}

void tabCashOperation::initPKO()
{
    m_companyLabel = tr("Получатель платежа");
    m_widgetClientLabel = tr("Плательщик");
    m_checkBoxPrintVisible = 1;
    setOrderTypeModel(receiptTypesModel);
}

void tabCashOperation::initRKO()
{
    m_companyLabel = tr("Организация-плательщик");
    m_widgetClientLabel = tr("Получатель платежа");
    // TODO: прокси-модель для исключения операций Возвратный РКО, выплата ЗП и др. Прокси модель только для новых.
    m_checkBoxPrintVisible = 0;
    setOrderTypeModel(expenditureTypesModel);
}

void tabCashOperation::setOrderTypeModel(SPaymentTypesModel *model)
{
    bool prevState = ui->comboBoxOperationType->signalsBlocked();

    if(m_orderId > 0)
        ui->comboBoxOperationType->blockSignals(true);

    m_operationTypesModel = model;
    ui->comboBoxOperationType->setModel(model);

    if(m_orderId > 0)
    {
        ui->comboBoxOperationType->setCurrentIndex(-1);
        ui->comboBoxOperationType->blockSignals(prevState);
    }
}

void tabCashOperation::deleteLinkObject()
{
    if(m_repairModel != nullptr)
    {
        delete m_repairModel;
        m_repairModel = nullptr;
    }
    if(m_documentModel != nullptr)
    {
        delete m_documentModel;
        m_documentModel = nullptr;
    }
    if(m_invoiceModel != nullptr)
    {
        delete m_invoiceModel;
        m_invoiceModel = nullptr;
    }
}

void tabCashOperation::updateLinkWidgets()
{
    bool state = 1;
    if(m_linkType == LinkType::NoLink)
        state = 0;
    else if(m_linkType == LinkType::Repair)
        ui->labelOfLinkedObject->setText(tr("Ремонт"));
    else if(m_linkType == LinkType::Invoice)
        ui->labelOfLinkedObject->setText(tr("Счёт"));
    else
        ui->labelOfLinkedObject->setText(tr("Документ"));

    if(!m_linkId.isEmpty())
        ui->lineEditLinkedObjId->setText(m_linkId);
    else
        ui->lineEditLinkedObjId->clear();

    ui->labelOfLinkedObject->setVisible(state);
    ui->lineEditLinkedObjId->setText(m_linkId);
    ui->lineEditLinkedObjId->setVisible(state);
    ui->lineEditLinkedObjCaption->setText(m_linkTitle);
    ui->lineEditLinkedObjCaption->setVisible(state);
    ui->pushButtonOpenLinkedObject->setVisible(state);
}

void tabCashOperation::updateClientWidget()
{
    bool state = (m_clientWidgetMode != ClientMode::NotVisible);

    if(state)
    {
        ui->widgetClientInput->setLabelText(m_widgetClientLabel);
        ui->widgetClientInput->setBalanceVisible(m_showBalance);    // отображение баланса и/или обновление значений
        ui->widgetClientInput->setHandlingButtons(m_clientWidgetButtons);
    }
    else
        ui->widgetClientInput->clearCreds(false);

    ui->widgetClientInput->setVisible(state);
}

void tabCashOperation::setDefaultStylesheets()
{
    ui->lineEditLinkedObjCaption->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheet);
    ui->widgetClientInput->setDefaultStylesheets();
    ui->doubleSpinBoxAmount->setStyleSheet(commonSpinBoxStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
}

void tabCashOperation::updateModelData()
{
    if(m_orderId <= 0)
    {
        m_cashRegisterModel->setOperationType(m_orderType);
        m_cashRegisterModel->setCompany(companiesModel->databaseIDByRow(ui->comboBoxCompany->currentIndex(), "id"));
        m_cashRegisterModel->setCreatedDate(ui->dateEdit->date());
        m_cashRegisterModel->setAmount(amountAbsToSign(ui->doubleSpinBoxAmount->value()));
        m_cashRegisterModel->setSkipLogRecording(m_skipAutoLogRecord);
        m_cashRegisterModel->setReason(ui->lineEditReason->text());    // если пользователь отредактировал автоматически сгенерированный комментарий

        if(m_orderType == SCashRegisterModel::RecptRepair)
            m_cashRegisterModel->setLogText(tr("Поступление оплаты в размере %1").arg(m_cashRegisterModel->amountStr()));   // в случае поступления оплаты за ремонт до передачи уст-ва клиенту, нужно сделать запись в журнал

        switch(m_linkType)
        {
            case LinkType::Document: m_cashRegisterModel->setDocumentId(m_documentModel->id()); break;
            case LinkType::Repair: m_cashRegisterModel->setRepairId(m_repairModel->id()); break;
            case LinkType::Invoice: m_cashRegisterModel->setInvoiceId(m_invoiceModel->id()); break;
            default: break;
        }
    }
    else
        m_cashRegisterModel->setSkipLogRecording(true);

    if(m_cashRegisterModel->systemId() != paymentSystemId())
        m_cashRegisterModel->setSystemId(paymentSystemId());
}

bool tabCashOperation::commit(bool repeatAfter)
{
    QSqlQuery *query;
    bool nErr = 1;
    int initial_order_id = m_orderId;

    if(!checkInput())
        return 0;

    query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QUERY_LOG_START(metaObject()->className());

    updateModelData();

    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        m_cashRegisterModel->commit();
        m_orderId = m_cashRegisterModel->id();
        commitLink();

#ifdef QT_DEBUG
//        Global::ThrowType::Debug; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif

        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        m_cashRegisterModel->setId(0);
        m_orderId = initial_order_id;
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
    }

#ifdef QT_DEBUG
//    nErr = 1; // и это для отладки (чтобы проверить работу дальше)
#endif

    QUERY_LOG_STOP;

    delete query;

    if(nErr)
    {
        if(repeatAfter)
        {
            m_orderId = m_initialOrderId;
//            resetWidgetsSettings();
        }
        else
        {
            m_initialOrderId = m_orderId;
            emit updateTabTitle(this);
            p_instance.remove(initial_order_id);   // Если всё ОК, то нужно заменить указатель
            p_instance.insert(m_orderId, this);    // иначе будет падать при попытке создать новую вкладку

            m_comboBoxTypeRO = 1;
            m_amountRO = 1;
            m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard;
            m_checkBoxPrintVisible = 0;
            m_reasonRO = 1;
        }

        updateWidgets();
        if(ui->checkBoxPrintCheck->isChecked())
            print();
    }
    return nErr;
}

/* Выполнение запросов к БД для обновления данных связанного объекта.
 * Обработке подлежат только новые КО
 * Например, установка признака наличия предоплаты ремонта в таблице `workshop`
*/
bool tabCashOperation::commitLink()
{
    bool nErr = 1;

    if(m_initialOrderId <= 0)
    {
        switch(m_orderType)
        {
            case SCashRegisterModel::RecptSimple:
            case SCashRegisterModel::ExpSimple: nErr = commitSimple(); break;
            case SCashRegisterModel::RecptBalance:
            case SCashRegisterModel::ExpBalance: nErr = commitBalance(m_amount); break;
            case SCashRegisterModel::RecptInvoice: nErr = commitInvoice(); break;
            case SCashRegisterModel::RecptGoods: nErr = commitDocument(); break;
            case SCashRegisterModel::RecptPrepayRepair: nErr = commitPrepayRepair(); break;
//            case SCashRegisterModel::ExpRevert: nErr = commitRevert(); break;    // см. комметарии к методу
        }
    }

    return nErr;
}

bool tabCashOperation::commitSimple()
{
    return 1;
}

bool tabCashOperation::commitBalance(const double amount)
{
    if(amount < 0 && !m_clientModel->balanceEnough(amount))
        throw Global::ThrowType::UserCanceled;

    QString note;
    bool nErr = 1;

    if(amount > 0)
    {
        note = tr("Баланс клиента №%1 пополнен на %2").arg(m_clientModel->id()).arg(sysLocale.toCurrencyString(amount));
    }
    else
    {
        note = tr("С баланса клиента №%1 списано %2").arg(m_clientModel->id()).arg(sysLocale.toCurrencyString(-amount));
    }

    nErr = m_clientModel->updateBalance(amount, note);

    return nErr;
}

/*  Дополнительные действия при проведении операций, связанных с каким-либо документом (РН или ПН)
*/
bool tabCashOperation::commitDocument()
{
    if(m_documentModel->type() == SDocumentModel::OutInvoice)
        m_documentModel->setOrderId(m_orderId); // Для РН сумма автозаполняется, поэтому нужно лишь записать номер операции.
    else if(m_documentModel->type() == SDocumentModel::InInvoice)
    {
        // TODO: Для ПН возможна оплата частями и нужно следить, когда сумма всех платежей будет больше равна суммы накладной
    }
    return m_documentModel->commit();
}

/*  Дополнительные действия для ПКО оплаты счёта
*/
bool tabCashOperation::commitInvoice()
{
    m_invoiceModel->setPaid(QDateTime::currentDateTime());
    m_invoiceModel->setState(SInvoiceModel::Payed);
    return m_invoiceModel->commit();
}

/*  Дополнительные действия для ПКО предоплаты ремонта
*/
bool tabCashOperation::commitPrepayRepair()
{
    m_repairModel->addPrepay(m_amount);
    return m_repairModel->commit();
}

/*  Дополнительные действия для возвратного РКО
 *  TODO: В АСЦ v3.7.31.1123 при проведении возвратного РКО соответствующий ПКО не помечается; возможно, это стоит делать (для противодействия тупости или даже умышленного вредительства со стороны пользователя)
 *  Также при этом не производится никаких действий с ремонтом/расходной накладной/счётом, если производится возврат средств, ранее уплоченных по ним.
*/
bool tabCashOperation::commitRevert()
{
    return 1;
}

void tabCashOperation::print()
{
    QMap<QString, QVariant> report_vars;
    report_vars.insert("type", Global::Reports::pko);
    report_vars.insert("record_id", m_orderId);
    emit generatePrintout(report_vars);
}

void tabCashOperation::updateReasonWidget()
{
    ui->lineEditReason->setReadOnly(m_reasonRO);
    if(m_reasonRO)
        ui->lineEditReason->setText(m_generatedReason);
}

void tabCashOperation::updateAmountWidget()
{
    double amountAbs = qMax(-m_amount, m_amount);
    ui->doubleSpinBoxAmount->setRange(m_minAmount, m_maxAmount);
    ui->doubleSpinBoxAmount->setReadOnly(m_amountRO);
    ui->doubleSpinBoxAmount->setValue(amountAbs);
}

void tabCashOperation::updateLineEditOrderId()
{
    if(m_orderId > 0)
    {
        ui->lineEditOrderNum->setText(QString::number(m_orderId));
        ui->lineEditOrderNum->setStyleSheet("QLineEdit { background: #90EE90;}");   // светло-зелёный
    }
    else
    {
        ui->lineEditOrderNum->clear();
        ui->lineEditOrderNum->setStyleSheet(commonLineEditStyleSheet);
    }
}


QString tabCashOperation::tabTitle()
{
    if(m_orderId <= 0)
    {
        switch(m_initialOrderId)
        {
            case 0:
            case tabCashOperation::PKO: return tr("Новый ПКО");
            case tabCashOperation::RKO: return tr("Новый РКО");
        }
    }
    else
    {
        if(m_amount > 0)
            return tr("ПКО №%1").arg(m_orderId);
        else
            return tr("РКО №%1").arg(m_orderId);
    }

    return tr("Кассовый ордер №%1").arg(m_orderId);
}

/* Инициализация вкладки данными data
 * Например, метод может исопльзоваться при создании возвратного РКО
*/
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

    m_comboBoxTypeRO = 1;
    QMap<int, QVariant>::const_iterator i = data.constBegin();
    while(i != data.constEnd())
    {
        switch(i.key())
        {   // QMap сортирует данные по ключу, поэтому вызов методов будет последовательным
            case TmplType: ui->comboBoxOperationType->setCurrentIndex(m_operationTypesModel->rowByDatabaseID(i.value().toInt())); break;
            case TmplObjId: initLinkVariables(i.value().toInt()); break;
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
        ui->labelDate->setVisible(true);
        ui->lineEditDate->setVisible(true);
        ui->lineEditDate->setText(m_cashRegisterModel->created());
        ui->dateEdit->setVisible(false);
        ui->buttonPrint->show();
        ui->buttonSave->hide();
        ui->buttonSaveMore->hide();
        updateLineEditOrderId();
        ui->checkBoxAutoReason->setVisible(false);
        ui->pushButtonOpenLinkedObject->setText(tr("Открыть"));
        ui->comboBoxCompany->setCurrentIndex(m_cashRegisterModel->companyIndex());
        ui->comboBoxCompany->setEnabled(false);
        if(m_clientWidgetMode == ClientMode::Employee)
            ui->widgetClientInput->setLineEditText(allUsersModel->getDisplayRole(m_cashRegisterModel->employee()));    // замена ФИО на логин сотрудника
    }
    else
    {
        ui->labelDate->setVisible(permissions->createBackdatedDocuments);
        ui->lineEditDate->setVisible(false);
        ui->dateEdit->setVisible(permissions->createBackdatedDocuments);
        ui->buttonSave->show();
        ui->buttonSaveMore->show();
        ui->buttonPrint->hide();
        ui->buttonRevert->hide();
        ui->checkBoxAutoReason->setVisible(true);
        ui->comboBoxCompany->setCurrentIndex(SOfficeModel::current()->defaultCompanyIndex());
    }

    ui->comboBoxOperationType->setEnabled(!m_comboBoxTypeRO);
    updateLinkWidgets();
    ui->labelCompany->setText(m_companyLabel);
    updateClientWidget();
    updateAmountWidget();
    ui->checkBoxPrintCheck->setVisible(m_checkBoxPrintVisible);
    ui->comboBoxPaymentAccount->setEnabled(!m_paymentAccountRO);
    ui->toolButtonApplyPaymentSystem->hide();
    updateReasonWidget();
    ui->buttonRevert->setVisible(m_buttonRevertVisible);
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
    if(m_linkType != LinkType::NoLink)
    {
        if(m_linkId.isEmpty())
        {
            error |= 1<<1;
            ui->lineEditLinkedObjCaption->setStyleSheet(commonLineEditStyleSheetRed);
        }
    }
    // TODO: В АСЦ v3.7.31.1123 только ПКО может быть проведён на анонимного клиента; возможно, это неправильно
    if(m_clientWidgetMode != ClientMode::NotVisible && (m_orderType != SCashRegisterModel::RecptSimple))
    {
        if(ui->widgetClientInput->checkInput())
        {
            error |= 1<<2;
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
    if(m_clientModel->id() && ((m_orderType == SCashRegisterModel::RecptBalance) || (m_orderType == SCashRegisterModel::ExpBalance)) )
    {
        if(ui->widgetClientInput->checkBalanceEnabled())
            error |= 1<<5;
    }
    if ( ui->comboBoxCompany->currentIndex() < 0)
    {
        ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheetRed);
        error |= 1<<6;
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
    m_cashRegisterModel->load(orderId);
    m_orderId = m_cashRegisterModel->id();

    if(orderId == 0)
        return;

    m_orderType = m_cashRegisterModel->operationType();
    m_amount = m_cashRegisterModel->amount();
    m_generatedReason = m_cashRegisterModel->reason();
    if(m_amount >= 0)
    {
        initPKO();
    }
    else
    {
        initRKO();
    }

    ui->comboBoxOperationType->setCurrentIndex(m_operationTypesModel->rowByDatabaseID(m_orderType));
    initLinkVariables();
    if(m_linkType == LinkType::NoLink)
    {
        fillClientCreds(m_cashRegisterModel->client());
    }
    setPaymentAccountIndex(m_cashRegisterModel->systemId());
}

void tabCashOperation::initCashRegisterModel()
{
    m_cashRegisterModel->setId(0);
}

void tabCashOperation::fillClientCreds(int clientId)
{
    ui->widgetClientInput->fillCreds(clientId);   // загрузка данных из БД
    updateClientWidget();
}

void tabCashOperation::clientModelUpdated()
{
    m_cashRegisterModel->setClient(m_clientModel->id());
    if(m_orderId <= 0)
        m_generatedReason = m_cashRegisterModel->constructReason(m_linkId);
    updateReasonWidget();
}

/* Устаревший метод
 * Позже, после переработки MainWindow, будет удалён
*/
void tabCashOperation::fillDeviceCreds(int repairId)
{
    initLinkVariables(repairId);
}

void tabCashOperation::initRepairVariables(int repairId)
{
    m_repairModel = new SRepairModel();
    m_repairModel->load(repairId);
    m_linkId = QString::number(m_repairModel->id());
    m_linkTitle = m_repairModel->title();
    if(m_cashRegisterModel->id() <= 0)
    {
        m_amount = m_repairModel->realRepairCost() - m_repairModel->paymentsAmount();
        m_maxAmount = m_amount;
        if(m_orderType == SCashRegisterModel::RecptPrepayRepair)
            m_minAmount = qMin(0.1 * m_repairModel->realRepairCost(), m_maxAmount);    // TODO: настройка минимального процента предоплаты в общих настройках системы (Настройки-»Финансы)
        else
            m_minAmount = m_maxAmount;
    }
    else
        m_amount = m_cashRegisterModel->amountAbs();

    fillClientCreds(m_repairModel->clientId());    // загрузка данных клиента последним шагом
}

void tabCashOperation::initDocumentVariables(int documentId)
{
    m_documentModel = new SDocumentModel();
    m_documentModel->load(documentId);
    m_linkId = QString::number(documentId);
    m_linkTitle = m_documentModel->title();
    m_amount = m_documentModel->amount();
    fillClientCreds(m_documentModel->client());    // загрузка данных клиента последним шагом
}

void tabCashOperation::initInvoiceVariables(int invoiceId)
{
    m_invoiceModel = new SInvoiceModel();
    m_invoiceModel->load(invoiceId);
    m_linkId = m_invoiceModel->num();  // id != num
    m_linkTitle = m_linkId;
    m_amount = m_invoiceModel->summ();
    fillClientCreds(m_invoiceModel->clientId());    // загрузка данных клиента последним шагом
}

/* Вызов метода инициализации переменных при открытии или создании ордера, связанного с другим документом
 * objId используется при создании нового ордера; при открытии ранее созданного ордера id объекта берётся из модели
*/
void tabCashOperation::initLinkVariables(int objId)
{
    bool isNew = (m_cashRegisterModel->id() == 0);

    switch (m_linkType)
    {
        case LinkType::Document: initDocumentVariables(isNew?objId:m_cashRegisterModel->documentId()); break;
        case LinkType::Repair: initRepairVariables(isNew?objId:m_cashRegisterModel->repairId()); break;
        case LinkType::Invoice: initInvoiceVariables(isNew?objId:m_cashRegisterModel->invoiceId()); break;
    }

    updateLinkWidgets();
    updateAmountWidget();
}

void tabCashOperation::resetLinkVariables()
{
    deleteLinkObject();
    m_linkId.clear();
    m_linkTitle.clear();
}

void tabCashOperation::resetWidgetsSettings()
{
//    m_checkBoxPrintVisible = 0;   // сброс не требуется, т. к. значение переменной задаётся в методах initPKO/initRKO

    m_comboBoxTypeRO = 0;
    m_linkType = LinkType::NoLink;
    m_clientWidgetMode = ClientMode::Client;
    m_clientWidgetButtons = SClientSelectForm::AccessMode::Full;
    m_clientWidgetVisible = true;
    m_amountRO = false;
    m_minAmount = 0;
    m_maxAmount = 999999999999999.0000; // MySQL DECIMAL(19,4) = 999999999999999.9999
    m_showBalance = 0;
    m_paymentAccountRO = !permissions->editPaymentSystemInCommittedCashRegisters;
    m_skipAutoLogRecord = 0;
    m_reasonRO = 1;
    m_buttonRevertVisible = 0;
}

/* Слот, вызываемый при изменении текущего индекса ComboBox
 * Предназначен для установки параметров в соответствии с типом операции
*/
void tabCashOperation::operationTypeChanged(int index)
{
    resetLinkVariables();
    resetWidgetsSettings();

    if(m_orderId <= 0)
    {
        m_orderType = m_operationTypesModel->databaseIDByRow(index);
        m_cashRegisterModel->setOperationType(m_orderType); // необходимо сразу задать тип ордера, чтобы работал метод constructReason
        m_generatedReason = m_cashRegisterModel->constructReason(m_linkId);
        if(m_orderType > SCashRegisterModel::ExpCustom)
        {
            fillClientCreds(expenditureTypesModel->clientByDatabaseId(m_orderType));
            m_amount = expenditureTypesModel->defaultAmountByDatabaseId(m_orderType);

            int ps = expenditureTypesModel->paymentSystemByDatabaseId(m_orderType);
            if(ps)
            {
                setPaymentAccountIndex(ps);
//                m_paymentAccountRO = true;  // в АСЦ если в шаблоне РКО задана платежная система, то на вкладке её изменить нельзя
            }
        }
    }
    else
    {
        m_comboBoxTypeRO = 1;
        m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard;
        m_amountRO = true;
        m_checkBoxPrintVisible = 0;
        if(m_amount >= 0)
            if(m_cashRegisterModel->systemId() == 0)   // TODO: в АСЦ возврат доступен только для ПКО в кассу Наличные, возможно, стоит это доработать
                m_buttonRevertVisible = 1;
    }


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
        case SCashRegisterModel::ExpInvoice:   m_linkType = LinkType::Document; m_clientWidgetButtons = SClientSelectForm::AccessMode::SelectClear; break; /* выбор ПН; поля клиента RO; сумма редактируемая для "оплаты часятями" */
        case SCashRegisterModel::RecptGoods:   m_linkType = LinkType::Document; m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard; m_amountRO = true; break;  /* выбор РН; поля клиента RO */
        case SCashRegisterModel::ExpZ:         m_clientWidgetMode = ClientMode::NotVisible; m_skipAutoLogRecord = 1; break;                /* нет полей связанного объекта; нет полей клиента */
        case SCashRegisterModel::RecptPrepayRepair: m_skipAutoLogRecord = 1; m_linkType = LinkType::Repair; m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard; break;     /* поля клиента RO; при предоплате за ремонт запись в журнал производится в классе SRepair */
        case SCashRegisterModel::RecptRepair:  m_amountRO = true; m_linkType = LinkType::Repair; m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard; break;    /* аналогично предоплате, но сумма не редактируемая */
        case SCashRegisterModel::RecptInvoice: m_linkType = LinkType::Invoice; m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard; m_amountRO = true; m_skipAutoLogRecord = 1; break;   /* выбор счета; поля клиента RO */
        case SCashRegisterModel::RecptSimple:
        case SCashRegisterModel::ExpSimple:    m_skipAutoLogRecord = 1; break;
        case SCashRegisterModel::ExpSubsist:
        case SCashRegisterModel::ExpSalary:    m_clientWidgetMode = ClientMode::Employee; m_widgetClientLabel = tr("Сотрудник"); m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard; break;  /* только просмотр; в поле клиента пишем данные сотрудника */
        case SCashRegisterModel::AddSubCash:   m_clientWidgetMode = ClientMode::NotVisible; break;                /* только просмотр; нет полей клиента */
        case SCashRegisterModel::ExpRepair:    m_linkType = LinkType::Repair; break;       /* только просмотр */
        case SCashRegisterModel::ExpGoods:     m_linkType = LinkType::Document; break;     /* только просмотр */
        case SCashRegisterModel::ExpDealer:    m_clientWidgetButtons = SClientSelectForm::AccessMode::ViewCard; break;          /* только просмотр */
        case SCashRegisterModel::ExpRevert:    /*m_paymentAccountRO = 1; не помню почему не допускается выбор платёжной системы */m_skipAutoLogRecord = 1; break;
//        case SCashRegisterModel::MoveCash:     m_clientWidgetMode = ClientMode::NotVisible; break;              /* TODO: */
    }
    if(m_orderType > SCashRegisterModel::ExpCustom)
        m_skipAutoLogRecord = 1;

    updateWidgets();

#ifdef QT_DEBUG
    if(test_scheduler_counter == RandomFillerStep::End)  // если заполнение случайными данными уже выполнено, а я решил изменить тип операции
    {
        test_scheduler_counter = RandomFillerStep::LinkedObj;
        randomFill();
    }
#endif
}

void tabCashOperation::buttonOpenDocClicked()
{
    if(m_orderId > 0)
    {
        switch(m_linkType)
        {
            case LinkType::Repair: emit createTabOpenRepair(m_cashRegisterModel->repairId()); break;
            case LinkType::Document: emit createTabOpenDocument(m_cashRegisterModel->documentId()); break;
            case LinkType::Invoice: emit createTabOpenInvoice(m_cashRegisterModel->invoiceId());
        }
    }
    else
    {
        switch(m_linkType)
        {
            // TODO: создавать вкладку выбора связанного объекта нужно с предустановленным фильтром
            // Например, список расходных накладных по умолчанию не должен содержать документы на анонимных покупателей
            case LinkType::Repair: emit createTabSelectRepair(1, this); break;
            case LinkType::Document: emit createTabSelectDocument(1, this); break;
            case LinkType::Invoice: emit createTabSelectInvoice(1, this);
        }
        resetLinkVariables();
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

    QString text = tabTitle().append(" на сумму %1").arg(m_cashRegisterModel->amountStr());
    shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Проведён"), text, QColor(214,239,220), QColor(229,245,234));
    m_orderId = m_initialOrderId;
//    ui->comboBoxOperationType->setCurrentIndex(-1);
    resetLinkVariables();

    initCashRegisterModel();
    m_cashRegisterModel->setOperationType(m_orderType);    // тип операции тот же
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
//    m_reasonManEdited = 1;
}

void tabCashOperation::paymentSystemChanged(int index)
{
    Q_UNUSED(index)

    if(m_orderId > 0 && permissions->editPaymentSystemInCommittedCashRegisters)
    {
        bool state = (m_cashRegisterModel->systemId() != paymentSystemId());
        ui->toolButtonApplyPaymentSystem->setVisible(state);
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
    if(m_clientModel->id())
        data.insert(TmplClient, m_clientModel->id());
    data.insert(TmplAmount, m_amount);

    emit createTabUndoOperation(tabCashOperation::RKO, data);
}

/* Преобразование абсолютного значения в значение со знаком
*/
double tabCashOperation::amountAbsToSign(const double amountAbs)
{
    return (m_orderId == Type::RKO)?-amountAbs:amountAbs;
}

int tabCashOperation::paymentSystemId()
{
    return m_paymentAccountsProxyModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id");
}

void tabCashOperation::setPaymentAccountIndex(const int id)
{
    ui->comboBoxPaymentAccount->setCurrentIndex(m_paymentAccountsProxyModel->rowByDatabaseID(id, "system_id"));
}

/* Слот, вызываемый по сигналу spinBox'а
 * spinBox должен принимать только абсолютное знаение
*/
void tabCashOperation::amountChanged(double amountAbs)
{
    if(m_orderId > 0 || amountAbs < 0)
        return;

    m_amount = amountAbsToSign(amountAbs);
    m_cashRegisterModel->setAmount(m_amount);
    m_generatedReason = m_cashRegisterModel->constructReason(m_linkId);
    updateReasonWidget();
}

void tabCashOperation::applyPaymentSystem()
{
    if(commit())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "", tr("Платёжная система изменена"), QColor(214,239,220), QColor(229,245,234));
        ui->toolButtonApplyPaymentSystem->hide();
    }
    else
        shortlivedNotification *newPopup = new shortlivedNotification(this, "", tr("Ошибка"), QColor(255,164,119), QColor(255,199,173));
}

void tabCashOperation::guiFontChanged()
{
    QFont font(guiFont());

    ui->lineEditOrderNum->setFont(font);
    ui->lineEditLinkedObjCaption->setFont(font);
    ui->comboBoxCompany->setFont(font);
    ui->doubleSpinBoxAmount->setFont(font);
    ui->comboBoxPaymentAccount->setFont(font);
    ui->dummyForRowHeight->setFont(font);
}

#ifdef QT_DEBUG
void tabCashOperation::randomFill()
{
    int i;

    if (test_scheduler_counter == RandomFillerStep::OpType)
    {
        i = QRandomGenerator::global()->bounded(m_operationTypesModel->rowCount());
        ui->comboBoxOperationType->setCurrentIndex(i);
    }
    else if (test_scheduler_counter == RandomFillerStep::LinkedObj)
    {
        switch (m_linkType)
        {
            case LinkType::Document: dbgRandomDocument(m_orderId); break;
            case LinkType::Repair: dbgRandomRepair(); break;
            case LinkType::Invoice: dbgRandomInvoice(); break;
        }
    }
    else if (test_scheduler_counter == RandomFillerStep::RClient)
    {
        if(m_linkType == LinkType::NoLink)   // только если нет связанного объекта
        {
            ui->widgetClientInput->randomFill();
        }
    }
    else if (test_scheduler_counter == RandomFillerStep::Amount)
    {
        if(!m_amountRO)
        {
            i = QRandomGenerator::global()->bounded(20);
            m_amount = i*50;
            updateAmountWidget();
        }
    }
    else if (test_scheduler_counter == RandomFillerStep::PaymentSys)
    {
        i = QRandomGenerator::global()->bounded(m_paymentAccountsProxyModel->rowCount());
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
            query->exec(QString("SELECT `id` FROM (SELECT ROUND((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM workshop AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.state IN (0,1,2,3,4,5,6,13,14,15,31,32,33,34,35) AND `type` <> 10) AS t1 ON t1.`num` = `rand`.`rand`;"));
        else
            query->exec(QString("SELECT `id` FROM (SELECT ROUND((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM workshop AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.state IN (6,7,8,12,16) AND `type` <> 10) AS t1 ON t1.`num` = `rand`.`rand`;"));
        if(j<2)   // для mysql 5.6.51 (win) результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их
            continue;

        query->first();
        if(query->isValid())
            initLinkVariables(query->record().value(0).toInt());
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
            initLinkVariables(query->record().value(0).toInt());
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
            initLinkVariables(query->record().value(0).toInt());
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

