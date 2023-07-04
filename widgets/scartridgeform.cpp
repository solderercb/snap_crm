#include "scartridgeform.h"
#include "ui_scartridgeform.h"

SCartridgeForm::SCartridgeForm(const int repairId, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SCartridgeForm)
{
    ui->setupUi(this);

//    ui->frameHeader->setStyleSheet("QFrame::hover {background-color: rgb(205,230,247);}");    // подсветка заголовка
    installEventFilter(this);
    m_repairId = repairId;
}

void SCartridgeForm::initModels()
{
    m_cartridgeCard = new SCartridgeCardModel();
    if(m_repairId)
    {
        m_repairModel = new SRepairModel();
        m_repairModel->load(m_repairId);
        m_cardId = m_repairModel->cartridge()->cardId();
        statusesProxyModel = new SSortFilterProxyModel;
        statusesProxyModel->setSourceModel(statusesModel);
        statusesProxyModel->setFilterKeyColumn(Global::RepStateHeaders::Id);
        statusesProxyModel->setFilterRegularExpression("");

        worksAndPartsModel = new SSaleTableModel();
        worksAndPartsModel->setTableMode(SSaleTableModel::WorkshopSale);
        worksAndPartsModel->setPriceColumn(0);
        connect(worksAndPartsModel, &SSaleTableModel::amountChanged, this, &SCartridgeForm::updateTotalSumms);
        connect(worksAndPartsModel, &SSaleTableModel::tableSaved, this, &SCartridgeForm::saveTotalSumms);
        worksAndPartsModel->repair_loadTable(m_repairModel->id());
    }

    m_cartridgeCard->load(m_cardId);
}

