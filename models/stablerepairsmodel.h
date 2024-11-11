#ifndef STABLEREPAIRSMODEL_H
#define STABLEREPAIRSMODEL_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QColor>
#include <QStandardItemModel>
#include <QStandardItem>
#include "models/ssqlfetchingmodel.h"
#include "models/stablemodelscommonmethods.h"
#include "global.h"

class STableRepairsModel : public SSqlFetchingModel, public STableModelsCommonMethods
{
    Q_OBJECT
public:
    enum Columns {Marks = 0, Id = 1, Title = 2, SerialNumber = 3, Fault = 4, Master = 5, Status = 6, RealRepairCost = 7, InDate = 8,
                  OutDate = 9, Client = 10, Phone = 11, Box = 12, CurrentManager = 13, Progress = 14, ExtNotes = 15, DevClassName = 16,
                  Device = 17, Office = 18, State = 19, NewState = 20, UserLock = 21, LockDatetime = 22, QuickRepair = 23, ThirsPartySc = 24,
                  LastSave = 25, LastStatusChanged = 26, WarrantyDays = 27, Color = 28, Early = 29, ExtEarly = 30, IssuedMsg = 31,
                  VendorId = 32, Termscontrol = 33, Hidden = 34, Company = 35, InformedStatus = 36, ClientId = 37, ClientShortName = 38};
    Q_ENUM(Columns)
    explicit STableRepairsModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void initDemo();
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
private:
    QModelIndex indexForShortData(const QModelIndex &index) const override;
    QVariant clientName(const QModelIndex &index) const;
    QVariant dateTime(const QModelIndex &index) const;
};

#endif // STABLEREPAIRSMODEL_H
