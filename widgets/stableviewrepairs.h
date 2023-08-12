#ifndef STABLEVIEWREPAIRS_H
#define STABLEVIEWREPAIRS_H

#include "stableviewbase.h"
#include <QWidget>
#include "stableviewrepairsitemdelegates.h"

class STableViewRepairs : public STableViewBase
{
    Q_OBJECT
public:
    enum Column{info = 0, ID = 1, Title = 2, Serial = 3, Fault = 4, Engineer = 5, State = 6, RepairAmount = 7, InDate = 8, OutDate = 9, ClientFullName = 10, ClientPrimaryPhone = 11, Place = 12, Manager = 13, Progress = 14};
    Q_ENUM(Column)
    enum ColumnWidth{info_w = 40, ID_w = 80, Title_w = 250, Serial_w = 120, Fault_w = 120, Engineer_w = 110, State_w = 130, RepairAmount_w = 60, InDate_w = 120, OutDate_w = 120, ClientFullName_w = 200, ClientPrimaryPhone_w = 130, Place_w = 60, Manager_w = 110, Progress_w = 160};
    enum ServiceColumn{RealRepairCost = 7, StateId = 17, ClientId = 35};
    Q_ENUM(ColumnWidth)
    enum TableMode{ModeRepairs = 0, ModeCartridges};
    explicit STableViewRepairs(QWidget *parent = nullptr);
    ~STableViewRepairs();
    QList<int> *selectedRepairsList();
    bool selectedCanBeIssued();
    void setMode(const int mode);
private:
    int m_mode = 0;
    void translateNames();
};

#endif // STABLEVIEWREPAIRS_H
