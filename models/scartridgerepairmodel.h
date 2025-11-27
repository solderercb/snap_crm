#ifndef SCARTRIDGEREPAIRMODEL_H
#define SCARTRIDGEREPAIRMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(refill, refill, bool, 0)                                    \
    TABLE_FIELD(chip, chip, bool, 0)                                        \
    TABLE_FIELD(opc_drum, drum, bool, 0)                                    \
    TABLE_FIELD(c_blade, blade, bool, 0)                                    \
    TABLE_FIELD(card_id, cardId, int, 0)

class SCartridgeRepairModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class SRepairModel;
public:
    explicit SCartridgeRepairModel(QObject *parent = nullptr);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    ~SCartridgeRepairModel();
    void load(const int id);
    void loadError(const int type) override;
    int repairId() const;
    void setRepairId(int id);

private:
    int m_repairId = 0;
};

#endif // SCARTRIDGEREPAIRMODEL_H
