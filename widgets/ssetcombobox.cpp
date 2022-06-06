#include "ssetcombobox.h"

daughterLineEdit::daughterLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    fm = new QFontMetrics(this->font());
    frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth)*2;

    connect(this, SIGNAL(editingFinished()), this, SLOT(home()));
    this->installEventFilter(this);
    setFocusPolicy(Qt::ClickFocus); // rev 38: Qt::NoFocus и Qt::ClickFocus дают одинаковое поведение  переключения по Tab: все дочерние элементы расположены в конце очереди
                                    // Qt::TabFocus ничем по поведению не отличается от политики по умолчанию
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

QSize daughterLineEdit::sizeHint()
{
    return QSize(40, QLineEdit::sizeHint().height());
}

void SSetComboBox::clearEditText()
{
//    qDebug() << "Deleting daughterlinedits...";
    for(int i=daughterLineEdits.size()-1;i>=0;i--)
    {
        daughterLineEdits[i]->deleteLater();
        this->daughterLineEdits.removeAt(i);
        rearrangeDaughterLineEdits(this->width());
    }
}

bool daughterLineEdit::eventFilter(QObject *watched, QEvent *event)
{
//    if ( event->type() != QEvent::Paint && event->type() != QEvent::Paint && event->type() != QEvent::HoverMove &&
//         event->type() != QEvent::HoverEnter && event->type() != QEvent::HoverLeave && event->type() != QEvent::MouseMove &&
//         event->type() != QEvent::MetaCall && event->type() != QEvent::WindowDeactivate &&
//         event->type() != QEvent::WindowActivate && event->type() != QEvent::InputMethodQuery )
//        qDebug() << this->parent()->parent()->objectName() << ": daughterLineEdit::eventFilter(): " << event << "; object@" << this;

    return false;
}

void daughterLineEdit::resizeEvent(QResizeEvent *)
{
//    qDebug() << "daughterLineEdit::resizeEvent";

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
//    qDebug() << "daughterLineEdit::resizeToText: maximumWidth()=" << maximumWidth();
    // размер LineEdit это: ширина_текста + стандартный_зазор + ширина_кнопки + стандартный_зазор
    int pixelsWide = fm->size(Qt::TextSingleLine, this->text()).width() + frameWidth*2 + (deleteButtonEnabled?(buttonSize.width() + frameWidth):0);
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

//    emit textChanged();
}

void daughterLineEdit::home() // перемещаем курсор в начало текста
{
//    qDebug() << "daughterLineEdit::home()";
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
//        delete buttonIcon;
        delete deleteButton;
    }
    // TODO: проверить не остаётся ли в памяти чего-то ненужного
}

CBLineEdit::CBLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    installEventFilter(this);
}

CBLineEdit::~CBLineEdit()
{

}

void CBLineEdit::mousePressEvent(QMouseEvent *event)
{
//    qDebug() << this->objectName() << ": lineEditMousePressEvent(QMouseEvent *event))";
    emit mouseButtonPress();
}

void CBLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
//    qDebug() << this->objectName() << ": lineEditMouseReleaseEvent(QMouseEvent *event))";
    /* Ничего не делаем */
}

void CBLineEdit::resizeEvent(QResizeEvent *event)
{
//    qDebug() << this->parent()->objectName() << ": lineEditResizeEvent(QResizeEvent *event): event: " << event;
}

bool CBLineEdit::eventFilter(QObject *obj, QEvent *event)
{
//    if ( event->type() != QEvent::Paint && event->type() != QEvent::Paint && event->type() != QEvent::HoverMove &&
//         event->type() != QEvent::HoverEnter && event->type() != QEvent::HoverLeave && event->type() != QEvent::MouseMove &&
//         event->type() != QEvent::MetaCall && event->type() != QEvent::WindowDeactivate &&
//         event->type() != QEvent::WindowActivate )
//        qDebug() << this->parent()->objectName() << ": lineEditEventFilter: " << event;

    return false;
}

viewEventFilter::viewEventFilter(QObject *parent) :
    QObject(parent)
{
}

