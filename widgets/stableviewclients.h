#ifndef STABLEVIEWCLIENTS_H
#define STABLEVIEWCLIENTS_H

#include "stableviewbase.h"
#include <QWidget>
#include "stableviewclientsitemdelegates.h"

class STableViewClients : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewClients(QWidget *parent = nullptr);
    ~STableViewClients();
};

#endif // STABLEVIEWCLIENTS_H
