#include "ssetcombobox.h"

daughterLineEdit::daughterLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    fm = new QFontMetrics(this->font());
    frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth)*2;
    setStyleSheet("QLineEdit { background-color: #E5E5E5; border: none }\
                                         QToolButton { background-color: #E5E5E5; border: none }");     // серый
    resize(parent->sizeHint().width(), fm->height());  // resize, похоже, принимает в качестве параметров размер с учётом окантовки, а size отдаёт размер без окантовки
    enableDeleteButton();
    setMaximumHeight(fm->height());
    setMaximumWidth(parent->width());    // размер дочернего LineEdit не доложен превышать размер родительского

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(resizeToText(QString)));
    connect(this, &QLineEdit::editingFinished, this, &daughterLineEdit::home);
    this->installEventFilter(this);
}

void daughterLineEdit::enableDeleteButton(bool newState)
{
    deleteButtonEnabled = newState;
    if (deleteButtonEnabled)
    {
        deleteButton = new QToolButton(this);
        deleteButton->resize(buttonSize); // обязательно задать размер, иначе размер кнопки будет на 3 (?) пикселя больше размера картинки
        deleteButton->setCursor(Qt::ArrowCursor);
        deleteButton->setStyleSheet("QToolButton { border: 0px; padding: 0px; }"); // здесь это не работает, потому что замещается настройкой стиля, заданной сразу после
        deleteButton->setStyleSheet("::hover { border: 0px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"); // а это работает
        QObject::connect( deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()) );

//        deleteButton->setIconSize(QSize(buttonSize.width() - frameWidth*2, buttonSize.height() - frameWidth*2));  // не влияет на потребление ресурсов
//        deleteButton->setText("🗙");     // эконом вариант
        // ✖ или 🗙 или ⌫
        deleteButton->setIcon(QIcon(":/icons/light/1F5D9_32.png"));

//        QPixmap pixmap("pencil.png");
//        deleteButton->setIcon(QIcon(pixmap));   // неплохой результат с иконкой .png размером 35кБ, загружаемой с диска, потребление выросло не более чем на 7% по сравнению со встроенной иконкой Qt
                                                  // при чем размер иконки, похоже, не влияет (с файлом .ico размером 300кБ потребление ОЗУ было даже чуть-чуть меньше).
    }
}

void daughterLineEdit::setMaximumHeight(int height)
{
    QMargins mrg;
    setMaximumSize(maximumWidth(), height); // если текущее значение высоты больше нового максимального значения высоты, то оно будет уменьшено
    mrg = this->textMargins();
    mrg.setTop((this->size().height() - fm->height()));
    this->setTextMargins(mrg);
}

void daughterLineEdit::setMaximumWidth(int w)
{
    QLineEdit::setMaximumWidth(w);
}

void daughterLineEdit::resize(const QSize &size)
{
    sz.setHeight(size.height());
    buttonSize = {size.height() - frameWidth, size.height() - frameWidth};  // обновляем значение
    QLineEdit::resize( size );
}

void daughterLineEdit::resize(int w, int h)
{
    resize(QSize(w, h));
}

QSize daughterLineEdit::sizeHint() const
{
    return QSize(40, QLineEdit::sizeHint().height());
}

void daughterLineEdit::setText(const QString &text)
{
    QLineEdit::setText(text);
    home();
}

bool daughterLineEdit::eventFilter(QObject*, QEvent*)
{
    return false;
}

void daughterLineEdit::resizeEvent(QResizeEvent *)
{
    if (deleteButtonEnabled)
        this->deleteButton->move( rect().right() - buttonSize.width(),
                                (rect().height() - buttonSize.height())/2 );
}

void daughterLineEdit::deleteButtonClicked()
{
    emit this->buttonClicked(this);
}

void daughterLineEdit::resizeToText()
{
    resizeToText(this->text());
}

