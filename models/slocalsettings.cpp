#include "appver.h"
#include "global.h"
#include "slocalsettings.h"

const QLatin1String SLocalSettings::base32StringEncodeMap = QLatin1String("abcdefghijklmnopqrstuvwxyz012345");

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

void SLocalSettings::genSettingsFileName(QFile &file, const QString subVariant)
{
    switch(m_settingsVariant)
    {
        case UserSettings:
            file.setFileName("user.config");
            break;
        case StoreItemsGrid:
        case ProductsGrid:
            file.setFileName(QString(metaObject()->enumerator(metaObject()->indexOfEnumerator("SettingsVariant")).valueToKey(m_settingsVariant))
                             + "-"
                             + subVariant
                             + "-"
                             + userDbData->username
                             + ".xml");
            break;
        default:
            file.setFileName(QString(metaObject()->enumerator(metaObject()->indexOfEnumerator("SettingsVariant")).valueToKey(m_settingsVariant))
                             + "-"
                             + userDbData->username
                             + ".xml");
    }
}

QString SLocalSettings::appSettingsPath()
{
    QDir settingsPath;
    QString appPath = QApplication::applicationDirPath();
    QCryptographicHash hash(QCryptographicHash::Md5);

    hash.addData(appPath.toUtf8(), appPath.length());
    QString appPathHash = hash.result().toHex();

    // рабочий каталог C:/Users/<user>/AppData/Local/snap/<MD5-hash>
    settingsPath.setPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + appPathHash + "/" + APP_VER_STR);  // путь, соответствующий текущей версии приложения
    if (!settingsPath.exists())                 // C:/Users/<user</AppData/Local/snap/<MD5-hash>/<APP_VER>
        settingsPath.mkpath(settingsPath.path());

    return settingsPath.path();
}

bool SLocalSettings::genSettingsFileFullPath(QFile &file, Operation op)
{
    QString fileName = file.fileName();
    QString settingsPath = appSettingsPath();
    QDir::setCurrent(settingsPath + "/..");

    file.setFileName(settingsPath + "/" + fileName);
    if(file.exists() || op == SaveFile)
    {
        return 1;
    }

    if (selMostRecentSettingFile(fileName))    // QDir::current() будет обновлён
    {
        file.setFileName(QDir::current().absolutePath() + "/" + fileName);
        return 1;
    }

    return 0;
}

