#ifndef STABLEVIEWBOQSTORE_H
#define STABLEVIEWBOQSTORE_H
#include "widgets/stableviewbase.h"
#include "widgets/stableviewboqitemdelegates.h"
#include "models/ssaletablemodel.h"

class STableViewBOQStore : public STableViewBase
{
    Q_OBJECT
signals:
    void createTabSparePart(int);
public:
    explicit STableViewBOQStore(QWidget *parent = nullptr);
    ~STableViewBOQStore();
    void setModel(QAbstractItemModel *model) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    SSaleTableModel *m_model = nullptr;
    void clearModel() override;
    void setModelQuery(const QString &query, const QSqlDatabase &database) override;
};

#endif // STABLEVIEWBOQSTORE_H
