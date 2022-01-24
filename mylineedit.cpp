#include "mylineedit.h"

myLineEdit::myLineEdit(QWidget *parent)
    : QLineEdit(parent)
//    ,
//      ui(new Ui::myLineEdit)
{
}

void myLineEdit::setButtons(QString buttons)
{
    QStringList buttonsList;
    QStringList allowedButtonsList = { "Clear", "DownArrow", "Edit", "Search" };
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
            lineEditButtons[i]->setIconSize(QSize(12,12));
            lineEditButtons[i]->setCursor(Qt::ArrowCursor);
            lineEditButtons[i]->setStyleSheet("QToolButton { border: none; padding: 0px; }");
            lineEditButtons[i]->setStyleSheet("::hover { border: 1px solid #5798C6; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"); // LOL работает
            signalMapper->setMapping(lineEditButtons[i], i);
            QObject::connect(lineEditButtons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));

            if (buttonsList.value(i) == "Clear")
            {
                // ✖ или 🗙 или ⌫
                lineEditButtons[i]->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
            }
            else if (buttonsList.value(i) == "DownArrow")
            {
                // ▼
                lineEditButtons[i]->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));
            }
            else if (buttonsList.value(i) == "Edit")
            {
                // 🖉
                QPixmap pixmap("pencil.png");
                lineEditButtons[i]->setIcon(QIcon(pixmap));
            }
            else if (buttonsList.value(i) == "Search")
            {
                // 🔍
                QPixmap pixmap("magnifier.png");
                lineEditButtons[i]->setIcon(QIcon(pixmap));
            }

        }

        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(lineEditButtons[0]->sizeHint().width() + frameWidth + 1));
        QSize msz = minimumSizeHint();
        setMinimumSize(qMax(msz.width(), lineEditButtons[0]->sizeHint().height() + frameWidth * 2 + 2),
                       qMax(msz.height(), lineEditButtons[0]->sizeHint().height() + frameWidth * 2 + 2));
    }

}

QString myLineEdit::buttons()
{

}

void myLineEdit::resizeEvent(QResizeEvent *)
{
    int i = 0;
    if (this->buttonsCount)
    {
        QSize sz = this->lineEditButtons[i]->sizeHint();
        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        for (i = 0; i < this->buttonsCount; i++)
        {
            (this->lineEditButtons[i])->move(rect().right() - frameWidth - sz.width()*(this->buttonsCount - i),
                              (rect().bottom() + 1 - sz.height())/2);
        }
    }
}

void myLineEdit::updateCloseButton(const QString& text)
{
//    if (lineEditButton[0])
//    {
//        for (int i = 0; i < buttonsList.size(); i++)
//        {
//            lineEditButtons[i]->setVisible(!text.isEmpty());
//        }
//    }
}

void myLineEdit::slotButtonClicked(int buttonId)
{
    emit this->buttonClicked(buttonId);
}


myLineEdit::~myLineEdit()
{
//    delete ui;
}
