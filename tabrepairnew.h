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
    int exist_client_id;

private slots:
    void changeClientType();
    void showLineEditPrevRepair();
    void changeDeviceType();
    void changeDeviceMaker();
    void clearClientCreds();
    void lineEditPrevRepairButtonsHandler(int);
    void setPrevRepair(int);
    void fillClientCreds(int);
    void buttonSelectExistingClientHandler();
    void findMatchingCllient();

};

#endif // TABREPAIRNEW_H
