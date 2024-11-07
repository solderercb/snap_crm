#include "sbillofquantities.h"
#include "ui_sbillofquantities.h"
#include "mainwindow.h"
#include "modules/purchasemanager/tabrequest.h"

SBillOfQuantities::SBillOfQuantities(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SBillOfQuantities)
{
    ui->setupUi(this);

    m_model = new SSaleTableModel(this);
    STableViewBOQItemDelegates *itemDelagates = new STableViewBOQItemDelegates(m_model, ui->tableViewBOQ);
    m_model->setMode(SSaleTableModel::WorkshopSale);
    m_model->setPriceColumn(SStoreItemModel::PriceOptionService); // по умолчанию цена для сервиса

    if(permissions->addGoodsFromWarehouse)  // Устанавливать детали со склада
    {
        ui->lineEditQuickAddPart->setButtons("Apply");
        connect(ui->lineEditQuickAddPart, &SLineEdit::buttonClicked, this, &SBillOfQuantities::onReturnQuickAddPart);
        connect(ui->lineEditQuickAddPart, &SLineEdit::returnPressed, this, &SBillOfQuantities::onReturnQuickAddPart);
    }

    connect(ui->pushButtonRequestSparePart, &QPushButton::clicked, this, &SBillOfQuantities::createTabNewPartRequest);
    connect(ui->switchEditStrategy, &QPushButton::toggled, this, &SBillOfQuantities::switchEditStrategy);
    connect(ui->toolButtonSaveSaleTable, &QToolButton::clicked, this, &SBillOfQuantities::saveSaleTableClicked);
    connect(ui->pushButtonAdmEditWorks, &QPushButton::clicked, this, &SBillOfQuantities::buttonWorksAdminEdit);
    connect(m_model, &SSaleTableModel::amountChanged, this, &SBillOfQuantities::updateAmounts);
    connect(m_model, &SSaleTableModel::tableSaved, this, &SBillOfQuantities::saveAmounts);
    connect(m_model, &SSaleTableModel::tableDataChanged, this, &SBillOfQuantities::setSaveSaleTableEnabled);
    connect(ui->pushButtonAddWork, &QPushButton::clicked, this, &SBillOfQuantities::addCustomWork);

    if(!userDbData->autosavePartList)    // установка состояния должна выполняться после соединения сигналов и слотов
        m_model->setEditStrategy(SSaleTableModel::OnManualSubmit);

    ui->tableViewBOQ->setModel(m_model);
    ui->tableViewBOQ->setItemDelegate(itemDelagates);
    ui->tableViewBOQ->setMinimumHeight(150);
    connect(itemDelagates, &STableViewBOQItemDelegates::addItem, this, &SBillOfQuantities::buttonAddItemClicked);
    connect(ui->tableViewBOQ, &STableViewBOQWorkshop::pressed, m_model, &SSaleTableModel::indexSelected);

#ifdef QT_DEBUG
    connect(ui->dbgBtnAddRandomPart, &QPushButton::clicked, m_model, &SSaleTableModel::dbgAddRandomItem);
    connect(ui->dbgBtnAddRandomPartBasket, &QPushButton::clicked, m_model, &SSaleTableModel::dbgAddRandomItemBasket);
#else
    ui->dbgBtnAddRandomPart->setHidden(true);
    ui->dbgBtnAddRandomPartBasket->setHidden(true);
#endif

    updateWidgets();
}

SBillOfQuantities::~SBillOfQuantities()
{
    m_repairModel = nullptr;
    delete ui;
}

void SBillOfQuantities::setRepairId(const int id)
{
    m_model->setRepairId(id);
}

/* Связь модели данных таблицы работ и деталей с моделью данных ремонта
 * При передаче nullptr связь не разрывается, но внутренний указатель
 * на модель данных ремонта обновляется для правильной работы других методов
*/
void SBillOfQuantities::linkWithRepairModel(SRepairModel *model)
{
    m_repairModel = model;
    if(model)
    {
        m_repairModel->setBOQModel(m_model);
        connect(m_repairModel, &SRepairModel::modelUpdated, this, &SBillOfQuantities::repairModelUpdated);
    }
}

void SBillOfQuantities::setReadOnly(bool state)
{
    m_modelRO = state;
    m_model->setState(m_modelRO?SSaleTableModel::WorkshopRO:SSaleTableModel::WorkshopRW);
}

bool SBillOfQuantities::isDirty()
{
    return m_model->isDirty();
}

void SBillOfQuantities::commit()
{
    if(!m_model->isDirty())
        return;

    m_model->repair_saveTablesStandalone();
}

void SBillOfQuantities::load(const int repair)
{
    if(!m_model->isDirty())
        m_model->repair_loadTable(repair);
}

bool SBillOfQuantities::isEmpty()
{
    return (m_model->rowCount() == 0);
}

void SBillOfQuantities::clearTable()
{
    if(!m_repairModel || !m_repairModel->id())
        m_model->removeRows(0, m_model->rowCount());
}

