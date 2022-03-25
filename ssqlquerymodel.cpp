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
    }
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
QString SSqlQueryModel::getDisplayRole(int id, int column)
{
    for(int i=0; i<QSqlQueryModel::rowCount(); i++)
    {
        if(QSqlQueryModel::index(i, column).data().toInt() == id)
        {
            return QSqlQueryModel::index(i, 0).data().toString();
        }
    }
    return NULL;
}

QString SSqlQueryModel::getDisplayRole(int id, QString field)
{
    int fieldId = getFieldIdByName(field);
    if(fieldId >= 0)
        return getDisplayRole(id, fieldId);
    else
        return "unknown";
}

