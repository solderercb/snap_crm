#ifndef SCLIENTMATCH_H
#define SCLIENTMATCH_H

#include <QWidget>
#include "global.h"
#include "com_sql_queries.h"
#include "sgroupboxeventfilter.h"
#include "stableviewbase.h"
#include "models/ssqlfetchingmodel.h"

namespace Ui {
class SClientMatch;
}

class SClientMatch : public QWidget
{
    Q_OBJECT

signals:
    void clientSelected(int);

public:
    explicit SClientMatch(QWidget *parent = nullptr);
    ~SClientMatch();
    void clear();
private:
    Ui::SClientMatch *ui;
    SGroupBoxEventFilter *groupBoxEventFilter;
    SSqlFetchingModel* clientsMatchTable;
    QString lastName;
    QString phone;
    QString phoneMask;
    void findClient();

public slots:
    void setPhoneMask(const int index);
    void findByLastname(const QString&);
    void findByPhone(const QString&);
    void findByPhone(const QString&, const int);

private slots:
    void clientMatchTableDoubleClicked(QModelIndex);
    void toggleElementsVisibility();
};

class matchingClientsTable : public STableViewBase
{
    Q_OBJECT
public:
    enum Column{ID = 0, FullName = 1, Balance = 2, Repairs = 3, Purchases = 4, Type = 5, PrimaryPhone = 6};
    Q_ENUM(Column)
    enum ColumnWidth{ID_w = 80, FullName_w = 200, Balance_w = 80, Repairs_w = 80, Purchases_w = 80, Type_w = 40, PrimaryPhone_w = 160};
    Q_ENUM(ColumnWidth)
    explicit matchingClientsTable(QWidget *parent = nullptr);
    ~matchingClientsTable();
    void setModel(QAbstractItemModel *model) override;
private:
    SSqlFetchingModel *m_model = nullptr;
    void translateNames();
};

#endif // SCLIENTMATCH_H
