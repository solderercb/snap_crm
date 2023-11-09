#include "global.h"
#include "tabreceptcartridge.h"
#include "ui_tabreceptcartridge.h"
#include "tabrepairs.h"

tabReceptCartridge* tabReceptCartridge::p_instance = nullptr;

tabReceptCartridge::tabReceptCartridge(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabReceptCartridge)
{
    logUserActivity();

    QString query;
    QSqlQuery q(QSqlDatabase::database("connMain"));

    ui->setupUi(this);
    tabReceptCartridge::guiFontChanged();
    // установка белого фона только для scrollArea (не наследуется)
    // https://qtcentre.org/threads/14099-How-to-stop-child-widgets-from-inheriting-parent-s-StyleSheet-and-use-system-default?p=73101#post73101
    ui->scrollAreaWidgetContents->setStyleSheet("#scrollAreaWidgetContents {\
                                                    background-color: rgb(255, 255, 255);\
                                                }");
    m_tableModel = new QStandardItemModel();
    m_vendorsModel = new SSqlQueryModel();
    m_cartridgesModel = new SSqlQueryModel();
    m_findBySerialDelay = new QTimer();
    connect(m_findBySerialDelay, &QTimer::timeout, this, &tabReceptCartridge::findAndAddBySerial);
    m_findBySerialDelay->setSingleShot(true);
    initWidgets();

    query = QUERY_SEL_CARTRIDGE_CAT_ID;
    q.exec(query);
    if(!q.first())
        return;
    m_deviceClassId = q.value(0).toInt();
    query = QUERY_SEL_DEVICE_MAKERS(q.value(1).toString());
    m_vendorsModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxVendor->setCurrentIndex(-1);

#ifdef QT_DEBUG
    test_scheduler->start(200);
//    SRepairModel *repair;
//    repair = new SRepairModel();
//    repair->initCartridgeModel(1);
//    appendToReceptList(repair);
//    repair = new SRepairModel();
//    appendToReceptList(repair);
//    repair->initCartridgeModel(2);
#else
//    ui->groupBoxDebugPanel->hide();
#endif
}

tabReceptCartridge::~tabReceptCartridge()
{
    delete ui;
    delete m_tableModel;
    delete m_findBySerialDelay;
    p_instance = nullptr;   // Обязательно блять!
}

tabReceptCartridge* tabReceptCartridge::getInstance(MainWindow *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
    if( !p_instance )
        p_instance = new tabReceptCartridge(parent);
    return p_instance;
}

bool tabReceptCartridge::tabCloseRequest()
{
    // TODO: сделать проверку не сохранённых данных перед закрытием
    return 1;
}

QString tabReceptCartridge::tabTitle()
{
    return "Приём картриджей";
}

void tabReceptCartridge::setDefaultStyleSheets()
{
    ui->comboBoxPresetEngineer->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheet);
//    ui->comboBoxOffice->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPresetPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxVendor->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxModel->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditSerial->setStyleSheet(commonLineEditStyleSheet);
}

