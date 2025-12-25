#include "scartridgeform.h"
#include "ui_scartridgeform.h"
#include <QMenu>
#include <tabRepairs>
#include <SUserSettings>
#include <SComSettings>
#include <SPermissions>
#include <SRepairModel>
#include <SCartridgeRepairModel>
#include <SCartridgeCardModel>
#include <SCartridgeMaterialsModel>
#include <SSortFilterProxyModel>
#include <SSqlQueryModel>
#include <SSaleTableModel>
#include <FlashPopup>

SCartridgeForm::SCartridgeForm(QWidget *parent) :
    SFrame(parent),
    ui(new Ui::SCartridgeForm),
    m_lineEditStyleSheet(commonLineEditStyleSheet),
    m_spinBoxStyleSheet(commonSpinBoxStyleSheet)
{
    ui->setupUi(this);
    guiFontChanged();
    setDefaultStyleSheets();

//    ui->frameHeader->setStyleSheet("QFrame::hover {background-color: rgb(205,230,247);}");    // подсветка заголовка

    connect(userDbData, &SUserSettings::fontSizeChanged, this, &SCartridgeForm::guiFontChanged);
    installEventFilter(this);
}

SCartridgeForm::~SCartridgeForm()
{
    delete ui;
    delete m_cartridgeCard; // перед удалением модели m_repair
    if(m_repair)
        delete m_repair;
    if(m_BOQModel)
        delete m_BOQModel;
    if(statusesProxyModel)
        delete statusesProxyModel;
}

void SCartridgeForm::initModels()
{
    m_cartridgeCard = new SCartridgeCardModel(this);
    if(m_repairId)
    {
        m_repair = new SRepairModel();
        m_cartridgeCard->setParent(m_repair);

        statusesProxyModel = new SSortFilterProxyModel;
        statusesProxyModel->setSourceModel(comSettings->repairStatusesVariantCopy().Model);
        statusesProxyModel->setFilterKeyColumn(Global::RepStateHeaders::Id);
        statusesProxyModel->setFilterRegularExpression("");

        m_BOQModel = new WorkshopSaleModel();
        m_BOQModel->setPriceColumn(SStoreItemModel::PriceOptionService);
        connect(m_BOQModel, &SSaleTableModel::amountChanged, this, &SCartridgeForm::updateTotalSumms);
        connect(m_BOQModel, &SSaleTableModel::tableSaved, this, &SCartridgeForm::saveTotalSumms);
        connect(m_BOQModel, &SSaleTableModel::modelReset, this, &SCartridgeForm::updateLists);
        connect(m_BOQModel, &SSaleTableModel::modelReset, this, &SCartridgeForm::updateWorksActionsCheckedState);
        m_BOQModel->setEditStrategy(SSaleTableModel::OnManualSubmit);
        m_BOQModel->setRepairType(WorkshopSaleModel::Cartridge);
        m_BOQModel->setCartridgeCardModel(m_cartridgeCard);
        m_repair->setBOQModel(m_BOQModel);
    }
}

