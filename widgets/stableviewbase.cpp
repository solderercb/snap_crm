#include "stableviewbase.h"

STableViewBase::STableViewBase(QWidget *parent) :
    QTableView(parent)
{
    m_fontMetrics = new QFontMetrics(this->font());
    i_gridLayout = new XtraSerializer();
    i_gridLayout->$GridControl.Columns.resize(0);
    installEventFilter(this);
    initHorizontalHeaderMenu();
    horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);

    verticalHeader()->hide();
    horizontalHeader()->setMinimumSectionSize(15);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    horizontalHeader()->setHighlightSections(false);
    connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &STableViewBase::horizontalHeaderMenuRequest);
    connect(horizontalHeader(),&QHeaderView::sortIndicatorChanged, this, &STableViewBase::orderChanged);

    connect(this->horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this, SLOT(columnResized(int,int,int)));
}

STableViewBase::~STableViewBase()
{
    delete m_fontMetrics;
    delete i_gridLayout;
    clearFilter();
    clearGrouping();
    deleteHorizontalHeaderMenu();
    if(i_itemDelegates)
        delete i_itemDelegates;
}

void STableViewBase::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    adoptAutosizedColumns();
//    applyGridlayout();
}

void STableViewBase::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<STableBaseModel*>(model);
    QTableView::setModel(model);
    if(i_itemDelegates)
        i_itemDelegates->setTableModel(m_model);
    applyGridlayout();
}

void STableViewBase::setStoreItemsCategory(const int category)
{
    m_storeItemsCategory = category;
}

bool STableViewBase::eventFilter(QObject *object, QEvent *event)
{
    if(QString("QScrollBar").compare(object->metaObject()->className()) == 0)
    {
        if(static_cast<QScrollBar *>(object)->orientation() == Qt::Vertical && event->type() == QEvent::Show)
        {
            adoptAutosizedColumns();
        }
    }
    return false;
}

void STableViewBase::setItemDelegate(STableViewBaseItemDelegates *delegate)
{
    i_itemDelegates = delegate;
    QTableView::setItemDelegate(i_itemDelegates);
    i_itemDelegates->setFontMetrics(this->font());
}

int STableViewBase::sizeHintForColumn(int column) const
{
    return i_defaultColumnsWidths[column, 0];
}

QVariant STableViewBase::headerData(int section, Qt::Orientation orientation, int role) const
{
    return m_model->headerData(section, orientation, role);
}

int STableViewBase::columnSizeByContents(int column)
{
    if(!m_model)
        return 0;
    if(m_model->rowCount() == 0 || column >= m_model->columnCount())
        return i_defaultColumnsWidths[column];

    int cellWidth = 0;

    for(int i = 0; i < m_model->rowCount(); i++)
        cellWidth = qMax(cellWidth, m_fontMetrics->size(Qt::TextSingleLine, m_model->index(i, column).data().toString()).width() + 10);
    cellWidth = qMax(cellWidth, m_fontMetrics->size(Qt::TextSingleLine, m_model->headerData(column, Qt::Horizontal).toString()).width() + 10);

    return cellWidth;
}

void STableViewBase::resizeColumnToContents(int column)
{
    int columnWidth = 0;
    if(i_gridLayout->$GridControl.Columns[column].Visible)
    {
        columnWidth = columnSizeByContents(column);
    }

    setColumnWidth(column, columnWidth);
}

void STableViewBase::resizeColumnsToContents()
{
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        resizeColumnToContents(i);
    }
}

void STableViewBase::applyGridlayout()
{
    QMap<int, int> visualIndexes;
    int i;

    if(i_gridLayout->$GridControl.Columns.size() == 0)
        return;

    initAutosizedColumns();
    for(i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        if(i_gridLayout->$GridControl.Columns[i].Visible)
        {
            visualIndexes.insert(i_gridLayout->$GridControl.Columns[i].VisibleIndex, i);
        }
        else
            horizontalHeader()->hideSection(i);

//        if(!i_gridLayout->$GridControl.Columns[i].Width_marked)
        {
            setColumnWidth(i, i_gridLayout->$GridControl.Columns[i].ActualWidth);
        }
    }

    // Установка порядка стобцов
    for(int i : visualIndexes.keys())
    {
        horizontalHeader()->moveSection(horizontalHeader()->visualIndex(visualIndexes[i]), i);
    }

//    adoptAutosizedColumns();
}

