#ifndef SLOGRECORDMODEL_NEW_H
#define SLOGRECORDMODEL_NEW_H

#include <SSingleRowModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                \
    TABLE_FIELD(id, id, int, 0) \
    TABLE_FIELD(group, group, int, 0) \
    TABLE_FIELD(type, type, int, 0) \
    TABLE_FIELD(arh, arh, bool, 0) \
    TABLE_FIELD(user, user, int, 0) \
    TABLE_FIELD(created, created, QDateTime, 0) \
    TABLE_FIELD(values, values, QString, 0) \
    TABLE_FIELD(values_after, valuesAfter, QString, 0) \
    TABLE_FIELD(office, office, int, 0) \
    TABLE_FIELD(client, client, int, 0) \
    TABLE_FIELD(repair, repair, int, 0) \
    TABLE_FIELD(item, item, int, 0) \
    TABLE_FIELD(document, document, int, 0) \
    TABLE_FIELD(cash_order, cashOrder, int, 0) \
    TABLE_FIELD(part_request, partRequest, int, 0) \
    TABLE_FIELD(notes, text, QString, 0)

class SLogRecordModel : public SSingleRowModel
{
    Q_OBJECT
public:
    // типы записей для не входящие ни в какие группы (`logs`.`group` = NULL); Doc = 5 в АСЦ не использовался
    enum RecordTypes{ACP = 1, Client = 2, Repair = 3, User = 4, Doc = 5, Part = 6, PartRequest = 8};
    explicit SLogRecordModel(QObject *parent = nullptr);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    bool commit() override;
    bool commit(const QString &);
    void unsetClient(); // удаление модели клиента (анонимный)
    void setAllState(ModifiedField::State state) override;
private:
    void queryNewId() override {};
};

#endif // SLOGRECORDMODEL_NEW_H
