#ifndef SPARTSREQUESTSGROUPINGMODEL_H
#define SPARTSREQUESTSGROUPINGMODEL_H

#include <STableModelBase>
#include <STableViewBase>

class SStandardItemModel;

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
    static QString urlFormat(const QString &url);
public slots:
    void changeGroup(int id);
private:
    int m_group = Group::No;
    void translateNames();
};

#endif // SPARTSREQUESTSGROUPINGMODEL_H
