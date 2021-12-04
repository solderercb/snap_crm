#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QStringList>
#include <QToolButton>
#include <QStyle>
#include <QVector>
#include <QSignalMapper>

namespace Ui {
class myLineEdit;
}

class myLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString buttons READ buttons WRITE setButtons)

public:
    myLineEdit(QWidget *parent = nullptr);
    void setButtons(QString buttons);
    QString buttons();
    ~myLineEdit();

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
//    Ui::myLineEdit *ui;
};


#endif // MYLINEEDIT_H
