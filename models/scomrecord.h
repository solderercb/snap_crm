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
#include <LimeReport>
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
    virtual void initDemo(){};
    virtual void setDirty(const bool state) override;
protected:
    virtual bool commit() override;
    virtual void dbErrFlagHandler(bool flushCache = true) override;
    bool commitLogs();
    void initQueryFields(const QList<QStandardItem *> &record);
    virtual void setQueryField(const int, const QVariant, const QVariant){};
    virtual void updateLogAssociatedRecId();
    QMap<QString, QString> *i_logTexts;
    SLogRecordModel *i_logRecord;
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
};

#endif // SCOMRECORD_H
