#include <QString>

class SSqlQueryModel
{
public:
    explicit SSqlQueryModel(){};
    ~SSqlQueryModel(){};
    QString getDisplayRole(int id, QString = "id"){if(id == 1) return "test1"; else return "test2";};
    int databaseIDByRow(int row, int){if(row == 0) return 1; else return 2;};
    int databaseIDByRow(int row, QString = "id"){return databaseIDByRow(row, 0);};
};
