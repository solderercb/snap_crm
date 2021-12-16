#ifndef TABREPAIRNEW_H
#define TABREPAIRNEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QSqlQueryModel>
#include "dbconnection.h"

namespace Ui {
class tabRepairNew;
}

class tabRepairNew : public QWidget
{
    Q_OBJECT

signals:
    void createTabSelectPrevRepair(int);
    void createTabSelectExistingClient(int);

public:
    explicit tabRepairNew(QWidget *parent = nullptr);
    static tabRepairNew* getInstance(QWidget *parent = nullptr);
    ~tabRepairNew();

private:
    Ui::tabRepairNew *ui;
    static tabRepairNew* p_instance;
    void getDevices();
    DBConnection* dbConnection;
//    QStandardItemModel* comboboxDevicesModel;
    QSqlQueryModel* comboboxDevicesModel;
    QSqlQueryModel* comboboxDeviceMakersModel;
    QSqlQueryModel* comboboxDeviceModelsModel;
    QSqlQueryModel* clientModel;
    QSqlQueryModel* clientPhonesModel;
    QSqlQueryModel* clientAdTypesList;
    QStandardItemModel* clientPhoneTypesList;
    QList<QStandardItem*> clientPhoneTypesSelector[2];
    QSqlQueryModel* clientsMatchTable;
    QSqlQueryModel* devicesMatchTable;
    int exist_client_id;
    void phoneTypeChanged(int, int);

private slots:
    void changeClientType();
    void showLineEditPrevRepair();
    void changeDeviceType();
    void changeDeviceMaker();
    void clearClientCreds();
    void lineEditPrevRepairButtonsHandler(int);
    void setPrevRepair(int);
    void fillClientCreds(int);
    void fillDeviceCreds(QModelIndex);
    void buttonSelectExistingClientHandler();
    void findMatchingClient(QString);
    void findMatchingDevice(QString);
    void phone1TypeChanged(int);
    void phone2TypeChanged(int);
    void clientMatchTableDoubleClicked(QModelIndex);
    void lineEditSNClearHandler(int);

};

#endif // TABREPAIRNEW_H
