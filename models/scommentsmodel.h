#ifndef SCOMMENTSMODEL_H
#define SCOMMENTSMODEL_H

#include <QSqlDatabase>
#include <SStandardItemModel>
#include <STableModelCommonMethods>

class SCommentModel;

class SCommentsModel : public SStandardItemModel, public STableModelsCommonMethods
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
    void clear();
private:
    QSqlQueryModel *m_queryData;
    int m_mode;
    int m_objId = 0;
    int m_hiddenColumns = 0xFFF1;
    std::shared_ptr<SSingleRowJModel> singleRowModel(const int row) override;
    void initSingleRowModel(const int row, std::shared_ptr<SSingleRowJModel> model);
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
    QString createdStr(const QModelIndex &index) const;
private slots:
    void sqlDataChanged();
};

#endif // SCOMMENTSMODEL_H
