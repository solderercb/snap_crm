#include "global.h"
#include "appver.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "com_sql_queries.h"

LoginWindow::LoginWindow(QObject*) :
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
    logo = new QGraphicsPixmapItem(QPixmap("logo.png"));
    logoScene = new QGraphicsScene();
    logoScene->addItem(logo);
    ui->graphicsLogo->setScene(logoScene);
    ui->labelAppVer->setText(QString(APP_VER_STR));

    debugInitLoginOptions();
    userLocalData = new t_userSettings;

//  shortlivedNotification::setSize(385, 90);
    shortlivedNotification::setAppearance(this, shortlivedNotification::bottomRight);
    statusBarDelay = new QTimer();
    statusBarDelay->setSingleShot(true);

    connect(statusBarDelay, SIGNAL(timeout()), this, SLOT(clearStatusLabel()));
    connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(btnLoginHandler()));
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(btnCancelHandler()));
    connect(ui->pushButtonSettingsImport, &QPushButton::clicked, this, &LoginWindow::selectAscExe);

    localSettings->read(userLocalData);
    if(userLocalData->FontFamily.value.isEmpty())
        userLocalData->FontFamily.value = "Segoe UI";
    fillConnectionParams();
}

LoginWindow::~LoginWindow()
{
    if(updateController)
        delete updateController;
    delete logo;
    delete ui;
}

void LoginWindow::fillConnectionParams()
{
    ui->editIPaddr->setText(userLocalData->dbHost.value);
    ui->editPort->setText(userLocalData->dbPort.value);
    ui->editDBName->setText(userLocalData->dbName.value);
    ui->editLogin->setText(userLocalData->lastLogin.value);
    ui->editPassword->setFocus();
    ui->checkBoxSSL->setChecked(userLocalData->SSLConnection.value);
}

void LoginWindow::startMaintanaceTool()
{
    QString targerUpdateVer;
    QString updateChannel ;
    QSqlQuery query = QSqlQuery(QSqlDatabase::database("connMain"));

    query.exec(QUERY_SEL_UPDATE_CHANNEL);
    query.first();
    updateChannel = query.value(0).toString();
    if(!updateChannel.contains("github.com/solderercb/snap_crm"))
        return;

    // Если канал обновления официальный, то скачивачиваем конкретную версию, а не самую последнюю
    // По задумке такой механизм позволит избежать лишней головной боли админа организации
    query.exec(QUERY_SEL_APP_VER);
    query.first();
    targerUpdateVer= query.value(0).toString();

    updateChannel.replace("releases/latest/download", QString("releases/download/%1").arg(targerUpdateVer));

    QVariantMap config;
    config.insert(QLatin1String("path"), QLatin1String("maintenancetool.exe"));
    config.insert(QLatin1String("repoPathArg"), updateChannel);
    auto updater = QtAutoUpdater::Updater::create(QLatin1String("qtifw"), config, qApp);
//    Q_ASSERT(updater);
    updateController = new QtAutoUpdater::UpdateController(updater, this);
    updateController->start(QtAutoUpdater::UpdateController::DisplayLevel::Ask);
}

void LoginWindow::statusBarMsg(const QString &text, int delay)
{
    ui->labelStatus->setText(text);
    ui->labelStatus->setStyleSheet("color: red; font: bold;");
    if(delay)
        statusBarDelay->start(delay);
}

/*  Сравнение версии запущенного приложения с "рабочей" версией приложения, записанной в БД в `config`.`version_snap`.
 *  Возвращает 1, если требуется обновление записи в БД.
 *  Если запущена устаревшая версия, генерируется исключение.
 */
bool LoginWindow::checkAppVer()
{
    QStringList appVer;
    QSqlQuery queryCheckAppVer = QSqlQuery(QSqlDatabase::database("connMain"));
    queryCheckAppVer.exec(QUERY_SEL_APP_VER);
    queryCheckAppVer.first();
    appVer = queryCheckAppVer.value(0).toString().split('.');

    if( appVer.at(3).toInt() > APP_COMMIT )
        throw 5;
    else if(appVer.at(3).toInt() < APP_COMMIT)
        return 1;
    return 0;
}

/*  Проверка соответствия версии приложения версии структуры БД
 *  Возвращает номер скрипта, с которого нужно начать обновление
 */
