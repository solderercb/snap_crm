#include "scombobox.h"

SComboBox::SComboBox(QWidget *parent) :
    QComboBox(parent)
{
    fontMetrics = new QFontMetrics(this->font());
    installEventFilter(this);
    szHint = QComboBox::sizeHint();
    minSzHint = QComboBox::minimumSizeHint();
}

SComboBox::~SComboBox()
{
    delete fontMetrics;
}

QSize SComboBox::sizeHint() const
{
    return szHint;
}

QSize SComboBox::minimumSizeHint() const
{
    return minSzHint;
}

void SComboBox::disableWheelEvent(bool state)
{
    wheelEventOn = !state;
}

void SComboBox::wheelEvent(QWheelEvent *event)
{
    if (wheelEventOn)
    {
        QComboBox::wheelEvent(event);
    }
}

bool SComboBox::eventFilter(QObject *watched, QEvent *e)
{
    if(watched == this)
        return eventFilterComboBox(e);
    else if(watched == lineEditWidget)
        return eventFilterLineEdit(e);
    else if(watched == listViewWidget)
        return eventFilterListView(e);
    else
        return false;
}

bool SComboBox::eventFilterComboBox(QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress )
    {
        // это событие происходит только когда список не отображается
        QMouseEvent *me = static_cast<QMouseEvent*>(e);
        // щелчек в области виджета
        if( isPointInArea( me->pos(), QRect(QPoint(0, 0), frameGeometry().size())) )
        {
            if(!isPopupVisible())
                showPopup();
            // нажатие на область кнопки: если список отображается, то событие не происходит, а список скрывается по потере фокуса
            if( me->x() > (this->frameGeometry().width() - this->iconSize().width()) ) // по стрелке
            {
                setIsPopupVisible(1);
            }   // нажатие по виджету (вне кнопки): событие происходит независимо от того, отображался ли выпадающий список или нет
            else  // между QLineEdit и стрелкой
            {
                toggleIsPopupVisible();
            }
            return true;
        }
    }

    if(e->type() == QEvent::MouseButtonDblClick)
    {
        lineEditWidget->selectAll();
        return true;
    }

    // Раскрытие списка при переключении на виджет по Tab/Shift+Tab
    if ( e->type() == QEvent::FocusIn )
    {
        Qt::FocusReason reason = static_cast<QFocusEvent*>(e)->reason();
        if (reason == Qt::TabFocusReason || reason == Qt::BacktabFocusReason )    // генерируем только по Tab/Shift+Tab, по клику мышкой будет другое событие
        {
            showPopup();
        }
    }

    if ( e->type() == QEvent::KeyPress )
    {
        int key = static_cast<QKeyEvent*>(e)->key();
        // если поле пустое, то нажатие Enter вызовет отображение раскрывающегося списка. Это для удобства работы с клавиатурой
        if ( (key == Qt::Key_Enter || key == Qt::Key_Return) && lineEdit()->text() == "" )
            showPopup();
    }

    // после переключения в другое окно список прячется
    if( e->type() == QEvent::WindowDeactivate )
    {
        setIsPopupVisible(0);
    }

    if( e->type() == QEvent::FocusOut )
    {
        if(ignoreFocusOut())
        {
            setIgnoreFocusOut(0); // однократный игнор только при показе списка
            return true;
        }
    }

    return false;
}

bool SComboBox::eventFilterLineEdit(QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)    // щелчек по LineEdit
    {
        if(!isPopupVisible())
        {
            showPopup();
        }
        toggleIsPopupVisible();
        lineEditWidget->setCursorPosition(lineEditWidget->cursorPositionAt(static_cast<QMouseEvent*>(e)->pos()));
        return true;
    }
    else if(e->type() == QEvent::MouseButtonDblClick)
    {
        lineEditWidget->selectAll();
        setIsPopupVisible(0);
        return true;
    }
    return false;
}

