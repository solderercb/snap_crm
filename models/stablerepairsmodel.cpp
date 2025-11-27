#include "stablerepairsmodel.h"
#include <QColor>
#include <ProjectGlobals>
#include <SRepairsView>
#include <SUserSettings>
#include <SComSettings>
#include <SPermissions>
#include <SSqlQueryModel>
#include <STableBaseModel>

STableRepairsModel::STableRepairsModel(QObject *parent) :
    SSqlFetchingModel(parent),
    STableModelsCommonMethods(parent)
{
    m_sqlFetchingModel = this;
}

QVariant STableRepairsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    // TODO: изучить разницу в скорости выполнения:
    //  1) определение нужного столбца путём вызова кучи методов (до rev277)
    //  2) использование enum (с rev277)
    if (role == Qt::BackgroundRole)
    {
        if(index.column() == Columns::Status)
            return QColor(comSettings->repairStatusesVariantCopy()[unformattedData(index, Qt::DisplayRole).toInt()].Color);

        QString rowColor = SSqlFetchingModel::data(index.siblingAtColumn(Columns::Color)).toString();
        if(!rowColor.isEmpty())
            return QColor(rowColor);
    }
    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case Columns::Status:           return comSettings->repairStatusesVariantCopy()[unformattedData(index, Qt::DisplayRole).toInt()].Name;
            case Columns::RealRepairCost:   return dataLocalizedFromDouble(index);
            case Columns::InDate:
            case Columns::OutDate:          return dateTime(index);
            case Columns::Master:
            case Columns::CurrentManager:   return userFromId(index);
            case Columns::Box:              return QVariant(repairBoxesModel->getDisplayRole(unformattedData(index, role).toInt()));
            case Columns::Id:               return QString("%1-%2").arg(unformattedData(index.siblingAtColumn(Columns::Office)).toString().rightJustified(3, '0')).arg(unformattedData(index).toString().rightJustified(6, '0'));
            case Columns::Client:           return clientName(index);
            case Columns::Phone:            if(!permissions->viewClients) return tr("no permissions");
            default: ;
        }
    }
    return SSqlFetchingModel::data(index, role);
}

Qt::ItemFlags STableRepairsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void STableRepairsModel::initDemo()
{
    QString demoQuery;
    QStringList demoIds = {"1234", "2345", "3456", "4567"};
    QStringList demoTitles = {"ноутбук Lenovo 330S-15IKB", "мобильный телефон Xiaomi Redmi Note 4X", "телевизор LG 32LB563U-ZT", "АКБ с контроллером Lenovo L18C3PF7"};
    QStringList demoSNs = {"0987Y4T", "NXAGIUSYE", "2YHIGH", "P983T"};
    QStringList demoClients = {"Иванов Иван Иванович", "Петренко Пётр Петрочич", "Андреев Андрей Андреевич", "Никитенко Никита Никитович"};
    QStringList demoFaults = {"поврежден корпус", "разбит дисплей/сенсор", "нет подсветки ЖК-панели", "Низкое время работы"};
    QStringList demoExtNotes = {"кошка бежала хвостиком махнула", "", "", "установить элементы наибольшей ёмкости"};
    QStringList demoConstsForUnion;

    demoQuery = QUERY_SEL_WORKSHOP_STATIC;   // названия полей в отчете
    demoQuery.replace(STableBaseModel::queryPrepareRegexpPattern, "\n");
    demoQuery  += " WHERE t1.`id` IS NULL";

    SSqlFetchingModel::setQuery(demoQuery, QSqlDatabase::database("connMain"));

    for(int r = 0; r < demoIds.count(); r++)
    {
        demoConstsForUnion.clear();
        for(int c = 0; c < columnCount(); c++)
        {
            switch (c)
            {
                case Columns::Id:           demoConstsForUnion.append(demoIds.at(r)); break;
                case Columns::Title:        demoConstsForUnion.append(demoTitles.at(r)); break;
                case Columns::SerialNumber: demoConstsForUnion.append(demoSNs.at(r)); break;
                case Columns::Fault:        demoConstsForUnion.append(demoFaults.at(r)); break;
                case Columns::InDate:       demoConstsForUnion.append(QDateTime::currentDateTime().toString("yyyy.MM.dd HH:mm")); break;
                case Columns::Client:       demoConstsForUnion.append(demoClients.at(r)); break;
                case Columns::Office:       demoConstsForUnion.append(QString::number(userDbData->currentOffice())); break;
                case Columns::ExtNotes:     demoConstsForUnion.append(demoExtNotes.at(r)); break;
                default:                    demoConstsForUnion.append("<data>");
            }
        }
        demoQuery += "\nUNION ALL\n" \
                     "SELECT '" + demoConstsForUnion.join("', '") + "'";
    }

    SSqlFetchingModel::setQuery(demoQuery, QSqlDatabase::database("connMain"));
}

void STableRepairsModel::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = 0; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = columnName(info.index); break;
        case LimeReport::CallbackInfo::ColumnData: data = index(info.index, columnIndex(info.columnName)).data(); break;
        case LimeReport::CallbackInfo::ColumnCount: data = columnCount(); break;
        case LimeReport::CallbackInfo::RowCount: data = rowCount(); break;
    }
}

QModelIndex STableRepairsModel::indexForShortData(const QModelIndex &index) const
{
    switch(index.column())
    {
        case Columns::Client: return index.siblingAtColumn(Columns::ClientShortName);
    }

    return QModelIndex();
}

QVariant STableRepairsModel::clientName(const QModelIndex &idx) const
{
    if(!permissions->viewClients)
        return tr("no permissions"); // TODO: заменить "no permissions" на "Имя Ф." (как при переводе денег на карту)

    return dataShort(idx);
}

QVariant STableRepairsModel::dateTime(const QModelIndex &idx) const
{
    int width = i_columnWidths[idx.column()];

    if(width > 16)
        return timestampLocal(idx).toString("dd.MM.yyyy hh:mm");
    else if(width > 10)
        return timestampLocal(idx).toString("dd.MM.yyyy");
    else
        return timestampLocal(idx).toString("dd.MM.yy");
}
