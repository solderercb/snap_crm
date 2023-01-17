#ifndef SCOMTEXTEDIT_H
#define SCOMTEXTEDIT_H

#include <QAbstractScrollArea>
#include <QTextDocument>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QDebug>

class SComTextEdit
{
public:
    explicit SComTextEdit(QWidget *parent = nullptr);
    ~SComTextEdit();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
protected:
    void updateTextBufferWidth(QResizeEvent *e);
    QFontMetrics *widgetFontMetrics;
    QTextDocument *textBufferForHeightCalc;
    QFontMetrics *textBufferFontMetrics;
    void updateHeight();
private:
    QWidget *q_ptr;
    QAbstractScrollArea *baseAbstractScrollArea = nullptr;
    int m_minimumHeightHint = 21;

};

#endif // SCOMTEXTEDIT_H
