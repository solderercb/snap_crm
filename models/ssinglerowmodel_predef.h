/* Скрипты препроцессора, которые нужно переопределять в каждом новом классе
 * Файл должен быть подключен после базового класса ssinglerowmodel.h, но до списка элементов (полей)
*/
#ifdef TABLE_FIELDS
#undef TABLE_FIELDS
#endif

#ifdef TABLE_FIELD
#undef TABLE_FIELD
#endif

#define TABLE_FIELD(field, ...)                                                                         \
    DECLARE_GETTER_AND_SETTER(__VA_ARGS__)                                                              \

#ifdef ADDITIONAL_REPORT_FIELDS
#undef ADDITIONAL_REPORT_FIELDS
#endif

#define ADDITIONAL_REPORT_FIELDS
