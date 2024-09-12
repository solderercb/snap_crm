#include "sclientinputformbase.h"
#ifdef QT_DEBUG
#include "models/sdatabaseauxiliary.h"
#endif


SClientInputFormBase::SClientInputFormBase(QWidget *parent) :
    SWidget(parent)
{
    i_clientsTabTitle = tr("Выбрать клиента");
}

/* Изменение названия вкладки выбора клиента.
 * Например, на вкладке создания запроса на закупку есть два поля:
 * выбор поставщика и выбор клиента и гораздо удобнее когда
 * вкладка будет иметь соответствующее название.
*/
void SClientInputFormBase::setClientsTabTitle(const QString &title)
{
    i_clientsTabTitle = title;
}

/* Настройка категории по умолчанию для вкладки Выбор клиента
*/
void SClientInputFormBase::setClientsTabDefaultCategory(int category)
{
    i_clientsTabDefaultCategory = category;
}

void SClientInputFormBase::fillCreds(int id)
{
    Q_UNUSED(id)
}

/* Случайный id из имеющихся клиентов.
 * В качестве параметров можно передать название поля таблицы `clients` и значение для дополнительной фильтрации,
 * например, чтобы выбор происходил только среди постоянных клиентов или среди поставщиков. Фильтр строгий (=).
 * Для mysql 5.6.51 (win) результаты первых двух запросов будут состоять из NULL,
 * поэтому запросы повторяются до получения числового значения, но не более 5 раз
*/
#ifdef QT_DEBUG
int SClientInputFormBase::randomClientIdFromDB(const QString &filterField, QVariant filterValue)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    int id = 0;
    QString f;

    if(!filterField.isEmpty())
    {
        f = " AND t1.`" + filterField + "` = " + SDatabaseAuxiliary::fieldValueHandler(filterValue);
    }

    for(int j = 0; j < 5; j++)
    {
        query->exec(QString("SELECT `id` FROM (SELECT ROUND((@i+1) * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM clients AS t1 CROSS JOIN (SELECT @i := -1) AS dummy WHERE t1.`state` = 1 %1) AS t1 ON t1.`num` = `rand`.`rand`;").arg(f));
        if(j < 2)   // первые сгенерированные числа одинаковые в каждой новой сесии, поэтому их пропускаем
            continue;

        query->first();
        if(query->isValid())
        {
            id = query->record().value(0).toInt();
            if(id)
                break;
        }
    }
    delete query;

    return id;
}
#endif
