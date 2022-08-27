#ifndef SDEVMDLSMODEL_H
#define SDEVMDLSMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include "sdevmdlmodel.h"

class SDevMdlsModel : public QObject
{
    Q_OBJECT
public:
    explicit SDevMdlsModel(QObject *parent = nullptr);
    ~SDevMdlsModel();
    QSqlQueryModel *models();
private:
    QSqlQueryModel* m_sqlModel;
public slots:
    void indexChanged(int);

signals:

};

#endif // SDEVMDLSMODEL_H
