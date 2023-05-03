#ifndef TABSALARY_H
#define TABSALARY_H

#include <QWidget>
#include "tabcommon.h"
#include "models/susermodel.h"
#include "models/ssortfilterproxymodel.h"
#include "models/salarytabmodels/stablesalaryrepairsmodel.h"
#include "models/salarytabmodels/stablesalaryreceptedissued.h"
#include "models/salarytabmodels/stablesalaryrepairworksmodel.h"
#include "models/salarytabmodels/stablesalaryrepairpartsmodel.h"
#include "models/salarytabmodels/stablesalarypaymentsmodel.h"
#include "models/salarytabmodels/stablesalarysalesmodel.h"
#include "models/salarytabmodels/stablesalaryitemsmodel.h"
#include "models/salarytabmodels/stablesalarysalepartsmodel.h"

namespace Ui {
class tabSalary;
}

class tabSalary : public tabCommon
{
    Q_OBJECT
    friend class SPageSalarySummary;
    friend class SPageSalaryRepairs;
    friend class SPageSalarySales;
    friend class SPageSalaryItems;
    friend class SPageSalaryReceptedIssued;
    friend class SPageSalaryPayments;
signals:
    void updateDaughterTabsModels();
    void updateDaughterTabsWidgets();
public:
    explicit tabSalary(MainWindow *parent = nullptr);
    static tabSalary* getInstance(MainWindow *parent = nullptr);
    ~tabSalary();
    QString tabTitle() override;
    QString periodBegin();
    QString periodEnd();
private:
    Ui::tabSalary *ui;
    static tabSalary* p_instance;
    SUserModel *m_userModel = nullptr;
    SSortFilterProxyModel *usersModelF;
    QDateTime m_periodBegin;
    QDateTime m_periodEnd;
    bool m_showSubsistanceGroup = 1;
    bool m_showSalaryGroup = 1;
    int m_employeeId = 0;
    STableSalaryRepairsModel *m_repairs = nullptr;
    STableSalaryRepairWorksModel *m_repairWorks = nullptr;
    STableSalaryRepairPartsModel *m_repairParts = nullptr;
    STableSalarySalesModel *m_sales = nullptr;
    STableSalarySalePartsModel *m_saleParts = nullptr;
    QSqlQueryModel *m_addtitional = nullptr;
    STableSalaryItemsModel *m_items = nullptr;
    STableSalaryPaymentsModel *m_payments = nullptr;
    STableSalaryReceptedIssued *m_recepted = nullptr;
    STableSalaryReceptedIssued *m_issued = nullptr;
    int m_modelUpdatedFlags = 0;
    double m_summReceped = 0;
    double m_summRecepedToPay = 0;
    double m_summIssued = 0;
    double m_summIssuedToPay = 0;
    void updateModels();
    void modelsUpdated();
    void updateWidgets();
    void setModelUpdatedFlag(const int pos);
public slots:
    void loadButtonPressed();
    void periodDateChanged(const QDate date);
};

#endif // TABSALARY_H
