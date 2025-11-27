#ifndef SSETTINGSPAGEUSER_H
#define SSETTINGSPAGEUSER_H

#include <QObject>
#include <SSettingsPageBase>

class QWidget;

namespace Ui {
class SSettingsPageUser;
}

class SSettingsPageUser : public SSettingsPageBase
{
    Q_OBJECT

public:
    explicit SSettingsPageUser(QWidget *parent = nullptr);
    ~SSettingsPageUser();
    void updateWidgets();
public slots:
#ifdef QT_DEBUG
    void randomFill() override {};
#endif
private:
    Ui::SSettingsPageUser *ui;
    QMap<int, QWidget*> m_extraWidgets;
//    void SSettingsPageUser::updateWidgetsOf...Group();
    void saveSettings() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override {};
    void test_scheduler2_handler() override {};
#endif
};

#endif // SSETTINGSPAGEUSER_H
