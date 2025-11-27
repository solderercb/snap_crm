#ifndef SCLIENTINPUTFORMBASE_H
#define SCLIENTINPUTFORMBASE_H

#include <SWidget>

class SClientModel;

class SClientInputFormBase : public SWidget
{
    Q_OBJECT
public:
    explicit SClientInputFormBase(QWidget *parent = nullptr);
    void setClientsTabTitle(const QString &title);
    void setClientsTabDefaultCategory(int category);
    virtual void setMatchWidgetFilter(int category) = 0;
protected:
    QString i_clientsTabTitle;
    int i_clientsTabDefaultCategory;
public slots:
    virtual void fillCreds(int id);
#ifdef QT_DEBUG
    static int randomClientIdFromDB(const QString &filterField = QString(), QVariant filterValue = QVariant());
#endif
};

#endif // SCLIENTINPUTFORMBASE_H
