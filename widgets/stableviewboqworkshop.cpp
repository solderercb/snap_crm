#include <MainWindow>
#include "stableviewboqworkshop.h"
#include <STableViewGridLayout>
#include <SSaleTableModel>

STableViewBOQWorkshop::STableViewBOQWorkshop(QWidget *parent) :
    STableViewBase(SLocalSettings::RepairWorksGrid, parent)
{
    // столбец "Доступно" (4) скрыт
    i_defaultColumnsWidths = {{0, 60},{1, 90},{2, 270},{3, 45},{4, 0},{5, 70},{6, 70},{7, 120},{8, 120},{9, 80},{10, 100}};
    i_defaultHeaderLabels << tr("") << tr("UID") << tr("Наименование") << tr("Кол-во") << tr("Доступно") << tr("Цена") << tr("Сумма") << tr("Место") << tr("Серийный номер") << tr("Гарантия") << tr("Сотрудник");
    readLayout();
    i_gridLayout->$GridControl.Columns[2].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

STableViewBOQWorkshop::~STableViewBOQWorkshop()
{
}

void STableViewBOQWorkshop::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SSaleTableModel*>(model);
    STableViewBase::setModel(model);
}

void STableViewBOQWorkshop::mouseDoubleClickEvent(QMouseEvent *event)
{
    int row = currentIndex().row();
    if(m_model->recordType(row))
    {
        if(event->modifiers() == Qt::ControlModifier)
        {
            clearSelection();
            selectionModel()->select(currentIndex(), QItemSelectionModel::Select);
            MainWindow::getInstance()->createTabSparePart(m_model->index(row, SSaleTableModel::Columns::ItemId).data().toInt());
        }
        else
            MainWindow::getInstance()->createTabSparePartReserve(m_model->index(row, SSaleTableModel::Columns::Id).data().toInt());
    }
}

#if QT_VERSION >= 0x060000
void STableViewBOQWorkshop::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void STableViewBOQWorkshop::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
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
                case SSaleTableModel::Columns::Name: resizeRowToContents(row); break;
                default: /*resizeColumnToContents(col)*/;
            }
        }
}