void tabReceptCartridge::initWidgets()
{
    ui->comboBoxCompany->setPlaceholderText(tr("организация"));
    ui->comboBoxOffice->setPlaceholderText(tr("офис"));
    ui->comboBoxOffice->setEnabled(false);  // TODO: см. комментарий к аналогичному вызову в классе tabRepairNew
    ui->comboBoxPresetEngineer->setPlaceholderText(tr("назначить инженером"));
    ui->comboBoxPresetEngineer->setButtons("Clear");
    ui->comboBoxPresetPaymentAccount->setPlaceholderText(tr("тип оплаты"));
    if(permissions->editCartridgeCards)
        ui->comboBoxModel->setButtons("Add, Edit");

    setDefaultStyleSheets();

    // установка моделей данных
    ui->comboBoxCompany->setModel(companiesModel);
    ui->comboBoxOffice->setModel(officesModel);
    if(comSettings->isAutoSetCompanyOnRepairRecept)
        ui->comboBoxCompany->setCurrentIndex(companiesModel->rowByDatabaseID(userDbData->currentOffice));
    else
        ui->comboBoxCompany->setCurrentIndex(-1);
    ui->comboBoxOffice->setCurrentIndex(officesModel->rowByDatabaseID(userDbData->currentOffice));
    ui->comboBoxPresetEngineer->setModel(engineersModel);
    ui->comboBoxPresetEngineer->setCurrentIndex(-1);
    ui->comboBoxPresetPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxPresetPaymentAccount->setCurrentIndex(-1);
    ui->spinBoxStickersCount->setValue(comSettings->defaultRepairStickersQty);
    ui->comboBoxVendor->setModel(m_vendorsModel);
    ui->comboBoxVendor->setCurrentIndex(-1);
    ui->comboBoxModel->setModel(m_cartridgesModel);
    m_client = ui->widgetClient->model();
    ui->checkBoxPrintReceipt->setChecked(comSettings->printCartridgeReceptDoc);

//    connect(ui->widgetDeviceMatch,SIGNAL(deviceSelected(int)),this,SLOT(fillDeviceCreds(int)));
    connect(ui->widgetClient, &SClientInputForm::createTabClient, this, &tabReceptCartridge::createTabClient);
    connect(ui->widgetClient, &SClientInputForm::createTabSelectExistingClient, this, &tabReceptCartridge::relayCreateTabSelectExistingClient);
    connect(ui->comboBoxVendor, SIGNAL(currentIndexChanged(int)), this, SLOT(changeVendor(int)));
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, qOverload<>(&tabReceptCartridge::appendToReceptList));
    connect(ui->buttonRecept, &QPushButton::clicked, this, &tabReceptCartridge::createRepairs);
    connect(ui->buttonReceptAndClose, &QPushButton::clicked, this, &tabReceptCartridge::createRepairsAndClose);
    connect(ui->lineEditSerial, &QLineEdit::textEdited, this, &tabReceptCartridge::serialTextEdited);
    connect(ui->lineEditSerial, &QLineEdit::returnPressed, this, &tabReceptCartridge::findAndAddBySerial);
    connect(ui->comboBoxModel, &SComboBox::buttonClicked, this, &tabReceptCartridge::comboBoxModelButtonClickHandler);
    connect(m_client, &SClientModel::modelUpdated, [=](){ui->checkBoxPrintReceipt->setChecked(comSettings->printCartridgeReceptDoc && !(m_client->options() & (SClientModel::BalanceEnabled | SClientModel::Company | SClientModel::Regular)));});
}

void tabReceptCartridge::clearWidgets()
{
    SCartridgeForm *form;

    ui->checkBoxIsHighPriority->setCheckState(Qt::Unchecked);
    ui->comboBoxPresetEngineer->setCurrentIndex(-1);
    ui->spinBoxStickersCount->setValue(comSettings->defaultRepairStickersQty);
    ui->lineEditSerial->setText("");
    ui->spinBoxQty->setValue(1);
    ui->comboBoxVendor->setCurrentIndex(-1);
    ui->comboBoxModel->setCurrentIndex(-1);
    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        form->deleteLater();
    }
}

void tabReceptCartridge::setModelData()
{
    int preferredPaymentAccIndex;
//    if(ui->checkBoxIsHighPriority->isChecked())
//        repairModel->setExpressRepair(1);
//        repairModel->setEngineerIndex(ui->comboBoxPresetEngineer->currentIndex());
//    repairModel->setCompanyIndex(ui->comboBoxCompany->currentIndex());
//    repairModel->setOffice(userDbData->currentOffice);
//    repairModel->setStartOffice(userDbData->currentOffice);
    preferredPaymentAccIndex = ui->comboBoxPresetPaymentAccount->currentIndex();

    if( preferredPaymentAccIndex >= 0)
    {
//        repairModel->setPaymentSystemIndex(preferredPaymentAccIndex);
//        if(paymentSystemsModel->databaseIDByRow(preferredPaymentAccIndex, "system_id") == -1)
//            repairModel->setIsCardPayment(1);
    }
//    repairModel->setVendorId(m_vendorsModel->databaseIDByRow(ui->comboBoxVendor->currentIndex()));
}

