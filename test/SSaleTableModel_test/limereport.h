/* Это заглушка библиотеки LimeReport
*/
#ifndef LIMEREPORT_H
#define LIMEREPORT_H

#include <QString>

namespace LimeReport {

struct CallbackInfo{
    enum DataType{IsEmpty, HasNext, ColumnHeaderData, ColumnData, ColumnCount, RowCount};
    enum ChangePosType{First, Next};
    DataType dataType;
    int index;
    QString columnName;
};

} // namespace LimeReport

#endif // LIMEREPORT_H
