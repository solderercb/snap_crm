#include "mainwindow.h"
#include "sworksandparts.h"
#include "ui_sworksandparts.h"

SWorksAndParts::SWorksAndParts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SWorksAndParts)
{
    ui->setupUi(this);

    m_model = new SSaleTableModel(this);
    m_model->setTableMode(SSaleTableModel::WorkshopSale);
    m_model->setPriceColumn(SStoreItemModel::PriceOptionService); // по умолчанию цена для сервиса

    if(permissions->addGoodsFromWarehouse)  // Устанавливать детали со склада
    {
        ui->lineEditQuickAddPart->setButtons("Apply");
        connect(ui->lineEditQuickAddPart, &SLineEdit::buttonClicked, this, &SWorksAndParts::onReturnQuickAddPart);
        connect(ui->lineEditQuickAddPart, &SLineEdit::returnPressed, this, &SWorksAndParts::onReturnQuickAddPart);
    }

    connect(ui->switchEditStrategy, &QPushButton::toggled, this, &SWorksAndParts::switchEditStrategy);
    connect(ui->toolButtonSaveSaleTable, &QToolButton::clicked, this, &SWorksAndParts::saveSaleTableClicked);
    connect(ui->pushButtonAdmEditWorks, &QPushButton::clicked, this, &SWorksAndParts::buttonWorksAdminEdit);
    connect(m_model, &SSaleTableModel::amountChanged, this, &SWorksAndParts::updateAmounts);
    connect(m_model, &SSaleTableModel::tableSaved, this, &SWorksAndParts::saveAmounts);
    connect(m_model, &SSaleTableModel::addItem, this, &SWorksAndParts::buttonAddItemClicked);
    connect(m_model, &SSaleTableModel::tableDataChanged, this, &SWorksAndParts::setSaveSaleTableEnabled);
    connect(ui->pushButtonAddWork, &QPushButton::clicked, this, &SWorksAndParts::addCustomWork);

    if(!userDbData->autosavePartList)    // установка состояния должна выполняться после соединения сигналов и слотов
        m_model->setEditStrategy(SSaleTableModel::OnManualSubmit);

    ui->tableViewWorksAndSpareParts->setModel(m_model);
    connect(ui->tableViewWorksAndSpareParts, &worksAndSparePartsTable::pressed, m_model, &SSaleTableModel::indexSelected);

#ifdef QT_DEBUG
    connect(ui->dbgBtnAddRandomPart, &QPushButton::clicked, m_model, &SSaleTableModel::dbgAddRandomItem);
    connect(ui->dbgBtnAddRandomPartBasket, &QPushButton::clicked, m_model, &SSaleTableModel::dbgAddRandomItemBasket);
#else
    ui->dbgBtnAddRandomPart->setHidden(true);
    ui->dbgBtnAddRandomPartBasket->setHidden(true);
#endif

    updateWidgets();
}

SWorksAndParts::~SWorksAndParts()
{
    m_repairModel = nullptr;
    delete ui;
}

void SWorksAndParts::setRepairId(const int id)
{
    m_model->setRepairId(id);
}

/* Связь модели данных таблицы работ и деталей с моделью данных ремонта
 * При передаче nullptr связь не разрывается, но внутренний указатель
 * на модель данных ремонта обновляется для правильной работы других методов
*/
void SWorksAndParts::linkWithRepairModel(SRepairModel *model)
{
    m_repairModel = model;
    if(model)
    {
        m_repairModel->setBOQModel(m_model);
        connect(m_repairModel, &SRepairModel::modelUpdated, this, &SWorksAndParts::repairModelUpdated);
    }
}

void SWorksAndParts::setReadOnly(bool state)
{
    m_modelRO = state;
}

bool SWorksAndParts::isCommitted()
{
    return !ui->switchEditStrategy->isChecked() && m_model->isUnsaved();
}

void SWorksAndParts::commit()
{
    if(!m_model->isUnsaved())
        return;

    m_model->repair_saveTablesStandalone();
}

void SWorksAndParts::load(const int repair)
{
    m_model->repair_loadTable(repair);
}

