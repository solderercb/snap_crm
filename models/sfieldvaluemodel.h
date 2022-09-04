#ifndef SFIELDVALUEMODEL_H
#define SFIELDVALUEMODEL_H

#include "scomrecord.h"
#include <QObject>
#include <QSqlRecord>

class SFieldValueModel : public SComRecord
{
    Q_OBJECT
signals:
    void emptied(SFieldValueModel*);
public:
    enum WidgetType {LineEdit = 1, ComboBox, DateEdit, dummy};
    explicit SFieldValueModel(QObject *parent = nullptr);
    explicit SFieldValueModel(const int, QObject *parent = nullptr);
    ~SFieldValueModel();
    void load(const int);
    void load(QSqlRecord);
    QWidget *widget();
    QWidget *createWidget(const QSqlRecord&);
    void setDefaultStyleSheet();
    bool validate();
    void deleteWidget();
    int id();
    void setId(const int);
    int fieldId();
    void setFieldId(const int);
    int repairId();
    void setRepairId(const int);
    int itemId();
    void setItemId(const int);
    QString name();
    QString value();
    bool commit();
    bool delDBRecord();
    bool isValid();
private:
    QWidget *createLineEdit(const QSqlRecord&);
    QWidget *createComboBox(const QSqlRecord&);
    QWidget *createDateTime(const QSqlRecord&);
    QWidget *createDummyWidget(const QSqlRecord&);
    int m_id;
    int m_fieldId;
    QString m_name;
    int m_repairId;
    int m_itemId;
    QString m_value;
    bool m_isValid = 0;
    QWidget *m_widget = nullptr;
public slots:
    void setValue(const QString &);
};

#endif // SFIELDVALUEMODEL_H
