#ifndef STABLEVIEWCLIENTS_H
#define STABLEVIEWCLIENTS_H

#include <STableViewBase>

class QWidget;
class STableBaseModel;

class STableViewClients : public STableViewBase
{
    Q_OBJECT
public:
    enum Column{ID = 0, FullName = 1, Balance = 2, Repairs = 3, Purchases = 4, Type = 5, PrimaryPhone = 6, EMail = 7};
    Q_ENUM(Column)
    enum ColumnWidth{ID_w = 50, FullName_w = 700, Balance_w = 60, Repairs_w = 60, Purchases_w = 60, Type_w = 40, PrimaryPhone_w = 120, EMail_w = 150};
    Q_ENUM(ColumnWidth)
    explicit STableViewClients(QWidget *parent = nullptr);
    ~STableViewClients();
    void setModel(QAbstractItemModel *model) override;
private:
    STableBaseModel *m_model = nullptr;
    void setColumnWidth(int column, int width) override;
    void translateNames();
};

#endif // STABLEVIEWCLIENTS_H
