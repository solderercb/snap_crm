#include "scartridgeform.h"
#include "ui_scartridgeform.h"

SCartridgeForm::SCartridgeForm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SCartridgeForm)
{
    ui->setupUi(this);

//    ui->frameHeader->setStyleSheet("QFrame::hover {background-color: rgb(205,230,247);}");    // подсветка заголовка
    installEventFilter(this);
}

SCartridgeForm::SCartridgeForm(const int repairId, QWidget *parent) :
    SCartridgeForm(parent)
{
    m_repairId = repairId;
    initWidgets();
}

SCartridgeForm::~SCartridgeForm()
{
    delete ui;
    delete m_cartridgeCard; // перед удалением модели m_repair
    if(m_repair)
        delete m_repair;
    if(worksAndPartsModel)
        delete worksAndPartsModel;
    if(statusesProxyModel)
        delete statusesProxyModel;
}

void SCartridgeForm::initModels()
{
    m_cartridgeCard = new SCartridgeCardModel(this);
    if(m_repairId)
    {
        m_repair = new SRepairModel();
        connect(m_repair, &SRepairModel::modelUpdated, this, &SCartridgeForm::updateWidgets);
        m_cartridgeCard->setParent(m_repair);

        statusesProxyModel = new SSortFilterProxyModel;
        statusesProxyModel->setSourceModel(statusesModel);
        statusesProxyModel->setFilterKeyColumn(Global::RepStateHeaders::Id);
        statusesProxyModel->setFilterRegularExpression("");

        worksAndPartsModel = new SSaleTableModel();
        worksAndPartsModel->setTableMode(SSaleTableModel::WorkshopSale);
        worksAndPartsModel->setPriceColumn(SStoreItemModel::PriceOptionService);
        connect(worksAndPartsModel, &SSaleTableModel::amountChanged, this, &SCartridgeForm::updateTotalSumms);
        connect(worksAndPartsModel, &SSaleTableModel::tableSaved, this, &SCartridgeForm::saveTotalSumms);
        connect(worksAndPartsModel, &SSaleTableModel::modelReset, this, &SCartridgeForm::updateLists);
        worksAndPartsModel->setEditStrategy(SSaleTableModel::OnManualSubmit);
        worksAndPartsModel->setRepairType(SSaleTableModel::CartridgeRepair);
        m_repair->setWorksAndPartsModel(worksAndPartsModel);
    }

    updateModels();
}

void SCartridgeForm::updateModels()
{
    if(m_repairId)
    {
        m_repair->load(m_repairId);
        m_cardId = m_repair->cartridge()->cardId();
        m_clientId = m_repair->clientId();
        worksAndPartsModel->repair_loadTable(m_repair->id());
        updateStatesModel(m_repair->state());
    }

    m_cartridgeCard->load(m_cardId);
}

