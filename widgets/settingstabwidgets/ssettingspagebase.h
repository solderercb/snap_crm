#ifndef SSETTINGSPAGEBASE_H
#define SSETTINGSPAGEBASE_H

#include <widgets/stabpage.h>
#include <QObject>
#include "tabsettings.h"

class SSettingsPageBase : public STabPage
{
    Q_OBJECT
public:
    explicit SSettingsPageBase(QWidget *parent = nullptr);
protected:
    tabSettings *parentTab = nullptr;
    virtual void saveSettings(){};
    virtual void updateModels(){};
};

#endif // SSETTINGSPAGEBASE_H
