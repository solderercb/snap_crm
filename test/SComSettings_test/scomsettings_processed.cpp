#include "scomsettings_processed.h"
#include <QTimeZone>
#include <SStandardItemModel>
#include <ProjectGlobals>
#include <ProjectQueries>

/* Иницилизация QSqlRecord при создании объекта.
 * Названия полей будут использоваться для формирования запросов
 * Может быть переопределён.
*/
void SSingleRowModelBase::insertNewField(const int index, const QString &name)
{
    QSqlField f;
    f.setName(name);
    m_record->append(f);
}

inline bool SSingleRowModelBase::isEqual(const QVariant &newData, const QVariant &oldData)
{
    return newData == oldData && newData.isNull() == oldData.isNull();
}

/* Возвращает ссылку на объект в кэше.
 * Если объект не существует, то инициализирует новый.
*/
std::shared_ptr<SSingleRowModelBase::ModifiedField> &SSingleRowModelBase::cacheRef(const int index)
{
    std::shared_ptr<ModifiedField> &f = cache[index];
    if(!f.get())
        f = std::make_shared<ModifiedField>();

    return f;
}

/* Значение, загруженное из БД или изменённое (из кэша) */
QVariant SSingleRowModelBase::data(const int index, const int state) const
{
    QVariant v;
    if (!isIndexValid(index))
        return v;

    const auto f = cache.constFind(index);
    if (f != cache.constEnd() && ((*f)->state() & state))
        return (*f)->data();

    int dbi = dbFieldIndex(index);
    if(dbi >= 0)
        return loadedValue(dbi);

    return v;
}

/* Значение, гарантированно соответствующее текущему значению в БД */
std::optional<QVariant> SSingleRowModelBase::commitedData(const int &index) const
{
    if (!isIndexValid(index))
        return std::nullopt;

    const auto f = cache.constFind(index);
    if (f != cache.constEnd())
    {
        if((*f)->state() & (ModifiedField::Executed | ModifiedField::Commited))
            return (*f)->data();
        else if ((*f)->state() & ModifiedField::Updated && (*f)->prev().get())
            return (*f)->prev()->data();
    }

    int dbi = dbFieldIndex(index);
    if((dbi >= 0) && isModelLoaded())
        return loadedValue(dbi);

    return std::nullopt;
}

bool SSingleRowModelBase::setData(const int &index, const QVariant &value)
{
    if (!isIndexValid(index))
        return false;

    QVariant newValue = value;
    signalDataToBeChanged(index, newValue);

    std::shared_ptr<ModifiedField> &f = cacheRef(index);

    if(f->state() & ModifiedField::Updated)
    {
       if(isEqual(newValue, f->data()))
           return true;

       if(f->prev().get() && isEqual(newValue, f->prev()->data()))
           f = f->prev();
       else
           f->setData(newValue);

       signalDataChanged(index, newValue);
       return true;
    }

    const QVariant oldValue = data(index, ModifiedField::Executed|ModifiedField::Commited);
    if(!isModelLoaded())
    {
        if(!(f->state() & (ModifiedField::Executed|ModifiedField::Commited)))
        {
            f->setData(newValue);
            f->setState(ModifiedField::Updated);
            signalDataChanged(index, newValue);
            return true;
        }
    }

    if(isEqual(newValue, oldValue))
        return true;

    if (f->state() & (ModifiedField::None|ModifiedField::Executed|ModifiedField::Commited))
    {
        if(f->state() & ModifiedField::None)
            f->setData(oldValue);

        f = std::make_shared<ModifiedField>(ModifiedField::Updated, f);
    }

    f->setData(newValue);
    f->setState(ModifiedField::Updated);
    signalDataChanged(index, newValue);

    return true;
}

QDate SSingleRowModelBase::localDate(const QDate &utcDate)
{
    return localDateTime(utcDate.startOfDay(QTimeZone::utc())).date();
}

QDate SSingleRowModelBase::localDate(const QVariant &utcDate)
{
    return localDate(utcDate.toDate());
}

QDate SSingleRowModelBase::utcDate(const QDate &localDate)
{
    return utcDateTime(localDate.startOfDay()).date();
}

QDate SSingleRowModelBase::utcDate(const QVariant &localDate)
{
    return utcDate(localDate.toDate());
}

QDateTime SSingleRowModelBase::localDateTime(const QDateTime &utcDateTime)
{
    QDateTime t = utcDateTime;
    t.setTimeZone(QTimeZone::utc());
    return t.toLocalTime();
}

QDateTime SSingleRowModelBase::localDateTime(const QVariant &utcDateTime)
{
    return localDateTime(utcDateTime.toDateTime());
}

QDateTime SSingleRowModelBase::utcDateTime(QDateTime localDateTime)
{
    localDateTime.setTimeZone(QTimeZone::systemTimeZone());
    return localDateTime.toUTC();
}

QDateTime SSingleRowModelBase::utcDateTime(const QVariant &localDateTime)
{
    return utcDateTime(localDateTime.toDateTime());
}

