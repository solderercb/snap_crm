#ifndef SFIELDVALUEMODEL_H
#define SFIELDVALUEMODEL_H

#include "scomrecord.h"
#include <QObject>

class SFieldValueModel : public SComRecord
{
    Q_OBJECT
public:
    explicit SFieldValueModel(QObject *parent = nullptr);
    explicit SFieldValueModel(const int, QObject *parent = nullptr);
    ~SFieldValueModel();
    void load(const int);
    void load(QSqlRecord);
    int id();
    void setId(const int);
    int fieldId();
    void setFieldId(const int);
    int repairId();
    void setRepairId(const int);
    int itemId();
    void setItemId(const int);
    QString value();
    bool commit();
    bool delDBRecord();
    bool isValid();
private:
    int m_id;
    int m_fieldId;
    QString m_name;
    int m_repairId;
    int m_itemId;
    QString m_value;
    bool m_isValid = 0;
public slots:
    void setValue(const QString &);
};

#endif // SFIELDVALUEMODEL_H
