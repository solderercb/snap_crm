/* Скрипты препроцессора для инициализации массива полей, которые также нужно переопределять в каждом новом классе
 * Файл должен быть подключен в заголовочном файле в опеределении класса с модификатором private
 * Ошибки анализатора следует игнорировать.
*/
public:
    enum Fields {SETTING_FIELDS};

#undef SETTING_FIELD
#undef SETTING_FIELD_VARIANT_COPY
#define SETTING_FIELD(field, ...)  DECLARE_GETTER_AND_SETTER(__VA_ARGS__)  // переопределение макроса для декларации геттеров и сеттеров

// геттер из SETTING_FIELD_VARIANT_COPY будет с суффиксом VariantCopy, сеттер такой же, как из SETTING_FIELD
#define SETTING_FIELD_VARIANT_COPY(field, ...) DECLARE_GETTER_AND_SETTER_VARIANT_COPY(__VA_ARGS__)
    SETTING_FIELDS

#undef SETTING_FIELD
#undef SETTING_FIELD_VARIANT_COPY
#define SETTING_FIELD(field, ...) #field,         // переопределение макроса для инициализации переменной sz

// далее SETTING_FIELD_VARIANT_COPY идентичен SETTING_FIELD
#define SETTING_FIELD_VARIANT_COPY(field, ...) SETTING_FIELD(field, __VA_ARGS__)

private:
    void mapFields() final {
        sz = GET_SETTING_FIELDS_COUNT();
        #undef SETTING_FIELD
        #define SETTING_FIELD(dbField, name, ...)  REGISTER_SETTINGS_TABLE_FIELD(dbField, name)  // переопределение макроса для инициализации массива полей таблицы
        SETTING_FIELDS
  
        #undef SETTING_FIELD
        #define SETTING_FIELD(name, ...)  REGISTER_SETTING_FIELD(__VA_ARGS__)  // переопределение макроса для инициализации массива полей класса
        SETTING_FIELDS

    };
    void registerReportFields() final {
        #undef SETTING_FIELD
        #define SETTING_FIELD(name, ...)  REGISTER_SETTING_REP_FIELD(__VA_ARGS__)  // переопределение макроса для инициализации массива полей для отчетов
        SETTING_FIELDS
        ADDITIONAL_REPORT_FIELDS
    };

public:
    void initWidgets() final {
        #undef SETTING_FIELD
        #define SETTING_FIELD(name, ...)  WIDGET_INIT_CALL(__VA_ARGS__)  // переопределение макроса для формирования списка вызовов инициализации виджетов
        SETTING_FIELDS

        configureWidgets();
        setWidgetsData();
    };
