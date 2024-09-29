#ifndef SPARTSREQUESTSGROUPINGMODEL_H
#define SPARTSREQUESTSGROUPINGMODEL_H

#include <QObject>
#include "models/sstandarditemmodel.h"
#include "models/stablebasemodel.h"
#include "widgets/stableviewbase.h"

class SPartsRequestsGroupingModel : public STableBaseModel
{
    Q_OBJECT
public:
    enum Group{No, Repair, Client, Tracking, Supplier};
    Q_ENUM(Group)
    enum Columns{Group, Selected, Total, Id, C};
    explicit SPartsRequestsGroupingModel(QObject *parent = nullptr);
    static SStandardItemModel *groupsList();
    FilterField queryClauseForSelection(const int &index);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    int id(const QModelIndex &item);
    QString name(const QModelIndex &item);
    QString urlFormat(const QString &url) const;
public slots:
    void changeGroup(int id);
private:
    int m_group = Group::No;
    void translateNames();
};

#endif // SPARTSREQUESTSGROUPINGMODEL_H
