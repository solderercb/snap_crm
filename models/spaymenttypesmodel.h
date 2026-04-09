#ifndef SPAYMENTTYPESMODEL_H
#define SPAYMENTTYPESMODEL_H

#include <SSortFilterProxyModel>
#include <QConcatenateTablesProxyModel>

class SStandardItemModel;

class SPaymentTypesModel : public SSortFilterProxyModel
{
    Q_OBJECT
public:
    enum Type {PKO = 0, RKO = 1, All = -1};
    enum ExtraTypeFields {Client = 3, Amount = 6, Reason = 7, PaymenSystem = 10};
    explicit SPaymentTypesModel(QObject *parent = nullptr);
    explicit SPaymentTypesModel(const int type, QObject *parent = nullptr);
    ~SPaymentTypesModel();
    void loadExtraTypes(const int type, const bool loadArchive);
    void loadExtraTypes(const bool loadArchive = true);
    void filterTypes(std::vector<int> filter);
    QVariant dataByDatabaseId(int id, int role = Qt::DisplayRole);
    QString reasonByDatabaseId(int id);
    int clientByDatabaseId(int id);
    int paymentSystemByDatabaseId(int id);
    double defaultAmountByDatabaseId(int id);
    void setSourceModel(QAbstractItemModel *model) override;
private:
    QConcatenateTablesProxyModel *m_fullModel;
    SStandardItemModel *m_predefTypes;
    std::unique_ptr<QSqlQueryModel> m_extraTypes;
    int m_type = Type::All;
    QModelIndex extraTypeModelIndex(const int typeId);
};

#endif // SPAYMENTTYPESMODEL_H
