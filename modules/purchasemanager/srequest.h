#ifndef SPARTREQUEST_H
#define SPARTREQUEST_H

#include "models/seditablebasemodel.h"
#include "models/scomrecord.h"
#include <QObject>

namespace ManagersModelFields
{
    enum Fields {Id = 0, Employee, RequestId};
};
namespace NotificationsModelFields
{
    enum Fields {Id = 0, Created, Employee, Type, Client, TaskId, RepairId, SMS, StoreRequest, BuyRequest, Subject, Text, Phone, State};
};

class SPartRequest : public SComRecord
{
    Q_OBJECT
public:
    // Статусы заявок в АСЦ: 1 - не начато; 2 - выполнено; 3 - отложено; 4 - в процессе; 5 - отменено
    enum State{NewUncommitted = -1, Created = 1, Finished = 2, Suspended = 3, InWork = 4, Cancelled = 5, InBasket = 11, OrderSubmitted = 12, OrderConfirmed = 13, Payed = 14, Shipped = 15};
    Q_ENUM(State);
    enum Priority{Regular = 1, High = 2, Low = 3};
    Q_ENUM(Priority);
    explicit SPartRequest(QObject *parent = nullptr);
    ~SPartRequest();
    static SStandardItemModel *statesList();
    static SStandardItemModel *prioritiesList();
    static SSqlQueryModel *managersList();
    int id();
    void setId(const int id);
    void load(const int &id);
    QDateTime createdUtc() override;
    void setCreated(const QDateTime &timestamp);
    int employee();
    void setEmployee(const int &id);
    int repair();
    void setRepair(const int &id);
    int client();
    void setClient(const int &id);
    int state();
    void setState(const int &newState);
    QDate endDate();
    void setEndDate(const QDate &date);
    QDate planEndDate();
    void setPlanEndDate(const QDate &date);
    double amount();
    void setAmount(const double &amount);
    QString tracking();
    void setTracking(const QString &tracking);
    int itemId();
    void setItemId(const int &id);
    QString name();
    void setName(const QString &name);
    QString notes();
    void setNotes(const QString &notes);
    QString url();
    void setUrl(const QString &url);
    int priority();
    void setPriority(const int &priority);
    int count();
    void setCount(const int &count);
    int dealer();
    void setDealer(const int &id);
    void updateAdditionalModel(SEditableBaseModel *model, QList<int> newList, const int columnUser);
    void updateAdditionalModelsFilters(const int id);
    QList<int> managers();
    void setManagers(QList<int> managers);
    void updateNotifications(QList<int> managers);
    void setDirty(const bool state) override;
    bool commit() override;
    bool commitManagers();
protected:
    int m_employee = userDbData->id;
    int m_repair = 0;
    int m_client = 0;
    int m_state = State::NewUncommitted;
    QDate m_endDate;
    QDate m_planEndDate;
    double m_amount = 0;
    QString m_tracking;
    int m_itemId = 0;
    QString m_name;
    QString m_notes;
    QString m_url;
    int m_priority = Priority::Regular;
    int m_count = 1;
    int m_dealer = 0;
    SEditableBaseModel *m_managers = nullptr;
    SEditableBaseModel *m_notifications = nullptr;
    void translateNames();
private:
    void dbErrFlagHandler(bool flushCache = true) override;
private slots:
    void managersPrimeInsert(int row, QSqlRecord &record);
    void notificationsPrimeInsert(int row, QSqlRecord &record);
};

#endif // SPARTREQUEST_H
