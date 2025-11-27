#include "snotification.h"

SNotification::SNotification(QWidget *parent)
    : QWidget{parent}
{

}

QString SNotification::typeToSubject(const int type)
{
    switch (type)
    {
        case Type::NotDefined: return ""; break;
        case Type::Comment: return tr("Новый комментарий"); break;
        case Type::RepairStatus: return tr("Изменён статус заказа"); break;
        case Type::SMS: return tr("SMS от: "); break;
        case Type::RepairTermControl: return tr("Превышение сроков"); break;
        case Type::Task: return tr("Задание: "); break;
        case Type::DeviceMatch: return tr("Совпадение устройства"); break;
        case Type::OrderFromSite: return tr("Заказ товара с сайта"); break;
        case Type::StoreRequest: return tr("Запрос детали"); break;
        case Type::BuyRequest: return tr("Запрос на покупку"); break;
        case Type::IncomingCall: return tr("Входящий звонок от: "); break;
        default: break;
    }
    return QString();
}
