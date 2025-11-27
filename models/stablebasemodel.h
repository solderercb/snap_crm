/*  Общая модель данных таблиц.
*/
#ifndef STABLEBASEMODEL_H
#define STABLEBASEMODEL_H

#include <QSqlQueryModel>
#include <STableModelCommonMethods>
#include <QObject>

class STableBaseModel : public QSqlQueryModel, public STableModelsCommonMethods
{
    Q_OBJECT
public:
    static const QRegularExpression queryPrepareRegexpPattern;
    explicit STableBaseModel(QObject *parent = nullptr);
    ~STableBaseModel();
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // STABLEBASEMODEL_H