int LoginWindow::checkSchema()
{
    QString fileName;
    QSqlQuery queryCheckSchema = QSqlQuery(QSqlDatabase::database("connMain"));
    int i = 1;

    queryCheckSchema.exec(QUERY_SEL_ASC_SCHEMA_VER);
    queryCheckSchema.first();
    if(queryCheckSchema.value(0).toString() != "ASC.Scripts.Script000326.sql")  // версия БД АСЦ должна соответствовать версии приложения 3.7.37.1184
        throw 3;    // Попытка подключения к старой версии БД ...
    queryCheckSchema.clear();

    // подсчет кол-ва патчей в файле ресурсов
    while(QFile::exists(fileName = ":/schema-updates/script" + QString::number(i).rightJustified(6, '0') + ".sql"))
    {
        i++;
    };
    i -= 1;

    // определение кол-ва применённых патчей
    queryCheckSchema.exec(QUERY_SEL_SCHEMA_VER);
    if(queryCheckSchema.size() > i)
        throw 5;    // сообщение об устаревшей версии программы/запуск обновления ПО
    else if (queryCheckSchema.size() < i)
        return queryCheckSchema.size() + 1; // возвращаем номер патча, с которого нужно начать обновление структуры БД

    return 0;
}

/*  Проверка активных подключений к базе
 *  Этот метод нужен "на всякий случай"; базу нужно обновлять в нерабочее время :-)
 *  Возвращает список имён пользователей или пустой список.
 */
QStringList LoginWindow::usersOnline()
{
    QSqlQuery queryCheckUsersOnline = QSqlQuery(QSqlDatabase::database("connMain"));

    // запрос активных соединений
    queryCheckUsersOnline.exec(QUERY_SEL_ACTIVE_USERS(QSqlDatabase::database("connMain").databaseName()));
    queryCheckUsersOnline.first();
    if(queryCheckUsersOnline.size())
        return queryCheckUsersOnline.value(0).toString().split(',');

    // пользователи, значение  `users`.`last_activity` которых вписывается в период timeout до текущего момента (ХЗ зачем, потом придумаю)
//    queryCheckUsersOnline.exec(QUERY_SEL_GLOB_WAIT_TIMEOUT);
//    queryCheckUsersOnline.first();
//    timeout = queryCheckUsersOnline.value(0).toInt();
//    queryCheckUsersOnline.exec(QString(QUERY_SEL_ACTIVE_USERS2(timeout));
//    if(queryCheckUsersOnline.size())
//        return queryCheckUsersOnline.value(0).toString().split(',');

    return QStringList();
}

/*  Обновление БД
 *  startFrom > 0 — обновление структуры, значение — это номер скрипта, с которого нужно начать
 *  startFrom == 0 — обновление значения `config`.`version_snap`
 */
bool LoginWindow::updateDB(int startFrom)
{
    QString fileName;
    QFile file;
    QByteArray fileContent;
    int i = startFrom;
    QSqlQuery queryUpdate = QSqlQuery(QSqlDatabase::database("connThird"));

    if (QSqlDatabase::database("connMain").userName() == "admin")  // только админ может обновлять БД
    {
        qDebug() << "startFrom =" << startFrom;
        if(usersOnline().isEmpty())
        {
            queryUpdate.exec(QUERY_BEGIN);
            try
            {
                if(startFrom) // обновление структуры БД
                {
                    // TODO: переключение БД в режим обслуживания (на всякий случай, чтобы пользователи не смогли подключиться во время обновления)
                    while(QFile::exists(fileName = ":/schema-updates/script" + QString::number(i).rightJustified(6, '0') + ".sql"))
                    {
                        qDebug() << "fileName = " << fileName;
                        file.setFileName(fileName);
                        file.open(QIODevice::ReadOnly);
                        fileContent = file.readAll();
                        file.close();
                        // TODO: предусмотреть наличие директивы DELIMITER в скриптах
                        qDebug() << "queries:" << fileContent;
                        if(!queryUpdate.exec(QString::fromLocal8Bit(fileContent)))
                            throw 1;
                        if(!queryUpdate.exec(QUERY_INS_SCHEMAVERSIONS("SNAP.schema-updates.script" + QString::number(i).rightJustified(6, '0') + ".sql")))
                            throw 1;
                        i++;
                    }
                    // TODO: переключение БД в обычный режим
                }
                else    // обновление значения `config`.`version_snap`
                {
                    if(!queryUpdate.exec(QUERY_UPD_APP_VER(APP_VER_STR)))
                        throw 1;
                }
                queryUpdate.exec(QUERY_COMMIT);
                // TODO: отправка служебного уведомления, что база обновлена (например, для случаев, когда компьютер находился в спящем режиме)
            }
            catch (int)
            {
                queryUpdate.exec(QUERY_ROLLBACK);
                // TODO: запись в журнал приложения причины ошибки обновления
                return 1;
            }
        }
        else
        {
            qDebug() << "usersOnline(): " << usersOnline();
            // TODO: отправка служебного сообщения и инициация закрытия приложения у пользователей
            throw 2;
        }
    }
    else
    {
        throw 3;
    }

    return 0;
}

