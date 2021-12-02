#ifndef TABREPAIRNEW_H
#define TABREPAIRNEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include "dbconnection.h"

namespace Ui {
class tabRepairNew;
}

class tabRepairNew : public QWidget
{
    Q_OBJECT

public:
    explicit tabRepairNew(QWidget *parent = nullptr);
    static tabRepairNew* getInstance(QWidget *parent = nullptr);
    ~tabRepairNew();

private:
    Ui::tabRepairNew *ui;
    static tabRepairNew* p_instance;
    void getDevices();
    DBConnection* dbConnection;
    QStandardItemModel* comboboxDevicesModel;
    QStandardItemModel* comboboxDeviceMakersModel;
    QStandardItemModel* comboboxDeviceModelsModel;

private slots:
    void changeClientType();
    void showLineEditPrevRepair();
    void changeDeviceType();
    void changeDeviceMaker();
    void clearClientCreds();

};

#endif // TABREPAIRNEW_H
