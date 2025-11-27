#ifndef SPLAINTEXTEDIT_H
#define SPLAINTEXTEDIT_H

#include <QObject>
#include <QPlainTextEdit>
#include <SComTextEdit>

class QWidget;
class QFocusEvent;
class QResizeEvent;

class SPlainTextEdit : public QPlainTextEdit, public SComTextEdit
{
    Q_OBJECT
    friend class SComTextEdit;
signals:
    void keyPress(int key);
public:
    enum PressedKey{Undef = 0, Enter = 1, ShiftEnter, Up, Escape};
    explicit SPlainTextEdit(QWidget *parent = nullptr);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void setFixedHeight(int);
    void enableExtKeyPressHandler(bool);
    bool isEmpty();
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    int m_minimumHeightHint = 21;
    bool m_extKeyPressHandler = 0;
    void focusOutEvent(QFocusEvent *event) override;
    void setMinimumHeight(const int rows);
    void resizeEvent(QResizeEvent *e) override;

signals:
    void editFinished();
public slots:
    void updateHeight();
};

#endif // SPLAINTEXTEDIT_H
