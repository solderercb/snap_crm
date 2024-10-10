#ifndef SPARTSREQUESTS_H
#define SPARTSREQUESTS_H

#include "widgets/swidget.h"
#include <QTableView>
#include "widgets/stableviewbase.h"
#include "modules/purchasemanager/srequestsmodel.h"
#include "widgets/shortlivednotification.h"

namespace Ui {
class SPartsRequests;
}

class SPartsRequests : public SWidget
{
    Q_OBJECT
    friend class tabPurchaseManager;
signals:
    void rowSelected(const int id);
    void requestStateChanged(const SPartRequest::State state);
    void modelReset();
public:
    explicit SPartsRequests(QWidget *parent = nullptr);
    ~SPartsRequests();
    void setFilter(const FilterList &filter);
    void setPeriod();
    void refresh(bool preserveScrollPos = STableViewBase::ScrollPosPreserve, bool preserveSelection = STableViewBase::SelectionReset);
    void delayedRefresh(const int msec);
    void setButtonsVisible(bool visible = true);
    bool isDirty();
    int requestCount();
    void setReadOnly(const bool state);
    void setTableLayout(const SLocalSettings::SettingsVariant &layoutVariant);
private:
    Ui::SPartsRequests *ui;
    SPartsRequestsModel *m_model;
    bool m_buttonsVisible = 1;
    void updateWidgets();
    bool eventFilter(QObject *object, QEvent *event) override;
    STableViewBase* tableView();
public slots:
    void commit();
private slots:
    void tableRowSelected(const QModelIndex &current, const QModelIndex &prev);
    void tableRowDoubleClicked(const QModelIndex &index);
    void cancelRequests();
    void createRequestCopy();
    void checkChanges(int &result, const int mask, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
};

#endif // SPARTSREQUESTS_H