void daughterLineEdit::resizeToText(const QString &text)
{
    // размер LineEdit это: ширина_текста + стандартный_зазор + ширина_кнопки + стандартный_зазор
    int pixelsWide = fm->size(Qt::TextSingleLine, text).width() + frameWidth*2 + (deleteButtonEnabled?(buttonSize.width() + frameWidth):0);
    if (pixelsWide <= maximumWidth())
    {   // если длина текста не превышает максимальной длины
        setMinimumSize(pixelsWide, buttonSize.height() + frameWidth); // минимальный размер lineEdit изменяем в соотв. с размером кнопки (-ок) плюс зазоры
        sz.setWidth(pixelsWide);    // устанавливаем размер
    }
    else
    {    // если текст длинее максимальной ширины
        setMinimumSize(maximumWidth(), buttonSize.height() + frameWidth); // минимальный размер lineEdit изменяем в соотв. с размером кнопки (-ок) плюс зазоры
        sz.setWidth(maximumWidth());    // устанавливаем ширину равную максимальной
    }

    this->resize(sz);
    updateTextMargins();    // обновляем поле для текста

    emit textChanged();
}

void daughterLineEdit::home() // перемещаем курсор в начало текста
{
    QLineEdit::home(false);
}

void daughterLineEdit::updateTextMargins()    // пересчитываем границы текста в соответствии с размером lineEdit и кол-вом кнопок
{
    setTextMargins(0, deleteButtonEnabled?((buttonSize.width() - fm->height())/2):0, (deleteButtonEnabled?this->height():0), deleteButtonEnabled?((buttonSize.width() - fm->height())/2):0); // лучше задавать отступ справа так, а не с пом. setStyleSheet, т. к. в вышестоящей функции может потребоваться изменить внешний вид
}

daughterLineEdit::~daughterLineEdit()
{
    delete fm;
    if (deleteButtonEnabled)
    {
        delete deleteButton;
    }
}

SSetComboBox::SSetComboBox(QWidget *parent) :
    SComboBox(parent)
{
    setEditable(true);
    setProperty("selectOnTab", false);
    setInsertPolicy(SComboBox::NoInsert);    // введённые пользователем не нужно добавлять в выпадающий список.
    setRowHeight(16);
    lineEditWidget->enableAutoSetCursorPositionToBegin(false);
    daughterLineEditHeight = fontMetrics->height();     // высота дочерних lineEdit по умолчанию будет рассчитываться по размеру шрифта
    defaultComboBoxHeight = this->height();      // = 22; этот размер базовый, используется при перестановке дочерних LineEdit

    proxyModel = new SSortFilterProxyModel(this);
    connect( this, SIGNAL(activated(int)), this, SLOT(activatedHandler(int)) );
    connect( lineEditWidget, SIGNAL(textEdited(QString)), this, SLOT(updateProxyModelFilter(QString)) );
    connect( lineEditWidget, SIGNAL(editingFinished()), this, SLOT(parentLineEditEditingFinished()) );

    field_separator = "; "; // разделитель по умолчанию
}

SSetComboBox::~SSetComboBox()
{
    clearEditText();
    delete lineEditWidget;
    delete proxyModel;

#ifdef CUSTOM_COMPLETER     // если используется собственный compliter
    delete p_completer;
#endif

    // TODO: проверить не остаётся ли в памяти чего-то ненужного
}

void SSetComboBox::setEditable(bool editable)
{
    SComboBox::setEditable(editable);
    keyPressReceiver = lineEditWidget;
}

