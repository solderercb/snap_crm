#include "saletableitemdelegates.h"

SaleTableItemDelegates::SaleTableItemDelegates(SaleTableModel *model, QObject *parent) :
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
//    qDebug() << "SaleTableItemDelegates::createEditor() | parent: " << parent;
    if( index.column() == tableModel->fields->value("count") )
    {
        // Create the spinbox and populate it
        QSpinBox *sb = new QSpinBox(parent);
        sb->setMinimum(1);
        sb->setMaximum(tableModel->index(index.row(), tableModel->fields->value("avail")).data().toInt());
        return sb;
    }
    else if( index.column() == tableModel->fields->value("price") )
    {
        // Create the spinbox and populate it
        QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
        sb->setDecimals(2);
        sb->setMinimum(0.01);
        sb->setMaximum(9999999999.99);
        return sb;
    }
    else if( index.column() == tableModel->fields->value("warranty") )
    {
    // Create the combobox and populate it
        QComboBox *cb = new QComboBox(parent);
        cb->setEditable(false);
        cb->setModel(warrantyTermsModel);
        cb->setCurrentIndex(-1);
        return cb;
    }
    else
    {
//            MLineEdit *lineEdit = new MLineEdit(parent);
//            lineEdit->setFrame(false);
//            return lineEdit;
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}


void SaleTableItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
{
//    qDebug() << "SaleTableItemDelegates::setEditorData()";
    if( index.column() == tableModel->fields->value("count") )
    {
        QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
        Q_ASSERT(sb);
        sb->setValue(index.data().toInt());
    }
    else if( index.column() == tableModel->fields->value("price") )
    {
        QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
        Q_ASSERT(sb);
        sb->setValue(sysLocale.toFloat(index.data().toString()));
    }
    else if( index.column() == tableModel->fields->value("warranty") )
    {
    // Create the combobox and populate it
        QComboBox *cb = qobject_cast<QComboBox *>(editor);
        Q_ASSERT(cb);
        cb->setCurrentText(index.data().toString());
        if(m_must_open_box) // авто раскрытие списка комбобокса при; взято: https://stackoverflow.com/a/53304106
        {
            m_must_open_box = false;
            cb->showPopup();
        }
    }
    else
    {
//            MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        Q_ASSERT(lineEdit);
        lineEdit->setFrame(false);
        const QString text = index.data().toString();
        lineEdit->setText(text);
    }
}


void SaleTableItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
//    qDebug() << "SaleTableItemDelegates::setModelData()";
    if( index.column() == tableModel->fields->value("count") )
    {
        QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
        Q_ASSERT(sb);
        model->setData(index, sb->value(), Qt::EditRole);
    }
    else if( index.column() == tableModel->fields->value("price") )
    {
        QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
        Q_ASSERT(sb);
        model->setData(index, sb->value(), Qt::EditRole);
    }
    else if( index.column() == tableModel->fields->value("warranty") )
    {
        QComboBox *cb = qobject_cast<QComboBox *>(editor);
        Q_ASSERT(cb);
        if(cb->currentIndex() >= 0)
        {
            model->setData(index, warrantyTermsModel->index(cb->currentIndex(), 1).data().toInt(), Qt::EditRole);
        }
    }
    else
    {
//            MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
//            Q_ASSERT(lineEdit);
//            model->setData(index, lineEdit->text(), Qt::EditRole);
        QStyledItemDelegate::setModelData(editor, model, index);
    }
//    qDebug() << "new value:" << index.data().toString();
}

void SaleTableItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    qDebug() << "SaleTableItemDelegates::paint()";
    if(index.column() == tableModel->fields->value("id") )
    {
        if(tableModel->getModelState() < 3)
        {
            if(!index.data(Qt::UserRole).toBool()) // В UserRole хранится значение is_cancellation (чтобы лишний раз не лезть в другие индексы объекта модели)
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
        // Если   getModelState() == 3, то ничего не отображаем
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

bool SaleTableItemDelegates::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
//    qDebug() << "SaleTableItemDelegates::editorEvent() | event: " << event;
    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent * e = (QMouseEvent *)event;
        if( index.column() == tableModel->fields->value("id") )
        {
            if(tableModel->getModelState() < 3)
            {
                if(!index.data(Qt::UserRole).toBool()) // В UserRole хранится значение is_cancellation (чтобы лишний раз не лезть в другие индексы объекта модели)
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
                            if(tableModel->getModelState() == 0)   // в режиме создания новой РН удаляем строки
                                model->removeRows(index.row(), 1);
                            else    // в режимах просмотра резерва или проведённой накладной, помечаем строки на возврат/отмену резерва
                                tableModel->removeRowHandler(index.row(), index.data().toInt());
                        emit tableModel->amountChanged(tableModel->totalAmount());
                    }
                }
            }
            // Если   getModelState() == 3, то никакой реакции
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
//    qDebug() << "SaleTableItemDelegates::event() | event: " << event;
    return QStyledItemDelegate::event(event);
}

bool SaleTableItemDelegates::eventFilter(QObject *obj, QEvent *event)
{
//    qDebug() << "SaleTableItemDelegates::eventFilter() | object: " << obj << "; event: " << event;

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
