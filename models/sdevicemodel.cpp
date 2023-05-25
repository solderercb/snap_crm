#include "sdevicemodel.h"

SDeviceModel::SDeviceModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "fault_list" << "look_list" << "complect_list" << "company_list";
    i_tableName = "devices";
    i_idColumnName = "id";
}

SDeviceModel::~SDeviceModel()
{

}

int SDeviceModel::id()
{
    return m_id;
}

void SDeviceModel::setId(int id)
{
    i_valuesMap.insert("id", id);
}

QString SDeviceModel::name()
{
    return m_name;
}

void SDeviceModel::setName(QString name)
{
    i_valuesMap.insert("name", name);
}

int SDeviceModel::position()
{
    return m_position;
}

void SDeviceModel::setPosition(int position)
{
    i_valuesMap.insert("position", position);
}

bool SDeviceModel::isEnable()
{
    return m_isEnable;
}

void SDeviceModel::setEnabled(bool enable)
{
    i_valuesMap.insert("enable", enable);
}

bool SDeviceModel::isRefill()
{
    return m_isRefill;
}

void SDeviceModel::setRefill(bool refill)
{
    i_valuesMap.insert("refill", refill);
}

QString SDeviceModel::faultList()
{
    return m_faultList;
}

void SDeviceModel::setFaultList(QString fault_list)
{
    i_valuesMap.insert("fault_list", fault_list);
}

QString SDeviceModel::lookList()
{
    return m_lookList;
}

void SDeviceModel::setLookList(QString look_list)
{
    i_valuesMap.insert("look_list", look_list);
}

QString SDeviceModel::complectList()
{
    return m_complectList;
}

void SDeviceModel::setComplectList(QString complect_list)
{
    i_valuesMap.insert("complect_list", complect_list);
}

QString SDeviceModel::companyList()
{
    return m_companyList;
}

void SDeviceModel::setCompanyList(QString company_list)
{
    i_valuesMap.insert("company_list", company_list);
}

