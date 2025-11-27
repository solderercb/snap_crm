#include <QStandardItemModel>
#include <QString>
#include <QDebug>

class SStandardItemModel : public QStandardItemModel
{
public:
    explicit SStandardItemModel(){};
    ~SStandardItemModel(){};
    QString getDisplayRole(int id, QString idField = "id"){
        Q_UNUSED(idField)
        if(id == 1)
            return "test1";
        else
            return "test2";
    };
    QString getDisplayRole(int id, int){
        return getDisplayRole(id);
    };
    static SStandardItemModel* modelFromEnum(QMetaEnum &, QString (*)(const char *, const char *, int) = nullptr){return new SStandardItemModel();};
    void setColumnCount(int){};
    void setObjectName(const QString&){};
    int rowByDatabaseID(int id, int){return rowByDatabaseID(id);};
    int rowByDatabaseID(int id, QString field = "id"){Q_UNUSED(field) if(id == 0) return 0; else return 1;};
};