bool SSetComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == listViewWidget)
    {
        if(event->type() == QEvent::Show)
        {
            updatePopupGeometry();
            setIgnorePopupHide(1);
        }
        if(event->type() == QEvent::Enter)   //
            setIgnorePopupHide(1);
        if(event->type() == QEvent::Leave)
            setIgnorePopupHide(0);
        if(event->type() == QEvent::ShortcutOverride)
        {
            int key = static_cast<QKeyEvent*>(event)->key();
            if( (key == Qt::Key_Enter || key == Qt::Key_Return) )
                setIgnorePopupHide(1);
        }
    }

    if ( event->type() == QEvent::KeyPress )
    {
        int key = static_cast<QKeyEvent*>(event)->key();
        if( (key == Qt::Key_Backspace || key == Qt::Key_Escape) )
            if( this->daughterLineEdits.size() && (watched == lineEditWidget || watched == this) ) // если есть дочерние элементы и пользователь нажимает backspace или Esc
                this->deleteDaughterLineEditOnKeyPress(key);

        if( (key == Qt::Key_Enter || key == Qt::Key_Return) )
        {
            setIgnorePopupHide(1);
            if( lineEditWidget->text() != "" )
                addItem(lineEditWidget->text());
        }

        if( watched == this && (key == Qt::Key_Down || key == Qt::Key_Up) ) // игнор вверх/вниз при скрытом списке
            return true;

        // TODO: добавить возможность навигации по добавленным дочерним lineEdit'ам и их редактирования с помощью клавиатуры
    }

    if( event->type() == QEvent::FocusOut )
    {
        if( lineEditWidget->text() != "" )
            addItem(lineEditWidget->text());
    }

    // после добавления дочернего эл-та курсор в родительском lineEdit перестаёт мигать
    // чтобы это исправить эмулируется нажатие клавиши. Фильтрация этих событий проблему не решает.
    // Событие KeyRelease происходит при выборе эл-та клавиатурой, а событие Timer при выборе мышью).
    // Может позже найдётся решение проще.
    if ( watched == listViewWidget && (event->type() == QEvent::KeyRelease || event->type() == QEvent::Timer) )
    {
        QKeyEvent *emu = new QKeyEvent(QEvent::KeyPress, Qt::Key_Control, Qt::NoModifier);
        QCoreApplication::postEvent(listViewWidget, emu);
    }

    if(SComboBox::eventFilter(watched, event))
        return true;

    return false;
}

void SSetComboBox::setModel(QAbstractItemModel *model)
{
    proxyModel->setSourceModel(model);
    SComboBox::setModel(proxyModel);

    p_completer = completer();
#ifdef CUSTOM_COMPLETER     // если я вдруг захочу заменить completer, созданный автоматически при установке модели
    p_completer->deleteLater();           // то нужно удалить автоматически созданный
    p_completer = new QCompleter(model);  // создать свой
    setCompleter(p_completer);            // установить его comboBox'у
#endif
    p_completer->setCompletionMode(QCompleter::InlineCompletion);   // можно использовать QCompleter::InlineCompletion

    setCurrentIndex(-1);

#ifdef QT_DEBUG
    view()->setObjectName(this->objectName()+"->view()");
#endif
}

/* перестановка дочерних элементов; порядок не изменяется; элементы передвигаются левее/выше,
 * если появилось достаточно места (например, был удалён элемент левее/выше), или правее/ниже,
 * если элемент левее/выше был изменён (увеличилось кол-во знаков)
 * Фукнция возвращает значение необходимой высоты для заданной ширины (height-for-width)
 * Также в функции объединяются тексты дочерних эл-тов в одну строку с заданным разделителем (см. функцию separator())
 * TODO: размер ComboBox'а по высоте никак не ограничен; нужно предустмотреть ограничение и полоску прокрутки для случаев
 * очень большого кол-ва дочерних эл-тов.
 */
