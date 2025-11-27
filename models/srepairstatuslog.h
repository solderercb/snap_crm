#ifndef SREPAIRSTATUSLOG_H
#define SREPAIRSTATUSLOG_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(created_at, created, QDateTime, 0)                          \
    TABLE_FIELD(repair_id, repair, int, 0)                                  \
    TABLE_FIELD(status_id, status, int, 0)                                  \
    TABLE_FIELD(user_id, user, int, 0)                                      \
    TABLE_FIELD(manager_id, manager, int, 0)                                \
    TABLE_FIELD(master_id, engineer, int, 0)

class SRepairStatusLog : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SRepairStatusLog(QObject *parent = nullptr);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void set_managerIndex(const int);
    void set_engineerIndex(const int);
    bool commit() override;
    bool isDirty() override;
    void setAllState(ModifiedField::State state) override;
private slots:
    void setDataRework(const int index, QVariant &data);
};

#endif // SREPAIRSTATUSLOG_H
