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
    explicit SPropertyCollection(){};
    virtual ~SPropertyCollection() = default;
};

#define PROPSTRUCT_GET(name) get_##name
#define PROPSTRUCT_SET(name) set_##name

/* Create variable for attribute */
#define PROPSTRUCT_DECLARE_ATTRIBUTE(name, defaultValue)                                                \
    public :                                                                                            \
    QString attribute_##name = QString(defaultValue);                                                   \

/* Create variable */
#define PROPSTRUCT_DECLARE_MEMBER(type, name)                                                           \
    public :                                                                                            \
    type name = type();

/* primitive type field*/
#define PROPSTRUCT_BIND_FIELD(type, name)                                                               \
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

/* CREATE AND BIND: */
/* Make primitive field and generate propertyes */
/* For example: PROPSTRUCT_FIELD(int, digit), PROPSTRUCT_FIELD(bool, flag) */
#define PROPSTRUCT_FIELD(type, name)                                                                    \
    PROPSTRUCT_DECLARE_MEMBER(type, name)                                                               \
    PROPSTRUCT_BIND_FIELD(type, name)

#endif // PROPSTRUCT_H

