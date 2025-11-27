#include <SApplication>
#include "susersettings.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <tabPrintDialog>
#include <SLocalSettingsStructs>
#include <QListView>
#include <SSqlQueryModel>
#include <QPrinterInfo>
#include <SStandardItemModel>

static QString errMsg(QObject::tr("Не удалось сохранить персональные настройки пользователя"));

SUserSettings::SUserSettings() :
    SSettingsBase()
{
    m_record_settings = std::make_shared<QSqlRecord>();
    m_classDbMap = std::make_unique<QMap<int, int>>();  // создание объекта до вызова mapFields()
    mapFields();
    registerReportFields();
}

SUserSettings::~SUserSettings()
{
    if(xlsColumnsList)
        delete xlsColumnsList;
}

void SUserSettings::insertNewField(const int index, const QString &name)
{
    QSqlField f;
    if(name.startsWith("usersParams."))
    {
        m_classDbMap->insert(index, (Table::UsersParams | m_record_settings->count()));
        f.setName(name.split('.').at(1));
        m_record_settings->append(f);
    }
    else
    {
        m_classDbMap->insert(index, m_record->count());
        f.setName(name);
        m_record->append(f);
    }
}

void SUserSettings::configureWidgets()
{
    configureWidgetsForLocalSettings();

    QMetaEnum types = SUserSettings::staticMetaObject.enumerator(SUserSettings::staticMetaObject.indexOfEnumerator("XlsColumns"));
    xlsColumnsList = SStandardItemModel::modelFromEnum(types, tr);
    xlsColumnsList->setObjectName("xlsColumns");
    xlsColumnsList->setHorizontalHeaderLabels({"name", "id"});
    const QList<int> xlsIndexes = i_propertyGroup.keys(16);
    foreach (auto i, xlsIndexes)
    {
        static_cast<QComboBox*>(i_editorWidgets.value(i))->setModel(xlsColumnsList);
    }

    QStringList wsModes = {"", tr("Картриджи"), tr("Ремонты")};
    static_cast<QComboBox*>(i_editorWidgets.value(C_defWsFilter))->addItems(wsModes);
    qobject_cast<QListView *>(static_cast<QComboBox*>(i_editorWidgets.value(C_defWsFilter))->view())->setRowHidden(0, true);

    setComboBoxModel(C_defaultPaymentSystem, paymentSystemsModel);

/**************************************************************************************/
/******************* Виджеты, работа с которыми еще не реализована ********************/
/******************** см. метод SUserSettings::prepareUpdateList() ********************/
/**************************************************************************************/
    disableWidget(C_rowColor);
    disableWidget(C_fontFamily);
    disableWidget(C_geHighlightColor);
    disableWidget(C_issuedColor);
    disableWidget(C_defStore);
    disableWidget(C_roles);
    disableWidget(C_office);
    disableWidget(C_kkt);
    disableWidget(C_pinpad);
    disableWidget(C_defOffice);
    disableWidget(C_defEmployee);
    disableWidget(C_defStatus);
    disableWidget(C_phoneMask);
    disableWidget(C_phone2Mask);
}

/* Конфигурация виджетов, значения которых хранятся в localSettings:
 *  - принтеры по умолчанию
 *  - шрифт
 *  - высота строк в таблицах
*/
void SUserSettings::configureWidgetsForLocalSettings()
{
    m_printersList = QPrinterInfo::availablePrinterNames();
    static_cast<QComboBox*>(i_editorWidgets.value(C_defaultDocumentPrinter))->addItems(m_printersList);
    static_cast<QComboBox*>(i_editorWidgets.value(C_defaultStickerPrinter))->addItems(m_printersList);
    static_cast<QComboBox*>(i_editorWidgets.value(C_defaultPosPrinter))->addItems(m_printersList);
    setCacheData(C_defaultDocumentPrinter, tabPrintDialog::findPrinterIndex(m_printersList, userLocalData->DocsPrinter.value), ModifiedField::Executed);
    setCacheData(C_defaultStickerPrinter, tabPrintDialog::findPrinterIndex(m_printersList, userLocalData->StickersPrinter.value), ModifiedField::Executed);
    setCacheData(C_defaultPosPrinter, tabPrintDialog::findPrinterIndex(m_printersList, userLocalData->PosPrinter.value), ModifiedField::Executed);

    setCacheData(C_fontFamily, userLocalData->FontFamily.value, ModifiedField::Executed);
}

void SUserSettings::load(const QString &connectionLogin)
{
    i_primaryKeyIndex = C_username;
    setPrimaryKey(connectionLogin);

    this->load();
}

void SUserSettings::load()
{
    SSettingsBase::load();    // загрузка из таблицы `users`, очистка кэша
    // инициализация поля company происходит в windowsDispatcher::createMainWindow()
    loadPermissions();
    loadFromUsersParams();

    deserializeData();
}

