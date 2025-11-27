#ifndef SHORTLIVEDNOTIFICATION_H
#define SHORTLIVEDNOTIFICATION_H

#include <QObject>
#include <QColor>
#include <QVector>

class shortlivedNotification
{
    friend class TClassTest;
public:
    explicit shortlivedNotification(QObject*, QString, QString, QColor = captionColor, QColor = messageColor, int = 3000);
 
private:
    static QColor captionColor;
    static QColor messageColor;
    static QVector<shortlivedNotification*> p_instance; // по кол-ву будет производиться проверка вызвался ли конструктор
};

#endif // SHORTLIVEDNOTIFICATION_H
