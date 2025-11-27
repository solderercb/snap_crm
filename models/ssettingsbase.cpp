#include "ssettingsbase.h"
#include <SStandardItemModel>
#include <ProjectGlobals>
#include <ProjectQueries>

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