void SCartridgeForm::initWidgets()
{
    initModels();
    ui->comboBoxWasEarly->setModel(cartridgeRepeatReason);
    ui->comboBoxWasEarly->setCurrentIndex(-1);
    ui->comboBoxWasEarly->disableWheelEvent(true);
    ui->comboBoxPlace->setButtons("Clear");
    ui->comboBoxPlace->setModel(repairBoxesModel);
    ui->comboBoxPlace->setCurrentIndex(-1);
    ui->comboBoxPlace->disableWheelEvent(true);
    connect(ui->comboBoxPlace, qOverload<int>(&QComboBox::currentIndexChanged), this, &SCartridgeForm::savePlace);
    connect(ui->comboBoxPlace, &SComboBox::buttonClicked, this, &SCartridgeForm::comboBoxPlaceButtonClickHandler);
    ui->labelLimitReached->setVisible(checkResource());
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &SCartridgeForm::removeWidget);

    if(m_repairId)
    {
        ui->checkBoxPreagreedRefill->installEventFilter(this);
        ui->checkBoxPreagreedChipReplace->installEventFilter(this);
        ui->checkBoxPreagreedDrumReplace->installEventFilter(this);
        ui->checkBoxPreagreedBladeReplace->installEventFilter(this);
        ui->comboBoxEngineer->setModel(engineersModel);
        ui->comboBoxEngineer->disableWheelEvent(true);
        connect(ui->comboBoxEngineer, qOverload<int>(&QComboBox::currentIndexChanged), this, &SCartridgeForm::comboBoxEngineerChanged);
        ui->comboBoxState->setModel(statusesProxyModel);
        ui->comboBoxState->setCurrentIndex(-1);
        ui->comboBoxState->disableWheelEvent(true);
        connect(ui->comboBoxState, qOverload<int>(&QComboBox::currentIndexChanged), this, &SCartridgeForm::stateIndexChanged);
        ui->checkBoxRefill->installEventFilter(this);
        ui->checkBoxChipReplace->installEventFilter(this);
        ui->checkBoxDrumReplace->installEventFilter(this);
        ui->checkBoxBladeReplace->installEventFilter(this);
        connect(ui->pushButtonClientCard, &QPushButton::clicked, this, &SCartridgeForm::buttonClientCardClicked);
        connect(ui->toolButtonClassicTab, &QPushButton::clicked, this, &SCartridgeForm::buttonClassicTabClicked);
        connect(ui->toolButtonCartridgeCard, &QPushButton::clicked, this, &SCartridgeForm::buttonCartridgeCardClicked);
        connect(ui->lineEditComment, &QLineEdit::editingFinished, this, &SCartridgeForm::updateComment);

        updateHeader();
        updateWidgets();
    }
    else
    {
        ui->pushButtonClientCard->hide();
        ui->toolButtonClassicTab->hide();
        ui->toolButtonCartridgeCard->hide();
        ui->splitterRefillWeight->hide();
        ui->splitterRefill->hide();
        ui->splitterRealRefillWeight->hide();
        ui->splitterDrumReplace->hide();
        ui->splitterChipReplace->hide();
        ui->splitterBladeReplace->hide();
        ui->splitterState->hide();
        ui->splitterTotalAmount->hide();
        ui->splitterEngineer->hide();
        ui->listWidgetWorks->hide();
        ui->listWidgetParts->hide();
        ui->comboBoxWasEarly->setCurrentIndex(m_isRepeat?1:-1);

        connect(ui->checkBoxPreagreedRefill, &QCheckBox::stateChanged, this, &SCartridgeForm::setRefill);
        connect(ui->checkBoxPreagreedChipReplace, &QCheckBox::stateChanged, this, &SCartridgeForm::setChipReplace);
        connect(ui->checkBoxPreagreedDrumReplace, &QCheckBox::stateChanged, this, &SCartridgeForm::setDrumReplace);
        connect(ui->checkBoxPreagreedBladeReplace, &QCheckBox::stateChanged, this, &SCartridgeForm::setBladeReplace);
        ui->checkBoxPreagreedRefill->setChecked(true);
    }
}

void SCartridgeForm::updateHeader()
{
    ui->labelTitle->setText(QString("%1 [%2]").arg(m_repair->id()).arg(m_repair->title()));

    SClientModel *client = new SClientModel(m_repair->clientId());
    ui->pushButtonClientCard->setText(client->fullLongName());
    delete client;  // TODO: модель нужна только для получения ФИО; подумать над более оптимальным способом.
}

void SCartridgeForm::updateWidgets()
{
//    ui->splitterComment->hide();    // TODO: придумать где расположить поле с примечанием; поле должно быть редактируемым.
    ui->splitterWasEarly->hide();   // TODO: ссылка на предыдущий ремонт (открытие классической карты ремонта)
    ui->lineEditSerial->setText(m_repair->serialNumber());
    ui->lineEditSerial->setReadOnly(true);
    ui->checkBoxPreagreedRefill->setChecked(m_repair->cartridge()->refill());
    ui->checkBoxPreagreedChipReplace->setChecked(m_repair->cartridge()->chip());
    ui->checkBoxPreagreedDrumReplace->setChecked(m_repair->cartridge()->drum());
    ui->checkBoxPreagreedBladeReplace->setChecked(m_repair->cartridge()->Blade());
    ui->spinBoxRefillWeight->setValue(m_cartridgeCard->tonerWeight());
    ui->doubleSpinBoxPreagreedAmount->setValue(m_repair->preAgreedAmount());
    ui->comboBoxWasEarly->setCurrentIndex(m_repair->isRepeat()?1:-1);
    ui->comboBoxWasEarly->setCurrentIndex(m_repair->isWarranty()?0:-1);
    ui->comboBoxWasEarly->setEnabled(false);

    setWidgetsParams(m_repair->state());

    ui->checkBoxRefill->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Toner));
    ui->checkBoxChipReplace->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Chip));
    ui->checkBoxDrumReplace->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Drum));
    ui->checkBoxBladeReplace->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Blade));
    ui->comboBoxEngineer->setEnabled(engineerComboBoxEn && (permissions->setRepairEngineer || permissions->beginUnengagedRepair));
    updateComboBoxEngineer(m_repair->engineer());
    ui->doubleSpinBoxTotalAmount->setValue(m_repair->realRepairCost());
    ui->comboBoxPlace->setCurrentIndex(m_repair->boxIndex());
    ui->comboBoxPlace->setEnabled(placeComboBoxEn);
    ui->lineEditComment->setText(m_repair->extNotes());
}

