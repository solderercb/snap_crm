#include "stableviewboqstore.h"

STableViewBOQStore::STableViewBOQStore(QWidget *parent) :
    STableViewBase(SLocalSettings::SaleItemsGrid, parent)
{
    i_defaultColumnsWidths = {{0, 60},{1, 90},{2, 270},{3, 45},{4, 60},{5, 70},{6, 70},{7, 120},{8, 120},{9, 80}};
    i_defaultHeaderLabels << tr("") << tr("UID") << tr("Наименование") << tr("Кол-во") << tr("Доступно") << tr("Цена") << tr("Сумма") << tr("Место") << tr("Серийный номер") << tr("Гарантия");
    readLayout();
    i_gridLayout->$GridControl.Columns[2].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewBOQStore::~STableViewBOQStore()
{
}

void STableViewBOQStore::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SSaleTableModel*>(model);
    STableViewBase::setModel(model);
    STableViewBOQItemDelegates *itemDelagates = new STableViewBOQItemDelegates(m_model, this);
    setItemDelegate(itemDelagates);
}

void STableViewBOQStore::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    int row = currentIndex().row();
    if(m_model->recordType(row))
    {
        clearSelection();
        selectionModel()->select(currentIndex(), QItemSelectionModel::Select);
        emit createTabSparePart(m_model->index(row, SStoreItemModel::SaleOpColumns::ColItemId).data().toInt());
    }
}

/* Пустышка
 * Необходима для избежания потенциальных сбоев при вызове метода в родительском классе
*/
void STableViewBOQStore::clearModel()
{

}

/* Пустышка
 * Необходима для избежания потенциальных сбоев при вызове метода в родительском классе
*/
void STableViewBOQStore::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    Q_UNUSED(query)
    Q_UNUSED(database)
}

