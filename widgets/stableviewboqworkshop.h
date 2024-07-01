#ifndef STABLEVIEWBOQWORKSHOP_H
#define STABLEVIEWBOQWORKSHOP_H
#include "widgets/stableviewbase.h"
#include "models/ssaletablemodel.h"

class STableViewBOQWorkshop : public STableViewBase
{
    Q_OBJECT

signals:
    void createTabSparePart(int);
    void createTabSparePartReserve(int);
public:
    explicit STableViewBOQWorkshop(QWidget *parent = nullptr);
    ~STableViewBOQWorkshop();
    void setModel(QAbstractItemModel *model) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    SSaleTableModel *m_model = nullptr;
private slots:
#if QT_VERSION >= 0x060000
    void dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>()) override;
#else
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>()) override;
#endif
};

#endif // STABLEVIEWBOQWORKSHOP_H
