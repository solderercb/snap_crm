#include "modules/purchasemanager/sgroupingmodel.h"

SPartsRequestsGroupingModel::SPartsRequestsGroupingModel(QObject *parent) :
    STableBaseModel{parent}
{

}

SStandardItemModel *SPartsRequestsGroupingModel::groupsList()
{
    SStandardItemModel *list;
    QMetaEnum types = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Group"));

    list = SStandardItemModel::modelFromEnum(types, tr);
    list->setObjectName("partsRequestsGroupsList");

    return list;
}

/* В списке группировки по поставщику нет смысла отображать протокол
 * Также этот метод вызывается из PSItemDelegate::drawItemsCount
*/
QString SPartsRequestsGroupingModel::urlFormat(const QString &url) const
{
    QString ret(url);
    return ret.replace("https://", "");
}

QVariant SPartsRequestsGroupingModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole && item.column() == Columns::Group)
    {
        if(item.siblingAtColumn(Columns::C).data().toInt() == 0)
            return tr("Все");

        return urlFormat(STableBaseModel::data(item, role).toString());
    }
    return STableBaseModel::data(item, role);
}

int SPartsRequestsGroupingModel::id(const QModelIndex &item)
{
    QVariant data = item.siblingAtColumn(Columns::Id).data();
    if(data.isNull())
        return -1;
    return data.toInt();
}

QString SPartsRequestsGroupingModel::name(const QModelIndex &item)
{
    return unformattedData(item.siblingAtColumn(Columns::Group)).toString();
}

void SPartsRequestsGroupingModel::changeGroup(int id)
{
    m_group = id;
}

/* Локализованные элементы enum
*/
void SPartsRequestsGroupingModel::translateNames()
{
    tr("No");
    tr("Repair");
    tr("Client");
    tr("Tracking");
    tr("Supplier");
}

