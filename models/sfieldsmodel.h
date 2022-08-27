#ifndef SFIELDSMODEL_H
#define SFIELDSMODEL_H

#include <QObject>
#include <QList>
#include "sfieldvaluemodel.h"

class SFieldsModel : public QObject
{
    Q_OBJECT
public:
    enum Type {LineEdit = 1, ComboBox, DateEdit, dummy};
    explicit SFieldsModel(bool type, QObject *parent = nullptr);
    ~SFieldsModel();
    QList<SFieldValueModel*> list();
    QList<QWidget*> widgetsList();
    bool initWidgets(const int);
    QWidget *createLineEdit(const QSqlRecord&);
    QWidget *createComboBox(const QSqlRecord&);
    QWidget *createDateTime(const QSqlRecord&);
    QWidget *createDummyWidget(const QSqlRecord&);
    bool load(int);
    void add(SFieldValueModel*);
    void remove(SFieldValueModel *phone);
    bool isEmpty();
    void setRepair(const int id);
    void setItem(const int id);
    bool commit();
    void clear();
    bool isUpdated();
    void markUpdated();
private:
    QSqlQuery *query;
    QList<SFieldValueModel*> m_fieldsList;
    QList<SFieldValueModel*> m_removeList;
    QList<QWidget*> m_widgetsList;
    SFieldValueModel* itemHandler(const QSqlRecord &phone = QSqlRecord());
    SLogRecordModel *logRecord = nullptr;
    int m_repair = 0;
    bool m_item = 0;
    bool m_isRepair = 0;
    void deleteWidgets();
    void deleteFields();

signals:

};

#endif // SFIELDSMODEL_H
