#ifndef SPHONESMODEL_H
#define SPHONESMODEL_H

#include <QObject>
#include <QSqlQuery>
#include "sphonemodel.h"
#include "slogrecordmodel.h"

class SPhonesModel : public QObject
{
    Q_OBJECT
signals:
    void modelUpdated();
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
    bool isUpdated();
    void reset();
    void markUpdated(bool state = true);

private:
    QSqlQuery *query;
    QList<SPhoneModel*> m_phonesList;
    QList<SPhoneModel*> m_removeList;
    SPhoneModel* sqlRecordHandler(const QSqlRecord &phone = QSqlRecord());
    SLogRecordModel *logRecord = nullptr;
    int m_client = 0;
    bool m_updated = false;
    void clear();

private slots:
    void switchPrimaryPhone(SPhoneModel*);
};

#endif // SPHONESMODEL_H
