#include "stextedit.h"

STextEdit::STextEdit(QWidget *parent) : QTextEdit(parent)
{
}

void STextEdit::focusOutEvent(QFocusEvent *event)
{
    emit editFinished();
    QTextEdit::focusOutEvent(event);
}
