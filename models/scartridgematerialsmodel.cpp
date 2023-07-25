#include "scartridgematerialsmodel.h"
#include <QSqlTableModel>
#include "widgets/scartridgecard.h"

SCartridgeMaterialsModel::SCartridgeMaterialsModel(QObject *parent) :
    SEditableBaseModel(parent, QSqlDatabase::database("connThird"))
{
    setEditStrategy(OnManualSubmit);
    setTable("materials");
    connect(this, &SCartridgeMaterialsModel::primeInsert, this, &SCartridgeMaterialsModel::slotPrimeInsert);
}

SCartridgeMaterialsModel::~SCartridgeMaterialsModel()
{

}

SStandardItemModel *SCartridgeMaterialsModel::materialsList()
{
    SStandardItemModel *list;
    QMetaEnum types = SCartridgeMaterialModel::staticMetaObject.enumerator(SCartridgeMaterialModel::staticMetaObject.indexOfEnumerator("Type"));

    list = SStandardItemModel::modelFromEnum(types, tr);
    list->setObjectName("cartridgeMaterialsList");

    return list;
}

QVariant SCartridgeMaterialsModel::data(const QModelIndex &idx, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (idx.column())
        {
            case materialsTable::Column::Type: return cartridgeMaterialsListModel->getDisplayRole(unformattedData(idx).toInt());
            case materialsTable::Column::Summ: return data(index(idx.row(), materialsTable::Column::Price)).toDouble()+data(index(idx.row(), materialsTable::Column::PriceWork)).toDouble();
            default: ;
        }
    }

    return SEditableBaseModel::data(idx, role);
}

Qt::ItemFlags SCartridgeMaterialsModel::flags(const QModelIndex &item) const
{
    if( item.column() == materialsTable::Column::ID || item.column() == materialsTable::Column::Summ )
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool SCartridgeMaterialsModel::appendRow()
{
    insertRow(rowCount());
    findNextMaterial();
}

void SCartridgeMaterialsModel::setDefaultTonerWeight(const int weight)
{
    m_defaultTonerWeight = weight;
}

bool SCartridgeMaterialsModel::removeRow(const int row, const QModelIndex&)
{
    removeRows(row, materialsTable::Column::Type); // сначала удаление, потом установка значения -1
    setData(index(row, materialsTable::Column::Type), -1, Qt::EditRole);
}

void SCartridgeMaterialsModel::setCardId(const int id)
{
    m_cardId = id;
    setSelectStatement(QUERY_SEL_CARTRIDGE_MATERIALS);
    setFilter(QString("`card_id` = %1").arg(m_cardId));
}

bool SCartridgeMaterialsModel::commit()
{
    if(!submitAll())
    {
        throw Global::ThrowType::QueryError;
    }
}

int SCartridgeMaterialsModel::findNextMaterial(const int targetRow)
{
    int i = 0;
    QMetaEnum types = SCartridgeMaterialModel::staticMetaObject.enumerator(SCartridgeMaterialModel::staticMetaObject.indexOfEnumerator("Type"));
    int materialId = types.value(i);
    int rowData;

    for(int row = 0; row < rowCount();)
    {
        if(row == targetRow)
        {
            row++;
            continue;
        }

        rowData = unformattedData(index(row, materialsTable::Column::Type)).toInt();
        if(rowData >= 0)
        {
            if(rowData == materialId)
            {
                i++;
                materialId = types.value(i);
                row = 0;
                continue;
            }
        }
        row++;
    }

    if(materialId == -1)
        emit noFurtherMaterialAddition(true);

    return materialId;
}

void SCartridgeMaterialsModel::slotPrimeInsert(int row, QSqlRecord &record)
{
    QSqlRecord *rec = new QSqlRecord();
    QSqlField *field;
    int type = findNextMaterial(row);

    field = new QSqlField("id", QVariant::Int);
    rec->append(*field);
    delete field;

    field = new QSqlField("type", QVariant::Int);
    field->setValue(type);
    rec->append(*field);
    delete field;

    field = new QSqlField("count", QVariant::Int);
    field->setValue((type == SCartridgeMaterialModel::Toner)?m_defaultTonerWeight:1);
    rec->append(*field);
    delete field;

    field = new QSqlField("price", QVariant::Double);
    field->setValue(0);
    rec->append(*field);
    delete field;

    field = new QSqlField("works_price", QVariant::Double);
    field->setValue(0);
    rec->append(*field);
    delete field;

    field = new QSqlField("summ", QVariant::Double);
    rec->append(*field);
    delete field;

    field = new QSqlField("name", QVariant::String);
    rec->append(*field);
    delete field;

    field = new QSqlField("articul", QVariant::Int);
    rec->append(*field);
    delete field;

    record = *rec;
}

/* Локализованные элементы enum
*/
void SCartridgeMaterialsModel::translateNames()
{
    tr("Toner");
    tr("Drum");
    tr("Chip");
    tr("Blade");
}

bool SCartridgeMaterialsModel::insertRowIntoTable(const QSqlRecord &values)
{
    QSqlRecord r(values);
    QSqlField f("card_id");
    f.setValue(m_cardId);
    r.remove(materialsTable::Column::Summ);
    r.append(f);
    return SEditableBaseModel::insertRowIntoTable(r);
}
