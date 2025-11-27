#include <QString>
#include <QDebug>

class SStandardItemModel
{
public:
    explicit SStandardItemModel(){};
    ~SStandardItemModel(){};
    QString getDisplayRole(int id, QString = "id"){
        if(id == 1)
            return "test1";
        else
            return "test2";
    };
    QString getDisplayRole(int id, int){
        return getDisplayRole(id);
    };
};
