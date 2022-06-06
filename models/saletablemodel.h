#ifndef SALETABLEMODEL_H
#define SALETABLEMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QMap>
#ifdef QT_DEBUG
#include <QRandomGenerator>
#endif
#include "../global.h"
#include "../com_sql_queries.h"

class SaleTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit SaleTableModel(QObject *parent = nullptr);
    ~SaleTableModel();
    QVariant value(int, int) const;
    QVariant value(int, QString) const;
    QVariant value(int, QString, int role) const;
    void setModelState(int state) {modelState = state;};
    int getModelState() {return modelState;};
    void setQuery(const QString&, const QSqlDatabase &db = QSqlDatabase());
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    bool insertRecord(int, const QSqlRecord&);
    bool appendRecord(const QSqlRecord&);
    bool removeRowHandler(int, int);
    QMap<int, int>* getPendingRemoveList();
    bool revertMode();
    void setHorizontalHeaderLabels(const QStringList &labels);
    double totalAmount();
    QMap<QString, int> *fields;
    void setPriceColumn(QSqlQuery*);

//    void setDocument(int);

signals:
    void dataChanged();
    void amountChanged(float);
    void modelReset();

private:
    QSqlQueryModel *queryData;
    int modelState = 0; // 0 - новая РН (всё редактируется, строки удаляются); 1 - проведённая РН (не редактируется, строки помечаются серым); 2 - резерв (всё редактируется, строки помечаются серым); 3 - распроведённая РН или снятый резерв (не редактируется, кнопок в первом столбце нет)
    QMap<int, int> *pendingRemoveList;
    QString price_field_name = "`price2`";
    bool amountChangedSignalFilter = 0;
    int countCol = 3;
    int priceCol = 5;
    int availCol = 6;
    int summCol = 6;

    // названия столбцов по-умолчанию; подробнее см. в комментарии к методу SaleTableModel::setHorizontalHeaderLabels
    QStringList fieldsDep = {"id", "UID", "name", "count", "avail", "price", "summ", "box", "sn", "warranty", "is_realization", "return_percent", "is_cancellation", "cancellation_reason", "item_id", "in_price", "dealer"};

private slots:
    void sqlDataChanged();
#if QT_VERSION >= 0x060000
    void dataChanaged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>());
#else
    void dataChanaged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>());
#endif

};

#endif // SALETABLEMODEL_H
