#include "shortlivednotification.h"

QVector<shortlivedNotification*> shortlivedNotification::p_instance;

shortlivedNotification::shortlivedNotification(QObject*, QString, QString, QColor, QColor, int)
{
    p_instance.append(this);
};
