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
#include <3rdparty/QSerializer/QSerializer>

class SLocalSettings : public QObject
{
    Q_OBJECT
public:
    // Списки настроек (файлы в папке cfg АСЦ CRM); настройки, помеченные TODO, в АСЦ v3.7.37.1184 не реализованы
    enum SettingsVariant {UserSettings = 1, RepairsGrid, CartridgesGrid, RepairWorksGrid, PriceGrid, RepairMassEditorGrid, CommentsGrid,
                      StoreItemsGrid, ProductsGrid, ArrivalGrid, SaleItemsGrid/*, TODO: Товары->Документы*/, BuyRequestManagerGrid, StoreManagementGrid, PriceEditorGrid, XmlExportGrid, StockTakingGrid,
                      MoveGrid/*, TODO: таблица Документы в карточке товара*/, HistoryGrid,
                      CustomersGrid, SmsGrid,
                      CustomerHistoryGrid, CustomerPurchasesGrid, CustomerRepairsGrid, DealerSalesGrid,
                      KassaGrid, InvoicesGrid, AdditionalPaymentsGrid, SalaryExtraChargesGrid, SalaryRepairsGrid, SalaryRepairWorksGrid, SalaryRepairPartsGrid, SalaryReceptedGrid, SalaryIssuedGrid,
                      SalarySalesGrid, SalaryItemsGrid, SalarySalePartsGrid, SalaryPaymentsGrid,
                      EmployeesReportGrid, VisitSourceGrid, StatusChecksGrid, EmployeeActivityGrid, FFRSalaryGrid,
                      TasksGrid,
                      CartridgeMaterialsGrid,
                      ClientsMatchGrid, RepairsMatchGrid};
    Q_ENUM(SettingsVariant)
    enum Operation {SaveFile = 0, ReadFile};
    static QString appSettingsPath();
    bool import(QSerializer *obj, SettingsVariant variant = UserSettings, const QString subVariant = "");
    bool read(QSerializer *obj, SettingsVariant variant = UserSettings, const QString subVariant = "");
    bool save(QSerializer *obj, SettingsVariant variant = UserSettings, const QString subVariant = "");
    QString ascAppDataPath(const QString &executablePath);
protected:
private:
    static const QLatin1String base32StringEncodeMap;
    SettingsVariant m_settingsVariant;
    void genSettingsFileName(QFile &file, const QString subVariant = "");
    bool selMostRecentSettingFile(const QString &fileName);
    bool genSettingsFileFullPath(QFile &file, Operation op);
    bool genAscSettingsFileFullPath(QFile &file);
    bool openFile(QFile &file, QIODevice::OpenModeFlag mode = QIODevice::ReadOnly);
    bool read(QSerializer *obj, QFile &file);
    bool save(QSerializer *obj, QFile &file);
    QString base32StringSuitableForDirName(const QByteArray &payloadSha1);
};

#endif // SLOCALSETTINGS_H
