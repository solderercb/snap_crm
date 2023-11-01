#include "stableviewsummarybase.h"

STableViewSummaryBase::STableViewSummaryBase(SLocalSettings::SettingsVariant layoutVariant, QWidget *parent) :
    STableViewBase(layoutVariant, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalHeader()->setVisible(false);
    m_dataModel = new QStandardItemModel();
    setModel(m_dataModel);
}

STableViewSummaryBase::~STableViewSummaryBase()
{
    i_gridLayout = nullptr;
}

void STableViewSummaryBase::setGridLayout(XtraSerializer *layout)
{
    delete i_gridLayout;
    i_gridLayout = layout;
    m_dataModel->setColumnCount(i_gridLayout->$GridControl.Columns.count());
    QList<QStandardItem*> *emptyModelData = new QList<QStandardItem*>();
    for(int i = 0; i < i_gridLayout->$GridControl.Columns.count(); i++)
        *emptyModelData << new QStandardItem();
    m_dataModel->appendRow(*emptyModelData);
}

void STableViewSummaryBase::applyGridlayout()
{
    STableViewBase::applyGridlayout();
}

void STableViewSummaryBase::setData(QModelIndex index, QVariant value)
{
    m_dataModel->setData(index, value);
}

void STableViewSummaryBase::setData(const int row, const int column, QVariant value)
{
    setData(m_dataModel->index(row, column), value);
}


void STableViewSummaryBase::setTotal(int column, double value)
{
    m_dataModel->setData(m_dataModel->index(0, column), value);
}

void STableViewSummaryBase::applyGuiSettings()
{
    STableViewBase::applyGuiSettings();
    setMinimumHeight(userDbData->rowHeight);
}

QSize STableViewSummaryBase::minimumSizeHint() const
{
    QSize size = STableViewBase::minimumSizeHint();
    size.setHeight(userDbData->rowHeight);
    return size;
}

QSize STableViewSummaryBase::sizeHint() const
{
    QSize size = STableViewBase::sizeHint();
    size.setHeight(userDbData->rowHeight);
    return size;
}
