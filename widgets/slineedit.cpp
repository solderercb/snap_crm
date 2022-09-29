#include "slineedit.h"

SLineEdit::SLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth)*2;
    signalMapper = new QSignalMapper(this);
    QObject::connect(signalMapper, SIGNAL(mappedInt(int)), this, SIGNAL(buttonClicked(int)));
}

void SLineEdit::setButtons(QString buttons)
{
    qDebug().nospace() << "[SLineEdit] setButtons()";
    if(!lineEditButtons.isEmpty())
        clearButtons();

    if (!buttons.isEmpty())
    {

        buttonsList = new QStringList(buttons.remove(' ').split(','));
        for (int i = 0; i < buttonsList->size();)
        {
            if (!allowedButtonsList.contains(buttonsList->value(i)))
            {
                buttonsList->removeAt(i);
                continue;
            }
            i++;
        }
        buttonsCount = buttonsList->size();
        lineEditButtons.resize(buttonsCount);

        for (int i = 0; i < buttonsCount; i++)
        {
            int buttonIndex = allowedButtonsList.indexOf( buttonsList->value(i) );
            lineEditButtons[i] = new QToolButton(this);
            lineEditButtons[i]->setCursor(Qt::ArrowCursor);
            lineEditButtons[i]->setStyleSheet("QToolButton { border: 0px;}    QToolButton::hover { border: 1px solid #0078D7;  background-color: #E5F1FB;}");
            signalMapper->setMapping(lineEditButtons[i], buttonIndex);
            QObject::connect(lineEditButtons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));

            lineEditButtons[i]->setIcon(QIcon(":/icons/light/" + buttonIconList[buttonIndex]));
            lineEditButtons[i]->show();
        }
    }
    arrangeButtons();
    qDebug().nospace() << "[SLineEdit] setButtons() | Out";
}

void SLineEdit::clearButtons()
{
    qDebug().nospace() << "[SLineEdit] clearButtons()";
    if(lineEditButtons.isEmpty())
        return;

    QToolButton *button;
    while(!lineEditButtons.isEmpty())
    {
        button = lineEditButtons.last();
        lineEditButtons.removeLast();
        QObject::disconnect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->removeMappings(button);
        delete button;
    }
    buttonsList->clear();
    buttonsCount = 0;
}

QString SLineEdit::buttons()
{
    return buttonsList->join(',');
}

void SLineEdit::tmp_set_buttons_style_sheet(const QString &styleSheet)
{
    int i = 0;

    if (this->buttonsCount)
    {
        for (i = 0; i < this->buttonsCount; i++)
        {
            (this->lineEditButtons[i])->setStyleSheet(styleSheet);
        }
    }
}

void SLineEdit::resize(const QSize &size)
{
    sz.setHeight(size.height());
    sz.setWidth(sizeHint().width());
    buttonSize = {size.height() - frameWidth, size.height() - frameWidth};  // обновляем значение
    QLineEdit::resize( size );
}

void SLineEdit::resize(int w, int h)
{
    resize(QSize(w, h));
}

void SLineEdit::arrangeButtons()
{
    int i = 0;
    buttonSize.setHeight(this->height());
    buttonSize.setWidth(this->height());
    if (this->buttonsCount)
    {
        for (i = 0; i < this->buttonsCount; i++)
        {
            this->lineEditButtons[i]->resize(buttonSize.width(), buttonSize.height());
            (this->lineEditButtons[i])->move( rect().right() + 1 - buttonSize.width()*(this->buttonsCount - i),
                                               (rect().height() - buttonSize.width())/2 );
        }
    }
    setTextMargins(0,0, (this->height() + frameWidth)*buttonsCount, 0) ; // лучше задавать отступ справа так, а не с пом. setStyleSheet, т. к. в вышестоящей функции может потребоваться изменить внешний вид
}

void SLineEdit::resizeEvent(QResizeEvent *)
{
    arrangeButtons();
}

void SLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit mouseDoubleClick();
    QLineEdit::mouseDoubleClickEvent(e);
}

SLineEdit::~SLineEdit()
{
    for(int i = lineEditButtons.size() - 1; i>=0; i--)
    {
        delete lineEditButtons[i];
    }
    if(buttonsCount)
    {
        delete buttonsList;
    }
    QObject::disconnect(signalMapper, SIGNAL(mappedInt(int)), this, SIGNAL(buttonClicked(int)));
    delete signalMapper;
}