void SCartridgeForm::configureWidgets()
{
    QIcon activeCheckBoxIcon;
    activeCheckBoxIcon.addFile(QString::fromUtf8(":/icons/light/checkbox-unchecked.png"), QSize(), QIcon::Normal, QIcon::Off);
    activeCheckBoxIcon.addFile(QString::fromUtf8(":/icons/light/checkbox-checked.png"), QSize(), QIcon::Normal, QIcon::On);

    QIcon unactiveCheckBoxIcon;
    unactiveCheckBoxIcon.addFile(QString::fromUtf8(":/icons/light/checkbox-flat-unchecked.png"), QSize(), QIcon::Normal, QIcon::Off);
    unactiveCheckBoxIcon.addFile(QString::fromUtf8(":/icons/light/checkbox-flat-checked.png"), QSize(), QIcon::Normal, QIcon::On);

    ui->comboBoxWasEarly->setModel(cartridgeRepeatReason);
    ui->comboBoxWasEarly->setCurrentIndex(-1);
    ui->comboBoxWasEarly->disableWheelEvent(true);
    ui->comboBoxWasEarly->view()->setMinimumWidth(230);
    ui->comboBoxPlace->setButtons("Clear");
    ui->comboBoxPlace->setModel(repairBoxesModel);
    ui->comboBoxPlace->setCurrentIndex(-1);
    ui->comboBoxPlace->disableWheelEvent(true);
    ui->comboBoxPlace->view()->setMinimumWidth(150);
    connect(ui->comboBoxPlace, qOverload<int>(&QComboBox::currentIndexChanged), this, &SCartridgeForm::savePlace);
    connect(ui->comboBoxPlace, &SComboBox::buttonClicked, this, &SCartridgeForm::comboBoxPlaceButtonClickHandler);
    ui->labelLimitReached->setVisible(prevRepairsCount() >= m_cartridgeCard->resource());
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &SCartridgeForm::removeWidget);
    ui->comboBoxState->view()->setMinimumWidth(230);
    ui->labelRepeatWarranty->setVisible(false);

    if(m_repairId)
    {
        initWorksMenu();
        m_lineEditStyleSheet.replace("background: #FFFFFF;", "background-color: rgba(255, 255, 255, 0);");
        m_spinBoxStyleSheet.replace("background: #FFFFFF;", "background-color: rgba(255, 255, 255, 0);");
        ui->lineEditSerial->setStyleSheet(m_lineEditStyleSheet);
        ui->spinBoxRefillWeight->setStyleSheet(m_spinBoxStyleSheet);
        ui->doubleSpinBoxPreagreedAmount->setStyleSheet(m_spinBoxStyleSheet);

        initCheckBoxWidgets(unactiveCheckBoxIcon, ui->pushButtonPreagreedRefill, true, true);
        initCheckBoxWidgets(unactiveCheckBoxIcon, ui->pushButtonPreagreedChipReplace, true, true);
        initCheckBoxWidgets(unactiveCheckBoxIcon, ui->pushButtonPreagreedDrumReplace, true, true);
        initCheckBoxWidgets(unactiveCheckBoxIcon, ui->pushButtonPreagreedBladeReplace, true, true);
        ui->comboBoxEngineer->setModel(engineersModel);
        ui->comboBoxEngineer->disableWheelEvent(true);
        connect(ui->comboBoxEngineer, qOverload<int>(&QComboBox::currentIndexChanged), this, &SCartridgeForm::comboBoxEngineerChanged);
        ui->comboBoxState->setModel(statusesProxyModel);
        ui->comboBoxState->setCurrentIndex(-1);
        ui->comboBoxState->disableWheelEvent(true);
        connect(ui->comboBoxState, qOverload<int>(&QComboBox::currentIndexChanged), this, &SCartridgeForm::stateIndexChanged);
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonRefill, false, true);
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonChipReplace, false, true);
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonDrumReplace, false, true);
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonBladeReplace, false, true);
        connect(ui->pushButtonClientCard, &QPushButton::clicked, this, &SCartridgeForm::buttonClientCardClicked);
        connect(ui->toolButtonClassicTab, &QPushButton::clicked, this, &SCartridgeForm::buttonClassicTabClicked);
        connect(ui->toolButtonCartridgeCard, &QPushButton::clicked, this, &SCartridgeForm::buttonCartridgeCardClicked);
        connect(ui->lineEditComment, &QLineEdit::editingFinished, this, &SCartridgeForm::updateComment);
        ui->labelWasEarly->hide();
        ui->comboBoxWasEarly->hide();

        updateHeader();
        updateWidgets();
    }
    else
    {
        hideWidgetsOnReceiptForm();
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonPreagreedRefill);
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonPreagreedChipReplace);
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonPreagreedDrumReplace);
        initCheckBoxWidgets(activeCheckBoxIcon, ui->pushButtonPreagreedBladeReplace);
        ui->comboBoxWasEarly->setCurrentIndex(m_isRepeat?1:-1);

        connect(ui->pushButtonPreagreedRefill, &QPushButton::toggled, this, &SCartridgeForm::setRefill);
        connect(ui->pushButtonPreagreedChipReplace, &QPushButton::toggled, this, &SCartridgeForm::setChipReplace);
        connect(ui->pushButtonPreagreedDrumReplace, &QPushButton::toggled, this, &SCartridgeForm::setDrumReplace);
        connect(ui->pushButtonPreagreedBladeReplace, &QPushButton::toggled, this, &SCartridgeForm::setBladeReplace);
        ui->pushButtonPreagreedRefill->setChecked(true);
    }
}

void SCartridgeForm::load()
{
    if(m_repairId)
    {
        m_repair->load(m_repairId);

        m_serialNumber = m_repair->serialNumber();
        setCardId(m_repair->cartridge()->cardId());
        m_clientId = m_repair->clientId();
        m_BOQModel->loadTable(m_repairId);
    }

    loadCardData();
}

void SCartridgeForm::loadCardData()
{
    m_cartridgeCard->load(m_cardId);
}

void SCartridgeForm::reloadData()
{
    try
    {
        m_repair->load(m_repairId);
        m_BOQModel->loadTable(m_repairId);
    }
    catch (Global::ThrowType)
    {
        // TODO: деактивация формы, наглядно дающая понять, что произошла ошибка
    }
}

void SCartridgeForm::randomFill()
{
    if(ui->lineEditSerial->text().isEmpty())
        ui->lineEditSerial->setText("T" + QString::number(QRandomGenerator::global()->bounded(9999)));

    int i = QRandomGenerator::global()->bounded(16384);

    ui->pushButtonPreagreedRefill->setChecked((i>>0)&0x01);
    ui->pushButtonPreagreedChipReplace->setChecked((i>>1)&0x01);
    ui->pushButtonPreagreedDrumReplace->setChecked((i>>2)&0x01);
    ui->pushButtonPreagreedBladeReplace->setChecked((i>>3)&0x01);
}

void SCartridgeForm::setRepair(int id)
{
    m_repairId = id;
}

void SCartridgeForm::init()
{
    initModels();
    load();
    configureWidgets();
    m_initDone = 1;
}

/* Настройка виджетов предварительно согласованных работ
 * isFlat: 0 - форма приёма картриджей; 1 - форма работы (заправки) с картриджем
*/
void SCartridgeForm::initCheckBoxWidgets(const QIcon &icon, QPushButton *button, bool isFlat, bool installEventFilter)
{
    button->setCheckable(true);
    button->setIcon(icon);
    button->setStyleSheet(QString::fromUtf8("QPushButton {"
                                            "  background-color: rgba(240, 240, 240, 0);"
                                            "}"));
    if(isFlat)
    {
        button->setFlat(true);
    }

    if(installEventFilter)
    {
        button->installEventFilter(this);
    }
}

void SCartridgeForm::updateHeader()
{
    ui->labelTitle->setText(QString("%1 [%2]").arg(m_repair->id()).arg(m_repair->title()));

    SClientModel *client = new SClientModel(m_repair->clientId());
    QString name = client->fullLongName().toString();
    if(this->fontMetrics().horizontalAdvance(name) > 400)   // TODO: заменить жестко заданное значение
    {
        if(client->isCompany() && !client->shortName().isEmpty() )
            name = client->shortName();
        else if(!client->isCompany())
            name = client->fullShortName().toString();
        else
            ui->pushButtonClientCard->setMaximumWidth(400);
    }
    ui->pushButtonClientCard->setText(name);
    delete client;  // TODO: модель нужна только для получения ФИО; подумать над более оптимальным способом.
}

