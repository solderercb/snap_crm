#ifndef STABLESALARYRECEPTEDISSUED_H
#define STABLESALARYRECEPTEDISSUED_H

#include "../stablebasemodel.h"
#include <QObject>

class STableSalaryRecepted : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalaryRecepted(QObject *parent = nullptr);
};


class STableSalaryIssued : public STableBaseModel
{
    Q_OBJECT
public:
    explicit STableSalaryIssued(QObject *parent = nullptr);
};
#endif // STABLESALARYRECEPTEDISSUED_H
