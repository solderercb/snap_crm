#include "scartridgematerialstableitemdelegates.h"
#include "scartridgecard.h"

SCartridgeMaterialsTableItemDelegates::SCartridgeMaterialsTableItemDelegates(QObject *parent) : STableViewBaseItemDelegates(parent)
{

}

SCartridgeMaterialsTableItemDelegates::SCartridgeMaterialsTableItemDelegates(SCartridgeMaterialsModel*, QObject*)
{

}

SCartridgeMaterialsTableItemDelegates::~SCartridgeMaterialsTableItemDelegates()
{

}

QWidget *SCartridgeMaterialsTableItemDelegates::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *w;
    switch (index.column())
    {
        case materialsTable::Column::Type:
            w = createComboBox(parent, cartridgeMaterialsListModel); break;
        case materialsTable::Column::Count:
            w = createSpinBox(parent, index); break;
        case materialsTable::Column::Price:
        case materialsTable::Column::PriceWork:
            w = createDoubleSpinBox(parent, index); break;
        case materialsTable::Column::Articul:
            w = createSpinBox(parent, index); static_cast<QSpinBox*>(w)->setButtonSymbols(QSpinBox::NoButtons); break;
        default:
            w = QStyledItemDelegate::createEditor(parent, option, index);
    }
    return w;
}

void SCartridgeMaterialsTableItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
        case materialsTable::Column::Type: setComboBoxData(editor, index.data().toString()); return;
        case materialsTable::Column::Count:
        case materialsTable::Column::Articul: setSpinBoxData(editor, index.data().toInt()); return;
        case materialsTable::Column::Price:
        case materialsTable::Column::PriceWork: setDoubleSpinBoxData(editor, sysLocale.toDouble(index.data().toString())); return;
        case materialsTable::Column::Name: setLineEditData(editor, index.data().toString()); return;
        default: ;
    }
}

void SCartridgeMaterialsTableItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
        case materialsTable::Column::Type:
            setModelDataFromComboBox(editor, model, index); return;
        case materialsTable::Column::Count:
        case materialsTable::Column::Articul:
            setModelDataFromSpinBox(editor, model, index); return;
        case materialsTable::Column::Price:
        case materialsTable::Column::PriceWork:
            setModelDataFromDoubleSpinBox(editor, model, index); return;
        case materialsTable::Column::Name:
            setModelDataFromLineEdit(editor, model, index); return;
        default:
            QStyledItemDelegate::setModelData(editor, model, index);
    }
}

bool SCartridgeMaterialsTableItemDelegates::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent *e = (QMouseEvent *)event;
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
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

bool SCartridgeMaterialsTableItemDelegates::eventFilter(QObject *obj, QEvent *event)
{
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

void SCartridgeMaterialsTableItemDelegates::setTableModel(SCartridgeMaterialsModel *model)
{
    tableModel = model;
}

/*****************************************************************************************************************
 * ЭТИ МЕТОДЫ СКОПИРОВАНЫ ИЗ КЛАССА SaleTableItemDelegates
 * TODO: вынести их в отдельный общий класс
 */
QLineEdit * SCartridgeMaterialsTableItemDelegates::createLineEdit(QWidget *parent) const
{
//    MLineEdit *lineEdit = new MLineEdit(parent);
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);
    return lineEdit;
}

void  SCartridgeMaterialsTableItemDelegates::setLineEditData(QWidget *editor, const QString &text) const
{
//    MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(lineEdit);
    lineEdit->setFrame(false);
    lineEdit->setText(text);
}

void  SCartridgeMaterialsTableItemDelegates::setModelDataFromLineEdit(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
//    MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(lineEdit);
    model->setData(index, lineEdit->text(), Qt::EditRole);
}

// Create the comboBox editor
QComboBox * SCartridgeMaterialsTableItemDelegates::createComboBox(QWidget *parent, QAbstractItemModel *model) const
{
        QComboBox *cb = new QComboBox(parent);
        cb->setEditable(false);
        cb->setModel(model);
        cb->setCurrentIndex(-1);
        return cb;
}

void  SCartridgeMaterialsTableItemDelegates::setComboBoxData(QWidget *editor, const QString &text) const
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

void  SCartridgeMaterialsTableItemDelegates::setModelDataFromComboBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    if(cb->currentIndex() >= 0)
    {
        model->setData(index, cb->model()->index(cb->currentIndex(), 1).data().toInt(), Qt::EditRole);
    }
}

// Create the spinbox and populate it
QSpinBox * SCartridgeMaterialsTableItemDelegates::createSpinBox(QWidget *parent, const QModelIndex&) const
{
    QSpinBox *sb = new QSpinBox(parent);
    sb->setMinimum(1);
    sb->setMaximum(999999);
    return sb;
}

void  SCartridgeMaterialsTableItemDelegates::setSpinBoxData(QWidget *editor, const int value) const
{
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    Q_ASSERT(sb);
    sb->setValue(value);
}

void  SCartridgeMaterialsTableItemDelegates::setModelDataFromSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    Q_ASSERT(sb);
    model->setData(index, sb->value(), Qt::EditRole);
}

// Create the spinbox and populate it
QDoubleSpinBox * SCartridgeMaterialsTableItemDelegates::createDoubleSpinBox(QWidget *parent, const QModelIndex&) const
{
    QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
    if(comSettings->classicKassa)
    {
        sb->setDecimals(2);
        sb->setMinimum(0.00);
        sb->setMaximum(999999.99);
    }
    else
    {
        sb->setDecimals(0);
        sb->setMinimum(0);
        sb->setMaximum(999999);
    }

    return sb;
}

void  SCartridgeMaterialsTableItemDelegates::setDoubleSpinBoxData(QWidget *editor, const double value) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
    Q_ASSERT(sb);
    sb->setValue(value);
}

void  SCartridgeMaterialsTableItemDelegates::setModelDataFromDoubleSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
    Q_ASSERT(sb);
    model->setData(index, sb->value(), Qt::EditRole);
}