void LoginWindow::closeConnections()
{
    for (int i = 0; i<connections.size(); i++)  // закрываем все соединения
    {
        connections[i]->close();
                while(connections[i]->isOpen());
        QSqlDatabase::removeDatabase(connections[i]->connectionName());
    }
}

void LoginWindow::debugInitLoginOptions()
{
    QVariant tmp;
    if(debugOptions == nullptr)
        return;

    if (debugOptions->childGroups().contains("debugLogin"))
    {
        debugLoginOptions = new QMap<QString, QVariant>;
        QStringList loginCreds;
        debugOptions->beginGroup("debugLogin");

        QStringList allKeys = debugOptions->allKeys();
        for(int i = 0; i < allKeys.size(); i++)
        {
            tmp = debugOptions->value(allKeys.at(i));
            if(tmp.isValid())
            {
                debugLoginOptions->insert(allKeys.at(i), tmp);
            }

        }
        debugOptions->endGroup();

        if(!loginCreds.isEmpty())
        {
            qDebug().noquote() << "Connection creds got from " << debugOptions->property("fileName").toString() << ": " << loginCreds.join(", ");
            appLog->appendRecord(QString("Connection creds got from %1: %2").arg(debugOptions->property("fileName").toString()).arg(loginCreds.join(", ")));
        }
    }
}

void LoginWindow::editPassword_onReturnPressed()
{
    if(ui->editPassword->text() != "")
        btnLoginHandler();
}

void LoginWindow::debugLogin()
{
    btnLoginHandler();
}

void LoginWindow::btnLoginHandler()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);
    QStringList connOptions;
    QResource res;

    connections.clear();
    connMain = QSqlDatabase::addDatabase("QMYSQL", "connMain");       // это соединение для получения данных (ремонты, клиенты и т. д.)
    connections.append(&connMain);
    connNtfy = QSqlDatabase::addDatabase("QMYSQL", "connNtfy");
    connections.append(&connNtfy);
    connThird = QSqlDatabase::addDatabase("QMYSQL", "connThird");     // это соединение для записи данных в БД и вспом. операций
    connections.append(&connThird);