void SBillOfQuantities::updateWidgets()
{
    bool repExists = m_repairModel && m_repairModel->id();
    ui->pushButtonAddWork->setEnabled((!m_modelRO || m_modelAdmEdit) && permissions->addCustomWork);
//    ui->pushButtonAddWorkFromPriceList->setEnabled(!m_modelRO || m_modelAdmEdit);
    ui->pushButtonRequestSparePart->setEnabled((!m_modelRO || m_modelAdmEdit) && repExists);
    ui->toolButtonSaveSaleTable->setVisible(repExists);
    ui->toolButtonSaveSaleTable->setEnabled((!m_modelRO || m_modelAdmEdit) && m_model->isDirty());
    ui->switchEditStrategy->setVisible(repExists);
    ui->switchEditStrategy->setEnabled(!m_modelRO || m_modelAdmEdit);
    ui->switchEditStrategy->setChecked(m_model->editStrategy() == SSaleTableModel::OnFieldChange || m_model->editStrategy() == SSaleTableModel::OnRowChange);
    ui->pushButtonAdmEditWorks->setVisible(permissions->advEditWorkList && repExists && m_modelRO);
    ui->pushButtonAdmEditWorks->setChecked(m_modelAdmEdit);
}

void SBillOfQuantities::addCustomWork()
{
    int row = m_model->rowCount();
    m_model->addCustomWork();

    ui->tableViewBOQ->scrollToBottom();
    ui->tableViewBOQ->setCurrentIndex(m_model->index(row, SStoreItemModel::SaleOpColumns::ColName));
    ui->tableViewBOQ->setFocus();
}

void SBillOfQuantities::onReturnQuickAddPart()
{
    QString text = ui->lineEditQuickAddPart->text();

    if(text.isEmpty())
        return;

    if(quickAddPart(text.toInt()))
        ui->lineEditQuickAddPart->setText("");
}

/* Быстрое добавление товара по UID
 * Возвращает 0 в случае неудачи
*/
bool SBillOfQuantities::quickAddPart(const int uid)
{
    if(m_modelRO && !m_modelAdmEdit)
        return 0;

    // TODO: диалог ввода кол-ва
    return m_model->addItemByUID(uid);
}

void SBillOfQuantities::updateAmounts(const double, const double, const double)
{
    ui->lineEditTotalAmount->setText(m_model->amountTotalLocale());
    ui->lineEditWorksAmount->setText(m_model->amountWorksLocale());
    ui->lineEditSparePartsAmount->setText(m_model->amountItemsLocale());
    emit amountUpdated(m_model->amountTotal());
}

void SBillOfQuantities::saveAmounts()
{
    if(!m_repairModel)
        return;

    m_repairModel->setRealRepairCost(m_model->amountTotal());
    m_repairModel->setPartsCost(m_model->amountItems());

    if(!m_repairModel->id())    // в быстром ремонте запись сумм происходит в общем запросе
        return;

    // по идее в очереди на обновление значений в БД кроме сумм ничего не должно быть, поэтому сигналы блокируются
    // чтобы избежать ненужной эмиссии modelReset() и как следствие вызова метода updateWidgets()
    m_repairModel->blockSignals(true);
    m_repairModel->commit();
    m_repairModel->blockSignals(false);
}

void SBillOfQuantities::switchEditStrategy(bool state)
{
    if(state)
    {
        if(m_model->isDirty())
            m_model->repair_saveTablesStandalone();
        m_model->setEditStrategy(SSaleTableModel::OnFieldChange);
    }
    else
    {
        m_model->setEditStrategy(SSaleTableModel::OnManualSubmit);
    }
    ui->toolButtonSaveSaleTable->setEnabled(m_model->isDirty());

    updateWidgets();
}

void SBillOfQuantities::saveSaleTableClicked()
{
    if(m_model->repair_saveTablesStandalone())
        ui->toolButtonSaveSaleTable->setEnabled(false);
}

void SBillOfQuantities::setSaveSaleTableEnabled()
{
    if(m_model->editStrategy() == SSaleTableModel::OnManualSubmit)
        ui->toolButtonSaveSaleTable->setEnabled(true);
}

void SBillOfQuantities::buttonWorksAdminEdit(bool state)
{
    if(state)
    {
        m_modelAdmEdit = 1;
        m_model->setState(SSaleTableModel::State::WorkshopAdm);
    }
    else
    {
        m_modelAdmEdit = 0;
        setReadOnly();
        if(m_model->isDirty())
            saveSaleTableClicked();
    }
    updateWidgets();
}

void SBillOfQuantities::buttonAddItemClicked()
{
    qDebug().nospace() << "[" << this << "] buttonAddItemClicked() | Not yet implemented";
    //        emit createTabSelectItem(1, this);
}

void SBillOfQuantities::repairModelUpdated()
{
    m_model->setRepairId(m_repairModel->id());
    if(m_repairModel->clientId() != m_clientId)  // перезагрузка данных клиента только при первом вызове метода или если был изменён клиент
    {
        m_clientId = m_repairModel->clientId();
        m_model->setClient(m_clientId);
    }
    m_model->setIsWarranty(m_repairModel->isWarranty());
}

void SBillOfQuantities::createTabNewPartRequest()
{
    tabPartRequest *tabRequest = static_cast<tabPartRequest*>(MainWindow::getInstance()->createTabPartRequest(0));
    tabRequest->setRepair(m_repairModel->id());
}

#ifdef QT_DEBUG
void SBillOfQuantities::dbgAddWork()
{
    addCustomWork();
    m_model->setData(ui->tableViewBOQ->currentIndex().siblingAtColumn(SStoreItemModel::ColName), "repair");
    m_model->setData(ui->tableViewBOQ->currentIndex().siblingAtColumn(SStoreItemModel::ColPrice), 123);
}
#endif
