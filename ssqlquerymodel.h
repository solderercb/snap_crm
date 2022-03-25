#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlField>
#include "sstandarditemmodel.h"

class SSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SSqlQueryModel(QObject *parent = nullptr);
    ~SSqlQueryModel();
    int getFieldIdByName(const QString &field);
    QString getDisplayRole(int id, int column);
    QString getDisplayRole(int id, QString field = "id");

signals:

};

#endif // SSQLQUERYMODEL_H
