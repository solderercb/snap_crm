#ifndef SCARTRIDGEMATERIALSMODEL_H
#define SCARTRIDGEMATERIALSMODEL_H

#include <SEditableBaseModel>

class SStandardItemModel;

class SCartridgeMaterialsModel : public SEditableBaseModel
{
    Q_OBJECT
signals:
    void noFurtherMaterialAddition(bool);
public:
    explicit SCartridgeMaterialsModel(QObject *parent = nullptr);
    ~SCartridgeMaterialsModel();
    static SStandardItemModel *materialsList();
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &item) const override;
    int cardId();
    void setCardId(const int id);
    bool removeRow(const int row, const QModelIndex &parent = QModelIndex());
    int findNextMaterial(const int targetRow = -1);
private:
    int m_cardId = 0;
    int m_defaultTonerWeight = 100;
    void translateNames();
    bool insertRowIntoTable(const QSqlRecord &values) override;
public slots:
    bool commit();
    bool appendRow();
    void setDefaultTonerWeight(const int weight);
private slots:
    void slotPrimeInsert(int row, QSqlRecord &record);
};

#endif // SCARTRIDGEMATERIALSMODEL_H