int SSetComboBox::rearrangeDaughterLineEdits(int width)
{
    int i;
    QMargins nextDaughterLineEditPosition = {0,0,0,0};
    int currParentLineEditHeight = minimumHeight();
    int newComboBoxHeight = defaultComboBoxHeight;

    currentDaughterLineEditPosition = {0,0,0,0};
    semicolon_separated_text = "";

    for (i=0; i < this->daughterLineEdits.size(); i++)
    {
        this->daughterLineEdits[i]->setMaximumWidth(lineEditWidget->width());    // размер дочернего LineEdit не доложен превышать размер родительского
        nextDaughterLineEditPosition.setLeft((currentDaughterLineEditPosition.left() + this->daughterLineEdits[i]->width() + daughterLineEditFrameSize));
        if ( nextDaughterLineEditPosition.left() > (width - iconSize().width()) ) // iconSize().width() - это ширина кнопки "v" ComboBox'а
        {   // если сумма ширин ранее размещённых дочерних LineEdit и текущего больше ширины ComboBox, то текущий LineEdit размещаем на новой строке
            currentDaughterLineEditPosition.setTop(currentDaughterLineEditPosition.top() + daughterLineEditFrameHeight);
            currentDaughterLineEditPosition.setLeft(0);
            newComboBoxHeight += daughterLineEditFrameHeight;
            nextDaughterLineEditPosition.setLeft((currentDaughterLineEditPosition.left() + this->daughterLineEdits[i]->width() + daughterLineEditFrameSize));
        }
        this->daughterLineEdits[i]->move(currentDaughterLineEditPosition.left(), currentDaughterLineEditPosition.top());   // перемещаем элемент
        if ( (width - iconSize().width()) - nextDaughterLineEditPosition.left() < 40 )
        {   // если для следующиего эл-та доступно менее 40px, то увеличиваем возвращаемое значение, а курсор устанавливаем на след. строку
            currentDaughterLineEditPosition.setTop(currentDaughterLineEditPosition.top() + daughterLineEditFrameHeight);
            currentDaughterLineEditPosition.setLeft(0);
            newComboBoxHeight += daughterLineEditFrameHeight;
        }
        else
        {
            currentDaughterLineEditPosition.setLeft( nextDaughterLineEditPosition.left() );   // увеличиваем отступ для следующего элемента или для текста
        }

        if (semicolon_separated_text != "") // если строка уже не пуста
            semicolon_separated_text.append(field_separator);   // добавляем разделитель
        semicolon_separated_text.append(this->daughterLineEdits[i]->text());    // добавляем эл-т
    }
    if (this->daughterLineEdits.size() == 0)
    {   // если был удалён последний дочерний эл-т, длина которого не позволяла разместить курсор за ним, то нужно принудительно обнулить параметры, чтобы comboBox "скукоржился" до нормального размера
        currentDaughterLineEditPosition.setTop(0);
        currentDaughterLineEditPosition.setLeft(0);
    }

    currentDaughterLineEditPosition.setTop(currentDaughterLineEditPosition.top() - 1);  // нужно чуть-чуть "приподнять" строку ввода, чтобы она была в уровень с дочерними LineEdit
    if (currParentLineEditHeight != newComboBoxHeight)
    {   // если высота изменяется, то сработает resizeEvent, в котором мы пересчитаем размеры и отступы родит. lineEdit
        setMinimumHeight(newComboBoxHeight);
    }
    // пересчитываем размеры и отступы родит. lineEdit
    updateLineEditGeometry();

    return newComboBoxHeight;   // возвращаем значение необходимой высоты
}

/* Удаление последнего дочернего эл-та двойным нажатием клавиши backspace
 * если элемент был ранее выделен и пользователь нажал Esc, то происходит отмена выделения
 * TODO: при отмене выделения закрывается выпадающий список, возможно, стоит противодействовать этому.
 */
void SSetComboBox::deleteDaughterLineEditOnKeyPress(int key)
{
    if(key == Qt::Key_Backspace && lineEditWidget->text() == "" )
    {
        if (daughterLineEdits.last()->styleSheet().contains("background-color: #3399FF"))     // если последний элемент уже с синим бэкграундом, значит это второе нажатие
        {
            deleteDaughterLineEdit(daughterLineEdits.last());
        }
        else    // иначе это первое нажатие
            daughterLineEdits.last()->setStyleSheet("QLineEdit { background-color: #3399FF; border: none }\
                                                     QToolButton { background-color: #3399FF; border: none }");     // первое нажатие выделяет синим
    }
    if(key == Qt::Key_Escape )
    {
        if (daughterLineEdits.last()->styleSheet().contains("background-color: #3399FF"))     // если последний элемент уже с синим бэкграундом
            daughterLineEdits.last()->setStyleSheet("QLineEdit { background-color: #E5E5E5; border: none }\
                                                     QToolButton { background-color: #E5E5E5; border: none }");     // отменяем выделение
    }
}

void SSetComboBox::showPopup()
{
    SComboBox::showPopup();
    keyPressReceiver = lineEditWidget;
    if(!popupWidget)
    {
        popupWidget = this->findChild<QFrame*>();
    }
}

void SSetComboBox::hidePopup()
{
    if(!ignorePopupHide())
    {
        // TODO: разобраться в глюке; Qt v5.15.4;
        // В этом классе, в отличие от родительского SComboBox, при щелчке мышью в области lineEdit
        // список скрывается, но последующих событий MouseButtonPress и MouseButtonRelease не происходит.
        // Поэтому выключаем учет положения курсора, как буд-то произошел щелчёк за пределами comboBox.
        setConsiderCursorPosOnHide(0);
        SComboBox::hidePopup();
    }
    setIgnorePopupHide(0);
    updateLineEditGeometry();
}