QString SSingleRowModelBase::fieldValueHandler(const QVariant &value)
{
    if(value.isNull())
        return "NULL";

    QString str_value;
    switch (value.type())
    {
        case QVariant::Invalid: str_value = "NULL"; break;
        case QVariant::String:
        case QVariant::ByteArray:
        case QVariant::UserType: str_value = "'" + value.toString().replace('\'',"\\\'") + "'"; break;
        case QVariant::DateTime: {
            QDateTime dt = value.toDateTime();
            if(dt.timeZone() != QTimeZone::utc())
            {
                dt.setTimeZone(QTimeZone::systemTimeZone());
                dt =  dt.toUTC();
            }
            str_value =  "'" + dt.toString("yyyy-MM-dd hh:mm:ss") + "'";
            break;
        }
        case QVariant::Date: str_value =  "'" + value.toDate().toString("yyyy-MM-dd") + "'"; break;
        case QVariant::Bool:
        case QVariant::Int: str_value = QString::number(value.toInt()); break;
        case QVariant::Double: str_value = QString::number(value.toDouble(), 'f', 4); break;
        default: str_value = value.toString(); break;
    }

    return str_value;
}

/* Загрузка всех полей записи с заданным первичным ключом
 * Названия полей будут взяты из текущего объекта m_record, после успешного
 * выполнения запроса этот объект будет заменён.
*/
void SSingleRowModelBase::load()
{
    initSqlQuery();

    i_query->exec(constructSelectQuery());

    if(!i_query->first())
        loadError(Global::ThrowType::ResultError);

    m_record = std::make_shared<QSqlRecord>(i_query->record());
    convertTimestampFieldsToLocal();
    cache.clear();  // очистка кэша в конце, т. к. в нём может храниться primaryKey
}

void SSingleRowModelBase::loadError(const int type)
{
    Global::throwError(type);
}

int SSingleRowModelBase::indexOfCreated()
{
    return fieldIndex("created");
}

/* В БД все таймштампы в UTC, в коде — локальное время.
 * Если виджет обновляется значением модели, которое было задано ранее, а
 * модель при этом не перезагружалась, то createdStr() вернёт неправильное время.
 * Этот метод должен вызываться только при загрузке данных из БД.
*/
void SSingleRowModelBase::convertTimestampFieldsToLocal()
{
    int index = indexOfCreated();
    if(index == -1)
        return;

    m_record->setValue(index, localDateTime(m_record->value(index).toDateTime()));
}

std::shared_ptr<QSqlDatabase> SSingleRowModelBase::database() const
{
    return m_database;
}

void SSingleRowModelBase::setDatabase(const QSqlDatabase &database)
{
    m_database = std::make_shared<QSqlDatabase>(database);
    i_query = std::make_unique<QSqlQuery>(*(m_database.get()));
}

void SSingleRowModelBase::initSqlQuery()
{
    initSqlQuery(QSqlDatabase::database("connThird"));
}

void SSingleRowModelBase::initSqlQuery(QSqlDatabase database)
{
    if(i_query)
        return;

    if(!m_database)
        m_database = std::make_shared<QSqlDatabase>(database);

    i_query = std::make_unique<QSqlQuery>(*(m_database.get()));
}

QString SSingleRowModelBase::fieldsForSelectQuery()
{
    QString fields;
    for(int i = 0; i < rec()->count(); i++)
    {
//        if(i == primaryKey())
//            continue;

        if(!fields.isEmpty())
            fields.append(",\n");

        fields.append(fieldForSelectQuery(i));
    }

    return fields;
}

QString SSingleRowModelBase::wrapSelectQueryFields(const QString &fields)
{
    QString query = fields;
    query.prepend("SELECT\n");
    query.append(QString("\nFROM `%1` WHERE `%2` = %3;")
                 .arg(table())
                 .arg(rec()->fieldName(primaryKeyIndex()))
                 .arg(fieldValueHandler(primaryKeyData())));

    return query;
}

QString SSingleRowModelBase::constructSelectQuery()
{
    return wrapSelectQueryFields(fieldsForSelectQuery());
}

/* Проверка текущего элемента кэша на соответствие условиям пакетной обработки.
 * Метод базового класса всегда возвращает 0 (поле не пропускается)
*/
bool SSingleRowModelBase::skipField(CacheMap::ConstIterator f)
{
    return dbFieldIndex(f.key()) < 0;
}

/* Проверка кэша на наличие изменённых данных.
*/
bool SSingleRowModelBase::isDirty()
{
    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(skipField(i))
            continue;

//        if(i.key() == primaryKeyIndex())
//            continue;

        if(isFieldDirty(i))
            return true;
    }
    return false;
}

bool SSingleRowModelBase::isFieldDirty(const int index)
{
    auto f = cache.constFind(index);
    if(f == cache.constEnd() || !((*f)->state() & (ModifiedField::Updated | ModifiedField::Failed)))
        return 0;

    return 1;
}

QString SSingleRowModelBase::constructInsertQuery()
{
    QString fields;
    QString values;
    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(skipField(i) || !isFieldDirty(i))
            continue;

        if(!fields.isEmpty())
            fields.append(",\n");
        fields.append("  `" + rec()->fieldName(i.key()) + '`');

        if(!values.isEmpty())
            values.append(",\n");
        values.append("  " + fieldValueHandler((*i)->data()));
    }

    QString q = QString("INSERT INTO `%1`(\n%2\n) VALUES (\n%3\n);").arg(table(), fields, values);
    //    qDebug().noquote() << q;
    return q;
}