void SCartridgeForm::updateWidgets()
{
    updateStatesModel();
    updateWidgetsFlagsToState();
    updateWorksCheckBoxes();
    updateWorksActionsCheckedState();
    ui->labelRepeatWarranty->setVisible(false);
    if(m_repair->isRepeat() || m_prevRepairsCount)
        updateLabelRepeatWarranty(tr("Повтор"));

    if(m_repair->isWarranty())
        updateLabelRepeatWarranty(tr("Гарантия"));

    ui->lineEditSerial->setText(m_repair->serialNumber());
    ui->lineEditSerial->setReadOnly(true);
    ui->pushButtonPreagreedRefill->setChecked(m_repair->cartridge()->refill());
    ui->pushButtonPreagreedChipReplace->setChecked(m_repair->cartridge()->chip());
    ui->pushButtonPreagreedDrumReplace->setChecked(m_repair->cartridge()->drum());
    ui->pushButtonPreagreedBladeReplace->setChecked(m_repair->cartridge()->blade());
    ui->spinBoxRefillWeight->setValue(m_cartridgeCard->tonerWeight());
    ui->doubleSpinBoxPreagreedAmount->setValue(m_repair->preAgreedAmount());
    ui->comboBoxWasEarly->setCurrentIndex(m_repair->isRepeat()?1:-1);
    ui->comboBoxWasEarly->setCurrentIndex(m_repair->isWarranty()?0:-1);
    ui->comboBoxWasEarly->setEnabled(false);


    ui->comboBoxEngineer->setEnabled(engineerComboBoxEn && (permissions->setRepairEngineer || permissions->beginUnengagedRepair));
    updateComboBoxEngineer();
    ui->doubleSpinBoxTotalAmount->setValue(m_repair->realRepairCost());
    updateComboBoxPlace();
    ui->comboBoxPlace->setEnabled(placeComboBoxEn);
    ui->lineEditComment->setText(m_repair->extNotes());
}

void SCartridgeForm::updateWorksCheckBoxes()
{
    ui->pushButtonRefill->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Toner));
    ui->pushButtonChipReplace->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Chip));
    ui->pushButtonDrumReplace->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Drum));
    ui->pushButtonBladeReplace->setEnabled(worksCheckboxesEn && m_cartridgeCard->isMaterialSet(SCartridgeMaterialModel::Blade));
    ui->toolButtonOtherWorksMenu->setEnabled(worksCheckboxesEn);
}

/*  Скрытие виджетов, не используемых при приёме картриджа
*/
void SCartridgeForm::hideWidgetsOnReceiptForm()
{
    ui->labelRepeatWarranty->hide();
    ui->pushButtonClientCard->hide();
    ui->toolButtonClassicTab->hide();
    ui->toolButtonCartridgeCard->hide();

    ui->labelRefillWeight->hide();
    ui->spinBoxRefillWeight->hide();    // в АСЦ при приёме это поле отображается, но в нём нет смысла
    ui->labelEngineer->hide();
    ui->comboBoxEngineer->hide();
    ui->labelRefill->hide();
    ui->pushButtonRefill->hide();
    ui->labelRealRefillWeight->hide();
    ui->spinBoxRealRefillWeight->hide();
    ui->labelDrumReplace->hide();
    ui->pushButtonDrumReplace->hide();
    ui->labelChipReplace->hide();
    ui->pushButtonChipReplace->hide();
    ui->labelBladeReplace->hide();
    ui->pushButtonBladeReplace->hide();
    ui->labelState->hide();
    ui->comboBoxState->hide();
    ui->labelTotalAmount->hide();
    ui->doubleSpinBoxTotalAmount->hide();
    ui->labelOtherWorks->hide();
    ui->toolButtonOtherWorksMenu->hide();

    ui->listWidgetWorks->hide();
    ui->listWidgetParts->hide();
}

void SCartridgeForm::updateLabelRepeatWarranty(const QString text)
{
    ui->labelRepeatWarranty->setText(text);
    ui->labelRepeatWarranty->setVisible(true);
}

bool SCartridgeForm::eventFilter(QObject *watched, QEvent *event)
{
//    if(event->type() != QEvent::Paint)
//        qDebug().nospace() << "[" << this << "] eventFilter() | watched: " << m_repair->objectName() << "; event->type(): " << event->type();

    // TODO: выделение виджета (фокус):
    // QEvent::MouseButtonPress
    // любое взаимодействие с дочерними виджетами

    if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick)
    {
        if(!m_initDone)     // Если инициализация моделей и виджетов не выполнена, то обработка событий не должна выполняться
            return false;

        QPushButton *checkBoxWidget = dynamic_cast<QPushButton*>(watched);
        if(checkBoxWidget)
        {
            if(static_cast<QMouseEvent*>(event)->button() != Qt::MouseButton::LeftButton)   // Achtung! Щелчек любой кнопкой мыши, кроме левой, по любому QPushButton на форме будет проигнорирован
                return true;

            if( checkBoxWidget == ui->pushButtonPreagreedRefill ||
                checkBoxWidget == ui->pushButtonPreagreedDrumReplace ||
                checkBoxWidget == ui->pushButtonPreagreedChipReplace ||
                checkBoxWidget == ui->pushButtonPreagreedBladeReplace )
                return true;

            if(!checkBoxWidget->isEnabled())
                return true;

            int ret = 1;
            int nSt = checkBoxWidget->isChecked()?0:2;
            if(checkBoxWidget == ui->pushButtonRefill)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeRefill, nSt);
            else if(checkBoxWidget == ui->pushButtonChipReplace)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeChipReplace, nSt);
            else if(checkBoxWidget == ui->pushButtonDrumReplace)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeDrumReplace, nSt);
            else if(checkBoxWidget == ui->pushButtonBladeReplace)
                ret = !workAndPartHandler(SWorkModel::Type::CartridgeBladeReplace, nSt);
            return nSt?1:ret; // при добавлении работ нужно фильтровать событие, а при удалении нет
        }
    }

