#include "stableviewbaseitemdelegates.h"
#include <QProxyStyle>

STableViewBaseItemDelegates::STableViewBaseItemDelegates(QObject *parent) :
    QStyledItemDelegate(parent)
{
    installEventFilter(this);
    initProgressBarsStyles();
}

// пример itemDelegate взят по https://wiki.qt.io/Combo_Boxes_in_Item_Views
STableViewBaseItemDelegates::STableViewBaseItemDelegates(STableBaseModel *model, QObject *parent) :
    STableViewBaseItemDelegates(parent)
{
    setTableModel(model);
}


STableViewBaseItemDelegates::~STableViewBaseItemDelegates()
{
    if(i_fontMetrics)
        delete i_fontMetrics;
}


QWidget *STableViewBaseItemDelegates::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
//        case SStoreItemModel::SaleOpColumns::ColCount:
//            return createSpinBox(parent, index); break;
//        case SStoreItemModel::SaleOpColumns::ColPrice:
//            return createDoubleSpinBox(parent, index); break;
//        case SStoreItemModel::SaleOpColumns::ColWarranty:
//            return createComboBox(parent, warrantyTermsModel); break;
//        case SStoreItemModel::SaleOpColumns::ColUser:
//            return createComboBox(parent, usersModel); break;
//        default:
//            return QStyledItemDelegate::createEditor(parent, option, index);
    }
}


void STableViewBaseItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
//        case SStoreItemModel::SaleOpColumns::ColCount:
//            setSpinBoxData(editor, index.data().toInt()); return;
//        case SStoreItemModel::SaleOpColumns::ColPrice:
//            setDoubleSpinBoxData(editor, sysLocale.toDouble(index.data().toString())); return;
//        case SStoreItemModel::SaleOpColumns::ColWarranty:
//        case SStoreItemModel::SaleOpColumns::ColUser:
//            setComboBoxData(editor, index.data().toString()); return;
//        default:
//            setLineEditData(editor, index.data().toString());
    }
}


void STableViewBaseItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
//        case SStoreItemModel::SaleOpColumns::ColCount:
//            setModelDataFromSpinBox(editor, model, index); return;
//        case SStoreItemModel::SaleOpColumns::ColPrice:
//            setModelDataFromDoubleSpinBox(editor, model, index); return;
//        case SStoreItemModel::SaleOpColumns::ColWarranty:
//        case SStoreItemModel::SaleOpColumns::ColUser:
//            setModelDataFromComboBox(editor, model, index); return;
//        default:
//            QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void STableViewBaseItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

bool STableViewBaseItemDelegates::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent * e = (QMouseEvent *)event;

        if(0)   // щелчек по ячейке X, в которой могут располагаться кнопки
        {
            // do something

            // кнопка в ячеейке tableView; взято: https://stackoverflow.com/a/11778012
#if QT_VERSION >= 0x060000
            int clickX = e->position().x();
#else
            int clickX = e->localPos().x();
#endif
            clickX -= option.rect.x();  // коррекция координаты X курсора для случая когда пиктограммы уходят зя левый край виджета

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
                        m_autoDropDownList = true;
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

bool STableViewBaseItemDelegates::event(QEvent *event)
{
    return QStyledItemDelegate::event(event);
}

bool STableViewBaseItemDelegates::eventFilter(QObject *obj, QEvent *event)
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

QLineEdit *STableViewBaseItemDelegates::createLineEdit(QWidget *parent, QAbstractItemModel *index) const
{
//    MLineEdit *lineEdit = new MLineEdit(parent);
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setFrame(false);
    return lineEdit;
}

void STableViewBaseItemDelegates::setLineEditData(QWidget *editor, const QString &text) const
{
//    MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(lineEdit);
    lineEdit->setFrame(false);
    lineEdit->setText(text);
}

void STableViewBaseItemDelegates::setModelDataFromLineEdit(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
//    MLineEdit *lineEdit = qobject_cast<MLineEdit *>(editor);
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    Q_ASSERT(lineEdit);
    model->setData(index, lineEdit->text(), Qt::EditRole);
}

// Create the comboBox editor
QComboBox *STableViewBaseItemDelegates::createComboBox(QWidget *parent, QAbstractItemModel *model) const
{
        QComboBox *cb = new QComboBox(parent);
        cb->setEditable(false);
        cb->setModel(model);
        cb->setCurrentIndex(-1);
        return cb;
}

void STableViewBaseItemDelegates::setComboBoxData(QWidget *editor, const QString &text) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    cb->setCurrentText(text);
    if(m_autoDropDownList) // авто раскрытие списка комбобокса; взято: https://stackoverflow.com/a/53304106
    {
        m_autoDropDownList = false;
        cb->showPopup();
    }
}

