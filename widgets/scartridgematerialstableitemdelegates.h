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
    void setTableModel(QAbstractItemModel *model) override;
private:
    SCartridgeMaterialsModel *m_tableModel;
    mutable bool m_must_open_box;
    void setModelDataFromSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    void setModelDataFromDoubleSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
};

#endif // SCARTRIDGEMATERIALSTABLEITEMDELEGATES_H
