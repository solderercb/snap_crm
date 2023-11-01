#include "susersettings.h"
#include "global.h"
#include "tabprintdialog.h"

SUserSettings::SUserSettings()
{
}

SUserSettings::~SUserSettings()
{
    if(xlsColumnsList)
        delete xlsColumnsList;
}

void SUserSettings::initWidgets()
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        metaObject()->invokeMethod(this, QByteArray("init__widget").insert(5, metaObject()->property(i).name()), Qt::DirectConnection);

    m_printersList = QPrinterInfo::availablePrinterNames();
    static_cast<QComboBox*>(i_editorWidgets.value("defaultDocumentPrinter"))->addItems(m_printersList);
    static_cast<QComboBox*>(i_editorWidgets.value("defaultStickerPrinter"))->addItems(m_printersList);
    static_cast<QComboBox*>(i_editorWidgets.value("defaultPosPrinter"))->addItems(m_printersList);

    QMetaEnum types = SUserSettings::staticMetaObject.enumerator(SUserSettings::staticMetaObject.indexOfEnumerator("XlsColumns"));
    xlsColumnsList = SStandardItemModel::modelFromEnum(types, tr);
    xlsColumnsList->setObjectName("xlsColumns");
    xlsColumnsList->setHorizontalHeaderLabels({"name", "id"});
    for(int i = 1; i < 16; i++)
        static_cast<QComboBox*>(i_editorWidgets.value("xlsC" + QString::number(i)))->setModel(xlsColumnsList);


    // Модели данных виджетов (ComboBox) должны быть заданы до загрузки данных, иначе будет падать.
//   static_cast<QComboBox*>(i_editorWidgets.value("..."))->setModel(...);

    QStringList wsModes = {"", tr("Картриджи"), tr("Ремонты")};
    static_cast<QComboBox*>(i_editorWidgets.value("defWsFilter"))->addItems(wsModes);
    qobject_cast<QListView *>(static_cast<QComboBox*>(i_editorWidgets.value("defWsFilter"))->view())->setRowHidden(0, true);

    setComboBoxModel("defaultPaymentSystem", paymentSystemsModel);

    load(username);

/**************************************************************************************/
/******************* Виджеты, работа с которыми еще не реализована ********************/
/******************** см. метод SUserSettings::prepareUpdateList() ********************/
/**************************************************************************************/
    disableWidget("rowColor");
    disableWidget("colorLabelWs");
    disableWidget("fontFamily");
    disableWidget("geHighlightColor");
    disableWidget("issuedColor");
    disableWidget("defStore");
    disableWidget("workspaceRepairs");
    disableWidget("workspaceItems");
    disableWidget("office");
    disableWidget("kkt");
    disableWidget("pinpad");
    disableWidget("defOffice");
    disableWidget("defEmployee");
    disableWidget("defStatus");
    disableWidget("phoneMask");
    disableWidget("phone2Mask");
}

void SUserSettings::disableWidget(const QString propertyName)
{
    i_editorWidgets.value(propertyName)->setEnabled(false);
}

void SUserSettings::load(const QString &connectionLogin)
{
    QString q;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    int i = 0;

    for(i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        metaObject()->invokeMethod(this, QByteArray("register__db_field").insert(9, metaObject()->property(i).name()), Qt::DirectConnection);

    // "переворот" таблицы users
    q = QString("SELECT @smth := CONCAT(IF(@smth IS NOT NULL, CONCAT(@smth, '\\\nUNION ALL\\\n'), ''), 'SELECT \\\'', `COLUMN_NAME`, '\\\', `', `COLUMN_NAME`, '` FROM `users` WHERE `username` = \\\'%1\\\'') FROM information_schema.`COLUMNS` WHERE `TABLE_SCHEMA` = SCHEMA() AND `TABLE_NAME` = 'users';").arg(connectionLogin);
    query->exec("SET @smth := NULL");
    query->exec(q);

    query->last();
    q = query->value(0).toString();
    query->exec(q);
    while (query->next())
    {
        i = fieldToPropertyId(query->value(0).toString());
        if(i < 0)
            continue;

        metaObject()->property(i).write(this, query->value(1));
    }

    q = QString("SELECT GROUP_CONCAT(`role_id`) AS 'roles' FROM `roles_users` WHERE `user_id` = %1 GROUP BY `user_id`  LIMIT 1;").arg(id);
    query->exec(q);
    if(query->first())
        roles = query->value(0).toString();

    // TODO: несколько компаний
    company = 1;

    delete query;

    loadFromUsersParams();

    loadPrinterSettings();
}

void SUserSettings::loadFromUsersParams()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
    int i = 0;

    m_params.clear();
    query->exec(QString("SELECT t1.`name`, IFNULL(t2.`value`, t1.`default_value`) AS 'value', t1.`id` FROM `user_params` AS t1 LEFT JOIN (SELECT `param_id`, `value` FROM users_params WHERE `user_id` = %1)  t2 ON t1.`id` = t2.`param_id`;").arg(id));
    while (query->next())
    {
        i = fieldToPropertyId("usersParams." + query->value(0).toString());
        if(i < 0)
            continue;

        m_params.insert(query->value(0).toString(), query->value(2).toInt());
        metaObject()->property(i).write(this, query->value(1));
    }

    delete query;
}