QString SSingleRowModelBase::prepareQueryUpdatePairs()
{
    QString setPairs;
    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(skipField(i) || !isFieldDirty(i))
            continue;

        int index = i.key();
        if(index == primaryKeyIndex())
            continue;

        if(!setPairs.isEmpty())
            setPairs.append(",\n");
        setPairs.append("  `" + dbFieldName(index) + "` = " + fieldValueHandler((*i)->data()));
    }

    return setPairs;
}

QString SSingleRowModelBase::wrapUpdateQueryPairs(const QString &setPairs)
{
    QString q = QString("UPDATE\n  `%1`\nSET\n%2\nWHERE `%3` = %4;").arg(
                            table(),
                            setPairs,
                            rec()->fieldName(primaryKeyIndex()))
                            .arg(fieldValueHandler(primaryKeyData()));

    return q;
}

QString SSingleRowModelBase::constructUpdateQuery()
{
    return wrapUpdateQueryPairs(prepareQueryUpdatePairs());
}

/* Запись значения value непосредственно в кэш.
 * Метод предназначен для сохранения значения в кэш без дополнительных действий по сигналу dataChanged;
 * с помощью newState можно сразу задать состояние, по умолчанию Updated.
 * Пример: в классе SComSettings использутеся для парсинга данных, представленных в виде JSON, и сохранения
 * отдельных значений в поля класса, не сопоставленные с полями таблицы БД.
*/
void SSingleRowModelBase::setCacheData(const int index, const QVariant &value, ModifiedField::State newState)
{
    std::shared_ptr<ModifiedField> &f = cacheRef(index);
    f->setData(value);
    f->setState(newState);
}

void SSingleRowModelBase::setState(const int index, ModifiedField::State state)
{
    auto f = cache.constFind(index);
    if(f == cache.constEnd())
        return;

    setState(*f, state);
}

void SSingleRowModelBase::setState(std::shared_ptr<ModifiedField> field, ModifiedField::State state)
{
    if(field->state() == ModifiedField::None)
        return;

    field->setState(state);
    if(state & (ModifiedField::Executed|ModifiedField::Commited))
        field->prev().reset();

}

void SSingleRowModelBase::setAllState(ModifiedField::State state)
{
    CacheMap::iterator i = cache.begin();
    CacheMap::iterator e = cache.end();
    for (; i != e; ++i)
    {
        if(skipField(i) || (*i)->state()&ModifiedField::Commited)   // статус Commited изменять нельзя
            continue;

        setState(*i, state);
    }
}

/* Установка статуса Commited всем элементам кэша.
 * Статус указывает, что данные гарантированно записаны в БД (транзакция закрыта запросом COMMIT).
 * По значимости он равносилен Executed, но в отдельных сценариях может быть использован для обработки
 * сбоев без перезагрузки данных из БД. Однако, перезагрузка надёжнее и универсальнее (например, в базе
 * могут быть триггеры).
 *
 */
void SSingleRowModelBase::setFieldsCommited()
{
    setAllState(ModifiedField::Commited);
}

void SSingleRowModelBase::setFieldsExecuted()
{
    setAllState(ModifiedField::Executed);
}

void SSingleRowModelBase::setFieldsFailed()
{
    setAllState(ModifiedField::Failed);
}

void SSingleRowModelBase::reportData(const int index, QVariant &data)
{
    data = this->data(index);
}

void SSingleRowModelBase::reportData(const QString &field, QVariant &data)
{
    int id = m_namesMap->key(field, -1);
    if(id >= 0)
        data = this->data(id);
    else
        reportMetaData(field, data);
}

/* Виртуальный метод, передеющий через data значение дополнительного поля отчета.
 * Переопределён в наследующих классах, т. к. использует мета-систему.
*/
void SSingleRowModelBase::reportMetaData(const QString &field, QVariant &data)
{
}

SSettingsBase::SSettingsBase()
{
    m_namesMap = std::make_unique<QMap<int, QString>>();
    m_record = std::make_shared<QSqlRecord>();
    m_reportFieldsMap = std::make_unique<QHash<QString, QString>>();
    connect(this, &SSettingsBase::dataChanged, this, &SSettingsBase::dataChangeHandler);
}

void SSettingsBase::setPrimaryKey(const QVariant &data)
{
    std::shared_ptr<ModifiedField> &f = cacheRef(primaryKeyIndex());
    f->setData(data);
    f->setState(ModifiedField::Executed);
}

void SSettingsBase::disableWidget(const int index)
{
    i_editorWidgets.value(index)->setEnabled(false);
}

QLabel *SSettingsBase::label(const int index)
{
    return i_labelWidgets.value(index);
}

QWidget *SSettingsBase::widget(const int index)
{
    return i_editorWidgets.value(index);
}

void SSettingsBase::load()
{
    SSingleRowModelBase::initSqlQuery(QSqlDatabase::database("connMain"));
    SSingleRowModelBase::load();
}

