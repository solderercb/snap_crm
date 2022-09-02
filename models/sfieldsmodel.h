#ifndef SFIELDSMODEL_H
#define SFIELDSMODEL_H

#include <QObject>
#include <QList>
#include <QHash>
#include "sfieldvaluemodel.h"

class SFieldsModel : public QObject
{
    Q_OBJECT
public:
    enum Type {Item = 0, Repair = 1};
    enum WidgetType {LineEdit = 1, ComboBox, DateEdit, dummy};
    explicit SFieldsModel(Type type, QObject *parent = nullptr);
    ~SFieldsModel();
    QList<SFieldValueModel*> list();
    QList<QWidget*> widgetsList();
    QMap<QWidget*, SFieldValueModel*> widgetFieldMap;
    bool initWidgets(const int);
    QWidget *createLineEdit(const QSqlRecord&, SFieldValueModel*);
    QWidget *createComboBox(const QSqlRecord&, SFieldValueModel*);
    QWidget *createDateTime(const QSqlRecord&, SFieldValueModel*);
    QWidget *createDummyWidget(const QSqlRecord&, SFieldValueModel*);
    bool load(int);
    void add(SFieldValueModel*);
    void remove(SFieldValueModel *field);
    bool isEmpty();
    void setRepair(const int id);
    void setItem(const int id);
    bool commit();
    void clear();
    bool isUpdated();
    void markUpdated();
    bool validate();
    void resetIds();
private:
    QSqlQuery *query;
    QList<SFieldValueModel*> m_removeList;
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
