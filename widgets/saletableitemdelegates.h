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
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    bool event(QEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;
private:
    mutable bool m_must_open_box;
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
};

#endif // SaleTableItemDelegates_H