int SSettingsBase::widgetGroup(const int index)
{
    return i_propertyGroup.value(index);
};

/* Преобразование значения поля таблицы БД в индекс модели данных и переключение ComboBox
*  Настройки, которые редактируются с помощью ComboBox'ов в базе могут быть как минимум двух типов: число и строка;
*  например, валюта записывается в виде трёхбуквенного международного кода, а срок гарантии в виде количества дней.
*/
void SSettingsBase::setComboBoxIndex(QComboBox *cb, const QVariant &dbValue)
{
    SStandardItemModel *mdl = dynamic_cast<SStandardItemModel*>(cb->model());
    int index = -1;
    if(mdl)
        switch (dbValue.type())
        {
            case QVariant::Int: index = mdl->valueColumnToRow<int>(dbValue.toInt(), 1); break;
            case QVariant::String: index = mdl->valueColumnToRow<QString>(dbValue.toString(), 1); break;
            default: break;
        }
    else
        index = dbValue.toInt();

    cb->setCurrentIndex(index);
}

/* Преобразование индекса модели данных ComboBox'а в значение поля таблицы БД
*/
QVariant SSettingsBase::dbValueFromComboBox(QComboBox *cb, const int index)
{
    if(!isModelLoaded())
        return data(index);

    QVariant widgetValue;
    SStandardItemModel *mdl = dynamic_cast<SStandardItemModel*>(cb->model());
    if(mdl)
    {
        // Кастуем данные к тому же типу, что задан в QSqlRecord (тип инициализируется при загрузке записи из таблицы).
        QVariant::Type type = rec()->value(dbFieldIndex(index)).type();
        switch(type)
        {
            case QVariant::Int: widgetValue = mdl->rowColumnToValue<int>(cb->currentIndex(), 1); break;
            case QVariant::String: widgetValue = mdl->rowColumnToValue<QString>(cb->currentIndex(), 1); break;
            default: break;
        }
    }
    else
        widgetValue = cb->currentIndex();
                                                                                                    \
    if(!widgetValue.isValid())
        widgetValue = i_editorWidgetsDefaults.value(index);

    return widgetValue;
}

void SSettingsBase::setWidgetsData()
{
    QWidget *w;

    foreach (auto k, i_editorWidgets.keys())
    {
        w = i_editorWidgets[k];
        if(!w->isEnabled())
            continue;

        switch (i_editorWidgetsTypes[k])
        {
            case WidgetTypes::LineEdit: static_cast<QLineEdit*>(w)->setText(data(k).toString()); break;
            case WidgetTypes::ComboBox: setComboBoxIndex(static_cast<QComboBox*>(w), data(k)); break;
            case WidgetTypes::SCheckableComboBox: static_cast<SCheckComboBox*>(w)->setChecked(data(k).toString()); break;
//            case WidgetTypes::SColorPickBox: static_cast<SColorPickBox*>(w)->(data(k).to()); break; // не реализован виджет
            case WidgetTypes::CheckBox: static_cast<QCheckBox*>(w)->setCheckState((Qt::CheckState)(data(k).toInt()?2:0)); break;
            case WidgetTypes::SpinBox: static_cast<QSpinBox*>(w)->setValue(data(k).toInt()); break;
            case WidgetTypes::DoubleSpinBox: static_cast<QDoubleSpinBox*>(w)->setValue(data(k).toDouble()); break;
            case WidgetTypes::RadioButton: static_cast<QRadioButton*>(w)->setChecked(data(k).toInt()); break;
            case WidgetTypes::DateTimeEdit: static_cast<QDateTimeEdit*>(w)->setDateTime(data(k).toDateTime()); break;
            default: break;
        }
    }
}

bool SSettingsBase::skipField(CacheMap::ConstIterator f)
{
    return skipField(f, 0);
}

/* Проверка текущего элемента кэша на соответствие условиям пакетной обработки.
 * tableIndex используется для фильтрации, если класс работает более чем с одной таблицей.
*/
bool SSettingsBase::skipField(CacheMap::ConstIterator f, const int targetTable)
{
    int targetTableIndex = dbFieldIndex(f.key());
    if(targetTableIndex < 0)
        return 1;
    if(this->targetTable(targetTableIndex) != targetTable)
        return 1;

    return 0;
}

/* Перегруженный виртуальный метод.
*/
bool SSettingsBase::isDirty()
{
    return isDirty(0);
}

/* Проверка кэша на наличие изменённых данных.
 * targetTable используется для фильтрации полей, если класс работает более чем с одной таблицей.
*/
bool SSettingsBase::isDirty(const int targetTable)
{
    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(skipField(i, targetTable))
            continue;

//        if(i.key() == primaryKeyIndex())
//            continue;

        if(isFieldDirty(i))
            return true;
    }
    return false;
}

