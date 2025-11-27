/* Скрипты препроцессора для инициализации массива полей, которые также нужно переопределять в каждом новом классе
 * Файл должен быть подключен в заголовочном файле в опеределении класса с модификатором private
 * Ошибки анализатора следует игнорировать.
*/
TABLE_FIELDS

#undef TABLE_FIELD
#define TABLE_FIELD(field, ...)  REGISTER_ENUM_FIELD(__VA_ARGS__)  // переопределение макроса для инициализации enum
public:
    enum Columns {TABLE_FIELDS};

#undef TABLE_FIELD
#define TABLE_FIELD(field, ...) #field,         // переопределение макроса для инициализации переменной sz

private:
void mapFields() final {
    int index = 0;
    sz = GET_ELEMENT_COUNT();
    #undef TABLE_FIELD
    #define TABLE_FIELD(name, ...)  REGISTER_TABLE_FIELD(name)  // переопределение макроса для инициализации массива полей таблицы
    TABLE_FIELDS

    index = 0;
    #undef TABLE_FIELD
    #define TABLE_FIELD(name, ...)  REGISTER_CLASS_FIELD(__VA_ARGS__)  // переопределение макроса для инициализации массива полей класса
    TABLE_FIELDS

    #undef TABLE_FIELD
    #define TABLE_FIELD(name, ...)  REGISTER_REPORT_FIELD(__VA_ARGS__)  // переопределение макроса для инициализации массива полей для отчетов
    TABLE_FIELDS
    ADDITIONAL_REPORT_FIELDS
};
