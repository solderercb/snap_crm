/*  Вспомогательный класс для работы с записью таблицы БД
 *  Содержит некоторие ширпотребные методы и объекты, которые,
 *  в свою очередь, тоже наследуют базовый класс (например,
 *  объект класса SLogRecordModel)
 */
#ifndef SCOMRECORD_H
#define SCOMRECORD_H

#include "sdatabaserecord.h"
#include <QObject>
#include <QList>
#include <QStandardItem>
#include <QVariant>
#include "models/slogrecordmodel.h"

class SComRecord : public SDatabaseRecord
{
    Q_OBJECT
public:
    enum DataRoles {OldValue = Qt::UserRole, Changed = Qt::UserRole + 1};
    explicit SComRecord(QObject *parent = nullptr);
    ~SComRecord();
    void appendLogText(const QString&);
    void appendLogText(const QString&, const QString &disambiguation);
    void removeLogText(const QString &disambiguation);
protected:
    bool commitLogs();
    void initQueryFields(const QList<QStandardItem *> &record);
    virtual void setQueryField(const int, const QVariant, const QVariant){};
    QMap<QString, QString> *i_logTexts;
    SLogRecordModel *i_logRecord;

};

#endif // SCOMRECORD_H