void SUserSettings::loadPermissions()
{
    auto q = QString("SELECT GROUP_CONCAT(`role_id`) AS 'roles' FROM `roles_users` WHERE `user_id` = %1 GROUP BY `user_id`  LIMIT 1;").arg(id());
    i_query->exec(q);
    if(i_query->first())
        setCacheData(C_roles, i_query->value(0).toString(), ModifiedField::Executed);
}

void SUserSettings::loadFromUsersParams()
{
    int recordIndex = -1;
    int classIndex = 0;

    m_params.clear();
    i_query->exec(QString("SELECT t1.`name`, IFNULL(t2.`value`, t1.`default_value`) AS 'value', t1.`id` FROM `user_params` AS t1 LEFT JOIN (SELECT `param_id`, `value` FROM users_params WHERE `user_id` = %1)  t2 ON t1.`id` = t2.`param_id`;").arg(id()));
    while (i_query->next())
    {
        recordIndex = m_record_settings->indexOf(i_query->value(0).toString());
        if(recordIndex < 0)
            continue;

        m_record_settings->setValue(recordIndex, i_query->value(1));

        classIndex = m_classDbMap->key(Table::UsersParams | recordIndex);
        m_params.insert(classIndex, i_query->value(2).toInt());   // сохранение id параметра, он будет использоваться запросе INSERT (UPDATE)
    }
}

QString SUserSettings::table()
{
    return "users";
}

/* Инициализация отдельных полей класса при загрузке:
 *  - обработка полей с JSON;
 *  - обработка полей, связанных с виджетами RadioButton
*/
void SUserSettings::deserializeData()
{
    updateWorkspaceSwitch();
}

/* Обработка полей таблиц, представленных в виде JSON и копирование значений в отдельные поля класса
 * (такие поля хранятся только в кэше)
*/
void SUserSettings::loadFromJson(const int, bool)
{

}

void SUserSettings::updateWorkspaceSwitch()
{
    int val = workspaceMode();
    bool workspaceRepairs = 1, workspaceItems = 0;
    switch (val)
    {
        case 0: break;
        case 1: workspaceRepairs = 0; workspaceItems = 1; break;
        default: break;
    }
    setCacheData(C_workspaceRepairs, (Qt::CheckState)(workspaceRepairs), ModifiedField::Executed);
    setCacheData(C_workspaceItems, (Qt::CheckState)(workspaceItems), ModifiedField::Executed);
}

void SUserSettings::updateWorkspaceField()
{
    int v = 1*workspaceItems();
    setCacheData(C_workspaceMode, v, ModifiedField::Updated);
}

QVariant SUserSettings::loadedValue(const int dbTableIndex) const
{
    if(dbTableIndex & Table::UsersParams)
        return m_record_settings->value(dbTableIndex & (Table::UsersParams - 1));
    else
        return rec()->value(dbTableIndex);
}

void SUserSettings::updateJson(const int, const QVariant&)
{

}

int SUserSettings::dbFieldIndex(const int classIndex) const
{
    return m_classDbMap->value(classIndex, -1);
}

QString SUserSettings::dbFieldName(const int classIndex) const
{
    int dbi = dbFieldIndex(classIndex);
    if(dbi&Table::UsersParams)
        return m_record_settings->fieldName(dbi&(Table::UsersParams-1));

    return rec()->fieldName(dbi);
}

int SUserSettings::targetTable(const int targetTableIndex)
{
    if(targetTableIndex&Table::UsersParams)
        return 1;

    return 0;
}

void SUserSettings::dataChangeHandler(const int index, const QVariant &data)
{
    loadFromJson(index);
    updateJson(index, data);
    switch (index)
    {
        case C_rowHeight: {
            emit rowHeightChanged();
            break;
        }
        case C_fontSize:
        case C_fontFamily: {
            QFont f;
            f.setFamily(fontFamily());
            f.setPixelSize(fontSize());
            QApplication::setFont(f);
            emit fontSizeChanged();
            break;
        }
        case C_workspaceMode: updateWorkspaceSwitch(); break;
        case C_workspaceRepairs: break; // этот RadioButton пропускаем
        case C_workspaceItems: updateWorkspaceField(); break;   // в поле workspaceMode записываем только когда обрабатаем все
        default: break;
    }
}

void SUserSettings::save()
{
    SSettingsBase::save();
    saveToUsersParams();
    savePrinterSettings();

//    userLocalData->FontFamily.value = fontFamily; // TODO: модель данных шрифтов
}

void SUserSettings::saveToUsersParams()
{
    if(!isDirty(1))
        return;

    bool nErr = 1;

    i_query->prepare(QString("INSERT INTO `users_params` (`user_id`, `param_id`, `value`) VALUES (:user_id, :param_id, :value) ON DUPLICATE KEY UPDATE `value` = VALUES(`value`);"));
    i_query->bindValue(":user_id", id());

    CacheMap::ConstIterator i = cache.constBegin();
    CacheMap::ConstIterator e = cache.constEnd();
    for (; i != e; ++i)
    {
        if(!((*i)->state() & (ModifiedField::Updated | ModifiedField::Failed)))
            continue;
        if(dbFieldIndex(i.key()) >= 0)
        {
            i_query->bindValue(":param_id", m_params.value(i.key()));
            i_query->bindValue(":value", fieldValueHandler((*i)->data()));
            nErr = i_query->exec();
        }
        if(!nErr)
            Global::throwError(i_query->lastError(), errMsg);

        setState(*i, ModifiedField::Executed);
    }
}