bool SComboBox::eventFilterListView(QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        return true;
    }

    if (e->type() == QEvent::ShortcutOverride)
    {
        int key = static_cast<QKeyEvent*>(e)->key();
        int modifiers = static_cast<QKeyEvent*>(e)->modifiers();

        // при выборе элемента списка клавиатурой, нужно игнорировать позицию курсора, которая учитывается в событии скрытия списка
        if( (key == Qt::Key_Return) || (key == Qt::Key_Enter) )
            setConsiderCursorPosOnHide(0);

        // прячем раскрывающийся список при (ИЛИ):
        //      Tab/Shift+Tab
        //      Enter/Return при пустой строке
        //      Escape
        if( (key == Qt::Key_Backtab && modifiers == Qt::ShiftModifier) ||
            (key == Qt::Key_Tab && modifiers == Qt::NoModifier) ||
            (key == Qt::Key_Escape && modifiers == Qt::NoModifier) )
        {

            QComboBox::hidePopup();
            setIsPopupVisible(0);
        }
    }

    // ретрансляция нажатых клавиш (ввод значения вручную без закрытия списка)
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        int key = keyEvent->key();
        int modifiers = keyEvent->modifiers();

        // в lineEdit ретранслируем все нажатия кроме:
        //      Down/Up
        //      Enter/Return
        if( (key != Qt::Key_Down) && (key != Qt::Key_Up) &&
            (key != Qt::Key_Enter) && (key != Qt::Key_Return) )
        {
            retranslateKey(QEvent::KeyPress, key, (Qt::KeyboardModifiers)modifiers,
                           keyEvent->text(), keyEvent->isAutoRepeat(), keyEvent->count());
            return true;
        }
    }

    // скрытие выпадающего списка
    if(e->type() == QEvent::Hide)
    {
        // Скрытие списка происходит (условно) при потере фокуса, например, при щелчке мышью за границами списка.
        // Однако, если щелчек происходит по LineEdit этого виджета, то после скрытия списка возникают дполнительные
        // события MouseButtonPress и MouseButtonRelease, список скрывается и тут же отображается вновь. Чтобы это
        // исправить использована переменная-флаг и могут учитываться текущие координаты курсора (при выборе
        // элемента списка мышью)
        QPoint topLeft = mapToGlobal(QPoint(0, 0));
        QSize widthHeight = QSize((frameGeometry().width() - iconSize().width()), frameGeometry().height());
        QPoint cursor = QCursor::pos();
        if( (considerCursorPosOnHide() && !isPointInArea( cursor, QRect(topLeft, widthHeight))) || !considerCursorPosOnHide() )
        {
            setIsPopupVisible(0);
        }
        setConsiderCursorPosOnHide(1);
    }
    return false;
}

bool SComboBox::isPointInArea(const QPoint &point, const QRect &area) const
{
    return ( point.x() > area.x() && point.x() < (area.x() + area.width()) && \
             point.y() > area.y() && point.y() < (area.y() + area.height()) );
}

void SComboBox::retranslateKey(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text, bool autorep, ushort count)
{
    QKeyEvent* newEvent = new QKeyEvent(type, key, modifiers,
                                        text, autorep, count);
    QFocusEvent* focusEvent = new QFocusEvent(QEvent::FocusIn, Qt::OtherFocusReason);
    QCoreApplication::postEvent(lineEditWidget, focusEvent);
    QCoreApplication::postEvent(lineEditWidget, newEvent);
}

void SComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
}

/* Возвращает код, используемый в БД, выбранного индекса */
int SComboBox::currentDbId()
{
    //    model()->index(currentIndex(),)
    return 0;
}

void SComboBox::setEditable(bool editable)
{
    QComboBox::setEditable(editable);
    if(!lineEditWidget)
    {
        lineEditWidget = QComboBox::lineEdit();
        lineEditWidget->installEventFilter(this);
        szHint = lineEditWidget->sizeHint();
        minSzHint = lineEditWidget->minimumSizeHint();
        connect(lineEditWidget, &QLineEdit::editingFinished, this, &SComboBox::longTextHandler);
    }
}

void SComboBox::showPopup()
{
    setIgnoreFocusOut(1); // при показе списка нужно оставить мигающий курсор в строке
    QComboBox::showPopup();

    // инициализация указателя на объект выпадающего списка (чтобы не вызывать каждый раз метод view())
    // объект выпадающего списка создаётся при первом отображении и не изменяется даже при установке другой модели данных
    if(!listViewWidget)
    {
        listViewWidget = view();
        if(listViewWidget)
            listViewWidget->installEventFilter(this);
    }
}

void SComboBox::hidePopup()
{
    QComboBox::hidePopup();
}

bool SComboBox::isPopupVisible() const
{
    return m_isPopupVisible;
}

void SComboBox::setIsPopupVisible(bool state)
{
    m_isPopupVisible = state;
}

void SComboBox::toggleIsPopupVisible()
{
    m_isPopupVisible ^= 1;
}

bool SComboBox::considerCursorPosOnHide() const
{
    return m_considerCursorPosOnHide;
}

void SComboBox::setConsiderCursorPosOnHide(bool state)
{
    m_considerCursorPosOnHide = state;
}

bool SComboBox::ignoreFocusOut() const
{
    return m_ignoreFocusOut;
}

void SComboBox::setIgnoreFocusOut(bool state)
{
    m_ignoreFocusOut = state;
}

void SComboBox::longTextHandler()
{
    lineEditWidget->home(false);
    if(fontMetrics->size(Qt::TextSingleLine, lineEditWidget->text()).width() > width())
        setToolTip(lineEditWidget->text());
    else
        setToolTip(QString());
}

