/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#include "lineedit.h"

int buttonsCount;

myLineEdit::myLineEdit(QWidget *parent, QString* buttons)
    : QLineEdit(parent)
{
    QStringList buttonsList;
    QStringList allowedButtonsList = { "Clear", "DownArrow", "Edit", "Search" };

    if (buttons)
    {
        signalMapper = new QSignalMapper(this);
        QObject::connect(signalMapper, SIGNAL(mappedInt(int)), this, SLOT(slotButtonClicked(int)));
        qDebug() << "I'm here 1";
        buttonsList = buttons->split(',');
        for (int i = 0; i < buttonsList.size(); i++)
        {
            if (!allowedButtonsList.contains(buttonsList.value(i)))
            {
                qDebug() << "User defined button \"" << buttonsList.value(i) << "\" not found in allowed buttons list and will be ignored";
                buttonsList.removeAt(i);
            }

        }
        buttonsCount = buttonsList.size();
//        buttonsCount = 2;
        lineEditButtons.resize(buttonsCount);

//        lineEditButton_ = new QToolButton(this);
//        lineEditButton_->setIconSize(QSize(16,16));
//        lineEditButton_->setCursor(Qt::ArrowCursor);
//        lineEditButton_->setStyleSheet("QToolButton { border: none; padding: 0px; }");
//        lineEditButton_->setStyleSheet("::hover { border: 1px solid #5798C6; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"); // LOL работает
//        signalMapper->setMapping(lineEditButton_, 0);
//        QObject::connect(lineEditButton_, SIGNAL(clicked()), signalMapper, SLOT(map()));

        int i = 0;
        for (i = 0; i < buttonsCount; i++)
        {
            lineEditButtons[i] = new QToolButton(this);
            qDebug() << "I'm here 2, iterations!";
            qDebug() << "lineEditButton[" << i << "] = " << &(lineEditButtons[i]) << ", lineEditButton[" << i << "] = " << lineEditButtons[i];
            lineEditButtons[i]->setIconSize(QSize(16,16));
            lineEditButtons[i]->setCursor(Qt::ArrowCursor);
            lineEditButtons[i]->setStyleSheet("QToolButton { border: none; padding: 0px; }");
            lineEditButtons[i]->setStyleSheet("::hover { border: 1px solid #5798C6; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"); // LOL работает
            signalMapper->setMapping(lineEditButtons[i], i);
            QObject::connect(lineEditButtons[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
//            QObject::connect(lineEditButtons[i], SIGNAL(clicked()), this, SLOT(slotButtonClicked(i)));

            if (buttonsList.value(i) == "Clear")
            {
                lineEditButtons[i]->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
            }
            else if (buttonsList.value(i) == "DownArrow")
            {
                lineEditButtons[i]->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));
            }
            else if (buttonsList.value(i) == "Edit")
            {
                QPixmap pixmap("pencil.png");
                lineEditButtons[i]->setIcon(QIcon(pixmap));
            }
            else if (buttonsList.value(i) == "Search")
            {
                QPixmap pixmap("magnifier.png");
                lineEditButtons[i]->setIcon(QIcon(pixmap));
            }

        }

        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(lineEditButtons[0]->sizeHint().width() + frameWidth + 1));
        QSize msz = minimumSizeHint();
        setMinimumSize(qMax(msz.width(), lineEditButtons[0]->sizeHint().height() + frameWidth * 2 + 2),
                       qMax(msz.height(), lineEditButtons[0]->sizeHint().height() + frameWidth * 2 + 2));

//        lineEditButton_->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
//        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
//        setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(lineEditButton_->sizeHint().width() + frameWidth + 1));
//        QSize msz = minimumSizeHint();
//        setMinimumSize(qMax(msz.width(), lineEditButton_->sizeHint().height() + frameWidth * 2 + 2),
//                       qMax(msz.height(), lineEditButton_->sizeHint().height() + frameWidth * 2 + 2));

    }

}

void myLineEdit::resizeEvent(QResizeEvent *)
{
    int i = 0;
    qDebug() << "buttonsCount=" << this->buttonsCount;
    if (this->buttonsCount)
    {
        qDebug() << "I'm here 3";
        QSize sz = this->lineEditButtons[i]->sizeHint();
        qDebug() << "I'm here 5";
        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
        for (i = 0; i < this->buttonsCount; i++)
        {
            qDebug() << "I'm here 6, iterations!";
            qDebug() << "lineEditButton[" << i << "] = " << &(this->lineEditButtons[i]) << ", lineEditButton[" << i << "] = " << this->lineEditButtons[i];
            (this->lineEditButtons[i])->move(rect().right() - frameWidth - sz.width()*(this->buttonsCount - i),
                              (rect().bottom() + 1 - sz.height())/2);
        }
    }
//    QSize sz = this->lineEditButton_->sizeHint();
//    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
//        (this->lineEditButton_)->move(rect().right() - frameWidth - sz.width(),
//                          (rect().bottom() + 1 - sz.height())/2);

}

void myLineEdit::updateCloseButton(const QString& text)
{
//    if (lineEditButton[0])
//    {
//        qDebug() << "I'm here 4";

//        for (int i = 0; i < buttonsList.size(); i++)
//        {
//            lineEditButtons[i]->setVisible(!text.isEmpty());
//        }
//    }
}

void myLineEdit::slotButtonClicked(int buttonId)
{
    qDebug() << "slotButtonClicked: buttonId=" << buttonId;
    emit this->buttonClicked(buttonId);
}