bool viewEventFilter::eventFilter(QObject *watched, QEvent *event)
{
//    qDebug() << watched->objectName() << ": viewEventFilter: " << event;

    SSetComboBox *cparent = static_cast<SSetComboBox*>(parent());   // я точно знаю, что родитель — это SSetComboBox

//    if (event->type() == QEvent::Show)
//    {
//        qDebug() << watched->objectName() << ": viewEventFilter: Show: current index = " << cparent->currentIndex();
//    }

    if (event->type() == QEvent::ShortcutOverride)
    {
        QKeyEvent* shortcutEvent = static_cast<QKeyEvent*>(event);

        // прячем раскрывающийся список при (ИЛИ):
        //      Tab/Shift+Tab
        //      Enter/Return при пустой строке
        if( (shortcutEvent->key() == Qt::Key_Tab && shortcutEvent->modifiers() == Qt::Key_Shift) ||
            (shortcutEvent->key() == Qt::Key_Tab && shortcutEvent->modifiers() == Qt::NoModifier) ||
            ( ((shortcutEvent->key() == Qt::Key_Enter) || (shortcutEvent->key() == Qt::Key_Return)) && cparent->lineEdit()->text() == "" ) )
        {
//            qDebug() << watched->objectName() << ": viewEventFilter: Enter/Return on empty lineEdit pressed";
            cparent->hidePopup();
            cparent->isPopupShown = 0;
        }
//        qDebug() << watched->objectName() << ": viewEventFilter: ShortcutOverride: current index = " << cparent->currentIndex();
    }

    // чтобы при вводе текста в LineEdit не прятался popup
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // ретранслируем в lineEdit нажатия клавиш (ИЛИ):
        //      Enter/Return при НЕ пустой строке
        //      всех кроме Enter/Return/Down/Up
        if( ( ((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) && cparent->lineEdit()->text() != "" ) ||
            ( (keyEvent->key() != Qt::Key_Down) && (keyEvent->key() != Qt::Key_Up) &&
              (keyEvent->key() != Qt::Key_Enter) && (keyEvent->key() != Qt::Key_Return)) )
        {   // Enter/Return не ретранслируем в родительский LineEdit
            QKeyEvent* newEvent = new QKeyEvent(keyEvent->type(), keyEvent->key(), keyEvent->modifiers(),
                                            keyEvent->text(), keyEvent->isAutoRepeat(), keyEvent->count());
            QFocusEvent* focusEvent = new QFocusEvent(QEvent::FocusIn, Qt::OtherFocusReason);
            QCoreApplication::postEvent(cparent->lineEdit(), focusEvent);
            QCoreApplication::postEvent(cparent->lineEdit(), newEvent);
        }

        if( (keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Escape) && cparent->daughterLineEdits.size() ) // если есть дочерние элементы и пользователь нажимает backspace или Esc
        {
            cparent->deleteDaughterLineEditOnKeyPress(keyEvent->key());
        }
    }

//    qDebug() << watched->objectName() << ": viewEventFilter (return): " << event;
    return false;
}

SSetComboBox::SSetComboBox(QWidget *parent) :
    QComboBox(parent),
    proxyModel(new QSortFilterProxyModel(this))    // при создании инициализируем прокси модель пустой моделью
{
//    qDebug() << this->objectName() << ": Object constructor";

    setEditable(true);
    setInsertPolicy(QComboBox::NoInsert);    // введённые пользователем не нужно добавлять в выпадающий список.
    setRowHeight(16);
    fontMetrics = new QFontMetrics(this->font());
    daughterLineEditHeight = fontMetrics->height();     // высота дочерних lineEdit по умолчанию будет рассчитываться по размеру шрифта
    defaultComboBoxHeight = this->height();      // = 22; этот размер базовый, используется при перестановке дочерних LineEdit

    viewEventFilterObj = new viewEventFilter(this);
    view()->installEventFilter(viewEventFilterObj);

    installEventFilter(this);

    isPopupShown = 0;

    connect( this, SIGNAL(activated(int)), this, SLOT(activatedHandler(int)) );
    connect( lineEdit(), SIGNAL(textEdited(const QString&)), this, SLOT(updateProxyModelFilter(const QString&)) );
    connect( lineEdit(), SIGNAL(editingFinished()), this, SLOT(parentLineEditEditingFinished()) );
    connect( lineEdit(), SIGNAL(mouseButtonPress()), this, SLOT(parentLineEditFocused()) );

    field_separator = "; "; // разделитель по умолчанию
}

SSetComboBox::~SSetComboBox()
{
    clearEditText();
    delete lineEdit();
    delete proxyModel;
    delete fontMetrics;
    delete viewEventFilterObj;

#ifdef CUSTOM_COMPLETER     // если используется собственный compliter
    delete p_completer;
#endif

    // TODO: проверить не остаётся ли в памяти чего-то ненужного
}

