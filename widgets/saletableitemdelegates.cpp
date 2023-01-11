#include "saletableitemdelegates.h"

SaleTableItemDelegates::SaleTableItemDelegates(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

SaleTableItemDelegates::SaleTableItemDelegates(SSaleTableModel *model, QObject *parent) :
    SaleTableItemDelegates(parent)
{
    setTableModel(model);
    // пример itemDelegate взят по https://wiki.qt.io/Combo_Boxes_in_Item_Views
}


SaleTableItemDelegates::~SaleTableItemDelegates()
{
}


QWidget *SaleTableItemDelegates::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
            return QStyledItemDelegate::createEditor(parent, option, index);
    }
}


void SaleTableItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
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
            setLineEditData(editor, index.data().toString());
    }
}


void SaleTableItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
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
            QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void SaleTableItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

QRect SaleTableItemDelegates::pixmapRect(const QRect &delegateRect, const PixmapType p) const
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

void SaleTableItemDelegates::drawPixmap(const QRect &rect, PixmapType p, QPainter *painter) const
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

int SaleTableItemDelegates::rowConditionsForPixmap(const QModelIndex &index) const
{
    return m_tableModelMode << 16 |\
           m_tableModelState << 8 |\
           index.data(SSaleTableModel::DataRoles::State).toBool() << 1 |\
           index.data(SSaleTableModel::DataRoles::RecordType).toBool();
}

void SaleTableItemDelegates::modelStateChanged(const int state)
{
    m_tableModelState = state;
}

bool SaleTableItemDelegates::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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

            tableModel->buttonHandler(button, index.row());
        }
        else
        {
            if(e->button() == Qt::LeftButton)
            {   // редактирование по одинарному щелчку; взято: https://stackoverflow.com/a/53304106
                if( model->flags(index) & Qt::ItemIsEditable )
                {
                    QAbstractItemView* pView = qobject_cast<QAbstractItemView*>( const_cast<QWidget*>(option.widget) );
                    if(pView != nullptr)
                    {
                        pView->setCurrentIndex(index);
                        m_must_open_box = true;
                        pView->edit(index);
                    }
                    return true;
                }
                return false;
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

bool SaleTableItemDelegates::event(QEvent *event)
{
    return QStyledItemDelegate::event(event);
}

bool SaleTableItemDelegates::eventFilter(QObject *obj, QEvent *event)
{
    // если виджет-редактор — lineEdit, и он уже содержит данные, задаваемые в setEditorData(), то текст будет
    // выделен; чтобы по одинарному клику активировалось редактирование, а курсор устанавливался в
    // место, где нажал пользователь, вызываем метод установки курсора по координатам
    if(event->type() == QEvent::Enter && QString(obj->metaObject()->className()).compare("QExpandingLineEdit", Qt::CaseSensitive) == 0 )
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(obj);
#if QT_VERSION >= 0x060000
        lineEdit->setCursorPosition(lineEdit->cursorPositionAt(static_cast<QEnterEvent*>(event)->position().toPoint()));
#else
        lineEdit->setCursorPosition(lineEdit->cursorPositionAt(static_cast<QEnterEvent*>(event)->localPos().toPoint()));
#endif
    }

    return QStyledItemDelegate::eventFilter(obj, event);
}

QLineEdit *SaleTableItemDelegates::createLineEdit(QWidget *parent, QAbstractItemModel *index) const
{
//    MLineEdit *lineEdit = new MLineEdit(parent);
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);
    return lineEdit;
}

void SaleTableItemDelegates::setLineEditData(QWidget *editor, const QString &text) const
{
//    MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(lineEdit);
    lineEdit->setFrame(false);
    lineEdit->setText(text);
}

void SaleTableItemDelegates::setModelDataFromLineEdit(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
//    MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(lineEdit);
    model->setData(index, lineEdit->text(), Qt::EditRole);
}

// Create the comboBox editor
QComboBox *SaleTableItemDelegates::createComboBox(QWidget *parent, QAbstractItemModel *model) const
{
        QComboBox *cb = new QComboBox(parent);
        cb->setEditable(false);
        cb->setModel(model);
        cb->setCurrentIndex(-1);
        return cb;
}

void SaleTableItemDelegates::setComboBoxData(QWidget *editor, const QString &text) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    cb->setCurrentText(text);
    if(m_must_open_box) // авто раскрытие списка комбобокса при; взято: https://stackoverflow.com/a/53304106
    {
        m_must_open_box = false;
        cb->showPopup();
    }
}

void SaleTableItemDelegates::setModelDataFromComboBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    if(cb->currentIndex() >= 0)
    {
        model->setData(index, cb->model()->index(cb->currentIndex(), 1).data().toInt(), Qt::EditRole);
    }
}

// Create the spinbox and populate it
QSpinBox *SaleTableItemDelegates::createSpinBox(QWidget *parent, const QModelIndex &index) const
{
    QSpinBox *sb = new QSpinBox(parent);
    sb->setMinimum(1);
    sb->setMaximum(tableModel->index(index.row(), SStoreItemModel::SaleOpColumns::ColAvail).data().toInt());
    return sb;
}

void SaleTableItemDelegates::setSpinBoxData(QWidget *editor, const int value) const
{
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    Q_ASSERT(sb);
    sb->setValue(value);
}

void SaleTableItemDelegates::setModelDataFromSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    Q_ASSERT(sb);
    model->setData(index, sb->value(), Qt::EditRole);
}

// Create the spinbox and populate it
QDoubleSpinBox *SaleTableItemDelegates::createDoubleSpinBox(QWidget *parent, const QModelIndex &index) const
{
    QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
    if(comSettings->value("classic_kassa").toBool())
    {
        sb->setDecimals(2);
        sb->setMinimum(0.01);   // TODO: в гарантийном ремонте минимальная цена должна быть равна нулю
        sb->setMaximum(999999.99);
    }
    else
    {
        sb->setDecimals(0);
        sb->setMinimum(1);
        sb->setMaximum(999999);
    }
    if(tableModel->isWarranty() || !tableModel->index(index.row(), 0).data(SSaleTableModel::DataRoles::RecordType).toBool())
        sb->setMinimum(0);
    return sb;
}

void SaleTableItemDelegates::setDoubleSpinBoxData(QWidget *editor, const double value) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
    Q_ASSERT(sb);
    sb->setValue(value);
}

void SaleTableItemDelegates::setModelDataFromDoubleSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
    Q_ASSERT(sb);
    model->setData(index, sb->value(), Qt::EditRole);
}

void SaleTableItemDelegates::setTableModel(SSaleTableModel *model)
{
    tableModel = model;
    connect(tableModel, &SSaleTableModel::modelStateChanged, this, &SaleTableItemDelegates::modelStateChanged);
    m_tableModelMode = tableModel->tableMode();
    m_tableModelState = tableModel->modelState();
}