QString SSetComboBox::version()
{
    return QString("%1.%2.%3.%4").arg(MAJOR).arg(MINOR).arg(PATCH).arg(COMMIT);
}

void SSetComboBox::comboBoxSetFocus()
{
    lineEditWidget->setFocus();
}

void SSetComboBox::addItem(const QString &text)
{
    if (text == "")
    {
        return;
    }

    dLineEdit = new daughterLineEdit(lineEditWidget);
    dLineEdit->setText(text);

    connect(dLineEdit, SIGNAL(buttonClicked(daughterLineEdit*)), this, SLOT(deleteDaughterLineEdit(daughterLineEdit*)));
    connect(dLineEdit, &daughterLineEdit::textChanged, this, &SSetComboBox::daughterLineEditTextChanged);
    connect(dLineEdit, SIGNAL(editingFinished()), this, SLOT(comboBoxSetFocus()));
    this->daughterLineEdits.append(dLineEdit);

    dLineEdit->show();
    emit daughterLineEditAdded();
    // TODO: в АСЦ сделано так, что при выборе элемента из списка он в списке скрывается, при удалении дочернего LineEdit вновь отображается
    // а также при редактировании дочернего LineEdit, который прежде был добавлен из списка, но его текст изменился, он тоже вновь отображается
    // это можно сделать тоже через QSortFilterProxyModel, но потом...

    rearrangeDaughterLineEdits(this->width());

    updateProxyModelFilter("");

    // элементы в списке могут содержать спец. поле для уточнения выбранного свойства
    // например, если пользователь заполняет комплектность устройства и выбирает "адаптер питания <модель>",
    // то текст в треуголных скобках будет автоматически выделен и пользователь может без лишних телодвижений
    // записать модель после чего нажать Enter и продолжить заполнение комплектности
    // выделение будет работать только с одним спец. полем (т. е. выделит только первое, если их вдруг будет два или больше)
    if (text.size() >= 2)   // не уверен, что это нужно, но пусть будет
    {
        if (text.contains(QRegularExpression("\\<.*\\>")))
        {
            int start = text.indexOf('<');
            int end = text.indexOf('>');

            dLineEdit->setFocus();
            dLineEdit->setSelection(start, end - start + 1);
            keyPressReceiver = dLineEdit;
            setIgnorePopupHide(0);
            hidePopup();
        }
    }

}

void SSetComboBox::activatedHandler(int index)
{
    // После выбора элемента указателем раскрывающийся список прячется, а нужно чтобы он продолжал отображаться
    // Дополнительное условие: список не должен вновь отображаться, если была выбрана пустая строка;
    // это чтобы при нажатии Enter в пустой строке работало скрытие/отображение.
    if (this->itemText(index) != "")
    {
        setIgnorePopupHide(1);
        addItem(this->itemText(index));
    }
    else
    {
        setIgnorePopupHide(0);
        hidePopup();
    }
}

/*
 * TODO: придумать как добавлять в отфильтрованный список пустую строку, чтобы в случае, когда пользователь хочет
 * добавить дочерний эл-т с текстом, частично совпадающим с элементом из списка, добавился только введённый текст,
 * а не предложенный в отфильтрованном списке или автодополненный. На данный момент чтобы это сделать нужно нажать Esc
 */
void SSetComboBox::updateProxyModelFilter(const QString &text)
{
    int cursorPos = lineEditWidget->cursorPosition();
    proxyModel->setFilterFixedString(text);         // установка фильтра обновляет модель, и заменяет текущий текст
                                                    // LineEdit на текст первого элемента в обновлённом выпадающем списке
    lineEditWidget->setText(text);                  // поэтому перезаписываем текст обратно :-) дичь, а шо делать... не разобрался как (TODO) сделать это элегантнее
    lineEditWidget->setCursorPosition(cursorPos);   // восстановление позиции курсора

    updateLineEditGeometry();                       // также при обновлении прокси модели размер lineEdit уменьшается на 16пикс. и может частично скрыть дочерние lineEdit
}

void SSetComboBox::daughterLineEditTextChanged()
{
    rearrangeDaughterLineEdits(this->width());
}

