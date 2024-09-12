#include "scombobox.h"
#ifndef SCOMBOBOX_STANDALONE
#include "global.h"
#endif // SCOMBOBOX_STANDALONE

SComboBox::SComboBox(QWidget *parent) :
    QComboBox(parent)
{
    m_retranslatedKeyReceiver = this;
    fontMetrics = new QFontMetrics(this->font());
    installEventFilter(this);
    szHint = QComboBox::sizeHint();
    minSzHint = QComboBox::minimumSizeHint();
    connect(this, qOverload<int>(&QComboBox::activated), this, &SComboBox::rowActivated);

    // к виджету, получаемому при вызове view(), нельзя применить стиль, поэтому его нужно его заменить
    listViewWidget = new QListView(this);
    listViewWidget->installEventFilter(this);
    applyListViewRowHeight();
    setView(listViewWidget);

// чтобы в мини-проекте для отладки виджета не подключать лишнее
#ifndef SCOMBOBOX_STANDALONE
    connect(userDbData, &SUserSettings::rowHeightChanged, this, &SComboBox::applyGuiSettings);
#endif // SCOMBOBOX_STANDALONE
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
        // Если пользователь ввёл текст, то нажатие Enter нужно игнорировать
        bool show = 0;
        bool filter = 0;
        switch (static_cast<QKeyEvent*>(e)->key())
        {
            case Qt::Key_Space: show = m_lineEditRO; break;
            case Qt::Key_Enter:
            case Qt::Key_Return: show = m_lineEditRO || (currentText() == ""); filter = (currentText() != ""); break;
            default: ;
        }
        if(show)
        {
            showPopup();
        }
        if(filter)
            return true;
    }

    // после переключения в другое окно список прячется
    if( e->type() == QEvent::WindowDeactivate )
    {
        setIsPopupVisible(0);
    }

    if( e->type() == QEvent::FocusOut )
    {
        Qt::FocusReason reason = static_cast<QFocusEvent*>(e)->reason();
        setPreserveLineEditFocus(1);
        if(reason == Qt::PopupFocusReason)
        {
            return true;
        }
        if((reason == Qt::TabFocusReason) || (reason == Qt::BacktabFocusReason) || (reason == Qt::MouseFocusReason))
        {
            setPreserveLineEditFocus(0);
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

        if(!isPopupVisible() && m_popupShowIntervalObserved)
        {
            showPopup();
        }
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

    if(isKeepListShown())
    {
        switch (e->type())
        {
            case QEvent::Show: updatePopupGeometry(); Q_FALLTHROUGH();
            case QEvent::Enter: setIgnorePopupHide(1); break;
            case QEvent::Leave: setIgnorePopupHide(0); break;
            case QEvent::ShortcutOverride:
            {
                int key = static_cast<QKeyEvent*>(e)->key();
                if( (key == Qt::Key_Enter || key == Qt::Key_Return) )
                    setIgnorePopupHide(1);
                break;
            }
            default: break;
        }
    }

    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        int key = keyEvent->key();
        int modifiers = keyEvent->modifiers();

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
            case Qt::Key_Return:
                retranslate = 0;
                break;
            case Qt::Key_Tab:
                if( m_selectOnTab && (!(m_keepCustomText && m_customInput) || m_lineEditRO) )
                    setCurrentIndex(listViewWidget->currentIndex().row());
                Q_FALLTHROUGH();
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
            setIgnorePopupHide(false);
            hidePopup();
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
        // исправить использован таймер и переменная-флаг
        startPopupShowIntervalTimer();
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

void SComboBox::applyListViewRowHeight()
{
    listViewWidget->setStyleSheet(QString("QListView::item{height: %1px}").arg(m_listRowHeight));
}

/* Параметр отвечает за скрытие выпадающего списка после щелчка по строке
 * В классическом ComboBox список закрывается, но в дочерних классах SSetComboBox и SCheckComboBox
 * список после щелчка должен оставаться открытым.
*/
bool SComboBox::isKeepListShown()
{
    return m_keepListShown;
}

void SComboBox::setKeepListShown(const bool state)
{
    m_keepListShown = state;
}

void SComboBox::startPopupShowIntervalTimer()
{
    m_popupShowIntervalObserved = 0;
    QTimer::singleShot(100, this, &SComboBox::setPopupShowIntervalObserved);
}

void SComboBox::setPopupShowIntervalObserved()
{
    m_popupShowIntervalObserved = 1;
}

void SComboBox::setListRowHeight(const int height)
{
    m_listRowHeight = height;
    applyListViewRowHeight();
}

void SComboBox::applyGuiSettings()
{

#ifndef SCOMBOBOX_STANDALONE
   m_listRowHeight = userDbData->rowHeight;
#else
    QStyleOptionViewItem *opt = new QStyleOptionViewItem();
    QStyle *style = this->style();
    opt->font = this->font();
    opt->fontMetrics = QFontMetrics(this->font());
    m_listRowHeight = style->sizeFromContents(QStyle::CT_ItemViewItem, opt, QSize(), this).height();
#endif

    applyListViewRowHeight();
}

void SComboBox::rowActivated(int)
{
}

void SComboBox::retranslateKey(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text, bool autorep, ushort count)
{
    QKeyEvent* newEvent = new QKeyEvent(type, key, modifiers,
                                        text, autorep, count);
    QFocusEvent* focusEvent = new QFocusEvent(QEvent::FocusIn, Qt::PopupFocusReason);
    QCoreApplication::postEvent(m_retranslatedKeyReceiver, focusEvent);
    QCoreApplication::postEvent(m_retranslatedKeyReceiver, newEvent);
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
    if(lineEditWidget)
        return;

    QString oldPlaceHolderText = placeholderText();
    lineEditWidget = new SLineEdit();
    lineEditWidget->installEventFilter(this);
    lineEditWidget->enableAutoSetCursorPositionToBegin(true);
    lineEditWidget->setPlaceholderText(oldPlaceHolderText);
    connect(lineEditWidget, &SLineEdit::buttonClicked, this, &SComboBox::clearButtonPress);
    connect(lineEditWidget, &SLineEdit::buttonClicked, this, &SComboBox::buttonClicked);
    setLineEdit(lineEditWidget);
    setRetranslatedKeyReceiver(lineEditWidget);
}

void SComboBox::setEditable(bool editable)
{
    if(editable)
    {
        initLineEdit();
    }

    if(lineEditWidget)
    {
        lineEditWidget->setReadOnly(!editable);
    }
    m_lineEditRO = !editable;
}

void SComboBox::showPopup()
{
//    if(!m_lineEditRO)
//        setPreserveLineEditFocus(1); // при показе списка нужно оставить мигающий курсор в строке

    QComboBox::showPopup();
    m_isPopupVisible = true;

    // popupWidget используется для перемещения выпадающего списка
    if(!popupWidget)
    {
        popupWidget = this->findChild<QFrame*>();
    }
}

void SComboBox::hidePopup()
{
    if(!ignorePopupHide())
    {
        QComboBox::hidePopup();
        setIsPopupVisible(0);
    }
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

void SComboBox::updatePopupGeometry()
{
    int y;

    y = mapToGlobal(this->rect().bottomLeft()).y();
    if(popupWidget && isPopupVisible())
        popupWidget->move(popupWidget->x(), y);
}

bool SComboBox::isPopupVisible() const
{
    return m_isPopupVisible;
}

void SComboBox::setIsPopupVisible(bool state)
{
    m_isPopupVisible = state;
}

bool SComboBox::ignorePopupHide() const
{
    return m_ignorePopupHide;
}

void SComboBox::setIgnorePopupHide(bool state)
{
    m_ignorePopupHide = state & m_keepListShown;
}

void SComboBox::setRetranslatedKeyReceiver(QWidget *widget)
{
    m_retranslatedKeyReceiver = widget;
}

bool SComboBox::isPreserveLineEditFocus() const
{
    return m_preserveLineEditFocus;
}

void SComboBox::setPreserveLineEditFocus(bool state)
{
    m_preserveLineEditFocus = state;
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
