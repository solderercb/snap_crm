#include "ssettingspagerolesandpermissions.h"
#include "ui_ssettingspagerolesandpermissions.h"
#include <QMessageBox>
#include <tabSettings>
#include <SPermissions>

SSettingsPageRolesAndPermissions::SSettingsPageRolesAndPermissions(QWidget *parent) :
    SSettingsPageBase(parent),
    ui(new Ui::SSettingsPageRolesAndPermissions)
{
    ui->setupUi(this);

    m_permissionsModel = new SPermissions();
//    ui->tableViewPermissions->setModel(m_permissionsModel);
    QSortFilterProxyModel *permissions = new QSortFilterProxyModel();
    permissions->setSourceModel(m_permissionsModel);
    permissions->setFilterRegExp("^(?!(RFU)).*$");
    permissions->setFilterKeyColumn(1);
    permissions->sort(1, Qt::AscendingOrder);
    ui->tableViewPermissions->setModel(permissions);

    m_roles = new QSqlQueryModel();
    ui->tableViewRoles->setModel(m_roles);
    connect(ui->tableViewRoles, &QTableView::doubleClicked, this, &SSettingsPageRolesAndPermissions::loadRolePermissions);
    connect(ui->lineEditRoleName, &QLineEdit::textChanged, this, &SSettingsPageRolesAndPermissions::setRoleNameOrDescrChanged);
    connect(ui->lineEditRoleDescription, &QLineEdit::textChanged, this, &SSettingsPageRolesAndPermissions::setRoleNameOrDescrChanged);

    SSettingsPageRolesAndPermissions::updateModels();

#ifdef QT_DEBUG
    loadRolePermissions(m_roles->index(3, 0));
#endif
}

void SSettingsPageRolesAndPermissions::updateModels()
{
    m_roles->setQuery(QString("SELECT `name`, `description`, `id` FROM `roles` ORDER BY `name` ASC"), QSqlDatabase::database("connMain"));
    m_permissionsModel->load();

    ui->tableViewRoles->hideColumn(2);
    ui->tableViewRoles->setColumnWidth(0, 100);
}

void SSettingsPageRolesAndPermissions::saveSettings()
{
    if(m_roleNameOrDescrChanged)
        m_permissionsModel->save(ui->lineEditRoleName->text(), ui->lineEditRoleDescription->text());
    else
        m_permissionsModel->save();

    m_roleNameOrDescrChanged = 0;
}

void SSettingsPageRolesAndPermissions::setRoleNameOrDescrChanged()
{
    m_roleNameOrDescrChanged = 1;
}

void SSettingsPageRolesAndPermissions::loadRolePermissions(const QModelIndex &index)
{
    if(m_permissionsModel->isModelChanged())
    {
        auto result = QMessageBox::question(this, tr("Данные не сохранены"), tr("Разрешения роли изменены!\nСохранить перед загрузкой другой роли?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (result == QMessageBox::Cancel)
        {
            return;
        }
        else if (result == QMessageBox::Yes)
        {
            parentTab->manualSubmit();
        }
    }
    ui->lineEditRoleName->setText(m_roles->data(m_roles->index(index.row(), 0)).toString());
    ui->lineEditRoleDescription->setText(m_roles->data(m_roles->index(index.row(), 1)).toString());
    m_roleNameOrDescrChanged = 0;
    m_permissionsModel->load(m_roles->data(m_roles->index(index.row(), 2)).toString());
}
