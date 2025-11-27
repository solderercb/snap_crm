#ifndef TABCOMMON_H
#define TABCOMMON_H

#include <QList>
#include <SWidget>

class tabCommon : public SWidget
{
public:
    explicit tabCommon() : SWidget(){};
    ~tabCommon(){};
    QString tabTitle(){return "test";};

    template<typename t>
    QList<t> findChildren(){return QList<t>();};
};


#endif // TABCOMMON_H
