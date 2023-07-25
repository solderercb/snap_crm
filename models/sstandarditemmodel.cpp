#include "sstandarditemmodel.h"

SStandardItemModel::SStandardItemModel(QObject *parent):
    QStandardItemModel(parent),
    SAbstractItemModel(parent)
{
    abstractItemModel = this;
}

SStandardItemModel::~SStandardItemModel()
{

}

/* Создаёт модель данных из enum
 * _enum - ссылка на данные
 * contextTr - указатель на метод QObject::tr() объекта из которого вызывается данный метод (объекта, в контексте которого задан перевод)
*/
SStandardItemModel *SStandardItemModel::modelFromEnum(QMetaEnum &_enum, QString (*contextTr)(const char *, const char *, int))
{
    SStandardItemModel *list = new SStandardItemModel();
    QList<QStandardItem*> *item;
    if(contextTr == nullptr)
    {
        contextTr = QObject::tr;
    }

    for(int i = 0; i < _enum.keyCount(); i++)
    {
        item = new QList<QStandardItem*>();
        *item << new QStandardItem(contextTr(_enum.key(i), nullptr, -1)) << new QStandardItem(QString::number(_enum.value(i)));
        list->appendRow(*item);
    }
    list->setHorizontalHeaderLabels({"name", "id"});

    return list;
}