void SSettingsBase::collectWidgetsData()
{
    QMap<int, QWidget*>::const_iterator w = i_editorWidgets.constBegin();
    for(; w != i_editorWidgets.constEnd(); w++)
    {
        // Данные отключенных виджетов не записывать в БД
        if(!(*w)->isEnabled())
            continue;

        int index = i_editorWidgets.key(*w);
        QVariant data;
        switch(i_editorWidgetsTypes.value(index))
        {
            case WidgetTypes::LineEdit: data = static_cast<QLineEdit*>(*w)->text() ; break;
            case WidgetTypes::ComboBox: data = dbValueFromComboBox(static_cast<QComboBox*>(*w), index); break;
            case WidgetTypes::SCheckableComboBox: data = static_cast<SCheckComboBox*>(*w)->checkedString(); break;
//            case WidgetTypes::SColorPickBox: data = static_cast<SColorPickBox*>(*w)->value(); break; // не реализован виджет
            case WidgetTypes::CheckBox: data = static_cast<QCheckBox*>(*w)->isChecked(); break;
            case WidgetTypes::SpinBox: data = static_cast<QSpinBox*>(*w)->value(); break;
            case WidgetTypes::DoubleSpinBox: data = static_cast<QDoubleSpinBox*>(*w)->value(); break;
            case WidgetTypes::RadioButton: data = static_cast<QRadioButton*>(*w)->isChecked(); break;
            case WidgetTypes::DateTimeEdit: data = static_cast<QDateTimeEdit*>(*w)->dateTime(); break;
            default: break;
        }

        setData(index, data);
    }
}

/* Переопределённый метод.
 * При загрузке данных из БД необходимо задать newState = Executed;
 * при прямом изменении поля с JSON необходимо задать newState = Updated.
*/
void SSettingsBase::setCacheData(const int index, const QVariant &value, ModifiedField::State newState)
{
    SSingleRowModelBase::setCacheData(index, value, newState);
}

void SSettingsBase::save()
{
    collectWidgetsData();

    setDatabase(QSqlDatabase::database("connThird")); // переключение сессии до вызова isDirty()

    if(!isDirty())
        return;

    QUERY_EXEC_TH(i_query, 1, SSettingsBase::constructUpdateQuery());    // запись изменений в таблицу i_tableName
    setFieldsExecuted();
}

void SSettingsBase::registerReportField(const QString &name, int widgetType)
{
    switch(widgetType)
    {
        case WidgetTypes::NoWidget: return;
        default: break;
    }
    m_reportFieldsMap->insert(name, QString());
}

void SSettingsBase::reportMetaData(const QString &field, QVariant &data)
{
    metaObject()->invokeMethod(this, m_reportFieldsMap->value(field).toLocal8Bit(), Qt::DirectConnection, Q_RETURN_ARG(QVariant, data));
}

void SSettingsBase::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = 0; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = reportColumnHeader(info.index); break;
        case LimeReport::CallbackInfo::ColumnData: reportData(info.columnName, data); break;
        case LimeReport::CallbackInfo::ColumnCount: data = reportColumnCount(); break;
        case LimeReport::CallbackInfo::RowCount: data = 1; break;   // всегда одна строка
    }
}

void SSettingsBase::reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
    Q_UNUSED(type);
    result = 1;
}

void SSettingsBase::deleteWidgets()
{
    QWidget *w;
    int k;
    while(i_labelWidgets.count())
    {
        w = i_labelWidgets.last();
        k = i_labelWidgets.lastKey();
        i_labelWidgets.remove(k);
        delete w;
    }

    while(i_editorWidgets.count())
    {
        w = i_editorWidgets.last();
        k = i_editorWidgets.lastKey();
        i_editorWidgets.remove(k);
        delete w;
    }
}

int SSettingsBase::propertyGroup(const int index) const
{
    return i_propertyGroup.value(index);
}

static QString errMsg(QObject::tr("Не удалось сохранить основные настройки"));
static SComSettingsTypesRegistrator comSettingsTypesRegistrator;

SComSettings::SComSettings() :
    SSettingsBase()
{
    m_record_settings = std::make_shared<QSqlRecord>();
    m_classDbMap = std::make_unique<QMap<int, int>>();  // создание объекта до вызова mapFields()
    mapFields();
    registerReportFields();
    connect(this, &SSettingsBase::beginDataChange, this, &SComSettings::setDataRework);
}

void SComSettings::insertNewField(const int index, const QString &name)
{
    QSqlField f;
    if(name.startsWith("settings."))
    {
        m_classDbMap->insert(index, (Table::Settings | m_record_settings->count()));
        f.setName(name.split('.').at(1));
        m_record_settings->append(f);
    }
    else
    {
        m_classDbMap->insert(index, m_record->count());
        f.setName(name);
        m_record->append(f);
    }
}

void SComSettings::load()
{
    SSettingsBase::load();    // загрузка из таблицы `config`, очистка кэша

    loadFromTableSettings();

    deserializeData();
}

void SComSettings::loadFromTableSettings()
{
    i_query->exec("SELECT `name`, `value` FROM `settings`;");

    while(i_query->next())
    {
        int index = m_record_settings->indexOf(i_query->value(0).toString());
        if(index < 0)
            continue;

        m_record_settings->setValue(index, i_query->value(1));
    }
}