bool tabReceptCartridge::checkInput()
{
    int error = 0;
    SCartridgeForm *form;

    setDefaultStyleSheets();

    if ( ui->comboBoxOffice->currentIndex() < 0 && ui->comboBoxModel->currentText() == "" )
    {
        ui->comboBoxOffice->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 1;
    }
    if ( comSettings->isEngineerRequiredOnDeviceRecept && ui->comboBoxPresetEngineer->currentIndex() < 0 )
    {
        ui->comboBoxPresetEngineer->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 2;
    }

    error = qMax(error, ui->widgetClient->checkInput());

    if (error)
    {
        qDebug() << "Ошибка создания карточки(-ек) заправки: не заполнены общие обязательные поля (error " << error << ")";
    }

    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        error = qMax(error, form->checkInput());
    }

    return error;
}

bool tabReceptCartridge::checkInputBeforeAdd()
{
    int error = 0;
    setDefaultStyleSheets();

    if(ui->comboBoxVendor->currentIndex() == -1)
    {
        ui->comboBoxVendor->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 1;
    }
    if(ui->comboBoxModel->currentIndex() == -1)
    {
        ui->comboBoxModel->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 2;
    }
    if(ui->lineEditSerial->text() == "" && comSettings->isCartridgeSerialNumberRequired && ui->spinBoxQty->value() == 1)
    {
        ui->lineEditSerial->setStyleSheet(commonLineEditStyleSheetRed);
        error = 3;
    }

    if (error)
    {
        qDebug() << "Ошибка добавления карточки заправки: не все обязательные поля заполнены (error " << error << ")";
        return error;
    }

    return false;
}

void tabReceptCartridge::appendToReceptList(SCartridgeForm *form)
{
    ui->verticalLayoutCartridges->addWidget(form);
    connect(form, &SCartridgeForm::updateTotalPreagreedAmount, this, &tabReceptCartridge::updateTotalPreagreedAmount);
    form->initWidgets();
}

void tabReceptCartridge::appendToReceptList()
{
    if(checkInputBeforeAdd())
        return;

    for(int i = 0; i < ui->spinBoxQty->value(); i++)
    {
        SCartridgeForm *form = new SCartridgeForm();
        form->setDeviceClassId(m_deviceClassId);
        form->setDeviceVendorId(m_vendorsModel->databaseIDByRow(ui->comboBoxVendor->currentIndex()));
        form->setCardId(m_cartridgesModel->databaseIDByRow(ui->comboBoxModel->currentIndex()));
        form->setTitle(tr("картридж") + " " + ui->comboBoxVendor->currentText() + " " + ui->comboBoxModel->currentText());
        if(!ui->lineEditSerial->text().isEmpty() && ui->spinBoxQty->value() == 1)
            form->setSerialNumber(ui->lineEditSerial->text());
        else
        {
            // если не указан(ы) серийки, спросить пользователя о генерации; при положительном ответе распечатать новые этикетки
            // TODO: параметр глобальных настроек с паттерном серийного номера
        }

#ifdef QT_DEBUG
        form->randomFill();
#endif
        appendToReceptList(form);

    }

    ui->comboBoxModel->setCurrentIndex(-1);
    ui->lineEditSerial->setText("");
    ui->spinBoxQty->setValue(1);
}

void tabReceptCartridge::updateTotalPreagreedAmount()
{
    SCartridgeForm *form;
    double amount = 0;

    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        amount += form->preargeedAmount();
    }
    ui->doubleSpinBoxTotalPreagreedAmount->setValue(amount);
}

void tabReceptCartridge::createCartridgeCardForm(const int id)
{
    int vendorIndex = ui->comboBoxVendor->currentIndex();

    m_cartridgeCardForm = new SCartridgeCard(id, vendorIndex, Qt::SplashScreen, this);
    connect(m_cartridgeCardForm, &SCartridgeCard::onDelete, this, &tabReceptCartridge::closeCartridgeCardForm);
    connect(m_cartridgeCardForm, &SCartridgeCard::newCardCreated, this, &tabReceptCartridge::updateDevicesModel);
}

void tabReceptCartridge::closeCartridgeCardForm()
{
}