//    connFourth = QSqlDatabase::addDatabase("QMYSQL", "connFourth"); // в АСЦ сразу четыре соединения, зачем — ХЗ; понадобятся, тогда включим.
//    connections.append(&connFourth);


    // паролем пользователя БД — это MD5-хэш пароля пользователя АСЦ; это сделано для того, чтобы пользователь не мог получить доступ непосредственно к базе
    // TODO: Придумать механизм защиты (например, нестандартный инициализирующий вектор для алгоритма MD5 или другой алгоритм) с которым будет скомпилировано
    // приложение, а любознательный пользователь, обнаруживший исходные коды проекта в Интернет, опять же не мог получить доступ непосредственно к базе
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(ui->editPassword->text().toUtf8(), ui->editPassword->text().length());
//    qDebug() << hash.result().toHex();

    connOptions << ("MYSQL_OPT_RECONNECT=1");
    if(ui->checkBoxSSL->isChecked())
    {
        connOptions << QString("SSL_KEY=%1").arg(userLocalData->SSLKey.value);
        connOptions << QString("SSL_CERT=%1").arg(userLocalData->SSLCert.value);
        connOptions << QString("SSL_CA=%1").arg(userLocalData->SSLCA.value);
        connOptions << QString("SSL_CAPATH=%1").arg(userLocalData->SSLCAPath.value);
        connOptions << QString("SSL_CIPHER=%1").arg(userLocalData->SSLCipher.value);
    }
    for (int i=0; i<connections.size(); i++)
    {
        if(debugLoginOptions)
        {
            connections[i]->setUserName(debugLoginOptions->value("user", "user").toString());
            connections[i]->setPassword(debugLoginOptions->value("password", "password").toString());
            connections[i]->setHostName(debugLoginOptions->value("host", "127.0.0.2").toString());
            connections[i]->setPort(debugLoginOptions->value("port", 3306).toInt());
            connections[i]->setDatabaseName(debugLoginOptions->value("database", "service").toString());
        }
        else
        {
            connections[i]->setUserName(ui->editLogin->text());
            connections[i]->setPassword(hash.result().toHex());
            connections[i]->setHostName(ui->editIPaddr->text());
            connections[i]->setPort(ui->editPort->text().toUInt());
            connections[i]->setDatabaseName(ui->editDBName->text());
        }
    connections[i]->setConnectOptions(connOptions.join(";")+";");
    }

    if (!connMain.open())
    {
        qDebug() << "DB connect failed: " << connMain.lastError().text();
        statusBarMsg(connMain.lastError().text());
    }
    else
    {
        qDebug("DB successfully opened.");

        // установка режимов; начиная с MySQL v5.7.5 по умолчанию включен режим  ONLY_FULL_GROUP_BY
        QSqlQuery querySetSqlMode = QSqlQuery(QSqlDatabase::database("connMain"));
        querySetSqlMode.exec("SET SESSION sql_mode = sys.list_drop(@@session.sql_mode, 'ONLY_FULL_GROUP_BY');");

        // проверка состояния учетной записи пользователя (база программы, а не mysql)
        QSqlQuery queryCheckUser = QSqlQuery(QSqlDatabase::database("connMain"));
        queryCheckUser.exec(QUERY_SEL_USER_STATE(QSqlDatabase::database("connMain").userName()));
        queryCheckUser.first();
        try
        {
            if (queryCheckUser.isValid())
            {
                for (int i = 1; i<connections.size(); i++)  // открываем вспомогательные соединения
                    connections[i]->open();

                if(ui->checkBoxSaveOnSuccess->isChecked())
                {
                    userLocalData->dbHost.value = ui->editIPaddr->text();
                    userLocalData->dbPort.value = ui->editPort->text();
                    userLocalData->dbName.value = ui->editDBName->text();
                    userLocalData->lastLogin.value = ui->editLogin->text();
                    userLocalData->SSLConnection.value =  ui->checkBoxSSL->isChecked();
                    localSettings->save(userLocalData);
                }

                // TODO: SplashScreen с сообщениями об этапах инициализации
                if(!res.registerResource(QApplication::applicationDirPath() + "/schema-updates.rcc"))
                {
                    throw 3;    // Попытка подключения к старой версии БД ...
                }

                int schemaPatch = checkSchema();
                if( schemaPatch )
                {
                    updateDB(schemaPatch);  // обновление структуры БД
                }
                res.unregisterResource(QApplication::applicationDirPath() + "/schema-updates.rcc");
                if(checkAppVer())
                {
                    updateDB(0);    // обновление значения `config`.`version_snap`
                }

                emit this->DBConnectOK();
                this->hide();
                this->deleteLater();
            }
            else
            {
                throw 4;
            }
        }
        catch (int err)
        {
            switch (err) {
                case 1: statusBarMsg(tr("Не удалось обновить базу данных.")); break;
                case 2: statusBarMsg(tr("Не удалось обновить базу данных. Есть пользователи онлайн.")); break;
                case 3: statusBarMsg(tr("Попытка подключения к старой версии БД. Обратитесь к администратору.")); break;
                case 4: statusBarMsg(tr("Учетная запись отключена"), 0); break;
                case 5:
                {
                    statusBarMsg(tr("Требуется обновление программы"), 0);
                    startMaintanaceTool();
                    break;
                }
            }
            closeConnections();
        }
    }
}

void LoginWindow::btnCancelHandler()
{
    emit this->btnCancelClick();
}

void LoginWindow::clearStatusLabel()
{
    ui->labelStatus->clear();
}

void LoginWindow::createSSLOptionsDialog()
{
    overlay = new QWidget(this);
    overlay->setStyleSheet("QWidget { background: rgba(154, 154, 154, 128);}");
    overlay->resize(size());
    overlay->setVisible(true);

    modalWidget = new SSLOptionsDialog(this, Qt::SplashScreen);
    QObject::connect(modalWidget, SIGNAL(close()), this, SLOT(closeSSLOptionsDialog()));

    modalWidget ->setWindowModality(Qt::WindowModal);
    modalWidget ->show();
}

void LoginWindow::closeSSLOptionsDialog()
{
    if(modalWidget != nullptr)
    {
        modalWidget->deleteLater();
        modalWidget = nullptr;
    }
    if (overlay != nullptr)
    {
        overlay->deleteLater();
        overlay = nullptr;
    }
}

void LoginWindow::selectAscExe()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                                                          tr("Select ASC application executable"),
                                                                          "C:/Program Files (x86)/",
                                                                          tr("Executable (*.exe)"));
    if(!fileName.isEmpty())
    {
        userLocalData->ASCExecutablePath.value = fileName;  // до импорта
        localSettings->import(userLocalData);
        fillConnectionParams();
    }
}

