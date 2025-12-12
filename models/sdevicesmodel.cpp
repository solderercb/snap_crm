/*  Модель данных устройств, используемая при приёме в ремот.
 *  Представляет из себя "матрёшку", вложениями которой являются модели данных производителей и моделей устройств
 */
#include "sdevicesmodel.h"
#include <QSqlQueryModel>
#include <ProjectQueries>
#include <SDeviceMakersModel>

SDevicesModel::SDevicesModel(const int office, QObject *parent) : QObject(parent)
{
    m_sqlModel = new QSqlQueryModel(this);
    m_deviceMakersModel = new SDeviceMakersModel(this);
    load(office);
}

SDevicesModel::~SDevicesModel()
{
    delete m_deviceMakersModel;
    delete m_sqlModel;
}

/* FUTURE
 * Загрузка типов уст-в по коду офиса
 * Подразумевается, что если у организации есть несколько офисов, а перечень ремонтируемых уст-в в офисах разный
 */
void SDevicesModel::load(const int office)
{
    Q_UNUSED(office);
    m_sqlModel->setQuery(QUERY_SEL_DEVICES, QSqlDatabase::database(TdConn::main()));
}

QSqlQueryModel *SDevicesModel::devices()
{
    return m_sqlModel;
}

QSqlQueryModel *SDevicesModel::makers()
{
    return m_deviceMakersModel->makers();
}

QSqlQueryModel *SDevicesModel::models()
{
    return m_deviceMakersModel->models();
}

void SDevicesModel::deviceChanged(int)
{

}

void SDevicesModel::makerChanged(int)
{

}

void SDevicesModel::mdlChanged(int)
{

}
