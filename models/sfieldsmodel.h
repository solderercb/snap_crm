#ifndef SFIELDSMODEL_H
#define SFIELDSMODEL_H

#include <QObject>
#include <QList>
#include <QHash>
#include "sfieldvaluemodel.h"

class SFieldsModel : public QObject
{
    Q_OBJECT
signals:
public:
    enum Type {Item = 0, Repair = 1};
    enum WidgetType {LineEdit = 1, ComboBox, DateEdit, dummy};
    explicit SFieldsModel(Type type, QObject *parent = nullptr);
    ~SFieldsModel();
    QList<SFieldValueModel*> list();
    bool init(const int);
    bool load(int);
    void add(SFieldValueModel*);
    bool isEmpty();
    void setObjectId(const int id);
    bool commit();
    void clear();
    bool isUpdated();
    void markUpdated();
    bool validate();
    void resetIds();
    void enableEdit();
private:
    QSqlQuery *query;
    QList<SFieldValueModel*> m_fieldsList;
    QList<SFieldValueModel*> m_removeList;
    SFieldValueModel* itemHandler(const QSqlRecord &phone = QSqlRecord());
    SLogRecordModel *logRecord = nullptr;
    bool m_type = 0;
    bool m_nErr = 1;
    void deleteWidgets();
    void deleteFields();
public slots:
    void remove(SFieldValueModel *field);
};

#endif // SFIELDSMODEL_H
