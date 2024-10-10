#include "srequeststable.h"
#include "../../global.h"
#include "qlistview.h"

STableViewPartsRequests::STableViewPartsRequests(QWidget *parent) :
    STableViewBase(SLocalSettings::BuyRequestManagerGrid, parent)
{
    setUniqueIdColumn(Column::Id);
    readLayout();

    STableViewPartsRequestsItemDelegates *itemDelegates = new STableViewPartsRequestsItemDelegates();
    setItemDelegate(itemDelegates);
    viewport()->installEventFilter(this);
}

STableViewPartsRequests::~STableViewPartsRequests()
{

}

void STableViewPartsRequests::setDefaultLayoutParams()
{
    STableViewBase::setDefaultLayoutParams();
    i_gridLayout->$GridControl.Columns[Column::ItemName].Width_marked = true;  // автоширина по умолчанию
    if(m_layoutVariant == SLocalSettings::RepairPartsRequestsGrid)
    {
        setColumnLayoutHidden(Column::Check, true);
        setColumnLayoutHidden(Column::Customer, true);
//        setColumnLayoutHidden(Column::Dealer, true);
        setColumnLayoutHidden(Column::Repair, true);
        setColumnLayoutHidden(Column::Track, true);
    }
}

bool STableViewPartsRequests::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    if(object == viewport())
    {
        switch(event->type())
        {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonDblClick:
            case QEvent::MouseMove: return mouseEventOnCell(event);
            default: break;
        }
    }

    return false;
}

bool STableViewPartsRequests::mouseEventOnCell(QEvent *event)
{
    // Если ни одна строка в таблице не выделена проверять изменения не нужно
    QModelIndexList selection = selectionList();
    if(selection.empty())
        return false;

    QMouseEvent *e = static_cast<QMouseEvent*>(event);
    QModelIndex index = indexAt(e->localPos().toPoint());

    // При щелчке по другой ячейке в той же (выделенной) строке проверять изменения не нужно
    if(selection.first().row() == index.row())
        return false;

    int result = 0;
    emit checkChanges(result, 1);

    switch (result)
    {
        case QMessageBox::Cancel: return true;
        case QMessageBox::No: return false;
        default: ;
    }

    return false;
}

void STableViewPartsRequests::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SPartsRequestsModel*>(model);
    STableViewBase::setModel(model);
}

void STableViewPartsRequests::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    m_model->setQuery(query, database);
}

void STableViewPartsRequests::clearModel()
{
    m_model->clear();
}

void STableViewPartsRequests::setSorting(const int logicalIndex, const Qt::SortOrder order)
{
    int result = 0;
    // TODO: нужно придумать как обрабатывать кнопку Cancel, а именно блокировать событие щелчка по секции.
    // а пока будет выводиться сообщение с двумя кнопками
    emit checkChanges(result, 3, QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No));
    if(result != QMessageBox::Cancel)
        STableViewBase::setSorting(logicalIndex, order);
    else
        undoToggleSortIndicator();
}

/* Переопределённый метод.
 * Т. к. метод SEditableBaseModel::clear фактически не очищает модель (см. описание к нему)
 * то после обновления таблицы не происходит вызова слота STableViewBase::vsp_rangeChanged и
 * положение прокрутки никогда не восстанавливается.
*/
void STableViewPartsRequests::refresh(bool preserveScrollPos, bool preserveSelection)
{
    STableViewBase::refresh(preserveScrollPos, preserveSelection);
    if(preserveScrollPos == ScrollPosPreserve)
        restoreVScrollPos();
}

void STableViewPartsRequests::translateNames()
{
    tr("Check");
    tr("Id");
    tr("Date");
    tr("Employee");
    tr("Customer");
    tr("ItemName");
    tr("Count");
    tr("Dealer");
    tr("Repair");
    tr("State");
    tr("Track");
    tr("Priority");
    tr("Progress");
}

/*******************************************************************************************************************************
*/
STableViewPartsRequestsItemDelegates::STableViewPartsRequestsItemDelegates(QWidget *parent) :
    STableViewBaseItemDelegates(parent)
{

}

STableViewPartsRequestsItemDelegates::STableViewPartsRequestsItemDelegates(SPartsRequestsModel *model, QWidget *parent) :
    STableViewPartsRequestsItemDelegates(parent)
{
    setTableModel(model);
}

STableViewPartsRequestsItemDelegates::~STableViewPartsRequestsItemDelegates()
{

}

