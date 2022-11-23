#include "global.h"
#include "appver.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "sqlcreds.h"
#include "com_sql_queries.h"

LoginWindow::LoginWindow(QObject *parent) :
//	QWidget(parent),
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
    logo = new QGraphicsPixmapItem(QPixmap("logo.png"));
    logoScene = new QGraphicsScene();
    logoScene->addItem(logo);
    ui->graphicsLogo->setScene(logoScene);
    ui->labelAppVer->setText(QString(APP_VER));

    if (settingsDOM == nullptr)
        settingsDOM = new QDomDocument("settingsDOM");

    if (genAscSettingsFilePath())
        readSettings(settingsAscFile);  // сначала пробуем прочитать настройки ASC
    if (genSettingsFilePath())
        readSettings(settingsFile);     // затем перезаписываем их своими настройками (если файл существует)
    else
        if (prevAppVerSettingsFile.exists())        // если файл настроек не найден, но найден файл настроек для предыдущей версии приложения,
            readSettings(prevAppVerSettingsFile);   // то считываем его

    if (userLocalData->contains("lastLogin"))
    {
        ui->editIPaddr->setText(userLocalData->value("dbHost").toString());
    }
    if (userLocalData->contains("lastLogin"))
    {
        ui->editPort->setText(userLocalData->value("dbPort").toString());
    }
    if (userLocalData->contains("lastLogin"))
    {
        ui->editDBName->setText(userLocalData->value("dbName").toString());
    }
    if (userLocalData->contains("lastLogin"))
    {
        ui->editLogin->setText(userLocalData->value("lastLogin").toString());
        ui->editPassword->setFocus();
    }
    if (userLocalData->contains("SSLConnection"))
    {
        ui->checkBoxSSL->setChecked(userLocalData->value("SSLConnection").toBool());
    }

//  shortlivedNotification::setSize(385, 90);
    shortlivedNotification::setAppearance(this, shortlivedNotification::bottomRight);
    statusBarDelay = new QTimer();
    statusBarDelay->setSingleShot(true);

    QObject::connect(statusBarDelay, SIGNAL(timeout()), this, SLOT(clearStatusLabel()));
    QObject::connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(btnLoginHandler()));
    QObject::connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(btnCancelHandler()));

}

LoginWindow::~LoginWindow()
{
    delete logo;
    delete ui;
}

bool LoginWindow::selMostRecentSettingFile(QDir &dir)
{
    QCollator collator;
    QStringList list;

    if (!dir.exists())    // проверка существования C:/Users/user/AppData/Local/[snap|VLab]/<hash-based-folder-name>/
        return 0;

    list = dir.entryList(QDir::NoDot | QDir::NoDotDot | QDir::Dirs, QDir::Name);    // все поддиректории (соответствуют версии приложения)

    if (list.size() == 0)
        return 0;

    collator.setNumericMode(true);
    std::sort(list.begin(), list.end(), collator);    // числовая сортировка папок

    QStringList::iterator i = list.end();
    while ( --i != list.begin() )
    {
        if (QFile::exists(settingsAscPath.path() + "/" + i->toUtf8() + "/user.config"))     // определяем файл самой последней версии приложения
            break;
    }
    dir.setPath(dir.path() + "/" + i->toUtf8());

    return 1;
}

bool LoginWindow::genSettingsFilePath()
{
    QString appPath = QApplication::applicationDirPath();
    QCryptographicHash hash(QCryptographicHash::Md5);
    QString tmp;

    hash.addData(appPath.toUtf8(), appPath.length());
    QString appPathHash = hash.result().toHex();
    settingsPath = QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));   // C:/Users/user/AppData/Local/snap
//    qDebug() << "appDataPath: " << settingsPath.path();
    settingsPath = QDir(settingsPath.path() + "/" + appPathHash);    // C:/Users/user/AppData/Local/snap/<MD5-hash>
    tmp = settingsPath.path();
//    qDebug() << "settingsPath(hash): " << settingsPath.path();
    if (selMostRecentSettingFile(settingsPath))     // если найдутся файлы настроек предыдущих версий приложения, то settingsPath будет обновлена,
        prevAppVerSettingsFile.setFileName(settingsPath.path() + "/user.config"); // а в переменную будет записан путь к файлу настроек
//    qDebug() << "prevAppVerSettingsFile: " << prevAppVerSettingsFile.fileName();
    settingsPath.setPath(tmp + "/" + APP_VER);  // устанавливаем путь, соответствующий текущий текущей версии приложения
    if (!settingsPath.exists())                 // C:/Users/user/AppData/Local/snap/<MD5-hash>/<APP_VER>
        settingsPath.mkpath(settingsPath.path());

    settingsFile.setFileName(settingsPath.path() + "/user.config");
//    qDebug() << "settingsFile: " << settingsFile.fileName();
    if(!settingsFile.exists())
        return 0;
    return 1;
}

bool LoginWindow::genAscSettingsFilePath()
{
    settingsAscPath.setPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/../" + ASC_SETTINGS_PATH);   // C:/Users/user/AppData/Local/snap
//    qDebug() << "settingsAscPath:" << settingsAscPath.path();
    if (selMostRecentSettingFile(settingsAscPath))     // если найдутся файлы настроек АСЦ, то settingsPath будет обновлена,
        settingsAscFile.setFileName(settingsAscPath.path() + "/user.config"); // а в переменную будет записан путь к файлу настроек
//    qDebug() << "settingsAscFile: " << settingsAscFile.fileName();
    if (!settingsAscFile.exists())
        return 0;
    return 1;
}

