#include "ssettingspagebase.h"

SSettingsPageBase::SSettingsPageBase(QWidget *parent) :
    STabPage(parent)
{
    parentTab = MainWindow::getInstance()->findChild<tabSettings*>();
    connect(parentTab, &tabSettings::saveSettings, this, &SSettingsPageBase::saveSettings);
    connect(parentTab, &tabSettings::updateDaughterPagesModels, this, &SSettingsPageBase::updateModels);
}
