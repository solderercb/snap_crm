/* Скрипты препроцессора, которые нужно переопределять в каждом новом классе
 * Файл должен быть подключен после базового класса ssinglerowmodel.h, но до списка элементов (полей)
*/
#ifdef SETTING_FIELDS
#undef SETTING_FIELDS
#endif

#ifdef SETTING_FIELD
#undef SETTING_FIELD
#endif

#define SETTING_FIELD(field, ...) \
    REGISTER_ENUM_FIELD(__VA_ARGS__)

// для объявления enum SETTING_FIELD_VARIANT_COPY идентичен SETTING_FIELD
#define SETTING_FIELD_VARIANT_COPY(field, ...) SETTING_FIELD(field, __VA_ARGS__)

#ifdef ADDITIONAL_REPORT_FIELDS
#undef ADDITIONAL_REPORT_FIELDS
#endif

#define ADDITIONAL_REPORT_FIELDS
