#include "repairstablemodel.h"

repairsTableModel::repairsTableModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    uint32_t i = 0, j = 0;
    QSqlQuery* status_colors = new QSqlQuery(QSqlDatabase::database("connMain"));

    /* Подключать лишние модули (для обработки JSON) не наш метод :-) парсим данные в запросе */
    QString query = QString(
    "\
    SELECT\r\n\
      REPLACE(  SUBSTRING_INDEX(REPLACE(TRIM(LEADING '[{' FROM SUBSTRING_INDEX(TRIM(TRAILING '}]' FROM SUBSTRING_INDEX(t1.`statuses`,'},{',enumerator.`number`)),'},{',1)),'\"',''),',Name',1),'Id:','') AS 'id',\r\n\
      SUBSTRING_INDEX(SUBSTRING_INDEX(REPLACE(TRIM(LEADING '[{' FROM SUBSTRING_INDEX(TRIM(TRAILING '}]' FROM SUBSTRING_INDEX(t1.`statuses`,'},{',enumerator.`number`)),'},{',1)),'\"',''),'Name:',-1),',Contains:',1) AS 'name',\r\n\
      SUBSTRING_INDEX(SUBSTRING_INDEX(REPLACE(TRIM(LEADING '[{' FROM SUBSTRING_INDEX(TRIM(TRAILING '}]' FROM SUBSTRING_INDEX(t1.`statuses`,'},{',enumerator.`number`)),\'},{\',1)),\'\"\',\'\'),\'Roles:\',-1),\',Color:\',1) AS \'Acl\',\r\n\
      SUBSTRING_INDEX(SUBSTRING_INDEX(REPLACE(TRIM(LEADING \'[{\' FROM SUBSTRING_INDEX(TRIM(TRAILING \'}]\' FROM SUBSTRING_INDEX(t1.`statuses`,\'},{\',enumerator.`number`)),\'},{\',1)),\'\"\',\'\'),\'Contains:\',-1),\',Actions:\',1) AS \'Contains\',\r\n\
      SUBSTRING_INDEX(SUBSTRING_INDEX(REPLACE(TRIM(LEADING \'[{\' FROM SUBSTRING_INDEX(TRIM(TRAILING \'}]\' FROM SUBSTRING_INDEX(t1.`statuses`,\'},{\',enumerator.`number`)),\'},{\',1)),\'\"\',\'\'),\'Actions:\',-1),\',Roles:\',1) AS \'Actions\',\r\n\
      SUBSTRING_INDEX(SUBSTRING_INDEX(REPLACE(TRIM(LEADING \'[{\' FROM SUBSTRING_INDEX(TRIM(TRAILING \'}]\' FROM SUBSTRING_INDEX(t1.`statuses`,\'},{\',enumerator.`number`)),\'},{\',1)),\'\"\',\'\'),\'Color:\',-1),\',Terms:\',1) AS \'Color\',\r\n\
      SUBSTRING_INDEX(REPLACE(TRIM(LEADING \'[{\' FROM SUBSTRING_INDEX(TRIM(TRAILING \'}]\' FROM SUBSTRING_INDEX(t1.`statuses`,\'},{\',enumerator.`number`)),\'},{\',1)),\'\"\',\'\'),\'Terms:\',-1) AS \'Terms\',\r\n\
      REPLACE(TRIM(LEADING \'[{\' FROM SUBSTRING_INDEX(TRIM(TRAILING \'}]\' FROM SUBSTRING_INDEX(t1.`statuses`,\'},{\',enumerator.`number`)),\'},{\',1)),\'\"\',\'\') AS \'status\'\r\n\
    FROM `config` AS t1\r\n\
    JOIN enumerator\r\n\
    ON\r\n\
      (LENGTH(REPLACE(t1.`statuses`, \'},{\', \'}{\'))-LENGTH(t1.`statuses`) <= enumerator.`number`+1) WHERE `id` = 1;\
    ");
    status_colors->exec(query);

    status.resize(status_colors->size());
    status_ids_tmp.resize(status_colors->size());
    colors.resize(status_colors->size());

    while(status_colors->next())
    {
        QStandardItem* item = new QStandardItem(status_colors->value("name").toString());
        color = new QColor();
        item->setData(status_colors->value("Acl").toString(), Qt::UserRole+1);
        item->setData(status_colors->value("Contains").toString(), Qt::UserRole+2);
        item->setData(status_colors->value("Actions").toString(), Qt::UserRole+3);
//        item->setData(status_colors->value("Color").toString(), Qt::UserRole+4);
        item->setData(status_colors->value("Terms").toString(), Qt::UserRole+5);
        item->setData(status_colors->value("status").toString(), Qt::UserRole+6);
        status_ids_tmp[i] = status_colors->value("id").toInt(); //
        status[i] = item;  // указатели на объекты храним в массиве; ключ массива — id категории товара
        color->setNamedColor(status_colors->value("Color").toString());
        colors[i] = color->operator QVariant();
        if(status_ids_tmp[i] > j)
            j = status_ids_tmp[i]; // здесь будем хранить максимальный id статуса
        i++;
    }
    status_ids.resize(j+1);
    for(i=0; i < status_ids_tmp.size(); i++)
    {
       status_ids[status_ids_tmp[i]] = i;  // меняем ключ и значение массива, сохраняем его во второй массив
    }
    status_ids_tmp.clear();
    delete status_colors;  //
}

