#ifndef SSTANDARDITEMMODEL_H
#define SSTANDARDITEMMODEL_H

#include <QObject>

class SStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit SStandardItemModel(QObject *parent = nullptr);
    ~SStandardItemModel();
    int getFieldIdByName(const QString &field);
    QString getDisplayRole(int id, int column);
    QString getDisplayRole(int id, QString field = "id");

signals:

};

#endif // SSTANDARDITEMMODEL_H
