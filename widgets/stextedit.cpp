#include "stextedit.h"

STextEdit::STextEdit(QWidget *parent) :
    QTextEdit(parent),
    SComTextEdit(this)
{
    connect(this, &QTextEdit::textChanged, this, &STextEdit::updateHeight);
}

QSize STextEdit::sizeHint() const
{
//    qDebug().nospace() << "[" << this << "] sizeHint()";
    return SComTextEdit::sizeHint();
}

QSize STextEdit::minimumSizeHint() const
{
//    qDebug().nospace() << "[" << this << "] minimumSizeHint()";
    return SComTextEdit::minimumSizeHint();
}

void STextEdit::setFixedHeight(int height)
{
//    qDebug().nospace() << "[" << this << "] setFixedHeight()";
    QTextEdit::setFixedHeight(height);
}

void STextEdit::resizeEvent(QResizeEvent *e)
{
    SComTextEdit::updateTextBufferWidth(e);
    QTextEdit::resizeEvent(e);
}

void STextEdit::focusOutEvent(QFocusEvent *event)
{
    emit editFinished();
    QTextEdit::focusOutEvent(event);
}

void STextEdit::setMinimumHeight(const int rows)
{
    m_minimumHeightHint = qMax(3, qMin(rows, 8))*widgetFontMetrics->height() + 8;
//    qDebug().nospace() << "[" << this << "] setMinimumHeight() | m_minimumHeightHint = " << m_minimumHeightHint;
}

void STextEdit::updateHeight()
{
//    qDebug().nospace() << "[" << this << "] updateHeight()";
    SComTextEdit::updateHeight();
}
