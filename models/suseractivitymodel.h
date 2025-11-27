#ifndef SUSERACTIVITYMODEL_H
#define SUSERACTIVITYMODEL_H

#include <SSingleRowModel>
#include <QDateTime>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(user_id, userId, int, 0)                                    \
    TABLE_FIELD(datetime_, datetime, QDateTime, 0)                          \
    TABLE_FIELD(address, address, QString, 0)                               \
    TABLE_FIELD(notes, notes, QString, 0)                                   \
    TABLE_FIELD(app_version, appVersion, QString, 0)                        \
    TABLE_FIELD(machine_name, machineName, QString, 0)

class SUserActivityModel : public SSingleRowModel
{
    Q_OBJECT
public:
    explicit SUserActivityModel(QObject *parent = nullptr);
    ~SUserActivityModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void appendRecord(const QString &notes);
    void appendRecordStandalone(const QString &notes);
    void setAllState(ModifiedField::State state) override;
private:
    bool commit() override;
    void queryNewId() override {};    // после создания новой записи id не используется
};

#endif // SUSERACTIVITYMODEL_H
