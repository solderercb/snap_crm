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
    if(QStandardItemModel::columnCount())
    {
        for (int i = 0; i<QStandardItemModel::columnCount(); i++)
        {
            if (QStandardItemModel::headerData(i,Qt::Horizontal).toString() == field)
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
 * ComboBox'а, использующего эту модель (в отличие от класса SqlQueryModel, данные этой модели заданы жестко в коде).
 * id - код записи, используемый в БД; не путать с currentIndex ComboBox'а;
 * column - это номер поля, по которому будет происходить поиск id.
 * Пример. Модель содержит список платёжных систем, представленных наборами данных "name", "id", ...
 * В таблице БД records имеется поле type_id, содержащее цифровой код платёжной системы
 * Чтобы в ComboBox'е выбрать значение, соответствующее текущей записи в таблице records,
 * в функцию нужно передать id равный значению type_id и column равный 1.
 */
QString SStandardItemModel::getDisplayRole(int id, int searchColumn)
{
    int row = rowByDatabaseID(id, searchColumn);
    initDisplayRoleColumn();
    if(row == -1)
        return NULL;

    return QStandardItemModel::index(row, m_displayRoleColumn).data().toString();
}

QString SStandardItemModel::getDisplayRole(int id, QString searchField)
{
    int searchColumn = getFieldIdByName(searchField);

    return getDisplayRole(id, searchColumn);
}

int SStandardItemModel::rowByDatabaseID(int id, int searchColumn)
{
    if(this->rowCount() == 0)
        return -1;

    for(int i=0; i<QStandardItemModel::rowCount(); i++)
    {
        if(QStandardItemModel::index(i, searchColumn).data().toInt() == id)
        {
            return i;
        }
    }

    return -1;
}

int SStandardItemModel::rowByDatabaseID(int id, QString searchField)
{
    int searchColumn = getFieldIdByName(searchField);

    return rowByDatabaseID(id, searchColumn);
}

int SStandardItemModel::databaseIDByRow(int row, int column)
{
    return index(row, column).data().toInt();
}

int SStandardItemModel::databaseIDByRow(int row, QString field)
{
    int column = getFieldIdByName(field);
    return index(row, column).data().toInt();
}

void SStandardItemModel::setDisplayRoleColumn(const QString &field)
{
    m_displayRoleColumn = getFieldIdByName(field);
}

int SStandardItemModel::findIndex(const QString &search)
{
    initDisplayRoleColumn();
    int deviceModelIndex = rowCount();
    while (--deviceModelIndex >= 0 && index(deviceModelIndex, m_displayRoleColumn).data().toString() != search);
    return deviceModelIndex;
}

void SStandardItemModel::initDisplayRoleColumn()
{
    if(m_displayRoleColumn == -1)
    {
        m_displayRoleColumn = getFieldIdByName("name");
    }
}

