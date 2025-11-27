#ifndef SAPPLOG_H
#define SAPPLOG_H

class SAppLog
{
public:
    explicit SAppLog();
    QString filePath();
    void setFile(const QString&);
    bool appendRecord(const QString&);
    static SAppLog* getInstance();
private:
    QFile file;
    static SAppLog* p_instance;
};

#endif // SAPPLOG_H
