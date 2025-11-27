#ifndef STABLEVIEWPARTSREQUESTS_H
#define STABLEVIEWPARTSREQUESTS_H

#include <QMessageBox>
#include <STableViewBase>
#include <STableViewBaseDelegates>
#include <SPartsRequestsModel>
#include <QWidget>

class STableViewPartsRequestsItemDelegates : public STableViewBaseItemDelegates
{
public:
    explicit STableViewPartsRequestsItemDelegates(QWidget *parent = nullptr);
    explicit STableViewPartsRequestsItemDelegates(SPartsRequestsModel *model, QWidget *parent = nullptr);
    ~STableViewPartsRequestsItemDelegates();
    void setTableModel(QAbstractItemModel *model) override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
    SPartsRequestsModel *m_tableModel = nullptr;
    QComboBox* createComboBox(QWidget*parent, QAbstractItemModel *model) const;
    void paintProgressBar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class STableViewPartsRequests : public STableViewBase
{
    Q_OBJECT
    friend class SPartsRequests;
signals:
    void checkChanges(int &result, int mask, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
public:
    enum Column {Check = 0, Id, Date, Employee, Customer, ItemName, Count, Dealer, Repair, State, Track, Priority, Progress};
    Q_ENUM(Column)
    enum ColumnWidth {Check_w = 30, Id_w = 100, Date_w = 100, Employee_w = 150, Customer_w = 150, ItemName_w = 150, Count_w = 60, Dealer_w = 100, Repair_w = 100, State_w = 100, Track_w = 110, Priority_w = 90, Progress_w = 100};
    Q_ENUM(ColumnWidth)
    explicit STableViewPartsRequests(QWidget *parent = nullptr);
    ~STableViewPartsRequests();
    void setDefaultLayoutParams() override;
    bool eventFilter(QObject *object, QEvent *event) override;
    void setModel(QAbstractItemModel *model) override;
    void setModelQuery(const QString &query, const QSqlDatabase &database) override;
    void clearModel() override;
    void setSorting(const int logicalIndex, const Qt::SortOrder order) override;
    void refresh(bool preserveScrollPos = ScrollPosReset, bool preserveSelection = SelectionReset) override;
private:
    SPartsRequestsModel *m_model = nullptr;
    void setColumnWidth(int column, int width) override;
    bool mouseEventOnCell(QEvent *event);
    void translateNames();
private slots:
};

#endif // STABLEVIEWPARTSREQUESTS_H
