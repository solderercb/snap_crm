#include "appver.h"
#include "global.h"
#include "slocalsettings.h"

/*  Поиск существующего файла настроек, соответствующий самой последней версии программы
 *  Возвращает 1 и обновляет путь к к рабочей директории, если файл найден
*/
bool SLocalSettings::selMostRecentSettingFile(const QString &fileName)
{
    QDir dir = QDir::current();
    QCollator collator;
    QStringList subDirsList;

    if (!dir.exists())    // проверка существования C:/Users/user/AppData/Local/[snap|VLab]/<hash-based-folder-name>/
        return 0;

    subDirsList = dir.entryList(QDir::NoDot | QDir::NoDotDot | QDir::Dirs, QDir::Name);    // все поддиректории (соответствуют версии приложения)

    if (subDirsList.size() == 0)
        return 0;

    collator.setNumericMode(true);
    std::sort(subDirsList.begin(), subDirsList.end(), collator);    // числовая сортировка папок

    QStringList::iterator subDir = subDirsList.end();
    while ( --subDir != subDirsList.begin() )
    {
        if (QFile::exists(dir.path() + "/" + subDir->toUtf8() + "/" + fileName))     // определяем файл самой последней версии приложения
        {
            QDir::setCurrent(dir.path() + "/" + subDir->toUtf8());
            return 1;
        }
    }
    return 0;
}

bool SLocalSettings::genSettingsFilePath(QFile &file)
{
    QDir settingsPath;
    QString appPath = QApplication::applicationDirPath();
    QCryptographicHash hash(QCryptographicHash::Md5);

    hash.addData(appPath.toUtf8(), appPath.length());
    QString appPathHash = hash.result().toHex();

    // рабочий каталог C:/Users/<user>/AppData/Local/snap/<MD5-hash>
    settingsPath.setPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + appPathHash + "/" + APP_VER);  // путь, соответствующий текущей версии приложения
    if (!settingsPath.exists())                 // C:/Users/<user</AppData/Local/snap/<MD5-hash>/<APP_VER>
        settingsPath.mkpath(settingsPath.path());
    QDir::setCurrent(settingsPath.path() + "/..");

    switch(m_settingsVariant)
    {
        case UserSettings:
            file.setFileName("user.config");
            break;
        default:
            file.setFileName(QString(metaObject()->enumerator(metaObject()->indexOfEnumerator("SettingsVariant")).valueToKey(m_settingsVariant)) + "-" + userDbData->value("username").toString() + ".xml");
    }

    file.setFileName(settingsPath.path() + "/" + file.fileName());
    if(file.exists())
        return 1;

    if (selMostRecentSettingFile(file.fileName()))    // QDir::current() будет обновлён
    {
        file.setFileName(QDir::current().absolutePath() + "/" + file.fileName());
        return 1;
    }

    return 0;
}

bool SLocalSettings::genAscSettingsFilePath(QFile &file)
{
    switch(m_settingsVariant)
    {
        case UserSettings:
            QDir::setCurrent(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/../" + ASC_SETTINGS_PATH);
            file.setFileName("user.config");
            if (!selMostRecentSettingFile(file.fileName()))     // если найдутся файлы настроек АСЦ, то QDir::current будет обновлена
                return 0;
            break;
        case StoreItemsGrid:    // в АСЦ CRM до версии 3.7.37.1184 (включительно) файл имел имя "-0-<имя_пользователя>.xml"
            QDir::setCurrent(ASC_APP_PATH + QString("/cfg"));
            file.setFileName("-0-" + userDbData->value("username").toString() + ".xml");
            break;
        default:
            QDir::setCurrent(ASC_APP_PATH + QString("/cfg"));
            file.setFileName(QString(metaObject()->enumerator(metaObject()->indexOfEnumerator("SettingsVariant")).valueToKey(m_settingsVariant)) + "-" + userDbData->value("username").toString() + ".xml");
    }

    file.setFileName(QDir::current().absolutePath() + "/" + file.fileName()); // установка абсолютного пути к файлу

    return 1;
}

bool SLocalSettings::openFile(QFile &file, QIODevice::OpenModeFlag mode)
{
    if (!file.open(mode))
    {
        appLog->appendRecord(QString("Can't open settings file %1").arg(file.fileName()));
        qDebug() << "Can't open settings file " << file.fileName();
        return 0;
    }
    return 1;
}

bool SLocalSettings::import(QSerializer *obj, SettingsVariant variant)
{
    QFile file;
    QByteArray data;
    m_settingsVariant = variant;

    if(!genAscSettingsFilePath(file))
        return 0;

    if(!openFile(file, QIODevice::ReadOnly))
        return 0;

    data = file.readAll();
    switch(m_settingsVariant)
    {
        case UserSettings:
        {
            t_asc_configuration asc;
            asc.fromXml(data);
            // перенос элементов из узла "ASC.Properties.Settings" в узел "userSettings"
            obj->fromXml(asc.userSettings.ascPropertiesSettings.toXml());
            break;
        }
        default: obj->fromXml(data);
    }
    file.close();

    return 1;
}

bool SLocalSettings::read(QSerializer *obj, QFile &file)
{
    QByteArray data;
    if(!openFile(file, QIODevice::ReadOnly))
        return 0;

    data = file.readAll();
    obj->fromXml(data);

    file.close();

    return 1;
}

bool SLocalSettings::read(QSerializer *obj, SettingsVariant variant)
{
    QFile file;
    bool ret = 1;
    m_settingsVariant = variant;
    if (genSettingsFilePath(file))
        ret = read(obj, file);     // сначала пробуем прочитать свои настройки (соответствующий текущей версии или предыдущий)
    else
        ret = import(obj, m_settingsVariant);  // если ни один файл не найден, пробуем прочитать настройки ASC

    return ret;
}

bool SLocalSettings::save(QSerializer *obj, QFile &file)
{
    QByteArray data;
    if(!openFile(file, QIODevice::ReadWrite))
        return 0;

    QDomDocument node = obj->appendXmlHat(obj->toXml(), "UTF-8");
    data = obj->toByteArray(node).constData();
    file.resize(data.size());
    file.write(data);
    file.close();

    return 1;
}

bool SLocalSettings::save(QSerializer *obj, SettingsVariant variant)
{
    QFile file;
    m_settingsVariant = variant;
    genSettingsFilePath(file);

    return save(obj, file);
}