void tabReceptCartridge::updateDevicesModel(const int id)
{
    int vendorId = m_vendorsModel->databaseIDByRow(ui->comboBoxVendor->currentIndex());
    m_cartridgesModel->setQuery(QUERY_SEL_CARTRIDGE_MODELS(vendorId), QSqlDatabase::database("connMain"));
    ui->comboBoxModel->setCurrentIndex(m_cartridgesModel->rowByDatabaseID(id));
}

void tabReceptCartridge::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize);

    ui->comboBoxPresetEngineer->setFont(font);
    ui->comboBoxCompany->setFont(font);
//    ui->comboBoxOffice->setFont(font);
    ui->comboBoxPresetPaymentAccount->setFont(font);
    ui->comboBoxVendor->setFont(font);
    ui->comboBoxModel->setFont(font);
    ui->lineEditSerial->setFont(font);
}

void tabReceptCartridge::print()
{
    // печать чека-квитанции
    if(m_printReport)
    {
        qDebug().nospace() << "[" << this << "] print()";
        SPrintPOSReport *report = new SPrintPOSReport();
        report->setClientModel(m_client);
        report->openPrinter(userLocalData->PosPrinter.value);
        SCartridgeForm *form;
        for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
        {
            if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
                continue;

            form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
            report->addPrintJob(form->model());
        }

        report->closePrinter();
        delete report;
    }

    // печать стикеров
//    QMap<QString, QVariant> report_vars;
//    if(comSettings->printCartridgeStickers)
//    {
//        report_vars.insert("type", Global::Reports::rep_label);
//        report_vars.insert("repair_id", repair);
//        report_vars.insert("copies", ui->spinBoxStickersCount->value());
//        emit generatePrintout(report_vars);
//        report_vars.clear();
//    }
}

#ifdef QT_DEBUG
void tabReceptCartridge::randomFill()
{
    int i;
    if (test_scheduler_counter == 0)
    {
        ui->widgetClient->randomFill();
    }
    else if(test_scheduler_counter == 1)
    {
        ui->comboBoxVendor->setCurrentIndex(1);
    }
    else if(test_scheduler_counter == 2)
    {
        ui->comboBoxModel->setCurrentIndex(0);
    }
    else if(test_scheduler_counter == 3)
    {
        ui->lineEditSerial->setText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
    }
    else if(test_scheduler_counter == 4)
    {
//        ui->spinBoxQty->setValue(QRandomGenerator::global()->bounded(5));
        ui->spinBoxQty->setValue(2);
    }
    else if (test_scheduler_counter == 5)   // инженер
    {
        if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50
        {
            i = engineersModel->rowCount();
            ui->comboBoxPresetEngineer->setCurrentIndex(QRandomGenerator::global()->bounded(i));
        }
    }
    else if (test_scheduler_counter == 6)
    {
        i = QRandomGenerator::global()->bounded(16384);
        ui->checkBoxIsHighPriority->setChecked((i>>2)&0x01);
    }
    else if(test_scheduler_counter == 7)
    {
        ui->pushButtonAdd->click();
    }
    if (test_scheduler_counter < 6)
    {
        test_scheduler_counter++;
        test_scheduler->start(400);    //  (пере-)запускаем таймер
    }
}
#endif

void tabReceptCartridge::fillClientCreds(const int id)
{
    ui->widgetClient->fillClientCreds(id);
}

void tabReceptCartridge::changeVendor(int index)
{
    Q_UNUSED(index);
    updateDevicesModel();
}

void tabReceptCartridge::serialTextEdited(QString text)
{
    Q_UNUSED(text);
    m_findBySerialDelay->stop();
    m_findBySerialDelay->start(1000);
}