bool SCartridgeForm::eventFilter(QObject *watched, QEvent *event)
{
//    if(event->type() != QEvent::Paint)
//        qDebug().nospace() << "[" << this << "] eventFilter() | watched: " << m_repair->objectName() << "; event->type(): " << event->type();

    // TODO: выделение виджета (фокус):
    // QEvent::MouseButtonPress
    // любое взаимодействие с дочерними виджетами

    QCheckBox *checkBox = dynamic_cast<QCheckBox*>(watched);
    if(checkBox)
    {
        if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick)
        {
            if(static_cast<QMouseEvent*>(event)->button() != Qt::MouseButton::LeftButton)
                return true;

            if( checkBox == ui->checkBoxPreagreedRefill ||
                checkBox == ui->checkBoxPreagreedChipReplace ||
                checkBox == ui->checkBoxPreagreedDrumReplace ||
                checkBox == ui->checkBoxPreagreedBladeReplace )
                return true;

            if(!checkBox->isEnabled())
                return true;

            int ret = 1;
            int nSt = checkBox->checkState()?0:2;
            if(checkBox == ui->checkBoxRefill)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeRefill, nSt);
            else if(checkBox == ui->checkBoxChipReplace)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeChipReplace, nSt);
            else if(checkBox == ui->checkBoxDrumReplace)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeDrumReplace, nSt);
            else if(checkBox == ui->checkBoxBladeReplace)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeBladeReplace, nSt);
            return nSt?1:ret; // при добавлении работ нужно фильтровать событие, а при удалении нет
        }
    }

    return false;
}

SRepairModel *SCartridgeForm::model()
{
    return m_repair;
}

bool SCartridgeForm::createRepair()
{
    bool nErr = 1;
    m_repair = new SRepairModel(this);

    m_repair->initCartridgeRepairModel(m_cardId);

    QStringList faultList;
    if(ui->checkBoxPreagreedRefill->isChecked())
    {
        faultList.append(tr("Заправка"));
        m_repair->cartridge()->setRefill(1);
    }
    if(ui->checkBoxPreagreedChipReplace->isChecked())
    {
        faultList.append(tr("Чип"));
        m_repair->cartridge()->setChip(1);
    }
    if(ui->checkBoxPreagreedDrumReplace->isChecked())
    {
        faultList.append(tr("Фотовал"));
        m_repair->cartridge()->setDrum(1);
    }
    if(ui->checkBoxPreagreedBladeReplace->isChecked())
    {
        faultList.append(tr("Ракель"));
        m_repair->cartridge()->setBlade(1);
    }
    m_repair->setTitle(m_title);
    m_repair->setClassId(m_deviceClassId);
    m_repair->setVendorId(m_deviceVendorId);
    m_repair->setClientId(m_clientId);
    m_repair->setFault(faultList.join(", "));
    m_repair->setSerialNumber(ui->lineEditSerial->text());
    m_repair->setExtNotes(ui->lineEditComment->text());
    switch(ui->comboBoxWasEarly->currentIndex())
    {
        case -1: m_repair->setIsWarranty(0); m_repair->setIsRepeat(0); break;
        case 0: m_repair->setIsWarranty(1); m_repair->setIsRepeat(0); break;
        case 1: m_repair->setIsRepeat(1); m_repair->setIsWarranty(0); break;
    }
    m_repair->setPreAgreedAmount(ui->doubleSpinBoxPreagreedAmount->value());
    m_repair->setRejectReason("");
    m_repair->setCompanyIndex(m_companyIndex);
    m_repair->setOfficeIndex(m_officeIndex);
    m_repair->setStartOfficeIndex(m_officeIndex);
    m_repair->setManager(userDbData->id);
    m_repair->setCurrentManager(userDbData->id);
    m_repair->setEngineerIndex(m_engineerIndex);
    if(m_isHighPriority)
        m_repair->setExpressRepair(1);
    if( m_paymentSystemIndex >= 0)
    {
        m_repair->setPaymentSystemIndex(m_paymentSystemIndex);
        if(paymentSystemsModel->databaseIDByRow(m_paymentSystemIndex, "system_id") == -1)
            m_repair->setIsCardPayment(1);
    }

    nErr = m_repair->commit();

    if(ui->comboBoxPlace->currentIndex() >= 0)
    {
        m_repair->setBoxIndex(ui->comboBoxPlace->currentIndex());
        nErr = m_repair->commit();
    }

    return nErr;
}

