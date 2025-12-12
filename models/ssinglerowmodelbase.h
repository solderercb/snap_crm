#ifndef SDATABASEAUXILIARY_NEW_H
#define SDATABASEAUXILIARY_NEW_H

#ifndef SKIP_INCLUDES_IN_PREPROCESSING_EVALUATION
#include <memory>
#include <QDate>
#include <QDateTime>
#include <QTimeZone>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <TdConn>
#endif // SKIP_INCLUDES_IN_PREPROCESSING_EVALUATION

class SSingleRowModelBase
{
    friend class TClassTest;
public:
    class ModifiedField;
    int size() const;
    bool isIndexValid(int index) const;
    virtual QVariant data(const int index, const int state = (ModifiedField::Updated|ModifiedField::Executed|ModifiedField::Commited)) const;
    std::optional<QVariant> commitedData(const int &index) const;
    virtual bool setData(const int index, const QVariant &data);
    virtual void signalDataToBeChanged(const int index, QVariant &data) = 0;
    virtual void signalDataChanged(const int index, QVariant &data) = 0;
    bool isFieldDirty(const int index);
    virtual bool isDirty();
    static QDate localDate(const QDate &utcDate);
    static QDate localDate(const QVariant &utcDate);
    static QDate utcDate(const QDate &localDate);
    static QDate utcDate(const QVariant &localDate);
    static QDateTime localDateTime(const QDateTime &utcDateTime);
    static QDateTime localDateTime(const QVariant &utcDateTime);
    static QDateTime utcDateTime(QDateTime localDateTime);
    static QDateTime utcDateTime(const QVariant &localDateTime);
    static QString fieldValueHandler(const QVariant &value);
    virtual void load();
    virtual void initFieldWithPrevLoaded(const int index, const QVariant &value);

    class ModifiedField{
        friend class TClassTest;
    public:
        enum State {None = 1, Updated = 2, Executed = 4, Failed = 8, Commited = 16}; // статус Commited равносилен Executed, см. описание метода setFieldsCommited()
        inline ModifiedField(State s = None, std::shared_ptr<ModifiedField> prev = nullptr) :
            m_state(None), m_prev_data(prev)
        {
            Q_ASSERT_X(s != Executed, "ModifiedField", "creating an object with such status makes no sense");
            setState(s);
        }
        inline State state() const {
            return m_state;
        }
        inline void setState(State s){
            if(m_state == s)
                return;
            m_state = s;
        }
        inline QVariant data() const {
            return m_data;
        }
        inline void setData(QVariant data){
            m_data = data;
        }
        inline std::shared_ptr<ModifiedField>& prev(){
            return m_prev_data;
        }
    private:
        State m_state = None;
        QVariant m_data;
        std::shared_ptr<ModifiedField> m_prev_data;
    };