//    QAction *worksMenuAction = dynamic_cast<QAction*>(watched);
//    if(worksMenuAction)
//    {
//        workAndPartHandler(worksMenuAction->property("WorkType").toInt(), worksMenuAction->isChecked());
//    }

    return false;
}

SRepairModel *SCartridgeForm::model()
{
    return m_repair;
}

void SCartridgeForm::beginCreateRepair()
{
    m_repair = new SRepairModel();

    m_repair->initCartridgeRepairModel(m_cardId);

    QStringList faultList;
    if(ui->pushButtonPreagreedRefill->isChecked())
    {
        faultList.append(tr("Заправка"));
        m_repair->cartridge()->set_refill(1);
    }
    if(ui->pushButtonPreagreedChipReplace->isChecked())
    {
        faultList.append(tr("Чип"));
        m_repair->cartridge()->set_chip(1);
    }
    if(ui->pushButtonPreagreedDrumReplace->isChecked())
    {
        faultList.append(tr("Фотовал"));
        m_repair->cartridge()->set_drum(1);
    }
    if(ui->pushButtonPreagreedBladeReplace->isChecked())
    {
        faultList.append(tr("Ракель"));
        m_repair->cartridge()->set_blade(1);
    }
    m_repair->set_title(m_title);
    m_repair->set_classId(m_deviceClassId);
    m_repair->set_vendorId(m_deviceVendorId);
    m_repair->set_clientId(m_clientId);
    m_repair->set_fault(""+faultList.join(", "));
    m_repair->set_serialNumber(ui->lineEditSerial->text());
    m_repair->set_extNotes(ui->lineEditComment->text());
    switch(ui->comboBoxWasEarly->currentIndex())
    {
        case -1: m_repair->set_isWarranty(0); m_repair->set_isRepeat(0); break;
        case 0: m_repair->set_isWarranty(1); m_repair->set_isRepeat(0); break;
        case 1: m_repair->set_isRepeat(1); m_repair->set_isWarranty(0); break;
    }
    m_repair->set_preAgreedAmount(ui->doubleSpinBoxPreagreedAmount->value());
    m_repair->set_rejectReason("");
    m_repair->setCompanyIndex(m_companyIndex);
    m_repair->setOfficeIndex(m_officeIndex);
    m_repair->setStartOfficeIndex(m_officeIndex);
    m_repair->set_managerId(userDbData->id());
    m_repair->set_currentManagerId(userDbData->id());
    m_repair->setEngineerIndex(m_engineerIndex);
    if(m_isHighPriority)
        m_repair->set_isExpressRepair(1);
    if( m_paymentSystemIndex >= 0)
    {
        m_repair->setPaymentSystemIndex(m_paymentSystemIndex);
        if(paymentSystemsModel->databaseIDByRow(m_paymentSystemIndex, "system_id") == -1)
            m_repair->set_isCardPayment(1);
    }
    if(ui->comboBoxPlace->currentIndex() >= 0)
    {
        m_repair->setBoxIndex(ui->comboBoxPlace->currentIndex());
    }
}

void SCartridgeForm::createRepair()
{
    m_opType = CreateRepair;
    submit();
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

int SCartridgeForm::checkBeforeStateChange()
{
    if(ui->comboBoxState->currentIndex() == -1) // SAbstractItemModel::databaseIDByRow() для -1 вернёт 0, соответствующий статусу Приём в ремонт
        return 1;

    bool err = 0;
    int stateId = statusesProxyModel->databaseIDByRow(ui->comboBoxState->currentIndex());
    if(!checkStateAcl(stateId))
        err = 1;

    if(stateId == Global::RepStateIds::Ready)
    {
        if(m_BOQModel->amountTotal() == 0)
        {
            ui->doubleSpinBoxTotalAmount->setStyleSheet(commonSpinBoxStyleSheetRed);
            err = 1;
        }
    }

    if(err)
    {
        updateStatesModel();
        return 1;
    }

    return 0;
}

void SCartridgeForm::beginSaveState(const int stateId)
{
    QList<int> stateActions = comSettings->repairStatusesVariantCopy()[stateId].Actions;

    // подробное описание см. в методе tabRepair::doStateActions()
    if(stateId == Global::RepStateIds::InWork && permissions->beginUnengagedRepair)
    {
        stateActions << Global::RepStateActions::SetEngineer;
    }

    // TODO: этот код скопирован из метода tabRepair::doStateActions(), его нужно вынести куда-то в общий доступ
    for(const int &action : qAsConst(stateActions))
    {
        switch (action)
        {
            case Global::RepStateActions::NoPayDiag: setPricesToZero(); break;
            case Global::RepStateActions::ResetInformedStatus: if(m_repair->informedStatusIndex()) m_repair->setInformedStatusIndex(0); break;
            case Global::RepStateActions::SetEngineer: initEngineer(); break;
            case Global::RepStateActions::InformManager:; break;
            case Global::RepStateActions::InformEngineer:; break;
        }
    }
    m_repair->set_stateId(stateId);
}

void SCartridgeForm::updatePreagreedAmount(std::shared_ptr<SCartridgeMaterialModel> material, const bool state)
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
    ui->lineEditSerial->setStyleSheet(m_lineEditStyleSheet);
    ui->spinBoxRefillWeight->setStyleSheet(m_spinBoxStyleSheet);
    ui->doubleSpinBoxPreagreedAmount->setStyleSheet(m_spinBoxStyleSheet);
    ui->doubleSpinBoxTotalAmount->setStyleSheet(commonSpinBoxStyleSheet);
}

