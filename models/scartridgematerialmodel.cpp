#include "scartridgematerialmodel.h"

SCartridgeMaterialModel::SCartridgeMaterialModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "materials";
    i_obligatoryFields << "type" << "name" << "card_id";
    i_idColumnName = "id";
}

int SCartridgeMaterialModel::id()
{
    return i_id;
}

void SCartridgeMaterialModel::load(const int id)
{
    i_id = id;

    if(!id)
        loadError(Global::ThrowType::ConditionsError);

    QSqlQuery record(QSqlDatabase::database("connMain"));
    record.exec(QString("SELECT `name`, `type`, `count`, `articul`, `card_id`, `price`, `works_price`, `salary_summ` FROM `materials` WHERE `id` = %1;`").arg(id));
    if(!record.first())
        loadError(Global::ThrowType::ResultError);

    m_type = record.value("type").toInt();
    m_count = record.value("count").toInt();
    m_articul = record.value("articul").toInt();
    m_name = record.value("name").toString();
    m_cardId = record.value("card_id").toInt();
    m_price = record.value("price").toDouble();
    m_worksPrice = record.value("works_price").toDouble();
    m_salarySumm = record.value("salary_summ").toDouble();
    initWorkName((Type)m_type);
}

void SCartridgeMaterialModel::loadError(const int type)
{
    QString msg = tr("Не удалось загрузить SCartridgeMaterialModel; id материала: %1, id картриджа: %2").arg(i_id).arg(m_initializerCardId);
    Global::throwError(type, msg);
}

int SCartridgeMaterialModel::type()
{
    return m_type;
}

void SCartridgeMaterialModel::setType(const int type)
{
    i_valuesMap.insert("type", type);
}

int SCartridgeMaterialModel::count()
{
    return m_count;
}

void SCartridgeMaterialModel::setCount(const int count)
{
    i_valuesMap.insert("count", count);
}

int SCartridgeMaterialModel::articul()
{
    return m_articul;
}

void SCartridgeMaterialModel::setArticul(const int articul)
{
    i_valuesMap.insert("articul", articul);
}

QString SCartridgeMaterialModel::name()
{
    return m_name;
}

void SCartridgeMaterialModel::setName(const QString& name)
{
    i_valuesMap.insert("name", name);
}

int SCartridgeMaterialModel::cardId()
{
    return m_cardId;
}

void SCartridgeMaterialModel::setCardId(const int card_id)
{
    i_valuesMap.insert("card_id", card_id);
}

/* ID карточки картриджа, соответствующий i_id класса SCartridgeCardModel в родительском объекте.
 * Эта переменная используется только для записи сообщения об ошибке в журнал.
 * Например, если метод SCartridgeMaterialModel::load(const int id) вызван с некорректым id материала,
 * ошибка в журнале будет содержать id карточки картриджа и позволит облегчить анализ ошибок.
*/
void SCartridgeMaterialModel::setInitializerCardId(const int id)
{
    m_initializerCardId = id;
}

double SCartridgeMaterialModel::price()
{
    return m_price;
}

void SCartridgeMaterialModel::setPrice(const double price)
{
    i_valuesMap.insert("price", price);
}

double SCartridgeMaterialModel::worksPrice()
{
    return m_worksPrice;
}

void SCartridgeMaterialModel::setWorksPrice(const double works_price)
{
    i_valuesMap.insert("works_price", works_price);
}

QString SCartridgeMaterialModel::workName()
{
    return m_workName;
}

double SCartridgeMaterialModel::salarySumm() const
{
    return m_salarySumm;
}

void SCartridgeMaterialModel::setSalarySumm(double salarySumm)
{
    m_salarySumm = salarySumm;
    i_valuesMap.insert("salary_summ", salarySumm);
}

/* Название работы, которая будет добавлена в список работ и деталей
*/
void SCartridgeMaterialModel::initWorkName(const Type type)
{
    switch(type)
    {
        case Toner: m_workName = tr("Заправка"); break;
        case Drum: m_workName = tr("Замена фотобарабана"); break;
        case Chip: m_workName = tr("Замена чипа"); break;
        case Blade: m_workName = tr("Замена лезвия"); break;
        case Replace: m_workName = tr("Восстановление"); break; // данная работа, как таковая не выполняется ввиду нерентабельности; под ней подразумавается замена картриджа на новый не оригинальный; с таким названием работа будет фигурировать в документах, предназначенных для основной массы клиентов — организаций, и позволяет избежать лишней бюрократии со списанием изношенного и приёмом на баланс нового картриджа
        case TonerPlus: m_workName = tr("Заправка (увелич.)"); break;
        case MagRoller: m_workName = tr("Замена магн. вала"); break;
        case Prophilaxy: m_workName = tr("Профилактика"); break;
        case PrimRoller: m_workName = tr("Замена первич. вала"); break;
        case Marker: m_workName = tr("Флажок сброса"); break;
        case Scraper: m_workName = tr("Замена ракеля"); break;
        case Bushings: m_workName = tr("Замена втулок"); break;
    }
}
