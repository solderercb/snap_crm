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

public:
    explicit SWorksAndParts(QWidget *parent = nullptr);
    ~SWorksAndParts();
    void setRepairModel(SRepairModel *model);
    void setReadOnly(bool state = true);
    bool isCommitted();
    void commit();
    void load(const int repair);
    bool isEmpty();
private:
    Ui::SWorksAndParts *ui;
    SSaleTableModel *m_model = nullptr;
    SRepairModel *m_repairModel = nullptr;
    bool m_modelRO = 1;
    bool m_modelAdmEdit = 0;
    int m_clientId = 0;
public slots:
    void updateWidgets();
private slots:
    void addCustomWork();
    void onReturnQuickAddPart();
    bool quickAddPart(const int uid);
    void updateTotalSumms(const double, const double, const double);
    void saveTotalSumms();
    void switchEditStrategy(bool);
    void saveSaleTableClicked();
    void setSaveSaleTableEnabled();
    void buttonWorksAdminEdit(bool state);
    void buttonAddItemClicked();
    void repairModelUpdated();
};

#endif // SWORKSANDPARTS_H
