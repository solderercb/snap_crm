#ifndef SLOGRECORDMODEL_H
#define SLOGRECORDMODEL_H

#include <QObject>
#include <QMap>
#include <QSqlQuery>
#include "global.h"
#include "com_sql_queries.h"
#include "models/sdatabaserecord.h"

class SLogRecordModel : public SDatabaseRecord
{
    Q_OBJECT
public:
    // типы записей для не входящие ни в какие группы (`logs`.`group` = NULL); Doc = 5 в АСЦ не использовался
    enum RecordTypes{ACP = 1, Client = 2, Repair = 3, User = 4, Doc = 5, Part = 6, PartRequest = 8};
    explicit SLogRecordModel(QObject *parent = nullptr);
    bool commit() override;
    bool commit(const QString &);
    void setText(const QString &);
    void setClient(int);
    void unsetClient(); // удаление модели клиента (анонимный)
    void setGroup(int);
    void setType(int);
    void setCurrencyId(int);
    void setRepairId(int);
    void setDocumentId(int);
    void setItemId(int);
    void setCashOrderId(int);
    void setPartRequestId(int);
    void setValueBefore(const QString &);
    void setValueAfter(const QString &);   // `logs`.`values_after`
    void setOffice(const int id);
    void setUser(const int id);
    void clear();
    void findNewId() override {};    // после создания новой записи id не используется
private:
};

#endif // SLOGRECORDMODEL_H
