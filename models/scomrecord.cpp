#include "scomrecord.h"

SComRecord::SComRecord(QObject *parent) : SDatabaseRecord(parent)
{
    i_logRecord = new SLogRecordModel(this);
    i_logTexts = new QStringList();
}

SComRecord::~SComRecord()
{
    delete i_logRecord;
    delete i_logTexts;
}

/* Добавление текста журнала
 * При создании/обновлении записи может измениться несколько полей за раз,
 * а изменения некотрых полей должны обязательно записываться в журнал
 */
void SComRecord::appendLogText(const QString &text)
{
    i_logTexts->append(text);
}

bool SComRecord::commitLogs()
{
    QString logText;
    foreach(logText, *i_logTexts)
    {
        i_logRecord->setText(logText);
        i_nDBErr = i_logRecord->commit();
        if(!i_nDBErr)
            return 0;
    }
    i_logTexts->clear();
    return 1;
}

