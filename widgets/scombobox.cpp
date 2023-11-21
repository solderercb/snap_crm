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
    else if(lineEditWidget && watched == lineEditWidget)
        return eventFilterLineEdit(e);
    else if(watched == listViewWidget)
        return eventFilterListView(e);

    return false;
}

bool SComboBox::eventFilterComboBox(QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress )
    {
        if(!isEnabled())
        {
            return true;
        }

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

//    if(e->type() == QEvent::MouseButtonDblClick)
//    {
//        lineEditWidget->selectAll();
//        return true;
//    }

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
        // если поле редактируемое и пустое, то по нажатию Enter отображается выпадающий список,
        // а если поле не редактируемое, то список также отображается по нажатию Пробел
        bool show = 0;
        switch (static_cast<QKeyEvent*>(e)->key())
        {
            case Qt::Key_Space: show = m_lineEditRO; break;
            case Qt::Key_Enter:
            case Qt::Key_Return: show = m_lineEditRO || (currentText() == ""); break;
            default: ;
        }
        if(show)
        {
            showPopup();
            toggleIsPopupVisible();
        }
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
    if(!lineEditWidget)
        return false;

    if(e->type() == QEvent::MouseButtonPress)    // щелчек по LineEdit
    {
        if(!isEnabled())
            return true;

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
    else if ( e->type() == QEvent::KeyPress )
    {
        int key = static_cast<QKeyEvent*>(e)->key();
        // если поле пустое, то нажатие Enter вызовет отображение раскрывающегося списка. Это для удобства работы с клавиатурой
        if ( key == Qt::Key_Space && lineEdit()->isReadOnly() )
        {
            e->ignore();
            showPopup();
        }
    }

    return false;
}

bool SComboBox::eventFilterListView(QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        return true;
    }

    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        int key = keyEvent->key();
        int modifiers = keyEvent->modifiers();

        // при выборе элемента списка клавиатурой, нужно игнорировать позицию курсора, которая учитывается в событии скрытия списка
        if( (key == Qt::Key_Return) || (key == Qt::Key_Enter) )
            setConsiderCursorPosOnHide(0);

        // в lineEdit ретранслируем все нажатия кроме:
        //      Down/Up
        //      Enter/Return
        //      печатных символов в режиме readOnly
        bool retranslate = !m_lineEditRO;
        switch(key)
        {
            case Qt::Key_Down:
            case Qt::Key_Up: m_customInput = 0; Q_FALLTHROUGH();
            case Qt::Key_Enter:
            case Qt::Key_Return: retranslate = 0; break;
            case Qt::Key_Tab:
            case Qt::Key_Backtab: retranslate = 1; break;
            case Qt::Key_Control: break;
            default: m_customInput = 1;
        }
        if(retranslate)
        {
            retranslateKey(QEvent::KeyPress, key, (Qt::KeyboardModifiers)modifiers,
                           keyEvent->text(), keyEvent->isAutoRepeat(), keyEvent->count());
        }

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

        switch(key)
        {
            case Qt::Key_Tab:
                if( m_selectOnTab && (!(m_keepCustomText && m_customInput) || m_lineEditRO) )
                {
                    setCurrentIndex(listViewWidget->currentIndex().row());
                    break;
                }
            default: ;
        }

        return false;
    }

    if (e->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        int key = keyEvent->key();

        // после ввода произвольного текста и нажатия Enter в lineEdit всё равно устанавливается
        // значение выбранного элемента списка; чтобы этого не происходило нужно отфильтровать событие
        if( (key == Qt::Key_Return || key == Qt::Key_Enter) && m_customInput )
        {
            hidePopup();
            toggleIsPopupVisible();
            return true;
        }
        return false;
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

bool SComboBox::isSelectOnTab()
{
    return m_selectOnTab;
}

void SComboBox::setSelectOnTab(const bool state)
{
    m_selectOnTab = state;
}

bool SComboBox::isKeepCustomText()
{
    return m_keepCustomText;
}

void SComboBox::setKeepCustomText(const bool state)
{
    m_keepCustomText = state;
}

void SComboBox::retranslateKey(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text, bool autorep, ushort count)
{
    QWidget *target = this;
    if(!m_lineEditRO)   // m_lineEditRO по умолчанию 1
        target = lineEditWidget;

    QKeyEvent* newEvent = new QKeyEvent(type, key, modifiers,
                                        text, autorep, count);
    QFocusEvent* focusEvent = new QFocusEvent(QEvent::FocusIn, Qt::OtherFocusReason);
    QCoreApplication::postEvent(target, focusEvent);
    QCoreApplication::postEvent(target, newEvent);
}

void SComboBox::resizeEvent(QResizeEvent *event)
{
    if(lineEditWidget)
        lineEditWidget->setMinimumSize(event->size().width() - iconSize().width() - 6, lineEditWidget->minimumHeight());
    QComboBox::resizeEvent(event);
}

/* Возвращает код, используемый в БД, выбранного индекса */
int SComboBox::currentDbId()
{
    //    model()->index(currentIndex(),)
    return 0;
}

void SComboBox::initLineEdit()
{
    if(lineEditWidget == nullptr)
    {
        QString oldPlaceHolderText = placeholderText();
        lineEditWidget = new SLineEdit();
        lineEditWidget->enableAutoSetCursorPositionToBegin(false);
        lineEditWidget->setPlaceholderText(oldPlaceHolderText);
        connect(lineEditWidget, &SLineEdit::buttonClicked, this, &SComboBox::clearButtonPress);
        connect(lineEditWidget, &SLineEdit::buttonClicked, this, &SComboBox::buttonClicked);
    }
}

void SComboBox::setEditable(bool editable)
{
    if(editable)
    {
        initLineEdit();
        lineEditWidget->installEventFilter(this);
        connect(lineEditWidget, &SLineEdit::editingFinished, this, &SComboBox::longTextHandler);
        setLineEdit(lineEditWidget);
    }
    if(lineEditWidget)
    {
        lineEditWidget->setReadOnly(!editable);
    }
    m_lineEditRO = !editable;
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

QString SComboBox::buttons()
{
    if(!lineEditWidget)
        return QString();

    return lineEditWidget->buttons();
}

void SComboBox::setButtons(const QString &buttons)
{
    setEditable(true);  // инициализация lineEdit
    setEditable(false);

    lineEditWidget->setButtons(buttons);
    emit buttonsChanged();
}

void SComboBox::setPlaceholderText(const QString &text)
{
    if(!lineEditWidget)
    {
        QComboBox::setPlaceholderText(text);
        return;
    }

    lineEditWidget->setPlaceholderText(text);
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
    if(!lineEditWidget)
        return;

    lineEditWidget->home(false);
    if(fontMetrics->size(Qt::TextSingleLine, lineEditWidget->text()).width() > width())
        setToolTip(lineEditWidget->text());
    else
        setToolTip(QString());
}

void SComboBox::clearButtonPress(int id)
{
    if(id == SLineEdit::Clear)
    {
        lineEditWidget->clear();
        setCurrentIndex(-1);
    }
}