bool SCartridgeForm::updateRepair()
{
    return 1;
}

int SCartridgeForm::repairId()
{
    return m_repairId;
}

const QString &SCartridgeForm::title() const
{
    return m_title;
}

void SCartridgeForm::setTitle(const QString &title)
{
    m_title = title;
    ui->labelTitle->setText(m_title);
}

bool SCartridgeForm::isRepeat() const
{
    return m_isRepeat;
}

void SCartridgeForm::setIsRepeat(bool isRepeat)
{
    m_isRepeat = isRepeat;
}

int SCartridgeForm::deviceClassId() const
{
    return m_deviceClassId;
}

void SCartridgeForm::setDeviceClassId(int id)
{
    m_deviceClassId = id;
}

int SCartridgeForm::deviceVendorId() const
{
    return m_deviceVendorId;
}

void SCartridgeForm::setDeviceVendorId(int id)
{
    m_deviceVendorId = id;
}

int SCartridgeForm::deviceModelId() const
{
    return m_deviceModelId;
}

void SCartridgeForm::setDeviceModelId(int id)
{
    m_deviceModelId = id;
}

const QString &SCartridgeForm::serialNumber() const
{
    return m_serialNumber;
}

void SCartridgeForm::setSerialNumber(const QString &serial)
{
    m_serialNumber = serial;
    ui->lineEditSerial->setText(m_serialNumber);
}

int SCartridgeForm::cardId() const
{
    return m_cardId;
}

void SCartridgeForm::setCardId(int id)
{
    m_cardId = id;
}

int SCartridgeForm::clientId() const
{
    return m_clientId;
}

void SCartridgeForm::setClientId(int clientId)
{
    m_clientId = clientId;
}

// индекс (номер строки) модели companiesModel
int SCartridgeForm::companyIndex() const
{
    return m_companyIndex;
}

// индекс (номер строки) модели companiesModel
void SCartridgeForm::setCompanyIndex(int index)
{
    m_companyIndex = index;
}

// индекс (номер строки) модели officesModel
int SCartridgeForm::officeIndex() const
{
    return m_officeIndex;
}

// индекс (номер строки) модели officesModel
void SCartridgeForm::setOfficeIndex(int index)
{
    m_officeIndex = index;
}

// индекс (номер строки) модели engineersModel
int SCartridgeForm::engineerIndex() const
{
    return m_engineerIndex;
}

// индекс (номер строки) модели engineersModel
void SCartridgeForm::setEngineerIndex(int engineer)
{
    m_engineerIndex = engineer;
}

bool SCartridgeForm::isHighPriority() const
{
    return m_isHighPriority;
}

void SCartridgeForm::setIsHighPriority(bool isHighPriority)
{
    m_isHighPriority = isHighPriority;
}

int SCartridgeForm::paymentSystemIndex() const
{
    return m_paymentSystemIndex;
}

void SCartridgeForm::setPaymentSystemIndex(int paymentSystemIndex)
{
    m_paymentSystemIndex = paymentSystemIndex;
}

double SCartridgeForm::preargeedAmount()
{
    return m_amount;
}

bool SCartridgeForm::checkData(const int stateId)
{
    setDefaultStyleSheets();
    if(stateId == Global::RepStateIds::Ready || stateId == Global::RepStateIds::ReadyNoRepair)
    {
        if(worksAndPartsModel->amountTotal() == 0)
        {
            ui->doubleSpinBoxTotalAmount->setStyleSheet(commonSpinBoxStyleSheetRed);
            throw Global::ThrowType::ConditionsError;
        }
    }
    return 1;
}