void STableViewBaseItemDelegates::setModelDataFromComboBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    if(cb->currentIndex() >= 0)
    {
        model->setData(index, cb->model()->index(cb->currentIndex(), 1).data().toInt(), Qt::EditRole);
    }
}

// Create the spinbox and populate it
QSpinBox *STableViewBaseItemDelegates::createSpinBox(QWidget *parent, const QModelIndex &index) const
{
    QSpinBox *sb = new QSpinBox(parent);
    return sb;
}

void STableViewBaseItemDelegates::setSpinBoxData(QWidget *editor, const int value) const
{
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    Q_ASSERT(sb);
    sb->setValue(value);
}

void STableViewBaseItemDelegates::setModelDataFromSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    Q_ASSERT(sb);
    model->setData(index, sb->value(), Qt::EditRole);
}

// Create the spinbox and populate it
QDoubleSpinBox *STableViewBaseItemDelegates::createDoubleSpinBox(QWidget *parent, const QModelIndex &index) const
{
    QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
    return sb;
}

void STableViewBaseItemDelegates::setDoubleSpinBoxData(QWidget *editor, const double value) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
    Q_ASSERT(sb);
    sb->setValue(value);
}

void STableViewBaseItemDelegates::setModelDataFromDoubleSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
    Q_ASSERT(sb);
    model->setData(index, sb->value(), Qt::EditRole);
}

void STableViewBaseItemDelegates::setTableModel(STableBaseModel *model)
{
    i_tableModel = model;
}

void STableViewBaseItemDelegates::setFontMetrics(const QFont &font)
{
    i_fontMetrics = new QFontMetrics(font);
}


// Как раскрасить progressBar подсмотрел в проекте qBittorent
void STableViewBaseItemDelegates::initProgressBarsStyles()
{
    i_dummyProgressBarStyleSheet = QString("                                    \
                                           QProgressBar::chunk {                \
                                               border-top: 1px solid white;     \
                                               border-bottom: 1px solid white;  \
                                               background-color: %1;            \
                                               width: 1px;                      \
                                           }                                    \
                                                                                \
                                           QProgressBar {                       \
                                               border: 1px solid grey;          \
                                               border-radius: 0px;              \
                                               text-align: center;              \
                                           }                                    \
                                          ");
    i_dummyGreenProgressBar.setStyleSheet(QString(i_dummyProgressBarStyleSheet).replace("%1", PROGRESSBAR_GREEN));
    i_dummyYellowProgressBar.setStyleSheet(QString(i_dummyProgressBarStyleSheet).replace("%1", PROGRESSBAR_YELLOW));
    i_dummyOrangeProgressBar.setStyleSheet(QString(i_dummyProgressBarStyleSheet).replace("%1", PROGRESSBAR_ORANGE));
    i_dummyRedProgressBar.setStyleSheet(QString(i_dummyProgressBarStyleSheet).replace("%1", PROGRESSBAR_RED));
}

void STableViewBaseItemDelegates::paintColorizedProgressBar(const QStyleOptionProgressBar *option, QPainter *painter, const QWidget *widget) const
{
    painter->save();
//    i_style->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);

    // TODO: ..., а пока так...
    if(option->progress <= 25)
        i_dummyGreenProgressBar.style()->drawControl(QStyle::CE_ProgressBar, option, painter, &i_dummyGreenProgressBar);
    else if(option->progress <= 50)
        i_dummyYellowProgressBar.style()->drawControl(QStyle::CE_ProgressBar, option, painter, &i_dummyYellowProgressBar);
    else if(option->progress <= 75)
        i_dummyOrangeProgressBar.style()->drawControl(QStyle::CE_ProgressBar, option, painter, &i_dummyOrangeProgressBar);
    else
        i_dummyRedProgressBar.style()->drawControl(QStyle::CE_ProgressBar, option, painter, &i_dummyRedProgressBar);

    painter->restore();
}
