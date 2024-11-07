#include "scartridgematerialstableitemdelegates.h"
#include "scartridgecard.h"

SCartridgeMaterialsTableItemDelegates::SCartridgeMaterialsTableItemDelegates(QObject *parent) : STableViewBaseItemDelegates(parent)
{

}

SCartridgeMaterialsTableItemDelegates::SCartridgeMaterialsTableItemDelegates(SCartridgeMaterialsModel*, QObject*)
{

}

SCartridgeMaterialsTableItemDelegates::~SCartridgeMaterialsTableItemDelegates()
{

}

QWidget *SCartridgeMaterialsTableItemDelegates::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *w;
    switch (index.column())
    {
        case materialsTable::Column::Type:
            w = createComboBox(parent, cartridgeMaterialsListModel); break;
        case materialsTable::Column::Count:
            w = createSpinBox(parent, index); break;
        case materialsTable::Column::Price:
        case materialsTable::Column::PriceWork:
        case materialsTable::Column::SalarySumm:
            w = createDoubleSpinBox(parent, index); break;
        case materialsTable::Column::Articul:
            w = createSpinBox(parent, index); static_cast<QSpinBox*>(w)->setButtonSymbols(QSpinBox::NoButtons); static_cast<QSpinBox*>(w)->setMaximum((int)((((long)1) << 31) - 1)); break;
        default:
            w = QStyledItemDelegate::createEditor(parent, option, index);
    }
    return w;
}

void SCartridgeMaterialsTableItemDelegates::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
        case materialsTable::Column::Type: setComboBoxData(editor, index.data().toString()); return;
        case materialsTable::Column::Count:
        case materialsTable::Column::Articul: setSpinBoxData(editor, index.data().toInt()); return;
        case materialsTable::Column::Price:
        case materialsTable::Column::PriceWork:
        case materialsTable::Column::SalarySumm:
            setDoubleSpinBoxData(editor, sysLocale.toDouble(index.data().toString())); return;
        case materialsTable::Column::Name: setLineEditData(editor, index.data().toString()); return;
        default: ;
    }
}

void SCartridgeMaterialsTableItemDelegates::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
        case materialsTable::Column::Type:
            setModelDataFromComboBox(editor, model, index); return;
        case materialsTable::Column::Count:
        case materialsTable::Column::Articul:
            setModelDataFromSpinBox(editor, model, index); return;
        case materialsTable::Column::Price:
        case materialsTable::Column::PriceWork:
        case materialsTable::Column::SalarySumm:
            setModelDataFromDoubleSpinBox(editor, model, index); return;
        case materialsTable::Column::Name:
            setModelDataFromLineEdit(editor, model, index); return;
        default:
            QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void SCartridgeMaterialsTableItemDelegates::setTableModel(QAbstractItemModel *model)
{
    m_tableModel = static_cast<SCartridgeMaterialsModel*>(model);
}

void  SCartridgeMaterialsTableItemDelegates::setModelDataFromSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *sb = qobject_cast<QSpinBox *>(editor);
    Q_ASSERT(sb);
    if(sb->hasAcceptableInput())
        model->setData(index, sb->value(), Qt::EditRole);
    else
        model->setData(index, QVariant(), Qt::EditRole);
}

void  SCartridgeMaterialsTableItemDelegates::setModelDataFromDoubleSpinBox(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *sb = qobject_cast<QDoubleSpinBox *>(editor);
    Q_ASSERT(sb);
    if(sb->hasAcceptableInput())
        model->setData(index, sb->value(), Qt::EditRole);
    else
        model->setData(index, QVariant(), Qt::EditRole);
}
