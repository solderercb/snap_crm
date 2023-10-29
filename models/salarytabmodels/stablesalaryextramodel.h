#ifndef STABLESALARYEXTRAMODEL_H
#define STABLESALARYEXTRAMODEL_H

#include "../seditablebasemodel.h"
#include "sadditionalpaymentmodel.h"
#include "../../squerylog.h"
#include "widgets/shortlivednotification.h"

#include <QObject>
#include <QSqlField>

class STableSalaryExtraModel : public SEditableBaseModel
{
    Q_OBJECT
signals:
    void repopulate();
public:
    explicit STableSalaryExtraModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setEmployeeId(const int id);
private:
    int m_employee = 0;
    SAdditionalPaymentModel *additionalPayment;
    SQueryLog *m_queryLog;
public slots:
    void addNewRow();
    void saveTable();
private slots:
    void slotPrimeInsert(int row, QSqlRecord &record);
};

#endif // STABLESALARYEXTRAMODEL_H