bool SWorksAndParts::isEmpty()
{
    return (m_model->rowCount() == 0);
}

void SWorksAndParts::clearTable()
{
    if(!m_repairModel || !m_repairModel->id())
        m_model->removeRows(0, m_model->rowCount());
}

void SWorksAndParts::updateWidgets()
{
    ui->pushButtonAddWork->setEnabled((!m_modelRO || m_modelAdmEdit) && permissions->addCustomWork);
//    ui->pushButtonAddWorkFromPriceList->setEnabled(!m_modelRO || m_modelAdmEdit);
//    ui->pushButtonRequestSparePart->setEnabled(!m_modelRO || m_modelAdmEdit);
    ui->toolButtonSaveSaleTable->setVisible(m_repairModel && m_repairModel->id());
    ui->toolButtonSaveSaleTable->setEnabled((!m_modelRO || m_modelAdmEdit) && m_model->isUnsaved());
    ui->switchEditStrategy->setVisible(m_repairModel && m_repairModel->id());
    ui->switchEditStrategy->setEnabled(!m_modelRO || m_modelAdmEdit);
    ui->switchEditStrategy->setChecked(m_model->editStrategy() == SSaleTableModel::OnFieldChange || m_model->editStrategy() == SSaleTableModel::OnRowChange);
    ui->pushButtonAdmEditWorks->setVisible(permissions->advEditWorkList && m_repairModel && m_repairModel->id() && m_modelRO);
    ui->pushButtonAdmEditWorks->setChecked(false);
    m_model->setModelState((m_modelRO && !m_modelAdmEdit)?SSaleTableModel::WorkshopRO:SSaleTableModel::WorkshopRW);
}

void SWorksAndParts::addCustomWork()
{
    int row = m_model->rowCount();
    m_model->addCustomWork();

    ui->tableViewWorksAndSpareParts->scrollToBottom();
    ui->tableViewWorksAndSpareParts->setCurrentIndex(m_model->index(row, SStoreItemModel::SaleOpColumns::ColName));
    ui->tableViewWorksAndSpareParts->setFocus();
}

void SWorksAndParts::onReturnQuickAddPart()
{
    if(quickAddPart(ui->lineEditQuickAddPart->text().toInt()))
        ui->lineEditQuickAddPart->setText("");
}

/* Быстрое добавление товара по UID
 * Возвращает 0 в случае неудачи
*/
bool SWorksAndParts::quickAddPart(const int uid)
{
    if(m_modelRO && !m_modelAdmEdit)
        return 0;

    // TODO: диалог ввода кол-ва
    return m_model->addItemByUID(uid);
}

void SWorksAndParts::updateAmounts(const double, const double, const double)
{
    ui->lineEditTotalAmount->setText(m_model->amountTotalLocale());
    ui->lineEditWorksAmount->setText(m_model->amountWorksLocale());
    ui->lineEditSparePartsAmount->setText(m_model->amountItemsLocale());
    emit amountUpdated(m_model->amountTotal());
}

void SWorksAndParts::saveAmounts()
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

void SWorksAndParts::switchEditStrategy(bool state)
{
    if(state)
    {
        if(m_model->isUnsaved())
            m_model->repair_saveTablesStandalone();
        m_model->setEditStrategy(SSaleTableModel::OnFieldChange);
    }
    else
    {
        m_model->setEditStrategy(SSaleTableModel::OnManualSubmit);
    }
    ui->toolButtonSaveSaleTable->setEnabled(m_model->isUnsaved());

    updateWidgets();
}

void SWorksAndParts::saveSaleTableClicked()
{
    if(m_model->repair_saveTablesStandalone())
        ui->toolButtonSaveSaleTable->setEnabled(false);
}

void SWorksAndParts::setSaveSaleTableEnabled()
{
    if(m_model->editStrategy() == SSaleTableModel::OnManualSubmit)
        ui->toolButtonSaveSaleTable->setEnabled(true);
}

