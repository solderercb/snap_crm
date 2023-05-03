#ifndef SLOCALSETTINGS_H
#define SLOCALSETTINGS_H

#include <QApplication>
#include <QObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QResource>
#include <QIODevice>
#include <QCryptographicHash>
#include <QCollator>
#include <3rdparty/QSerializer>
#define ASC_APP_PATH   "C:/Program Files (x86)/ASCApp"
#define ASC_SETTINGS_PATH   "VLab/ASC.exe_Url_1ttwwpflaim4gbzvmtfpiarttwuebugu"    // путь к файлу настроек АЦС для пути к приложению "C:\Program Files (x86)\ASCApp\ASC.exe"

class SLocalSettings : public QObject
{
    Q_OBJECT
public:
    // Списки настроек (файлы в папке cfg АСЦ CRM); настройки, помеченные TODO, в АСЦ v3.7.37.1184 не реализованы
    enum SettingsVariant {UserSettings = 1, RepairsGrid, CartridgesGrid, RepairWorksGrid, PriceGrid, RepairMassEditorGrid, CommentsGrid,
                      StoreItemsGrid, ProductsGrid, ArrivalGrid, SaleGrid/*, TODO: Товары->Документы*/, BuyRequestManagerGrid, StoreManagementGrid, PriceEditorGrid, XmlExportGrid, StockTakingGrid,
                      MoveGrid/*, TODO: таблица Документы в карточке товара*/, HistoryGrid,
                      CustomersGrid, SmsGrid,
                      CustomerHistoryGrid, CustomerPurchasesGrid, CustomerRepairsGrid, DealerSalesGrid,
                      KassaGrid, InvoicesGrid, AdditionalPaymentsGrid, SalaryRepairsGrid, SalaryRepairWorksGrid, SalaryRepairPartsGrid, SalaryReceptedGrid, SalaryIssuedGrid,
                      SalarySalesGrid, SalaryItemsGrid, SalarySalePartsGrid, SalaryPaymentsGrid /*, TODO: таблицы в модуле Зарплата*/,
                      EmployeesReportGrid, VisitSourceGrid, StatusChecksGrid, EmployeeActivityGrid, FFRSalaryGrid,
                      TasksGrid};
    Q_ENUM(SettingsVariant)
    static QString appSettingsPath();
    bool import(QSerializer *obj, SettingsVariant variant = UserSettings, const QString subVariant = "");
    bool read(QSerializer *obj, SettingsVariant variant = UserSettings, const QString subVariant = "");
    bool save(QSerializer *obj, SettingsVariant variant = UserSettings, const QString subVariant = "");
protected:
private:
    SettingsVariant m_settingsVariant;
    void genSettingsFileName(QFile &file, const QString subVariant = "");
    bool selMostRecentSettingFile(const QString &fileName);
    bool genSettingsFileFullPath(QFile &file);
    bool genAscSettingsFileFullPath(QFile &file);
    bool openFile(QFile &file, QIODevice::OpenModeFlag mode = QIODevice::ReadOnly);
    bool read(QSerializer *obj, QFile &file);
    bool save(QSerializer *obj, QFile &file);
};

#endif // SLOCALSETTINGS_H
