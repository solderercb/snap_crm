#ifndef SFIELDSMODEL_H
#define SFIELDSMODEL_H

#include <QObject>
#include <QList>
#include "sfieldvaluemodel.h"

/*  Специальный LineEdit для доп. полей
 *  Ретранслирует стандартный сигнал editingFinished с новым значением
 */
class AFLineEdit : public QLineEdit
{
    Q_OBJECT
signals:
    void editingFinished(const QString&);
public:
    explicit AFLineEdit(QWidget *parent = nullptr);
private slots:
    void editingFinished();
};

/*  Специальный DateEdit для доп. полей
 *  Ретранслирует стандартный сигнал dateChanged с преобразованным в QString значением
 */
class AFDateEdit : public QDateEdit
{
    Q_OBJECT
signals:
    void dateChanged(const QString&);
public:
    explicit AFDateEdit(QWidget *parent = nullptr);
    explicit AFDateEdit(QDate, QWidget *parent = nullptr);
private slots:
    void dateChanged(const QDate&);
};

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
    void remove(SFieldValueModel *phone);
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
