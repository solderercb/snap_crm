#include <QObject>
#include <QStandardItem>

class SStoreItemModel
{
    friend class TClassTest;
public:
    enum SaleMode {Store = 1, Repair = 2};
    explicit SStoreItemModel(QObject *parent = nullptr){Q_UNUSED(parent)};
    explicit SStoreItemModel(const QList<QStandardItem *> &, const int commitedExpense = 0, QObject *parent = nullptr){Q_UNUSED(parent) m_commitedExpense = commitedExpense;};
    ~SStoreItemModel(){};
    void setSaleMode(SaleMode ){};
    // только для теста: записываем в переменную код, чтобы проверить, был ли вызван метод
    bool reserve(const QString &logText){m_logText = logText; m_lastOp = 1; m_commited = 1; return 1;};
    bool saleRepair(const QString &logText){m_logText = logText; m_lastOp = 2; m_commited = 1; return 1;};
    bool unsaleRepair(const QString &logText){m_logText = logText; m_lastOp = 3; m_commited = 1; return 1;};
    bool free(const QString &logText){m_logText = logText; m_lastOp = 4; m_commited = 1; return 1;};
    bool sale(const QString &logText){m_logText = logText; m_lastOp = 5; m_commited = 1; return 1;};
    bool saleStore(const QString &logText){m_logText = logText; m_lastOp = 6; m_commited = 1; return 1;};
    bool unsale(const QString &logText){m_logText = logText; m_lastOp = 7; m_commited = 1; return 1;};
    bool commit(){m_commited = 1; return 1;};
private:
    int m_commitedExpense;
    int m_lastOp = -1;
    int m_commited = 0;
    QString m_logText;
};
