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
    virtual QString selectStatement() const override;
    virtual void setSelectStatement(const QString &statement);
    virtual void clear() override;
protected:
    bool insertRowIntoTable(const QSqlRecord &values) override;
private:
    QString m_selectStatement;
public Q_SLOTS:
    bool submit() override;
    bool submitAll();
};

#endif // SEDITABLEBASEMODEL_H
