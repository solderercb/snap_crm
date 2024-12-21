#ifndef SCLIENTMATCH_H
#define SCLIENTMATCH_H

#include <QWidget>
#include "sgroupboxeventfilter.h"
#include "stableviewbase.h"
#include "models/ssqlfetchingmodel.h"
#include "models/sclientmodel.h"

namespace Ui {
class SClientMatch;
}

class STableClientMatchModel : public SSqlFetchingModel, public STableModelsCommonMethods
{
    Q_OBJECT
public:
    enum Columns {Id = 0, FullName = 1, Balance = 2, Repairs = 3, Purchases = 4, Type = 5, Phone = 6, ShortName = 7};
    Q_ENUM(Columns)
    explicit STableClientMatchModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
public slots:
private:
    QModelIndex indexForShortData(const QModelIndex &index) const override;
};

class SClientMatch : public QWidget
{
    Q_OBJECT

signals:
    void clientSelected(int);

public:
    enum NameSearchScope{LastName = 0, Name = 1, Both = 2};
        explicit SClientMatch(QWidget *parent = nullptr);
    ~SClientMatch();
    void clear();
    void setClientType(int type);
    void setCategory(int category);
    void setEnabled(bool state = true);
private:
    Ui::SClientMatch *ui;
    SGroupBoxEventFilter *groupBoxEventFilter;
    STableClientMatchModel *clientsMatchTable;
    QString lastName;
    QString companyName;
    QString phone;
    QString phoneMask;
    int clientType = 0;
    int m_category = SClientModel::Categories::All;
    bool m_enabled = 1;
    void findClient();
public slots:
    void setPhoneMask(const int index);
    void findByName(const QString&);
    void findByPhone(const QString&);
    void findByPhone(const QString&, const int);

private slots:
    void clientMatchTableDoubleClicked(QModelIndex);
    void toggleElementsVisibility();
};

class STableViewClientMatch : public STableViewBase
{
    Q_OBJECT
public:
    enum Column{ID = 0, FullName = 1, Balance = 2, Repairs = 3, Purchases = 4, Type = 5, PrimaryPhone = 6};
    Q_ENUM(Column)
    enum ColumnWidth{ID_w = 80, FullName_w = 200, Balance_w = 80, Repairs_w = 80, Purchases_w = 80, Type_w = 40, PrimaryPhone_w = 160};
    Q_ENUM(ColumnWidth)
    explicit STableViewClientMatch(QWidget *parent = nullptr);
    ~STableViewClientMatch();
    void setModel(QAbstractItemModel *model) override;
    void clearModel() override;
    void setModelQuery(const QString &query, const QSqlDatabase &database) override;
private:
    STableClientMatchModel *m_model = nullptr;
    void setColumnWidth(int column, int width) override;
    void translateNames();
};

#endif // SCLIENTMATCH_H
