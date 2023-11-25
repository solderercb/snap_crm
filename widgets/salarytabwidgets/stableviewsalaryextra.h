#ifndef STABLEVIEWSALARYEXTRA_H
#define STABLEVIEWSALARYEXTRA_H

#include "../stableviewbase.h"
#include <QObject>

class STableViewSalaryExtra : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    enum Column {ID = 0, Reason = 1, Amount = 2, Date = 3, User = 4};
    Q_ENUM(Column)
    enum ColumnWidth {ID_w = 60, Reason_w = 400, Amount_w = 60, Date_w = 115, User_w = 150};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryExtra(QWidget *parent = nullptr);
    ~STableViewSalaryExtra();
    XtraSerializer *gridLayout();
    void setModel(QAbstractItemModel *model);
private:
    void translateNames();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth);
};

#endif // STABLEVIEWSALARYEXTRA_H
