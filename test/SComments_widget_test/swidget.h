#ifndef SWIDGET_H
#define SWIDGET_H

#include <QString>

class SWidget
{
public:
    explicit SWidget();
    ~SWidget();
    virtual int checkInput();
    virtual QString queryLogFile();
    virtual void commit(const int stage = 0);
    virtual void endCommit();
};

#endif // SWIDGET_H
