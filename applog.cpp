#include "applog.h"
#include <SLocalSettings>
#include <QDebug>

SAppLog* SAppLog::p_instance = nullptr;

SAppLog::SAppLog()
{
    file.setFileName(SLocalSettings::appSettingsPath()+"/snap.log");
}

QString SAppLog::filePath()
{
    return file.fileName();
}

void SAppLog::setFile(const QString &filename)
{
    file.setFileName(filename);
}

bool SAppLog::appendRecord(const QString &text)
{
    if(text.isEmpty())
        return 1;

    QByteArray fileBuf = 0;

    if (file.open(QIODevice::ReadWrite))
    {
        fileBuf = file.readAll();
        fileBuf.append(text.toLocal8Bit()+"\r\n");
        file.reset();
        file.write(fileBuf);
        file.close();
        return 1;
    }
    else
    {
        qDebug() << "Can't open appLog file";
        return 0;
    }
}

SAppLog *SAppLog::getInstance()
{
    if( !p_instance )
      p_instance = new SAppLog();
    return p_instance;
}