void tabReceptCartridge::findAndAddBySerial()
{
    m_findBySerialDelay->stop();
    if(ui->lineEditSerial->text().isEmpty())
        return;

    QSqlQuery q(QSqlDatabase::database("connMain"));
    QString query;
    QStringList query_conditions;

    query_conditions.append(QString("`type` = %1").arg(m_deviceClassId));
    query_conditions.append(QString("`serial_number` LIKE '%1'").arg(ui->lineEditSerial->text()));

    if(ui->comboBoxVendor->currentIndex() >= 0 && !m_vendorsModel->rowColumnToValue<QString>(ui->comboBoxVendor->currentIndex(), 0).trimmed().isEmpty())
        query_conditions.append(QString("`maker` = %1").arg(m_vendorsModel->databaseIDByRow(ui->comboBoxVendor->currentIndex())));

    if(ui->comboBoxModel->currentIndex() >= 0)
        query_conditions.append(QString("t2.`card_id` = %1").arg(m_cartridgesModel->databaseIDByRow(ui->comboBoxModel->currentIndex())));

    query = QUERY_SEL_CARTRIDGE_BY_SN(query_conditions.join(" AND "));
    q.exec(query);
    if(!q.first())
        return;

    if(q.numRowsAffected() > 1) // если по введенному номеру найдено более одного картриджа, то автоматически добавлять в список нельзя
        return;

    if(!m_client->isNew() && m_client->id() != q.value("client").toInt())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this,
                                                                      tr("Информация"),
                                                                      tr("Нельзя принять картриджи разных клиентов вместе."),
                                                                      QColor(255,164,119),
                                                                      QColor(255,199,173));
        return;
    }

    fillClientCreds(q.value("client").toInt());

    SCartridgeForm *form = new SCartridgeForm();
    form->setDeviceClassId(q.value("type").toInt());
    form->setDeviceVendorId(q.value("maker").toInt());
    form->setCardId(q.value("card_id").toInt());
    form->setTitle(q.value("Title").toString());
    form->setSerialNumber(q.value("serial_number").toString());
    form->setIsRepeat(1);

    appendToReceptList(form);
    ui->lineEditSerial->setText("");
    ui->lineEditSerial->setFocus();
}

#ifdef QT_DEBUG
void tabReceptCartridge::dbgRandomFillClicked()
{
    test_scheduler_counter = 0;
    ui->widgetClient->clearClientCreds();
    clearWidgets();
    randomFill();
}
#endif

void tabReceptCartridge::relayCreateTabSelectExistingClient(int, QWidget*)
{
    emit createTabSelectExistingClient(1, this);
}

bool tabReceptCartridge::createRepairs()
{
    if(checkInput())
        return 0;

    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    SCartridgeForm *form;

    setDefaultStyleSheets();
    m_printReport = ui->checkBoxPrintReceipt->isChecked(); // кэширование состояния

    QUERY_LOG_START(metaObject()->className());

    try
    {
        ui->widgetClient->commit();
        m_client = ui->widgetClient->model();

        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
        {
            if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
                continue;

            form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
            form->setClientId(m_client->id());
            form->setCompanyIndex(ui->comboBoxCompany->currentIndex());
            form->setOfficeIndex(ui->comboBoxOffice->currentIndex());
            form->setEngineerIndex(ui->comboBoxPresetEngineer->currentIndex());
            if(ui->checkBoxIsHighPriority->isChecked())
                form->setIsHighPriority(1);
            form->setPaymentSystemIndex(ui->comboBoxPresetPaymentAccount->currentIndex());

            form->createRepair();
        }

#ifdef QT_DEBUG
//        throw Global::ThrowType::Debug; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query, nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
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

    QUERY_LOG_STOP;

    if (nErr)   // если все запросы выполнены без ошибок
    {
        print();
        clearWidgets();
        tabRepairs::refreshIfTabExists();
    }

    return nErr;
}

void tabReceptCartridge::createRepairsAndClose()
{
    if(!m_closePending)    // программа падает при двойном клике по кнопке.
    {
        m_closePending = 1;
        if (createRepairs())
            this->deleteLater();
        else
            m_closePending = 0;
    }

}

void tabReceptCartridge::comboBoxModelButtonClickHandler(int id)
{
    if(id == SLineEdit::Edit)
        createCartridgeCardForm(m_cartridgesModel->databaseIDByRow(ui->comboBoxModel->currentIndex()));
    else if(id == SLineEdit::Add)
        createCartridgeCardForm(0);
}

#ifdef QT_DEBUG
void tabReceptCartridge::test_scheduler_handler()
{
    randomFill();
}

void tabReceptCartridge::test_scheduler2_handler()
{

}
#endif
