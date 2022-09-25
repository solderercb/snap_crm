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
