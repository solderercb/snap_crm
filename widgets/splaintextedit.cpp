#include "splaintextedit.h"
#include <QWidget>
#include <QDebug>

SPlainTextEdit::SPlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    SComTextEdit(this)
{
    connect(this, &QPlainTextEdit::textChanged, this, &SPlainTextEdit::updateHeight);
//    qDebug().nospace() << "[" << this << "] SPlainTextEdit";
}

bool SPlainTextEdit::isEmpty()
{
    return document()->isEmpty();
}

void SPlainTextEdit::enableExtKeyPressHandler(bool state)
{
    m_extKeyPressHandler = state;
}

void SPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    int key = PressedKey::Undef;
    if(m_extKeyPressHandler)
    {
//        qDebug().nospace() << "[" << this << "] lineEditKeyPressEvent() | key: " << (Qt::Key)event->key() << "; modifiers: " << event->modifiers();
        if( ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && event->modifiers() == Qt::NoModifier) ||
            (event->key() == Qt::Key_Enter && event->modifiers() == Qt::KeypadModifier) )
            key = PressedKey::Enter;
        else if( event->key() == Qt::Key_Enter && event->modifiers() == Qt::KeyboardModifier::ShiftModifier )
            key = PressedKey::ShiftEnter;
        else if(event->key() == Qt::Key_Escape)
            key = PressedKey::Escape;
        else if(event->key() == Qt::Key_Up && isEmpty())
            key = PressedKey::Up;
    }
    if(key)
    {
        emit keyPress(key);
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}

QSize SPlainTextEdit::sizeHint() const
{
//    qDebug().nospace() << "[" << this << "] sizeHint()";
    return SComTextEdit::sizeHint();
}

QSize SPlainTextEdit::minimumSizeHint() const
{
//    qDebug().nospace() << "[" << this << "] minimumSizeHint()";
    return SComTextEdit::minimumSizeHint();
}

void SPlainTextEdit::setFixedHeight(int height)
{
//    qDebug().nospace() << "[" << this << "] setFixedHeight()";
    QPlainTextEdit::setFixedHeight(height);
}

void SPlainTextEdit::resizeEvent(QResizeEvent *e)
{
//    qDebug().nospace() << "[" << this << "] resizeEvent()";
    SComTextEdit::updateTextBufferWidth(e);
    QPlainTextEdit::resizeEvent(e);
}

void SPlainTextEdit::focusOutEvent(QFocusEvent *event)
{
    emit editFinished();
    QPlainTextEdit::focusOutEvent(event);
}

void SPlainTextEdit::setMinimumHeight(const int rows)
{
    m_minimumHeightHint = qMax(1, qMin(rows, 4))*widgetFontMetrics->height() + 8;
//    qDebug().nospace() << "[" << this << "] setMinimumHeight() | m_minimumHeightHint = " << m_minimumHeightHint;
}

void SPlainTextEdit::updateHeight()
{
//    qDebug().nospace() << "[" << this << "] updateHeight()";
    SComTextEdit::updateHeight();
}
