#ifndef SFIELDSMODEL_H
#define SFIELDSMODEL_H

#include <QObject>
#include <QList>
#include <QHash>
#include "sfieldvaluemodel.h"

class SFieldsModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ reportFieldName)
    Q_PROPERTY(QString value READ reportFieldValue)
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
    int printableFieldsCount();
    QString reportFieldName();
    QString reportFieldValue();
    void initDemo();
#ifdef QT_DEBUG
    void randomFill();
#endif
private:
    QSqlQuery *query;
    QList<SFieldValueModel*> m_fieldsList;
    QList<SFieldValueModel*> m_removeList;
    SFieldValueModel* itemHandler(const QSqlRecord &phone = QSqlRecord());
    SLogRecordModel *logRecord = nullptr;
    bool m_type = 0;
    bool m_nErr = 1;
    qsizetype m_reportFieldIndex = -1;
    void deleteWidgets();
    void deleteFields();
public slots:
    void remove(SFieldValueModel *field);
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
};

#endif // SFIELDSMODEL_H