    typedef QMap<int, std::shared_ptr<ModifiedField>> CacheMap;
    virtual void setState(const int index, ModifiedField::State state);
    virtual void setAllState(ModifiedField::State state);
    virtual void setFieldsCommited();
    virtual void setFieldsFailed();
protected:
    int sz = 0;
    std::unique_ptr<QSqlQuery> i_query;
    std::shared_ptr<QSqlDatabase> m_database;
    std::shared_ptr<QSqlRecord> m_record;
    std::unique_ptr<QMap<int, QString>> m_namesMap;      // список полей класса
    std::unique_ptr<QHash<QString, QString>> m_reportFieldsMap; // список полей для отчетов
    CacheMap cache;
    virtual void insertNewField(const int index, const QString &name);
    int fieldIndex(const QString &name) const;
    QString fieldName(const int index) const;
    virtual int indexOfCreated();
    virtual int dbFieldIndex(const int classIndex) const;
    virtual QString dbFieldName(const int classIndex) const;
    std::shared_ptr<QSqlDatabase> database() const;
    virtual void setDatabase(const QSqlDatabase &database);
    void initSqlQuery();
    virtual void initSqlQuery(QSqlDatabase database);
    QString fieldForSelectQuery(const int index);
    QString restrictedFieldForSelectQuery(const int index);
    virtual QString fieldsForSelectQuery();
    QString wrapSelectQueryFields(const QString &fields);
    virtual QString constructSelectQuery();
    virtual void loadError(const int type);
    virtual void convertTimestampFieldsToLocal();
    QString constructInsertQuery();
    QString fieldForUpdateQuery(const int index);
    QString prepareQueryUpdatePairs();
    QString wrapUpdateQueryPairs(const QString &setPairs);
    QString constructUpdateQuery();
    void setCacheData(const int index, const QVariant &value, ModifiedField::State newState = ModifiedField::Updated);
    virtual void setState(std::shared_ptr<ModifiedField> field, ModifiedField::State state);
    virtual bool skipField(CacheMap::ConstIterator f);
    bool isFieldDirty(CacheMap::ConstIterator f);
    virtual void setFieldsExecuted();
    virtual int primaryKeyIndex() = 0;
    virtual QVariant primaryKeyData() = 0;
    virtual bool isModelLoaded() const = 0;
    virtual QString table() = 0;
    virtual std::shared_ptr<QSqlRecord> rec() const = 0;
    virtual QVariant loadedValue(const int dbTableIndex) const;
    std::shared_ptr<ModifiedField>& cacheRef(const int index);
    bool isEqual(const QVariant& newData, const QVariant& oldData);
    int reportColumnCount();
    QVariant reportColumnHeader(const int index) const;
    void reportData(const int index, QVariant &data);
    void reportData(const QString &field, QVariant &data);
    virtual void reportMetaData(const QString &field, QVariant &data);
};

inline int SSingleRowModelBase::size() const
{
    return sz;
}

inline bool SSingleRowModelBase::isIndexValid(int index) const
{
    return index >= 0 && index < size();
}

inline int SSingleRowModelBase::fieldIndex(const QString &name) const
{
    return m_namesMap->key(name, -1);
}

inline QString SSingleRowModelBase::fieldName(const int index) const
{
    return m_namesMap->value(index);
}

inline int SSingleRowModelBase::dbFieldIndex(const int classIndex) const
{
    return classIndex;  // по умолчанию количество полей класса равно кол-ву полей таблицы БД
}

inline QString SSingleRowModelBase::dbFieldName(const int classIndex) const
{
    return rec()->fieldName(dbFieldIndex(classIndex));
}

inline QString SSingleRowModelBase::fieldForSelectQuery(const int index)
{
    return QString("  `%1`").arg(rec()->fieldName(index));
}

inline QString SSingleRowModelBase::restrictedFieldForSelectQuery(const int index)
{
    return QString("  '%1' AS '%2'").arg(QObject::tr("no permissions"), rec()->fieldName(index));
}

inline QString SSingleRowModelBase::fieldForUpdateQuery(const int index)
{
    return QString("  `%1` = ").arg(rec()->fieldName(index));
}

inline bool SSingleRowModelBase::isFieldDirty(CacheMap::ConstIterator f)
{
    return ((*f)->state() & (ModifiedField::Updated | ModifiedField::Failed)) != 0;
}

/* Возвращает значение из QSqlRecord (т. е. загруженное из БД)
 * Может быть переопределён в наследующем классе;
 * например, класс SComSettings взаимодействует с двумя таблицами БД, которые загружаются по отдельности.
*/
inline QVariant SSingleRowModelBase::loadedValue(const int dbTableIndex) const
{
    return rec()->value(dbTableIndex);
}

inline int SSingleRowModelBase::reportColumnCount()
{
    return m_reportFieldsMap->count();
}

inline QVariant SSingleRowModelBase::reportColumnHeader(const int index) const
{
    return m_reportFieldsMap->keys().value(index);
}

#endif // SDATABASEAUXILIARY_NEW_H
