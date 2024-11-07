#include "ssupplierswidget.h"
#include "ui_ssupplierswidget.h"
#include "widgets/shortlivednotification.h"
#include "modules/purchasemanager/tabmanager.h"

SPartSuppliers::SPartSuppliers(QWidget *parent) :
    SWidget(parent),
    ui(new Ui::SPartSuppliers)
{
    ui->setupUi(this);

    initModels();
    initWidgets();
}

SPartSuppliers::~SPartSuppliers()
{
    delete ui;
}

void SPartSuppliers::initModels()
{
    m_model = new SPartSuppliersModel(this, QSqlDatabase::database("connThird"));
    m_model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    connect(m_model, &SPartSuppliersModel::modelReset, this, &SPartSuppliers::modelReset);
}

void SPartSuppliers::initWidgets()
{
    int tableSortSection;

    initTableMenu();

    ui->tableViewLinks->setModel(m_model);
    ui->tableViewLinks->setContextMenuPolicy(Qt::CustomContextMenu);

    PSItemDelegate *itemDelegates = new PSItemDelegate(this);
    ui->tableViewLinks->setItemDelegate(itemDelegates);

    tableSortSection = ui->tableViewLinks->horizontalHeader()->sortIndicatorSection();
    if(tableSortSection >= 0)
        m_model->setSort(tableSortSection, ui->tableViewLinks->horizontalHeader()->sortIndicatorOrder());

    connect(ui->tableViewLinks->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &SPartSuppliers::orderChanged);
    connect(ui->tableViewLinks, &QTableView::customContextMenuRequested, this, &SPartSuppliers::menuRequest);
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &SPartSuppliers::addRow);
    connect(ui->pushButtonSave, &QPushButton::clicked, this, &SPartSuppliers::saveLinks);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &SPartSuppliers::removeSelected);
}

void SPartSuppliers::updateWidgets()
{
    ui->pushButtonSave->setVisible(m_saveButtonVisible);
}

void SPartSuppliers::select(const int &id)
{
    if(id == 0)
    {
        return;
    }

    m_requestId = id;
    if(!m_model->select(id))
        qDebug().nospace() << "[" << this << "] select() | error: " << m_model->lastError().driverText();
    ui->tableViewLinks->verticalScrollBar()->setValue(0);
}

/* Очистка модели
 * Вызывается при изменении списка запросов (текущий индекс становится не валидным), например, при изменении фильтра.
 * При вызове метода clear() очищаются заголовки и соответственно портится файл соответствующей gridLayout, поэтому
 * выполняется запрос, который заведомо не вернёт результата.
*/
void SPartSuppliers::clearModel()
{
    m_model->select(0);
}

/* Копирование ссылок из другого запроса
*/
void SPartSuppliers::copyLinks(const int id)
{
    QSqlQueryModel *old = new QSqlQueryModel();
    old->setQuery(QUERY_SEL_PRT_RQST_SUPPLIERS(id), QSqlDatabase::database("connMain"));
    if(old->rowCount())
    {
        auto result = QMessageBox::question(this, tr("Копирование заявки"), tr("Скопировать ссылки?"), QMessageBox::Yes, QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            for(int i = 0; i < old->rowCount(); i++)
            {
                int srcLinkSuppl = old->index(i, SPartSuppliersModel::Columns::Supplier).data().toInt();
                int modelSuppl = m_model->predefSupplierId();
                if(srcLinkSuppl)
                {
                    m_model->setPredefSupplierId(srcLinkSuppl);
                }
                m_model->appendRow();

                for(int j = 0; j < old->columnCount(); j++)
                {
                    if(j == SPartSuppliersModel::Columns::Id)
                        continue;

                    QModelIndex newIndex = m_model->index(m_model->rowCount() - 1, j);
                    if(j == SPartSuppliersModel::Columns::Supplier)
                        continue;   // поставщик был заполнен при добавлении строки в методе SPartSuppliersModel::slotPrimeInsert

                    if(j == SPartSuppliersModel::Columns::Select)
                    {
                        m_model->setData(newIndex, 0);
                        continue;
                    }

                    m_model->setData(newIndex, old->index(i, j).data());
                }
//                m_model->setRecord(m_model->rowCount()-1, old->record(i));    // данные не копируются таким методом; не разобрался почему

                if(srcLinkSuppl)
                {
                    m_model->setPredefSupplierId(modelSuppl);
                }
            }
        }
    }

    delete old;
}

bool SPartSuppliers::isDirty()
{
    return m_model->isDirty();
}

void SPartSuppliers::setSaveButtonVisible(bool visible)
{
    m_saveButtonVisible = visible;
    updateWidgets();
}

void SPartSuppliers::setRequestId(int requestId)
{
    m_requestId = requestId;
    m_model->setRequestId(requestId);
}

/* установка модели данных с группами
 * Модель использутеся для отображения в таблице ссылок общего кол-ва товаров у этого поставщика
 * (значение определяется по URL магазина или по клиенту)
*/
void SPartSuppliers::setSubgroupsModel(SPartsRequestsGroupingModel *model)
{
    static_cast<PSItemDelegate*>(ui->tableViewLinks->itemDelegate())->setSubgroupsModel(model);
}

void SPartSuppliers::setGroupingMode(const int mode)
{
    static_cast<PSItemDelegate*>(ui->tableViewLinks->itemDelegate())->setGroupingMode(mode);
}

void SPartSuppliers::setRowHighlightingClause(const int id, const QString &name)
{
    m_model->setRowHighlightingClause(id, name);
}

