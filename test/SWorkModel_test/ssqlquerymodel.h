#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QString>

class SSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SSqlQueryModel(QObject* = nullptr){};
    ~SSqlQueryModel(){};
    QString getDisplayRole(int id, int){if(id == 1) return "test1"; else return "test2";};
    QString getDisplayRole(int id, QString = "id"){if(id == 1) return "test1"; else return "test2";};
    virtual void clear(){};
    QVariant value(int id, QString, QString){if(id == 1) return 66; return 50;};
};

#endif // SSQLQUERYMODEL_H
