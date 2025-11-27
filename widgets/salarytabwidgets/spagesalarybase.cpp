#include "spagesalarybase.h"
#include <ProjectGlobals>
#include <SUserSettings>
#include <Mainwindow>
#include <tabSalary>

SPageSalaryBase::SPageSalaryBase(QWidget *parent) :
    SWidget(parent)
{
    parentTab = MainWindow::getInstance()->findChild<tabSalary*>();
    mainWindow = MainWindow::getInstance();
    connect(parentTab, &tabSalary::updateDaughterTabsModels, this, &SPageSalaryBase::updateModels);
    connect(parentTab, &tabSalary::updateDaughterTabsWidgets, this, &SPageSalaryBase::updateWidgets);
    connect(userDbData, &SUserSettings::fontSizeChanged, this, &SPageSalaryBase::guiFontChanged);
}
