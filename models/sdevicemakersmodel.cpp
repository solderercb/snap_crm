#include "sdevicemakersmodel.h"
#include <QSqlQueryModel>
#include <ProjectQueries>
#include <SDevMdlsModel>

SDeviceMakersModel::SDeviceMakersModel(QObject *parent) : QObject(parent)
{
    m_sqlModel = new QSqlQueryModel(this);
    m_devMdlsModel = new SDevMdlsModel(this);
}

SDeviceMakersModel::~SDeviceMakersModel()
{
    delete m_devMdlsModel;
    delete m_sqlModel;
}

void SDeviceMakersModel::load(const int device)
{
    QString query = QUERY_SEL_DEVICE_MAKERS(device);
    m_sqlModel->setQuery(query, QSqlDatabase::database("connMain"));
}

QSqlQueryModel *SDeviceMakersModel::makers()
{
    return m_sqlModel;
}

QSqlQueryModel *SDeviceMakersModel::models()
{
    return m_devMdlsModel->models();
}

void SDeviceMakersModel::indexChanged(int)
{

}
