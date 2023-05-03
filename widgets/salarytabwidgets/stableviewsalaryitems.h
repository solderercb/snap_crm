#ifndef STABLEVIEWSALARYITEMS_H
#define STABLEVIEWSALARYITEMS_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryItems : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    explicit STableViewSalaryItems(QWidget *parent = nullptr);
    ~STableViewSalaryItems();
    XtraSerializer *gridLayout();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth);
};

#endif // STABLEVIEWSALARYITEMS_H