void SCartridgeForm::doStateActions(const int stateId)
{
    QStringList stateActions = statusesProxyModel->value(stateId, Global::RepStateHeaders::Id, Global::RepStateHeaders::Actions).toString().split('|');

    // подробное описание см. в методе tabRepair::doStateActions()
    if(stateId == Global::RepStateIds::InWork)
    {
        stateActions << QString::number(Global::RepStateActions::SetEngineer);
    }

    // TODO: этот код скопирован из метода tabRepair::doStateActions(), его нужно вынести куда-то в общий доступ
    for(const QString &action : qAsConst(stateActions))
        switch (action.toInt())
        {
            case Global::RepStateActions::NoPayDiag: setPricesToZero(); break;
            case Global::RepStateActions::ResetInformedStatus: setInformedStatus(0); break;
            case Global::RepStateActions::SetEngineer: initEngineer(); break;
            case Global::RepStateActions::InformManager:; break;
            case Global::RepStateActions::InformEngineer:; break;
        }
}

void SCartridgeForm::updatePreagreedAmount(SCartridgeMaterialModel *material, const int state)
{
    m_amount = 0;
    preagreedAmounts.insert(material->type(), (material->price()+material->worksPrice())*(state?1:0));
    QMap<int, double>::const_iterator i = preagreedAmounts.constBegin();
    while(i != preagreedAmounts.constEnd())
    {
        m_amount += i.value();
        i++;
    }
    ui->doubleSpinBoxPreagreedAmount->setValue(m_amount);
    emit updateTotalPreagreedAmount();
}

void SCartridgeForm::setDefaultStyleSheets()
{
    ui->lineEditSerial->setStyleSheet(commonLineEditStyleSheet);
    ui->doubleSpinBoxPreagreedAmount->setStyleSheet(commonSpinBoxStyleSheet);
    ui->doubleSpinBoxTotalAmount->setStyleSheet(commonSpinBoxStyleSheet);
}

void SCartridgeForm::setWidgetsParams(const int stateId)
{
    worksCheckboxesEn = comSettings->useSimplifiedCartridgeRepair;
    engineerComboBoxEn = 0;
    placeComboBoxEn = 0;

    switch(stateId)
    {
        case Global::RepStateIds::GetIn: engineerComboBoxEn = 1; placeComboBoxEn = 1; break;
        case Global::RepStateIds::InWork: worksCheckboxesEn = 1; engineerComboBoxEn = 1; placeComboBoxEn = 1; break;
        case Global::RepStateIds::Ready:
        case Global::RepStateIds::ReadyNoRepair: worksCheckboxesEn = 0; engineerComboBoxEn = 1; placeComboBoxEn = 1; break;
        default: ;
    }
    updateStateWidget(stateId);
}

bool SCartridgeForm::checkStateAcl(const int stateId)
{
    const QString allowedForRoles = statusesModel->value(stateId, Global::RepStateHeaders::Id, Global::RepStateHeaders::Roles).toString();
    if(userDbData->roles.contains(QRegularExpression(QString("\\b(%1)\\b").arg(allowedForRoles))))
    {
        return 1;
    }
    return 0;
}

void SCartridgeForm::setPricesToZero()
{
//    tableWorksParts->setPricesToZero();
    m_repair->setRepairCost(0);
}

void SCartridgeForm::setInformedStatus(int status)
{
    if(m_repair->informedStatusIndex())
        return;

    m_repair->setInformedStatusIndex(status);

    if(m_groupUpdate)
        return;

    commit(tr("Успешно"), tr("Статус информирования клиента обновлён"));
}

void SCartridgeForm::initEngineer()
{
    if(m_repair->engineer())
        return;

    m_repair->setEngineer(userDbData->id);
}

void SCartridgeForm::updateComboBoxEngineer(const int id)
{
    ui->comboBoxEngineer->blockSignals(true);
    ui->comboBoxEngineer->setCurrentIndex(-1);
    ui->comboBoxEngineer->setPlaceholderText(usersModel->getDisplayRole(id));
    ui->comboBoxEngineer->blockSignals(false);
}