void STableViewPartsRequestsItemDelegates::setTableModel(QAbstractItemModel *model)
{
    SPartsRequestsModel *m = dynamic_cast<SPartsRequestsModel*>(model);
    m_tableModel = m;
}

bool STableViewPartsRequestsItemDelegates::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonRelease && index.column() == STableViewPartsRequests::Check)
    {
        QMouseEvent * e = (QMouseEvent *)event;
        if(e->button() == Qt::LeftButton)
        {
            if( model->flags(index) & (Qt::ItemIsEditable | Qt::ItemIsUserCheckable) )
            {
                model->setData(index, model->data(index, Qt::EditRole).toInt()?0:2, Qt::EditRole);
                event->accept();
                return true;
            }
        }
    }
    return STableViewBaseItemDelegates::editorEvent(event, model, option, index);
}

QWidget *STableViewPartsRequestsItemDelegates::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *w;
    switch (index.column())
    {
        case STableViewPartsRequests::Check: return nullptr;
        case STableViewPartsRequests::Count: w = createSpinBox(parent, index); break;
        case STableViewPartsRequests::State: w = createComboBox(parent, partRequestStatesListModel); break;
        case STableViewPartsRequests::Priority: w = createComboBox(parent, partRequestPrioritiesListModel); break;
        default: w = STableViewBaseItemDelegates::createEditor(parent, option, index);
    }
    return w;
}

void STableViewPartsRequestsItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
        case STableViewPartsRequests::Count: setSpinBoxData(editor, index.data().toInt()); break;
        case STableViewPartsRequests::State:
        case STableViewPartsRequests::Priority:  setComboBoxData(editor, index.data().toString()); break;
        default: STableViewBaseItemDelegates::setEditorData(editor, index);
    }
}

void STableViewPartsRequestsItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
        case STableViewPartsRequests::Count: setModelDataFromSpinBox(editor, model, index); break;
        case STableViewPartsRequests::State:
        case STableViewPartsRequests::Priority: setModelDataFromComboBox(editor, model, index); break;
        default: STableViewBaseItemDelegates::setModelData(editor, model, index);
    }
}

bool STableViewPartsRequestsItemDelegates::eventFilter(QObject *object, QEvent *event)
{
    return STableViewBaseItemDelegates::eventFilter(object, event);
}

QComboBox *STableViewPartsRequestsItemDelegates::createComboBox(QWidget *parent, QAbstractItemModel *model) const
{
    QComboBox *cb = STableViewBaseItemDelegates::createComboBox(parent, model);
    if(model == partRequestStatesListModel)
    {
        QListView *view = static_cast<QListView*>(cb->view());
        view->setRowHidden(partRequestStatesListModel->rowByDatabaseID(SPartRequest::NewUncommitted), true);
    }

    return cb;
}

void STableViewPartsRequestsItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == STableViewPartsRequests::Column::Progress)
    {
        paintProgressBar(painter, option, index);
    }
    else
        STableViewBaseItemDelegates::paint(painter, option, index);
}

void STableViewPartsRequestsItemDelegates::paintProgressBar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    uint progress = 0;
    QDateTime created = m_tableModel->unformattedData(index.siblingAtColumn(SPartsRequestsModel::Columns::Created)).toDateTime();
    created.setTimeZone(QTimeZone::utc());
    QDateTime plannedEnd;
    plannedEnd = m_tableModel->unformattedData(index.siblingAtColumn(SPartsRequestsModel::Columns::PlanEndDate)).toDateTime();
    plannedEnd.setTimeZone(QTimeZone::utc());
    QDateTime current = QDateTime::currentDateTime();
    current.setTimeZone(QTimeZone::utc());
    qint64 secondsSinceInDate = created.secsTo(current);
    qint64 secondsToEnd = created.secsTo(plannedEnd);

    if(secondsToEnd)
    {
        progress = qMin(100, (int)((100 * secondsSinceInDate)/secondsToEnd));
    }

    QStyleOptionProgressBar progressBar;
    QRect r2 = option.rect;
    progressBar.rect = r2;
    progressBar.textAlignment = Qt::AlignCenter;
    progressBar.minimum = 0;
    progressBar.maximum = 100;
    progressBar.progress = progress;
    progressBar.text = QString::number(progress) + "%";
    progressBar.textVisible = true;
    progressBar.state = option.state | QStyle::State_Horizontal;

    STableViewBaseItemDelegates::paintProgressBar(&progressBar, painter, option.widget);
}