void STableViewBase::applySorting()
{
    if (i_gridLayout->$GridControl.SortInfo.size())
    {
        m_sortColumn = columnByName(i_gridLayout->$GridControl.SortInfo[0].FieldName);
        m_sortOrder = (i_gridLayout->$GridControl.SortInfo[0].SortOrder.compare("Ascending") == 0)?Qt::AscendingOrder:Qt::DescendingOrder;
    }
    else
    {
        m_sortColumn = -1;
        m_sortOrder = Qt::AscendingOrder;
    }
    horizontalHeader()->blockSignals(true);
    horizontalHeader()->setSortIndicator(m_sortColumn, m_sortOrder);
    horizontalHeader()->blockSignals(false);
}

void STableViewBase::initAutosizedColumns()
{
    m_autosizedColumnsSummaryActualWidth = 0;
    m_autosizedColumnsSummaryDefaultWidth = 0;
    m_autosizedColumns.clear();
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        if(i_gridLayout->$GridControl.Columns[i].Width_marked)
        {
            m_autosizedColumns.insert(i, i_gridLayout->$GridControl.Columns[i].ActualWidth);
            m_autosizedColumnsSummaryActualWidth += i_gridLayout->$GridControl.Columns[i].ActualWidth;
            m_autosizedColumnsSummaryDefaultWidth += i_gridLayout->$GridControl.Columns[i].Width;
        }
    }
}

// настройка ширин столбцов, помеченных *
void STableViewBase::adoptAutosizedColumns()
{
    int spaceForVariableColumns = 0;    // доступное место для столбцов с автоподбором ширины
    if(m_autosizedColumns.size())
    {
        spaceForVariableColumns = visibleWidth();

        for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
        {
            if(!i_gridLayout->$GridControl.Columns[i].Width_marked)
            {
                spaceForVariableColumns -= columnWidth(i);
            }
        }
        for(int i : m_autosizedColumns.keys())
        {
            if(m_autosizedColumnsSummaryActualWidth <= spaceForVariableColumns)
            {
                // Если столбцы с автонастройкой ширины вписываются в доступное место, то их ширина рассчитывается пропорционально ширине текста
                setColumnWidth(i, spaceForVariableColumns*m_autosizedColumns[i]/m_autosizedColumnsSummaryActualWidth);
            }
            else
            {
                // Если столбцы с автонастройкой ширины требуют больше места, чем доступно, то их ширина рассчитывается пропорционально ширине, заданной в поле Width, но не меньше.
                setColumnWidth(i, qMax(spaceForVariableColumns*i_gridLayout->$GridControl.Columns[i].Width/m_autosizedColumnsSummaryDefaultWidth, i_gridLayout->$GridControl.Columns[i].Width));
            }
        }
    }
}

void STableViewBase::setColumnWidth(int column, int width)
{
    QTableView::setColumnWidth(column, width);
}

void STableViewBase::setDefaultLayoutParams()
{
    int count = qMax(i_defaultColumnsWidths.size(), i_defaultHeaderLabels.size());
    i_gridLayout->$GridControl.Columns.resize(count);

    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        setDefaultColumnParams(i, i_defaultHeaderLabels.value(i, ""), i_defaultColumnsWidths.value(i, 0));
    }
}

void STableViewBase::setDefaultColumnParams(const int column, const QString &label, const int width)
{
    i_gridLayout->$GridControl.Columns[column].Width = width;
    i_gridLayout->$GridControl.Columns[column].ActualWidth = width;
    i_gridLayout->$GridControl.Columns[column].Visible = 1;
    i_gridLayout->$GridControl.Columns[column].FieldName = label;
    i_gridLayout->$GridControl.Columns[column].VisibleIndex = column;
}

