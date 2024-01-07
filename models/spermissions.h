#ifndef SPERMISSIONS_H
#define SPERMISSIONS_H
#include <QMetaObject>
#include <QAbstractItemModel>
#include "propstruct.h"
#include "com_sql_queries.h"
#include "sdatabaseauxiliary.h"

class SPermissions : public QStandardItemModel, public SDatabaseAuxiliary
{
    Q_OBJECT
public:
    PROPSTRUCT_FIELD(int, editGlobalSettings, dummy)
    PROPSTRUCT_FIELD(int, receptDevices, dummy)
    PROPSTRUCT_FIELD(int, printStickers, dummy)
    PROPSTRUCT_FIELD(int, issueDevices, dummy)
    PROPSTRUCT_FIELD(int, saleGoods, dummy)
    PROPSTRUCT_FIELD(int, incomeGoods, dummy)
    PROPSTRUCT_FIELD(int, viewClients, dummy)
    PROPSTRUCT_FIELD(int, createNewClient, dummy)
    PROPSTRUCT_FIELD(int, writeOffGoods, dummy)
    PROPSTRUCT_FIELD(int, editGoodsPrices, dummy)
    PROPSTRUCT_FIELD(int, editClients, dummy)
    PROPSTRUCT_FIELD(int, createReports, dummy)
    PROPSTRUCT_FIELD(int, RFU13, dummy)         // разрешения с кодами 13, 14, 17 и 19 не используются
    PROPSTRUCT_FIELD(int, RFU14, dummy)
    PROPSTRUCT_FIELD(int, viewFinancialDocuments, dummy)
    PROPSTRUCT_FIELD(int, createCashRegisters, dummy)
    PROPSTRUCT_FIELD(int, RFU17, dummy)
    PROPSTRUCT_FIELD(int, createZReport, dummy)
    PROPSTRUCT_FIELD(int, RFU19, dummy)
    PROPSTRUCT_FIELD(int, viewCashOrders, dummy)
    PROPSTRUCT_FIELD(int, editGoods, dummy)
    PROPSTRUCT_FIELD(int, readSMS, dummy)
    PROPSTRUCT_FIELD(int, viewAnyRepair, dummy)
    PROPSTRUCT_FIELD(int, useTelephony, dummy)
    PROPSTRUCT_FIELD(int, advEditRepair, dummy)
    PROPSTRUCT_FIELD(int, repairRecepted, dummy)
    PROPSTRUCT_FIELD(int, repairDiagnostics, dummy)
    PROPSTRUCT_FIELD(int, repairNegotiation, dummy)
    PROPSTRUCT_FIELD(int, repairConformed, dummy)
    PROPSTRUCT_FIELD(int, repairInProgress, dummy)
    PROPSTRUCT_FIELD(int, repairWaitingParts, dummy)
    PROPSTRUCT_FIELD(int, repairReady, dummy)
    PROPSTRUCT_FIELD(int, repairReadyWithoutRepair, dummy)
    PROPSTRUCT_FIELD(int, repairIssued, dummy)
    PROPSTRUCT_FIELD(int, repairFaultNotDetected, dummy)
    PROPSTRUCT_FIELD(int, repairOfficeChange, dummy)
    PROPSTRUCT_FIELD(int, repairRefused, dummy)
    PROPSTRUCT_FIELD(int, editWorksPriceList, dummy)
    PROPSTRUCT_FIELD(int, editGoodsCatalog, dummy)
    PROPSTRUCT_FIELD(int, syncGoodsWithShop, dummy)
    PROPSTRUCT_FIELD(int, viewPrice1, dummy)
    PROPSTRUCT_FIELD(int, viewPrice2, dummy)
    PROPSTRUCT_FIELD(int, viewPrice3, dummy)
    PROPSTRUCT_FIELD(int, takeGoodsForRepair, dummy)
    PROPSTRUCT_FIELD(int, giveGoodsForRepair, dummy)
    PROPSTRUCT_FIELD(int, manageWarehouse, dummy)
    PROPSTRUCT_FIELD(int, removeGoodsFromUnissuedRepair, dummy)
    PROPSTRUCT_FIELD(int, paySubsistence, dummy)
    PROPSTRUCT_FIELD(int, remoteLogin, dummy)
    PROPSTRUCT_FIELD(int, addCustomWork, dummy)
    PROPSTRUCT_FIELD(int, undoOutInvoice, dummy)
    PROPSTRUCT_FIELD(int, makeSalaryExtraCharge, dummy)
    PROPSTRUCT_FIELD(int, repairDiagnosisConfirmation, dummy)
    PROPSTRUCT_FIELD(int, viewForeignOfficeCashOrders, dummy)
    PROPSTRUCT_FIELD(int, setNotificationState, dummy)
    PROPSTRUCT_FIELD(int, undoInInvoice, dummy)
    PROPSTRUCT_FIELD(int, beginUnengagedRepair, dummy)
    PROPSTRUCT_FIELD(int, handleOnlineOrders, dummy)
    PROPSTRUCT_FIELD(int, changeOffice, dummy)
    PROPSTRUCT_FIELD(int, setRepairEngineer, dummy)
    PROPSTRUCT_FIELD(int, setRepairColor, dummy)
    PROPSTRUCT_FIELD(int, startVideoRecording, dummy)
    PROPSTRUCT_FIELD(int, sendSMS, dummy)
    PROPSTRUCT_FIELD(int, repairIssuedInCredit, dummy)
    PROPSTRUCT_FIELD(int, handleCashlessOrders, dummy)
    PROPSTRUCT_FIELD(int, createDatabaseBackups, dummy)
    PROPSTRUCT_FIELD(int, createTasks, dummy)
    PROPSTRUCT_FIELD(int, veiwAllTasks, dummy)
    PROPSTRUCT_FIELD(int, addGoodsFromWarehouse, dummy)
    PROPSTRUCT_FIELD(int, enableSparepartRequestManager, dummy)
    PROPSTRUCT_FIELD(int, createBackdatedDocuments, dummy)
    PROPSTRUCT_FIELD(int, moveRepairToOffice, dummy)
    PROPSTRUCT_FIELD(int, createQuickRepair, dummy)
    PROPSTRUCT_FIELD(int, viewForeignOfficeRepair, dummy)
    PROPSTRUCT_FIELD(int, useCashRegisterEquipment, dummy)
    PROPSTRUCT_FIELD(int, setRepairManager, dummy)
    PROPSTRUCT_FIELD(int, viewAnyWorkList, dummy)
    PROPSTRUCT_FIELD(int, viewRepairLog, dummy)
    PROPSTRUCT_FIELD(int, editCashRegisterEquipmentSettings, dummy)
    PROPSTRUCT_FIELD(int, editCartridgeCards, dummy)
    PROPSTRUCT_FIELD(int, sentEmail, dummy)
    PROPSTRUCT_FIELD(int, RFU82, dummy)    // разрешения 82..100 зарезервированы для будущих версий АСЦ :-, dummy)
    PROPSTRUCT_FIELD(int, RFU83, dummy)
    PROPSTRUCT_FIELD(int, RFU84, dummy)
    PROPSTRUCT_FIELD(int, RFU85, dummy)
    PROPSTRUCT_FIELD(int, RFU86, dummy)
    PROPSTRUCT_FIELD(int, RFU87, dummy)
    PROPSTRUCT_FIELD(int, RFU88, dummy)
    PROPSTRUCT_FIELD(int, RFU89, dummy)
    PROPSTRUCT_FIELD(int, RFU90, dummy)
    PROPSTRUCT_FIELD(int, RFU91, dummy)
    PROPSTRUCT_FIELD(int, RFU92, dummy)
    PROPSTRUCT_FIELD(int, RFU93, dummy)
    PROPSTRUCT_FIELD(int, RFU94, dummy)
    PROPSTRUCT_FIELD(int, RFU95, dummy)
    PROPSTRUCT_FIELD(int, RFU96, dummy)
    PROPSTRUCT_FIELD(int, RFU97, dummy)
    PROPSTRUCT_FIELD(int, RFU98, dummy)
    PROPSTRUCT_FIELD(int, RFU99, dummy)
    PROPSTRUCT_FIELD(int, RFU100, dummy)
    PROPSTRUCT_FIELD(int, editRepairIncomingSet, dummy)
    PROPSTRUCT_FIELD(int, editAnyComment, dummy)
    PROPSTRUCT_FIELD(int, advEditWorkList, dummy)
    PROPSTRUCT_FIELD(int, viewClientPassportData, dummy)
    PROPSTRUCT_FIELD(int, viewClientBankData, dummy)
    PROPSTRUCT_FIELD(int, editPaymentSystemInCommittedCashRegisters, dummy)
    PROPSTRUCT_FIELD(int, viewEmployeesSalary, dummy)
    PROPSTRUCT_FIELD(int, workWithTechReports, dummy)
public:
    explicit SPermissions();
    QStringList list();
    void load(const QString &roles);
    void load();
    void save(const QString &name = QString(), const QString &description = QString());
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setAll(const int state);
    bool isModelChanged();
    int currentRole();
protected:
    int rowToPropertyId(const QModelIndex &index) const;
    int rowToPermissionId(const QModelIndex &index) const;
    int permissionIdToRow(const int) const;
    int permissionIdToPropertyId(const int) const;
    void translateNames();
private:
    QMap<QString, QString> i_fieldNames;    // затычка; в данном классе не используется
    bool m_modelEdited = 0;
    int m_currentRole = 0;
};

