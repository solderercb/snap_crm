#include "stableviewcashbox.h"
#include <QApplication>
#include <ProjectGlobals>
#include <STableViewGridLayout>
#include <STableCashboxModel>
#include <SComSettings>
#include <SUserSettings>
#include <SCashRegisterModel>

STableViewCashbox::STableViewCashbox(QWidget *parent) :
    STableViewBase(SLocalSettings::KassaGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Reason].Width_marked = true;  // автоширина по умолчанию
}

STableViewCashbox::~STableViewCashbox()
{
}

XtraSerializer* STableViewCashbox::gridLayout()
{
    return i_gridLayout;
}

void STableViewCashbox::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<STableBaseModel*>(model);
    STableViewBase::setModel(model);
    setItemDelegate(new STableViewCashboxItemDelegates(m_model, this));
}

void STableViewCashbox::updateImportedLayoutParams()
{
    STableViewBase::updateImportedLayoutParams();

    auto setName = [=](int key){
        i_gridLayout->$GridControl.Columns[key].FieldName = i_defaultHeaderLabels.value(key, "");
    };

    // Заголовки столбцов в файле АСЦ на английском языке
    setName(Column::AmountsByCash);
    setName(Column::AmountsCashless);
    setName(Column::AmountsByCard);
    setName(Column::AmountsOtherType);
}

void STableViewCashbox::setColumnWidth(int column, int width)
{
    if(m_model)
        m_model->setColumnWidth(column, (int)(width/m_fontMetrics->averageCharWidth()));

    STableViewBase::setColumnWidth(column, width);
}

void STableViewCashbox::translateNames()
{
    tr("Id");
    tr("Created");
    tr("Office");
    tr("AmountsByCash");
    tr("AmountsCashless");
    tr("AmountsByCard");
    tr("AmountsOtherType");
    tr("PaymentType");
    tr("Covenantor");
    tr("Employee");
    tr("Reason");
}

void STableViewCashbox::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}

STableViewCashboxSummary::STableViewCashboxSummary(QWidget *parent) :
    STableViewSummaryBase(SLocalSettings::KassaGrid, parent)
{
    setBackgroundRole(QPalette::Window);
//    setStyleSheet("background-color: rgb(240, 240, 240);");
}

void STableViewCashboxSummary::setGridLayout(XtraSerializer *layout)
{
    delete i_gridLayout;
    i_gridLayout = layout;
    m_dataModel->setColumnCount(i_gridLayout->$GridControl.Columns.count());
    for(int r = 0; r < 3; r++)
    {
        QList<QStandardItem*> *emptyModelData = new QList<QStandardItem*>();
        for(int i = 0; i < i_gridLayout->$GridControl.Columns.count(); i++)
            *emptyModelData << new QStandardItem();
        m_dataModel->appendRow(*emptyModelData);
        for(int i = STableViewCashbox::Column::AmountsByCash; i < STableViewCashbox::Column::AmountsOtherType; i++)
        {
            setData(r, i, "<value>");
        }
    }
}

QSize STableViewCashboxSummary::minimumSizeHint() const
{
    QSize size = STableViewBase::minimumSizeHint();
    size.setHeight(3*userDbData->rowHeight());
    return size;
}

void STableViewCashboxSummary::setTotal(int row, int column, double value)
{
    setTotal(row, column, value, comSettings->classicKassa()?2:0);
}

void STableViewCashboxSummary::setTotal(int row, int column, double value, int decimals)
{
    setTotal(row, column, STableModelsCommonMethods::dataLocalizedFromDouble(value, decimals));
}

void STableViewCashboxSummary::setTotal(int row, int column, QVariant value)
{
    m_dataModel->setData(m_dataModel->index(row, column), value);
}

/**********************************************************************************************************************************************************/
STableViewCashboxItemDelegates::STableViewCashboxItemDelegates(QObject *parent) : STableViewBaseItemDelegates(parent)
{

}

STableViewCashboxItemDelegates::STableViewCashboxItemDelegates(STableBaseModel *model, QObject *parent) :
    STableViewCashboxItemDelegates(parent)
{
    setTableModel(model);
}

STableViewCashboxItemDelegates::~STableViewCashboxItemDelegates()
{
}

void STableViewCashboxItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == STableCashboxModel::Columns::PaymentType )
        drawPixmap(option.rect, m_tableModel->unformattedData(index).toInt(), painter);
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QRect STableViewCashboxItemDelegates::pixmapRect(const QRect &delegateRect, const int pixmapType) const
{
    Q_UNUSED(pixmapType)

    int x,y,w,h;
    x = delegateRect.left()+(delegateRect.width() -  PIXMAP_W)/2;
    y = delegateRect.top()+(delegateRect.height()-PIXMAP_W)/2;
    w = PIXMAP_W;
    h = PIXMAP_W;

    return QRect(x,y,w,h);
}

void STableViewCashboxItemDelegates::drawPixmap(const QRect &rect, int pixmapType, QPainter *painter) const
{
    QPixmap pixmap;
    switch (pixmapType)
    {
        case SCashRegisterModel::PaymentType::RecptSimple:
        case SCashRegisterModel::PaymentType::ExpSimple: pixmap = QPixmap(":/icons/light/1F4B0_32.png"); break;
        case SCashRegisterModel::PaymentType::RecptInvoice:
        case SCashRegisterModel::PaymentType::ExpInvoice: pixmap = QPixmap(":/icons/light/1F5CE_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpInvoiceUndo:  pixmap = QPixmap(":/icons/light/ExpInvoiceUndo_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpBalance:
        case SCashRegisterModel::PaymentType::RecptBalance: pixmap = QPixmap(":/icons/light/1F464_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpSubsist:
        case SCashRegisterModel::PaymentType::ExpSalary: pixmap = QPixmap(":/icons/light/1F477_32.png"); break;
        case SCashRegisterModel::PaymentType::RecptRepair:
        case SCashRegisterModel::PaymentType::RecptPrepayRepair: pixmap = QPixmap(":/icons/light/1F6E0_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpRepair: pixmap = QPixmap(":/icons/light/ExpRepair_32.png"); break;
        case SCashRegisterModel::PaymentType::RecptGoods: pixmap = QPixmap(":/icons/light/1F6D2_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpGoods: pixmap = QPixmap(":/icons/light/ExpGoods_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpDealer: pixmap = QPixmap(":/icons/light/1F91D_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpZ:
        case SCashRegisterModel::PaymentType::AddSubCash: pixmap = QPixmap(":/icons/light/ExpZ_32.png"); break;
        case SCashRegisterModel::PaymentType::MoveCash: pixmap = QPixmap(":/icons/light/1F4B8_32.png"); break;
        case SCashRegisterModel::PaymentType::ExpRevert: pixmap = QPixmap(":/icons/light/ExpRevert_32.png"); break;
        default: pixmap = QPixmap(":/icons/light/1F6C8_32.png");
    }
    QApplication::style()->drawItemPixmap(painter, pixmapRect(rect, pixmapType), 1, pixmap.scaled(PIXMAP_W,PIXMAP_H));
}

void STableViewCashboxItemDelegates::setTableModel(QAbstractItemModel *model)
{
    m_tableModel = static_cast<STableCashboxModel*>(model);
}
