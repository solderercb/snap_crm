#ifndef SPARTSUPPLIERS_H
#define SPARTSUPPLIERS_H

#include "widgets/swidget.h"
#include <QClipboard>
#include "widgets/stableviewbase.h"
#include "modules/purchasemanager/sgroupingmodel.h"
#include "modules/purchasemanager/ssuppliersmodel.h"
#include "modules/purchasemanager/srequest.h"

namespace Ui {
class SPartSuppliers;
}

class SPartSuppliers : public SWidget
{
    Q_OBJECT
signals:
    void confirmChanges(int &result);
    void modelReset();
    void submitOk();
    void beginRowInsert();
public:
    explicit SPartSuppliers(QWidget *parent = nullptr);
    ~SPartSuppliers();
    void select(const int &id);
    void clearModel();
    void copyLinks(const int id);
    bool isDirty();
    void setSaveButtonVisible(bool visible = true);
    QSize minimumSizeHint() const override {return QSize(640, 200);};
    void setRequestId(int requestId);
    void setSubgroupsModel(SPartsRequestsGroupingModel *subgroupsModel);
    void setGroupingMode(const int mode);
    void setRowHighlightingClause(const int id, const QString &name);
    void setPredefSupplierId(const int id);
    void refresh(bool preserveScrollPos = STableViewBase::ScrollPosPreserve, bool preserveSelection = STableViewBase::SelectionReset);
    void connectSuppliersTableWithManager();
private:
    Ui::SPartSuppliers *ui;
    SPartSuppliersModel *m_model;
    int m_requestId = -1;
    bool m_saveButtonVisible = 1;
    SPartRequest::State m_state = SPartRequest::State::Created;
    QMenu *tableMenu = nullptr;
    void initModels();
    void initWidgets();
    void updateWidgets();
    void enableEdit(const bool state = true);
    void initTableMenu();
    void addRow();
    void copyLink();
public slots:
    void saveLinks();
    void orderChanged(const int section, Qt::SortOrder order);
    void removeSelected();
    void setRequestState(const int state);
    void addSupplierRecord(const int id);
private slots:
    void menuRequest(QPoint pos);
};
#endif // SPARTSUPPLIERS_H
