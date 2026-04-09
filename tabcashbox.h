#ifndef TABCASHBOX_H
#define TABCASHBOX_H

#include <tabCommon>
#include <QObject>
#include <STableViewBase>

class SStandardItemModel;
class SPartsRequestsGroupingModel;
class QConcatenateTablesProxyModel;
class STableCashboxModel;
class SSortFilterProxyModel;
class SPaymentTypesModel;

namespace Ui {
class tabCashbox;
}

class tabCashbox : public tabCommon
{
    Q_OBJECT
    friend class SPartsRequests;
    friend class STableViewSubgroups;
public:
    static tabCashbox* getInstance(MainWindow *parent = nullptr);
    static void refreshIfTabExists();
    explicit tabCashbox(MainWindow *parent = nullptr);
    ~tabCashbox();
    bool event(QEvent *event) override;
    QString tabTitle() override;
    void setFocusSearchField();
private:
    enum QueryFilter{Repair, Client, Tracking, SupplierUrl, SupplierId};
    enum DirtyFlags{SuppliersModel = 1, RequestsModel = 2};
    int m_company = -1;
    int m_office = -1;
    int m_paymentType = -1;
    int m_paymentSystem = -1;
    static tabCashbox* p_instance;
    Ui::tabCashbox *ui;
    std::shared_ptr<SPaymentTypesModel> m_allEnabledTypes;
    SSortFilterProxyModel *m_paymentSystems;
    std::shared_ptr<QConcatenateTablesProxyModel> m_allOrdersTypesModel;
    void constructQueryClause(FilterList &filter);
    void loadBalances();
    void loadMonthStat();
    QPalette paletteForValue(bool forNegativeValues);
public slots:
private slots:
    void refresh();
    void refreshManual();
    void lineEditSearchTextChanged(QString);
    void constructMainTableQueryFilter();
    void officeChanged(const int index);
    void filterChanged(int);
    void createMoveCashTab();
};

#endif // TABCASHBOX_H
