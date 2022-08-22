#ifndef SCLIENTMATCH_H
#define SCLIENTMATCH_H

#include <QWidget>
#include "global.h"
#include "com_sql_queries.h"
#include "sgroupboxeventfilter.h"

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

private:
    Ui::SClientMatch *ui;
    SGroupBoxEventFilter *groupBoxEventFilter;
    QSqlQueryModel* clientsMatchTable;
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

#endif // SCLIENTMATCH_H
