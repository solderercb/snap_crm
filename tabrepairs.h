#ifndef TABREPAIRS_H
#define TABREPAIRS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QTableWidget>
#include <QLabel>
#include <QSqlQueryModel>
#include <QRegularExpressionValidator>
#include <QFrame>
#include <QWidgetAction>
#include <QTimer>
#include "tabcommon.h"
#include "models/stablerepairsmodel.h"
#include "models/repairtablefiltermenu.h"
#include "widgets/stableviewbase.h"
#include "widgets/sdialogissuerepair.h"

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
    QMap<QString, int> *filterSettings;
    static tabRepairs* p_instance[2];
    bool m_tabType;
    QTableView* tableView;
    STableRepairsModel* repairs_table;
    QSqlQueryModel* cartridges_table = nullptr;
    QStringList query_group;
    QTimer *tableUpdateDelay;
    SDialogIssueRepair *m_dialogIssue;
    QMenu *tableRepairsMenu = nullptr;
    QMenu *tableCartridgesMenu = nullptr;
    void initTableRepairsMenu();
    void initTableCartridgesMenu();
#ifdef QT_DEBUG
    void randomFill() override{};
#endif
public slots:
    void autorefreshTable();
    void refreshTable(bool preserveScrollPos = STableViewBase::ScrollPosPreserve, bool preserveSelection = STableViewBase::SelectionReset);
private slots:
    void tableItemDoubleClick(QModelIndex);
    void tableItemClick(QModelIndex index);
    void lineEditSearchTextChanged(QString);
    void lineEditSearchReturnPressed();
    void tableModeChanged(bool mode);
    void filterMenuClosed();
    void tableLayoutChanged(int,int,int);
    void buttonReceptClicked();
    void buttonRefillClicked();
    void buttonIssueClicked();
    void buttonRefreshClicked();
    void buttonPrintClicked();
    void tableSelectionChanged(const QItemSelection &, const QItemSelection &);
    void createDialogIssue();
    void closeDialogIssue();
    void menuRequest(QPoint pos);
    void printCartridgeReceiptReport();
    void printCartridgeWorksReport();
#ifdef QT_DEBUG
    void test_scheduler_handler() override{};
    void test_scheduler2_handler() override{};
#endif
};

#endif // TABREPAIRS_H
