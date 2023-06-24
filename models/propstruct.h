/*
 Property object constructor template.

 Thanks to authors of QSerializer Agadzhanov Vladimir and Jerry Jacobs for idea.
                                                                                 */

#ifndef PROPSTRUCT_H
#define PROPSTRUCT_H

/* META OBJECT SYSTEM */
#include <QVariant>
#include <QMetaProperty>
#include <QMetaObject>
#include <QMetaType>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QDateTime>
#include <QDateTimeEdit>
#include <LimeReport>
#include <QDebug>
#include "models/sstandarditemmodel.h"
#include "models/scomsettingstypes.h"

#include <type_traits>

#define PROPSTRUCT_VERSION "1.0"

/* Generate metaObject method */
#define PROPSTRUCT_META_OBJECT_METHOD                                                                  \
    public:                                                                                            \
    virtual const QMetaObject * metaObject() const                                                     \
    {                                                                                                  \
        return &this->staticMetaObject;                                                                \
    }

#define PROPSTRUCTHEADER                                                                               \
    Q_OBJECT                                                                                           \
    PROPSTRUCT_META_OBJECT_METHOD

#define PROPSTRUCTURED PROPSTRUCT_META_OBJECT_METHOD

class SPropertyCollection : public QObject
{
    Q_OBJECT
//    PROPSTRUCT_META_OBJECT_METHOD
public:
    enum WidgetType{Label, Editor};
    explicit SPropertyCollection(){};
    virtual ~SPropertyCollection() = default;
    QMap<QString, QWidget*> i_labelWidgets;
    QMap<QString, QWidget*> i_editorWidgets;
    QMap<QString, int> i_propertyGroup;
    QMap<QString, QString> i_fieldNames;
    QMap<QString, QVariant> i_fieldModified;
    QMap<QString, QVariant> i_jsonFieldModified;
    QWidget *widget(const int propId, const WidgetType type);
    void deleteWidgets();
    int count();
    int propertyGroup(const int propId);
    int propertyId(const char *name) const;
    QStringList keys();
    QList<QVariant> values();
protected:
    int fieldToPropertyId(const QString &fieldName);
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
};

#define PROPSTRUCT_GET(name) get_##name
#define PROPSTRUCT_SET(name) set_##name
#define PROPSTRUCT_ASSERT(name) Q_ASSERT_X(i_editorWidgets.value(#name, nullptr), metaObject()->className(), "attempt to call READ method before widget initialization");