void SCartridgeForm::updateWidgetsFlagsToState()
{
    int stateId = m_repair->stateId();
    worksCheckboxesEn = comSettings->useSimplifiedCartridgeRepair();
    engineerComboBoxEn = 0;
    placeComboBoxEn = 0;

    switch(stateId)
    {
        case Global::RepStateIds::GetIn: engineerComboBoxEn = 1; placeComboBoxEn = 1; break;
        case Global::RepStateIds::InWork: worksCheckboxesEn = 1; engineerComboBoxEn = 1; placeComboBoxEn = 1; break;
        case Global::RepStateIds::Ready:
        case Global::RepStateIds::ReadyNoRepair: worksCheckboxesEn = 0; engineerComboBoxEn = 1; placeComboBoxEn = 1; break;
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair:
        case Global::RepStateIds::ReturnedInCredit: worksCheckboxesEn = 0; break;
        default: ;
    }
}

/* Проверка права пользователя изменять статус на stateId
 * Возвращает 1, если пользователь обладает правом
*/
bool SCartridgeForm::checkStateAcl(const int stateId)
{
    // проверка разрешенённых для статуса ролей (Настройки -» Статусы ремонта)
    const QString allowedForRoles = comSettings->repairStatusesVariantCopy()[stateId].Roles.join('|');
    if(userDbData->roles().contains(QRegularExpression(QString("\\b(%1)\\b").arg(allowedForRoles))))
    {
        return 1;
    }

    Global::errorPopupMsg(Global::ThrowType::AccessDenied);
    return 0;
}

void SCartridgeForm::setPricesToZero()
{
//    tableWorksParts->setPricesToZero();
    m_repair->set_repairCost(0);
}

void SCartridgeForm::setInformedStatus(int)
{
    m_opType = SaveInformedState;
    manualSubmit();
}

void SCartridgeForm::initEngineer()
{
    if(m_repair->engineerId())
        return;

    m_repair->set_engineerId(userDbData->id());
}

void SCartridgeForm::updateComboBoxEngineer()
{
    ui->comboBoxEngineer->blockSignals(true);
    ui->comboBoxEngineer->setCurrentIndex(-1);
    ui->comboBoxEngineer->setPlaceholderText(usersModel->getDisplayRole(m_repair->engineerId()));
    ui->comboBoxEngineer->blockSignals(false);
}

void SCartridgeForm::updateLists()
{
    QString recName;
    QString recSumm;
    int itemId = 0;
    int workType;
    QListWidget *list;
    QListWidgetItem *listItem;

    ui->listWidgetWorks->clear();
    ui->listWidgetParts->clear();

    for(int i = 0; i < m_BOQModel->rowCount(); i++)
    {
        recName = m_BOQModel->index(i, SSaleTableModel::Columns::Name).data().toString();
        recSumm = m_BOQModel->index(i, SSaleTableModel::Columns::Amount).data().toString();
        listItem = new QListWidgetItem(QString("%1 %2%3").arg(recName).arg(recSumm).arg(sysLocale.currencySymbol()));    // TODO: мультивалютность
        if(m_BOQModel->recordType(i) == SSaleTableModel::RecordType::Work)
        {
            list = ui->listWidgetWorks;
            workType = m_BOQModel->index(i, SSaleTableModel::Columns::WorkType).data().toInt();
            setWorkCheckBoxChecked(workType);
        }
        else
        {
            list = ui->listWidgetParts;
            itemId = m_BOQModel->index(i, SSaleTableModel::Columns::ItemId).data().toInt();
            listItem->setData(Qt::UserRole, itemId);    // для открытия карточки товара при двойном клике
        }

        list->addItem(listItem);
    }
}

bool SCartridgeForm::checkWorkMaterialItemCount()
{
    if(m_repair->stateId() == Global::RepStateIds::GetIn)
    {
        if(comSettings->useSimplifiedCartridgeRepair())
        {
            if(!checkStateAcl(Global::RepStateIds::InWork))
                return 1;
        }
    }

    m_workMaterialItemId = 0;
    auto material =  m_cartridgeCard->material((SWorkModel::Type)m_workType);
    auto workMaterial = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::session()));

    if(material->articul()) // если артикул не задан, подразумевается работа без склада
    {
        workMaterial->exec(QUERY_SEL_CARTRIDGE_MATERIAL(material->articul(), material->count()));
        if(!workMaterial->first())
        {
            Global::errorPopupMsg(tr("Необходимое кол-во материалов превышает остатки"));
            return 1;
        }

        m_workMaterialItemId = workMaterial->value(0).toInt();
    }

    return 0;
}

