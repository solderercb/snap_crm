#ifndef SSETTINGSPAGEROLESANDPERMISSIONS_H
#define SSETTINGSPAGEROLESANDPERMISSIONS_H

#include "..\stabpage.h"
#include <QObject>
#include "models/spermissions.h"
#include "widgets/settingstabwidgets/ssettingspagebase.h"

namespace Ui {
class SSettingsPageRolesAndPermissions;
}

class SSettingsPageRolesAndPermissions : public SSettingsPageBase
{
    Q_OBJECT
public:
    explicit SSettingsPageRolesAndPermissions(QWidget *parent = nullptr);
private:
    Ui::SSettingsPageRolesAndPermissions *ui;
    SPermissions *m_permissionsModel;
    QSqlQueryModel *m_roles;
    bool m_roleNameOrDescrChanged = 0;
    void saveSettings() override;
    void setRoleNameOrDescrChanged();
#ifdef QT_DEBUG
    void randomFill() override {};
#endif
protected slots:
    void loadRolePermissions(const QModelIndex &index);
    void updateModels() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override {};
    void test_scheduler2_handler() override {};
#endif
};

#endif // SSETTINGSPAGEROLESANDPERMISSIONS_H
