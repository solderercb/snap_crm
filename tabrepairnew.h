#ifndef TABREPAIRNEW_H
#define TABREPAIRNEW_H

#include <tabCommon>

class QWidget;
class QStandardItemModel;
class QSqlQueryModel;
class SSqlQueryModel;
class MainWindow;
class SFieldsModel;
class SClientModel;
class SRepairModel;
class SCommentModel;
class SSortFilterProxyModel;
class SCashRegisterModel;

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
    enum EndCommitOp {CloseTab, PrepareRepeat};
    explicit tabRepairNew(MainWindow *parent = nullptr);
    static tabRepairNew* getInstance(MainWindow *parent = nullptr);
    ~tabRepairNew();
    virtual bool tabCloseRequest() override;
    QString tabTitle() override;
private:
    Ui::tabRepairNew *ui;
    static tabRepairNew* p_instance;
    int m_repair = 0;
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
    int m_endCommitOp = CloseTab;
    void setDefaultStyleSheets();
    void getDevices();
    int checkInput() override;
    bool quickRepairCheckInput();
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
    void throwHandler(int) override;
    int commitStages() override;
    void beginCommit() override;
    bool skip(const int) override;
    void commitDevModel();
    void commitClient();
    void commitRepair();
    void commit(const int stage) override;
    void endCommit(const int stage) override;
    void endCommit() override;
    void prepareForRepeatedOp();
    void reloadDevicesModel();
#ifdef QT_DEBUG
    void randomFill() override;
#endif
public slots:
    void fillDeviceCreds(int);
private slots:
    void showLineEditPrevRepair();
    void enablePrepayWidgets(bool);
    void changeDeviceClass(int);
    void changeDeviceVendor(int);
    void lineEditPrevRepairButtonsHandler(int);
    void lineEditSNClearHandler(int);
    void createRepair();
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
