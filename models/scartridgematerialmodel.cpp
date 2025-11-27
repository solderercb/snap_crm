#include "scartridgematerialmodel.h"
#include <ProjectGlobals>

SCartridgeMaterialModel::SCartridgeMaterialModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "materials";
    i_obligatoryFields << "type" << "name" << "card_id";

    setPrimaryKeyIndex(0);
}

void SCartridgeMaterialModel::load(const int id)
{
    setPrimaryKey(id);

    if(!id)
        loadError(Global::ThrowType::ConditionsError);

    SSingleRowJModel::load();
    initWorkName((Type)type());
}

void SCartridgeMaterialModel::loadError(const int type)
{
    QString msg = tr("Не удалось загрузить SCartridgeMaterialModel; id материала: %1, id картриджа: %2").arg(id()).arg(m_initializerCardId);
    Global::throwError(type, msg);
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

QString SCartridgeMaterialModel::workName()
{
    return m_workName;
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
