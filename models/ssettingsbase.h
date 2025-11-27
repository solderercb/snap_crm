#ifndef SSETTINGSBASE_H
#define SSETTINGSBASE_H

#ifndef SKIP_INCLUDES_IN_PREPROCESSING_EVALUATION
#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <memory>
#include <type_traits>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QLabel>
#include <SCheckComboBox>
#include <LimeReport>
#endif // SKIP_INCLUDES_IN_PREPROCESSING_EVALUATION

#include <SSingleRowModelBase>

#define EMPTY()
// Конкатенация токенов
#define CAT(a, b) CAT_IMPL(a, b)
#define CAT_IMPL(a, b) a##b

// Преобразование в строку
#define TO_STRING(x) #x
#define STRINGIZE(x) TO_STRING(x)

// Детектирование IGNORE
#define GET_SECOND_ARG(arg1, ...) GET_SECOND_ARG_IMPL(__VA_ARGS__)
#define GET_SECOND_ARG_IMPL(arg2, ...) arg2
#define EXPR_EXPANSION(expr1, expr2, ...) GET_SECOND_ARG(expr2)
#define EXPR_CTR(x) CAT(IS_IGNORE_, x), 0
#define IS_IGNORE(x) EXPR_EXPANSION(EXPR_CTR(x), EXPR_CTR(x))
#define IS_IGNORE_IGNORE ~, 1

// Условный выбор
#define IF_0(true_case, false_case) false_case
#define IF_1(true_case, false_case) true_case
#define IF(cond, true_case, false_case) CAT(IF_, cond)(true_case, false_case)

// Генерация строки объявления
#define DECLARE_VAR(type, name) \
    IF(IS_IGNORE(name), \
        EMPTY(), \
        type name; \
    )

#define REGISTER_ENUM_FIELD(name, ...)  C_##name,

#define MODEL_FIELD_GET(name) name
#define MODEL_FIELD_GET_VARIANT_COPY(name) name##VariantCopy
#define MODEL_FIELD_SET(name) set_##name