void SCartridgeForm::updateLists()
{
    QString recName;
    QString recSumm;
    int itemId = 0;
    int workType;
    int recType;
    QListWidget *list;
    QListWidgetItem *listItem;

    ui->listWidgetWorks->clear();
    ui->listWidgetParts->clear();

    for(int i = 0; i < worksAndPartsModel->rowCount(); i++)
    {
        recType = worksAndPartsModel->index(i, SStoreItemModel::SaleOpColumns::ColRecordType).data().toInt();
        recName = worksAndPartsModel->index(i, SStoreItemModel::SaleOpColumns::ColName).data().toString();
        recSumm = worksAndPartsModel->index(i, SStoreItemModel::SaleOpColumns::ColSumm).data().toString();
        listItem = new QListWidgetItem(QString("%1 %2%3").arg(recName).arg(recSumm).arg(sysLocale.currencySymbol()));    // TODO: мультивалютность
        if(recType == SSaleTableModel::RecordType::Work)
        {
            list = ui->listWidgetWorks;
            workType = worksAndPartsModel->index(i, SStoreItemModel::SaleOpColumns::ColWorkType).data().toInt();
            switch(workType)
            {
                case SWorkModel::Type::CartridgeRefill: ui->checkBoxRefill->setChecked(true); break;
                case SWorkModel::Type::CartridgeDrumReplace: ui->checkBoxDrumReplace->setChecked(true); break;
                case SWorkModel::Type::CartridgeChipReplace: ui->checkBoxChipReplace->setChecked(true); break;
                case SWorkModel::Type::CartridgeBladeReplace: ui->checkBoxBladeReplace->setChecked(true); break;
            }
        }
        else
        {
            list = ui->listWidgetParts;
            itemId = worksAndPartsModel->index(i, SStoreItemModel::SaleOpColumns::ColItemId).data().toInt();
            listItem->setData(Qt::UserRole, itemId);    // для открытия карточки товара при двойном клике
        }

        list->addItem(listItem);
    }
}

bool SCartridgeForm::commit(const QString &notificationCaption, const QString &notificationText)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    i_queryLog = new SQueryLog();
    bool nErr = 1;
    QUERY_LOG_START(metaObject()->className());
    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        m_repair->updateLastSave();
        nErr = m_repair->commit();
        shortlivedNotification *newPopup = new shortlivedNotification(this, notificationCaption, notificationText, QColor(214,239,220), QColor(229,245,234));

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
    }
    QUERY_LOG_STOP;

    delete query;

    return nErr;
}

// Добавление работы и товара.
// Возвращает 0 в случае ошибки.
bool SCartridgeForm::addWorkAndPart(const int workType)
{
    int rowWork = worksAndPartsModel->rowCount();
    int rowItem = -1;
    int itemId = 0;
    int materialType = 0;
    QString workName;
    SCartridgeMaterialModel *material;
    QSqlQuery query(QSqlDatabase::database("connThird"));

    switch(workType)
    {
        case SWorkModel::Type::CartridgeRefill: workName = tr("Заправка"); materialType = SCartridgeMaterialModel::Toner; break;
        case SWorkModel::Type::CartridgeDrumReplace: workName = tr("Замена фотобарабана"); materialType = SCartridgeMaterialModel::Drum; break;
        case SWorkModel::Type::CartridgeChipReplace: workName = tr("Замена чипа"); materialType = SCartridgeMaterialModel::Chip; break;
        case SWorkModel::Type::CartridgeBladeReplace: workName = tr("Замена лезвия"); materialType = SCartridgeMaterialModel::Blade; break;
    }

    material =  m_cartridgeCard->material(materialType);
    if(material->articul()) // если артикул не задан, подразумевается работа без склада
    {
        query.exec(QUERY_SEL_CARTRIDGE_MATERIAL(material->articul(), material->count()));
        if(!query.first())
        {
            shortlivedNotification *newPopup = new shortlivedNotification(this,
                                                                          tr("Ошибка"),
                                                                          tr("Кол-во больше наличия, списание не возможно"),
                                                                          QColor(255,164,119),
                                                                          QColor(255,199,173));
            return 0;
        }

        itemId = query.value(0).toInt();
    }

    worksAndPartsModel->addCustomWork();
    worksAndPartsModel->setData(worksAndPartsModel->index(rowWork, SStoreItemModel::SaleOpColumns::ColName), workName);
    worksAndPartsModel->setData(worksAndPartsModel->index(rowWork, SStoreItemModel::SaleOpColumns::ColCount), 1);
    worksAndPartsModel->setData(worksAndPartsModel->index(rowWork, SStoreItemModel::SaleOpColumns::ColPrice), material->worksPrice());
    worksAndPartsModel->setData(worksAndPartsModel->index(rowWork, SStoreItemModel::SaleOpColumns::ColWorkType), workType);
//    worksAndPartsModel->setData(worksAndPartsModel->index(rowWork, SStoreItemModel::SaleOpColumns::ColSumm), material->worksPrice());

    rowItem = worksAndPartsModel->rowCount();
    if(material->articul())
    {
        worksAndPartsModel->addItemByUID(itemId, material->count());
#ifdef QT_DEBUG // в методе SSaleTableModel::insertRecord() для удобства отладки устанавливается случайное кол-во; здесь это не нужно
        worksAndPartsModel->setData(worksAndPartsModel->index(rowItem, SStoreItemModel::SaleOpColumns::ColCount), material->count());
#endif
        worksAndPartsModel->setData(worksAndPartsModel->index(rowItem, SStoreItemModel::SaleOpColumns::ColPrice), material->price()/material->count());
        //    worksAndPartsModel->setData(worksAndPartsModel->index(rowItem, SStoreItemModel::SaleOpColumns::ColSumm), material->count()*material->worksPrice());
    }

    return worksAndPartsModel->repair_saveTablesStandalone();
}

