#ifndef SDEVICESMODEL_H
#define SDEVICESMODEL_H

#include <QObject>

class SDeviceMakersModel;
class QSqlQueryModel;

class SDevicesModel : public QObject
{
    Q_OBJECT
signals:
public:
    explicit SDevicesModel(const int office = -1, QObject *parent = nullptr);
    ~SDevicesModel();
    void load(const int office = -1);
    QSqlQueryModel *devices();
    QSqlQueryModel *makers();
    QSqlQueryModel *models();

private:
    QSqlQueryModel* m_sqlModel;
    SDeviceMakersModel *m_deviceMakersModel;
public slots:
    void deviceChanged(int);
    void makerChanged(int);
    void mdlChanged(int);
};

#endif // SDEVICESMODEL_H