//QVariant repairsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    // FIXME: Implement me!
//}

//bool repairsTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
//{
//    if (value != headerData(section, orientation, role)) {
//        // FIXME: Implement me!
//        emit headerDataChanged(orientation, section, section);
//        return true;
//    }
//    return false;
//}


//int repairsTableModel::rowCount(const QModelIndex &parent) const
//{
//    if (parent.isValid())
//        return 0;

//    // FIXME: Implement me!
//}

//int repairsTableModel::columnCount(const QModelIndex &parent) const
//{
//    if (parent.isValid())
//        return 0;

//    // FIXME: Implement me!
//}

//bool repairsTableModel::hasChildren(const QModelIndex &parent) const
//{
//    // FIXME: Implement me!
//}

//bool repairsTableModel::canFetchMore(const QModelIndex &parent) const
//{
//    // FIXME: Implement me!
//    return false;
//}

//void repairsTableModel::fetchMore(const QModelIndex &parent)
//{
//    // FIXME: Implement me!
//}

QVariant repairsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    // FIXME: Implement me!
    if (role == Qt::BackgroundRole and index.column() == 11)
    {
        return colors[status_ids[this->index(index.row(), index.column()+1).data().toInt()]];
//        return colors[status_ids[index.data().toInt()]];
    }
//    if (role == Qt::CheckStateRole)     // если включить обработку этой роли, то отображаются checkbox'ы,
//        return false;         // а возвращаемое значение устанавливает состояние: 0 - выкл, 1 - част., 2 - вкл.
    if (role == Qt::DisplayRole and index.column() == 11)      // если для роли Qt::DisplayRole подменять текст, то всё ОК
    {
        return QVariant(status[status_ids[this->index(index.row(), index.column()+1).data().toInt()]]->text());
//        return QVariant("aaa");
    }
    return QSqlQueryModel::data(index, role);   // или если просто возвращать данные наследуемого объекта, то тоже всё ОК
//      return QVariant("foo");           // а если для всех остальных ролей возвращать свой текст, то отображаются checkbox'ы
//    return false;   // если возвращать false для всех остальных вариантов role, то отображаются checkbox'ы
}

//bool repairsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    if (data(index, role) != value) {
//        // FIXME: Implement me!
//        emit dataChanged(index, index, QVector<int>() << role);
//        return true;
//    }
//    return false;
//}

Qt::ItemFlags repairsTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled; // FIXME: Implement me!
}

//bool repairsTableModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    beginInsertRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endInsertRows();
//}

//bool repairsTableModel::insertColumns(int column, int count, const QModelIndex &parent)
//{
//    beginInsertColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endInsertColumns();
//}

//bool repairsTableModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endRemoveRows();
//}

//bool repairsTableModel::removeColumns(int column, int count, const QModelIndex &parent)
//{
//    beginRemoveColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endRemoveColumns();
//}
