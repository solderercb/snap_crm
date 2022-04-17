#include "slineedit.h"

SLineEdit::SLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth)*2;
}

void SLineEdit::setButtons(QString buttons)
{
    QStringList buttonsList;
    QStringList allowedButtonsList = { "Clear", "DownArrow", "Edit", "Search", "Print", "Apply" };
    if (buttons != nullptr)
    {
        signalMapper = new QSignalMapper(this);
        QObject::connect(signalMapper, SIGNAL(mappedInt(int)), this, SLOT(slotButtonClicked(int)));

        buttonsList = buttons.remove(' ').split(',');
        for (int i = 0; i < buttonsList.size(); i++)
        {
            if (!allowedButtonsList.contains(buttonsList.value(i)))
            {
                buttonsList.removeAt(i);
            }
        }
        buttonsCount = buttonsList.size();
        lineEditButtons.resize(buttonsCount);

        int i = 0;
        for (i = 0; i < buttonsCount; i++)
        {
            lineEditButtons[i] = new QToolButton(this);
//            lineEditButtons[i]->setIconSize(QSize(12,12));
            lineEditButtons[i]->setCursor(Qt::ArrowCursor);
            lineEditButtons[i]->setStyleSheet("QToolButton { border: 0px;}    QToolButton::hover { border: 1px solid #0078D7;  background-color: #E5F1FB;}");
            signalMapper->setMapping(lineEditButtons[i], i);
            QObject::connect(lineEditButtons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));

            if (buttonsList.value(i) == "Clear")
            {
                // ✖ или 🗙 или ⌫
                lineEditButtons[i]->setText("🗙");
            }
            else if (buttonsList.value(i) == "DownArrow")
            {
                // ▼
                lineEditButtons[i]->setText("▼");
            }
            else if (buttonsList.value(i) == "Edit")
            {
                // 🖉
                lineEditButtons[i]->setText("🖉");
            }
            else if (buttonsList.value(i) == "Search")
            {
                // 🔍
                lineEditButtons[i]->setText("🔍");
            }
            else if (buttonsList.value(i) == "Print")
            {
                // 🖶 печать (стикеров)
                lineEditButtons[i]->setText("🖶");
            }
            else if (buttonsList.value(i) == "Apply")
            {
                // 🗸 применить
                lineEditButtons[i]->setText("🗸");
            }

        }

    }

}

QString SLineEdit::buttons()
{

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

void SLineEdit::resizeEvent(QResizeEvent *)
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

void SLineEdit::updateCloseButton(const QString& text)
{
//    if (lineEditButton[0])
//    {
//        for (int i = 0; i < buttonsList.size(); i++)
//        {
//            lineEditButtons[i]->setVisible(!text.isEmpty());
//        }
//    }
}

void SLineEdit::slotButtonClicked(int buttonId)
{
    emit this->buttonClicked(buttonId);
}


SLineEdit::~SLineEdit()
{
}