void SWorksAndParts::buttonWorksAdminEdit(bool state)
{
    if(state)
    {
        m_modelAdmEdit = 0;
        m_model->setModelState(SSaleTableModel::State::WorkshopAdm);
        ui->switchEditStrategy->setEnabled(true);
        ui->toolButtonSaveSaleTable->setEnabled(m_model->isUnsaved());
        ui->pushButtonAddWork->setEnabled(true);
    }
    else
    {
        m_modelAdmEdit = 1;
        m_model->setModelState(m_modelAdmEdit?SSaleTableModel::WorkshopRO:SSaleTableModel::WorkshopRW);
        if(m_model->isUnsaved())
            saveSaleTableClicked();
        ui->switchEditStrategy->setEnabled(!m_modelAdmEdit);
        ui->toolButtonSaveSaleTable->setEnabled(!m_modelAdmEdit);
        ui->pushButtonAddWork->setEnabled(!m_modelAdmEdit && permissions->addCustomWork);
    }
}

void SWorksAndParts::buttonAddItemClicked()
{
    //        emit createTabSelectItem(1, this);
}

void SWorksAndParts::repairModelUpdated()
{
    m_model->setRepairId(m_repairModel->id());
    if(m_repairModel->clientId() != m_clientId)  // перезагрузка данных клиента только при первом вызове метода или если был изменён клиент
    {
        m_clientId = m_repairModel->clientId();
        m_model->setClient(m_clientId);
    }
    m_model->setIsWarranty(m_repairModel->isWarranty());
}

#ifdef QT_DEBUG
void SWorksAndParts::dbgAddWork()
{
    addCustomWork();
    m_model->setData(ui->tableViewWorksAndSpareParts->currentIndex().siblingAtColumn(SStoreItemModel::ColName), "repair");
    m_model->setData(ui->tableViewWorksAndSpareParts->currentIndex().siblingAtColumn(SStoreItemModel::ColPrice), 123);
}
#endif

// ===============================================================================================================
worksAndSparePartsTable::worksAndSparePartsTable(QWidget *parent) :
    STableViewBase(SLocalSettings::RepairWorksGrid, parent)
{
    // столбец "Доступно" (4) скрыт
    i_defaultColumnsWidths = {{0, 60},{1, 90},{2, 270},{3, 45},{4, 0},{5, 70},{6, 70},{7, 120},{8, 120},{9, 80},{10, 100}};
    i_defaultHeaderLabels << tr("") << tr("UID") << tr("Наименование") << tr("Кол-во") << tr("Доступно") << tr("Цена") << tr("Сумма") << tr("Место") << tr("Серийный номер") << tr("Гарантия") << tr("Сотрудник");
    readLayout();
    i_gridLayout->$GridControl.Columns[2].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

worksAndSparePartsTable::~worksAndSparePartsTable()
{
}

void worksAndSparePartsTable::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SSaleTableModel*>(model);
    STableViewBase::setModel(model);
    SaleTableItemDelegates *itemDelagates = new SaleTableItemDelegates(m_model, this);
    setItemDelegate(itemDelagates);
}

void worksAndSparePartsTable::mouseDoubleClickEvent(QMouseEvent *event)
{
    int row = currentIndex().row();
    if(m_model->index(row, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool())
    {
        if(event->modifiers() == Qt::ControlModifier)
        {
            clearSelection();
            selectionModel()->select(currentIndex(), QItemSelectionModel::Select);
            MainWindow::getInstance()->createTabSparePart(m_model->index(row, SStoreItemModel::SaleOpColumns::ColItemId).data().toInt());
        }
        else
            MainWindow::getInstance()->createTabSparePartReserve(m_model->index(row, SStoreItemModel::SaleOpColumns::ColId).data().toInt());
    }
}

#if QT_VERSION >= 0x060000
void worksAndSparePartsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void worksAndSparePartsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    STableViewBase::dataChanged(topLeft, bottomRight, roles);
    if(!roles.isEmpty() && !roles.contains(Qt::DisplayRole))
        return;

    for(int row = topLeft.row(); row <= bottomRight.row(); row++)
        for(int col = topLeft.column(); col <= bottomRight.column(); col++)
        {
            switch(col)
            {
                case SStoreItemModel::SaleOpColumns::ColName: resizeRowToContents(row); break;
                default: /*resizeColumnToContents(col)*/;
            }
        }
}
