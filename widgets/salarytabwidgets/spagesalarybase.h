#ifndef SPAGESALARYBASE_H
#define SPAGESALARYBASE_H

#include <QObject>
#include <QWidget>
#include "tabsalary.h"

class SPageSalaryBase : public QWidget
{
    Q_OBJECT
public:
    explicit SPageSalaryBase(QWidget *parent = nullptr);
protected:
    MainWindow *mainWindow;
    tabSalary *parentTab;
    virtual void updateModels(){};
    virtual void updateWidgets(){};
};

#endif // SPAGESALARYBASE_H

