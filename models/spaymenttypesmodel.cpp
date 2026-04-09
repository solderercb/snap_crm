#include "spaymenttypesmodel.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SCashRegisterModel>
#include <SStandardItemModel>

SPaymentTypesModel::SPaymentTypesModel(QObject *parent) : SSortFilterProxyModel(parent)
{
    m_fullModel = new QConcatenateTablesProxyModel();
    SSortFilterProxyModel::setSourceModel(m_fullModel);
    setFilterKeyColumn(1);

    QMetaEnum typesEnum = SCashRegisterModel::staticMetaObject.enumerator(SCashRegisterModel::staticMetaObject.indexOfEnumerator("PaymentType"));
    m_predefTypes = SStandardItemModel::modelFromEnum(typesEnum, SCashRegisterModel::tr);
    m_predefTypes->setObjectName("PaymentTypesModel");
    m_predefTypes->setHorizontalHeaderLabels({"name", "id"});

    m_fullModel->addSourceModel(m_predefTypes);
}

SPaymentTypesModel::SPaymentTypesModel(const int type, QObject *parent) : SPaymentTypesModel(parent)
{
    m_type = type;
    std::vector<int> filter;

    loadExtraTypes(false);

    switch(m_type)
    {
        case Type::PKO: filter.assign(std::begin(SCashRegisterModel::tabPKOFilterTypes), std::end(SCashRegisterModel::tabPKOFilterTypes)); setObjectName("receiptTypesModel"); break;
        case Type::RKO: filter.assign(std::begin(SCashRegisterModel::tabRKOFilterTypes), std::end(SCashRegisterModel::tabRKOFilterTypes)); setObjectName("expenditureTypesModel"); break;
        default: ;
    }

    filterTypes(filter);
}

SPaymentTypesModel::~SPaymentTypesModel()
{
    delete m_fullModel;
    delete m_predefTypes;
}

void SPaymentTypesModel::setSourceModel(QAbstractItemModel *model)
{
    m_fullModel->addSourceModel(model);
}

void SPaymentTypesModel::loadExtraTypes(const int type, const bool loadArchive)
{
    if(!m_extraTypes)
    {
        m_extraTypes = std::make_unique<QSqlQueryModel>();
        m_fullModel->addSourceModel(m_extraTypes.get());  // в объединённой модели будут только столбцы name и id
    }

    QString typeStr;
    if(type == Type::All)
        typeStr = "`type`";
    else
        typeStr = QString::number(type);

    m_extraTypes->setQuery(QUERY_SEL_EXTRA_PAYMENT_TYPES(SCashRegisterModel::ExpCustom, typeStr, loadArchive), QSqlDatabase::database(TdConn::main()));
}

/* Загрузка дополнительных типов кассовых операций при инициализации модели.
 * По умолчанию загружаются все настроенные пользователем шаблоны, в т. ч. архивные; это необходимо для отображения всплывающей подсказки на вкладке Финансы.
 * В АСЦ v3.7.31.1123 есть шаблоны только РКО (Настройки -» Финансы -» Расходный кассовый ордер)
*/
void SPaymentTypesModel::loadExtraTypes(const bool loadArchive)
{
    loadExtraTypes(m_type, loadArchive);
}

void SPaymentTypesModel::filterTypes(std::vector<int> filter)
{
    QStringList regexp;


    if(!filter.empty())
    {
        foreach (auto type, filter)
        {
            regexp.append(QString("(^%1$)").arg(type));
        }

        setFilterRegExp("^(?!" + regexp.join('|') + ").*$");
        setFilterKeyColumn(1);
    }
}

QModelIndex SPaymentTypesModel::extraTypeModelIndex(const int typeId)
{
    return m_fullModel->mapToSource(mapToSource(this->index(rowByDatabaseID(typeId), 0)));
}

QVariant SPaymentTypesModel::dataByDatabaseId(int id, int role)
{
    return index(rowByDatabaseID(id), 0).data(role);
}

/*  Возвращает текст основания ордера по id операции.
 *  Предназначена только для шаблонов кассовых ордеров
*/
QString SPaymentTypesModel::reasonByDatabaseId(int id)
{
    return extraTypeModelIndex(id).siblingAtColumn(ExtraTypeFields::Reason).data().toString();
}

int SPaymentTypesModel::clientByDatabaseId(int id)
{
    return extraTypeModelIndex(id).siblingAtColumn(ExtraTypeFields::Client).data().toInt();
}

double SPaymentTypesModel::defaultAmountByDatabaseId(int id)
{
    return extraTypeModelIndex(id).siblingAtColumn(ExtraTypeFields::Amount).data().toDouble();
}

int SPaymentTypesModel::paymentSystemByDatabaseId(int id)
{
    return extraTypeModelIndex(id).siblingAtColumn(ExtraTypeFields::PaymenSystem).data().toInt();
}

