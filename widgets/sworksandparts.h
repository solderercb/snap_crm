#ifndef SWORKSANDPARTS_H
#define SWORKSANDPARTS_H

#include <QWidget>
#include "models/srepairmodel.h"
#include "models/ssaletablemodel.h"
#include "widgets/stableviewbase.h"
#include "widgets/saletableitemdelegates.h"

namespace Ui {
class SWorksAndParts;
}

class worksAndSparePartsTable : public STableViewBase
{
    Q_OBJECT

signals:
    void createTabSparePart(int);
    void createTabSparePartReserve(int);
public:
    explicit worksAndSparePartsTable(QWidget *parent = nullptr);
    ~worksAndSparePartsTable();
    void setModel(QAbstractItemModel *model) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    SSaleTableModel *m_model = nullptr;
private slots:
#if QT_VERSION >= 0x060000
    void dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>()) override;
#else
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>()) override;
#endif
};

class SWorksAndParts : public QWidget
{
    Q_OBJECT
signals:
    void amountUpdated(const double amount);
public:
    explicit SWorksAndParts(QWidget *parent = nullptr);
    ~SWorksAndParts();
    void setRepairId(const int id);
    void linkWithRepairModel(SRepairModel *model);
    void setReadOnly(bool state = true);
    bool isCommitted();
    void commit();
    void load(const int repair);
    bool isEmpty();
    void clearTable();
#ifdef QT_DEBUG
    void dbgAddWork();
#endif
private:
    Ui::SWorksAndParts *ui;
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
};

#endif // SWORKSANDPARTS_H