void SUserSettings::savePrinterSettings()
{
    userLocalData->DocsPrinter.value = m_printersList.value(defaultDocumentPrinter(), "");
    userLocalData->StickersPrinter.value = m_printersList.value(defaultStickerPrinter(), "");
    userLocalData->PosPrinter.value = m_printersList.value(defaultPosPrinter(), "");
}

void SUserSettings::updateLoginTimestamp()
{
    bool nErr = 1;
    QSqlQuery query(QSqlDatabase::database("connThird"));

    try
    {
        QUERY_EXEC_TH(&query,nErr,QUERY_BEGIN);
        set_lastLogin(QDateTime::currentDateTime());
        SSettingsBase::save();
        QUERY_COMMIT_ROLLBACK(&query, nErr);
    }
    catch(Global::ThrowType type)
    {
        if (type != Global::ThrowType::ConnLost)
        {
            QUERY_COMMIT_ROLLBACK(&query, nErr);
        }
    }
}

void SUserSettings::updateActivityTimestamp()
{
    set_lastActivity(QDateTime::currentDateTime());
    SSettingsBase::save();
}

void SUserSettings::setComboBoxModel(const int index, SSqlQueryModel *model)
{
    if(i_editorWidgetsTypes.value(index) != WidgetTypes::ComboBox)
        return;

    QComboBox *cb;

    cb = static_cast<QComboBox*>(i_editorWidgets.value(index));
    Q_ASSERT_X(cb, metaObject()->className(), QString("comboBox for field %1 wasn't found").arg(index).toLocal8Bit());

    cb->setModel(model);
}

void SUserSettings::translate()
{
    tr("username");
    tr("name");
    tr("surname");
    tr("patronymic");
    tr("roles");
    tr("office");
    tr("address");
    tr("password");
    tr("notes");
    tr("signature");
    tr("phone");
    tr("phoneMask");
    tr("phone2");
    tr("phone2Mask");
    tr("sipUserId");
    tr("birthday");
    tr("inn");
    tr("kkt");
    tr("pinpad");
    tr("cardOnCall");
    tr("trackActivity");
    tr("state");
    tr("isBot");
    tr("photo");
    tr("clientUserId");
    tr("passportNum");
    tr("passportDate");
    tr("passportOrgan");
    tr("salaryRate");
    tr("payDayOff");
    tr("payDay");
    tr("payDeviceIn");
    tr("payDeviceOut");
    tr("payRepair");
    tr("payRepairQuick");
    tr("payCartridgeRefill");
    tr("paySale");
    tr("payRepairQSale");
    tr("pay4SaleInRepair");
    tr("created");
    tr("lastLogin");
    tr("lastActivity");
    tr("currentOffice");
    tr("company");
    tr("rowColor");
    tr("colorLabelWs");
    tr("fontSize");
    tr("rowHeight");
    tr("geHighlightColor");
    tr("defaultDocumentPrinter");
    tr("defaultStickerPrinter");
    tr("previewBeforePrint");
    tr("defaultPosPrinter");
    tr("displayOut");
    tr("displayComplete");
    tr("newOnTop");
    tr("saveStateOnClose");
    tr("autoRefreshWorkspace");
    tr("refreshTime");
    tr("issuedColor");
    tr("defOffice");
    tr("defEmployee");
    tr("defStatus");
    tr("defWsFilter");
    tr("autosavePartList");
    tr("autosaveDiagResult");
    tr("preferRegular");
    tr("groupStoreItems");
    tr("defStore");
    tr("xlsC1");
    tr("xlsC2");
    tr("xlsC3");
    tr("xlsC4");
    tr("xlsC14");
    tr("xlsC6");
    tr("xlsC5");
    tr("xlsC7");
    tr("xlsC8");
    tr("xlsC9");
    tr("xlsC10");
    tr("xlsC11");
    tr("xlsC12");
    tr("xlsC15");
    tr("xlsC13");
    tr("workspaceRepairs");
    tr("workspaceItems");
    tr("workspaceMode");
    tr("defaultPaymentSystem");
    tr("alternateRowsBackground");
    tr("useRegExpSearch");

    // Перевод значений в выпадающем списке
    tr("ColumnA"); tr("ColumnB"); tr("ColumnC"); tr("ColumnD"); tr("ColumnE");
    tr("ColumnF"); tr("ColumnG"); tr("ColumnH"); tr("ColumnI"); tr("ColumnJ");
    tr("ColumnK"); tr("ColumnL"); tr("ColumnM"); tr("ColumnN"); tr("ColumnO");
    tr("NoColumn");
}

