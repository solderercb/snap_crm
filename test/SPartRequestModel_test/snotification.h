#ifndef SNOTIFICATION_H
#define SNOTIFICATION_H

#include <QString>

class SNotification
{
public:
    enum Type {BuyRequest};
    SNotification();
    static QString typeToSubject(const int){return "TEST <Заголовок>";};
};

#endif // SNOTIFICATION_H