bool SCartridgeForm::removeWorkAndPart(const int workType)
{
    int nErr = 1;
    int row = 0;
    for(; row < worksAndPartsModel->rowCount(); row++)
    {
        if(worksAndPartsModel->index(row, SStoreItemModel::SaleOpColumns::ColWorkType).data().toInt() == workType)
            break;
    }
    worksAndPartsModel->removeRowHandler(row, worksAndPartsModel->index(row, SStoreItemModel::SaleOpColumns::ColId).data().toInt());
    nErr = worksAndPartsModel->repair_saveTablesStandalone();

    return nErr;
}

bool SCartridgeForm::workAndPartHandler(const int workType, const int checkboxState)
{
    int ret;
    if(comSettings->useSimplifiedCartridgeRepair && m_repair->state() == Global::RepStateIds::GetIn)
        saveState(Global::RepStateIds::InWork);

    if(checkboxState)
    {
        ret = addWorkAndPart(workType);
    }
    else
    {
        ret = removeWorkAndPart(workType);
    }
    return ret;
}

/* Проверка превышения ресурса заправок
 * Возвращает 1 если по серийному номеру было произведено больше заправок, чем задано в карточке картриджа.
 * Cм. описание метода SCartridgeCardModel::resource()
*/
bool SCartridgeForm::checkResource()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    bool ret = 0;

    query->exec(QUERY_SEL_CARTRIDGE_RESOURCE(m_repairId, m_serialNumber, SWorkModel::Type::CartridgeRefill));
    if(query->first())
    {
        if(query->value(0).toInt() >= m_cartridgeCard->resource())
            ret = 1;
    }

    delete query;

    return ret;
}

int SCartridgeForm::checkInput()
{
    int error = 0;
    setDefaultStyleSheets();

    if (ui->lineEditSerial->text() == "" && comSettings->isCartridgeSerialNumberRequired)
    {
        ui->lineEditSerial->setStyleSheet(commonLineEditStyleSheetRed);
        error = 1;
    }
    if(ui->doubleSpinBoxPreagreedAmount->value() == 0)
    {
        ui->doubleSpinBoxPreagreedAmount->setStyleSheet(commonSpinBoxStyleSheetRed);
        error = 2;
    }
    if (error)
    {
        qDebug() << "Ошибка создания карточки заправки: не все обязательные поля заполнены (error " << error << ")";
        return error;
    }

    return false;
}

int SCartridgeForm::isReady()
{
    switch(m_repair->state())
    {
        case Global::RepStateIds::InWork: return (comSettings->useSimplifiedCartridgeRepair && worksAndPartsModel->rowCount());
        case Global::RepStateIds::Ready:
        case Global::RepStateIds::ReadyNoRepair: return 1;
    }

    return 0;
}

void SCartridgeForm::updateStatesModel(const int statusId)
{

    QString allowedStates;// = statusesProxyModel->value(statusId, Global::RepStateHeaders::Id, Global::RepStateHeaders::Contains).toString();
    switch(statusId)
    {
        case Global::RepStateIds::GetIn: allowedStates = QString::number(Global::RepStateIds::InWork); break;
        case Global::RepStateIds::InWork: allowedStates = QString("%1|%2").arg(Global::RepStateIds::Ready).arg(Global::RepStateIds::ReadyNoRepair); break;
        case Global::RepStateIds::Ready: allowedStates = "---"; break;
        case Global::RepStateIds::ReadyNoRepair: allowedStates = "---"; break;
        default: allowedStates = "---";
    }
    statusesProxyModel->setFilterRegularExpression(QString("\\b(%1)\\b").arg(allowedStates));

    emit updateParentTab();
}

