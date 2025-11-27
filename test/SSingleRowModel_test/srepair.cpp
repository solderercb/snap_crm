#include "srepair.h"
#include <QDebug>
#include <SLogRecordModel>

SRepair::SRepair(QObject *parent) :
    SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "client" << "type" << "maker" << "office" << "manager" << "diagnostic_result" << "in_date" << "ext_notes" << "fault" << "reject_reason" << "company" << "start_office" << "current_manager" << "master";
    i_tableName = "workshop";
    setPrimaryKeyIndex(0);
}

QVariant SRepair::inDateTime()
{
    return "additional report field inDateTime";
}

QVariant SRepair::outDateTime()
{
    return "additional report field outDateTime";
}


void SRepair::logDataChange(const int index, const QVariant &data)
{
    if(!this->data(i_primaryKeyIndex, ModifiedField::Executed).isValid())
        return;

    switch (index)
    {
        case C_diagnosticResult: appendLogText(tr("Результат диагностики изменён на \"%1\"").arg(data.toString())); break;
        default: break;
    }
}


void SRepair::updateLogAssociatedRecId()
{
    i_logRecord->set_repair(data(C_id).toInt());
}

void SRepair::loadError(const int type, const QString &text)
{
    qDebug() << "[" << this << "] loadError() | type: " << type << ", text: " << text;
}
