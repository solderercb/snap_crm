#ifndef STABLEVIEWCASHBOX_H
#define STABLEVIEWCASHBOX_H

#include <QObject>
#include <STableViewBase>
#include <STableViewSummaryBase>
#include <STableViewBaseItemDelegates>

#define PIXMAP_W 16
#define PIXMAP_H 16
#define PIXMAP_GAP 2

class STableBaseModel;
class STableCashboxModel;
class STableViewCashboxItemDelegates;

// Таблица с суммами по столбцам
class STableViewCashboxSummary : public STableViewSummaryBase
{
    Q_OBJECT
public:
    explicit STableViewCashboxSummary(QWidget *parent = nullptr);
    void setGridLayout(XtraSerializer* layout) override;
    QSize minimumSizeHint() const override;
    void setTotal(int row, int column, double value);
    void setTotal(int row, int column, double value, int decimals);
    void setTotal(int row, int column, QVariant value);
};

class STableViewCashbox : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    enum Column {Id = 0, Created = 1, Office = 2, AmountsByCash = 3, AmountsCashless = 4, AmountsByCard = 5, AmountsOtherType = 6, PaymentType = 7, Covenantor = 8, Employee = 9, Reason = 10};
    Q_ENUM(Column)
    enum ColumnWidth {Id_w = 60, Created_w = 115, Office_w = 115, AmountsByCash_w = 70, AmountsCashless_w = 70, AmountsByCard_w = 70, AmountsOtherType_w = 70, PaymentType_w = 24, Covenantor_w = 100, Employee_w = 100, Reason_w = 390};
    Q_ENUM(ColumnWidth)
    explicit STableViewCashbox(QWidget *parent = nullptr);
    ~STableViewCashbox();
    XtraSerializer *gridLayout();
    void setModel(QAbstractItemModel *model) override;
    void updateImportedLayoutParams() override;
private:
    bool m_showPayedRepairs = 0;
    STableBaseModel *m_model = nullptr;
    void setColumnWidth(int column, int width) override;
    void translateNames();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth) override;
public slots:
};

class STableViewCashboxItemDelegates : public STableViewBaseItemDelegates
{
    Q_OBJECT
public:
    STableViewCashboxItemDelegates(QObject *parent = nullptr);
    STableViewCashboxItemDelegates(STableBaseModel*, QObject *parent = nullptr);
    ~STableViewCashboxItemDelegates();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setTableModel(QAbstractItemModel*) override;
private:
    STableCashboxModel *m_tableModel = nullptr;
    QRect pixmapRect(const QRect &delegateRect, const int pixmapType) const;
    void drawPixmap(const QRect &delegateRect, int pixmapType, QPainter *painter) const;
};


#endif // STABLEVIEWCASHBOX_H