// Event filter forwards view key events to the line edit.
bool SSetComboBox::eventFilter(QObject *watched, QEvent *event)
{
//    if ( event->type() != QEvent::Paint && event->type() != QEvent::Paint && event->type() != QEvent::HoverMove &&
//         event->type() != QEvent::HoverEnter && event->type() != QEvent::HoverLeave && event->type() != QEvent::MouseMove &&
//         event->type() != QEvent::MetaCall && event->type() != QEvent::WindowDeactivate &&
//         event->type() != QEvent::WindowActivate && event->type() != QEvent::InputMethodQuery )
//        qDebug() << this->objectName() << ": coboBoxEventFilter: (" << event << ")";
    if ( event->type() == QEvent::FocusIn )
    {
        QFocusEvent *fi = static_cast<QFocusEvent*>(event);
        if (fi->reason() == Qt::TabFocusReason || fi->reason() == Qt::BacktabFocusReason )    // генерируем только по Tab, Shift+Tab, по клику мышкой будет другое событие
        {
//            qDebug() << objectName() << ": focusIn (event:" << event << ")";
            parentLineEditFocused();
        }
    }

    return false;
}

void SSetComboBox::setModel(QAbstractItemModel *model)
{
//    qDebug() << this->objectName() << ": SSetComboBox::setModel";

    proxyModel->setSourceModel(model);
    QComboBox::setModel(proxyModel);

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

void SSetComboBox::setEditable(bool editable)
{
    QComboBox::setEditable(editable);   // сначала вызываем функцию наследуемого класса
    if (editable) {
        CBLineEdit *le = new CBLineEdit(this);  // создаём свой LineEdit
        lineEdit()->deleteLater();              // удаляем LineEdit, созданный функцией наследуемого класса (TODO: убедиться, что он удаляется)
        setLineEdit(le);                        // устанавливаем свой LineEdit
    }
}

int SSetComboBox::hasHeightForWidth()
{
    return 1;   // у SSetComboBox всегда включена опция hieght-for-width
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
//    qDebug() << this->objectName() << ": SSetComboBox::rearrangeDaughterLineEdits";
    int i;
    QMargins nextDaughterLineEditPosition = {0,0,0,0};
    int currParentLineEditHeight = minimumHeight();
    int newComboBoxHeight = defaultComboBoxHeight;

    currentDaughterLineEditPosition = {0,0,0,0};
    semicolon_separated_text = "";

    for (i=0; i < this->daughterLineEdits.size(); i++)
    {
//        qDebug() << this->objectName() << ": SSetComboBox::rearrangeDaughterLineEdits: this->lineEdit()->width()=" <<  this->lineEdit()->width() << "; int width = " << width;
        this->daughterLineEdits[i]->setMaximumWidth(this->lineEdit()->width());    // размер дочернего LineEdit не доложен превышать размер родительского
        this->daughterLineEdits[i]->resizeToText();
//        qDebug() << this->objectName() << ": SSetComboBox::rearrangeDaughterLineEdits: daughterLineEdits[i]->minimumWidth()=" <<  daughterLineEdits[i]->minimumWidth();
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
//        qDebug() << this->objectName() << ": SSetComboBox::rearrangeDaughterLineEdits: daughterLineEdits.size() == 0";
        currentDaughterLineEditPosition.setTop(0);
        currentDaughterLineEditPosition.setLeft(0);
    }

    currentDaughterLineEditPosition.setTop(currentDaughterLineEditPosition.top() - 1);  // нужно чуть-чуть "приподнять" строку ввода, чтобы она была в уровень с дочерними LineEdit
//    qDebug() << this->objectName() << ": SSetComboBox::rearrangeDaughterLineEdits: currParentLineEditHeight = " << currParentLineEditHeight << "; newComboBoxHeight = " << newComboBoxHeight;
    if (currParentLineEditHeight != newComboBoxHeight)
    {   // если высота изменяется, то сработает resizeEvent, в котором мы пересчитаем размеры и отступы родит. lineEdit
        setMinimumHeight(newComboBoxHeight);
    }
    // пересчитываем размеры и отступы родит. lineEdit
    updateLineEditGeometry();

    emit textChanged(semicolon_separated_text);   // посылаем сигнал (не знаю понадобится ли, но пусть будет)
//    qDebug() << this->objectName() << ": SSetComboBox::rearrangeDaughterLineEdits: lineEdit()->geometry()=" << lineEdit()->geometry() << "; lineEdit()->textMargins() = " << currentDaughterLineEditPosition;
//    qDebug() << this->objectName() << ": SSetComboBox::rearrangeDaughterLineEdits (return)";
    return newComboBoxHeight;   // возвращаем значение необходимой высоты
}

/* Удаление последнего дочернего эл-та двойным нажатием клавиши backspace
 * если элемент был ранее выделен и пользователь нажал Esc, то происходит отмена выделения
 * TODO: при отмене выделения закрывается выпадающий список, возможно, стоит противодействовать этому.
 */
void SSetComboBox::deleteDaughterLineEditOnKeyPress(int key)
{
    if(key == Qt::Key_Backspace && lineEdit()->text() == "" )
    {
//        qDebug() << this->objectName() << ": styleSheet" << daughterLineEdits.last()->styleSheet();
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

QString SSetComboBox::version()
{
    return QString("%1.%2.%3.%4").arg(MAJOR).arg(MINOR).arg(PATCH).arg(COMMIT);
}

void SSetComboBox::comboBoxSetFocus()
{
    this->lineEdit()->setFocus();
//    this->showPopup();
}

void SSetComboBox::addItem(const QString &text)
{
//    qDebug() << this->objectName() << ": SSetComboBox::addItem: text =" << text;
//    qDebug() << this->objectName() << ": daughterLineEditHeight = " << daughterLineEditHeight;

    if (text == "")
    {
//        qDebug() << this->objectName() << ": selected empty row";
        return;
    }

    dLineEdit = new daughterLineEdit(this->lineEdit());
    dLineEdit->setStyleSheet("QLineEdit { background-color: #E5E5E5; border: none }\
                                     QToolButton { background-color: #E5E5E5; border: none }");     // серый

    dLineEdit->resize(sizeHint().width(), daughterLineEditHeight);  // resize, похоже, принимает в качестве параметров размер с учётом окантовки, а size отдаёт размер без окантовки

    dLineEdit->enableDeleteButton();
    // размер дочернего LineEdit (в нём же устанавливается отступ текста от верхнего края)
    dLineEdit->setMaximumHeight(daughterLineEditHeight);    // default contentsRect(): QRect(0,0 40x16), default lineEdit size: 40x16
    dLineEdit->setMaximumWidth(this->lineEdit()->width());    // размер дочернего LineEdit не доложен превышать размер родительского
    // потом текст
    dLineEdit->setText(text);
    // TODO: по красоте, конечно, нужно сделать так, чтобы все эти настройки производились в конструкторе дочернего lineEdit, но пока я не умею :-(

    connect(dLineEdit, SIGNAL(buttonClicked(daughterLineEdit*)), this, SLOT(deleteDaughterLineEdit(daughterLineEdit*)));
    connect(dLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(daughterLineEditTextChanged()));
    connect(dLineEdit, SIGNAL(editingFinished()), this, SLOT(comboBoxSetFocus()));
    this->daughterLineEdits.append(dLineEdit);

    dLineEdit->show();
    emit daughterLineEditAdded();
    dLineEdit->home();
    // TODO: в АСЦ сделано так, что при выборе элемента из списка он в списке скрывается, при удалении дочернего LineEdit вновь отображается
    // а также при редактировании дочернего LineEdit, который прежде был добавлен из списка, но его текст изменился, он тоже вновь отображается
    // это можно сделать тоже через QSortFilterProxyModel, но потом...

    rearrangeDaughterLineEdits(this->width());

    proxyModel->setFilterFixedString(""); // очищаем строку фильтра
    this->lineEdit()->clear(); // если отключена вставка новых элементов, то нужно вручную очистить кастомный текст
    isPopupShown = 0;   // очищаем флаг отображения выпадающего списка

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
            isPopupShown = 1;
//            qDebug() << this->objectName() << ": SSetComboBox::addItem: selected sub-field in <>";
        }
    }

//    qDebug() << this->objectName() << ": SSetComboBox::addItem (return)";
}

void SSetComboBox::activatedHandler(int index)
{
//    qDebug() << this->objectName() << ": SSetComboBox::activatedHandler: index =" << index;
    // После выбора элемента указателем раскрывающийся список прячется, а нужно чтобы он продолжал оторажаться
    // список при этом, правда, успевает "мигнуть", но это мелочи, потом разберусь.
    // Дополнительное условие: список не должен вновь отображаться, если была выбрана пустая строка;
    // это чтобы при нажатии Enter в пустой строке работало скрытие/отображение.
    if (this->itemText(index) != "")
    {
        isPopupShown = 0;   // важен порядок! сначала изменяем состояние флага
        addItem(this->itemText(index));
        parentLineEditFocused(); // вызов функции-слота для повторного отображения списка
    }
    emit activatedDerived(index);
}

/*
 * TODO: придумать как добавлять в отфильтрованный список пустую строку, чтобы в случае, когда пользователь хочет
 * добавить дочерний эл-т с текстом, частично совпадающим с элементом из списка, добавился только введённый текст,
 * а не предложенный в отфильтрованном списке или автодополненный. На данный момент чтобы это сделать нужно нажать Esc/Backspace.
 */
void SSetComboBox::updateProxyModelFilter(const QString &text)
{
//    qDebug() << this->objectName() << ": SSetComboBox::updateProxyModelFilter";

    proxyModel->setFilterFixedString(text); // установка фильтра обновляет модель, и заменяет текущий текст
                                            // LineEdit на текст первого элемента в обновлённом выпадающем списке

    lineEdit()->setText(text);              // поэтому перезаписываем текст обратно :-) дичь, а шо делать... не разобрался как (TODO) сделать это элегантнее
}

void SSetComboBox::daughterLineEditTextChanged()
{
//    qDebug() << this->objectName() << ": SSetComboBox::daughterLineEditTextChanged()";
    rearrangeDaughterLineEdits(this->width());
}

void SSetComboBox::deleteDaughterLineEdit(daughterLineEdit *widget)
{
//    qDebug() << this->objectName() << ": SSetComboBox::deleteDaughterLineEdit";
    int i;

    for (i=0; this->daughterLineEdits[i] != widget; i++)
    {
    }
    this->daughterLineEdits.removeAt(i);
    widget->deleteLater();
    rearrangeDaughterLineEdits(this->width());

    this->lineEdit()->setFocus();    // возвращаем фокус
}

void SSetComboBox::parentLineEditEditingFinished()
{
//    qDebug() << this->objectName() << ": SSetComboBox::parentLineEditEditingFinished: lineEdit()->text() =" << this->lineEdit()->text();
    if (this->lineEdit()->text().length())
        addItem(this->lineEdit()->text());
}

void SSetComboBox::parentLineEditFocused()
{
//    qDebug() << this->objectName() << ": SSetComboBox::parentLineEditFocused()" << ": isPopupShown = " << isPopupShown << ": view()->isVisible() = " << view()->isVisible() << "; lineEdit()->text() =" << this->lineEdit()->text();
//    qDebug() << this->objectName() << ": SSetComboBox::parentLineEditFocused(): lineEdit()->geometry() = " << lineEdit()->geometry() << "lineEdit()->textMargins() = " << lineEdit()->textMargins();
    if (isPopupShown == 0)
    {   // Чтобы выпадающий список при первом щелчке оторажался, а при втором скрывался используем переменную-флаг
//        setCurrentIndex(0);     // при отображении принудительно устанавливаем текущий индекс, иначе при многократном
                                // нажатии Enter в список будет добавляться последний подсвеченный курсором элемент.
        setCurrentIndex(-1);

        // эмулируем нажатие на кнопку для отображения раскрывающегося списка (showPopup не работает как ожидается)
        QMouseEvent *e = new QMouseEvent(QEvent::MouseButtonPress, QPointF((this->frameGeometry().width() - this->iconSize().width()/2), (this->frameGeometry().height() - this->iconSize().height()/2)), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QComboBox::mousePressEvent(e);
        isPopupShown = 1;
        lineEdit()->setFocus();
        delete e;
    }
    else
        isPopupShown = 0;
}

QSize SSetComboBox::sizeHint()
{
//    qDebug() << this->objectName() << ": SSetComboBox::sizeHint()";
    return QSize(150, 22);
}

QSize SSetComboBox::minimumSizeHint()
{
//    qDebug() << this->objectName() << ": SSetComboBox::minimumSizeHint()";
    return QSize(150, 22);
}

/* Это перегруженная функция setGeometry(const QRect &geometry) */
void SSetComboBox::setGeometry(int x, int y, int w, int h)
{
//    qDebug() << this->objectName() << ": SSetComboBox::setGeometry(int x, int y, int w, int h)";
    setGeometry(QRect(x, y, w, h));
}

/* ширина устанавливается в соответствии с переданным значением, а высота выбирается большая из двух:
 * переданное значение или требуемое рассчитанное значение
 */
void SSetComboBox::setGeometry(const QRect &geometry)
{
//    qDebug() << this->objectName() << ": SSetComboBox::setGeometry(const QRect &geometry)";
    int mh = rearrangeDaughterLineEdits(geometry.width());

    // высота комбобокса может быть больше чем сумма высот дочерних элементов, но меньше не может
    if (geometry.height() > mh)
    {
        setMinimumHeight(geometry.height());
        QComboBox::setGeometry( geometry );
    }
    else
        QComboBox::setGeometry(geometry.left(), geometry.top(), geometry.width(), mh);
}

/* Размер строки родительского LineEdit'а; по этому значению будет задаваться размер дочерного LineEdit'а
 * значение не должно изменяться в процессе работы, а зависит только от масштабирования текста
 * TODO: сделать автоматический рассчет размера в соответствии с настройками интерфейса ОС
 */
void SSetComboBox::setRowHeight(int h)
{
    parentLineEditFrameSize = lineEdit()->geometry().left();   // = 3; до конца не разобрался, но это, вроде бы, статическая фигня и зависит от оконного менеджера ОС, поэтому посчитаем её сразу
    daughterLineEditFrameSize = lineEdit()->style()->pixelMetric(QStyle::PM_DefaultFrameWidth) * 2; // DefaultFrameWidth lineEdit'a равен 1
//    daughterLineEditHeight = h - 2 * parentLineEditFrameSize;
    daughterLineEditHeight = h;
    daughterLineEditFrameHeight = daughterLineEditHeight + daughterLineEditFrameSize;
//    qDebug() << this->objectName() << ": SSetComboBox::setRowHeight :  parentLineEditFrameSize = " << parentLineEditFrameSize << "; daughterLineEditHeight = " << daughterLineEditHeight << "; daughterLineEditFrameSize = " << daughterLineEditFrameSize;
}

void SSetComboBox::setMinimumSize(const QSize &size)
{
//    qDebug() << this->objectName() << ": SSetComboBox::setMinimumSize()";
    resize(qMax(this->size().width(),size.width()), qMax(this->size().height(),size.height()));
    QComboBox::setMinimumSize(size);
}

void SSetComboBox::setMinimumHeight(int h)
{
//    qDebug() << this->objectName() << ": SSetComboBox::setMinimumHeight(" << h << ")";
    eventTrigger |= SSetComboBox::minimumHeightUpdated;
    QComboBox::setMinimumHeight(h);
}

/*  размер задаётся аналогично функции setGeometry()
 *
 */
void SSetComboBox::resize(const QSize &size)
{
//    qDebug() << this->objectName() << ": SSetComboBox::resize(QSize &)";
//    qDebug() << this->objectName() << ": sizePolicy() = " << sizePolicy();
//    qDebug() << this->objectName() << ": SSetComboBox::resize: size = " << size;
    int mh = rearrangeDaughterLineEdits(size.width());

    // высота комбобокса может быть больше чем сумма высот дочерних элементов, но меньше не может
    if (size.height() > mh)
    {
        setMinimumHeight(size.height());
        QComboBox::resize( size );
    }
    else
        QComboBox::resize(size.width(), mh);
}

void SSetComboBox::resize(int w, int h)
{
//    qDebug() << this->objectName() << ": SSetComboBox::resize(int w, int h)";

    resize(QSize(w, h));
}

void SSetComboBox::updateLineEditGeometry()
{
//    qDebug() << this->objectName() << ": SSetComboBox::updateLineEditGeometry()";

    this->lineEdit()->resize(this->geometry().width() - parentLineEditFrameSize * 2 - iconSize().width(),
                             this->geometry().height() - parentLineEditFrameSize * 2);
    //                                                                                          ^
    //                                                                          это ширина кнопки "v" comboBox'а
    currentDaughterLineEditPosition.setBottom(this->geometry().height() - currentDaughterLineEditPosition.top() - daughterLineEditFrameHeight - parentLineEditFrameSize - daughterLineEditFrameSize );
    this->lineEdit()->setTextMargins(currentDaughterLineEditPosition);
}

QString SSetComboBox::separator()
{
    return field_separator;
}

QString SSetComboBox::text()
{
    return semicolon_separated_text;
}

QString SSetComboBox::currentText()
{
    return semicolon_separated_text;
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
    // Проверка соответствия политики размена комбобокса. Если будет не такой, то растягивание/сжатие по высоте не будет работать как задумано.
    QSizePolicy tsp(QSizePolicy::Preferred, QSizePolicy::Minimum);  // target sizePolicy
//    if (sp != tsp)
//        qDebug() << this->objectName() << "ACHTUNG! Политика размера не соответствует, должна быть QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum)";
    QComboBox::setSizePolicy(sp);
}

void SSetComboBox::setSizePolicy(QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical)
{
    setSizePolicy(QSizePolicy(horizontal, vertical));
}

void SSetComboBox::resizeEvent(QResizeEvent *event)
{
//    qDebug() << this->objectName() << ": SSetComboBox::resizeEvent: event: " << event << "; eventTrigger = " << eventTrigger;
//    qDebug() << this->objectName() << ": SSetComboBox::resizeEvent: minimumHeight() = " << minimumHeight() << "; maximumHeight()" << maximumHeight();
//    qDebug() << this->objectName() << ": SSetComboBox::resizeEvent: minimumSizeHint() = " << minimumSizeHint() << "; sizeHint()" << sizeHint();
//    qDebug() << this->objectName() << ": SSetComboBox::resizeEvent: height() = " << height();

    if ( !(eventTrigger & SSetComboBox::minimumHeightUpdated))   // если событие вызвано не установкой мин. высоты (а, например, изменением размера окна)
    {
        setMinimumHeight( rearrangeDaughterLineEdits(this->width()) );  // пересчитываем минимальную требуемую высоту
    }
    // при изменении размера нужно изменить размер lineEdit'а
    updateLineEditGeometry();

    eventTrigger &= ~(SSetComboBox::minimumHeightUpdated);   // всегда очищаем флаг; повторно вызывать rearrange не нужно

//    qDebug() << this->objectName() << ": SSetComboBox::resizeEvent (return); eventTrigger = " << eventTrigger;
}

void SSetComboBox::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug() << this->objectName() << ": SSetComboBox::mouseMoveEvent(QMouseEvent *event): event =" <<event;
}

void SSetComboBox::wheelEvent(QWheelEvent *event)
{
//    qDebug() << this->objectName() << ": SSetComboBox::wheelEvent(QWheelEvent *event): event =" <<event;
    /* Ничего не делаем, чтобы при прокручивании колёсика мыши не добавлялись дочерние элементы */
}

void SSetComboBox::keyPressEvent(QKeyEvent *event)
{
//    qDebug() << this->objectName() << ": SSetComboBox::keyPressEvent(QKeyEvent *event): event =" <<event;
    // когда фокус находится в LineEdit, нажатие клавиш вверх/вниз вызывает активацию элементов раскрывающегося списка (при этом список не отображается)
    // и, соответственно добавление дочерних элементов;
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ) // подавляем нажатие этих клавиш
        return;

    if( (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Escape) && daughterLineEdits.size() ) // если есть дочерние элементы и пользователь нажимает backspace или Esc
    {
        deleteDaughterLineEditOnKeyPress(event->key());
    }

    // если поле пустое, то нажатие Enter вызовет отображение раскрывающегося списка. Это для удобства работы с клавиатурой
    if ( (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && lineEdit()->text() == "" )
        parentLineEditFocused();
    QComboBox::keyPressEvent(event);
}

void SSetComboBox::mousePressEvent(QMouseEvent *event)
{
    // это срабатывает только при нажатии на кнопку "v"; вызываем обработчик из наследуемого класса
//    qDebug() << this->objectName() << ": SSetComboBox::mousePressEvent(QMouseEvent *event)";
    QComboBox::mousePressEvent(event);
}

void SSetComboBox::mouseDoubleClickEvent(QMouseEvent *event)
{
    // это срабатывает только при нажатии на кнопку "v"; двойной клик пока не нужен, поэтому ничего не делаем
//    qDebug() << this->objectName() << ": SSetComboBox::mouseDoubleClickEvent(QMouseEvent *event)";
    // TODO: добавить обработку двойного клика
}
