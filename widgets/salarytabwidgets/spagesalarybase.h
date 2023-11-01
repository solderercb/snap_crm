#ifndef SPAGESALARYBASE_H
#define SPAGESALARYBASE_H

#include <QObject>
#include <QWidget>
#include "tabsalary.h"

class SPageSalaryBase : public QWidget
{
    Q_OBJECT
public:
    enum ModelsNumbers{UserModel = 0, RepairsModel, ExtraChargesModel, SalesModel, ItemsModel, PaymentsModel, ReceptedModel, IssuedModel};
    explicit SPageSalaryBase(QWidget *parent = nullptr);
protected:
    MainWindow *mainWindow;
    tabSalary *parentTab;
    SUserModel *m_userModel;
    virtual void updateModels(){};
    virtual void updateWidgets(){};
private slots:
    virtual void guiFontChanged(){};
};

#endif // SPAGESALARYBASE_H

