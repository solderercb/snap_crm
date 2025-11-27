#ifndef SAPPLOG_H
#define SAPPLOG_H

#include <QString>

class SAppLog
{
public:
    explicit SAppLog();
    QString filePath(){return "./exa/mple";};
    void setFile(const QString&){};
    bool appendRecord(const QString&){return 1;};
    static SAppLog* getInstance();
private:
    static SAppLog* p_instance;
};

#endif // SAPPLOG_H
