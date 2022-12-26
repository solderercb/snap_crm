#ifndef SALETABLEITEMDELEGATES_H
#define SALETABLEITEMDELEGATES_H
#include <QApplication>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMouseEvent>
#include <QAbstractItemView>
#include "../models/ssaletablemodel.h"
#include "../global.h"

#define PIXMAP_W 16
#define PIXMAP_H 16
#define PIXMAP_GAP 2

class SaleTableItemDelegates : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum PixmapType {RemoveWork = 1, Work, AddPart, RemovePart, Part};
    SaleTableItemDelegates(QObject *parent = nullptr);
    SaleTableItemDelegates(SSaleTableModel*, QObject *parent = nullptr);
    ~SaleTableItemDelegates();

    SSaleTableModel *tableModel;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    bool event(QEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;
    void setTableModel(SSaleTableModel*);
private:
    mutable bool m_must_open_box;
    int m_tableMode = SSaleTableModel::TablesSet::StoreSale;
    int m_tableModelState = SSaleTableModel::State::StoreNew;
    QLineEdit* createLineEdit(QWidget*, QAbstractItemModel *) const;
    void setLineEditData(QWidget *editor, const QString&) const;
    void setModelDataFromLineEdit(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QComboBox* createComboBox(QWidget*, QAbstractItemModel *) const;
    void setComboBoxData(QWidget *editor, const QString&) const;
    void setModelDataFromComboBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    QSpinBox* createSpinBox(QWidget*, const QModelIndex &) const;
    void setSpinBoxData(QWidget *editor, const int) const;
    void setModelDataFromSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    QDoubleSpinBox* createDoubleSpinBox(QWidget*) const;
    void setDoubleSpinBoxData(QWidget *editor, const float) const;
    void setModelDataFromDoubleSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    QRect pixmapRect(const QRect &delegateRect, const PixmapType p) const;
    void drawPixmap(const QRect &delegateRect, PixmapType p, QPainter *painter) const;
};

#endif // SaleTableItemDelegates_H
