#include "stableviewboqitemdelegates.h"

STableViewBOQItemDelegates::STableViewBOQItemDelegates(QObject *parent) : STableViewBaseItemDelegates(parent)
{

}

STableViewBOQItemDelegates::STableViewBOQItemDelegates(SSaleTableModel *model, QObject *parent) :
    STableViewBOQItemDelegates(parent)
{
    setTableModel(model);
    // пример itemDelegate взят по https://wiki.qt.io/Combo_Boxes_in_Item_Views
}

STableViewBOQItemDelegates::~STableViewBOQItemDelegates()
{
}

QWidget *STableViewBOQItemDelegates::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
        case SStoreItemModel::SaleOpColumns::ColCount:
            return createSpinBox(parent, index); break;
        case SStoreItemModel::SaleOpColumns::ColPrice:
            return createDoubleSpinBox(parent, index); break;
        case SStoreItemModel::SaleOpColumns::ColWarranty:
            return createComboBox(parent, warrantyTermsModel); break;
        case SStoreItemModel::SaleOpColumns::ColUser:
            return createComboBox(parent, usersModel); break;
        default:
            return STableViewBaseItemDelegates::createEditor(parent, option, index);
    }
}

void STableViewBOQItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
        case SStoreItemModel::SaleOpColumns::ColCount:
            setSpinBoxData(editor, index.data().toInt()); return;
        case SStoreItemModel::SaleOpColumns::ColPrice:
            setDoubleSpinBoxData(editor, sysLocale.toDouble(index.data().toString())); return;
        case SStoreItemModel::SaleOpColumns::ColWarranty:
        case SStoreItemModel::SaleOpColumns::ColUser:
            setComboBoxData(editor, index.data().toString()); return;
        default:
            STableViewBaseItemDelegates::setEditorData(editor, index);
    }
}

void STableViewBOQItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
        case SStoreItemModel::SaleOpColumns::ColCount:
            setModelDataFromSpinBox(editor, model, index); return;
        case SStoreItemModel::SaleOpColumns::ColPrice:
            setModelDataFromDoubleSpinBox(editor, model, index); return;
        case SStoreItemModel::SaleOpColumns::ColWarranty:
        case SStoreItemModel::SaleOpColumns::ColUser:
            setModelDataFromComboBox(editor, model, index); return;
        default:
            STableViewBaseItemDelegates::setModelData(editor, model, index);
    }
}

void STableViewBOQItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // кнопка в ячеейке tableView; взято: https://stackoverflow.com/a/11778012
    if(index.column() == SStoreItemModel::SaleOpColumns::ColId )
    {
        switch (rowConditionsForPixmap(index))
        {
            case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreNew << 8                                                       | SSaleTableModel::RecordType::Item):
            case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreReserved << 8                                                  | SSaleTableModel::RecordType::Item):
            case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreSold << 8                                                      | SSaleTableModel::RecordType::Item):
            case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopAdm << 8                                                    | SSaleTableModel::RecordType::Item):
            case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopRW << 8                                                     | SSaleTableModel::RecordType::Item): drawPixmap(option.rect, RemovePart, painter); drawPixmap(option.rect, Part, painter); break;
            case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopAdm << 8                                                    | SSaleTableModel::RecordType::Work):
            case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopRW << 8                                                     | SSaleTableModel::RecordType::Work): drawPixmap(option.rect, RemoveWork, painter); drawPixmap(option.rect, Work, painter); drawPixmap(option.rect, AddPart, painter); break;
            case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreSold << 8     | SStoreSaleItemModel::Cancelled << 1            | SSaleTableModel::RecordType::Item):
            case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreCancelled << 8| SStoreSaleItemModel::Cancelled << 1            | SSaleTableModel::RecordType::Item):
            case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopRO << 8                                                     | SSaleTableModel::RecordType::Item): drawPixmap(option.rect, Part, painter); break;
            case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopRO << 8                                                     | SSaleTableModel::RecordType::Work): drawPixmap(option.rect, Work, painter); break;
        }
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QRect STableViewBOQItemDelegates::pixmapRect(const QRect &delegateRect, const PixmapType p) const
{
    int x,y,w,h;
    switch (p)
    {
        case RemoveWork: x = delegateRect.left() + PIXMAP_GAP; break;
        case RemovePart:
        case Work: x = delegateRect.left() + PIXMAP_GAP + (PIXMAP_W + PIXMAP_GAP); break;
        case AddPart:
        case Part: x = delegateRect.left() + PIXMAP_GAP + (PIXMAP_W + PIXMAP_GAP)*2; break;
    }
    y = delegateRect.top()+(delegateRect.height()-PIXMAP_W)/2;
    w = PIXMAP_W;
    h = PIXMAP_W;

    return QRect(x,y,w,h);
}

void STableViewBOQItemDelegates::drawPixmap(const QRect &rect, PixmapType p, QPainter *painter) const
{
    QPixmap pixmap;
    switch (p)
    {
        case RemoveWork:
        case RemovePart: pixmap = QPixmap(":/icons/light/1F5D1_32.png"); break;
        case Work: pixmap = QPixmap(":/icons/light/1F6E0_32.png"); break;
        case AddPart: pixmap = QPixmap(":/icons/light/1F4E6_32.png"); break;
        case Part: pixmap = QPixmap(":/icons/light/1F6D2_32.png"); break;
    }
//      🗑🛠🛒🗙📦
    QApplication::style()->drawItemPixmap(painter, pixmapRect(rect, p), 1, pixmap.scaled(PIXMAP_W,PIXMAP_H));
}

