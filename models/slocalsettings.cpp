#include "appver.h"
#include "global.h"
#include "slocalsettings.h"

SLocalSettings::SLocalSettings(QObject *parent) : QObject(parent)
{
    m_timeoutToSave = new QTimer;
    m_timeoutToSave->setSingleShot(true);
    QObject::connect(m_timeoutToSave, &QTimer::timeout, this, &SLocalSettings::saveSettings);

    if (settingsDOM == nullptr)
        settingsDOM = new QDomDocument("settingsDOM");

    if (genAscSettingsFilePath())
        readSettings(settingsAscFile);  // сначала пробуем прочитать настройки ASC
    if (genSettingsFilePath())
        readSettings(settingsFile);     // затем перезаписываем их своими настройками (если файл существует)
    else
        if (prevAppVerSettingsFile.exists())        // если файл настроек не найден, но найден файл настроек для предыдущей версии приложения,
            readSettings(prevAppVerSettingsFile);   // то считываем его
}

bool SLocalSettings::contains(const QString &setting)
{
    return m_localSettings.contains(setting);
}

QVariant SLocalSettings::value(const QString &setting)
{
    return m_localSettings.value(setting);
}

void SLocalSettings::insert(const QString &setting, QVariant value)
{
    m_changed = 1;
    m_timeoutToSave->start(5000);
    m_localSettings.insert(setting, value);
}

bool SLocalSettings::selMostRecentSettingFile(QDir &dir)
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

bool SLocalSettings::genSettingsFilePath()
{
    QString appPath = QApplication::applicationDirPath();
    QCryptographicHash hash(QCryptographicHash::Md5);
    QString tmp;

    hash.addData(appPath.toUtf8(), appPath.length());
    QString appPathHash = hash.result().toHex();
    settingsPath = QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));   // C:/Users/user/AppData/Local/snap
    settingsPath = QDir(settingsPath.path() + "/" + appPathHash);    // C:/Users/user/AppData/Local/snap/<MD5-hash>
    tmp = settingsPath.path();
    if (selMostRecentSettingFile(settingsPath))     // если найдутся файлы настроек предыдущих версий приложения, то settingsPath будет обновлена,
        prevAppVerSettingsFile.setFileName(settingsPath.path() + "/user.config"); // а в переменную будет записан путь к файлу настроек
    settingsPath.setPath(tmp + "/" + APP_VER);  // устанавливаем путь, соответствующий текущий текущей версии приложения
    if (!settingsPath.exists())                 // C:/Users/user/AppData/Local/snap/<MD5-hash>/<APP_VER>
        settingsPath.mkpath(settingsPath.path());

    settingsFile.setFileName(settingsPath.path() + "/user.config");
    if(!settingsFile.exists())
        return 0;
    return 1;
}

bool SLocalSettings::genAscSettingsFilePath()
{
    settingsAscPath.setPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/../" + ASC_SETTINGS_PATH);   // C:/Users/user/AppData/Local/snap
    if (selMostRecentSettingFile(settingsAscPath))     // если найдутся файлы настроек АСЦ, то settingsPath будет обновлена,
        settingsAscFile.setFileName(settingsAscPath.path() + "/user.config"); // а в переменную будет записан путь к файлу настроек
    if (!settingsAscFile.exists())
        return 0;
    return 1;
}

bool SLocalSettings::readSettings(QFile &file)
{
    if (!file.open(QIODevice::ReadOnly))
    {
        appLog->appendRecord("Can't open settings file");
        qDebug() << "Can't open settings file";
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

    QDomNode n = docElem.firstChild();
    readSettingsNode(n);

    return 1;
}

bool SLocalSettings::readSettingsNode(QDomNode &n)
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
                    m_localSettings.insert(e.attribute("name"), e2.firstChild().nodeValue());
            }
        }
        n = n.nextSibling();
    }

    return 1;
}

bool SLocalSettings::saveSettings()
{
    m_timeoutToSave->stop();
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

    QMap<QString, QVariant>::const_iterator setting = m_localSettings.constBegin();
    while (setting != m_localSettings.constEnd())
    {
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

    if (!settingsFile.open(QIODevice::ReadWrite))
    {
        appLog->appendRecord("Can't create settings file.");
        qDebug() << "Can't create settings file.";
        return 0;
    }
    settingsFile.resize(xml.size());
    settingsFile.write(xml.toUtf8());
    settingsFile.close();
    m_changed = 1;

    return 1;
}

