#ifndef TABREPAIRCARTRIDGES_H
#define TABREPAIRCARTRIDGES_H
#include "mainwindow.h"
#include "tabcommon.h"
#include "models/srepairmodel.h"
#include "widgets/scartridgeform.h"
#include "widgets/sdialogissuerepair.h"
#include "widgets/scartridgecard.h"

#include <QWidget>

namespace Ui {
class tabRepairCartridges;
}

class tabRepairCartridges : public tabCommon
{
    Q_OBJECT
signals:
    void createTabRepair(const int repairId);
    void createTabClient(const int repairId);
public:
    explicit tabRepairCartridges(MainWindow *parent = nullptr);
    explicit tabRepairCartridges(QList<int> *list, MainWindow *parent = nullptr);
    ~tabRepairCartridges();
    static tabRepairCartridges* getInstance(QList<int> *list, MainWindow *parent = nullptr);
    void loadForms(QList<int> *list);
    virtual bool tabCloseRequest() override;
    QString tabTitle() override;
    bool eventFilter(QObject *, QEvent *) override;
private:
    Ui::tabRepairCartridges *ui;
    static tabRepairCartridges* p_instance;
    bool m_issueButtonVisible = 1;
    bool m_readyButtonVisible = 1;
    SCartridgeCard *m_cartridgeCardForm;
    SDialogIssueRepair *m_dialogIssue;
    void appendToReceptList(SCartridgeForm *form);
    void updateWidgets();
    const QList<SCartridgeForm*> existentForms();
#ifdef QT_DEBUG
    void randomFill() override;
#endif
public slots:
private slots:
    void reloadRepairData();
    void createDialogIssue();
    void createCartridgeCardForm(const int id);
    void closeCartridgeCardForm();
    void reloadCardModel(int id);
    void setReadyToIssue();
    void closeTab();
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
#endif
};

#endif // TABREPAIRCARTRIDGES_H