void STableViewBase::readLayout(SLocalSettings::SettingsVariant variant)
{
    if( (!localSettings->read(i_gridLayout, variant))/* || (i_gridLayout->$GridControl.Columns.size() != i_defaultHeaderLabels.size())*/ )
    {
        setDefaultLayoutParams();
        localSettings->save(i_gridLayout, variant);
    }
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {   // в настройках АСЦ параметр Visible может отсутствовать; значение по умолчанию для такого случая true
        // TODO: добавить в QSerializer параметр по умолчанию для QS_FIELD
        if(i_gridLayout->$GridControl.Columns[i].ActualWidth)
            i_gridLayout->$GridControl.Columns[i].Visible = true;
    }

    applyGridlayout();
    applySorting();
}

void STableViewBase::saveLayout(SLocalSettings::SettingsVariant variant)
{
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        i_gridLayout->$GridControl.Columns[i].ActualWidth = columnWidth(i);
        i_gridLayout->$GridControl.Columns[i].Visible = !horizontalHeader()->isSectionHidden(i);
        i_gridLayout->$GridControl.Columns[i].VisibleIndex = horizontalHeader()->visualIndex(i);
    }

    if(m_sortColumn >= 0)
    {
        if(i_gridLayout->$GridControl.SortInfo.size() == 0)
            i_gridLayout->$GridControl.SortInfo.append(t_SortInfo());

        QString newSortingColumn = i_gridLayout->$GridControl.Columns[m_sortColumn].FieldName;
        QString newSortOrder = (m_sortOrder == Qt::AscendingOrder)?"Ascending":"Descending";

        i_gridLayout->$GridControl.SortInfo[0].FieldName = newSortingColumn;
        i_gridLayout->$GridControl.SortInfo[0].SortOrder = newSortOrder;
    }
    else
    {
        if(i_gridLayout->$GridControl.SortInfo.size())
        {
            i_gridLayout->$GridControl.SortInfo.clear();
        }
    }

    localSettings->save(i_gridLayout, variant);
}

void STableViewBase::columnResized(int column, int oldWidth, int newWidth)
{
    if(column < i_gridLayout->$GridControl.Columns.size())
    {
        if(newWidth == 0)
        {
            i_gridLayout->$GridControl.Columns[column].Visible = false;
        }
        else
        {
            i_gridLayout->$GridControl.Columns[column].Visible = true;
            i_gridLayout->$GridControl.Columns[column].ActualWidth = newWidth;
        }
        if(!m_autosizedColumns.contains(column))
            adoptAutosizedColumns();
    }
    QTableView::columnResized(column, oldWidth, newWidth);
//    verticalHeader()->resizeSections(QHeaderView::ResizeToContents); // ЭТО ДОЛГО!!!
}

void STableViewBase::reset()
{
    int i;
    int modelColumnCount = m_model->columnCount();
    int layoutColumnCount = i_gridLayout->$GridControl.Columns.size();

    for(i = 0; i < layoutColumnCount && i < modelColumnCount; i++)
    {
        if(i_gridLayout->$GridControl.Columns[i].Visible)
            m_model->setHeaderData(i, Qt::Horizontal, i_gridLayout->$GridControl.Columns[i].FieldName);
    }
    for(i = layoutColumnCount; i < modelColumnCount; i++)
    {   // столбцы, не описанные в файле настроек, — служебные; их необходимо скрыть
        horizontalHeader()->hideSection(i);
    }
    QTableView::reset();

//    if(m_modelRowCount == m_model->rowCount())
//        return;

//    m_modelRowCount = m_model->rowCount();
    applyGridlayout();  // при глобальном обновлении модели ширины столбцов устанавливаются по умолчанию, поэтому их нужно снова задать из файла
}

#if QT_VERSION >= 0x060000
void STableViewBase::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void STableViewBase::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    QTableView::dataChanged(topLeft, bottomRight, roles);
    if(!roles.isEmpty() && !roles.contains(Qt::DisplayRole))
        return;

    for(int col = topLeft.column(); col <= bottomRight.column(); col++)
    {
        if(i_gridLayout->$GridControl.Columns[col].Width_marked)
            continue;

        int diff = columnSizeByContents(col) - columnWidth(col);
        if(diff > 0)
            adoptAutosizedColumns();
    }
}