#include "global.h" // Это должно быть именно здесь

inline SPermissions::SPermissions()
{
    setRowCount(metaObject()->propertyCount() - metaObject()->propertyOffset());
    setColumnCount(2);
}

inline QStringList SPermissions::list()
{
    QStringList list;
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        list << tr(metaObject()->property(i).name());

    return list;
}

inline void SPermissions::load(const QString &roles)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    QStringList rolesList = roles.split(',');
    if(rolesList.count() == 1)
        m_currentRole = rolesList.first().toInt();
    else
        m_currentRole = 0;

    setAll(0);
    query->exec(QUERY_SEL_PERMISSIONS(roles));
    while (query->next())
    {
        if( query->value(0).toInt() > (metaObject()->propertyCount() - metaObject()->propertyOffset()) )
        {
            QString error = tr("Разрешение %1 не объявлено в классе %2!").arg(query->value(0).toInt()).arg(metaObject()->className());
            errorToLog(metaObject()->className(), error);
            continue;
        }
        setData(index(permissionIdToRow(query->value(0).toInt()), 0), 2, Qt::CheckStateRole);
    }
    m_modelEdited = 0;

    delete query;
    endResetModel();
}

inline void SPermissions::load()
{
    if(!m_currentRole)
        return;

    load(QString::number(m_currentRole));
}

