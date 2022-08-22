/*  Вспомогательный класс для работы с записью таблицы БД
 *  Содержит некоторие ширпотребные методы и объекты, которые,
 *  в свою очередь, тоже наследуют базовый класс (например,
 *  объект класса SLogRecordModel)
 */
#ifndef SCOMRECORD_H
#define SCOMRECORD_H

#include "sdatabaserecord.h"
#include <QObject>
#include "models/slogrecordmodel.h"

class SComRecord : public SDatabaseRecord
{
    Q_OBJECT
public:
    explicit SComRecord(QObject *parent = nullptr);
    ~SComRecord();
    void appendLogText(const QString&);
    void appendLogText(const QString&, const QString &disambiguation);
    void removeLogText(const QString &disambiguation);
protected:
    bool commitLogs();
    QMap<QString, QString> *i_logTexts;
    SLogRecordModel *i_logRecord;

};

#endif // SCOMRECORD_H