/* Инициализация отдельных полей класса при загрузке:
 *  - обработка полей с JSON;
 *  - обработка полей, связанных с виджетами RadioButton
*/
void SComSettings::deserializeData()
{
    loadFromJson(C_repairDispatcherUsersJson, 1);
    loadFromJson(C_repairDispatcherAssignCriteriaJson, 1);
    loadFromJson(C_repairStatuses, 1);
    loadFromJson(C_emailConfigJson, 1);
    loadFromJson(C_smsConfigJson, 1);
}

/* Обработка полей таблиц, представленных в виде JSON и копирование значений в отдельные поля класса
 * (такие поля хранятся только в кэше)
*/
void SComSettings::loadFromJson(const int index, bool init)
{
    ModifiedField::State newState = init?ModifiedField::Executed:ModifiedField::Updated;
    switch (index)
    {
        case C_repairDispatcherUsersJson: {
            t_repairDispatcherUsers val = repairDispatcherUsersJsonVariantCopy();
            setCacheData(C_repairDispatcherUsersJson, QVariant::fromValue(val), newState);
            setCacheData(C_repairDispatcherUsers, val.Users.join(','), newState);
            break;}
        case C_repairDispatcherAssignCriteriaJson: {
            t_repairDispatcherAssignCriteria val = repairDispatcherAssignCriteriaJsonVariantCopy();
            setCacheData(C_repairDispatcherAssignCriteriaJson, QVariant::fromValue(val), newState);
            setCacheData(C_repairDispatcherDayLimit, val.DayLimit, newState);
            setCacheData(C_repairDispatcherStatuses, val.StatusList.join(','), newState);
            break;}
        case C_repairStatuses: {
            t_repairStatuses val = repairStatusesVariantCopy();
            setCacheData(C_repairStatuses, QVariant::fromValue(val), newState);
            break;}
        case C_emailConfigJson: {
            t_emailConfig val = emailConfigJsonVariantCopy();
            setCacheData(C_emailConfigJson, QVariant::fromValue(val), newState);
            setCacheData(C_emailServer, val.Host, newState);
            setCacheData(C_emailPort, val.Port, newState);
            setCacheData(C_emailLogin, val.Login, newState);
            setCacheData(C_emailPassword, val.Password, newState);
            setCacheData(C_emailTimeout, val.Timeout, newState);
            setCacheData(C_emailEnableSsl, val.EnableSsl, newState);
            setCacheData(C_emailEnableImplicitSsl, val.EnableImplicitSsl, newState);
            setCacheData(C_emailTemplate, val.Template, newState);
            break;}
        case C_smsConfigJson: {
            t_smsConfig val = smsConfigJsonVariantCopy();
            setCacheData(C_smsConfigJson, QVariant::fromValue(val), newState);
            setCacheData(C_smsProvider, val.Provider, newState);
            setCacheData(C_smsAuthType, val.AuthType, newState);
            setCacheData(C_smsApiId, val.ApiId, newState);
            setCacheData(C_smsLogin, val.Login, newState);
            setCacheData(C_smsPassword, val.Password, newState);
            setCacheData(C_smsSender, val.Sender, newState);
            break;}
//        case C_repairStatuses: ; break;
        default: break;
    }
}

void SComSettings::updateJson(const int index, const QVariant &value)
{
    auto updateRepairDispatcherAssignCriteriaJson = [=](const int index){
        auto val = repairDispatcherAssignCriteriaJsonVariantCopy();
        switch (index)
        {
            case C_repairDispatcherDayLimit: val.DayLimit = value.toInt(); break;
            case C_repairDispatcherStatuses: val.StatusList = value.toString().split(','); break;
            default: break;
        }
        set_repairDispatcherAssignCriteriaJson(val);
    };

    auto updateEmailJson = [=](const int index){
        auto val = emailConfigJsonVariantCopy();
        switch (index)
        {
            case C_emailServer: val.Host = value.toString(); break;
            case C_emailPort: val.Port = value.toInt(); break;
            case C_emailLogin: val.Login = value.toString(); break;
            case C_emailPassword: val.Password = value.toString(); break;
            case C_emailTimeout: val.Timeout = value.toInt(); break;
            case C_emailEnableSsl: val.EnableSsl = value.toBool(); break;
            case C_emailEnableImplicitSsl: val.EnableImplicitSsl = value.toBool(); break;
            case C_emailTemplate: val.Template = value.toString(); break;
            default: break;
        }
        set_emailConfigJson(val);
    };

    auto updateSmsJson = [=](const int index){
        auto val = smsConfigJsonVariantCopy();
        switch (index)
        {
            case C_smsProvider: val.Provider = value.toInt(); break;
            case C_smsAuthType: val.AuthType = value.toInt(); break;
            case C_smsApiId: val.ApiId = value.toString(); break;
            case C_smsLogin: val.Login = value.toString(); break;
            case C_smsPassword: val.Password = value.toString(); break;
            case C_smsSender: val.Sender = value.toString(); break;
            default: break;
        }
        set_smsConfigJson(val);
    };

    blockSignals(true);
    switch (index)
    {
        case C_repairDispatcherUsers: {
            auto val = repairDispatcherUsersJsonVariantCopy();
            val.Users = value.toString().split(',');
            set_repairDispatcherUsersJson(val);
            break;}

        case C_repairDispatcherDayLimit: updateRepairDispatcherAssignCriteriaJson(C_repairDispatcherDayLimit); break;
        case C_repairDispatcherStatuses: updateRepairDispatcherAssignCriteriaJson(C_repairDispatcherStatuses); break;

        case C_repairStatuses: {
            auto val = repairStatusesVariantCopy();
            set_repairStatuses(val);
            break;}

        case C_emailServer: updateEmailJson(C_emailServer); break;
        case C_emailPort: updateEmailJson(C_emailPort); break;
        case C_emailLogin: updateEmailJson(C_emailLogin); break;
        case C_emailPassword: updateEmailJson(C_emailPassword); break;
        case C_emailTimeout: updateEmailJson(C_emailTimeout); break;
        case C_emailEnableSsl: updateEmailJson(C_emailEnableSsl); break;
        case C_emailEnableImplicitSsl: updateEmailJson(C_emailEnableImplicitSsl); break;
        case C_emailTemplate: updateEmailJson(C_emailTemplate); break;

        case C_smsProvider: updateSmsJson(C_smsProvider); break;
        case C_smsAuthType: updateSmsJson(C_smsAuthType); break;
        case C_smsApiId: updateSmsJson(C_smsApiId); break;
        case C_smsLogin: updateSmsJson(C_smsLogin); break;
        case C_smsPassword: updateSmsJson(C_smsPassword); break;
        case C_smsSender: updateSmsJson(C_smsSender); break;
        default: break;
    }
    blockSignals(false);
}

