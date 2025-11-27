#ifndef TABREPAIR_H
#define TABREPAIR_H

#include <tabCommon>

class QWidget;
class MainWindow;
class SRepairModel;
class SClientModel;
class SFieldsModel;
class SSortFilterProxyModel;
class SSaleTableModel;
class SDialogIssueRepair;

namespace Ui {
class tabRepair;
}

class tabRepair : public tabCommon
{
    Q_OBJECT

    friend class SDialogIssueRepair;

signals:
    void worksTreeDoubleClicked(int);
    void createTabPrevRepair(int);
    void generatePrintout(QMap<QString,QVariant>);
    void createTabClient(int);
    void createTabSparePart(int);
    void createTabSparePartReserve(int);
    void createTabSelectItem(int, QWidget*);

public:
    explicit tabRepair(int rep_id, MainWindow *parent = nullptr);
    static tabRepair* getInstance(int rep_id, MainWindow *parent = nullptr);
    ~tabRepair();
    QString tabTitle() override;
    bool tabCloseRequest() override;
    static void refreshIfTabExists(const int repairId);
private:
    enum OpType {SaveState, SaveInformedState, SaveDiagAmount, SaveBOQ, SavePlace, SaveBeforeClose};
    Ui::tabRepair *ui;
    static QMap<int, tabRepair*> p_instance;
    int repair_id;
    SRepairModel *repairModel;
    SClientModel *clientModel;
    SFieldsModel *additionalFieldsModel;
    SSortFilterProxyModel *statusesProxyModel = nullptr;
    SSaleTableModel *m_BOQModel;    // bill of quantities
    bool modelRO = 0;   // признак блокировки карты ремонта
    SDialogIssueRepair *m_dialogIssue;
    bool m_BOQModelRO = 1;
    bool m_diagRO = 1;
    bool m_summRO = 1;
    bool m_getOutButtonVisible = 0;
    bool m_comboBoxStateEnabled = 1;
    bool m_comboBoxNotifyStatusEnabled = 1;
    bool m_outDateVisible = 0;
    bool m_buttonDebtReceivedVisible = 0;
    int m_clientId = 0;
    QList<QWidget*> additionalFieldsWidgets;
    bool m_autosaveDiag = 0;
    QTimer *m_autosaveDiagTimer;
    bool m_diagChanged = 0;
    bool m_spinBoxAmountChanged = 0;
    bool m_buttonSaveStateEnabled = 0;
    QTimer *m_repairLockUpdateTimer = nullptr;
    OpType m_opType;
    void fillExtraInfo();
    void setLock(bool state = 1);
    void createAdditionalFieldsWidgets();
    void delAdditionalFieldsWidgets();
    void setInfoWidgetVisible(QWidget *w, bool state = 1);
    void setWidgetsParams(const int);
    void updateStatesModel();
    void updateComboBoxInformedStatus();
    void updateComboBoxPlace();
    void doStateActions(const int);
    void setPricesToZero();
    void initEngineer();
    void checkViewPermission();
    void loadData();
    void reloadRequestsList();
    void saveState(int);
    bool checkStateAcl(const int);
    bool checkBeforeStateChange();
    bool checkBeforeInformedStatusChange();
    bool checkBeforePlaceChange();
    int checkInput() override;
    void setModelDiagAmount();
    void doStateActionsBeforeClose();
    void beginCommit() override;
    bool skip(const int) override;
    int commitStages() override;
    void commit(const int) override;
    void throwHandler(int) override;
    void endCommit() override;
    void diagAmountSaved();
#ifdef QT_DEBUG
    void randomFill() override {};
#endif
private slots:
    void reloadData();
    void updateWidgets();
    void saveState();
    void endStateChange();
    void comboBoxStateIndexChanged(int);
    void createDialogIssue();
    void openPrevRepair();
    void printStickers(int);
    void changeOffice(int);
    void changeManager(int);
    void changeEngineer(int);
    void openInvoice(int);
    void editIncomingSet(int);
    void setAgreedAmount(int);
    void buttonClientClicked();
    void setInformedStatus(int);
    void diagChanged();
    void diagEditFinished();
    void spinBoxAmountChanged(double);
    void spinBoxAmountEditingFinished();
    void saveDiagAmount();
    void autosaveDiagAmount();
    void savePlace(int index);
    void comboBoxPlaceButtonClickHandler(int id);
    void guiFontChanged() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override{};
    void test_scheduler2_handler() override{};
#endif
};

#endif // TABREPAIR_H