void SCartridgeForm::beginAddWorkAndPart()
{
    int rowWork = m_BOQModel->rowCount();
    int rowItem = -1;
    auto material =  m_cartridgeCard->material((SWorkModel::Type)m_workType);

    m_BOQModel->addCustomWork();
    m_BOQModel->setData(m_BOQModel->index(rowWork, SSaleTableModel::Columns::Name), material->workName());
    m_BOQModel->setData(m_BOQModel->index(rowWork, SSaleTableModel::Columns::Count), 1);
    m_BOQModel->setData(m_BOQModel->index(rowWork, SSaleTableModel::Columns::Price), material->worksPrice());
    m_BOQModel->setData(m_BOQModel->index(rowWork, SSaleTableModel::Columns::WorkType), m_workType);
//    m_BOQModel->setData(m_BOQModel->index(rowWork, SSaleTableModel::Columns::Amount), material->worksPrice());

    rowItem = m_BOQModel->rowCount();
    if(material->articul() && m_workMaterialItemId)
    {
        m_BOQModel->addItemByID(m_workMaterialItemId, material->count());
#ifdef QT_DEBUG // в методе SSaleTableModel::insertRecord() для удобства отладки устанавливается случайное кол-во; здесь это не нужно
        m_BOQModel->setData(m_BOQModel->index(rowItem, SSaleTableModel::Columns::Count), material->count());
#endif
        m_BOQModel->setData(m_BOQModel->index(rowItem, SSaleTableModel::Columns::Price), material->price()/material->count());
        //    m_BOQModel->setData(m_BOQModel->index(rowItem, SSaleTableModel::Columns::Amount), material->count()*material->worksPrice());
    }
}

// Добавление работы и товара.
// Возвращает 0 в случае ошибки.
bool SCartridgeForm::addWorkAndPart()
{
    m_opType = AddWorkAndPart;
    return manualSubmit();
}

void SCartridgeForm::beginRemoveWorkAndPart()
{
    int row = 0;
    for(; row < m_BOQModel->rowCount(); row++)
    {
        if(m_BOQModel->index(row, SSaleTableModel::Columns::WorkType).data().toInt() == m_workType)
            break;
    }
    m_BOQModel->removeRow(row);
}

bool SCartridgeForm::removeWorkAndPart()
{
    m_opType = RemoveWorkAndPart;
    return manualSubmit();
}

bool SCartridgeForm::workAndPartHandler(const int workType, const int checkboxState)
{
    int ret;
    m_workType = workType;
    if(checkboxState)
    {
        ret = addWorkAndPart();
    }
    else
    {
        ret = removeWorkAndPart();
    }
    return ret;
}

/* Запрос кол-ва предыдущих заправок
 * Презназначен для включения индикатора о превышении ресурса и индикатора о повторной заправке, если об этом не было указано при приёмке
 * Cм. описание метода SCartridgeCardModel::resource()
*/
int SCartridgeForm::prevRepairsCount()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database(TdConn::main()));

    query->exec(QUERY_SEL_CARTRIDGE_RESOURCE(m_repairId, m_serialNumber, SWorkModel::Type::CartridgeRefill));
    if(query->first())
    {
        m_prevRepairsCount = query->value(0).toInt();
    }

    delete query;

    return m_prevRepairsCount;
}

int SCartridgeForm::checkBeforeInformedStatusChange()
{
    if(m_repair->informedStatusIndex())
        return 1;

    return 0;
}

int SCartridgeForm::checkBeforeRepairCreate()
{
    setDefaultStyleSheets();
    int error = 0;

    if (ui->lineEditSerial->text() == "" && comSettings->isCartridgeSerialNumberRequired())
    {
        ui->lineEditSerial->setStyleSheet(commonLineEditStyleSheetRed);
        error = 1;
    }
//    if(ui->doubleSpinBoxPreagreedAmount->value() == 0)
//    {
//        ui->doubleSpinBoxPreagreedAmount->setStyleSheet(commonSpinBoxStyleSheetRed);
//        error = 2;
//    }

    return error;
}

int SCartridgeForm::checkInput()
{
    setDefaultStyleSheets();

    switch(m_opType)
    {
        case CreateRepair: break; // проверка вызывается из метода tabReceptCartridge::checkInput()
        case SaveState: return checkBeforeStateChange();
        case AddWorkAndPart: return checkWorkMaterialItemCount();
        case SaveComment: if(ui->lineEditComment->text().compare(m_repair->extNotes()) == 0) return 1; break;
        case SaveEngineer:
        case RemoveWorkAndPart: break;
        case SavePlace: return checkBeforePlaceChange(); break;
        case SaveInformedState: return checkBeforeInformedStatusChange();
    }

    return false;
}

int SCartridgeForm::isReady()
{
    switch(m_repair->stateId())
    {
        case Global::RepStateIds::InWork: return (comSettings->useSimplifiedCartridgeRepair() && m_BOQModel->rowCount());
        case Global::RepStateIds::Ready:
        case Global::RepStateIds::ReadyNoRepair: return 1;
    }

    return 0;
}

void SCartridgeForm::updateStatesModel()
{
    int stateId = m_repair->stateId();
    QString allowedStates;

    switch(stateId)
    {
        case Global::RepStateIds::GetIn: allowedStates = QString::number(Global::RepStateIds::InWork); break;
        case Global::RepStateIds::InWork: allowedStates = QString("%1|%2").arg(Global::RepStateIds::Ready).arg(Global::RepStateIds::ReadyNoRepair); break;
        case Global::RepStateIds::Ready: allowedStates = "---"; break;
        case Global::RepStateIds::ReadyNoRepair: allowedStates = "---"; break;
        default: allowedStates = "---";
    }
    ui->comboBoxState->blockSignals(true);
    statusesProxyModel->setFilterRegularExpression(QString("\\b(%1)\\b").arg(allowedStates));
    ui->comboBoxState->setCurrentIndex(-1);
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    ui->comboBoxState->setPlaceholderText(comSettings->repairStatusesVariantCopy()[stateId].Name);
    ui->comboBoxState->blockSignals(false);

    emit updateParentTab();
}

void SCartridgeForm::updateTotalSumms(const double amountTotal, const double, const double)
{
    ui->doubleSpinBoxTotalAmount->setValue(amountTotal);
}

void SCartridgeForm::saveTotalSumms()
{
    m_repair->set_realRepairCost(m_BOQModel->amountTotal());
    m_repair->set_partsCost(m_BOQModel->amountItems());
    m_repair->commit();
}

