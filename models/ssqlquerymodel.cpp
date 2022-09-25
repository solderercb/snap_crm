#include "ssqlquerymodel.h"

SSqlQueryModel::SSqlQueryModel(QObject *parent):
    QSqlQueryModel(parent),
    SAbstractItemModel(parent)
{
    abstractItemModel = this;
}

SSqlQueryModel::~SSqlQueryModel()
{

}
