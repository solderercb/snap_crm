#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>
#include <QCryptographicHash>
#include <QFile>
#include <QIODevice>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QResource>
#include <ProjectGlobals>
#include <appVer>
#include <SAppLog>
#include <ProjectQueries>
#include <SSingleRowModel>
#include <SLocalSettings>
#include <SLocalSettingsStructs>
#include <SSLOptionsDialog>
#include <FlashPopup>
#include <windowsDispatcher>
#include <QtAutoUpdaterWidgets/UpdateController>
#include <QtAutoUpdaterWidgets/UpdateButton>

LoginWindow::LoginWindow(QObject*) :
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
    logo = new QGraphicsPixmapItem(QPixmap("logo.png"));
    logoScene = new QGraphicsScene();
    logoScene->addItem(logo);
    ui->graphicsLogo->setScene(logoScene);
    ui->labelAppVer->setText(QString(APP_VER_STR));

    userLocalData = new t_userSettings;
    localSettings->read(userLocalData);

    if(setDebugLoginCreds())
    {
        debugLoginDelay = std::make_unique<QTimer>(); // задержка нужна, чтобы автоматически выполняемые при отладке действия гарантированно запускались после вызова QApplication::exec()
        debugLoginDelay->setSingleShot(true);
        connect(debugLoginDelay.get(), &QTimer::timeout, this, &LoginWindow::debugLogin);
    }
    else
        fillConnectionParams();

//  shortlivedNotification::setSize(385, 90);
    shortlivedNotification::setAppearance(this, shortlivedNotification::bottomRight);
    statusBarDelay = new QTimer();
    statusBarDelay->setSingleShot(true);

    connect(statusBarDelay, SIGNAL(timeout()), this, SLOT(clearStatusLabel()));
    connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(btnLoginHandler()));
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(btnCancelHandler()));
    connect(ui->pushButtonSettingsImport, &QPushButton::clicked, this, &LoginWindow::selectAscExe);
    connect(ui->editDBName, &QLineEdit::returnPressed, this, &LoginWindow::btnLoginHandler);

    if(userLocalData->FontFamily.value.isEmpty())
        userLocalData->FontFamily.value = "Segoe UI";
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
    QSqlQuery query = QSqlQuery(QSqlDatabase::database(TdConn::main()));

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

void LoginWindow::show()
{
    QWidget::show();
    if(debugLoginDelay)
        debugLoginDelay->start(100);
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
    QSqlQuery queryCheckAppVer = QSqlQuery(QSqlDatabase::database(TdConn::main()));
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
    QSqlQuery queryCheckSchema = QSqlQuery(QSqlDatabase::database(TdConn::main()));
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

/* Проверка привилегии PROCESS
 * Пользователь должен обладать этой привилегией для проверки блокировки карточки ремонта
 * Это второй вариант проверки; первый (rev0.0.0.279 от 14.12.2023) проще в плане кода, но требует привилегии SELECT для таблицы mysql
*/
bool LoginWindow::checkProcessPriv()
{
    bool ret = 0;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database(TdConn::main()));
    QString grants;
    int end;
    query->exec("SHOW GRANTS;");
    while(query->next() && !ret)
    {
        grants = query->value(0).toString();
        grants.replace("GRANT ", "");
        end = grants.indexOf(" ON *.*");
        if(end < 0)
            continue;

        grants = grants.left(end).replace(", ", ",");
        QStringList grantsLst = grants.split(',');
        ret = grantsLst.contains("PROCESS") || grantsLst.contains("ALL PRIVILEGES");
    }

    delete query;

    return ret;
}

/*  Проверка активных подключений к базе
 *  Этот метод нужен "на всякий случай"; базу нужно обновлять в нерабочее время :-)
 *  Возвращает список имён пользователей или пустой список.
 */
