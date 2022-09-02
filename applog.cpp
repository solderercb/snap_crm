#include "applog.h"

SAppLog::SAppLog()
{
    QDir appDataDir = QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));   // C:/Users/user/AppData/Local
    file.setFileName(appDataDir.path()+"/snap/snap.log");
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
