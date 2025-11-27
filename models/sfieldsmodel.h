#ifndef SFIELDSMODEL_H
#define SFIELDSMODEL_H

#include <memory>
#include <QObject>
#include <QList>
#include <QSqlDatabase>
#include <LimeReport>

class QSqlQuery;
class QSqlRecord;
class SFieldModel;
class FieldFactory;
class SFieldValueModel;

class SFieldsModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ reportFieldName)
    Q_PROPERTY(QString value READ reportFieldValue)
    friend class TClassTest;
signals:
public:
    explicit SFieldsModel(std::unique_ptr<FieldFactory> factory, QObject *parent = nullptr);
    ~SFieldsModel();
    QList<std::shared_ptr<SFieldModel>> entitiesList();
    QList<std::shared_ptr<SFieldValueModel>> valuesList();
    void setSqlQuery(std::shared_ptr<QSqlQuery> query);
    void initSqlQuery();
    void initSqlQuery(QSqlDatabase database);
    bool init();
    bool init(const int);
    bool load(int);
    void add(std::shared_ptr<SFieldValueModel> item);
    void setFieldFactory(std::unique_ptr<FieldFactory> factory);
    void setOwnerId(const int id);
    bool commit();
    void clear();
    bool validate();
    void setAllFailed();
    void enableEdit(const int cat);
    int printableFieldsCount();
    QString reportFieldName();
    QString reportFieldValue();
    void initDemo();
#ifdef QT_DEBUG
    void randomFill();
#endif

private:
    std::shared_ptr<QSqlQuery> m_query;
    QSqlDatabase m_database;
    QList<std::shared_ptr<SFieldValueModel>> m_valueModels;
    QList<std::shared_ptr<SFieldValueModel>> m_removeList;
    std::unique_ptr<FieldFactory> m_fieldFactory;
    int m_category = 0;
    int m_ownerId = 0;
    bool m_initDone = 0;
    qsizetype m_reportFieldIndex = -1;
    void initValueModel();
    void initValueModel(const QSqlRecord &rec);
    void updateValueModelOwner();
    void initFieldsHandlers();
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
};


#endif // SFIELDSMODEL_H