inline void SPermissions::save(const QString &name, const QString &description)
{
    if(!m_currentRole)
        return;

    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    query->exec(QString("DELETE FROM `permissions_roles` WHERE `role_id` = %1").arg(m_currentRole));
    query->prepare(QString("INSERT INTO `permissions_roles` (`role_id`, `permission_id`) VALUES (:role, :perm)"));
    query->bindValue(":role", m_currentRole);
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
    {
        if(metaObject()->property(i).read(this).toInt() == 0)
            continue;

        query->bindValue(":perm", permissionIdToRow(i));
        nErr = query->exec();

        if(!nErr)
        {
            errorToLog(metaObject()->className(), query->lastError().text());
            throw 1;
        }
    }

    if(!name.isEmpty() || !description.isEmpty())
        nErr = query->exec(QString("UPDATE `roles` SET `name` = '%1', `description` = '%2' WHERE `id` = %3").arg(name, description).arg(m_currentRole));

    if(!nErr)
    {
        errorToLog(metaObject()->className(), query->lastError().text());
        throw 1;
    }

    // TODO Запись в журнал?

    m_modelEdited = 0;
    delete query;
}

inline QVariant SPermissions::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(index.column() == 1)
            return tr(metaObject()->property(rowToPropertyId(index)).name());
    }
    else if (role == Qt::CheckStateRole)
        if(index.column() == 0)
            return metaObject()->property(rowToPropertyId(index)).read(this);

    return QVariant();
}