void SSetComboBox::deleteDaughterLineEdit(daughterLineEdit *widget)
{
    int i;

    for (i=0; this->daughterLineEdits[i] != widget; i++)
    {
    }
    this->daughterLineEdits.removeAt(i);
    widget->deleteLater();
    rearrangeDaughterLineEdits(this->width());

//    setIsPopupVisible(0);
    keyPressReceiver = lineEditWidget;
}

void SSetComboBox::parentLineEditEditingFinished()
{
    if (lineEditWidget->text().length())
        addItem(lineEditWidget->text());
}

/* Это перегруженная функция setGeometry(const QRect &geometry) */
void SSetComboBox::setGeometry(int x, int y, int w, int h)
{
    setGeometry(QRect(x, y, w, h));
}

/* ширина устанавливается в соответствии с переданным значением, а высота выбирается большая из двух:
 * переданное значение или требуемое рассчитанное значение
 */
void SSetComboBox::setGeometry(const QRect &geometry)
{
    int mh = rearrangeDaughterLineEdits(geometry.width());

    // высота комбобокса может быть больше чем сумма высот дочерних элементов, но меньше не может
    if (geometry.height() > mh)
    {
        setMinimumHeight(geometry.height());
        SComboBox::setGeometry( geometry );
    }
    else
        SComboBox::setGeometry(geometry.left(), geometry.top(), geometry.width(), mh);
}

/* Размер строки родительского LineEdit'а; по этому значению будет задаваться размер дочерного LineEdit'а
 * значение не должно изменяться в процессе работы, а зависит только от масштабирования текста
 * TODO: сделать автоматический рассчет размера в соответствии с настройками интерфейса ОС
 */
void SSetComboBox::setRowHeight(int h)
{
    parentLineEditFrameSize = lineEditWidget->geometry().left();   // = 3; до конца не разобрался, но это, вроде бы, статическая фигня и зависит от оконного менеджера ОС, поэтому посчитаем её сразу
    daughterLineEditFrameSize = lineEditWidget->style()->pixelMetric(QStyle::PM_DefaultFrameWidth) * 2; // DefaultFrameWidth lineEdit'a равен 1
//    daughterLineEditHeight = h - 2 * parentLineEditFrameSize;
    daughterLineEditHeight = h;
    daughterLineEditFrameHeight = daughterLineEditHeight + daughterLineEditFrameSize;
}

void SSetComboBox::setMinimumSize(const QSize &size)
{
    resize(qMax(this->size().width(),size.width()), qMax(this->size().height(),size.height()));
    SComboBox::setMinimumSize(size);
}

void SSetComboBox::setMinimumHeight(int h)
{
    eventTrigger |= SSetComboBox::MinimumHeightUpdated;
    SComboBox::setMinimumHeight(h);
}

/*  размер задаётся аналогично функции setGeometry()
 *
 */
void SSetComboBox::resize(const QSize &size)
{
    int mh = rearrangeDaughterLineEdits(size.width());

    // высота комбобокса может быть больше чем сумма высот дочерних элементов, но меньше не может
    if (size.height() > mh)
    {
        setMinimumHeight(size.height());
        SComboBox::resize( size );
    }
    else
        SComboBox::resize(size.width(), mh);
}

void SSetComboBox::resize(int w, int h)
{
    resize(QSize(w, h));
}

void SSetComboBox::updateLineEditGeometry()
{
    lineEditWidget->resize(this->geometry().width() - parentLineEditFrameSize * 2 - iconSize().width(),
                             this->geometry().height() - parentLineEditFrameSize * 2);
    //                                                                                          ^
    //                                                                          это ширина кнопки "v" comboBox'а
    updatePopupGeometry();
    currentDaughterLineEditPosition.setBottom(this->geometry().height() - currentDaughterLineEditPosition.top() - daughterLineEditFrameHeight - parentLineEditFrameSize - daughterLineEditFrameSize );
    lineEditWidget->setTextMargins(currentDaughterLineEditPosition);
}

void SSetComboBox::updatePopupGeometry()
{


   if(popupWidget)
        popupWidget->move(popupWidget->x(), mapToGlobal(lineEditWidget->pos()).y() + lineEditWidget->height());
}

