#ifndef STABLEVIEWPARTREQUESTSUPPLIERS_H
#define STABLEVIEWPARTREQUESTSUPPLIERS_H

#include "widgets/stableviewbase.h"
#include <QWidget>
#include "widgets/stableviewbaseitemdelegates.h"
#include <QPainter>
#include <QPaintEngine>
#include <QDesktopServices>
#include "modules/purchasemanager/ssuppliersmodel.h"
#include "modules/purchasemanager/sgroupingmodel.h"

class PSItemDelegate : public STableViewBaseItemDelegates
{
    Q_OBJECT
    friend class SPartSuppliers;
    friend class STableViewPartRequestSuppliers;
public:
    explicit PSItemDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void setTableModel(SPartSuppliersModel *model);
private:
    SPartSuppliersModel *m_tableModel = nullptr;
    int m_groupingMode = 0;
    SPartsRequestsGroupingModel *m_subgroupsModel = nullptr;
    QRect pixmapRect(const QRect &delegateRect, const int index = -1, const int span = 1) const;
    void drawPixmap(const QStyleOptionViewItem &option, QPainter *painter) const;
    void drawUrl(const QModelIndex &index, const QStyleOptionViewItem &option, QPainter *painter) const;
    void drawItemsCount(const QModelIndex &index, const QStyleOptionViewItem &option, QPainter *painter) const;
    static QRect textRect(const QRect &delegateRect);
    static bool isUrl(const QModelIndex &index);
    static QSize urlSize(const QModelIndex &index);
    static QRect urlRect(const QRect &cell, const QSize &urlSize);
    static bool pointInRect(const QPoint &point, const QRect &rect);
    bool quickEdit(const QStyleOptionViewItem &option, const QModelIndex &index);
    QSpinBox* createSpinBox(QWidget*, const QModelIndex &) const;
    QDoubleSpinBox* createDoubleSpinBox(QWidget*, const QModelIndex&) const;
    void setSubgroupsModel(SPartsRequestsGroupingModel *subgroupsModel);
    void setGroupingMode(const int mode);
};

class STableViewPartRequestSuppliers : public STableViewBase
{
    Q_OBJECT
public:
    enum Column {Id = 0, RequestId, Supplier, SupplierUrl, ItemUrl, Moq, Price, Select, Notes};
    Q_ENUM(Column)
    enum ColumnWidth {Id_w = 30, RequestId_w = 30, Supplier_w = 150, SupplierUrl_w = 170, ItemUrl_w = 300, Moq_w = 35, Price_w = 60, Select_w = 30, Notes_w = 200};
    Q_ENUM(ColumnWidth)
    explicit STableViewPartRequestSuppliers(QWidget *parent = nullptr);
    ~STableViewPartRequestSuppliers();
    void mouseMoveEvent(QMouseEvent* event) override;
    void clearModel() override;
    void setModelQuery(const QString &query, const QSqlDatabase &database) override;
    void clearVScrollPos() override;
    void refresh(bool preserveScrollPos = ScrollPosReset, bool preserveSelection = SelectionReset) override;
private:
    void setDefaultLayoutParams() override;
    void translateNames();
};

#endif // STABLEVIEWPARTREQUESTSUPPLIERS_H
