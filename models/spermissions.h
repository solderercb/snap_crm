#ifndef SPERMISSIONS_H
#define SPERMISSIONS_H
#include <QMetaObject>
#include <QAbstractItemModel>
#include <SSettingsBase>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SSingleRowModelBase>

#define PROPSTRUCT_MEMBER(name) name

#undef PROPSTRUCT_DECLARE_MEMBER
#define PROPSTRUCT_DECLARE_MEMBER(type, name)                                                           \
    type name = type();

#define PROPSTRUCT_GET(name) get_##name
#define PROPSTRUCT_SET(name) set_##name

#undef PROPSTRUCT_BIND_FIELD
#define PROPSTRUCT_BIND_FIELD(type, name)                                                               \
    Q_PROPERTY(type name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                          \
    private:                                                                                            \
    type PROPSTRUCT_GET(name)() const                                                                   \
    {                                                                                                   \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(const type &var##name)                                                    \
    {                                                                                                   \
        name = QVariant(var##name).value<type>();                                                       \
    }

#undef PROPSTRUCT_GET_VALUE_FOR_REPORT
#define PROPSTRUCT_GET_VALUE_FOR_REPORT(varName)                                                        \
    public Q_SLOTS:                                                                                     \
        QVariant get_##varName##_for_report()                                                           \
        {                                                                                               \
            return varName;                                                                             \
        };

#ifdef PROPSTRUCT_FIELD
#undef PROPSTRUCT_FIELD
#endif

#define PROPSTRUCT_ENUM_FIELD(name, ...)  C_##name,
#define PROPSTRUCT_FIELD(type, ...) PROPSTRUCT_ENUM_FIELD(__VA_ARGS__)

#undef PROPSTRUCT_FIELDS
#define PROPSTRUCT_FIELDS                                                                               \
    PROPSTRUCT_FIELD(int, editGlobalSettings)                                                           \
    PROPSTRUCT_FIELD(int, receptDevices)                                                                \
    PROPSTRUCT_FIELD(int, printStickers)                                                                \
    PROPSTRUCT_FIELD(int, issueDevices)                                                                 \
    PROPSTRUCT_FIELD(int, saleGoods)                                                                    \
    PROPSTRUCT_FIELD(int, incomeGoods)                                                                  \
    PROPSTRUCT_FIELD(int, viewClients)                                                                  \
    PROPSTRUCT_FIELD(int, createNewClient)                                                              \
    PROPSTRUCT_FIELD(int, writeOffGoods)                                                                \
    PROPSTRUCT_FIELD(int, editGoodsPrices)                                                              \
    PROPSTRUCT_FIELD(int, editClients)                                                                  \
    PROPSTRUCT_FIELD(int, createReports)                                                                \
    PROPSTRUCT_FIELD(int, RFU13)         /* разрешения с кодами 13, 14, 17 и 19 не используются */      \
    PROPSTRUCT_FIELD(int, RFU14)                                                                        \
    PROPSTRUCT_FIELD(int, viewFinancialDocuments)                                                       \
    PROPSTRUCT_FIELD(int, createCashRegisters)                                                          \
    PROPSTRUCT_FIELD(int, RFU17)                                                                        \
    PROPSTRUCT_FIELD(int, createZReport)                                                                \
    PROPSTRUCT_FIELD(int, RFU19)                                                                        \
    PROPSTRUCT_FIELD(int, viewCashOrders)                                                               \
    PROPSTRUCT_FIELD(int, editGoods)                                                                    \
    PROPSTRUCT_FIELD(int, readSMS)                                                                      \
    PROPSTRUCT_FIELD(int, viewAnyRepair)                                                                \
    PROPSTRUCT_FIELD(int, useTelephony)                                                                 \
    PROPSTRUCT_FIELD(int, advEditRepair)                                                                \
    PROPSTRUCT_FIELD(int, RFU26)   /* В ранних версиях АСЦ разрешения 26..37, 53 и 64 позволяли */      \
    PROPSTRUCT_FIELD(int, RFU27)   /* включать статус ремонта из списка жестко заданных; */             \
    PROPSTRUCT_FIELD(int, RFU28)   /* позже настройка таких разрешений была перенесена на */            \
    PROPSTRUCT_FIELD(int, RFU29)   /* страницу редактирования статусов */                               \
    PROPSTRUCT_FIELD(int, RFU30)                                                                        \
    PROPSTRUCT_FIELD(int, RFU31)                                                                        \
    PROPSTRUCT_FIELD(int, RFU32)                                                                        \
    PROPSTRUCT_FIELD(int, RFU33)                                                                        \
    PROPSTRUCT_FIELD(int, RFU34)                                                                        \
    PROPSTRUCT_FIELD(int, RFU35)                                                                        \
    PROPSTRUCT_FIELD(int, RFU36)                                                                        \
    PROPSTRUCT_FIELD(int, RFU37)                                                                        \
    PROPSTRUCT_FIELD(int, editWorksPriceList)                                                           \
    PROPSTRUCT_FIELD(int, editGoodsCatalog)                                                             \
    PROPSTRUCT_FIELD(int, syncGoodsWithShop)                                                            \
    PROPSTRUCT_FIELD(int, viewPrice1)                                                                   \
    PROPSTRUCT_FIELD(int, viewPrice2)                                                                   \
    PROPSTRUCT_FIELD(int, viewPrice3)                                                                   \
    PROPSTRUCT_FIELD(int, takeGoodsForRepair)                                                           \
    PROPSTRUCT_FIELD(int, giveGoodsForRepair)                                                           \
    PROPSTRUCT_FIELD(int, manageWarehouse)                                                              \
    PROPSTRUCT_FIELD(int, removeGoodsFromUnissuedRepair)                                                \
    PROPSTRUCT_FIELD(int, paySubsistence)                                                               \
    PROPSTRUCT_FIELD(int, remoteLogin)                                                                  \
    PROPSTRUCT_FIELD(int, addCustomWork)                                                                \
    PROPSTRUCT_FIELD(int, undoOutInvoice)                                                               \
    PROPSTRUCT_FIELD(int, makeSalaryExtraCharge)                                                        \
    PROPSTRUCT_FIELD(int, RFU53)                                                                        \
    PROPSTRUCT_FIELD(int, viewForeignOfficeCashOrders)                                                  \
    PROPSTRUCT_FIELD(int, setNotificationState)                                                         \
    PROPSTRUCT_FIELD(int, undoInInvoice)                                                                \
    PROPSTRUCT_FIELD(int, beginUnengagedRepair)                                                         \
    PROPSTRUCT_FIELD(int, handleOnlineOrders)                                                           \
    PROPSTRUCT_FIELD(int, changeOffice)                                                                 \
    PROPSTRUCT_FIELD(int, setRepairEngineer)                                                            \
    PROPSTRUCT_FIELD(int, setRepairColor)                                                               \
    PROPSTRUCT_FIELD(int, startVideoRecording)                                                          \
    PROPSTRUCT_FIELD(int, sendSMS)                                                                      \
    PROPSTRUCT_FIELD(int, RFU64)                                                                        \
    PROPSTRUCT_FIELD(int, handleCashlessOrders)                                                         \
    PROPSTRUCT_FIELD(int, createDatabaseBackups)                                                        \
    PROPSTRUCT_FIELD(int, createTasks)                                                                  \
    PROPSTRUCT_FIELD(int, veiwAllTasks)                                                                 \
    PROPSTRUCT_FIELD(int, addGoodsFromWarehouse)                                                        \
    PROPSTRUCT_FIELD(int, enableSparepartRequestManager)                                                \
    PROPSTRUCT_FIELD(int, createBackdatedDocuments)                                                     \
    PROPSTRUCT_FIELD(int, moveRepairToOffice)                                                           \
    PROPSTRUCT_FIELD(int, createQuickRepair)                                                            \
    PROPSTRUCT_FIELD(int, viewForeignOfficeRepair)                                                      \
    PROPSTRUCT_FIELD(int, useCashRegisterEquipment)                                                     \
    PROPSTRUCT_FIELD(int, setRepairManager)                                                             \
    PROPSTRUCT_FIELD(int, viewAnyWorkList)                                                              \
    PROPSTRUCT_FIELD(int, viewRepairLog)                                                                \
    PROPSTRUCT_FIELD(int, editCashRegisterEquipmentSettings)                                            \
    PROPSTRUCT_FIELD(int, editCartridgeCards)                                                           \
    PROPSTRUCT_FIELD(int, sentEmail)                                                                    \
    PROPSTRUCT_FIELD(int, RFU82)    /* разрешения 82..100 зарезервированы для будущих версий АСЦ :-) */ \
    PROPSTRUCT_FIELD(int, RFU83)                                                                        \
    PROPSTRUCT_FIELD(int, RFU84)                                                                        \
    PROPSTRUCT_FIELD(int, RFU85)                                                                        \
    PROPSTRUCT_FIELD(int, RFU86)                                                                        \
    PROPSTRUCT_FIELD(int, RFU87)                                                                        \
    PROPSTRUCT_FIELD(int, RFU88)                                                                        \
    PROPSTRUCT_FIELD(int, RFU89)                                                                        \
    PROPSTRUCT_FIELD(int, RFU90)                                                                        \
    PROPSTRUCT_FIELD(int, RFU91)                                                                        \
    PROPSTRUCT_FIELD(int, RFU92)                                                                        \
    PROPSTRUCT_FIELD(int, RFU93)                                                                        \
    PROPSTRUCT_FIELD(int, RFU94)                                                                        \
    PROPSTRUCT_FIELD(int, RFU95)                                                                        \
    PROPSTRUCT_FIELD(int, RFU96)                                                                        \
    PROPSTRUCT_FIELD(int, RFU97)                                                                        \
    PROPSTRUCT_FIELD(int, RFU98)                                                                        \
    PROPSTRUCT_FIELD(int, RFU99)                                                                        \
    PROPSTRUCT_FIELD(int, RFU100)                                                                       \
    PROPSTRUCT_FIELD(int, editRepairIncomingSet)                                                        \
    PROPSTRUCT_FIELD(int, editAnyComment)                                                               \
    PROPSTRUCT_FIELD(int, advEditWorkList)                                                              \
    PROPSTRUCT_FIELD(int, viewClientPassportData)                                                       \
    PROPSTRUCT_FIELD(int, viewClientBankData)                                                           \
    PROPSTRUCT_FIELD(int, editPaymentSystemInCommittedCashRegisters)                                    \
    PROPSTRUCT_FIELD(int, viewEmployeesSalary)                                                          \
    PROPSTRUCT_FIELD(int, workWithTechReports)                                                          \
    PROPSTRUCT_FIELD(int, viewAllPartsRequests)                                                         \

class SPermissions : public QStandardItemModel//, public SSingleRowModelBase
{
    Q_OBJECT
public:
    enum Fields {dummy = 0, PROPSTRUCT_FIELDS};
    #undef PROPSTRUCT_FIELD
    #define PROPSTRUCT_FIELD(type, name) PROPSTRUCT_DECLARE_MEMBER(type, name)
    PROPSTRUCT_FIELDS
    explicit SPermissions();
    void load(const QString &roles);
    void load();
    void save(const QString &name = QString(), const QString &description = QString());
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setAll(const int state);
    bool isModelChanged();
    int currentRole();
    int permissionId(const char *name);
protected:
    QMap<int, int*> m_indexMap;
    QMap<int, QString> m_nameMap;
    void mapFields(){
        #undef PROPSTRUCT_FIELD
        #define PROPSTRUCT_FIELD(type, name)                                                            \
                m_indexMap.insert(C_##name, &PROPSTRUCT_MEMBER(name));                                  \
                m_nameMap.insert(C_##name, #name);
        PROPSTRUCT_FIELDS
    };
    int rowToPermissionId(const QModelIndex &index) const;
    int permissionIdToRow(const int) const;
    void translateNames();
private:
    bool m_modelEdited = 0;
    int m_currentRole = 0;
};

inline SPermissions::SPermissions()
{
    mapFields();
    setRowCount(m_indexMap.count());
    setColumnCount(2);
}

inline void SPermissions::load(const QString &roles)
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    QStringList rolesList = roles.split(',');
    if(rolesList.count() == 1)
        m_currentRole = rolesList.first().toInt();
    else
        m_currentRole = 0;

    setAll(0);
    query->exec(QUERY_SEL_PERMISSIONS(roles));
    while (query->next())
    {
        int key = query->value(0).toInt();
        if(!m_indexMap.contains(key))
        {
            QString error = tr("Разрешение %1 не объявлено в классе %2!").arg(key).arg(metaObject()->className());
            Global::errorMsg(QString("%1: %2").arg(metaObject()->className(), error));
            continue;
        }
        setData(index(permissionIdToRow(key), 0), 2, Qt::CheckStateRole);
    }
    m_modelEdited = 0;

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

    QString q;
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::session()));

    try
    {
        q = QString("DELETE FROM `permissions_roles` WHERE `role_id` = %1").arg(m_currentRole);
        if(!query->exec(q))
            throw 1;

        q = QString("INSERT INTO `permissions_roles` (`role_id`, `permission_id`) VALUES (:role, :perm)");
        if(!query->prepare(q))
            throw 2;
        query->bindValue(":role", m_currentRole);
        foreach (auto f, m_indexMap)
        {
            if(*f == 0)
                continue;

            query->bindValue(":perm", m_indexMap.key(f));
            if(!query->exec())
                throw 3;
        }

        if(!name.isEmpty() || !description.isEmpty())
        {
            q = QString("UPDATE `roles` SET `name` = '%1', `description` = '%2' WHERE `id` = %3").arg(name, description).arg(m_currentRole);
            if(!query->exec(q))
                throw 4;
        }
    }
    catch(int)
    {
        Global::throwError(query->lastError(), tr("Не удалось сохранить настройки роли"));
    }

    m_modelEdited = 0;
}

inline QVariant SPermissions::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(index.column() == 1)
            return tr(m_nameMap.value(rowToPermissionId(index)).toLocal8Bit());
    }
    else if (role == Qt::CheckStateRole)
        if(index.column() == 0)
            return *m_indexMap.value(rowToPermissionId(index));

    return QVariant();
}

inline bool SPermissions::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole)
    {
        m_modelEdited = 1;
        return (*(m_indexMap.value(rowToPermissionId(index))) = value.toInt());
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
    foreach (auto f, m_indexMap)
    {
        *f = state;
    }
}

inline bool SPermissions::isModelChanged()
{
    return m_modelEdited;
}

inline int SPermissions::currentRole()
{
    return m_currentRole;
}

inline int SPermissions::permissionId(const char *name)
{
    int index = m_nameMap.key(name, -1);
    Q_ASSERT_X(index != -1, "SPermissions::permissionId(const char *name)", QString("unknown permission %1").arg(name).toLocal8Bit());

    return index;
}

inline int SPermissions::rowToPermissionId(const QModelIndex &index) const
{
    return index.row() + 1;
}

inline int SPermissions::permissionIdToRow(const int id) const
{
    return id - 1;
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
    tr("viewAllPartsRequests");
}

#endif // SPERMISSIONS_H
