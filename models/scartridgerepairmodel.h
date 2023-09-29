#ifndef SCARTRIDGEREPAIRMODEL_H
#define SCARTRIDGEREPAIRMODEL_H

#include "scomrecord.h"
#include <QObject>
#include "widgets/shortlivednotification.h"

class SCartridgeRepairModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SCartridgeRepairModel(QObject *parent = nullptr);
    explicit SCartridgeRepairModel(const int id, QObject *parent = nullptr);
    ~SCartridgeRepairModel();
    int id();
    void load(const int id);
    void loadError();
    bool commit();
    bool refill();
    void setRefill(const bool);
    bool chip();
    void setChip(const bool);
    bool drum();
    void setDrum(const bool);
    bool Blade();
    void setBlade(const bool);
    int cardId();
    void setCardId(const int);
private:
    bool m_refill = 0;
    bool m_chip = 0;
    bool m_drum = 0;
    bool m_Blade = 0;
    int m_cardId = 0;
};

#endif // SCARTRIDGEREPAIRMODEL_H
