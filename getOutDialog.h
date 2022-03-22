#ifndef GETOUTDIALOG_H
#define GETOUTDIALOG_H

#include <QWidget>
#include "shortlivednotification.h"

namespace Ui {
class getOutDialog;
}

class getOutDialog : public QWidget
{
    Q_OBJECT

signals:
    void close();
public:
    explicit getOutDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~getOutDialog();

private:
    Ui::getOutDialog *ui;
private slots:
    void getOut();
    void cancel();
    void createLooseDoc();
};

#endif // GETOUTDIALOG_H
