/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**        
****************************************************************************/

#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QStringList>
#include <QToolButton>
#include <QStyle>
#include <QVector>
#include <QSignalMapper>

class QToolButton;

class myLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    myLineEdit(QWidget *parent = 0, QString* buttons = nullptr);

signals:
    void buttonClicked(int buttonId = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void updateCloseButton(const QString &text);
    void slotButtonClicked(int buttonId);

private:
    QStringList buttonsList;
    int buttonsCount;
    QVector<QToolButton*> lineEditButtons;
    QToolButton* lineEditButton_;
    QSignalMapper *signalMapper;
};

#endif // LIENEDIT_H
