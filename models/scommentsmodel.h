#ifndef SCOMMENTSMODEL_H
#define SCOMMENTSMODEL_H

#include <QObject>
#include "sstandarditemmodel.h"
#include "scommentmodel.h"

class SCommentsModel : public SStandardItemModel
{
    Q_OBJECT
signals:
    void modelReset();
public:
    explicit SCommentsModel(QObject *parent = nullptr);
    ~SCommentsModel();
    void load(const int repairId);
    bool add(const QString&);
    bool edit(const int commentId, const QString&);
    QString text(const int commentId);
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
private:
    QSqlQueryModel *queryData;
    QMap<QString, int> *fields;
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
private slots:
    void sqlDataChanged();
};

#endif // SCOMMENTSMODEL_H
