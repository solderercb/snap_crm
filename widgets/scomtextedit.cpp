#include "scomtextedit.h"
#include "stextedit.h"
#include "splaintextedit.h"

SComTextEdit::SComTextEdit(QWidget *derived) :
    q_ptr(derived)
{
    baseAbstractScrollArea = dynamic_cast<QAbstractScrollArea*>(q_ptr);
    textBufferForHeightCalc = new QTextDocument();
    textBufferFontMetrics = new QFontMetrics(textBufferForHeightCalc->defaultFont());
    textBufferForHeightCalc->setDocumentMargin(0);
    widgetFontMetrics = new QFontMetrics(derived->font());
}

SComTextEdit::~SComTextEdit()
{
    delete textBufferForHeightCalc;
    delete widgetFontMetrics;
    delete textBufferFontMetrics;
}

QSize SComTextEdit::sizeHint() const
{
//    qDebug().nospace() << "[" << this << "] SComTextEdit::sizeHint()";
    return QSize(baseAbstractScrollArea->QAbstractScrollArea::sizeHint().width(), m_minimumHeightHint);
}

QSize SComTextEdit::minimumSizeHint() const
{
//    qDebug().nospace() << "[" << this << "] SComTextEdit::minimumSizeHint()";
    return QSize(baseAbstractScrollArea->QAbstractScrollArea::minimumSizeHint().width(), m_minimumHeightHint);
}

void SComTextEdit::updateTextBufferWidth(QResizeEvent *e)
{
    int sbw = baseAbstractScrollArea->verticalScrollBar()->width();
    float heightChange = 0;
    // При вставке текста, часть из которого будет перенесена но новую строку, или при изменении ширины виджета
    // может кратковременно появиться полоса прокрутки. Это появление вызывает лишние пересчеты высоты виджета.
    heightChange = ((float)(e->oldSize().width() - e->size().width())/sbw);
//    qDebug().nospace() << "[" << this << "] SComTextEdit::updateTextBufferWidth() | old size: " << e->oldSize() << "; size: " << e->size() << "; heightChange = " << heightChange << " (sbw = " << sbw << ")";
    if( heightChange != -1 && heightChange != 0 && heightChange != 1 )
    {
//        qDebug().nospace() << "[" << this << "] SComTextEdit::updateTextBufferWidth()";
        textBufferForHeightCalc->setTextWidth(baseAbstractScrollArea->QAbstractScrollArea::width());
        updateHeight();
    }
}

void SComTextEdit::updateHeight()
{
//    qDebug().nospace() << "[" << this << "] SComTextEdit::updateHeight()";
    // TODO: придумать бы более элегантный способ обращаться к методам STextEdit/SPlainTextEdit...
    QString text;
    STextEdit *te = nullptr;
    SPlainTextEdit *pte = nullptr;

    te = dynamic_cast<STextEdit*>(q_ptr);
    pte = dynamic_cast<SPlainTextEdit*>(q_ptr);
    if(te)
        text = te->document()->toPlainText();
    else if(pte)
        text = pte->document()->toPlainText();
    else
        return;

//    qDebug().nospace() << "[" << this << "] SComTextEdit::updateHeight() | *te: " << te << "; *pte: " << pte;

    textBufferForHeightCalc->setTextWidth(baseAbstractScrollArea->QAbstractScrollArea::width() - 2*baseAbstractScrollArea->verticalScrollBar()->width());
    textBufferForHeightCalc->setPlainText(text);
    int textBufferRows = textBufferForHeightCalc->size().toSize().height()/textBufferFontMetrics->height();

    if(te)
    {
        te->setMinimumHeight(textBufferRows);
//        qDebug().nospace() << "[" << baseAbstractScrollArea << "] setFixedHeight()";
//        baseAbstractScrollArea->setFixedHeight(te->m_minimumHeightHint);
        baseAbstractScrollArea->setMinimumHeight(te->m_minimumHeightHint);
    }
    else if(pte)
    {
        pte->setMinimumHeight(textBufferRows);
//        qDebug().nospace() << "[" << baseAbstractScrollArea << "] setFixedHeight()";
//        baseAbstractScrollArea->setFixedHeight(pte->m_minimumHeightHint);
        baseAbstractScrollArea->setMinimumHeight(pte->m_minimumHeightHint);
    }

}

