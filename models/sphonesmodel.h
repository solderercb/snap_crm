#ifndef SPHONESMODEL_H
#define SPHONESMODEL_H

#include <QObject>
#include <QSqlQuery>
#include "sphonemodel.h"
#include "slogrecordmodel.h"

class SPhonesModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString primary READ primaryStr)
    Q_PROPERTY(QString phone2 READ phone2)
    Q_PROPERTY(QString phone3 READ phone3)
    Q_PROPERTY(QString allPhones READ allPhones)
    Q_PROPERTY(QString iterativePhone READ iterativePhone)
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
    QString primaryStr();
    QString phone2();
    QString phone3();
    QString allPhones();
    QString iterativePhone();
    void initDemo();
private:
    QSqlQuery *query;
    QList<SPhoneModel*> m_phonesList;
    QList<SPhoneModel*> m_removeList;
    SPhoneModel* sqlRecordHandler(const QSqlRecord &phone = QSqlRecord());
    SLogRecordModel *logRecord = nullptr;
    int m_client = 0;
    bool m_updated = false;
    qsizetype m_reportPhoneIndex = -1;
    void clear();
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
private slots:
    void switchPrimaryPhone(SPhoneModel*);
};

#endif // SPHONESMODEL_H
