#ifndef TABPARTSREQUESTSMANAGER_H
#define TABPARTSREQUESTSMANAGER_H

#include "tabcommon.h"
#include "modules/purchasemanager/sgroupingmodel.h"
#include "widgets/speriodselector.h"

namespace Ui {
class tabPurchaseManager;
}

class tabPurchaseManager : public tabCommon
{
    Q_OBJECT
    friend class SPartsRequests;
    friend class STableViewSubgroups;
public:
    static tabPurchaseManager* getInstance(MainWindow *parent = nullptr);
    explicit tabPurchaseManager(MainWindow *parent = nullptr);
    ~tabPurchaseManager();
    QString tabTitle() override;
    bool tabCloseRequest() override;
    static void refreshIfTabExists();
private:
    enum QueryFilter{Repair, Client, Tracking, SupplierUrl, SupplierId};
    enum DirtyFlags{SuppliersModel = 1, RequestsModel = 2};
    int m_office = -1;
    int m_creator = -1;
    int m_manager = -1;
    int m_state = -1;
    int m_priority = -1;
    int m_grouping = -1;
    QModelIndex m_subgroup;
    SStandardItemModel *groupsModel;
    SPartsRequestsGroupingModel *subgroupsModel;
    static tabPurchaseManager* p_instance;
    Ui::tabPurchaseManager *ui;
    QTimer *tableUpdateDelay;
    QString m_excludeStates;
    QString constructSubgroupListQuery(const int &group, const QString &field = QString());
    void constructQueryClause(FilterList &filter, const int &group = -1);
public slots:
    void quickFilterBySupplierUrl(const QString &url);
private slots:
    void refresh();
    void refreshManual();
    void refreshQuiet();
    void lineEditSearchTextChanged(QString);
    void groupChanged(int index);
    void subgroupsUpdated();
    void constructMainTableQueryFilter();
    void subgroupIndexChanged(const QModelIndex &item);
    bool comboBoxIndexChanged(QComboBox *widget, int &oldIndex, int newIndex);
    void undoComboBoxIndexChange(QComboBox *widget, int &oldIndex);
    void filterChanged(int);
    int checkChanges(int mask);
    void confirmChanges(int &result, int mask = SuppliersModel, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
    bool commitChanges(int flags = SuppliersModel);
};

class STableViewSubgroups : public STableViewBase
{
    Q_OBJECT
    friend class tabPurchaseManager;
signals:
    void refreshed();
public:
    enum Column {Group = 0, Selected = 1, Total = 2};
    Q_ENUM(Column)
    enum ColumnWidth {Group_w = 30, Selected_w= 30, Total_w= 30};
    Q_ENUM(ColumnWidth)
    explicit STableViewSubgroups(QWidget *parent = nullptr);
    ~STableViewSubgroups();
    bool eventFilter(QObject *object, QEvent *event) override;
    void setModel(QAbstractItemModel *model) override;
    void clearModel() override;
    void setModelQuery(const QString &query, const QSqlDatabase &database) override;
    QString formatFilterGroup(const FilterList &filter);
    void resetSubgroupIndex();
    void refresh(bool preserveScrollPos = ScrollPosReset, bool preserveSelection = SelectionReset) override;
    void refreshPending() override;
private:
    SPartsRequestsGroupingModel *m_model = nullptr;
    void setDefaultLayoutParams() override;
    bool mouseEventOnCell(QEvent *event);
    QString prepareOrderClause(const int column, Qt::SortOrder order) override;
    void translateNames();
};

#endif // TABPARTSREQUESTSMANAGER_H