QStringList LoginWindow::usersOnline()
{
    QSqlQuery queryCheckUsersOnline = QSqlQuery(QSqlDatabase::database(TdConn::main()));

    // запрос активных соединений
    queryCheckUsersOnline.exec(QUERY_SEL_ACTIVE_USERS(loginCreds->value("database").toString()));
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
    QSqlQuery queryUpdate = QSqlQuery(QSqlDatabase::database(TdConn::session()));

    if (loginCreds->value("user").toString() == "admin")  // только админ может обновлять БД
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

/* Инициализация контейнера с данными для подключения к БД.
 * Возвращает 1 если контейнер инициализирован данными, заданными в debug.ini.
*/
bool LoginWindow::setDebugLoginCreds()
{
    QVariant tmp;
    QStringList creds = {"user", "password", "host", "port", "database"};
    if(debugOptions == nullptr)
        return 0;

    if (!debugOptions->childGroups().contains("debugLogin"))
        return 0;

    debugOptions->beginGroup("debugLogin");

    QStringList allKeys = debugOptions->allKeys();
    for(int i = 0; i < allKeys.size(); i++)
    {
        tmp = debugOptions->value(allKeys.at(i));
        if(tmp.isValid())
        {
            loginCreds->insert(allKeys.at(i), tmp);
            creds.removeOne(allKeys.at(i));
        }
    }
    debugOptions->endGroup();

    if(creds.contains("port"))
    {
        loginCreds->insert("port", 3306);
        creds.removeOne("port");
    }

    if(creds.empty())
        loginCreds->insert("isDebug", 1);

    return 1;
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
    connMain = QSqlDatabase::addDatabase("QMYSQL", TdConn::main());       // это основное соединение; используется преимущественно для запросов SELECT
    connections.append(&connMain);
    connThird = QSqlDatabase::addDatabase("QMYSQL", TdConn::session());   // это соединение для транзакционных запросов (BEGIN ... COMMIT)
    connections.append(&connThird);
//    connNtfy = QSqlDatabase::addDatabase("QMYSQL", "connNtfy");
//    connections.append(&connNtfy);
//    connFourth = QSqlDatabase::addDatabase("QMYSQL", "connFourth"); // в АСЦ сразу четыре соединения, зачем — ХЗ; понадобятся, тогда включим.
//    connections.append(&connFourth);

    connOptions << ("MYSQL_OPT_RECONNECT=1") << "MYSQL_OPT_CONNECT_TIMEOUT=3";
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
        if(!loginCreds->contains("isDebug"))
        {
            // паролем пользователя БД — это MD5-хэш пароля пользователя АСЦ; это сделано для того, чтобы пользователь не мог получить доступ непосредственно к базе
            // TODO: Придумать механизм защиты (например, нестандартный инициализирующий вектор для алгоритма MD5 или другой алгоритм) с которым будет скомпилировано
            // приложение, а любознательный пользователь, обнаруживший исходные коды проекта в Интернет, опять же не мог получить доступ непосредственно к базе
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(ui->editPassword->text().toUtf8(), ui->editPassword->text().length());
            //    qDebug() << hash.result().toHex();

            loginCreds->insert("user", ui->editLogin->text());
            loginCreds->insert("password", hash.result().toHex());
            loginCreds->insert("host", ui->editIPaddr->text());
            loginCreds->insert("port", ui->editPort->text().toUInt());
            loginCreds->insert("database", ui->editDBName->text());
        }
        connections[i]->setUserName(loginCreds->value("user", "user").toString());
        connections[i]->setPassword(loginCreds->value("password", "password").toString());
        connections[i]->setHostName(loginCreds->value("host", "127.0.0.1").toString());
        connections[i]->setPort(loginCreds->value("port", 3306).toInt());
        connections[i]->setDatabaseName(loginCreds->value("database", "service").toString());
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
        QSqlQuery querySetSqlMode = QSqlQuery(QSqlDatabase::database(TdConn::main()));
        querySetSqlMode.exec("SET SESSION sql_mode = sys.list_drop(@@session.sql_mode, 'ONLY_FULL_GROUP_BY');");

        // проверка состояния учетной записи пользователя (база программы, а не mysql)
        QSqlQuery queryCheckUser = QSqlQuery(QSqlDatabase::database(TdConn::main()));
        queryCheckUser.exec(QUERY_SEL_USER_STATE(loginCreds->value("user").toString()));
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

                SSingleRowModel::checkSystemTime();

                if(!checkProcessPriv())
                    throw 6;

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
                case 6: statusBarMsg(tr("Пользователь не обладает привилегией PROCESS")); break;
            }
            closeConnections();
        }
        catch (const Global::ThrowType err)
        {
            switch (err) {
                case Global::ThrowType::TimeError: statusBarMsg(tr("Локальное время и время сервера не совпадают")); break;
                default: statusBarMsg(tr("Необрабатанное исключение"));
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

