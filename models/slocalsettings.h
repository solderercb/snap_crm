#ifndef SLOCALSETTINGS_H
#define SLOCALSETTINGS_H

#include <QApplication>
#include <QObject>
#include <QDomDocument>
#include <QtXml>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QResource>
#include <QIODevice>
#define ASC_SETTINGS_PATH   "VLab/ASC.exe_Url_1ttwwpflaim4gbzvmtfpiarttwuebugu"    // путь к файлу настроек АЦС для пути к приложению "C:\Program Files (x86)\ASCApp\ASC.exe"

class SLocalSettings : public QObject
{
    Q_OBJECT
public:
    explicit SLocalSettings(QObject *parent = nullptr);
    bool contains(const QString &setting);
    QVariant value(const QString &setting);
    void insert(const QString &setting, QVariant value);
    bool saveSettings();
private:
    QMap<QString, QVariant> m_localSettings;
    QTimer *m_timeoutToSave;
    bool m_changed;
    bool selMostRecentSettingFile(QDir &);
    bool genSettingsFilePath();
    bool genAscSettingsFilePath();
    bool readSettings(QFile &file);
    bool readSettingsNode(QDomNode &n);
    QDomDocument *settingsDOM = nullptr;
    QDomElement docElem;
    QFile settingsFile;
    QFile settingsAscFile;
    QDir settingsPath;
    QDir settingsAscPath;
    QFile prevAppVerSettingsFile;
signals:

};

#endif // SLOCALSETTINGS_H
