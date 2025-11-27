#ifndef SCARTRIDGEMATERIALMODEL_H
#define SCARTRIDGEMATERIALMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(type, type, int, 0)                                         \
    TABLE_FIELD(count, count, int, 0)                                       \
    TABLE_FIELD(articul, articul, int, 0)                                   \
    TABLE_FIELD(name, name, QString, 0)                                     \
    TABLE_FIELD(card_id, cardId, int, 0)                                    \
    TABLE_FIELD(price, price, double, 0)                                    \
    TABLE_FIELD(works_price, worksPrice, double, 0)                         \
    TABLE_FIELD(salary_summ, salarySumm, double, 0)

class SCartridgeMaterialModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    enum Type{Toner = 0, Chip = 2, Blade = 3, Drum = 1, Replace = 4, TonerPlus, MagRoller,  Prophilaxy, PrimRoller, Marker, Scraper, Bushings}; // в таком порядке добавляются ЗИП в карточке картриджа; коды 0..4 используются в АСЦ, остальные добавлены на основании статистики
    Q_ENUM(Type)
    explicit SCartridgeMaterialModel(QObject *parent = nullptr);
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load(const int id);
    QString workName();
    void setInitializerCardId(const int id);

private:
    int m_initializerCardId = 0;    // см. комментарий к сеттеру
    QString m_workName;
    void initWorkName(const Type type);
    void loadError(const int type);
};

#endif // SCARTRIDGEMATERIALMODEL_H
