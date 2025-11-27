#ifndef SDATABASERECORD_NEW_H
#define SDATABASERECORD_NEW_H
#include <QObject>
#include <memory>
#include <SSingleRowModelBase>
#include <QSqlError>
#include <QSqlRecord>

class QSqlDatabase;
class QSqlQuery;

#define MODEL_FIELD_GET(name) name
#define MODEL_FIELD_SET(name) set_##name

#define REPORT_FIELD(fieldName)                                                                     \
    m_reportFieldsMap->insert(#fieldName, QString());

/* Макрос для объявления дополнительных полей для отчетов.
 * В качестве параметров указываются имя поля в отчете и название метода, отдающего
 * данные; методы должны быть помечены как public Q_SLOTS (их вызов происходит с
 * помощью QMetaObject::invokeMethod); тип возвращаемых данных должен быть QVariant.
*/
#define ADDITIONAL_REPORT_FIELD(fieldName, slotName)                                                \
    m_reportFieldsMap->insert(#fieldName, #slotName);

#define OPT_0(...)
#define OPT_1(name) REPORT_FIELD(name)
#define OPT_SELECTOR(cond) OPT_##cond
// Макросa для инициализации массивов
#define REGISTER_ENUM_FIELD(name, ...)  C_##name,
#define REGISTER_TABLE_FIELD(name) insertNewField(index++, #name);
#define REGISTER_CLASS_FIELD(name, ...) m_namesMap->insert(index++, #name);
#define REGISTER_REPORT_FIELD(name, type, cond) OPT_SELECTOR(cond)(name)

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#undef DECLARE_GETTER_AND_SETTER
#define DECLARE_GETTER_AND_SETTER(name, type, ...)                                                  \
public:                                                                                             \
    inline type MODEL_FIELD_GET(name)() const                                                       \
    {                                                                                               \
        return data(C_##name).value<type>();                                                        \
    }                                                                                               \
    inline void MODEL_FIELD_SET(name)(const type &data)                                             \
    {                                                                                               \
        setData(C_##name, data);                                                                    \
    }

// Макросы для подсчета элементов
#define COUNT_TABLE_FIELDS(...) sizeof((const QString[]){__VA_ARGS__})/sizeof(const QString)
#define GET_ELEMENT_COUNT() COUNT_TABLE_FIELDS(TABLE_FIELDS)

class SSingleRowModel : public QObject, public SSingleRowModelBase
{
    Q_OBJECT
    friend class SSingleRowJModel;
    friend class TClassTest;
signals:
    void beginDataChange(const int index, QVariant &data);
    void dataChanged(const int index, QVariant &data);
private: signals:
    void modelAboutToBeUpdated();
    void modelUpdated();
public:
    enum EditStrategy {OnFieldChange, OnManualSubmit};
    explicit SSingleRowModel(QObject *parent = nullptr);
    ~SSingleRowModel();
    bool isPrimaryKeyValid() const;
    virtual bool setData(const int index, const QVariant &data) override;
    void setTable(const QString &table);
    int primaryKeyIndex() override;
    virtual void setPrimaryKeyIndex(const int index);
    void setPrimaryKey(const QVariant &data);
    void setEditStrategy(int strategy);
    void setQueryIdBeforeInsert(bool state);
    virtual int flags(const int index);
    virtual void load() override;
    const QVariant operator[](int index);
    virtual bool commit();
    virtual QDateTime createdUtc();
    QString createdStr();
    static bool checkSystemTime();
    int cacheSize();
    void clearCache();
    void clearEverything();
    QSqlError lastError();
    void beginUpdateModel();
    void endUpdateModel();
protected:
    int i_primaryKeyIndex = 0;
    QString i_tableName;
    QStringList i_obligatoryFields = {};   // поля не имеющие значения по умолчанию
    int m_editStrategy = OnManualSubmit;
    bool m_queryIdBeforeInsert = 1;
    virtual void mapFields() = 0;
    virtual void queryNewId();
    virtual void queryLastInsertId();
    void signalDataToBeChanged(const int index, QVariant &data) override;
    void signalDataChanged(const int index, QVariant &data) override;
    bool insert();
    bool update();
    bool del();
    virtual void dbErrFlagHandler(bool);
    void initMandatoryField(const int index, const QVariant &data);
    bool checkObligatoryFields();
    bool checkTableName();
    QVariant primaryKeyData() override;
    bool isModelLoaded() const override;
    QString table() override;
    std::shared_ptr<QSqlRecord> rec() const override;
    void reportMetaData(const QString &field, QVariant &data) override;
protected slots:
    virtual void logDataChange(const int, const QVariant &){};
};

inline void SSingleRowModel::initMandatoryField(const int index, const QVariant &data)
{
    if(!isFieldDirty(index))
        setData(index, data);
}

inline int SSingleRowModel::primaryKeyIndex()
{
    return i_primaryKeyIndex;
}

inline QVariant SSingleRowModel::primaryKeyData()
{
    return data(primaryKeyIndex());
}

/* Признак, что все поля модели инициализированы данными из БД (т. е.
 * вызывался метод load().
 * В "полной" модели возвращаемый пустой QVariant
 * соответствует значению NULL в базе и должен сохраняться как предыдущее
 * значение.
 * В неполной модели любой элемент m_record инициализируется пустым
 * QVariant и использовать его при сравнении старого значения с новым нельзя.
*/
inline bool SSingleRowModel::isModelLoaded() const
{
    return m_record->value(i_primaryKeyIndex).isValid();
}

inline QString SSingleRowModel::table()
{
    return i_tableName;
}

inline std::shared_ptr<QSqlRecord> SSingleRowModel::rec() const
{
    return m_record;
}

#endif // SDATABASERECORD_NEW_H