inline bool SPermissions::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole)
    {
        m_modelEdited = 1;
        return metaObject()->property(rowToPropertyId(index)).write(this, value);
    }

    return true;
}

inline Qt::ItemFlags SPermissions::flags(const QModelIndex &index) const
{
    if(index.column() == 0)
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled;
}

inline void SPermissions::setAll(const int state)
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        setData(index(i - metaObject()->propertyOffset(), 0), state, Qt::CheckStateRole);
}

inline bool SPermissions::isModelChanged()
{
    return m_modelEdited;
}

inline int SPermissions::currentRole()
{
    return m_currentRole;
}

inline int SPermissions::rowToPropertyId(const QModelIndex &index) const
{
    return index.row() + metaObject()->propertyOffset();
}

inline int SPermissions::rowToPermissionId(const QModelIndex &index) const
{
    return index.row() + 1;
}

inline int SPermissions::permissionIdToRow(const int id) const
{
    return id - 1;
}

inline int SPermissions::permissionIdToPropertyId(const int permissionId) const
{
    return permissionIdToRow(permissionId) + metaObject()->propertyOffset();
}

/* Перевод названий разрешений
 * Локализованные названия будут использоваться в настройках
*/
inline void SPermissions::translateNames()
{
    tr("editGlobalSettings");
    tr("receptDevices");
    tr("printStickers");
    tr("issueDevices");
    tr("saleGoods");
    tr("incomeGoods");
    tr("viewClients");
    tr("createNewClient");
    tr("writeOffGoods");
    tr("editGoodsPrices");
    tr("editClients");
    tr("createReports");
    tr("viewFinancialDocuments");
    tr("createCashRegisters");
    tr("createZReport");
    tr("viewCashOrders");
    tr("editGoods");
    tr("readSMS");
    tr("viewAnyRepair");
    tr("useTelephony");
    tr("advEditRepair");
    tr("repairRecepted");
    tr("repairDiagnostics");
    tr("repairNegotiation");
    tr("repairConformed");
    tr("repairInProgress");
    tr("repairWaitingParts");
    tr("repairReady");
    tr("repairReadyWithoutRepair");
    tr("repairIssued");
    tr("repairFaultNotDetected");
    tr("repairOfficeChange");
    tr("repairRefused");
    tr("editWorksPriceList");
    tr("editGoodsCatalog");
    tr("syncGoodsWithShop");
    tr("viewPrice1");
    tr("viewPrice2");
    tr("viewPrice3");
    tr("takeGoodsForRepair");
    tr("giveGoodsForRepair");
    tr("manageWarehouse");
    tr("removeGoodsFromUnissuedRepair");
    tr("paySubsistence");
    tr("remoteLogin");
    tr("addCustomWork");
    tr("undoOutInvoice");
    tr("makeSalaryExtraCharge");
    tr("repairDiagnosisConfirmation");
    tr("viewForeignOfficeCashOrders");
    tr("setNotificationState");
    tr("undoInInvoice");
    tr("beginUnengagedRepair");
    tr("handleOnlineOrders");
    tr("changeOffice");
    tr("setRepairEngineer");
    tr("setRepairColor");
    tr("startVideoRecording");
    tr("sendSMS");
    tr("repairIssuedInCredit");
    tr("handleCashlessOrders");
    tr("createDatabaseBackups");
    tr("createTasks");
    tr("veiwAllTasks");
    tr("addGoodsFromWarehouse");
    tr("enableSparepartRequestManager");
    tr("createBackdatedDocuments");
    tr("moveRepairToOffice");
    tr("createQuickRepair");
    tr("viewForeignOfficeRepair");
    tr("useCashRegisterEquipment");
    tr("setRepairManager");
    tr("viewAnyWorkList");
    tr("viewRepairLog");
    tr("editCashRegisterEquipmentSettings");
    tr("editCartridgeCards");
    tr("sentEmail");
    tr("editRepairIncomingSet");
    tr("editAnyComment");
    tr("advEditWorkList");
    tr("viewClientPassportData");
    tr("viewClientBankData");
    tr("editPaymentSystemInCommittedCashRegisters");
    tr("viewEmployeesSalary");
    tr("workWithTechReports");
}

#endif // SPERMISSIONS_H
