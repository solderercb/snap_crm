#ifndef SEDITABLEBASEMODEL_H
#define SEDITABLEBASEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include "stablemodelscommonmethods.h"

class SEditableBaseModel : public QSqlTableModel, public STableModelsCommonMethods
{
    Q_OBJECT
public:
    explicit SEditableBaseModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
private slots:
    void slotModelReset();
};

#endif // SEDITABLEBASEMODEL_H
