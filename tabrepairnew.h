#ifndef TABREPAIRNEW_H
#define TABREPAIRNEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateEdit>
#include "mainwindow.h"
#include "tabcommon.h"
#include "models/sclientmodel.h"
#include "models/sfieldsmodel.h"
#include "models/sdevmdlmodel.h"
#include "models/srepairmodel.h"
#include "models/scommentmodel.h"
#include "widgets/sgroupboxeventfilter.h"
#include "widgets/sphones.h"

namespace Ui {
    class tabRepairNew;
}

class tabRepairNew : public tabCommon
{
    Q_OBJECT

signals:
    void createTabSelectPrevRepair(int, QWidget *);
    void createTabSelectExistingClient(int, QWidget *);
    void createTabClient(int);
    void generatePrintout(QMap<QString, QVariant>);

public:
    explicit tabRepairNew(MainWindow *parent = nullptr);
    static tabRepairNew* getInstance(MainWindow *parent = nullptr);
    ~tabRepairNew();
    virtual bool tabCloseRequest();
    QString tabTitle() override;
private:
    Ui::tabRepairNew *ui;
    static tabRepairNew* p_instance;
    MainWindow *parent;
    SRepairModel *repairModel;
    SCashRegisterModel *cashRegister;
    SCommentModel *comment;
    void setDefaultStyleSheets();
    void getDevices();
//    QStandardItemModel* deviceClassesModel;
    SSqlQueryModel* deviceClassesModel;
    SSqlQueryModel* deviceVendorsModel;
    SSqlQueryModel* devicesModel;
    QSqlQueryModel* classProblemsModel;
    QSqlQueryModel* classIncomingSetsModel;
    QSqlQueryModel* classExteriorsModel;
    SClientModel *clientModel;
    SFieldsModel *additionalFields;
    QSqlQueryModel* queryDevice;
    QStandardItemModel* prepayReasonsModel;
    QList<QStandardItem*> *prepayReason;
    QSqlQueryModel* clientsMatchTable;
    QSqlQueryModel* devicesMatchTable;
    int client = 0;
    bool checkInput();
    QMessageBox msgBox;
    SGroupBoxEventFilter *groupBoxEventFilter;
    bool createClient();
#ifdef QT_DEBUG
    void randomFill();
#endif

private slots:
    void changeClientType();
    void showLineEditPrevRepair();
    void enablePrepayWidgets(bool);
    void changeDeviceClass(int);
    void changeDeviceVendor(int);
    void clearClientCreds(bool hideCoincidence = true);
    void lineEditPrevRepairButtonsHandler(int);
    void fillClientCreds(int);
    void fillDeviceCreds(int);
    void buttonSelectExistingClientHandler();
    void buttonCreateTabClientHandler();
    void findMatchingDevice(QString);
    void deviceMatchTableDoubleClicked(QModelIndex);
    void clientMatchTableDoubleClicked(QModelIndex);
    void lineEditSNClearHandler(int);
    bool createRepair();
    void createRepairClose();
    void primaryPhoneEdited(QString);
    void preferredPaymentSystemChanged(int);
    void prepayPaymentSystemChanged(int);
#ifdef QT_DEBUG
    void test_scheduler_handler();
    void test_scheduler2_handler();
#endif
};

#endif // TABREPAIRNEW_H
