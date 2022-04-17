#ifndef SLINEEDIT_H
#define SLINEEDIT_H

#include <QLineEdit>
#include <QStringList>
#include <QToolButton>
#include <QStyle>
#include <QVector>
#include <QSignalMapper>

class SLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString buttons READ buttons WRITE setButtons)

public:
    SLineEdit(QWidget *parent = nullptr);
    void setButtons(QString buttons);
    void resize(const QSize &);
    void resize(int, int);
    QString buttons();
    void tmp_set_buttons_style_sheet(const QString&);
    ~SLineEdit();

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
    int frameWidth;
    QSize sz;
    QSize buttonSize;
};


#endif // SLINEEDIT_H
