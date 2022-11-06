#ifndef STEXTEDIT_H
#define STEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QDebug>

class STextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit STextEdit(QWidget *parent = nullptr);
private:
    void focusOutEvent(QFocusEvent *event);

signals:
    void editFinished();

};

#endif // STEXTEDIT_H
