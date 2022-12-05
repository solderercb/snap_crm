#include "saletableitemdelegates.h"

SaleTableItemDelegates::SaleTableItemDelegates(SSaleTableModel *model, QObject *parent) :
    QStyledItemDelegate(parent),
    tableModel(model)   // tableModel нужен для установки spinBox'у максимального значения, равного доступному кол-ву товаров
{
    // пример itemDelegate взят по https://wiki.qt.io/Combo_Boxes_in_Item_Views
}


SaleTableItemDelegates::~SaleTableItemDelegates()
{
}


QWidget *SaleTableItemDelegates::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( tableModel->index(index.row(), SStoreSaleItemModel::ColRecordType).data().toBool() == SSaleTableModel::Item )
    {}
    switch (index.column())
    {
        case SStoreSaleItemModel::ColCount:
            return createSpinBox(parent, index);
        case SStoreSaleItemModel::ColPrice:
            return createDoubleSpinBox(parent);
        case SStoreSaleItemModel::ColWarranty:
            return createComboBox(parent, warrantyTermsModel);
        case SStoreSaleItemModel::ColUser:
            return createComboBox(parent, allUsersModel);
        default:
            return QStyledItemDelegate::createEditor(parent, option, index);
    }
}


void SaleTableItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
        case SStoreSaleItemModel::ColCount:
            setSpinBoxData(editor, index.data().toInt()); return;
        case SStoreSaleItemModel::ColPrice:
            setDoubleSpinBoxData(editor, sysLocale.toFloat(index.data().toString())); return;
        case SStoreSaleItemModel::ColWarranty:
        case SStoreSaleItemModel::ColUser:
            setComboBoxData(editor, index.data().toString()); return;
        default:
            setLineEditData(editor, index.data().toString());
    }
}


void SaleTableItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
        case SStoreSaleItemModel::ColCount:
            setModelDataFromSpinBox(editor, model, index); return;
        case SStoreSaleItemModel::ColPrice:
            setModelDataFromDoubleSpinBox(editor, model, index); return;
        case SStoreSaleItemModel::ColWarranty:
        case SStoreSaleItemModel::ColUser:
            setModelDataFromComboBox(editor, model, index); return;
        default:
            QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void SaleTableItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == SStoreSaleItemModel::ColId )
    {
        if(tableModel->modelState() != SSaleTableModel::StoreCancelled)
        {
            if(!index.data(SSaleTableModel::DataRoles::State).toBool())
            {
                // кнопка в ячеейке tableView; взято: https://stackoverflow.com/a/11778012
                QStyleOptionButton button;
                QRect r = option.rect;//getting the rect of the cell
                int x,y,w,h;
                x = r.left();//the X coordinate
                y = r.top();//the Y coordinate
                w = r.width();//button width
                h = r.height();//button height
                button.rect = QRect(x,y,w,h);
                button.icon = QIcon(":/icons/light/1F5D1_32.png");
                button.iconSize = QSize(16,16);
//                button.text = "🗑"; // 🛠🛒🗙📦
                button.state = QStyle::State_Enabled;

                QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
            }
        }
        // Если   modelState() == 3, то ничего не отображаем
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

bool SaleTableItemDelegates::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent * e = (QMouseEvent *)event;
        if( index.column() == SStoreSaleItemModel::ColId )
        {
            if(tableModel->modelState() != SSaleTableModel::StoreCancelled)
            {
                if(!index.data(SSaleTableModel::DataRoles::State).toBool())
                {
                    // кнопка в ячеейке tableView; взято: https://stackoverflow.com/a/11778012
#if QT_VERSION >= 0x060000
                    int clickX = e->position().x();
                    int clickY = e->position().y();
#else
                    int clickX = e->localPos().x();
                    int clickY = e->localPos().y();
#endif

                    QRect r = option.rect;//getting the rect of the cell
                    int x,y,w,h;
                    x = r.left();//the X coordinate
                    y = r.top();//the Y coordinate
                    w = r.width();//button width
                    h = r.height();//button height

                    if( clickX > x && clickX < x + w )
                        if( clickY > y && clickY < y + h )
                        {
                            tableModel->removeRowHandler(index.row(), index.data().toInt());
                        }
                }
            }
            // Если   modelState() == 3, то никакой реакции
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
    return true;
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
    sb->setMaximum(tableModel->index(index.row(), SStoreSaleItemModel::ColAvail).data().toInt());
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
QDoubleSpinBox *SaleTableItemDelegates::createDoubleSpinBox(QWidget *parent) const
{
    QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
    sb->setDecimals(2);
    sb->setMinimum(0.01);   // TODO: в гарантийном ремонте минимальная цена должна быть равна нулю
    sb->setMaximum(9999999999.99);
    return sb;
}

void SaleTableItemDelegates::setDoubleSpinBoxData(QWidget *editor, const float value) const
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
