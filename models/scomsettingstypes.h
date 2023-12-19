#ifndef SCOMSETTINGSTYPES_H
#define SCOMSETTINGSTYPES_H
#include <3rdparty/QSerializer/QSerializer>
#include "models/sstandarditemmodel.h"
#include <QTimeZone>

class t_status : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_FIELD(int, Id)
    QS_JSON_FIELD(QString, Name)
    QS_JSON_COLLECTION(QList, int, Contains)
    QS_JSON_COLLECTION(QList, int, Actions)
    QS_JSON_COLLECTION(QList, QString, Roles)   // QStringList удобнее для работы
    QS_JSON_FIELD(QString, Color)
    QS_JSON_FIELD(QString, Terms)
    public:
        qint64 TermsSec;
        QStringList ContainsStr;    // данные в таком формате нужны для фильтрации в прокси-моделях
        QStringList ActionsStr;

        void fromJson(const QJsonValue & val)
        {
            QSerializer::fromJson(val);

            int statusTermDays = 0;
            int statusTermSecons = 0;
            QTime statusTermTime;
            QString statusTermStr = Terms;
            QString statusTermDaysStr = Terms;  // дни отдельно
            statusTermDaysStr.chop(9);   // удаление части ".hh:mm:ss"; если дни не заданы, то результатом будет пустая строка
            statusTermDays = statusTermDaysStr.toInt();
            statusTermStr = statusTermStr.right(8);    // оставить только часы:минуты:секунды
            statusTermTime = QTime::fromString(statusTermStr, "hh:mm:ss");
            statusTermSecons = QTime(0, 0, 0).secsTo(statusTermTime);
            TermsSec = statusTermDays*24*3600 + statusTermSecons;
            ContainsStr = toStringList(Contains);
            ActionsStr = toStringList(Actions);

            if(Color.isEmpty())   // для вновь созданного статуса, если не был выбран цвет, в БД записывается значение null
                Color = "#00000000";
        }
    private:
        QStringList toStringList(QList<int> intList)
        {
            QStringList list;
            for(int i=0; i<intList.size(); i++)
            {
                list << QString::number(intList.at(i));
            }
            return list;
        }
};

class t_repairStatuses : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_COLLECTION_OBJECTS(QVector, t_status, status)
    public:
        SStandardItemModel *Model;
        qint64 RepairTermSec;
        void fromJsonArray(const QByteArray &data)
        {
            RepairTermSec = 0;
            int repairTermDays = 0;
            QSerializer::fromJsonArray(data);

            // Инициализация модели данных для comboBox'ов
            Model = new SStandardItemModel();
            QList<QStandardItem*> *item;
            for(int i=0; i<status.size(); i++)
            {
                repairTermDays += status[i].TermsSec/(24*3600);
                item = new QList<QStandardItem*>;
                *item << new QStandardItem(status[i].Name) << new QStandardItem(QString::number(status[i].Id));
                Model->appendRow(*item);
            }
            Model->setHorizontalHeaderLabels({"name","id"});
            if(repairTermDays < 2)
                RepairTermSec = 0;  // на случай, если в таблице нет статусов с длительностью в несколько дней
            else
                RepairTermSec = (repairTermDays*24 - 30)*3600;  // Такой странный алгоритм используется в АСЦ (подобран путём анализа)
        }

        t_status operator[](int id)
        {
            for(int i=0; i<status.size(); i++)
            {
                if(status[i].Id == id)
                    return status[i];
            }
            return t_status();
        };
};

class t_emailConfig : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_FIELD(QString, Host)
    QS_JSON_FIELD(int, Port)
    QS_JSON_FIELD(QString, Login)
    QS_JSON_FIELD(QString, Password)
    QS_JSON_FIELD(int, Timeout)
    QS_JSON_FIELD(bool, EnableSsl)
    QS_JSON_FIELD(bool, EnableImplicitSsl)
    QS_JSON_FIELD(QString, Template)
};

class t_repairDispatcherUsers : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_COLLECTION(QList, QString, Users)
};

class t_repairDispatcherAssignCriteria : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_COLLECTION(QList, QString, StatusList)
    QS_JSON_FIELD(int, DayLimit)
};

class t_smsConfig : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    QS_JSON_FIELD(int, Provider)
    QS_JSON_FIELD(int, AuthType)
    QS_JSON_FIELD(QString, ApiId)
    QS_JSON_FIELD(QString, Login)
    QS_JSON_FIELD(QString, Password)
    QS_JSON_FIELD(QString, Sender)
};

#endif // SCOMSETTINGSTYPES_H
