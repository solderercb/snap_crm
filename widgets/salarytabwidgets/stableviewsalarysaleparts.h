#ifndef STABLEVIEWSALARYSALEPARTS_H
#define STABLEVIEWSALARYSALEPARTS_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalarySaleParts : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSalarySaleParts(QWidget *parent = nullptr);
    ~STableViewSalarySaleParts();
};

#endif // STABLEVIEWSALARYSALEPARTS_H