void SCartridgeForm::updateStateWidget(const int statusId)
{
    QString activeState = statusesModel->getDisplayRole(statusId);
    ui->comboBoxState->blockSignals(true);
    ui->comboBoxState->setCurrentIndex(-1);
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    ui->comboBoxState->setPlaceholderText(activeState);
    updateStatesModel(statusId);
    ui->comboBoxState->blockSignals(false);
}

void SCartridgeForm::updateTotalSumms(const double, const double, const double)
{
    ui->doubleSpinBoxTotalAmount->setValue(worksAndPartsModel->amountTotal());
}

void SCartridgeForm::saveTotalSumms()
{
    m_repair->setRealRepairCost(worksAndPartsModel->amountTotal());
    m_repair->setPartsCost(worksAndPartsModel->amountItems());
    m_repair->commit();
}

void SCartridgeForm::setRefill(int state)
{
    SCartridgeMaterialModel *material = m_cartridgeCard->material(SCartridgeMaterialModel::Toner);
    updatePreagreedAmount(material, state);
}

void SCartridgeForm::setChipReplace(int state)
{
    SCartridgeMaterialModel *material = m_cartridgeCard->material(SCartridgeMaterialModel::Chip);
    updatePreagreedAmount(material, state);
}

void SCartridgeForm::setDrumReplace(int state)
{
    SCartridgeMaterialModel *material = m_cartridgeCard->material(SCartridgeMaterialModel::Drum);
    updatePreagreedAmount(material, state);
}

void SCartridgeForm::setBladeReplace(int state)
{
    SCartridgeMaterialModel *material = m_cartridgeCard->material(SCartridgeMaterialModel::Blade);
    updatePreagreedAmount(material, state);
}

void SCartridgeForm::savePlace(int index)
{
    int currentPlace = m_repair->boxIndex();

    if(currentPlace == index)
        return;

    m_repair->setBoxIndex(index);
    if(!commit())
    {
        ui->comboBoxPlace->blockSignals(true);
        ui->comboBoxPlace->setCurrentIndex(currentPlace);
        ui->comboBoxPlace->blockSignals(false);
    }
}

void SCartridgeForm::comboBoxPlaceButtonClickHandler(int id)
{
    if(id == SLineEdit::Clear)
        savePlace(-1);
}

void SCartridgeForm::comboBoxEngineerChanged(int index)
{
    m_repair->setEngineerIndex(index);
    commit();
}

void SCartridgeForm::stateIndexChanged(int index)
{
    if (index < 0)
        return;

    saveState(statusesProxyModel->databaseIDByRow(index));
}

void SCartridgeForm::saveState(int stateId)
{
    if (stateId < 0)
        return;

    m_groupUpdate = 1;

    try
    {
        if(!checkStateAcl(stateId))
        {
            shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Проверьте права доступа или обратитесь к администратору"), QColor(212,237,242), QColor(229,244,247));
            throw Global::ThrowType::ConditionsError;
        }
        checkData(stateId);
        doStateActions(stateId);
        m_repair->setState(stateId);

        if(!commit())
            throw Global::ThrowType::QueryError;
    }
    catch (Global::ThrowType type)
    {
        ui->comboBoxState->blockSignals(true);
        ui->comboBoxState->setCurrentIndex(-1);
        ui->comboBoxState->blockSignals(false);
        return;
    }
    updateWidgets();
    emit updateParentTab();

    m_groupUpdate = 0;
}

void SCartridgeForm::removeWidget()
{
    this->deleteLater();
}

void SCartridgeForm::buttonClientCardClicked()
{
    emit createTabClient(m_repair->clientId());
}

void SCartridgeForm::buttonClassicTabClicked()
{
    emit createTabRepair(m_repair->id());
}

void SCartridgeForm::buttonCartridgeCardClicked()
{
    emit createCartridgeCardForm(m_cardId);
}

void SCartridgeForm::updateComment()
{
    m_repair->setExtNotes(ui->lineEditComment->text());
    if(m_repair->commit())
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Успешно"), tr("Примечание сохранено"), QColor(214,239,220), QColor(229,245,234));
}
