#ifndef SNOTIFICATION_H
#define SNOTIFICATION_H

#include <QWidget>

class SNotification : public QWidget
{
    Q_OBJECT
public:
    // тип уведомления используется для цветового различия; цвета задаются в основных настроках; для типа 0 заголовок и рамка сообщения будут белыми
    enum Type {NotDefined = 0, Comment = 1, RepairStatus = 2, SMS = 3, RepairTermControl = 4, Task = 5, DeviceMatch = 6, OrderFromSite = 7, StoreRequest = 8, BuyRequest = 9, IncomingCall = 10};
    explicit SNotification(QWidget *parent = nullptr);
    static QString typeToSubject(const int type);
signals:

};

#endif // SNOTIFICATION_H
