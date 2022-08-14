#ifndef SPHONESMODEL_H
#define SPHONESMODEL_H

#include <QObject>
#include <QSqlQuery>
#include "sphonemodel.h"

class SPhonesModel : public QObject
{
    Q_OBJECT
public:
    explicit SPhonesModel(int client = 0, QObject *parent = nullptr);
    ~SPhonesModel();
    SPhoneModel* primary();
    void setPrimary(SPhoneModel*);
    QList<SPhoneModel*> phonesList();
    bool load(int);
    void add(SPhoneModel*);
    void remove(SPhoneModel *phone);
    bool isEmpty();
    void setClient(const int id);
    bool commit();
    void clear();

private:
    QSqlQuery *query;
    QList<SPhoneModel*> m_phonesList;
    SPhoneModel* phoneItemHandler(const QSqlRecord &phone = QSqlRecord());

private slots:
    void switchPrimaryPhone(SPhoneModel*);
};

#endif // SPHONESMODEL_H
