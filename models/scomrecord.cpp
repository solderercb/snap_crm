#include "scomrecord.h"

SComRecord::SComRecord(QObject *parent) : SDatabaseRecord(parent)
{
    i_logRecord = new SLogRecordModel(this);
    i_logTexts = new QMap<QString, QString>();
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
    i_logTexts->insert(QString::number(i_logTexts->size()), text);
}

void SComRecord::appendLogText(const QString &text, const QString &disambiguation)
{
    i_logTexts->insert(disambiguation, text);
}

void SComRecord::removeLogText(const QString &disambiguation)
{
    i_logTexts->remove(disambiguation);
}

bool SComRecord::commitLogs()
{
    QString logText;
    foreach(logText, *i_logTexts)
    {
        i_logRecord->setText(logText);
        i_nErr &= i_logRecord->commit();
        if(!i_nErr)
        {
            return 0;
        }
    }

    i_logTexts->clear();
    return 1;
}

