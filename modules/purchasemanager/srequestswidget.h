#ifndef SPARTSREQUESTS_H
#define SPARTSREQUESTS_H

#include <QMessageBox>
#include <SWidget>
#include <STableViewBase>
#include <SPartsRequestsModel>

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
    void commit(const int stage = 0) override;
private:
    Ui::SPartsRequests *ui;
    SPartsRequestsModel *m_model;
    bool m_buttonsVisible = 1;
    void updateWidgets();
    bool eventFilter(QObject *object, QEvent *event) override;
    STableViewBase* tableView();
    void endCommit() override;
private slots:
    void tableRowSelected(const QModelIndex &current, const QModelIndex &prev);
    void tableRowDoubleClicked(const QModelIndex &index);
    void cancelRequests();
    void createRequestCopy();
    void checkChanges(int &result, const int mask, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
};

#endif // SPARTSREQUESTS_H
