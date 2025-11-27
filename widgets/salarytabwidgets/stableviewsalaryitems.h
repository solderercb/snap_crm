#ifndef STABLEVIEWSALARYITEMS_H
#define STABLEVIEWSALARYITEMS_H

#include <STableViewBase>
#include <QObject>

class STableViewSalaryItems : public STableViewBase
{
    Q_OBJECT
signals:
    void signalColumnResized(int column, int newWidth);
public:
    enum Column {UID = 0, Name = 1, Date = 2, Qty = 3, Price = 4, Amount = 5, State = 6};
    Q_ENUM(Column)
    enum ColumnWidth {UID_w = 100, Name_w = 280, Date_w = 115, Qty_w = 60, Price_w = 115, Amount_w = 115, State_w = 115};
    Q_ENUM(ColumnWidth)
    explicit STableViewSalaryItems(QWidget *parent = nullptr);
    ~STableViewSalaryItems();
    XtraSerializer *gridLayout();
private:
    void translateNames();
protected slots:
    void columnResized(int column, int oldWidth, int newWidth);
};

#endif // STABLEVIEWSALARYITEMS_H
