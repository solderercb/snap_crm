#ifndef SUSERACTIVITYMODEL_H
#define SUSERACTIVITYMODEL_H

#include "sdatabaserecord.h"
#include <QObject>
#include <QDateTime>
#include <QHostInfo>

class SUserActivityModel : public SDatabaseRecord
{
    Q_OBJECT
public:
    explicit SUserActivityModel(QObject *parent = nullptr);
    ~SUserActivityModel();
    void setNotes(const QString);
    void appendRecord(const QString &notes);
    void appendRecordStandalone(const QString &notes);
private:
    int m_userId;
    QDateTime m_datetime;
    QString m_address;
    QString m_notes;
    QString m_appVersion;
    QString m_machineName;
    int id();
    void setId(const int);
    int userId();
    void setUserId(const int);
    QDateTime datetime();
    void setDatetime(const QDateTime);
    QString address();
    void setAddress(const QString);
    QString notes();
    QString appVersion();
    void setAppVersion(const QString);
    QString machineName();
    void setMachineName(const QString);
    bool commit() override;
    void queryNewId(int&) override {};    // после создания новой записи id не используется
    void queryLastInsertId() override {}; // id записи в журнале не нужен
};

#endif // SUSERACTIVITYMODEL_H
