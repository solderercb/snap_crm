#include "susersettings.h"
#include "global.h"

SUserSettings::SUserSettings()
{
}

void SUserSettings::initWidgets()
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        metaObject()->invokeMethod(this, QByteArray("init__widget").insert(5, metaObject()->property(i).name()), Qt::DirectConnection);

    // Модели данных виджетов (ComboBox) должны быть заданы до загрузки данных, иначе будет падать.
//   static_cast<QComboBox*>(i_editorWidgets.value("..."))->setModel(...);

    load(username);
}

void SUserSettings::load(const QString &connectionLogin)
{
    QString q;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    int i = 0;

    for(i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        metaObject()->invokeMethod(this, QByteArray("register__db_field").insert(9, metaObject()->property(i).name()), Qt::DirectConnection);

    // "переворот" таблицы users
    q = QString("SELECT @smth := CONCAT(IF(@smth IS NOT NULL, CONCAT(@smth, '\\\nUNION ALL\\\n'), ''), 'SELECT \\\'', `COLUMN_NAME`, '\\\', `', `COLUMN_NAME`, '` FROM `users` WHERE `username` = \\\'%1\\\'') FROM information_schema.`COLUMNS` WHERE `TABLE_SCHEMA` = SCHEMA() AND `TABLE_NAME` = 'users';").arg(connectionLogin);
    query->exec("SET @smth := NULL");
    query->exec(q);

    query->last();
    q = query->value(0).toString();
    query->exec(q);
    while (query->next())
    {
        i = fieldToPropertyId(query->value(0).toString());
        if(i < 0)
            continue;

        metaObject()->property(i).write(this, query->value(1));
    }

    q = QString("SELECT GROUP_CONCAT(`role_id`) AS 'roles' FROM `roles_users` WHERE `user_id` = %1 GROUP BY `user_id`  LIMIT 1;").arg(id);
    query->exec(q);
    if(query->first())
        roles = query->value(0).toString();

    // TODO: несколько компаний
    company = 1;

    delete query;

    loadFromUsersParams();
}

void SUserSettings::loadFromUsersParams()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    int i = 0;

    m_params.clear();
    query->exec(QString("SELECT t1.`name`, IFNULL(t2.`value`, t1.`default_value`) AS 'value', t1.`id` FROM `user_params` AS t1 LEFT JOIN `users_params` AS t2 ON t1.`id` = t2.`param_id` WHERE t2.`user_id` = %1 OR t2.`user_id` IS NULL;").arg(id));
    while (query->next())
    {
        i = fieldToPropertyId("usersParams." + query->value(0).toString());
        if(i < 0)
            continue;

        m_params.insert(query->value(0).toString(), query->value(2).toInt());
        metaObject()->property(i).write(this, query->value(1));
    }

    delete query;
}

bool SUserSettings::commit()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QString q;

    fieldsUpdFormatter();
    if(!i_valuesMap.isEmpty())
    {
        q = QString("UPDATE\n  `users`\nSET\n  %2\nWHERE `id` = %1;").arg(id).arg(fields.join(",\n  "));
        QUERY_EXEC(query, nErr)(q);
    }
    delete query;

    if(nErr)
        i_valuesMap.clear();

    return nErr;
}

// распределение изменённых значений по группам по названию таблиц
void SUserSettings::prepareUpdateList(Table table)
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
    {
        QString field = i_fieldNames.value(metaObject()->property(i).name());
        if(field.startsWith("usersParams") != table)
            continue;

        if(table)   // Table::UsersParams
            field = field.replace("usersParams.", "");

        QVariant val = i_fieldModified.value(metaObject()->property(i).name());
        if(!val.isValid())
            val = i_jsonFieldModified.value(metaObject()->property(i).name());
        if(!val.isValid())
            continue;

        i_valuesMap.insert(field, val);
    }
}

void SUserSettings::save()
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
    {
        metaObject()->property(i).read(this);
    }

    i_valuesMap.clear();
    prepareUpdateList(Table::Users);

    if(!commit())
        throw Global::ThrowType::QueryError;
    saveToUsersParams();

    i_fieldModified.clear();
}

void SUserSettings::saveToUsersParams()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    i_valuesMap.clear();
    prepareUpdateList(Table::UsersParams);

    query->prepare(QString("INSERT INTO `users_params` (`user_id`, `param_id`, `value`) VALUES (:user_id, :param_id, :value) ON DUPLICATE KEY UPDATE `value` = VALUES(`value`);"));
    query->bindValue(":user_id", id);
    QMap<QString, QVariant>::const_iterator i = i_valuesMap.constBegin();
    while(i != i_valuesMap.constEnd())
    {
        query->bindValue(":param_id", m_params.value(i.key()));
        query->bindValue(":value", i.value());
        nErr = query->exec();
        if(!nErr)
        {
            errorToLog(metaObject()->className(), query->lastError().text());
            throw Global::ThrowType::QueryError;
        }
        i++;
    }

    delete query;
}

void SUserSettings::updateLoginTimestamp()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    query->exec(QUERY_BEGIN);
    i_valuesMap.insert("last_login", QDateTime::currentDateTime());
    nErr = commit();
    QUERY_COMMIT_ROLLBACK(query, nErr);

    delete query;
}

void SUserSettings::updateActivityTimestamp(const bool standalone)
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    if(standalone)
    {
        query->exec(QUERY_BEGIN);
    }
    i_valuesMap.insert("last_activity", QDateTime::currentDateTime());
    nErr = commit();
    if(standalone)
    {
        QUERY_COMMIT_ROLLBACK(query, nErr);
    }

    delete query;
}

void SUserSettings::translate()
{

}