bool SLocalSettings::genAscSettingsFileFullPath(QFile &file)
{
    switch(m_settingsVariant)
    {
        case UserSettings:
            if(userLocalData->ASCExecutablePath.value.isEmpty())
                return 0;
            QDir::setCurrent(ascAppDataPath(userLocalData->ASCExecutablePath.value));
            if (!selMostRecentSettingFile(file.fileName()))     // если найдутся файлы настроек АСЦ, то QDir::current будет обновлена
                return 0;
            break;
        case StoreItemsGrid:
            // в АСЦ CRM до версии 3.7.37.1184 (включительно) файл имел имя "-<категория>-<имя_пользователя>.xml", например, "-0-admin.xml"
            file.setFileName(file.fileName().replace(metaObject()->enumerator(metaObject()->indexOfEnumerator("SettingsVariant")).valueToKey(m_settingsVariant), ""));
        default:
            QDir::setCurrent(QFileInfo(userLocalData->ASCExecutablePath.value).absolutePath() + QString("/cfg"));
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

bool SLocalSettings::import(QSerializer *obj, SettingsVariant variant, const QString subVariant)
{
    QFile file;
    QByteArray data;
    m_settingsVariant = variant;

    genSettingsFileName(file, subVariant);
    if(!genAscSettingsFileFullPath(file))
        return 0;

    if(!openFile(file, QIODevice::ReadOnly))
        return 0;

    data = file.readAll();
    switch(m_settingsVariant)
    {
        case UserSettings:
        {
            QString backup = userLocalData->ASCExecutablePath.value; // в конфигурации АСЦ нет такого параметра, поэтому его нужно сохранить перед импортом
            t_asc_configuration asc;
            asc.fromXml(data);
            // перенос элементов из узла "ASC.Properties.Settings" в узел "userSettings"
            obj->fromXml(asc.userSettings.ascPropertiesSettings.toXml());
            userLocalData->ASCExecutablePath.value = backup;
            break;
        }
//        case WorksGrid: data.replace("TreeListControl", "GridControl"); // в АСЦ использована не таблица, а дерево и тэги элементов структуры данных имеют другие значения; но есть и другие косяки файла АСЦ, требующие "костылей", поэтому данные настройки не импортируются (изменено название файла)...
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

bool SLocalSettings::read(QSerializer *obj, SettingsVariant variant, const QString subVariant)
{
    QFile file;
    bool ret = 1;
    m_settingsVariant = variant;

    genSettingsFileName(file, subVariant);
    if (genSettingsFileFullPath(file, ReadFile))
        ret = read(obj, file);     // сначала пробуем прочитать свои настройки (соответствующий текущей версии или предыдущий)
    else
        ret = import(obj, m_settingsVariant, subVariant);  // если ни один файл не найден, пробуем прочитать настройки ASC

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

bool SLocalSettings::save(QSerializer *obj, SettingsVariant variant, const QString subVariant)
{
    QFile file;
    m_settingsVariant = variant;

    genSettingsFileName(file, subVariant);
    genSettingsFileFullPath(file, SaveFile);

    return save(obj, file);
}

/* Определение пути к конфигурации приложения АСЦ
 * Спасибо товарищу freekzy за исследование алгоритма и написание демонстрационного кода.
*/
QString SLocalSettings::ascAppDataPath(const QString &executablePath)
{
    if(executablePath.isEmpty())
        return QString();

    int len;
    QString result;
    QString payloadData = executablePath;
    QByteArray payloadHead = QByteArrayLiteral("\x00\x01\x00\x00\x00\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x00\x00\x00\x00\x06\x01");
    QByteArray payloadLength;
    QByteArray payloadTail = QByteArrayLiteral("\x0B");
    QCryptographicHash cryptographicHash(QCryptographicHash::Sha1);
    QByteArray payloadSha1;
    QFileInfo execFile(executablePath);

    payloadData = "FILE:///" + payloadData.replace('/', "\\").toUpper();

    len = payloadData.length();
    for(int i=24; i>=0; i-=8)
        payloadLength.append((char)(len >> i) & 0xFF);

    QByteArray src;
    src = payloadHead + payloadLength + payloadData.toLocal8Bit() + payloadTail;
    cryptographicHash.addData(src);
    payloadSha1 = cryptographicHash.result();

    result = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/VLab/" + execFile.fileName() + "_Url_" + base32StringSuitableForDirName(payloadSha1);

    return result;
}

/* Кодирование Sha1 хэша в base32 по специфическому алгоритму
 * Ориганальный код: https://github.com/microsoft/referencesource/blob/51cf7850defa8a17d815b4700b67116e3fa283c2/System.Configuration/System/Configuration/ClientConfigPaths.cs#L585
*/
QString SLocalSettings::base32StringSuitableForDirName(const QByteArray &payloadSha1)
{
    QString result;
    const int length = 20;

    unsigned char b[5];
    int i = 0;
    do
    {
        for(int j=0; j<5; j++)
        {
            b[j] = (i < length) ? payloadSha1.at(i++) : 0;
            result.append(base32StringEncodeMap[b[j] & 0x1f]);
        }
        result.append(base32StringEncodeMap[(((b[0] & 0xe0) >> 5) | ((b[3] & 0x60) >> 2))]);
        result.append(base32StringEncodeMap[(((b[1] & 0xe0) >> 5) | ((b[4] & 0x60) >> 2))]);
        b[2] >>= 5;
        if ((b[3] & 0x80) != 0)
            b[2] |= 0x08;
        if ((b[4] & 0x80) != 0)
            b[2] |= 0x10;
        result.append(base32StringEncodeMap[b[2]]);
    } while (i < length);

    return result;
}

