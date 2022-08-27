#include "sdevmdlsmodel.h"

SDevMdlsModel::SDevMdlsModel(QObject *parent) : QObject(parent)
{
    m_sqlModel = new QSqlQueryModel(this);
}

SDevMdlsModel::~SDevMdlsModel()
{
    delete m_sqlModel;
}

QSqlQueryModel *SDevMdlsModel::models()
{
    return m_sqlModel;
}

void SDevMdlsModel::indexChanged(int)
{

}
