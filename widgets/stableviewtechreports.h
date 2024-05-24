#ifndef STABLEVIEWTECHREPORTS_H
#define STABLEVIEWTECHREPORTS_H

#include "stableviewbase.h"
#include <QWidget>
#include "models/stabletechreportsmodel.h"

class STableViewTechReports : public STableViewBase
{
    Q_OBJECT
public:
    enum Column{ID = 0, Num, Created, User, Company, Client, Device, InvNumber, SerialNumber, ProductionDate, PurchaseDate, InitCost, ResidualCost, Fault, DiagResult, Conclusion, Notes};
    Q_ENUM(Column)
    enum ColumnWidth{ID_w = 40, Num_w = 40, Created_w = 120, User_w = 110, Company_w = 110, Client_w = 150, Device_w = 120, InvNumber_w = 80, SerialNumber_w = 80, ProductionDate_w = 80, PurchaseDate_w = 80, InitCost_w = 60, ResidualCost_w = 60, Fault_w = 120, DiagResult_w = 120, Conclusion_w = 120, Notes_w = 120};
    Q_ENUM(ColumnWidth)
    explicit STableViewTechReports(QWidget *parent = nullptr);
    void setModel(QAbstractItemModel *model) override;
    QList<int> *selectedReportsList();
private:
    STableTechReportsModel *m_model = nullptr;
    void setColumnWidth(int column, int width) override;
    void clearModel() override;
    void setModelQuery(const QString &query, const QSqlDatabase &database) override;
    void translateNames();
};

#endif // STABLEVIEWTECHREPORTS_H
