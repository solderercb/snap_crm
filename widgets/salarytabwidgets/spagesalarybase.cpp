#include "spagesalarybase.h"

SPageSalaryBase::SPageSalaryBase(QWidget *parent) :
    QWidget(parent)
{
    parentTab = MainWindow::getInstance()->findChild<tabSalary*>();
    mainWindow = MainWindow::getInstance();
    connect(parentTab, &tabSalary::updateDaughterTabsModels, this, &SPageSalaryBase::updateModels);
    connect(parentTab, &tabSalary::updateDaughterTabsWidgets, this, &SPageSalaryBase::updateWidgets);
    connect(userDbData, &SUserSettings::fontSizeChanged, this, &SPageSalaryBase::guiFontChanged);
}
