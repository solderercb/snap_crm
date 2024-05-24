function Controller() {
  installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
  installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
//  installer.setTemporaryRepositories(("https://github.com/solderercb/snap_crm/releases/latest/download"), true); // репозиторий будет добавлен в maintanancetool.dat; попытка разобраться в исходных кодах installer framework чтобы этого избежать не увенчалась успехом
}
