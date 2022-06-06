#include "sstandarditemmodel.h"

SStandardItemModel::SStandardItemModel(QObject *parent) : QStandardItemModel(parent)
{

}

SStandardItemModel::~SStandardItemModel()
{

}

/* Возвращает номер столбца по имени (если установлены заголовки)
 */
int SStandardItemModel::getFieldIdByName(const QString &field)
{
    if(QStandardItemModel::rowCount())
    {
        for (int i = 0; i<QStandardItemModel::columnCount(); i++)
        {
            if (QStandardItemModel::headerData(i,Qt::Horizontal).toString() == field)
            {
                return i;
            }
        }
    }
    return -1;
}

/* Возвращает т. н. DisplayRole модели данных; в основном, эта функция будет использоваться для установки currentText
 * ComboBox'а, использующего эту модель (в отличие от класса SqlQueryModel, данные этой модели заданы жестко в коде).
 * id - код записи, используемый в БД; не путать с currentIndex ComboBox'а;
 * column - это номер поля, по которому будет происходить поиск id.
 * Пример. Модель содержит список платёжных систем, представленных наборами данных "name", "id", ...
 * В таблице БД records имеется поле type_id, содержащее цифровой код платёжной системы
 * Чтобы в ComboBox'е выбрать значение, соответствующее текущей записи в таблице records,
 * в функцию нужно передать id равный значению type_id и column равный 1.
 */
QString SStandardItemModel::getDisplayRole(int id, int column)
{
    for(int i=0; i<QStandardItemModel::rowCount(); i++)
    {
        if(QStandardItemModel::index(i, column).data().toInt() == id)
        {
            return QStandardItemModel::index(i, 0).data().toString();
        }
    }
    return NULL;
}

QString SStandardItemModel::getDisplayRole(int id, QString field)
{
    int fieldId = getFieldIdByName(field);
    if(fieldId >= 0)
        return getDisplayRole(id, fieldId);
    else
        return "unknown";
}

