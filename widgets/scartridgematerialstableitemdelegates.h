#ifndef SCARTRIDGEMATERIALSTABLEITEMDELEGATES_H
#define SCARTRIDGEMATERIALSTABLEITEMDELEGATES_H

#include <QObject>
#include "stableviewbaseitemdelegates.h"
#include "models/scartridgematerialsmodel.h"

class SCartridgeMaterialsTableItemDelegates : public STableViewBaseItemDelegates
{
    Q_OBJECT
public:
    explicit SCartridgeMaterialsTableItemDelegates(QObject *parent = nullptr);
    explicit SCartridgeMaterialsTableItemDelegates(SCartridgeMaterialsModel *, QObject *parent = nullptr);
    ~SCartridgeMaterialsTableItemDelegates();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    bool eventFilter(QObject *, QEvent *) override;
    void setTableModel(SCartridgeMaterialsModel *);
private:
    SCartridgeMaterialsModel *tableModel;
    mutable bool m_must_open_box;
    QLineEdit* createLineEdit(QWidget*) const;
    void setLineEditData(QWidget *editor, const QString&) const;
    void setModelDataFromLineEdit(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QComboBox* createComboBox(QWidget*, QAbstractItemModel *) const;
    void setComboBoxData(QWidget *editor, const QString&) const;
    void setModelDataFromComboBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    QSpinBox* createSpinBox(QWidget*, const QModelIndex &) const;
    void setSpinBoxData(QWidget *editor, const int) const;
    void setModelDataFromSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    QDoubleSpinBox* createDoubleSpinBox(QWidget*, const QModelIndex&) const;
    void setDoubleSpinBoxData(QWidget *editor, const double) const;
    void setModelDataFromDoubleSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
};

#endif // SCARTRIDGEMATERIALSTABLEITEMDELEGATES_H
