#ifndef SCARTRIDGEMATERIALMODEL_H
#define SCARTRIDGEMATERIALMODEL_H

#include "scomrecord.h"
#include <QObject>

class SCartridgeMaterialModel : public SComRecord
{
    Q_OBJECT
public:
    enum Type{Toner = 0, Chip = 2, Blade = 3, Drum = 1}; // в таком порядке добавляются ЗИП в карточке картриджа
    Q_ENUM(Type)
    explicit SCartridgeMaterialModel(QObject *parent = nullptr);
    int id();
    void load(const int id);
    int type();
    void setType(const int);//type
    int count();
    void setCount(const int);//count
    int articul();
    void setArticul(const int);//articul
    QString name();
    void setName(const QString&);//name
    int cardId();
    void setCardId(const int);//card_id
    double price();
    void setPrice(const double);//price
    double worksPrice();
    void setWorksPrice(const double);//works_price
private:
    int m_type;
    int m_count;
    int m_articul;
    QString m_name;
    int m_cardId;
    double m_price;
    double m_worksPrice;
};

#endif // SCARTRIDGEMATERIALMODEL_H