#undef DECLARE_GETTER_AND_SETTER
#define DECLARE_GETTER_AND_SETTER(name, type, ...)                                                  \
    inline type MODEL_FIELD_GET(name)() const                                                       \
    {                                                                                               \
        return data(C_##name).value<type>();                                                        \
    }                                                                                               \
    inline void MODEL_FIELD_SET(name)(const type &data)                                             \
    {                                                                                               \
        setData(C_##name, data);                                                                    \
    }
#define DECLARE_GETTER_AND_SETTER_VARIANT_COPY(name, type, ...)                                     \
    inline type MODEL_FIELD_GET_VARIANT_COPY(name)() const                                          \
    {                                                                                               \
        return data(C_##name).value<type>();                                                        \
    }                                                                                               \
    inline void MODEL_FIELD_SET(name)(const type &data)                                             \
    {                                                                                               \
        setData(C_##name, data);                                                                    \
    }

// Макросы для подсчета элементов
#define COUNT_SETTING_FIELDS(...) sizeof((const QString[]){__VA_ARGS__})/sizeof(const QString)
#define GET_SETTING_FIELDS_COUNT() COUNT_SETTING_FIELDS(SETTING_FIELDS)

#define REGISTER_SETTINGS_TABLE_FIELD(dbField, name) \
    IF(IS_IGNORE(dbField), \
        EMPTY(), \
        insertNewField(C_##name, #dbField); \
    )

#define REGISTER_SETTING_FIELD(name, ...) m_namesMap->insert(C_##name, #name);
#define REGISTER_SETTING_REP_FIELD(name, dataType, widget, ...) \
        registerReportField(#name, widget);

/* Макрос для объявления дополнительных полей для отчетов.
 * В качестве параметров указываются имя поля в отчете и название метода, отдающего
 * данные; методы должны быть помечены как public Q_SLOTS (их вызов происходит с
 * помощью QMetaObject::invokeMethod); тип возвращаемых данных должен быть QVariant.
*/
#define ADDITIONAL_REPORT_FIELD(fieldName, slotName)                                                \
    m_reportFieldsMap->insert(#fieldName, #slotName);

#define WIDGET_INIT_CALL(name, dataType, ...) \
        initWidget(C_##name, tr(#name), __VA_ARGS__);

class SSettingsBase : public QObject, public SSingleRowModelBase
{
    Q_OBJECT
    friend class TClassTest;
signals:
    void beginDataChange(const int index, QVariant &data);
    void dataChanged(const int index, QVariant &data);
public:
    enum WidgetTypes {NoWidget, CheckBox, ComboBox, SCheckableComboBox, SColorPickBox, DateTimeEdit, LineEdit, RadioButton, SpinBox, DoubleSpinBox};
    explicit SSettingsBase();
    void setPrimaryKey(const QVariant &data);
    void deleteWidgets();
    int propertyGroup(const int index) const;
    QLabel *label(const int index);
    QWidget *widget(const int index);
    void load() override;
protected:
    QMap<int, QLabel*> i_labelWidgets;
    QMap<int, QWidget*> i_editorWidgets;
    QMap<int, int> i_editorWidgetsTypes;
    QMap<int, QVariant> i_editorWidgetsDefaults;
    QMap<int, int> i_propertyGroup;
    virtual void mapFields() = 0;
    virtual void registerReportFields() = 0;
    virtual void initWidgets() = 0;
    virtual void configureWidgets(){};
    void disableWidget(const int index);
    int widgetGroup(const int index);
    void signalDataToBeChanged(const int index, QVariant &data) override;
    void signalDataChanged(const int index, QVariant &data) override;
    bool isModelLoaded() const override;
    virtual std::shared_ptr<QSqlRecord> rec() const override;
    void setComboBoxIndex(QComboBox *cb, const QVariant &dbValue);
    QVariant dbValueFromComboBox(QComboBox *cb, const int index);
    virtual void setWidgetsData();
    virtual int targetTable(const int index);
    bool skipField(CacheMap::ConstIterator f) override;
    bool skipField(CacheMap::ConstIterator f, const int targetTable);
    bool isDirty() override;
    bool isDirty(const int targetTable);
    void collectWidgetsData();
    void setCacheData(const int index, const QVariant &value, ModifiedField::State newState = ModifiedField::Updated);
    virtual void save();
    template<typename T>
    void setData(int index, const T& value){
        SSingleRowModelBase::setData(index, QVariant::fromValue(value));
    };

    template<typename... Args>
    QWidget* initLineEdit(int, Args&&...) {
        QLineEdit *w = new QLineEdit();
        return w;
    };

    template<typename T>
    QWidget* initComboBox(int index, T defaultValue) {
        QComboBox *w = new QComboBox();
        i_editorWidgetsDefaults.insert(index, defaultValue);
        return w;
    };
    template<typename... Args>
    QWidget* initComboBox(int, Args&&...) {
        QComboBox *w = new QComboBox();
        return w;
    };

    template<typename... Args>
    QWidget* initCheckableComboBox(int, Args&&...) {
        SCheckComboBox *w = new SCheckComboBox();
        return w;
    };

    template<typename... Args>
    QWidget* initColorPickBox(int, Args&&...) {
//        SColorPickBox *w = new SColorPickBox();
//        return w;
        return new QComboBox();
    };

    template<typename... Args>
    QWidget* initCheckBox(int, Args&&...) {
        QCheckBox *w = new QCheckBox();
        w->setText("");
        w->setLayoutDirection(Qt::RightToLeft);
        return w;
    };

    template<typename... Args>
    QWidget* initSpinBox(int, Args&&...) {  // заглушка для компилятора и защита от дурака
        Q_ASSERT_X(0, "Can't initialize QSpinBox", " wrong arguments types");
        return nullptr;
    }
    template<typename T1, typename T2, typename... Args,
             typename = std::enable_if_t<
                 std::is_same<std::decay_t<T1>, int>::value &&
                 std::is_same<std::decay_t<T2>, int>::value
             >>
    QWidget* initSpinBox(int, T1&& min, T2&& max, Args&&...) {
        QSpinBox *w = new QSpinBox();
        w->setRange(min, max);
        return w;
    };

    template<typename... Args>
    QWidget* initDSpinBox(int, Args&&...) {  // заглушка для компилятора и защита от дурака
        Q_ASSERT_X(0, "Can't initialize QDoubleSpinBox", " wrong arguments types");
        return nullptr;
    }
    template<typename T1, typename T2, typename... Args,
             typename = std::enable_if_t<
                 std::is_same<std::decay_t<T1>, double>::value &&
                 std::is_same<std::decay_t<T2>, double>::value
             >>
    QWidget* initDSpinBox(int, T1&& min, T2&& max, Args&&...) {
        QDoubleSpinBox *w = new QDoubleSpinBox();
        w->setRange(min, max);
        return w;
    };

    template<typename... Args>
    QWidget* initRadioButton(int, Args&&...) {
        QRadioButton *w = new QRadioButton();
        w->setText("");
        w->setLayoutDirection(Qt::RightToLeft);
        return w;
    };

    template<typename... Args>
    QWidget* initDateTimeEdit(int, Args&&...) {
        QDateTimeEdit*w = new QDateTimeEdit();
        return w;
    };

    template<typename... Args>
    void initWidget(int, const QString &, int, Args&&...) { // заглушка для полей без виджета
/*      switch(type) {
          case WidgetTypes::NoWidget: return;
          case WidgetTypes::NoWidget_JsonArray: return;
          case WidgetTypes::NoWidget_JsonObject: return;
          default: break;
      }
*/
    };

    template<typename... Args>
    void initWidget(int index, const QString &label, int type, int group, Args&&... args) {
      QWidget *w;
      switch(type) {
          case WidgetTypes::LineEdit: w = initLineEdit(index, std::forward<Args>(args)...); break;
          case WidgetTypes::ComboBox: w = initComboBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::SCheckableComboBox: w = initCheckableComboBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::SColorPickBox: w = initColorPickBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::CheckBox: w = initCheckBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::SpinBox: w = initSpinBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::DoubleSpinBox: w = initDSpinBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::RadioButton: w = initRadioButton(index, std::forward<Args>(args)...); break;
          case WidgetTypes::DateTimeEdit: w = initDateTimeEdit(index, std::forward<Args>(args)...); break;
          default: break;
      }

      QLabel *l = new QLabel(label);
      i_labelWidgets.insert(index, l);
      i_editorWidgets.insert(index, w);
      i_editorWidgetsTypes.insert(index, type);
      i_propertyGroup.insert(index, group);
    };

    void registerReportField(const QString &name, int widgetType);
    void reportMetaData(const QString &field, QVariant &data) override;
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
protected slots:
    virtual void dataChangeHandler(const int, const QVariant &){};
};

inline void SSettingsBase::signalDataToBeChanged(const int index, QVariant &data)
{
    emit beginDataChange(index, data);
}

inline void SSettingsBase::signalDataChanged(const int index, QVariant &data)
{
    emit dataChanged(index, data);
}

inline bool SSettingsBase::isModelLoaded() const
{
    return m_record->value(0).isValid();
}

inline std::shared_ptr<QSqlRecord> SSettingsBase::rec() const
{
    return m_record;
}

/* Возвращает идентификатор таблицы над которой выполняются запросы в текущий момент (например, при формировании запроса UPDATE).
 * Метод базового класса возвращает 0, т. к. по умолчанию класс работает с одной таблицей
*/
inline int SSettingsBase::targetTable(const int)
{
    return 0;
}

#endif // SSETTINGSBASE_H


