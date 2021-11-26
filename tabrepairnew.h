#ifndef TABREPAIRNEW_H
#define TABREPAIRNEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include "dbconnection.h"

namespace Ui {
class tabRepairNew;
}

class tabRepairNew : public QWidget
{
    Q_OBJECT

public:
    explicit tabRepairNew(QWidget *parent = nullptr);
    ~tabRepairNew();

private:
    Ui::tabRepairNew *ui;
    void getDevices();
    DBConnection* dbConnection;
    QStandardItemModel* comboboxDevicesModel;

private slots:
    void changeClientType();
    void enableLineEditPrevRepair();

};

#endif // TABREPAIRNEW_H