void SCartridgeForm::initWidgets()
{
    initModels();
    ui->comboBoxWasEarly->setModel(cartridgeRepeatReason);
    ui->comboBoxWasEarly->setCurrentIndex(-1);
    ui->comboBoxPlace->setModel(repairBoxesModel);
    ui->comboBoxPlace->setCurrentIndex(-1);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &SCartridgeForm::removeWidget);

    if(m_repairId)
    {
        ui->labelTitle->setText(m_repairModel->title());
        ui->splitterComment->hide();
        ui->splitterWasEarly->hide();
        ui->lineEditSerial->setText(m_repairModel->serialNumber());
        ui->lineEditSerial->setReadOnly(true);
        ui->checkBoxPreagreedRefill->setChecked(m_repairModel->cartridge()->refill());
        ui->checkBoxPreagreedRefill->setCheckable(false);   // сработает?
        ui->checkBoxPreagreedChipReplace->setChecked(m_repairModel->cartridge()->chip());
        ui->checkBoxPreagreedChipReplace->setCheckable(false);
        ui->checkBoxPreagreedDrumReplace->setChecked(m_repairModel->cartridge()->drum());
        ui->checkBoxPreagreedDrumReplace->setCheckable(false);
        ui->checkBoxPreagreedBladeReplace->setChecked(m_repairModel->cartridge()->Blade());
        ui->checkBoxPreagreedBladeReplace->setCheckable(false);
        ui->spinBoxRefillWeight->setValue(m_cartridgeCard->tonerWeight());
        ui->doubleSpinBoxPreagreedAmount->setValue(m_repairModel->preAgreedAmount());
        ui->comboBoxEngineer->setCurrentIndex(m_repairModel->engineerIndex());
        ui->comboBoxWasEarly->setCurrentIndex(m_repairModel->isRepeat()?1:-1);
        ui->comboBoxWasEarly->setCurrentIndex(m_repairModel->isWarranty()?0:-1);
        ui->comboBoxWasEarly->setEnabled(false);

        ui->comboBoxState->setModel(statusesProxyModel);
        ui->comboBoxState->setCurrentIndex(m_repairModel->stateIndex());
        connect(ui->comboBoxState, SIGNAL(currentIndexChanged()), this, SLOT(saveState()));

        ui->doubleSpinBoxTotalAmount->setValue(m_repairModel->realRepairCost());

        ui->comboBoxPlace->setCurrentIndex(m_repairModel->boxIndex());
        ui->comboBoxPlace->setButtons("Apply");
        connect(ui->comboBoxPlace, &SComboBox::buttonClicked, this, &SCartridgeForm::comboBoxPlaceButtonClickHandler);

        connect(ui->checkBoxRefill, &QCheckBox::stateChanged, this, &SCartridgeForm::addRefillWorksAndParts);
        connect(ui->checkBoxChipReplace, &QCheckBox::stateChanged, this, &SCartridgeForm::addChipWorksAndParts);
        connect(ui->checkBoxDrumReplace, &QCheckBox::stateChanged, this, &SCartridgeForm::addDrumWorksAndParts);
        connect(ui->checkBoxBladeReplace, &QCheckBox::stateChanged, this, &SCartridgeForm::addBladeWorksAndParts);
    }
    else
    {
        ui->pushButtonClientCard->hide();
        ui->toolButtonClassicRepairCard->hide();
        ui->toolButtonHelp->hide();
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

SCartridgeForm::~SCartridgeForm()
{
    delete ui;
    if(m_repairModel)
        delete m_repairModel;
    if(worksAndPartsModel)
        delete worksAndPartsModel;
    if(statusesProxyModel)
        delete statusesProxyModel;
    delete m_cartridgeCard;
}

bool SCartridgeForm::eventFilter(QObject *watched, QEvent *event)
{
//    if(event->type() != QEvent::Paint)
//        qDebug().nospace() << "[" << this << "] eventFilter() | watched: " << m_repairModel->objectName() << "; event->type(): " << event->type();

    // TODO: выделение виджета (фокус):
    // QEvent::MouseButtonPress
    // любое взаимодействие с дочерними виджетами

    return false;
}

SRepairModel *SCartridgeForm::model()
{
    return m_repairModel;
}

bool SCartridgeForm::createRepair()
{
    bool nErr = 1;
    SRepairModel *repair = new SRepairModel(this);
    repair->initCartridgeRepairModel(m_cardId);

    QStringList faultList;
    if(ui->checkBoxPreagreedRefill->isChecked())
    {
        faultList.append(tr("Заправка"));
        repair->cartridge()->setRefill(1);
    }
    if(ui->checkBoxPreagreedChipReplace->isChecked())
    {
        faultList.append(tr("Чип"));
        repair->cartridge()->setChip(1);
    }
    if(ui->checkBoxPreagreedDrumReplace->isChecked())
    {
        faultList.append(tr("Фотовал"));
        repair->cartridge()->setDrum(1);
    }
    if(ui->checkBoxPreagreedBladeReplace->isChecked())
    {
        faultList.append(tr("Ракель"));
        repair->cartridge()->setBlade(1);
    }
    repair->setTitle(m_title);
    repair->setClassId(m_deviceClassId);
    repair->setVendorId(m_deviceVendorId);
    repair->setClientId(m_clientId);
    repair->setFault(faultList.join(", "));
    repair->setSerialNumber(ui->lineEditSerial->text());
    repair->setExtNotes(ui->lineEditComment->text());
    switch(ui->comboBoxWasEarly->currentIndex())
    {
        case -1: repair->setIsWarranty(0); repair->setIsRepeat(0); break;
        case 0: repair->setIsWarranty(1); repair->setIsRepeat(0); break;
        case 1: repair->setIsRepeat(1); repair->setIsWarranty(0); break;
    }
    repair->setPreAgreedAmount(ui->doubleSpinBoxPreagreedAmount->value());
    repair->setRejectReason("");
    repair->setCompanyIndex(m_companyIndex);
    repair->setOfficeIndex(m_officeIndex);
    repair->setStartOfficeIndex(m_officeIndex);
    repair->setManager(userDbData->id);
    repair->setCurrentManager(userDbData->id);
    repair->setEngineerIndex(m_engineerIndex);
    if(m_isHighPriority)
        repair->setExpressRepair(1);
    if( m_paymentSystemIndex >= 0)
    {
        repair->setPaymentSystemIndex(m_paymentSystemIndex);
        if(paymentSystemsModel->databaseIDByRow(m_paymentSystemIndex, "system_id") == -1)
            repair->setIsCardPayment(1);
    }

    nErr = repair->commit();

    if(ui->comboBoxPlace->currentIndex() >= 0)
    {
        repair->setBoxIndex(ui->comboBoxPlace->currentIndex());
        nErr = repair->commit();
    }

    delete repair;
    return nErr;
}

bool SCartridgeForm::updateRepair()
{

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

bool SCartridgeForm::checkData(const int)
{

}

void SCartridgeForm::doStateActions(const int)
{

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

void SCartridgeForm::updateTotalSumms(const double, const double, const double)
{
    ui->doubleSpinBoxTotalAmount->setValue(worksAndPartsModel->amountTotal());
}

void SCartridgeForm::saveTotalSumms()
{
    m_repairModel->setRealRepairCost(worksAndPartsModel->amountTotal());
//    m_repairModel->setPartsCost(worksAndPartsModel->amountItems());
    m_repairModel->commit();
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

void SCartridgeForm::addRefillWorksAndParts(int state)
{

}

void SCartridgeForm::addChipWorksAndParts(int state)
{

}

void SCartridgeForm::addDrumWorksAndParts(int state)
{

}

void SCartridgeForm::addBladeWorksAndParts(int state)
{

}

void SCartridgeForm::comboBoxPlaceButtonClickHandler(int)
{
    qDebug().nospace() << "[" << this << "] comboBoxPlaceButtonClickHandler()";
}

void SCartridgeForm::saveState(int index)
{
    if (index < 0)
        return;

    int newStateId = statusesProxyModel->databaseIDByRow(index);

    try
    {
//        if(!checkStateAcl(newStateId))
//        {
//            shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Проверьте права доступа или обратитесь к администратору"), QColor(212,237,242), QColor(229,244,247));
//            throw 2;
//        }
        checkData(newStateId);
        doStateActions(newStateId);
//        repairModel->setState(newStateId);
    }
    catch (int type)
    {
//        ui->comboBoxState->blockSignals(true);
//        ui->comboBoxState->setCurrentIndex(-1);
//        ui->comboBoxState->blockSignals(false);
//        return;
    }
//    updateStatesModel(newStateId);
//    setWidgetsParams(newStateId);

//    if(!commit())
//        return;

    //    updateWidgets();
}

void SCartridgeForm::removeWidget()
{
    this->deleteLater();
}
