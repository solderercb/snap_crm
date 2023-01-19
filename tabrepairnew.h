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
#include "models/srepairstatuslog.h"
#include "models/ssortfilterproxymodel.h"
#include "widgets/sphones.h"
#include "widgets/ssetcombobox.h"
#ifdef QT_DEBUG
#include <QTest>
#endif

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
    SRepairModel *repairModel;
    SSortFilterProxyModel *paymentSystemsProxyModel;
    SCashRegisterModel *cashRegister;
    SCommentModel *comment;
    SSqlQueryModel* deviceClassesModel;
    SSqlQueryModel* deviceVendorsModel;
    SSqlQueryModel* devicesModel;
    QSqlQueryModel* classProblemsModel;
    QSqlQueryModel* classIncomingSetsModel;
    QSqlQueryModel* classExteriorsModel;
    SClientModel *clientModel;
    SFieldsModel *additionalFields;
    QStandardItemModel* prepayReasonsModel;
    int m_client = 0;
    int m_prevRepair = 0;
    void setDefaultStyleSheets();
    void getDevices();
    bool checkInput();
    void createClient();
    int deviceId();
    int createDeviceModel();
    void print(int);
    void initDataModels();
    void initWidgets();
    void clearWidgets();
    void getPrepayment(double summ);
    void saveInternalComment();
    void setModelData();
#ifdef QT_DEBUG
    void randomFill() override;
#endif
public slots:
    void fillDeviceCreds(int);
private slots:
    void changeClientType();
    void showLineEditPrevRepair();
    void enablePrepayWidgets(bool);
    void changeDeviceClass(int);
    void changeDeviceVendor(int);
    void clearClientCreds(bool hideCoincidence = true);
    void lineEditPrevRepairButtonsHandler(int);
    void fillClientCreds(int);
    void buttonSelectExistingClientHandler();
    void buttonCreateTabClientHandler();
    void lineEditSNClearHandler(int);
    bool createRepair();
    void createRepairClose();
    void primaryPhoneEdited(QString);
    void preferredPaymentSystemChanged(int);
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
    void dbgRandomFillClicked();
#endif
};

#endif // TABREPAIRNEW_H