void STableViewBase::orderChanged(int logicalIndex, Qt::SortOrder order)
{

    if(m_sortColumn == -1 || m_sortColumn != logicalIndex)
        m_sortOrder = Qt::AscendingOrder;
    else if(m_sortOrder == Qt::AscendingOrder)
        m_sortOrder = Qt::DescendingOrder;
    else
    {
        clearSorting();
        return;
    }

    m_sortColumn = logicalIndex;

    refresh();
}

void STableViewBase::clearSorting()
{
    m_sortColumn = -1;
    horizontalHeader()->setSortIndicator(m_sortColumn, Qt::AscendingOrder);
    refresh();
}

int STableViewBase::visibleWidth()
{
    int w;
    w = this->width() - this->frameWidth()*2;
    if(verticalHeader()->isVisible())
        w -= verticalHeader()->width();
    if (verticalScrollBar()->isVisible())
        w -= verticalScrollBar()->width();

    return w;
}

void STableViewBase::initHorizontalHeaderMenu()
{
    horizontalHeaderMenu = new QMenu(this);

    QAction * autoWidth = new QAction(tr("Автоширина"), horizontalHeaderMenu);
    autoWidth->setCheckable(true);
    autoWidth->setProperty("type", horizontalHeaderMenuActions::ToggleAutoWidth);
    connect(autoWidth, SIGNAL(triggered(bool)), this, SLOT(toggleAutoWidth(bool)));
    horizontalHeaderMenu->addAction(autoWidth);

    QAction * fitContent = new QAction(tr("Размер по содержимому"), horizontalHeaderMenu);
    fitContent->setProperty("type", horizontalHeaderMenuActions::FitContent);
    connect(fitContent, SIGNAL(triggered()), this, SLOT(fitContent()));
    horizontalHeaderMenu->addAction(fitContent);

    QAction * bestFitAll = new QAction(tr("Вписать все"), horizontalHeaderMenu);
    bestFitAll->setProperty("type", horizontalHeaderMenuActions::BestFitAll);
    connect(bestFitAll, SIGNAL(triggered()), this, SLOT(bestFitAll()));
    horizontalHeaderMenu->addAction(bestFitAll);

    QAction * setDefault = new QAction(tr("Сделать значением по умолчанию"), horizontalHeaderMenu);
    setDefault->setProperty("type", horizontalHeaderMenuActions::SetDefault);
    connect(setDefault, SIGNAL(triggered()), this, SLOT(setDefault()));
    horizontalHeaderMenu->addAction(setDefault);

    QAction * hideColumn = new QAction(tr("Скрыть"), horizontalHeaderMenu);
    hideColumn->setDisabled(true);
    hideColumn->setProperty("type", horizontalHeaderMenuActions::Hide);
    connect(hideColumn, SIGNAL(triggered()), this, SLOT(hideColumn()));
    horizontalHeaderMenu->addAction(hideColumn);

    QAction * columnChooser = new QAction(tr("Выбрать столбцы"), horizontalHeaderMenu);
    columnChooser->setDisabled(true);
    columnChooser->setProperty("type", horizontalHeaderMenuActions::ColumnChooser);
    connect(columnChooser, SIGNAL(triggered()), this, SLOT(showColumnChooser()));
    horizontalHeaderMenu->addAction(columnChooser);
}

void STableViewBase::deleteHorizontalHeaderMenu()
{
    for(QAction *a : horizontalHeaderMenu->actions())
    {
        horizontalHeaderMenu->removeAction(a);
        delete a;
    }
    delete horizontalHeaderMenu;
}

