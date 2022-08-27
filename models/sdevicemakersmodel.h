#ifndef SDEVICEMAKERSMODEL_H
#define SDEVICEMAKERSMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include "sdevmdlsmodel.h"
#include "sdevicemakermodel.h"

class SDeviceMakersModel : public QObject
{
    Q_OBJECT
public:
    explicit SDeviceMakersModel(QObject *parent = nullptr);
    ~SDeviceMakersModel();
    void load(const int);
    QSqlQueryModel *makers();
    QSqlQueryModel *models();
private:
    QSqlQueryModel* m_sqlModel;
    SDevMdlsModel* m_devMdlsModel;
public slots:
    void indexChanged(int);

signals:

};

#endif // SDEVICEMAKERSMODEL_H
