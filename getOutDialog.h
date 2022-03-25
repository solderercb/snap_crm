#ifndef GETOUTDIALOG_H
#define GETOUTDIALOG_H

#include <QWidget>
#include <QLocale>
#include "global.h"
#include "shortlivednotification.h"
#include "amountToWords.h"
#define OTHER_REJECT_REASON_ID 5

namespace Ui {
class getOutDialog;
}

class getOutDialog : public QWidget
{
    Q_OBJECT

signals:
    void close();
    void getOutOk();
public:
    explicit getOutDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~getOutDialog();

private:
    Ui::getOutDialog *ui;
    void checkSums();
    bool summsNotEq = 0;
    bool balanceNotEnough = 1;
    QWidget *parent;
    int state, newState;
    void setDefaultStyleSheets();

private slots:
    void getOut();
    void cancel();
    void createLooseDoc();
    void enableComboBoxPaymentAccount(int);
    void otheRejectReasonShow(QString);
    void textEditTextChanged();
};

#endif // GETOUTDIALOG_H
