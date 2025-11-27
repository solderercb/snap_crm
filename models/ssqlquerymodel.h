#ifndef SSQLQUERYMODEL_H
#define SSQLQUERYMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <SAbstractItemModel>

class SSqlQueryModel : public QSqlQueryModel, public SAbstractItemModel
{
    Q_OBJECT
public:
    explicit SSqlQueryModel(QObject *parent = nullptr);
    ~SSqlQueryModel();
};

#endif // SSQLQUERYMODEL_H
