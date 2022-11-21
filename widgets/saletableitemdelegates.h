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

class SaleTableItemDelegates : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SaleTableItemDelegates(SSaleTableModel*, QObject *parent = nullptr);
    ~SaleTableItemDelegates();

    SSaleTableModel *tableModel;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    bool event(QEvent *);
    bool eventFilter(QObject *, QEvent *);
private:
    mutable bool m_must_open_box;
};

#endif // SaleTableItemDelegates_H
