#include <QString>

class SStandardItemModel
{
public:
    explicit SStandardItemModel(){};
    ~SStandardItemModel(){};
    QString getDisplayRole(int, int){
        return "test1";
    };
    QString getDisplayRole(int id, QString = "id"){
      if(id == 1)
          return "test1";
      else
          return "test2";
    };
};
