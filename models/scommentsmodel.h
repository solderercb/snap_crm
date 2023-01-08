#ifndef SCOMMENTSMODEL_H
#define SCOMMENTSMODEL_H

#include <QObject>
#include "sstandarditemmodel.h"
#include "scommentmodel.h"

class SCommentsModel : public SStandardItemModel
{
    Q_OBJECT
signals:
public:
    explicit SCommentsModel(QObject *parent = nullptr);
    ~SCommentsModel();
    void load(const int objId);
    bool add(const QString&);
    bool edit(const int commentId, const QString&);
    bool remove(const int row);
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    int mode();
    void setMode(const int);
    void setObjId(const int);
    bool isColumnHidden(const int);
    int recordId(const int row);
    int userId(const int row);
    QDateTime created(const int row) const;
    QString text(const int row);
    bool setText(const int row, const QString &text);

private:
    QSqlQueryModel *m_queryData;
    int m_mode = SCommentModel::Repair;
    int m_objId = 0;
    int m_hiddenColumns = 0xFFF1;
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
    QList<QStandardItem *> row(int) const;
    QString createdStr(const QModelIndex &index) const;
    void clearChangedFlagForAllField();
private slots:
    void sqlDataChanged();
};

#endif // SCOMMENTSMODEL_H