int STableViewBOQItemDelegates::rowConditionsForPixmap(const QModelIndex &index) const
{
    return m_tableModel->mode() << 16 |\
           m_tableModel->state() << 8 |\
           index.data(SSaleTableModel::DataRoles::State).toBool() << 1 |\
                               index.data(SSaleTableModel::DataRoles::RecordType).toBool();
}

void STableViewBOQItemDelegates::buttonHandler(const int buttonNum, const int row)
{
    // действия при прямой продаже и в карте ремонта разные; формируем уникальный идентификатор
    int action = m_tableModel->mode() << 8 | m_tableModel->recordType(row) << 4 | buttonNum;
    switch(action)
    {
        case (SSaleTableModel::StoreSale << 8    | SSaleTableModel::RecordType::Item << 4 | 1):
        case (SSaleTableModel::WorkshopSale << 8 | SSaleTableModel::RecordType::Work << 4 | 0):
        case (SSaleTableModel::WorkshopSale << 8 | SSaleTableModel::RecordType::Item << 4 | 1): m_tableModel->removeRow(row); break; // кнопка "удалить"
        case (SSaleTableModel::WorkshopSale << 8 | SSaleTableModel::RecordType::Work << 4 | 2): emit addItem(); break; // кнопка "добавить деталь"
        case (SSaleTableModel::StoreSale << 8    | 2): ; break; // нет действия
    }
}

bool STableViewBOQItemDelegates::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent * e = (QMouseEvent *)event;
        if( index.column() == SStoreItemModel::SaleOpColumns::ColId )
        {
            switch (rowConditionsForPixmap(index)) // условия, при которых обработка не требуется
            {
                case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreCancelled << 8 | SStoreSaleItemModel::State::Cancelled << 1     | SSaleTableModel::RecordType::Item):
                case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreReserved << 8  | SStoreSaleItemModel::State::Cancelled << 1     | SSaleTableModel::RecordType::Item):
                case (SSaleTableModel::TablesSet::StoreSale << 16    | SSaleTableModel::StoreSold << 8      | SStoreSaleItemModel::State::Cancelled << 1     | SSaleTableModel::RecordType::Item):
                case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopRO << 8                                                      | SSaleTableModel::RecordType::Item):
                case (SSaleTableModel::TablesSet::WorkshopSale << 16 | SSaleTableModel::WorkshopRO << 8                                                      | SSaleTableModel::RecordType::Work): return true;
            }

            // кнопка в ячеейке tableView; взято: https://stackoverflow.com/a/11778012
#if QT_VERSION >= 0x060000
            int clickX = e->position().x();
#else
            int clickX = e->localPos().x();
#endif
            clickX -= option.rect.x();  // коррекция координаты X курсора для случая когда пиктограммы уходят за левый край виджета

            //  0                     10                  20                  30                  40                  50
            //  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9
            // |gap|        p i x m a p            |gap|        p i x m a p            |gap|        p i x m a p            |gap|
            int button = 1; // определение номера нажатой кнопки начинается со средней
            int button_x = PIXMAP_GAP+button*(PIXMAP_W + PIXMAP_GAP); // step 0: 2+1*(16+2)=20; clickX = 18
            while(button_x >= 0 && button_x <= option.rect.width())
            {
                if(clickX >= (button_x - PIXMAP_GAP/2) && clickX < (button_x + PIXMAP_W + PIXMAP_GAP/2))
                    break;

                if(clickX - button_x >= 0)
                    button++;
                else
                    button--;
                button_x = PIXMAP_GAP+button*(PIXMAP_W + PIXMAP_GAP);
            }

            buttonHandler(button, index.row());
            return true; // true indicates that the event was handled (see doc for QAbstractItemDelegate::editorEvent)
        }
    }
    return STableViewBaseItemDelegates::editorEvent(event, model, option, index);
}

QSpinBox *STableViewBOQItemDelegates::createSpinBox(QWidget *parent, const QModelIndex &index) const
{
    QSpinBox *sb = STableViewBaseItemDelegates::createSpinBox(parent, index);
    sb->setMinimum(1);
    sb->setMaximum(m_tableModel->index(index.row(), SStoreItemModel::SaleOpColumns::ColAvail).data().toInt());
    return sb;
}

QDoubleSpinBox *STableViewBOQItemDelegates::createDoubleSpinBox(QWidget *parent, const QModelIndex &index) const
{
    QDoubleSpinBox *sb = STableViewBaseItemDelegates::createDoubleSpinBox(parent, index);

    // в гарантийном ремонте минимальная цена может быть равна нулю
    if(m_tableModel->isWarranty() || !m_tableModel->index(index.row(), 0).data(SSaleTableModel::DataRoles::RecordType).toBool())
        sb->setMinimum(0);

    return sb;
}

void STableViewBOQItemDelegates::setTableModel(QAbstractItemModel *model)
{
    m_tableModel = static_cast<SSaleTableModel*>(model);
}
