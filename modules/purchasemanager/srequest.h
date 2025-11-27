#ifndef SPARTREQUEST_H
#define SPARTREQUEST_H

#include <SSingleRowJModel>

class SEditableBaseModel;
class SStandardItemModel;
class SSqlQueryModel;

namespace ManagersModelFields
{
    enum Fields {Id = 0, Employee, RequestId};
};
namespace NotificationsModelFields
{
    enum Fields {Id = 0, Created, Employee, Type, Client, TaskId, RepairId, SMS, StoreRequest, BuyRequest, Subject, Text, Phone, State};
};

#include "../../models/ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(request_time, created, QDateTime, 0)                        \
    TABLE_FIELD(from_user, employee, int, 0)                                \
    TABLE_FIELD(repair, repair, int, 0)                                     \
    TABLE_FIELD(client, client, int, 0)                                     \
    TABLE_FIELD(state, state, int, 0)                                       \
    TABLE_FIELD(end_date, endDate, QDate, 0)                                \
    TABLE_FIELD(plan_end_date, planEndDate, QDate, 0)                       \
    TABLE_FIELD(summ, amount, double, 0)                                    \
    TABLE_FIELD(tracking, tracking, QString, 0)                             \
    TABLE_FIELD(item_id, itemId, int, 0)                                    \
    TABLE_FIELD(item_name, name, QString, 0)                                \
    TABLE_FIELD(notes, notes, QString, 0)                                   \
    TABLE_FIELD(url, url, QString, 0)                                       \
    TABLE_FIELD(pririty, priority, int, 0)                                  \
    TABLE_FIELD(count, count, int, 0)                                       \
    TABLE_FIELD(dealer, dealer, int, 0)

class SPartRequest : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
public:
    // Статусы заявок в АСЦ: 1 - не начато; 2 - выполнено; 3 - отложено; 4 - в процессе; 5 - отменено
    enum State{NewUncommitted = -1, Created = 1, Finished = 2, Suspended = 3, InWork = 4, Cancelled = 5, InBasket = 11, OrderSubmitted = 12, OrderConfirmed = 13, Payed = 14, Shipped = 15};
    Q_ENUM(State);
    enum Priority{Regular = 1, High = 2, Low = 3};
    Q_ENUM(Priority);
    explicit SPartRequest(QObject *parent = nullptr);
    ~SPartRequest();
#include "../../models/ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    static SStandardItemModel *statesList();
    static SStandardItemModel *prioritiesList();
    static SSqlQueryModel *managersList();
    void load() override;
    void load(const int &id, QSqlDatabase &db);
    void load(const int &id);
    static QString logMsgStateChange(const int &newState);
    static QString logMsgPlanEndDateChange(const QDate &date);
    static QString logMsgAmountChange(const double &amount);
    static QString logMsgTrackingChange(const QString &newTrack, const QString &oldTrack = QString());
    static QString logMsgPriorityChange(const int &priority);
    static QString logMsgCountChange(const int &count);
    QList<int> managers();
    void setManagers(QList<int> managers);
    void updateNotifications(QList<int> managers);
    void updateAdditionalModel(SEditableBaseModel *model, QList<int> newList, const int columnUser);
    void updateAdditionalModelsFilters(const int id);
    bool commit() override;
    bool isManagersModelDirty();
    bool commitManagers();
protected:
    SEditableBaseModel *m_managers = nullptr;
    SEditableBaseModel *m_notifications = nullptr;
    int indexOfCreated() override;
    QString constructSelectQuery() override;
    void stateChanged(const int newState);
    void planEndDateChanged(const QDate &date);
    void amountChanged(const double &amount);
    void trackingChanged(const QString &tracking);
    void priorityChanged(const int &priority);
    void countChanged(const int &count);
    void logDataChange(const int index, const QVariant &data) override;
    void translateNames();
private:
    void updateLogAssociatedRecId() override;
private slots:
    void managersPrimeInsert(int row, QSqlRecord &record);
    void notificationsPrimeInsert(int row, QSqlRecord &record);
    void setDataRework(const int index, QVariant &data);
};


inline int SPartRequest::indexOfCreated()
{
    return C_created;
}

#endif // SPARTREQUEST_H