void SSetComboBox::setCurrentIndex(int index)
{
    SComboBox::setCurrentIndex(index);
    updateLineEditGeometry();
}

QString SSetComboBox::text()
{
    return semicolon_separated_text;
}

QString SSetComboBox::currentText()
{
    return semicolon_separated_text;
}

void SSetComboBox::clearEditText()
{
    for(int i=daughterLineEdits.size()-1;i>=0;i--)
    {
        daughterLineEdits[i]->deleteLater();
        this->daughterLineEdits.removeAt(i);
        rearrangeDaughterLineEdits(this->width());
    }
}

QString SSetComboBox::separator()
{
    return field_separator;
}

void SSetComboBox::setSeparator(QString s)
{
    field_separator = s;
}

void SSetComboBox::setSeparator(char c)
{
    field_separator = QString(c);
}

void SSetComboBox::setSizePolicy(QSizePolicy sp)
{
    // Проверка соответствия политики размера комбобокса. Если будет не такой, то растягивание/сжатие по высоте не будет работать как задумано.
    SComboBox::setSizePolicy(sp);
}

void SSetComboBox::setSizePolicy(QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical)
{
    setSizePolicy(QSizePolicy(horizontal, vertical));
}

void SSetComboBox::resizeEvent(QResizeEvent*)
{

    if ( !(eventTrigger & SSetComboBox::MinimumHeightUpdated))   // если событие вызвано не установкой мин. высоты (а, например, изменением размера окна)
    {
        setMinimumHeight( rearrangeDaughterLineEdits(this->width()) );  // пересчитываем минимальную требуемую высоту
    }
    // при изменении размера нужно изменить размер lineEdit'а
    updateLineEditGeometry();

    eventTrigger &= ~(SSetComboBox::MinimumHeightUpdated);   // всегда очищаем флаг; повторно вызывать rearrange не нужно

}

void SSetComboBox::retranslateKey(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text, bool autorep, ushort count)
{
    QKeyEvent* newEvent = new QKeyEvent(type, key, modifiers,
                                        text, autorep, count);
    QFocusEvent* focusEvent = new QFocusEvent(QEvent::FocusIn, Qt::OtherFocusReason);
    QCoreApplication::postEvent(keyPressReceiver, focusEvent);
    QCoreApplication::postEvent(keyPressReceiver, newEvent);
}

bool SSetComboBox::ignorePopupHide() const
{
    return m_ignorePopupHide;
}

void SSetComboBox::setIgnorePopupHide(bool state)
{
    m_ignorePopupHide = state;
}

#ifdef QT_DEBUG
/*  Добавление случайного дочернего lineEdit из списка элементов модели данных
 *  или случайное число, если модель пуста
 *  Метод использует
*/
void SSetComboBox::addRandomItem()
{
    // нормальное добавление
    int i = QRandomGenerator::global()->bounded(model()->rowCount());
    addItem(model()->index(i, 0).data().toString());
    if (daughterLineEdits.isEmpty())
    {
        addItem(QString::number(QRandomGenerator::global()->bounded(2147483647)));
    }

    // Добавление путём иммитации работы пользователя (может глючить из-за недостаточных задержек)
//    showPopup();

//    QListView *lw = this->findChild<QListView *>();
//    if(lw)
//    {
//        while(!lw->isVisible())
//            QTest::qWait(10);

//        int i = QRandomGenerator::global()->bounded(lw->model()->rowCount());
//        QModelIndex idx = lw->model()->index(i,0);
//        lw->scrollTo(idx);
//        QPoint itemPt = lw->visualRect(idx).center();
//        if(!itemPt.isNull())
//        {
//            QTest::mouseClick(lw->viewport(), Qt::LeftButton, Qt::NoModifier, itemPt);
//            QTest::qWait(200);
//            if(isPopupVisible())
//            {
//                QTest::keyPress(lw, Qt::Key_Return);
//            }
//            QTest::qWait(200);
//        }
//    }
//    if (daughterLineEdits.isEmpty())
//    {
//        setCurrentText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
//        QTest::keyPress(lineEditWidget, Qt::Key_Return);
//        QTest::qWait(200);
//    }
//    hidePopup();
}
#endif