void SCartridgeForm::setRefill(bool state)
{
    auto material = m_cartridgeCard->material(SCartridgeMaterialModel::Toner);
    if(material)
        updatePreagreedAmount(material, state);
}

void SCartridgeForm::setChipReplace(bool state)
{
    auto material = m_cartridgeCard->material(SCartridgeMaterialModel::Chip);
    if(material)
        updatePreagreedAmount(material, state);
}

void SCartridgeForm::setDrumReplace(bool state)
{
    auto material = m_cartridgeCard->material(SCartridgeMaterialModel::Drum);
    if(material)
        updatePreagreedAmount(material, state);
}

void SCartridgeForm::setBladeReplace(bool state)
{
    auto material = m_cartridgeCard->material(SCartridgeMaterialModel::Blade);
    if(material)
        updatePreagreedAmount(material, state);
}
bool SCartridgeForm::checkBeforePlaceChange()
{
    if(!m_repair)
        return 1;

    if(m_repair->boxIndex() == ui->comboBoxPlace->currentIndex())
        return 1;

    return 0;
}

void SCartridgeForm::savePlace(int)
{
    m_opType = SavePlace;
    manualSubmit();
}

void SCartridgeForm::updateComboBoxPlace()
{
    ui->comboBoxPlace->blockSignals(true);
    ui->comboBoxPlace->setCurrentIndex(m_repair->boxIndex());
    ui->comboBoxPlace->blockSignals(false);
}

void SCartridgeForm::comboBoxPlaceButtonClickHandler(int id)
{
    if(id == SLineEdit::Clear)
        ui->comboBoxPlace->setCurrentIndex(-1);
}

void SCartridgeForm::updateComboBoxInformedStatus()
{
//    ui->comboBoxNotifyStatus->blockSignals(true);
//    ui->comboBoxNotifyStatus->setCurrentIndex(m_repair->informedStatusIndex());
//    ui->comboBoxNotifyStatus->blockSignals(false);
}

void SCartridgeForm::comboBoxEngineerChanged(int)
{
    m_opType = SaveEngineer;
    manualSubmit();
}

void SCartridgeForm::stateIndexChanged(int index)
{
    if (index < 0)
        return;

    saveState();
}

void SCartridgeForm::saveState()
{
    m_opType = SaveState;
    manualSubmit();
}


void SCartridgeForm::removeWidget()
{
    if(m_repair)
        updateComment();

    preagreedAmounts.clear();
    m_amount = 0;
    emit updateTotalPreagreedAmount();
    this->deleteLater();
}

void SCartridgeForm::buttonClientCardClicked()
{
    emit createTabClient(m_repair->clientId());
}

void SCartridgeForm::buttonClassicTabClicked()
{
    emit createTabRepair(m_repair->id());
    this->deleteLater();    // TODO: когда будет настроена синхронизация модели данных таблицы работ и деталей, тогда можно будет не удалять форму из списка
}

void SCartridgeForm::buttonCartridgeCardClicked()
{
    emit createCartridgeCardForm(m_cardId);
}

void SCartridgeForm::updateComment()
{
    m_opType = SaveComment;
    manualSubmit();
}

void SCartridgeForm::initWorksMenu()
{
    QMenu *works_menu = new QMenu(this);
    QMetaEnum types = SWorkModel::staticMetaObject.enumerator(SWorkModel::staticMetaObject.indexOfEnumerator("Type"));
    SWorkModel::Type type;
    std::shared_ptr<SCartridgeMaterialModel> material;

    for(int i = SWorkModel::Type::CartridgeReplaceOfWorn; i < types.keyCount(); i++)
    {
        type = (SWorkModel::Type)types.value(i);
        material = m_cartridgeCard->material(type);
        if(material)
        {
            QAction *item = new QAction(material->workName(), this);
            item->setCheckable(true);
            item->setProperty("WorkType", type);
            works_menu->addAction(item);
//            item->installEventFilter(this);
            connect(item, &QAction::triggered, this, [=](){workAndPartHandler(item->property("WorkType").toInt(), item->isChecked());});
        }
    }
    if(works_menu->actions().count())
    {
        ui->toolButtonOtherWorksMenu->setMenu(works_menu);

        updateWorksActionsCheckedState();
    }
    else
        ui->toolButtonOtherWorksMenu->setDisabled(true);
}

void SCartridgeForm::updateWorksMenu()
{
    QMenu *menu = ui->toolButtonOtherWorksMenu->menu();

    // TODO: сделать обновление вместо замены на новый объект
    initWorksMenu();

    if(menu)
        delete menu;
}

void SCartridgeForm::setWorkCheckBoxChecked(const int workType)
{
    switch(workType)
    {
        case SWorkModel::Type::CartridgeRefill: ui->pushButtonRefill->setChecked(true); break;
        case SWorkModel::Type::CartridgeDrumReplace: ui->pushButtonDrumReplace->setChecked(true); break;
        case SWorkModel::Type::CartridgeChipReplace: ui->pushButtonChipReplace->setChecked(true); break;
        case SWorkModel::Type::CartridgeBladeReplace: ui->pushButtonBladeReplace->setChecked(true); break;
        default: ;
    }
}

void SCartridgeForm::updateWorksActionsCheckedState()
{
    if(!ui->toolButtonOtherWorksMenu->menu())
        return;

    QList<QAction*> actions = ui->toolButtonOtherWorksMenu->menu()->actions();
    if(actions.isEmpty())
        return;

    QList<QAction*>::const_iterator action = actions.constBegin();
    while(action != actions.constEnd())
    {
        (*action)->setChecked(false);
        for(int row = 0; row < m_BOQModel->rowCount(); row++)
        {
            if(m_BOQModel->recordType(row) != SSaleTableModel::RecordType::Work)
                continue;

            if((*action)->property("WorkType").toInt() == m_BOQModel->workType(row))
            {
                (*action)->setChecked(true);
                return;
            }
        }
        action++;
    }
}

