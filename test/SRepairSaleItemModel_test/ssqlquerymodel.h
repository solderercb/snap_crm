#include <QString>

class SSqlQueryModel
{
public:
    explicit SSqlQueryModel(){};
    ~SSqlQueryModel(){};
    QString getDisplayRole(int id, QString = "id"){if(id == 1) return "test1"; else return "test2";};
};