void STableViewBase::horizontalHeaderMenuRequest(const QPoint &pos) const
{
    int col = horizontalHeader()->logicalIndexAt(pos);
    horizontalHeaderMenu->setProperty("column", col);
    const QList<QAction*> actions = horizontalHeaderMenu->actions();
    for(QAction *action : actions)
    {
        switch (action->property("type").toInt())
        {
            case horizontalHeaderMenuActions::ToggleAutoWidth: action->setChecked(i_gridLayout->$GridControl.Columns[col].Width_marked); break;
            case horizontalHeaderMenuActions::FitContent: action->setDisabled(i_gridLayout->$GridControl.Columns[col].Width_marked); break;
        }
    }

    horizontalHeaderMenu->popup(mapToGlobal(pos));
}

void STableViewBase::toggleAutoWidth(bool)
{
    int col = horizontalHeaderMenu->property("column").toInt();
    i_gridLayout->$GridControl.Columns[col].Width_marked ^= 1;
    initAutosizedColumns();
    adoptAutosizedColumns();
}

void STableViewBase::fitContent()
{
    int col = horizontalHeaderMenu->property("column").toInt();
    resizeColumnToContents(col);
}

void STableViewBase::bestFitAll()
{
    QMap<int, int> widths;
    int sumOfWidths = 0;
    int spaceForVariableColumns = visibleWidth();

    // Опледеление ширин столбцов по содержимому ячеек и подсчет суммы ширин
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
    {
        widths.insert(i, columnSizeByContents(i));
        sumOfWidths += widths[i];
    }

    // пропорциональное изменение ширин
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size() && sumOfWidths; i++)
    {
//        setColumnWidth(i, qMax(spaceForVariableColumns*widths[i]/sumOfWidths, i_gridLayout->$GridControl.Columns[i].Width));
        setColumnWidth(i, spaceForVariableColumns*widths[i]/sumOfWidths);
    }
}

/* Запись значения ActualWidth в поле Width*/
void STableViewBase::setDefault()
{
    int col = horizontalHeaderMenu->property("column").toInt();
    i_gridLayout->$GridControl.Columns[col].Width = i_gridLayout->$GridControl.Columns[col].ActualWidth;
}

void STableViewBase::hideColumn()
{
    int col = horizontalHeaderMenu->property("column").toInt();
    horizontalHeader()->hideSection(col);
}

void STableViewBase::showColumnChooser()
{

}

void STableViewBase::setQuery(const QString& query, const QSqlDatabase& db)
{
    m_query = query;
    m_db = db;

    clearFilter();
    clearGrouping();
}

void STableViewBase::setFilter(const FilterList &filter)
{
    if(m_filter)
        delete m_filter;

    m_filter = new FilterList;
    *m_filter = filter;
}

void STableViewBase::clearFilter()
{
    if(m_filter)
        delete m_filter;
    m_filter = nullptr;
}

QString STableViewBase::formatFilterGroup(const FilterList &filter)
{

    QString subCond;
    QStringList cond;

    if(&filter == nullptr)
        return QString();

    if(filter.childs.count())
    {
        QList<FilterList>::const_iterator i = filter.childs.constBegin();
        while(i != filter.childs.constEnd())
        {
            subCond = formatFilterGroup(*i);
            if(!subCond.isEmpty())
                cond << QString("(%1)").arg(subCond);
            i++;
        }
    }

    if(filter.fields.count())
    {
        QList<FilterField>::const_iterator i = filter.fields.constBegin();
        while(i != filter.fields.constEnd())
        {
            subCond = formatFilterField(*i);
            if(!subCond.isEmpty())
                cond << subCond;
            i++;
        }
    }

    return cond.join(filter.op?" OR ":" AND ");
}