void SCartridgeForm::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize());

    QFont font1(font);
    font1.setBold(true);
    font1.setWeight(75);

    QFont font2(font1);
    font2.setPixelSize(userDbData->fontSize()+1);

    ui->labelTitle->setFont(font);
    ui->labelLimitReached->setFont(font1);
    ui->labelLimitReached->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 175, 176);\n"
                                                           "color: rgb(197, 255, 172);"));
    ui->labelLimitReached->setMargin(5);
    ui->lineEditSerial->setFont(font2);

}

void SCartridgeForm::beginSaveInformedState()
{
    int stateId = 0;
//    stateId = notifyStatusesModel->databaseIDByRow(ui->comboBoxNotifyStatus->currentIndex());
    m_repair->setInformedStatusIndex(stateId);
}

void SCartridgeForm::beginCommit()
{
    switch(m_opType)
    {
        case SaveState: beginSaveState(statusesProxyModel->databaseIDByRow(ui->comboBoxState->currentIndex())); break;
        case AddWorkAndPart: beginAddWorkAndPart(); break;
        case RemoveWorkAndPart: beginRemoveWorkAndPart(); break;
        case CreateRepair:beginCreateRepair(); break;
        case SaveInformedState: beginSaveInformedState(); break;
        case SaveEngineer: m_repair->setEngineerIndex(ui->comboBoxEngineer->currentIndex()); break;
        case SaveComment: m_repair->set_extNotes(ui->lineEditComment->text()); break;
        case SavePlace: m_repair->setBoxIndex(ui->comboBoxPlace->currentIndex()); break;
    }

    m_repair->updateLastSave();
}

int SCartridgeForm::commitStages()
{
    switch(m_opType)
    {
        case AddWorkAndPart: return 2;  // Переключение статуса в случае включенного упрощенного режима, затем сохранение таблицы работ и деталей
        default: ;
    }
    return 1;
}

bool SCartridgeForm::skip(const int stage)
{
    switch(m_opType)
    {
        case AddWorkAndPart: return ((stage == 0) && (!comSettings->useSimplifiedCartridgeRepair() || m_repair->stateId() != Global::RepStateIds::GetIn));
        default: ;
    }

    return 0;
}

void SCartridgeForm::beginCommit(const int stage)
{
    if(stage == 0)
    {
        switch(m_opType)
        {
            case AddWorkAndPart: beginSaveState(Global::RepStateIds::InWork); break;
            default: ;
        }
    }
}

void SCartridgeForm::commit(const int stage)
{
    if(stage == 0)
    {
        switch(m_opType)
        {
            case RemoveWorkAndPart: break;
            default: m_repair->commit(); return;
        }
    }

    // stage == 1 или m_opType == RemoveWorkAndPart
    m_BOQModel->commit();
}

void SCartridgeForm::endCommit(const int)
{
}

void SCartridgeForm::endBOQCommit()
{
    updateLists();
    updateWorksActionsCheckedState();
}

void SCartridgeForm::endCommit()
{
    QString msg;

    switch (m_opType)
    {
        case SaveEngineer: msg = tr("Инженер назначен"); break;
        case SaveState: msg = tr("Статус изменён"); break;
        case AddWorkAndPart:
        case RemoveWorkAndPart: msg = tr("Список работ и деталей сохранён"); break;
        case SaveComment: msg = tr("Примечание сохранено"); break;
        case SavePlace: msg = tr("Место хранения сохранено"); break;
        case SaveInformedState: msg = tr("Статус информирования клиента сохранён"); break;
        case CreateRepair: return;
        default: ;
    }

    m_repair->SSingleRowModelBase::load();
    switch (m_opType)
    {
        case AddWorkAndPart: endBOQCommit(); break; // при удалении работ или деталей эмитируется сигнал modelReset
        default: ;
    }

    updateWidgets();
    new shortlivedNotification(this, tr("Успешно"), msg, QColor(214,239,220), QColor(229,245,234));
    emit updateParentTab();
    tabRepairs::refreshIfTabExists();
}

void SCartridgeForm::undoBOQModelChanges()
{
    // TODO: отмена изменений в модели работ и деталей
    // Реализация механизма отложена на будущее, т. к. потребует сущетсвенно доработки модели.
    // В данном случае модель работает по принципу ручного сохранения изменений в БД. Однако,
    // если сбой произойдёт по середине сессии, то часть данных уже будет записана (в одну из таблиц),
    // а соответствующие флаги полей изменят свой статус, что не позволит по этим флагам определить
    // какие из изменений нужно отменить.

}

void SCartridgeForm::throwHandler(int type)
{
    switch (m_opType)
    {
        case CreateRepair: return; // изменение состояния не требуется, т. к. при приёме модель временная
        case RemoveWorkAndPart:
        case AddWorkAndPart: undoBOQModelChanges(); break;
        default: break;
    }

    if(type == Global::ThrowType::ConnLost)
        return;

    m_repair->setFieldsFailed();
    switch (m_opType)
    {
        case SaveEngineer: updateComboBoxEngineer(); break;
        case AddWorkAndPart:
        case SaveState: updateStatesModel(); break;
        case SaveComment: ui->lineEditComment->setFocus(Qt::FocusReason::MouseFocusReason); break;
        case SavePlace: updateComboBoxPlace(); break;
        case SaveInformedState: updateComboBoxInformedStatus(); break;
        default: break;
    }
}
