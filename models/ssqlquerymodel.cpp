#include "ssqlquerymodel.h"

SSqlQueryModel::SSqlQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

SSqlQueryModel::~SSqlQueryModel()
{

}

/* Возвращает номер поля по его названию
 * поиск производится по 0-вой записи
 */
int SSqlQueryModel::getFieldIdByName(const QString &field)
{
    if(QSqlQueryModel::rowCount())
    {
        for (int i = 0; i<QSqlQueryModel::columnCount(); i++)
        {
            if (QSqlQueryModel::record(0).field(i).name() == field)
            {
                return i;
            }
        }
#ifdef  QT_DEBUG
        Q_ASSERT_X(0, objectName().toLocal8Bit(), QString("field [\"name\"] not defined").toLocal8Bit());
#endif
    }
#ifdef  QT_DEBUG
    Q_ASSERT_X(0, objectName().toLocal8Bit(), QString("object is empty").toLocal8Bit());
#endif
    return -1;
}

/* Возвращает т. н. DisplayRole модели данных; в основном, эта функция будет использоваться для установки currentText
 * ComboBox'а, использующего эту модель.
 * id - код записи, используемый в БД; не путать с currentIndex ComboBox'а;
 * column - это номер поля, по которому будет происходить поиск id.
 * Пример. Модель содержит данные с результатом запроса SELECT name, id FROM table types;
 * В таблице records имеется поле type_id, являющееся связанным ключом с types.id
 * Чтобы в ComboBox'е выбрать значение, соответствующее текущей записи в таблице records,
 * в функцию нужно передать id равный значению type_id и column равный 1.
 */
QString SSqlQueryModel::getDisplayRole(int id, int searchColumn)
{
    int row = rowByDatabaseID(id, searchColumn);
    if(m_displayRoleColumn == -1)
    {
        m_displayRoleColumn = getFieldIdByName("name");
    }

    if(row == -1)
        return NULL;

    return QSqlQueryModel::index(row, m_displayRoleColumn).data().toString();
}

QString SSqlQueryModel::getDisplayRole(int id, QString searchField)
{
    int searchColumn = getFieldIdByName(searchField);

    return getDisplayRole(id, searchColumn);
}

int SSqlQueryModel::rowByDatabaseID(int id, int searchColumn)
{
    if(this->rowCount() == 0)
        return -1;

    for(int i=0; i<QSqlQueryModel::rowCount(); i++)
    {
        if(QSqlQueryModel::index(i, searchColumn).data().toInt() == id)
        {
            return i;
        }
    }

    return -1;
}

int SSqlQueryModel::rowByDatabaseID(int id, QString searchField)
{
    int searchColumn = getFieldIdByName(searchField);

    return rowByDatabaseID(id, searchColumn);
}

int SSqlQueryModel::databaseIDByRow(int row, int column)
{
    return index(row, column).data().toInt();
}

int SSqlQueryModel::databaseIDByRow(int row, QString field)
{
    int column = getFieldIdByName(field);
    return index(row, column).data().toInt();
}

