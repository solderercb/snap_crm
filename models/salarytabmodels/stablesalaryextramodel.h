#ifndef STABLESALARYEXTRAMODEL_H
#define STABLESALARYEXTRAMODEL_H

#include <SEditableBaseModel>

class STableSalaryExtraModel : public SEditableBaseModel
{
    Q_OBJECT
public:
    enum Columns {Id = 0, Name = 1, Price = 2, PaymentDate = 3, User = 4};
    explicit STableSalaryExtraModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setEmployeeId(const int id);
private:
    int m_employee = 0;
public slots:
    void addNewRow();
    void saveTable();
private slots:
    void slotPrimeInsert(int row, QSqlRecord &record);
};

#endif // STABLESALARYEXTRAMODEL_H
