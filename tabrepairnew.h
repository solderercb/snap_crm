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
#include "models/sfieldsmodel.h"
#include "models/sdevmdlmodel.h"
#include "models/srepairmodel.h"
#include "models/scommentmodel.h"
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
    void generatePrintout(QMap<QString, QVariant>);

public:
    explicit tabRepairNew(MainWindow *parent = nullptr);
    static tabRepairNew* getInstance(MainWindow *parent = nullptr);
    ~tabRepairNew();
    virtual bool tabCloseRequest() override;
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
    SClientModel *clientModel = nullptr;
    SFieldsModel *additionalFields;
    QStandardItemModel* prepayReasonsModel;
    int m_client = 0;
    int m_prevRepair = 0;
    bool m_closePending = 0;
    bool m_isQuick = 0;
    QString m_checkBoxIsCheckNeededText;
    QList<SRepairModel*> m_quickRepairIssueList;
    bool m_stickersCount = 0;
    void setDefaultStyleSheets();
    void getDevices();
    bool checkInput();
    bool quickRepairCheckInput();
    int deviceId();
    int createDeviceModel();
    void print(int);
    void initDataModels();
    void initWidgets();
    void clearWidgets();
    void getPrepayment(double summ);
    void saveInternalComment();
    void setModelData();
    void updateWidgets();
    void updateWidgetsOnQuickRepairToggled();
    void setQuickRepair(const int state);
#ifdef QT_DEBUG
    void randomFill() override;
#endif
public slots:
    void fillClientCreds(const int);
    void fillDeviceCreds(int);
private slots:
    void showLineEditPrevRepair();
    void enablePrepayWidgets(bool);
    void changeDeviceClass(int);
    void changeDeviceVendor(int);
    void lineEditPrevRepairButtonsHandler(int);
    void lineEditSNClearHandler(int);
    bool createRepair();
    void createRepairClose();
    void issueQuickRepair();
    void preferredPaymentSystemChanged(int);
    void guiFontChanged() override;
    void quickRepairToggled(const int state);
    void updateCheckBoxQuickRepairPrintBOQ();
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
    void dbgRandomFillClicked();
#endif
};

#endif // TABREPAIRNEW_H
