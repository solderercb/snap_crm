#ifndef SDEVICEMATCH_H
#define SDEVICEMATCH_H

#include <QWidget>

class SGroupBoxEventFilter;
class QSqlQueryModel;

namespace Ui {
class SDeviceMatch;
}

class SDeviceMatch : public QWidget
{
    Q_OBJECT

signals:
    void deviceSelected(int);
public:
    explicit SDeviceMatch(QWidget *parent = nullptr);
    ~SDeviceMatch();
    void clearFields();
    void clearTable();
private:
    Ui::SDeviceMatch *ui;
    SGroupBoxEventFilter *groupBoxEventFilter;
    QSqlQueryModel* tableModel;
    QString serialNumber;
    QMap<int, QString> fieldClauses;
    QStringList fieldIds;
    QString fieldsClause;
    void findDevice();
    QString prepareFieldClause(const int, const QString &text);
public slots:
    void findByField(const int, const QString&);
    void findBySN(const QString&);
private slots:
    void deviceMatchTableDoubleClicked(QModelIndex);
    void toggleElementsVisibility();
};

#endif // SDEVICEMATCH_H
