#ifndef SPAGESALARYBASE_H
#define SPAGESALARYBASE_H

#include <QObject>
#include <SWidget>

class MainWindow;
class tabSalary;
class SUserModel;

class SPageSalaryBase : public SWidget
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

