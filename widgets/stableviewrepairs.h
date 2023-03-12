#ifndef STABLEVIEWREPAIRS_H
#define STABLEVIEWREPAIRS_H

#include "stableviewbase.h"
#include <QWidget>
#include "stableviewrepairsitemdelegates.h"

class STableViewRepairs : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewRepairs(QWidget *parent = nullptr);
    ~STableViewRepairs();
    void setModel(STableBaseModel *model);
private:
    STableViewRepairsItemDelegates *m_itemDelegates;
};

#endif // STABLEVIEWREPAIRS_H
