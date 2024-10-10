#ifndef STEXTEDIT_H
#define STEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QDebug>
#include <QScrollBar>
#include "widgets/scomtextedit.h"

class STextEdit : public QTextEdit, public SComTextEdit
{
    Q_OBJECT
    friend class SComTextEdit;
public:
    explicit STextEdit(QWidget *parent = nullptr);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void setFixedHeight(int);
    void setMinimumHeight(const int rows);
protected:
private:
    int m_minimumHeightHint = 21;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *e) override;

signals:
    void editFinished();
public slots:
    void updateHeight();
};

#endif // STEXTEDIT_H
