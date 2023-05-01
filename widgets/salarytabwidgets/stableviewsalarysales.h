#ifndef STABLEVIEWSALARYSALES_H
#define STABLEVIEWSALARYSALES_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalarySales : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalarySales(QWidget *parent = nullptr);
    ~STableViewSalarySales();
};

#endif // STABLEVIEWSALARYSALES_H