int SComSettings::dbFieldIndex(const int classIndex) const
{
    return m_classDbMap->value(classIndex, -1);
}

QString SComSettings::dbFieldName(const int classIndex) const
{
    int dbi = dbFieldIndex(classIndex);
    if(dbi&Table::Settings)
        return m_record_settings->fieldName(dbi&(Table::Settings-1));

    return rec()->fieldName(dbi);
}

int SComSettings::targetTable(const int targetTableIndex)
{
    if(targetTableIndex&Table::Settings)
        return 1;

    return 0;
}

void SComSettings::setDataRework(const int index, QVariant &data)
{
    switch (index)
    {
        case C_onlineStoreUrl:
        case C_onlineStoreKey: if(data.toString().isEmpty()) data = QVariant(); break;
        default: break;
    }
}

void SComSettings::dataChangeHandler(const int index, const QVariant &data)
{
    loadFromJson(index);
    updateJson(index, data);
}

void SComSettings::save()
{
    SSettingsBase::save();
    saveToTableSettings();
}

void SComSettings::saveToTableSettings()
{
    if(!isDirty(1))
        return;

    bool nErr = 1;

    i_query->prepare(QString("UPDATE `settings` SET `value` = :value WHERE `name` = :name"));

    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(!((*i)->state() & (ModifiedField::Updated | ModifiedField::Failed)))
            continue;

        int index = i.key();
        if(dbFieldIndex(index) >= 0)
        {
            i_query->bindValue(":name", dbFieldName(index));
            i_query->bindValue(":value", fieldValueHandler((*i)->data()));
            nErr = i_query->exec();
        }
        if(!nErr)
            Global::throwError(i_query->lastError(), errMsg);

        setState(*i, ModifiedField::Executed);
    }
}

QString SComSettings::table()
{
    return "config";
}

QVariant SComSettings::loadedValue(const int dbTableIndex) const
{
    if(dbTableIndex & Table::Settings)
        return m_record_settings->value(dbTableIndex & (Table::Settings - 1));
    else
        return rec()->value(dbTableIndex);
}

/* Конфигурирование виджетов: установка моделей данных, деактивация не реализованных и прочие действия
*/
void SComSettings::configureWidgets()
{
    auto setComboBoxModel = [&](const int index, QAbstractItemModel *model){
        QComboBox *cb = dynamic_cast<QComboBox*>(i_editorWidgets[index]);
        if(cb)
            cb->setModel(model);
    };

    setComboBoxModel(C_currencyId, currencyListModel);
    setComboBoxModel(C_defaultWorksWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultNewItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultUsedItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultRefItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultDisasmItemsWarranty, warrantyTermsModel);
    setComboBoxModel(C_defaultOtherItemsWarranty, warrantyTermsModel);

    SCheckComboBox *scb = static_cast<SCheckComboBox*>(i_editorWidgets[C_repairDispatcherUsers]);
    scb->setModel(engineersModel);
    scb->showSelectAllRow(true);

    scb = static_cast<SCheckComboBox*>(i_editorWidgets[C_repairDispatcherStatuses]);
#ifdef S_TEST
    scb->setModel(repairDispatcherStatesModel);
#endif
    scb->showSelectAllRow(false);

    /**************************************************************************************/
    /******************* Виджеты, работа с которыми еще не реализована ********************/
    /***************** данные таких виджетов не собираются при сохранении******************/
    /**************************************************************************************/
#ifndef S_TEST
    disableWidget(C_repairDispatcherUsers);
    disableWidget(C_repairDispatcherDayLimit);
    disableWidget(C_repairDispatcherStatuses);
#endif
    disableWidget(C_ascPhoneMask1);
    disableWidget(C_ascPhoneMask2);
    disableWidget(C_timeZoneId);
    disableWidget(C_voipId);
    disableWidget(C_onlineStoreId);
    disableWidget(C_notifyNewCommentColor);
    disableWidget(C_notifyRepairStatusUpdateColor);
    disableWidget(C_notifyIncomingSMSColor);
    disableWidget(C_notifyOutOfTermAlarmColor);
    disableWidget(C_notifyDeviceMatchColor);
    disableWidget(C_notifyCustomTaskColor);
    disableWidget(C_notifyItemRequestColor);
    disableWidget(C_notifyOrderFromOnlineStoreColor);
    disableWidget(C_notifyIncomingCallColor);
    disableWidget(C_notifyItemPurchaseRequestColor);
    disableWidget(C_exchangeSourceId);
}

