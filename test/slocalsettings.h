#ifndef SLOCALSETTINGS_H
#define SLOCALSETTINGS_H

#include <QString>

class SLocalSettings
{
public:
    enum SettingsVariant {RepairsGrid, RepairWorksGrid, SaleItemsGrid};
    explicit SLocalSettings(){};
    static QString appSettingsPath(){return m_logsDir;};
    static void setAppSettingsPath(const QString &settingsPath){m_logsDir = settingsPath;};
private:
    static QString m_logsDir;
};

#endif // SLOCALSETTINGS_H