#define NEW_LABEL(name)                                                                                 \
    QLabel *l = new QLabel(#name);                                                                      \
    i_labelWidgets.insert(#name, l);                                                                    \

#define NEW_LINEEDIT                                                                                    \
    QLineEdit *w = new QLineEdit;                                                                       \

#define NEW_CHECKBOX                                                                                    \
    QCheckBox *w = new QCheckBox;                                                                       \
    w->setText("");                                                                                     \
    w->setLayoutDirection(Qt::RightToLeft);

#define NEW_COMBOBOX                                                                                    \
    QComboBox *w = new QComboBox;                                                                       \

#define NEW_SPINBOX                                                                                     \
    QSpinBox *w = new QSpinBox;                                                                         \

#define NEW_DSPINBOX                                                                                    \
    QDoubleSpinBox *w = new QDoubleSpinBox;                                                             \

#define NEW_RADIOBTN                                                                                    \
    QRadioButton *w = new QRadioButton;                                                                 \

#define NEW_DATETIMEEDIT                                                                                \
    QDateTimeEdit *w = new QDateTimeEdit;                                                               \

#define PROPSTRUCT_INIT_WIDGET(name, widget, group)                                                     \
    public Q_SLOTS:                                                                                     \
        void init_##name##_widget()                                                                     \
        {                                                                                               \
            NEW_LABEL(name)                                                                             \
            NEW_##widget                                                                                \
            i_editorWidgets.insert(#name, w);                                                           \
            i_propertyGroup.insert(#name, group);                                                       \
        };

#define PROPSTRUCT_REGISTER_DB_FIELD(varName, fieldName)                                                \
    public Q_SLOTS:                                                                                     \
        void register_##varName##_db_field()                                                            \
        {                                                                                               \
            if(QString("ignore").compare(#fieldName) == 0)                                              \
                return;                                                                                 \
                                                                                                        \
            i_fieldNames.insert(#varName, #fieldName);                                                  \
        };

#define PROPSTRUCT_GET_VALUE_FOR_REPORT(varName)                                                        \
    public Q_SLOTS:                                                                                     \
        QVariant get_##varName##_for_report()                                                           \
        {                                                                                               \
            return varName;                                                                             \
        };

/* Create variable */
#define PROPSTRUCT_DECLARE_MEMBER(type, name)                                                           \
    public :                                                                                            \
    type name = type();

/* primitive type field*/
#define PROPSTRUCT_BIND_FIELD(type, name, fieldName)                                                    \
    Q_PROPERTY(type name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                          \
    private:                                                                                            \
    type PROPSTRUCT_GET(name)() const                                                                   \
    {                                                                                                   \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(const type &var##name)                                                    \
    {                                                                                                   \
        name = QVariant(var##name).value<type>();                                                       \
    }

#define PROPSTRUCT_BIND_JSON_ARRAY(type, name, fieldName)                                               \
    Q_PROPERTY(QString name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                       \
    private:                                                                                            \
    QString PROPSTRUCT_GET(name)()                                                                      \
    {                                                                                                   \
        return name.toRawJsonArray();                                                                   \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(const QString &var##name)                                                 \
    {                                                                                                   \
        name.fromJsonArray(var##name.toLocal8Bit());                                                    \
    }

#define PROPSTRUCT_BIND_JSON_OBJ(type, name, fieldName)                                                 \
    Q_PROPERTY(QString name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                       \
    private:                                                                                            \
    QString PROPSTRUCT_GET(name)()                                                                      \
    {                                                                                                   \
        return name.toRawJson();                                                                        \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(const QString &var##name)                                                 \
    {                                                                                                   \
        name.fromJson(var##name.toLocal8Bit());                                                         \
    }

#define PROPSTRUCT_BIND_LINEEDIT(type, name, group, fieldName)                                          \
    Q_PROPERTY(QVariant name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                      \
    private:                                                                                            \
    QVariant PROPSTRUCT_GET(name)()                                                                     \
    {                                                                                                   \
        QString widgetValue = static_cast<QLineEdit*>(i_editorWidgets.value(#name))->text();            \
        if(QString("ignore").compare(#fieldName) !=0 && widgetValue.compare(name) != 0)                 \
            i_fieldModified.insert(#name, widgetValue);                                                 \
        name = widgetValue;                                                                             \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(QVariant value)                                                           \
    {                                                                                                   \
        name = value.value<type>();                                                                     \
        QLineEdit *le = dynamic_cast<QLineEdit*>(i_editorWidgets.value(#name, 0));                      \
        if(le)                                                                                          \
            le->setText(value.toString());                                                              \
    }

#define PROPSTRUCT_BIND_CHECKBOX(type, name, group, fieldName)                                          \
    Q_PROPERTY(QVariant name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                      \
    private:                                                                                            \
    QVariant PROPSTRUCT_GET(name)()                                                                     \
    {                                                                                                   \
        int widgetValue = static_cast<QCheckBox*>(i_editorWidgets.value(#name))->isChecked();           \
        if(QString("ignore").compare(#fieldName) !=0 && widgetValue != name)                            \
            i_fieldModified.insert(#name, widgetValue);                                                 \
        name = widgetValue;                                                                             \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(QVariant value)                                                           \
    {                                                                                                   \
        name = value.value<type>();                                                                     \
        QCheckBox *cb = dynamic_cast<QCheckBox*>(i_editorWidgets.value(#name, 0));                      \
        if(cb)                                                                                          \
            cb->setCheckState((Qt::CheckState)(value.toBool()?2:0));                                    \
    }

/*  Параметр, использующий виджет QComboBox, подразумевает использование SStandardItemModel в качестве модели данных.
 *  Поскольку для инициализации виджетов используется метод WRITE, нужно пресечь попытку установки currentIndex на
 *  этом этапе, т. к. модель еще не задана и программа будет падать.
*/
#define PROPSTRUCT_BIND_COMBOBOX(type, name, group, fieldName, invalidIndexValue)                       \
    Q_PROPERTY(QVariant name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                      \
    private:                                                                                            \
    QVariant PROPSTRUCT_GET(name)()                                                                     \
    {                                                                                                   \
        QComboBox *cb = dynamic_cast<QComboBox*>(i_editorWidgets.value(#name));                         \
        SStandardItemModel *mdl = dynamic_cast<SStandardItemModel*>(cb->model());                       \
        int widgetValue;                                                                                \
        if(mdl)                                                                                         \
            widgetValue = mdl->databaseIDByRow(cb->currentIndex(), 1);                                  \
        else                                                                                            \
        {                                                                                               \
            widgetValue = cb->currentIndex();                                                           \
        }                                                                                               \
        if(QString("ignore").compare(#fieldName) !=0 && widgetValue != name)                            \
            i_fieldModified.insert(#name, (widgetValue == -1)?QVariant(invalidIndexValue):widgetValue); \
        name = widgetValue;                                                                             \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(QVariant value)                                                           \
    {                                                                                                   \
        name = value.value<type>();                                                                     \
        QComboBox *cb = dynamic_cast<QComboBox*>(i_editorWidgets.value(#name, 0));                      \
        if(cb)                                                                                          \
        {                                                                                               \
            SStandardItemModel *mdl = dynamic_cast<SStandardItemModel*>(cb->model());                   \
            if(mdl)                                                                                     \
                cb->setCurrentIndex(mdl->rowByDatabaseID(value.toInt(), 1));                            \
            else                                                                                        \
                cb->setCurrentIndex(value.toInt());                                                     \
        }                                                                                               \
    }

#define PROPSTRUCT_BIND_SPINBOX(type, name, group, fieldName, min, max)                                 \
    Q_PROPERTY(QVariant name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                      \
    private:                                                                                            \
    QVariant PROPSTRUCT_GET(name)()                                                                     \
    {                                                                                                   \
        int widgetValue = static_cast<QSpinBox*>(i_editorWidgets.value(#name))->value();                \
        if(QString("ignore").compare(#fieldName) !=0 && widgetValue != name)                            \
            i_fieldModified.insert(#name, widgetValue);                                                 \
        name = widgetValue;                                                                             \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(QVariant value)                                                           \
    {                                                                                                   \
        name = value.value<type>();                                                                     \
        QSpinBox *sb = dynamic_cast<QSpinBox*>(i_editorWidgets.value(#name, 0));                        \
        if(sb)                                                                                          \
        {                                                                                               \
            sb->setMinimum(min);                                                                        \
            sb->setMaximum(max);                                                                        \
            sb->setValue(value.toInt());                                                                \
        }                                                                                               \
    }

#define PROPSTRUCT_BIND_DSPINBOX(type, name, group, fieldName, min, max)                                \
    Q_PROPERTY(QVariant name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                      \
    private:                                                                                            \
    QVariant PROPSTRUCT_GET(name)()                                                                     \
    {                                                                                                   \
        int widgetValue = static_cast<QDoubleSpinBox*>(i_editorWidgets.value(#name))->value();          \
        if(QString("ignore").compare(#fieldName) !=0 && widgetValue != name)                            \
            i_fieldModified.insert(#name, widgetValue);                                                 \
        name = widgetValue;                                                                             \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(QVariant value)                                                           \
    {                                                                                                   \
        name = value.value<type>();                                                                     \
        QDoubleSpinBox *sb = dynamic_cast<QDoubleSpinBox*>(i_editorWidgets.value(#name, 0));            \
        if(sb)                                                                                          \
        {                                                                                               \
            sb->setMinimum(min);                                                                        \
            sb->setMaximum(max);                                                                        \
            sb->setValue(value.toDouble());                                                             \
        }                                                                                               \
    }

#define PROPSTRUCT_BIND_RADIOBTN(type, name, group, fieldName)                                          \
    Q_PROPERTY(QVariant name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                      \
    private:                                                                                            \
    QVariant PROPSTRUCT_GET(name)()                                                                     \
    {                                                                                                   \
        name = static_cast<QRadioButton*>(i_editorWidgets.value(#name))->isChecked();                   \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(QVariant value)                                                           \
    {                                                                                                   \
        name = value.value<type>();                                                                     \
        QRadioButton *rb = dynamic_cast<QRadioButton*>(i_editorWidgets.value(#name, 0));                \
        if(rb)                                                                                          \
            rb->setChecked(name);                                                                       \
    }

#define PROPSTRUCT_BIND_DATETIMEEDIT(type, name, group, fieldName)                                      \
    Q_PROPERTY(QVariant name READ PROPSTRUCT_GET(name) WRITE PROPSTRUCT_SET(name))                      \
    private:                                                                                            \
    QVariant PROPSTRUCT_GET(name)()                                                                     \
    {                                                                                                   \
        QDateTime widgetValue = static_cast<QDateTimeEdit*>(i_editorWidgets.value(#name))->dateTime();  \
        if(QString("ignore").compare(#fieldName) !=0 && widgetValue != name)                            \
            i_fieldModified.insert(#name, widgetValue);                                                 \
        name = widgetValue;                                                                             \
        return name;                                                                                    \
    }                                                                                                   \
    void PROPSTRUCT_SET(name)(QVariant value)                                                           \
    {                                                                                                   \
        name = value.value<type>();                                                                     \
        QDateTimeEdit *dt = dynamic_cast<QDateTimeEdit*>(i_editorWidgets.value(#name, 0));              \
        if(dt)                                                                                          \
            dt->setDateTime(name);                                                                      \
    }

/* CREATE AND BIND: */
/* Make primitive field and generate propertyes */
/* For example: PROPSTRUCT_FIELD(int, digit), PROPSTRUCT_FIELD(bool, flag) */
#define PROPSTRUCT_FIELD(type, name, fieldName)                                                         \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_FIELD(type, name, fieldName)                                                        \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#define PROPSTRUCT_JSON_ARRAY(type, name, fieldName)                                                    \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_JSON_ARRAY(type, name, fieldName)                                                   \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)

#define PROPSTRUCT_JSON_OBJ(type, name, fieldName)                                                      \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_JSON_OBJ(type, name, fieldName)                                                     \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)

#define PROPSTRUCT_LINEEDIT(type, name, group, fieldName)                                               \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_LINEEDIT(type, name, group, fieldName)                                              \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_INIT_WIDGET(name, LINEEDIT, group)                                                       \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#define PROPSTRUCT_CHECKBOX(type, name, group, fieldName)                                               \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_CHECKBOX(type, name, group, fieldName)                                              \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_INIT_WIDGET(name, CHECKBOX, group)                                                       \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#define PROPSTRUCT_COMBOBOX(type, name, group, fieldName, invalidIndexValue)                            \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_COMBOBOX(type, name, group, fieldName, invalidIndexValue)                           \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_INIT_WIDGET(name, COMBOBOX, group)                                                       \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

//#define PROPSTRUCT_CHECKABLECOMBOBOX(type, name, group, fieldName)                                      \
//    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
//    PROPSTRUCT_BIND_CHECKABLECOMBOBOX(type, name, group, fieldName)                                     \
//    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
//    PROPSTRUCT_INIT_WIDGET(name, CHECKABLECOMBOBOX, group)                                              \
//    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

//#define PROPSTRUCT_COLORPICKER(type, name, group, fieldName)                                            \
//    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
//    PROPSTRUCT_BIND_COLORPICKER(type, name, group, fieldName)                                           \
//    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
//    PROPSTRUCT_INIT_WIDGET(name, COLORPICKER, group)                                                    \
//    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#define PROPSTRUCT_SPINBOX(type, name, group, fieldName, min, max)                                      \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_SPINBOX(type, name, group, fieldName, min, max)                                     \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_INIT_WIDGET(name, SPINBOX, group)                                                        \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#define PROPSTRUCT_DSPINBOX(type, name, group, fieldName, min, max)                                     \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_DSPINBOX(type, name, group, fieldName, min, max)                                    \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_INIT_WIDGET(name, DSPINBOX, group)                                                       \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#define PROPSTRUCT_RADIOBTN(type, name, group, fieldName)                                               \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_RADIOBTN(type, name, group, fieldName)                                              \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_INIT_WIDGET(name, RADIOBTN, group)                                                       \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#define PROPSTRUCT_DATETIMEEDIT(type, name, group, fieldName)                                           \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_DATETIMEEDIT(type, name, group, fieldName)                                          \
    PROPSTRUCT_REGISTER_DB_FIELD(name, fieldName)                                                       \
    PROPSTRUCT_INIT_WIDGET(name, DATETIMEEDIT, group)                                                   \
    PROPSTRUCT_GET_VALUE_FOR_REPORT(name)

#endif // PROPSTRUCT_H

