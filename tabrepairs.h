#ifndef TABREPAIRS_H
#define TABREPAIRS_H

#include <tabCommon>
#include <SRepairsViewFilterMenu>

class QWidget;
class MainWindow;
class STableRepairsModel;
class SDialogIssueRepair;
class SRepairModel;

namespace Ui {
class tabRepairs;
}

class tabRepairs : public tabCommon
{
    Q_OBJECT

signals:
    void doubleClickRepair(int);
    void cartridgesRefill(QList<int> *);
    void receptRepair();
    void receptCartridges();
public:
    enum Type{Workshop = 0, SelectRepair};
    explicit tabRepairs(bool type = 0, MainWindow *parent = nullptr);
    static tabRepairs* getInstance(bool type, MainWindow *parent = nullptr);
    ~tabRepairs();
    QString tabTitle() override;
    void updateWidgets();
    static void refreshIfTabExists();
private:
    enum WorkshopFilter{NoFilter = 0, CartridgesOnly = 1, RepairsOnly = 2};
//    enum MenuRepairsActions{};
    enum MenuCartridgesActions{PrintReceipt = 0, PrintIssue};
    Ui::tabRepairs *ui;
    SRepairsTableFilterMenu::Filter *tableFilter;
    static tabRepairs* p_instance[2];
    bool m_tabType = 0;
    QTableView* tableView;
    STableRepairsModel* repairs_table;
    STableRepairsModel* cartridges_table = nullptr;
    QStringList query_group;
    SDialogIssueRepair *m_dialogIssue;
    QMenu *tableRepairsMenu = nullptr;
    QMenu *tableCartridgesMenu = nullptr;
    void initTableRepairsMenu();
    void initTableCartridgesMenu();
    QList<SRepairModel*> repairsListFromSelection();
    bool event(QEvent *event) override;
    void constructQueryClause();
#ifdef QT_DEBUG
    void randomFill() override{};
#endif
public slots:
    void refreshTable(bool preserveScrollPos, bool preserveSelection) override;
    void setFocusSearchField();
private slots:
    void tableItemDoubleClick(QModelIndex);
    void tableItemClick(QModelIndex index);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableModeChanged(bool mode);
    void filterMenuClosed();
    void buttonReceptClicked();
    void buttonRefillClicked();
    void buttonIssueClicked();
    void buttonRefreshClicked();
    void buttonPrintClicked();
    void tableSelectionChanged(const QItemSelection &, const QItemSelection &);
    void createDialogIssue();
    void menuRequest(QPoint pos);
    void printCartridgeReceiptReports();
    void printCartridgeWorksReports();
#ifdef QT_DEBUG
    void test_scheduler_handler() override{};
    void test_scheduler2_handler() override{};
#endif
};

#endif // TABREPAIRS_H
