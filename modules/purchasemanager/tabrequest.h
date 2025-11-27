#ifndef TABPARTREQUEST_H
#define TABPARTREQUEST_H

#include <tabCommon>
#include <QObject>

class SPartRequest;
class SClientModel;

namespace Ui {
class tabPartRequest;
}

class tabPartRequest : public tabCommon
{
    Q_OBJECT
public:
    enum EndCommitOp {SwitchToViewMode, PrepareRepeat, UpdateWidgets};
    static tabPartRequest* getInstance(int id, MainWindow *parent = nullptr);
    explicit tabPartRequest(int id = 0, MainWindow *parent = nullptr);
    ~tabPartRequest();
    QString tabTitle() override;
    bool tabCloseRequest() override;
    void copyRequest(const int id);
    void setRepair(const int id);
    void setClient(const int id);
private:
    Ui::tabPartRequest *ui;
    static QMap<int, tabPartRequest*> p_instance;
    int m_initialRequestId = 0;
    int m_requestId = 0;
    SPartRequest *m_partRequest = nullptr;
    int m_dealer = 0;
    int m_client = 0;
    int m_repair = 0;
    SClientModel *m_dealerModel = nullptr;
    SClientModel *m_clientModel = nullptr;
    int m_dealerRO = false;
    int m_clientRO = false;
    bool m_requestRO = false;
    bool m_deadlineVisible = 1;
    bool m_endDateVisible = 0;
    bool m_commentsVisible = 0;
    bool m_newRequest = 1;
    int m_endCommitOp = 0;
    void load(const int id);
    void setDefaultStyleSheets();
    void initWidgets();
    void updateWidgets();
    int checkInput() override;
    void throwHandler(int) override;
    void beginCommit() override;
    void commit(const int stage = 0) override;
    void endCommit() override;
    void prepareForRepeatedOp();
    void switchTabToViewMode();
    bool isManagersChanged();
    void setModelData();
    void guiFontChanged() override;
    void stateHandler(const int state);
    void updateTabPtr(const int oldId, const int newId);
#ifdef QT_DEBUG
    void randomFill() override;
#endif
private slots:
    void create();
    void createAndRepeat();
    void createCopyTab();
    void save();
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override {};
#endif
};

#endif // TABPARTREQUEST_H
