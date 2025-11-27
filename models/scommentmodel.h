#ifndef SCOMMENTMODEL_H
#define SCOMMENTMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
// Порядок полей изменён, он важен!
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(created, created, QDateTime, 0)                             \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(text, text, QString, 0)                                     \
    TABLE_FIELD(remont, repair, int, 0)                                     \
    TABLE_FIELD(client, client, int, 0)                                     \
    TABLE_FIELD(task_id, task, int, 0)                                      \
    TABLE_FIELD(part_request, partRequest, int, 0)

class SCommentModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    enum Mode {NotSet = 0, Repair = 1, Client, Task, PartRequest};
    explicit SCommentModel(QObject *parent = nullptr);
    ~SCommentModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    bool commit() override;
    void setObjId(const int mode, const int id);
    bool remove();
};

#endif // SCOMMENTMODEL_H
