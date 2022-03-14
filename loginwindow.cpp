#include "global.h"
#include "appver.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "sqlcreds.h"

LoginWindow::LoginWindow(QObject *parent) :
//	QWidget(parent),
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
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

    QObject::connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(btnLoginHandler()));
    QObject::connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(btnCancelHandler()));

}

LoginWindow::~LoginWindow()
{
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
    if (settingsDOM == nullptr)
        settingsDOM = new QDomDocument("settingsDOM");

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

    QDomElement tag2 = settingsDOM->createElement("ASC.Properties.Settings");
    tag1.appendChild(tag2);

    QMap<QString, QVariant>::const_iterator setting = userLocalData->constBegin();
    while (setting != userLocalData->constEnd())
    {
//        qDebug() << setting.key() << ":" << setting.value();
        domElement = new QDomElement;
        *domElement = settingsDOM->createElement("setting");
        domElement->setAttribute("name", setting.key());
        tag2.appendChild(*domElement);

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

void LoginWindow::editPassword_onReturnPressed()
{
    if(ui->editPassword->text() != "")
        btnLoginHandler();
}

#ifdef QT_DEBUG
#ifdef NO_LOGIN
void LoginWindow::debugLogin()
{
    btnLoginHandler();
}
#endif
#endif

void LoginWindow::btnLoginHandler()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);
    QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)

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

    for (int i=0; i<connections.size(); i++)
    {

    connections[i]->setUserName(ui->editLogin->text());
    connections[i]->setPassword(hash.result().toHex());
    connections[i]->setHostName(ui->editIPaddr->text());
    connections[i]->setPort(ui->editPort->text().toUInt());
    connections[i]->setDatabaseName(ui->editDBName->text());
#ifdef NO_LOGIN
    connections[i]->setUserName(SQL_USER);
    connections[i]->setPassword(SQL_PSWD);
    connections[i]->setHostName(SQL_HOST);
    connections[i]->setPort(SQL_PORT);
    connections[i]->setDatabaseName(SQL_DB);
#endif
//		connections[i]->setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5;MYSQL_OPT_READ_TIMEOUT=1");
    }

    if (!connMain.open())
    {
        qDebug() << "DB connect failed: " << connMain.lastError().driverText();
        ui->labelStatus->setText(connMain.lastError().driverText());
        ui->labelStatus->setStyleSheet("color: red; font: bold;");
    }
    else
    {
        qDebug("DB successfully opened.");
        QSqlQuery queryCheckUser = QSqlQuery(QSqlDatabase::database("connMain"));   // проверка состояния учетной записи пользователя (база программы, а не mysql)
        queryCheckUser.exec(QString("SELECT `id` FROM `users` WHERE `username` = '%1' AND `state` = 1 AND `is_bot` = 0 LIMIT 1;").arg(QSqlDatabase::database("connMain").userName()));
        queryCheckUser.first();
        if (queryCheckUser.isValid())
        {
            for (int i = 1; i<connections.size(); i++)  // открываем вспомогательные соединения
                connections[i]->open();
            emit this->DBConnectOK();

            userLocalData->insert("dbHost", ui->editIPaddr->text());
            userLocalData->insert("dbPort", ui->editPort->text());
            userLocalData->insert("dbName", ui->editDBName->text());
            userLocalData->insert("lastLogin", ui->editLogin->text());
            saveSettings();

            this->hide();
            this->deleteLater();
        }
        else
        {
            ui->labelStatus->setText("Учетная запись отключена");
            ui->labelStatus->setStyleSheet("color: red; font: bold;");
        }
    }
}

void LoginWindow::btnCancelHandler()
{
	emit this->btnCancelClick();
}
