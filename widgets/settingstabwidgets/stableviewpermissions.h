#ifndef STABLEVIEWPERMISSIONS_H
#define STABLEVIEWPERMISSIONS_H

#include <QTableView>
#include <QObject>

class STableViewPermissions : public QTableView
{
    Q_OBJECT
public:
    explicit STableViewPermissions(QWidget *parent);
    ~STableViewPermissions();
};

#endif // STABLEVIEWPERMISSIONS_H
