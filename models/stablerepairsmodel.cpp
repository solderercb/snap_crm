#include "stablerepairsmodel.h"
#include "widgets/stableviewrepairs.h"

STableRepairsModel::STableRepairsModel(QObject *parent) : STableBaseModel(parent)
{
}

QVariant STableRepairsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    // TODO: изучить разницу в скорости выполнения:
    //  1) определение нужного столбца путём вызова кучи методов (до rev277)
    //  2) использование enum (с rev277)
    //  3) кэширование названий полей БД по сигналу resetModel() (слот, подключенный в классе STableBaseModel вызывается раньше слота, подключенного в классе виджета STableViewBase)
    if (role == Qt::BackgroundRole)
    {
        if(index.column() == Columns::Status)
            return QColor(comSettings->repairStatuses[QSqlQueryModel::data(index, Qt::DisplayRole).toInt()].Color);

        QString rowColor = STableBaseModel::data(index.siblingAtColumn(Columns::Color)).toString();
        if(!rowColor.isEmpty())
            return QColor(rowColor);
    }
    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case Columns::Status:           return comSettings->repairStatuses[QSqlQueryModel::data(index, Qt::DisplayRole).toInt()].Name;
            case Columns::RealRepairCost:   return dataLocalizedFromDouble(index);
            case Columns::InDate:
            case Columns::OutDate:          return dateTime(index);
            case Columns::Master:
            case Columns::CurrentManager:   return userFromId(index);
            case Columns::Box:              return QVariant(repairBoxesModel->getDisplayRole(QSqlQueryModel::data(index, role).toInt()));
            case Columns::Id:               return QString("%1-%2").arg(record(index.row()).value("office").toString().rightJustified(3, '0')).arg(QSqlQueryModel::data(index).toString().rightJustified(6, '0'));
            case Columns::Client:           return clientName(index);
            case Columns::Phone:            if(!permissions->viewClients) return tr("no permissions");
            default: ;
        }
    }
    return STableBaseModel::data(index, role);
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
    QRegularExpression re("( +\n)|((;?) +$)");  // удаление пробелов-заполнителей в конце строк, а также точки с запятой в конце запроса (при наличии ; не будет работать сортировка)
    demoQuery.replace(re, "\n");
    demoQuery  += " WHERE t1.`id` IS NULL";

    QSqlQueryModel::setQuery(demoQuery, QSqlDatabase::database("connMain"));

    for(int r = 0; r < demoIds.count(); r++)
    {
        demoConstsForUnion.clear();
        for(int c = 0; c < query().record().count(); c++)
        {
            switch (c)
            {
                case Columns::Id:           demoConstsForUnion.append(demoIds.at(r)); break;
                case Columns::Title:        demoConstsForUnion.append(demoTitles.at(r)); break;
                case Columns::SerialNumber: demoConstsForUnion.append(demoSNs.at(r)); break;
                case Columns::Fault:        demoConstsForUnion.append(demoFaults.at(r)); break;
                case Columns::InDate:       demoConstsForUnion.append(QDateTime::currentDateTime().toString("yyyy.MM.dd HH:mm")); break;
                case Columns::Client:       demoConstsForUnion.append(demoClients.at(r)); break;
                case Columns::Office:       demoConstsForUnion.append(QString::number(userDbData->currentOffice)); break;
                case Columns::ExtNotes:     demoConstsForUnion.append(demoExtNotes.at(r)); break;
                default:                    demoConstsForUnion.append("<data>");
            }
        }
        demoQuery += "\nUNION ALL\n" \
                     "SELECT '" + demoConstsForUnion.join("', '") + "'";
    }

    QSqlQueryModel::setQuery(demoQuery, QSqlDatabase::database("connMain"));
}

void STableRepairsModel::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = 0; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = query().record().fieldName(info.index).toLatin1(); break;
        case LimeReport::CallbackInfo::ColumnData: data = index(info.index, field(info.columnName)).data(); break;
        case LimeReport::CallbackInfo::ColumnCount: data = columnCount(); break;
        case LimeReport::CallbackInfo::RowCount: data = rowCount(); break;
    }
}

QVariant STableRepairsModel::clientName(const QModelIndex &idx) const
{
    if(!permissions->viewClients)
        return tr("no permissions");

    QString value;

    // полное имя (название организации)
    value = STableBaseModel::data(idx).toString();
    if(value.length() < i_columnWidths[idx.column()])
        return value;

    // короткое имя
    value = STableBaseModel::data(index(idx.row(), Columns::ClientShortName)).toString();
    if(!value.isEmpty() && value.length() < i_columnWidths[idx.column()])
        return value;

    return STableBaseModel::data(idx);
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