void SPartSuppliers::setPredefSupplierId(const int id)
{
    m_model->setPredefSupplierId(id);
}

/* Обновление списка ссылок
*/
void SPartSuppliers::refresh(bool preserveScrollPos, bool preserveSelection)
{
    ui->tableViewLinks->setQuery(m_model->query().lastQuery(), m_model->database());
    ui->tableViewLinks->refresh(preserveScrollPos, preserveSelection);
}

/* Настройка связи сигнал-слот для быстрой фильтрации по URL поставщика
 * Связь должна работать только на вкладке Менеджера.
*/
void SPartSuppliers::connectSuppliersTableWithManager()
{
    tabPurchaseManager *tab = static_cast<tabPurchaseManager*>(findParentTab());

    connect(ui->tableViewLinks, &QTableView::clicked, this, [=](const QModelIndex &index){
        if(index.column() == STableViewPartRequestSuppliers::Column::SupplierUrl && QGuiApplication::queryKeyboardModifiers() & Qt::AltModifier)
        {
            QString searchStr = ui->tableViewLinks->model()->data(index).toString();
            if(userDbData->useRegExpSearch)
            { // TODO: код скопирован из класса tabRepairs::tableItemClick; нужно унифицировать
                searchStr = searchStr.replace("(", "\\(").replace(")", "\\)");
                searchStr = searchStr.replace("[", "\\[").replace("]", "\\]");
                searchStr = searchStr.replace("^", "\\^").replace("$", "\\$");
                searchStr = searchStr.replace("?", "\\?").replace("\\", "\\\\");
            }
            tab->quickFilterBySupplierUrl(searchStr.replace("\\\\","\\"));
        }
    });
}

void SPartSuppliers::saveLinks()
{
    if(!m_model->submitAll())
    {
        qDebug() << "[" << this << "] saveLinks() | : " << m_model->lastError();
        throw Global::ThrowType::QueryError;
    }

    emit submitOk();
}

void SPartSuppliers::orderChanged(const int section, Qt::SortOrder order)
{
    if(section == -1)
        m_model->setSort(SPartSuppliersModel::Columns::Id, Qt::AscendingOrder);
    else
        m_model->setSort(section, order);

    if(m_requestId >= 0)
        m_model->select(m_requestId);
}

void SPartSuppliers::removeSelected()
{
    QItemSelectionModel *selection = ui->tableViewLinks->selectionModel();

    if(!selection->hasSelection())
        return;

    QModelIndexList rows = selection->selectedRows();
    QModelIndexList::const_iterator it = rows.constEnd();
    while(--it >= rows.constBegin())
    {
        int id = m_model->index(it->row(), SPartSuppliersModel::Columns::Id).data().toInt();

        m_model->removeRows(it->row(), 1);
        if(id)
            ui->tableViewLinks->hideRow(it->row());
    }
}

void SPartSuppliers::enableEdit(const bool state)
{
    ui->pushButtonAdd->setEnabled(state);
    ui->pushButtonSave->setEnabled(state);
    ui->pushButtonDelete->setEnabled(state);
}

void SPartSuppliers::initTableMenu()
{
    tableMenu = new QMenu(this);
    QAction * copyLink = new QAction(tr("Копировать значение"), tableMenu);
    connect(copyLink, &QAction::triggered, this, &SPartSuppliers::copyLink);
    tableMenu->addAction(copyLink);

//    QAction * filterBySupplier = new QAction(tr("Фильтр по поставщику"), tableMenu);
//    connect(filterBySupplier, &QAction::triggered, this, ...));
//    tableMenu->addAction(filterBySupplier);
}

/* Добавление новой строки в таблицу
 * Перед добавлением список сохраняется; это сделано для того, чтобы избежать потери данных в случае,
 * когда пользователь последовательно добавил несколько ссылок, но при сохранении произошла ошибка выполнения запроса.
 * Например, если в одной из строк не будет указан URL товара (поле NOT NULL), произойдёт ошибка.
*/
void SPartSuppliers::addRow()
{
    emit beginRowInsert();
    m_model->appendRow();
}

void SPartSuppliers::copyLink()
{
    if(!ui->tableViewLinks->selectionModel()->hasSelection())
        return;

    QModelIndex index = ui->tableViewLinks->selectionModel()->currentIndex();
    QString data = index.data().toString();
    qApp->clipboard()->setText(data);
}

void SPartSuppliers::setRequestState(const int state)
{
    m_model->setRequestState(state);

    switch(state)
    {
        case SPartRequest::State::Cancelled:
        case SPartRequest::State::Finished: enableEdit(false); break;
        default: enableEdit(true);
    }
}

/* Добавление указанного поставщика в список.
 * При создании или редактировании запроса, выбранный поользователем поставщик
 * будет автоматически добавлен в список ссылок. Пользователь, при желании, может указать
 * URL товара, мин. кол-во для заказа, цену и комментарий; указывать URL поставщика не требуется,
 * т. к. на вкладке Менеджер закупок для группировки запросов по поставщику будет использоваться id
 * контрагента.
 * Указанный id запоминается в контексте текущей вкладки и используется при добавлении записи
 * по нажатию кнопки Добавить.
*/
void SPartSuppliers::addSupplierRecord(const int id)
{
    if(id == 0)
    {
        m_model->setPredefSupplierId(id);
        return;
    }

    m_model->addSupplierRecord(id);
}

void SPartSuppliers::menuRequest(QPoint pos)
{
    tableMenu->popup(ui->tableViewLinks->viewport()->mapToGlobal(pos));
}

