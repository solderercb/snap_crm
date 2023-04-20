#ifndef SPAGESALARYRECEPTEDISSUED_H
#define SPAGESALARYRECEPTEDISSUED_H

#include <QWidget>
#include "spagesalarybase.h"

namespace Ui {
class SPageSalaryReceptedIssued;
}

class SPageSalaryReceptedIssued : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalaryReceptedIssued(QWidget *parent = nullptr);
    ~SPageSalaryReceptedIssued();
private:
    Ui::SPageSalaryReceptedIssued *ui;
public slots:
    void updateModels();
    void updateWidgets();
    void tableRepairsRowDoubleClicked(const QModelIndex &index);
};

#endif // SPAGESALARYRECEPTEDISSUED_H
