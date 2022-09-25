#include "sabstractitemmodel.h"

SAbstractItemModel::SAbstractItemModel(QObject *)
{
}

SAbstractItemModel::~SAbstractItemModel()
{

}

/* Возвращает номер столбца по его названию.
 * Поиск производится по заголовкам и для StandardItemModel их нужно задать явно.
 */
int SAbstractItemModel::getFieldIdByName(const QString &field)
{
    if(abstractItemModel->columnCount())
    {
        for (int i = 0; i<abstractItemModel->columnCount(); i++)
        {
            if (abstractItemModel->headerData(i,Qt::Horizontal).toString() == field)
            {
                return i;
            }
        }
#ifdef  QT_DEBUG
        Q_ASSERT_X(0, abstractItemModel->objectName().toLocal8Bit(), QString("field [\"%1\"] not defined").arg(field).toLocal8Bit());
#endif
    }
#ifdef  QT_DEBUG
    Q_ASSERT_X(0, abstractItemModel->objectName().toLocal8Bit(), QString("object is empty").toLocal8Bit());
#endif
    return -1;
}

/* Возвращает литеральное представление элемента модели; и предназначен для использования с ComboBox (и др. виджетами)
 * id - код записи, используемый в БД.
 * searchColumn - это номер поля, по которому будет происходить поиск id.
 * Напирмер. ComboBox содержит список платёжных систем; модель данных инициализируется запросом
 * "SELECT name, id FROM types". При вызове getDisplayRole(x, 1) будет возвращено название платёжной системы
 * с id = x.
 */
QString SAbstractItemModel::getDisplayRole(int id, int searchColumn)
{

    int row = rowByDatabaseID(id, searchColumn);
    initDisplayRoleColumn();
    if(row == -1)
        return NULL;

    return abstractItemModel->index(row, m_displayRoleColumn).data().toString();
}

/* Это перегруженный метод.
 * Возвращает литеральное представление элемента модели; поиск производится в столбце с указанным именем
 */
QString SAbstractItemModel::getDisplayRole(int id, QString searchField)
{
    int searchColumn = getFieldIdByName(searchField);

    return getDisplayRole(id, searchColumn);
}

/*  Аналогичен getDisplayRole(int, int), но возвращает index
*/
int SAbstractItemModel::rowByDatabaseID(int id, int searchColumn)
{
    if(abstractItemModel->rowCount() == 0)
        return -1;

    for(int i=0; i<abstractItemModel->rowCount(); i++)
    {
        if(abstractItemModel->index(i, searchColumn).data().toInt() == id)
        {
            return i;
        }
    }

    return -1;
}

/*  Аналогичен getDisplayRole(int, QString), но возвращает index
*/
int SAbstractItemModel::rowByDatabaseID(int id, QString searchField)
{
    int searchColumn = getFieldIdByName(searchField);

    return rowByDatabaseID(id, searchColumn);
}

/*  Возвращает id, используемый в БД, по index'у элемента.
*/
int SAbstractItemModel::databaseIDByRow(int row, int column)
{
    return abstractItemModel->index(row, column).data().toInt();
}

/*  Это перегруженный метод.
*/
int SAbstractItemModel::databaseIDByRow(int row, QString field)
{
    int column = getFieldIdByName(field);
    return abstractItemModel->index(row, column).data().toInt();
}

/*  Установка порядкового номера столбца в котором содержатся литеральные представления элементов модели
*/
void SAbstractItemModel::setDisplayRoleColumn(const QString &field)
{
    m_displayRoleColumn = getFieldIdByName(field);
}

/*  Поиск index'а элемента по литеральному представлению
*/
int SAbstractItemModel::findIndex(const QString &search)
{
    initDisplayRoleColumn();
    int deviceModelIndex = abstractItemModel->rowCount();
    while (--deviceModelIndex >= 0 && abstractItemModel->index(deviceModelIndex, m_displayRoleColumn).data().toString() != search);
    return deviceModelIndex;
}

void SAbstractItemModel::initDisplayRoleColumn()
{
    if(m_displayRoleColumn == -1)
    {
        m_displayRoleColumn = getFieldIdByName("name");
    }
}

