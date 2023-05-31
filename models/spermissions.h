#ifndef SPERMISSIONS_H
#define SPERMISSIONS_H
#include <QMetaObject>
#include <QAbstractItemModel>
#include "propstruct.h"
#include "com_sql_queries.h"

class SPermissions : public QStandardItemModel
{
    Q_OBJECT
public:
    PROPSTRUCT_FIELD(int, editGlobalSettings)
    PROPSTRUCT_FIELD(int, receptDevices)
    PROPSTRUCT_FIELD(int, printStickers)
    PROPSTRUCT_FIELD(int, issueDevices)
    PROPSTRUCT_FIELD(int, saleGoods)
    PROPSTRUCT_FIELD(int, incomeGoods)
    PROPSTRUCT_FIELD(int, viewClients)
    PROPSTRUCT_FIELD(int, createNewClient)
    PROPSTRUCT_FIELD(int, writeOffGoods)
    PROPSTRUCT_FIELD(int, editGoodsPrices)
    PROPSTRUCT_FIELD(int, editClients)
    PROPSTRUCT_FIELD(int, createReports)
    PROPSTRUCT_FIELD(int, RFU13)         // разрешения с кодами 13, 14, 17 и 19 не используются
    PROPSTRUCT_FIELD(int, RFU14)
    PROPSTRUCT_FIELD(int, viewFinancialDocuments)
    PROPSTRUCT_FIELD(int, createCashRegisters)
    PROPSTRUCT_FIELD(int, RFU17)
    PROPSTRUCT_FIELD(int, createZReport)
    PROPSTRUCT_FIELD(int, RFU19)
    PROPSTRUCT_FIELD(int, viewCashOrders)
    PROPSTRUCT_FIELD(int, editGoods)
    PROPSTRUCT_FIELD(int, readSMS)
    PROPSTRUCT_FIELD(int, viewAnyRepair)
    PROPSTRUCT_FIELD(int, useTelephony)
    PROPSTRUCT_FIELD(int, advEditRepair)
    PROPSTRUCT_FIELD(int, repairRecepted)
    PROPSTRUCT_FIELD(int, repairDiagnostics)
    PROPSTRUCT_FIELD(int, repairNegotiation)
    PROPSTRUCT_FIELD(int, repairConformed)
    PROPSTRUCT_FIELD(int, repairInProgress)
    PROPSTRUCT_FIELD(int, repairWaitingParts)
    PROPSTRUCT_FIELD(int, repairReady)
    PROPSTRUCT_FIELD(int, repairReadyWithoutRepair)
    PROPSTRUCT_FIELD(int, repairIssued)
    PROPSTRUCT_FIELD(int, repairFaultNotDetected)
    PROPSTRUCT_FIELD(int, repairOfficeChange)
    PROPSTRUCT_FIELD(int, repairRefused)
    PROPSTRUCT_FIELD(int, editWorksPriceList)
    PROPSTRUCT_FIELD(int, editGoodsCatalog)
    PROPSTRUCT_FIELD(int, syncGoodsWithShop)
    PROPSTRUCT_FIELD(int, viewPrice1)
    PROPSTRUCT_FIELD(int, viewPrice2)
    PROPSTRUCT_FIELD(int, viewPrice3)
    PROPSTRUCT_FIELD(int, takeGoodsForRepair)
    PROPSTRUCT_FIELD(int, giveGoodsForRepair)
    PROPSTRUCT_FIELD(int, manageWarehouse)
    PROPSTRUCT_FIELD(int, removeGoodsFromUnissuedRepair)
    PROPSTRUCT_FIELD(int, paySubsistence)
    PROPSTRUCT_FIELD(int, remoteLogin)
    PROPSTRUCT_FIELD(int, addCustomWork)
    PROPSTRUCT_FIELD(int, undoOutInvoice)
    PROPSTRUCT_FIELD(int, makeSalaryExtraCharge)
    PROPSTRUCT_FIELD(int, repairDiagnosisConfirmation)
    PROPSTRUCT_FIELD(int, viewForeignOfficeCashOrders)
    PROPSTRUCT_FIELD(int, setNotificationState)
    PROPSTRUCT_FIELD(int, undoInInvoice)
    PROPSTRUCT_FIELD(int, beginUnengagedRepair)
    PROPSTRUCT_FIELD(int, handleOnlineOrders)
    PROPSTRUCT_FIELD(int, changeOffice)
    PROPSTRUCT_FIELD(int, setRepairEngineer)
    PROPSTRUCT_FIELD(int, setRepairColor)
    PROPSTRUCT_FIELD(int, startVideoRecording)
    PROPSTRUCT_FIELD(int, sendSMS)
    PROPSTRUCT_FIELD(int, repairIssuedInCredit)
    PROPSTRUCT_FIELD(int, handleCashlessOrders)
    PROPSTRUCT_FIELD(int, createDatabaseBackups)
    PROPSTRUCT_FIELD(int, createTasks)
    PROPSTRUCT_FIELD(int, veiwAllTasks)
    PROPSTRUCT_FIELD(int, addGoodsFromWarehouse)
    PROPSTRUCT_FIELD(int, enableSparepartRequestManager)
    PROPSTRUCT_FIELD(int, createBackdatedDocuments)
    PROPSTRUCT_FIELD(int, moveRepairToOffice)
    PROPSTRUCT_FIELD(int, createQuickRepair)
    PROPSTRUCT_FIELD(int, viewForeignOfficeRepair)
    PROPSTRUCT_FIELD(int, useCashRegisterEquipment)
    PROPSTRUCT_FIELD(int, setRepairManager)
    PROPSTRUCT_FIELD(int, viewAnyWorkList)
    PROPSTRUCT_FIELD(int, viewRepairLog)
    PROPSTRUCT_FIELD(int, editCashRegisterEquipmentSettings)
    PROPSTRUCT_FIELD(int, editCartridgeCards)
    PROPSTRUCT_FIELD(int, sentEmail)
    PROPSTRUCT_FIELD(int, RFU82)    // разрешения 82..100 зарезервированы для будущих версий АСЦ :-)
    PROPSTRUCT_FIELD(int, RFU83)
    PROPSTRUCT_FIELD(int, RFU84)
    PROPSTRUCT_FIELD(int, RFU85)
    PROPSTRUCT_FIELD(int, RFU86)
    PROPSTRUCT_FIELD(int, RFU87)
    PROPSTRUCT_FIELD(int, RFU88)
    PROPSTRUCT_FIELD(int, RFU89)
    PROPSTRUCT_FIELD(int, RFU90)
    PROPSTRUCT_FIELD(int, RFU91)
    PROPSTRUCT_FIELD(int, RFU92)
    PROPSTRUCT_FIELD(int, RFU93)
    PROPSTRUCT_FIELD(int, RFU94)
    PROPSTRUCT_FIELD(int, RFU95)
    PROPSTRUCT_FIELD(int, RFU96)
    PROPSTRUCT_FIELD(int, RFU97)
    PROPSTRUCT_FIELD(int, RFU98)
    PROPSTRUCT_FIELD(int, RFU99)
    PROPSTRUCT_FIELD(int, RFU100)
    PROPSTRUCT_FIELD(int, editRepairIncomingSet)
    PROPSTRUCT_FIELD(int, editAnyComment)
    PROPSTRUCT_FIELD(int, advEditWorkList)
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
    void translateNames();
private:
    bool m_modelEdited = 0;
    int m_currentRole = 0;
};

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
        setData(index(query->value(0).toInt() - metaObject()->propertyOffset() + 1, 0), 2, Qt::CheckStateRole);
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

        query->bindValue(":perm", i - metaObject()->propertyOffset() + 1);
        nErr = query->exec();

        if(!nErr)
            throw 1;
    }

    if(!name.isEmpty() || !description.isEmpty())
        nErr = query->exec(QString("UPDATE `roles` SET `name` = '%1', `description` = '%2' WHERE `id` = %3").arg(name, description).arg(m_currentRole));

    if(!nErr)
        throw 1;

    // TODO Запись в журнал?

    m_modelEdited = 0;
    delete query;
}

inline QVariant SPermissions::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(index.column() == 1)
            return tr(metaObject()->property(index.row() + metaObject()->propertyOffset()).name());
    }
    else if (role == Qt::CheckStateRole)
        if(index.column() == 0)
            return metaObject()->property(index.row() + metaObject()->propertyOffset()).readOnGadget(this);

    return QVariant();
}

inline bool SPermissions::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole)
    {
        m_modelEdited = 1;
        return metaObject()->property(index.row() + metaObject()->propertyOffset()).writeOnGadget(this, value);
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
}

#endif // SPERMISSIONS_H
