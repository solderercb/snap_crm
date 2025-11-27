#include <QObject>
#include <QStandardItem>

class SStoreItemModel
{
    friend class TClassTest;
public:
    enum SaleMode {Store = 1, Repair = 2};
    explicit SStoreItemModel(QObject *parent = nullptr){Q_UNUSED(parent)};
    explicit SStoreItemModel(const QList<QStandardItem *> &, const int commitedExpense = 0, QObject *parent = nullptr){Q_UNUSED(parent)m_commitedExpense = commitedExpense;};
    ~SStoreItemModel(){};
    void setPrimaryKey(const QVariant &data){m_id = data.toInt();};
    void setSaleMode(SaleMode ){};
    // только для теста: записываем в переменную код, чтобы проверить, был ли вызван метод
    bool reserve(const QString &){m_lastOp = 1; m_commited = 1; i_logTexts.clear(); m_commitedExpense = m_newExpense; return 1;};
    bool saleRepair(const QString &){m_lastOp = 2; m_commited = 1; return 1;};
    bool unsaleRepair(const QString &){m_lastOp = 3; m_commited = 1; return 1;};
    bool free(const QString &){m_lastOp = 4; m_commited = 1; return 1;};
    bool commit(){m_commited = 1; return 1;};
    void commitLogs(){if(i_logTexts.isEmpty()) return; m_lastOp = 5; m_commited = 1;};
    void setCommitedExpense(const int count){m_commitedExpense = count;};
    void setNewExpense(const int count){m_newExpense = count;};
    void appendLogText(const QString &text, const QString &disambiguation){i_logTexts.insert(disambiguation, text);};
private:
    int m_id = 0;
    int m_commitedExpense = 0;
    int m_newExpense = 0;
    int m_lastOp = -1;
    int m_commited = 0;
    QMap<QString, QString> i_logTexts;
};