void SUserSettings::loadPrinterSettings()
{
    int i;

    i = metaObject()->indexOfProperty("defaultDocumentPrinter");
    metaObject()->property(i).write(this, tabPrintDialog::findPrinterIndex(m_printersList, userLocalData->DocsPrinter.value));
    i = metaObject()->indexOfProperty("defaultStickerPrinter");
    metaObject()->property(i).write(this, tabPrintDialog::findPrinterIndex(m_printersList, userLocalData->StickersPrinter.value));
    i = metaObject()->indexOfProperty("defaultPosPrinter");
    metaObject()->property(i).write(this, tabPrintDialog::findPrinterIndex(m_printersList, userLocalData->PosPrinter.value));

}

bool SUserSettings::commit()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QString q;

    fieldsUpdFormatter();
    if(!i_valuesMap.isEmpty())
    {
        q = QString("UPDATE\n  `users`\nSET\n  %2\nWHERE `id` = %1;").arg(id).arg(fields.join(",\n  "));
        QUERY_EXEC(query, nErr)(q);
    }
    delete query;

    if(nErr)
        i_valuesMap.clear();

    return nErr;
}

// распределение изменённых значений по группам по названию таблиц
void SUserSettings::prepareUpdateList(Table table)
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
    {
        QString propName = metaObject()->property(i).name();
        QString field = i_fieldNames.value(propName);
        if(field.startsWith("usersParams") != table)
            continue;

        if(table)   // Table::UsersParams
            field = field.replace("usersParams.", "");

        QVariant val = i_fieldModified.value(propName);
        if(!val.isValid())
            val = i_jsonFieldModified.value(propName);
        if(!val.isValid())
            continue;

/******* TEMPORARY *******/
        // Данные отключенных виджетов не записывать в БД
        QWidget *w = i_editorWidgets.value(propName);
        if(w && !w->isEnabled())
            continue;
/**************/

        i_valuesMap.insert(field, val);
    }
}

void SUserSettings::save()
{
    QString oldFontFamily;

    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
    {
        metaObject()->property(i).read(this);
    }

    savePrinterSettings();
    oldFontFamily = userLocalData->FontFamily.value;
//    userLocalData->FontFamily.value = fontFamily; // TODO: модель данных шрифтов

    i_valuesMap.clear();
    prepareUpdateList(Table::Users);

    if(i_valuesMap.contains("fontsize") || oldFontFamily.compare(fontFamily) != 0)
    {
        QFont f;
        f.setFamily(userLocalData->FontFamily.value);
        f.setPixelSize(userDbData->fontSize);
        QApplication::setFont(f);
        emit fontSizeChanged();
    }

    if(i_valuesMap.contains("rowheight"))
    {
        emit rowHeightChanged();
    }

    if(!commit())
        throw Global::ThrowType::QueryError;
    saveToUsersParams();

    i_fieldModified.clear();
}

void SUserSettings::saveToUsersParams()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    i_valuesMap.clear();
    prepareUpdateList(Table::UsersParams);

    query->prepare(QString("INSERT INTO `users_params` (`user_id`, `param_id`, `value`) VALUES (:user_id, :param_id, :value) ON DUPLICATE KEY UPDATE `value` = VALUES(`value`);"));
    query->bindValue(":user_id", id);
    QMap<QString, QVariant>::const_iterator i = i_valuesMap.constBegin();
    while(i != i_valuesMap.constEnd())
    {
        query->bindValue(":param_id", m_params.value(i.key()));
        query->bindValue(":value", i.value());
        nErr = query->exec();
        if(!nErr)
        {
            errorToLog(metaObject()->className(), query->lastError().text());
            throw Global::ThrowType::QueryError;
        }
        i++;
    }

    delete query;
}

void SUserSettings::savePrinterSettings()
{
    userLocalData->DocsPrinter.value = m_printersList.value(defaultDocumentPrinter, "");
    userLocalData->StickersPrinter.value = m_printersList.value(defaultStickerPrinter, "");
    userLocalData->PosPrinter.value = m_printersList.value(defaultPosPrinter, "");
}

void SUserSettings::updateLoginTimestamp()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    query->exec(QUERY_BEGIN);
    i_valuesMap.insert("last_login", QDateTime::currentDateTime());
    nErr = commit();
    QUERY_COMMIT_ROLLBACK(query, nErr);

    delete query;
}

void SUserSettings::updateActivityTimestamp(const bool standalone)
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    if(standalone)
    {
        query->exec(QUERY_BEGIN);
    }
    i_valuesMap.insert("last_activity", QDateTime::currentDateTime());
    nErr = commit();
    if(standalone)
    {
        QUERY_COMMIT_ROLLBACK(query, nErr);
    }

    delete query;
}

void SUserSettings::setComboBoxModel(const QString propertyName, SSqlQueryModel *model)
{
    QComboBox *cb;

    cb = static_cast<QComboBox*>(i_editorWidgets.value(propertyName));
    Q_ASSERT_X(cb, metaObject()->className(), QString("comboBox %1 wasn't found").arg(propertyName).toLocal8Bit());

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

    // Перевод значений в выпадающем списке
    tr("ColumnA"); tr("ColumnB"); tr("ColumnC"); tr("ColumnD"); tr("ColumnE");
    tr("ColumnF"); tr("ColumnG"); tr("ColumnH"); tr("ColumnI"); tr("ColumnJ");
    tr("ColumnK"); tr("ColumnL"); tr("ColumnM"); tr("ColumnN"); tr("ColumnO");
    tr("NoColumn");
}