QString STableViewBase::formatFilterField(const FilterField &field)
{
    if (field.column.isEmpty())
        return QString();
    if (field.operation != FilterField::NoOp && field.operation != FilterField::Null && field.value.toString().isEmpty())
        return QString();

    QSqlDriver* driver = m_db.driver();
    QString result;
    QString escFilterString = field.value.toString();
    QString escFilterColumn = field.column;
//    escFilterColumn = driver->escapeIdentifier(field->column, QSqlDriver::FieldName);

    switch(field.operation & ~FilterField::NotMark)
    {
        case FilterField::Equals:
        {
            if(field.operation & FilterField::NotMark)
                result = "%1 <> %2";
            else
                result = "%1 = %2";
            break;
        }
        case FilterField::StartsWith:
        case FilterField::EndsWith:
        case FilterField::Wildcard:
        case FilterField::Contains:
        {
            if(field.operation & FilterField::NotMark)
                result = "%1 NOT LIKE %2";
            else
                result = "%1 LIKE %2";
            break;
        }
        case FilterField::RegExp:
        {
            if(field.operation & FilterField::NotMark)
                result = "%1 NOT REGEXP %2";
            else
                result = "%1 REGEXP %2";
            break;
        }
        case FilterField::NoOp:
        {
            return escFilterColumn;
            break;
        }
        case FilterField::Null:
        {
            if(field.operation & FilterField::NotMark)
                result = "%1 IS NOT NULL";
            else
                result = "%1 IS NULL";
            break;
        }
        default: return QString(); // unhandled filterflag
    }

    switch(field.operation & ~FilterField::NotMark)
    {
        case FilterField::Contains: escFilterString.prepend("%");
        case FilterField::StartsWith: escFilterString.append("%"); break;
        case FilterField::EndsWith:  escFilterString.prepend("%"); break;
    }

    QSqlField f;
    f.setType(field.value.type());
    f.setValue(escFilterString);

    escFilterString = driver->formatValue(f);

    if(field.caseSensitivity == Qt::CaseInsensitive)
    {
        escFilterColumn = QString("LCASE(%1)").arg(escFilterColumn);
        escFilterString = QString("LCASE(%1)").arg(escFilterString);
    }
//    if (m_filterFlags & Qt::MatchStartsWith || Qt::MatchEndsWith || Qt::MatchWildcard)
//    {
//        escFilterString = escFilterString.replace("", "");
//        // if (m_filterFlags & Qt::MatchWildcard)
//        escFilterString = escFilterString.replace("*", "");
//    }

    result = result.replace("%1", escFilterColumn).replace("%2", escFilterString);
    return result;
}

/* Определение логического номера столбца по имени */
int STableViewBase::columnByName(const QString &name)
{
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.size(); i++)
        if(i_gridLayout->$GridControl.Columns[i].FieldName.compare(name) == 0)
            return i;
    return -1;
}

void STableViewBase::refresh()
{
    if (m_query.isEmpty() || (!m_db.isValid()))
        return;

    QString query = m_query;
    QString filter = formatFilterGroup(*m_filter);

    if(!filter.isEmpty())
        query.append("\nWHERE " + filter);

    if(m_grouping)
        query.append(m_grouping->count()>0?"\nGROUP BY " + m_grouping->join(", "):"");

    if (m_sortColumn >= 0)
    {
        QString orderClause;
        int field = m_sortColumn + 1; // нумерация полей в запросе с 1
        orderClause = QString("\nORDER BY %1 %2").arg(field).arg((m_sortOrder == Qt::AscendingOrder)?"ASC":"DESC");
        query.append(orderClause);

    }

#ifdef QT_DEBUG
//    query.append(" LIMIT 1000");
#endif

    m_model->setQuery(query, QSqlDatabase::database("connMain"));
}

void STableViewBase::filter(const FilterList &filter)
{
    clearFilter();
    setFilter(filter);
    refresh();
}

/* Описание поля для фильтра в одну строку. Для удобства и компактности кода */
FilterField STableViewBase::initFilterField(const QString &column, FilterField::Op operation, const QVariant &value, Qt::CaseSensitivity caseSensitivity)
{
    FilterField f;
    f.column = column;
    f.operation = operation;
    f.value = value;
    f.caseSensitivity = caseSensitivity;

    return f;
}

void STableViewBase::setGrouping(const QStringList &grouping)
{
    clearGrouping();
    m_grouping = new QStringList(grouping);
}

void STableViewBase::clearGrouping()
{
    if(m_grouping)
        delete m_grouping;
    m_grouping = nullptr;
}