bool LoginWindow::readSettings(QFile &file)
{
    if (!file.open(QIODevice::ReadOnly))
    {
//        qDebug() << "Can't open settings file";
        return 0;
    }
    if (!settingsDOM->setContent(&file)) {
        file.close();
        return 0;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    docElem = settingsDOM->documentElement();   // root
//    qDebug() << "root: " << docElem.tagName();

    QDomNode n = docElem.firstChild();
    readSettingsNode(n);

    return 1;
}

bool LoginWindow::readSettingsNode(QDomNode &n)
{
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            QDomNode child = e.firstChild();
            // по образу и подобию файла настроек АСЦ: элемент с тэгом setting — это конечный элемент
            if (e.tagName() != "setting")
                readSettingsNode(child);    // считываем дочернние элементы
            else
            {
                QDomElement e2 = child.toElement(); // try to convert the node to an element.
                if(!e2.isNull())
                    userLocalData->insert(e.attribute("name"), e2.firstChild().nodeValue());
            }
        }
        n = n.nextSibling();
    }

    return 1;
}

bool LoginWindow::saveSettings()
{
    settingsDOM->clear();
    QDomElement *domElement;
    QDomNode    *value;
    QDomText    *text;
    QDomElement root = settingsDOM->createElement("configuration");
    settingsDOM->appendChild(root);

    QDomNode xmlNode = settingsDOM->createProcessingInstruction("xml",
                               "version=\"1.0\" encoding=\"UTF-8\"");
    settingsDOM->insertBefore(xmlNode,settingsDOM->firstChild());

    QDomElement tag1 = settingsDOM->createElement("userSettings");
    root.appendChild(tag1);

    QMap<QString, QVariant>::const_iterator setting = userLocalData->constBegin();
    while (setting != userLocalData->constEnd())
    {
//        qDebug() << setting.key() << ":" << setting.value();
        domElement = new QDomElement;
        *domElement = settingsDOM->createElement("setting");
        domElement->setAttribute("name", setting.key());
        tag1.appendChild(*domElement);

        value = new QDomNode();
        *value = settingsDOM->createElement("value");
        domElement->appendChild(*value);

        text = new QDomText;
        *text = settingsDOM->createTextNode(setting.value().toString());
        value->appendChild(*text);

        ++setting;
    }

    QString xml = settingsDOM->toString();
//    qDebug() << xml;

    if (!settingsFile.open(QIODevice::ReadWrite))
    {
//        qDebug() << "Can't create settings file";
        return 0;
    }
    settingsFile.resize(xml.size());
    settingsFile.write(xml.toUtf8());
    settingsFile.close();
    return 1;
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
    if(queryCheckSchema.value(0).toString() != "ASC.Scripts.Script000307.sql")  // версия БД АСЦ должна соответствовать версии приложения 3.7.31.1123
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
    queryCheckUsersOnline.exec(QUERY_SEL_ACTIVE_USERS(QSqlDatabase::database("connMain").databaseName(), QSqlDatabase::database("connMain").userName()));
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
                    if(!queryUpdate.exec(QUERY_UPD_APP_VER(APP_VER)))
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

void LoginWindow::editPassword_onReturnPressed()
{
    if(ui->editPassword->text() != "")
        btnLoginHandler();
}

#ifdef NO_LOGIN_
void LoginWindow::debugLogin()
{
    btnLoginHandler();
}
#endif

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
        if(userLocalData->contains("SSLKey") && userLocalData->value("SSLKey").toString() != "")
            connOptions << QString("SSL_KEY=%1").arg(userLocalData->value("SSLKey").toString());
        if(userLocalData->contains("SSLCert") && userLocalData->value("SSLCert").toString() != "")
            connOptions << QString("SSL_CERT=%1").arg(userLocalData->value("SSLCert").toString());
        if(userLocalData->contains("SSLCA") && userLocalData->value("SSLCA").toString() != "")
            connOptions << QString("SSL_CA=%1").arg(userLocalData->value("SSLCA").toString());
        if(userLocalData->contains("SSLCAPath") && userLocalData->value("SSLCAPath").toString() != "")
            connOptions << QString("SSL_CAPATH=%1").arg(userLocalData->value("SSLCAPath").toString());
        if(userLocalData->contains("SSLCipher") && userLocalData->value("SSLCipher").toString() != "")
            connOptions << QString("SSL_CIPHER=%1").arg(userLocalData->value("SSLCipher").toString());
    }
    for (int i=0; i<connections.size(); i++)
    {
#ifdef NO_LOGIN_
    connections[i]->setUserName(SQL_USER);
    connections[i]->setPassword(SQL_PSWD);
    connections[i]->setHostName(SQL_HOST);
    connections[i]->setPort(SQL_PORT);
    connections[i]->setDatabaseName(SQL_DB);
#else
    connections[i]->setUserName(ui->editLogin->text());
    connections[i]->setPassword(hash.result().toHex());
    connections[i]->setHostName(ui->editIPaddr->text());
    connections[i]->setPort(ui->editPort->text().toUInt());
    connections[i]->setDatabaseName(ui->editDBName->text());
#endif
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

                userLocalData->insert("dbHost", ui->editIPaddr->text());
                userLocalData->insert("dbPort", ui->editPort->text());
                userLocalData->insert("dbName", ui->editDBName->text());
                userLocalData->insert("lastLogin", ui->editLogin->text());
                userLocalData->insert("SSLConnection", ui->checkBoxSSL->isChecked());
                if(ui->checkBoxSaveOnSuccess->isChecked())
                    saveSettings();

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
                case 5: statusBarMsg(tr("Требуется обновление программы"), 0);/* TODO: обновление ПО (до нужной версии) */ break;
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

