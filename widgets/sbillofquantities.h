#ifndef SBILLOFQUANTITIES_H
#define SBILLOFQUANTITIES_H

#include <QWidget>
#include "models/srepairmodel.h"
#include "models/ssaletablemodel.h"
#include "widgets/stableviewboqworkshop.h"
#include "widgets/stableviewboqitemdelegates.h"

namespace Ui {
class SBillOfQuantities;
}

class SBillOfQuantities : public QWidget
{
    Q_OBJECT
signals:
    void amountUpdated(const double amount);
public:
    explicit SBillOfQuantities(QWidget *parent = nullptr);
    ~SBillOfQuantities();
    void setRepairId(const int id);
    void linkWithRepairModel(SRepairModel *model);
    void setReadOnly(bool state = true);
    bool isDirty();
    void commit();
    void load(const int repair);
    bool isEmpty();
    void clearTable();
#ifdef QT_DEBUG
    void dbgAddWork();
#endif
private:
    Ui::SBillOfQuantities *ui;
    SSaleTableModel *m_model = nullptr;
    SRepairModel *m_repairModel = nullptr;
    bool m_modelRO = 1;
    bool m_modelAdmEdit = 0;
    int m_clientId = 0;
public slots:
    void updateWidgets();
    void switchEditStrategy(bool);
private slots:
    void addCustomWork();
    void onReturnQuickAddPart();
    bool quickAddPart(const int uid);
    void updateAmounts(const double, const double, const double);
    void saveAmounts();
    void saveSaleTableClicked();
    void setSaveSaleTableEnabled();
    void buttonWorksAdminEdit(bool state);
    void buttonAddItemClicked();
    void repairModelUpdated();
    void createTabNewPartRequest();
};

#endif // SBILLOFQUANTITIES_H
