#ifndef SSETTINGSPAGEGLOBAL_H
#define SSETTINGSPAGEGLOBAL_H

#include <QWidget>
#include <QPushButton>
#include "models/scomsettings.h"
#include "widgets/settingstabwidgets/ssettingspagebase.h"

namespace Ui {
class SSettingsPageGlobal;
}

class SSettingsPageGlobal : public SSettingsPageBase
{
    Q_OBJECT

public:
    explicit SSettingsPageGlobal(QWidget *parent = nullptr);
    ~SSettingsPageGlobal();
    void updateWidgets();
    void updateWidgetsOfVoipGroup();
    void updateWidgetsOfOnlineStoreGroup();
    void updateWidgetsOfNotificationGroup();
public slots:
    void toggleLineEditEchoMode();
    void voipTest();
#ifdef QT_DEBUG
    void randomFill() override {};
#endif
private:
    Ui::SSettingsPageGlobal *ui;
    SComSettings *comSettings;
    QMap<int, QWidget*> m_extraWidgets;
    QPushButton *voipTestButton = nullptr;
    void saveSettings() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override {};
    void test_scheduler2_handler() override {};
#endif
};

#endif // SSETTINGSPAGEGLOBAL_H