void SComSettings::translate()
{
    tr("currencyId");
    tr("classicKassa");
    tr("timeZoneId");
    tr("ascPhoneMask1");
    tr("ascPhoneMask2");
    tr("updateChannel");
    tr("isPriceColOptVisible");
    tr("isPriceColOpt2Visible");
    tr("isPriceColOpt3Visible");
    tr("isPriceColRoznVisible");
    tr("isPriceColServiceVisible");
    tr("voipId");
    tr("isRealizatorEnable");
    tr("onlineStoreId");
    tr("isCartridgeRepairEnabled");
    tr("isEngineerRequiredOnDeviceRecept");
    tr("isVisitSourceRequired");
    tr("isSerialNumberRequired");
    tr("isCartridgeSerialNumberRequired");
    tr("isSearchOnlyBySerial");
    tr("isPhotoOnReceptRequired");
    tr("isPhotoOnIssueRequired");
    tr("isDiagRequired");
    tr("isVendorAddingAllowedOnRecept");
    tr("isRepairSummSetByManager");
    tr("isPaySalaryForRepairsIssuedInDebt");
    tr("isAutoSetCompanyOnRepairRecept");
    tr("isVendorWarrantyEnabled");
    tr("useSimplifiedCartridgeRepair");
    tr("autoCloseRepairTabTimeout");
    tr("repairImagesLimit");
    tr("repairDispatcherUsers");
    tr("repairDispatcherDayLimit");
    tr("repairDispatcherStatuses");
    tr("isClientPatronymicRequired");
    tr("isClientEmailRequired");
    tr("isClientAddressRequired");
    tr("isClientPhoneRequired");
    tr("isAnyClientDealer");
    tr("timeoutForItemsRequestsHandling");
    tr("timeoutForDiagnosisConfirmation");
    tr("defaultItemReserveTime");
    tr("itemImagesLimit");
    tr("isReasonForItemIncomeRequired");
    tr("defaultWorksWarranty");
    tr("defaultNewItemsWarranty");
    tr("defaultUsedItemsWarranty");
    tr("defaultRefItemsWarranty");
    tr("defaultDisasmItemsWarranty");
    tr("defaultOtherItemsWarranty");
    tr("notifyNewComment");
    tr("notifyRepairStatusUpdateColor");
    tr("notifyIncomingSMS");
    tr("notifyOutOfTermAlarmColor");
    tr("notifyDeviceMatch");
    tr("notifyCustomTask");
    tr("notifyItemRequest");
    tr("notifyOrderFromOnlineStore");
    tr("notifyIncomingCall");
    tr("notifyItemPurchaseRequest");
    tr("printPKO");
    tr("printOutInvoice");
    tr("printInInvoice");
    tr("printRKO");
    tr("printWarrantyDoc");
    tr("printWorksList");
    tr("printDiagResult");
    tr("printRepairRejectDoc");
    tr("printRKOOnItemIncome");
    tr("printCheck");
    tr("printRepairReceptDoc");
    tr("printRepairStickers");
    tr("printCartridgeReceptDoc");
    tr("printCartridgeStickers");
    tr("defaultRepairStickersQty");
    tr("autoSwitchKeyboardLayout");
    tr("voipAsteriskHost");
    tr("voipAsteriskPort");
    tr("voipAsteriskLogin");
    tr("voipAsteriskPassword");
    tr("voipAsteriskWebPort");
    tr("voipPrefix");
    tr("voipEndpoint");
    tr("voipKey");
    tr("voipSecret");
    tr("onlineStoreUrl");
    tr("onlineStoreKey");
    tr("salaryClassic");
    tr("salaryIncludeNotIssuedByDefault");
    tr("newClientSmsEnabled");
    tr("exchangeTypeAuto");
    tr("exchangeTypeManual");
    tr("backupEnable");
    tr("backupImages");
    tr("backupTime");
    tr("smsProvider");
    tr("smsAuthType");
    tr("smsApiId");
    tr("smsLogin");
    tr("smsPassword");
    tr("smsSender");
    tr("emailServer");
    tr("emailPort");
    tr("emailLogin");
    tr("emailPassword");
    tr("emailTimeout");
    tr("emailEnableSsl");
    tr("emailEnableImplicitSsl");
}
