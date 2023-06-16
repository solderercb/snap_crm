/*
 MIT License

 Copyright (c) 2020-2021 Agadzhanov Vladimir
 Portions Copyright (c) 2021 Jerry Jacobs

 Adopted to project SNAP CRM

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
                                                                                 */

#ifndef QSERIALIZER_H
#define QSERIALIZER_H

/* TYPES */
#include <QSize>
#include <QPoint>

/* JSON */
#ifdef QS_HAS_JSON
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#endif

/* XML */
#ifdef QS_HAS_XML
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#endif

/* META OBJECT SYSTEM */
#include <QVariant>
#include <QMetaProperty>
#include <QMetaObject>
#include <QMetaType>

#include <type_traits>
#include <QDebug>

#define QS_VERSION "1.3.1"

/* Generate metaObject method */
#define QS_META_OBJECT_METHOD                                                                          \
    virtual const QMetaObject * metaObject() const                                                     \
    {                                                                                                  \
        return &this->staticMetaObject;                                                                \
    }

#define QSERIALIZABLE                                                                                  \
    Q_GADGET                                                                                           \
    QS_META_OBJECT_METHOD

/* Mark class as serializable */
#define QS_SERIALIZABLE QS_META_OBJECT_METHOD

#ifdef QS_HAS_XML
Q_DECLARE_METATYPE(QDomNode)
Q_DECLARE_METATYPE(QDomElement)
#endif

static QString tmp_indent;

class QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
protected:
    virtual inline QString nodeTagName() const
    {
        return metaObject()->className();
    };
    virtual inline bool isIdentifiedByTag() const
    {
        return 0;
    };
    virtual inline QString childNodeTagName() const
    {
        return "property";
    };
public:
    virtual ~QSerializer() = default;

#ifdef QS_HAS_JSON
    /*! \brief  Convert QJsonValue in QJsonDocument as QByteArray. */
    static QByteArray toByteArray(const QJsonValue & value)
    {
        if(value.isObject())
            return QJsonDocument(value.toObject()).toJson();

        if(value.isArray())
        {
            QStringList l;
            for(int i = 0; i < value.toArray().count(); i++)
            {
                l.append(toByteArray(value.toArray().at(i)));
            }
            return l.join(',').append(']').prepend('[').toLocal8Bit();
        }

        if(value.isString())
            return value.toString().toLocal8Bit();
    }
#endif

#ifdef QS_HAS_XML
    /*! \brief  Convert QDomNode in QDomDocument as QByteArray. */
    static QByteArray toByteArray(const QDomNode & value)
    {
        QDomDocument doc = value.toDocument();
        return doc.toByteArray();
    }

    /*! \brief  Make xml processing instruction (hat) and returns new XML QDomDocument. On deserialization procedure all processing instructions will be ignored. */
    static QDomDocument appendXmlHat(const QDomNode &node, const QString & encoding, const QString & version = "1.0")
    {
        QDomDocument doc = node.toDocument();
        QDomNode xmlNode = doc.createProcessingInstruction("xml", QString("version=\"%1\" encoding=\"%2\"").arg(version).arg(encoding));
        doc.insertBefore(xmlNode, doc.firstChild());
        return doc;
    }
#endif


#ifdef QS_HAS_JSON
    /*! \brief  Serialize all accessed JSON propertyes for this object. */
    QJsonObject toJson() const
    {
        QJsonObject json;
        for(int i = 0; i < metaObject()->propertyCount(); i++)
        {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))
            {
                continue;
            }
#else
            if(metaObject()->property(i).metaType().id() != QMetaType::QJsonValue)
            {
                continue;
            }
#endif

            json.insert(metaObject()->property(i).name(), metaObject()->property(i).readOnGadget(this).toJsonValue());
        }
        return json;
    }

    /*! \brief  Returns JsonValue of single element (property) object. */
    QByteArray toRawJsonArray() const
    {
#ifdef QS_HAS_XML
        Q_ASSERT_X(metaObject()->propertyCount() == 2, metaObject()->className(), "Property count more than one");
#else
        Q_ASSERT_X(metaObject()->propertyCount() == 1, metaObject()->className(), "Property count more than one");
#endif

        int i = 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))
        {
            i++;
        }
#else
        if(metaObject()->property(i).metaType().id() != QMetaType::QJsonValue)
        {
            i++;
        }
#endif
        return toByteArray(metaObject()->property(i).readOnGadget(this).toJsonValue());
    }

    /*! \brief  Returns QByteArray representation this object using json-serialization. */
    QByteArray toRawJson() const
    {
        return toByteArray(toJson());
    }

    /*! \brief  Deserialize all accessed propertyes for this object. */
    void fromJson(const QJsonValue & val)
    {
        if(val.isObject())
        {
            QJsonObject json = val.toObject();
            QStringList keys = json.keys();
            int propCount = metaObject()->propertyCount();
            for(int i = 0; i < propCount; i++)
            {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))
                {
                    continue;
                }
#else
                if(metaObject()->property(i).metaType().id() != QMetaType::QJsonValue)
                {
                    continue;
                }
#endif

                for(auto key : json.keys())
                {
                    if(key == metaObject()->property(i).name())
                    {
                        metaObject()->property(i).writeOnGadget(this, json.value(key));
                        break;
                    }
                }
            }
        }
    }

    /*! \brief  Deserialize array of values for objects containing single element (property) */
    void fromJsonArray(const QByteArray &data)
    {
#ifdef QS_HAS_XML
        Q_ASSERT_X(metaObject()->propertyCount() == 2, metaObject()->className(), "Property count more than one");
#else
        Q_ASSERT_X(metaObject()->propertyCount() == 1, metaObject()->className(), "Property count more than one");
#endif

        int i = 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if(QString(metaObject()->property(i).typeName()) != QMetaType::typeName(qMetaTypeId<QJsonValue>()))
        {
            i++;
        }
#else
        if(metaObject()->property(i).metaType().id() != QMetaType::QJsonValue)
        {
            i++;
        }
#endif
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if(!doc.isArray() || doc.isEmpty())
            return;

        metaObject()->property(i).writeOnGadget(this, QJsonValue(doc.array()));
    }

    /*! \brief  Deserialize all accessed JSON propertyes for this object. */
    void fromJson(const QByteArray & data)
    {
        fromJson(QJsonDocument::fromJson(data).object());
    }
#endif // QS_HAS_JSON

    void dbg_message(const QString &message)
    {
#ifdef QT_DEBUG
//        qDebug().nospace().noquote() << tmp_indent << message;
#endif
    }

    void dbg_message_at_method_invoke(const QString &message)
    {
#ifdef QT_DEBUG
        dbg_message(message + "\n" + tmp_indent + "{");
        tmp_indent.append("    ");
#endif
    }

    void dbg_message_before_return(const QString &message)
    {
#ifdef QT_DEBUG
        dbg_message(message);
        tmp_indent.chop(4);
        dbg_message("}");
#endif
    }

    void dbg_nodeFirstBytes(const QDomNode &node)
    {
#ifdef QT_DEBUG
        QString str;
        QTextStream stream(&str);
        node.save(stream, 0 /*indent*/);
        dbg_message(QString("dbg_nodeFirstBytes() | isNull: %1; attributes.count() = %2; bytes: %3").arg(node.isNull()).arg(node.attributes().count()).arg(str.left(40).replace("\n", "\\n")));
#endif
    }

#ifdef QS_HAS_XML
    /*! \brief  Serialize all accessed XML propertyes for this object. */
    QDomNode toXml() const
    {
        QDomDocument doc;
        QDomElement el;
        el = doc.createElement(nodeTagName());

        for(int i = 0; i < metaObject()->propertyCount(); i++)
        {
            QMetaProperty p = metaObject()->property(i);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if(QString(p.typeName()) != QMetaType::typeName(qMetaTypeId<QDomNode>()))
            {
                continue;
            }
#else
            if(p.metaType().id() != qMetaTypeId<QDomNode>())
            {
                continue;
            }
#endif
            if(QString(p.name()).startsWith("attribute_"))
                el.setAttributeNode(p.readOnGadget(this).value<QDomNode>().toAttr());
            else
                el.appendChild(QDomNode(p.readOnGadget(this).value<QDomNode>()));
        }
        doc.appendChild(el);
        return doc;
    }

    /*! \brief  Returns QByteArray representation this object using xml-serialization. */
    QByteArray toRawXml() const
    {
        return toByteArray(toXml());
    }

    void readAttributes(const QDomElement &e)
    {
        QDomNamedNodeMap attributes = e.attributes();
        for(int i = 0; i < attributes.count(); i++)
        {
            QDomNode a = attributes.item(i);
            int propertyIndex = metaObject()->indexOfProperty("attribute_" + a.toAttr().name().toLocal8Bit());
            dbg_message(QString("attribute: %1; value: %2; property index: %3").arg(a.toAttr().name()).arg(a.toAttr().value()).arg(propertyIndex));
            if(propertyIndex == -1)
                continue;
            metaObject()->property(propertyIndex).writeOnGadget(this, QVariant::fromValue(a));
        }
    }

    /*! \brief  Deserialize all accessed XML propertyes for this object. */
    void fromXml(const QDomNode &val)
    {
        dbg_message_at_method_invoke("fromXml()");
        QDomNode doc = val;

        auto n = doc.isDocument()?firstChildElement(doc, nodeTagName()):doc.toElement();
        dbg_nodeFirstBytes(n);
        if(!n.isNull())
        {
            dbg_message(QString("fromXml() | node is not null; propertyCount = %1").arg(metaObject()->propertyCount()));
            readAttributes(n);
            for(int i = 0; i < metaObject()->propertyCount(); i++)
            {
                dbg_message(QString("fromXml() | try to read property %1").arg(metaObject()->property(i).name()));
                QString name = metaObject()->property(i).name();
                if(name.startsWith("attribute_"))
                    continue;

                auto f = firstChildElement(n, name);
                metaObject()->property(i).writeOnGadget(this, QVariant::fromValue<QDomNode>(f));
            }
        }
        else
        {
            dbg_message(QString("fromXml() | node is null"));
            QDomNamedNodeMap attributes = n.attributes();
            readAttributes(doc.toElement());
            for(int i = 0; i < metaObject()->propertyCount(); i++)
            {
                QString name = metaObject()->property(i).name();
                if(name.startsWith("attribute_"))
                    continue;
                auto f = firstChildElement(doc, name);
                metaObject()->property(i).writeOnGadget(this, QVariant::fromValue<QDomNode>(f));
            }
        }
        dbg_message_before_return("");
    }

    /*! \brief  Deserialize all accessed XML propertyes for this object. */
    void fromXml(const QByteArray & data)
    {
        QDomDocument d;
        d.setContent(data);
        fromXml(d);
    }
protected:
    QDomElement firstChildElement(const QDomNode& node, const QString& name)
    {
        dbg_message_at_method_invoke(QString("searching node %1").arg(isIdentifiedByTag()?childNodeTagName():name));
        QDomElement elem;
        QDomElement rtn;
        if(node.isDocument())
        {
            dbg_message_before_return("firstChildElement() | node is document (top level)");
            return node.firstChildElement();
        }
        else
            elem = node.toElement();

        if(elem.isNull())
        {
            dbg_message_before_return("firstChildElement() | node not found");
            return QDomElement();
        }

        if(isIdentifiedByTag())
        {
            rtn = elem.firstChildElement(childNodeTagName());
            if(!rtn.isNull())
            {
                dbg_message_before_return("firstChildElement() | node founded by tag name");
                return rtn;
            }
            dbg_message(QString("firstChildElement() | node not found by tag name"));
        }
        else
        {
            if( elem.attribute("name") == name )
            {
                dbg_message_before_return("firstChildElement() | node founded by attribute");
                return elem;
            }

            QDomNode child = elem.firstChild();

            while( !child.isNull() )
            {
                rtn = firstChildElement(child, name);
                if(!rtn.isNull())
                {
                    dbg_message_before_return("firstChildElement() | node founded in sub");
                    return rtn;
                }
                child = child.nextSibling();
            }
        }

        dbg_message_before_return("firstChildElement() | node not found");
        return QDomElement();
    };
#endif // QS_HAS_XML
};

#define OVERRIDE_TAG_NAME(name)                                                                         \
protected:                                                                                              \
    inline QString nodeTagName() const override                                                         \
    {                                                                                                   \
        return ((name));                                                                                \
    };                                                                                                  \

#define OVERRIDE_CHILD_NODE_TAG_NAME(name)                                                              \
protected:                                                                                              \
    inline QString childNodeTagName() const override                                                    \
    {                                                                                                   \
        return ((name));                                                                                \
    };

#define IDENTIFIED_BY_TAG(state)                                                                        \
protected:                                                                                              \
    inline bool isIdentifiedByTag() const override                                                      \
    {                                                                                                   \
        return state;                                                                                   \
    };

#define GET(prefix, name) get_##prefix##_##name
#define SET(prefix, name) set_##prefix##_##name

/* Create variable for attribute */
#define QS_DECLARE_ATTRIBUTE(name, defaultValue)                                                        \
    public :                                                                                            \
    QString attribute_##name = QString(defaultValue);                                                   \

/* Create variable */
#define QS_DECLARE_MEMBER(type, name)                                                                   \
    public :                                                                                            \
    type name = type();                                                                                 \

/* Create XML property and methods for node attribute*/
#ifdef QS_HAS_XML
#define QS_XML_ATTRIBUTE(name)                                                                          \
    Q_PROPERTY(QDomNode attribute_##name READ GET(xml, attribute_##name) WRITE SET(xml, attribute_##name))  \
    private:                                                                                            \
    QDomNode GET(xml, attribute_##name)() const                                                         \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QDomAttr a = doc.createAttribute(#name);                                                        \
        a.setValue(QString(attribute_##name));                                                          \
        return a;                                                                                       \
    }                                                                                                   \
    void SET(xml, attribute_##name)(const QDomNode &node)                                               \
    {                                                                                                   \
        attribute_##name = node.toAttr().value();                                                       \
    }
#else
#define QS_XML_ATTRIBUTE(name)
#endif

/* Create JSON property and methods for primitive type field*/
#ifdef QS_HAS_JSON
#define QS_JSON_FIELD(type, name)                                                                       \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                              \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QJsonValue val = QJsonValue::fromVariant(QVariant(name));                                       \
        return val;                                                                                     \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        /*QString text = varname.toString();*/                                                          \
        /* NaN will be converted to a null JSON value */                                                \
        /*if(varname.isNull() && QString::compare(#type, "double") == 0)                                \
            name = 0.0/0.0;                                                                             \
        else                                                                                            \
            name = QVariant(text).value<type>();*/                                                      \
        name = varname.toVariant().value<type>();                                                       \
    }
#else
#define QS_JSON_FIELD(type, name)
#endif

/* Create XML property and methods for primitive type field*/
#ifdef QS_HAS_XML
#define QS_XML_FIELD(type, name)                                                                        \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QString text;                                                                                   \
        QDomElement element = doc.createElement(childNodeTagName());                                    \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        if(QString::compare(#type, "bool") == 0)                                                        \
            text = QVariant(name).toBool()?"true":"false";                                              \
        else                                                                                            \
            text = QVariant(name).toString();                                                           \
        QDomText valueOfProp = doc.createTextNode(text);                                                \
        element.appendChild(valueOfProp);                                                               \
        doc.appendChild(element);                                                                       \
        return  QDomNode(element);                                                                      \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode &node)                                                           \
    {                                                                                                   \
        if(!node.isNull() && node.isElement()){                                                         \
            QDomElement domElement = node.toElement();                                                  \
            QString text = domElement.text();                                                           \
            if( (isIdentifiedByTag()?1:(domElement.attribute("name").compare(#name) == 0)) )            \
            {                                                                                           \
                if(QString::compare(#type, "bool") == 0)                                                \
                    name = QVariant(text.compare("true", Qt::CaseInsensitive)?0:1).value<type>();       \
                else                                                                                    \
                    name = QVariant(text).value<type>();                                                \
            }                                                                                           \
        }                                                                                               \
    }                                                                                       
#else
#define QS_XML_FIELD(type, name)
#endif

/* Create JSON property and methods for primitive type field with asterisk mark at end */
#ifdef QS_HAS_JSON
#define QS_JSON_MARKED_FIELD(type, name)                                                                \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                              \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QString value = QVariant(name).toString();                                                      \
        if(name##_marked)                                                                               \
            value.append('*');                                                                          \
        QJsonValue val = QJsonValue::fromVariant(value);                                                \
        return val;                                                                                     \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        QString value = varname.toString();                                                             \
        name##_marked = value.endsWith('*');                                                            \
        if(name##_marked)                                                                               \
            value.chop(1);                                                                              \
        name = QVariant(value).value<type>();                                                           \
    }
#else
#define QS_JSON_MARKED_FIELD(type, name)
#endif

/* Create XML property and methods for primitive type field with asterisk mark at end */
#ifdef QS_HAS_XML
#define QS_XML_MARKED_FIELD(type, name)                                                                 \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QString text;                                                                                   \
        QDomElement element = doc.createElement(childNodeTagName());                                    \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        if(QString::compare(#type, "bool") == 0)                                                        \
            text = QVariant(name).toBool()?"true":"false";                                              \
        else                                                                                            \
            text = QVariant(name).toString();                                                           \
        if(name##_marked)                                                                               \
            text.append('*');                                                                           \
        QDomText valueOfProp = doc.createTextNode(text);                                                \
        element.appendChild(valueOfProp);                                                               \
        doc.appendChild(element);                                                                       \
        return  QDomNode(element);                                                                      \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode &node)                                                           \
    {                                                                                                   \
        if(!node.isNull() && node.isElement()){                                                         \
            QDomElement domElement = node.toElement();                                                  \
            QString text = domElement.text();                                                           \
            name##_marked = text.endsWith('*');                                                         \
            if(name##_marked)                                                                           \
                text.chop(1);                                                                           \
            if( (isIdentifiedByTag()?1:(domElement.attribute("name").compare(#name) == 0)) )            \
            {                                                                                           \
                if(QString::compare(#type, "bool") == 0)                                                \
                    name = text.compare("true", Qt::CaseInsensitive)?0:1;                               \
                else                                                                                    \
                    name = QVariant(text).value<type>();                                                \
            }                                                                                           \
        }                                                                                               \
    }
#else
#define QS_XML_MARKED_FIELD(type, name)
#endif

/* Create JSON property and methods for paired type fields (QSize, QPoint)*/
#ifdef QS_HAS_JSON
#define QS_JSON_PAIR_FIELD(type, name)                                                                  \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                              \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QString text;                                                                                   \
        if(QString::compare("QSize", #type) == 0)                                                       \
            text = QString("%1,%2").arg(QVariant(name).toSize().width()).arg(QVariant(name).toSize().height()); \
        else                                                                                            \
            text = QString("%1,%2").arg(QVariant(name).toPoint().x()).arg(QVariant(name).toPoint().y());\
        QJsonValue val = QJsonValue::fromVariant(text);                                                 \
        return val;                                                                                     \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        QString text = varname.toVariant().value<QString>();                                            \
        name = type(text.split(',').at(0).toInt(), text.split(',').at(1).toInt());                      \
    }
#else
#define QS_JSON_PAIR_FIELD(type, name)
#endif

/* Create XML property and methods for QSize type field*/
#ifdef QS_HAS_XML
#define QS_XML_PAIR_FIELD(type, name)                                                                   \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QString text;                                                                                   \
        QDomElement element = doc.createElement(childNodeTagName());                                    \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        if(QString::compare("QSize", #type) == 0)                                                       \
            text = QString("%1,%2").arg(QVariant(name).toSize().width()).arg(QVariant(name).toSize().height()); \
        else                                                                                            \
            text = QString("%1,%2").arg(QVariant(name).toPoint().x()).arg(QVariant(name).toPoint().y());\
        QDomText valueOfProp = doc.createTextNode(text);                                                \
        element.appendChild(valueOfProp);                                                               \
        doc.appendChild(element);                                                                       \
        return  QDomNode(element);                                                                      \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode &node)                                                           \
    {                                                                                                   \
        if(!node.isNull() && node.isElement()){                                                         \
            QDomElement domElement = node.toElement();                                                  \
            QString text = domElement.text();                                                           \
            if( (isIdentifiedByTag()?1:(domElement.attribute("name").compare(#name) == 0)) )            \
            {                                                                                           \
                name = type(text.split(',').at(0).toInt(), text.split(',').at(1).toInt());              \
            }                                                                                           \
        }                                                                                               \
    }
#else
#define QS_XML_PAIR_FIELD(type, name)
#endif

/* Generate JSON-property and methods for primitive type objects */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#ifdef QS_HAS_JSON
#define QS_JSON_ARRAY(itemType, name)                                                                   \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                              \
    private:                                                                                            \
        QJsonValue GET(json, name)() const                                                              \
        {                                                                                               \
            QJsonArray val;                                                                             \
            for(int i = 0; i < name.size(); i++)                                                        \
                val.push_back(name.at(i));                                                              \
            return QJsonValue::fromVariant(val);                                                        \
        }                                                                                               \
        void SET(json, name)(const QJsonValue & varname)                                                \
        {                                                                                               \
            if(!varname.isArray())                                                                      \
                return;                                                                                 \
            name.clear();                                                                               \
            QJsonArray val = varname.toArray();                                                         \
            for(auto item : val)                                                                        \
            {                                                                                           \
                itemType tmp;                                                                           \
                tmp = item.toVariant().value<itemType>();                                               \
                name.append(tmp);                                                                       \
            }                                                                                           \
        }
#else
#define QS_JSON_ARRAY(itemType, name)
#endif

/* Generate XML-property and methods for primitive type objects */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#ifdef QS_HAS_XML
#define QS_XML_ARRAY(itemType, name)                                                                    \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
        QDomNode GET(xml, name)() const                                                                 \
        {                                                                                               \
            QDomDocument doc;                                                                           \
            QDomElement arrayXml = doc.createElement(childNodeTagName());                               \
            arrayXml.setAttribute("value", name.size());                                                \
            arrayXml.setAttribute("iskey", "true");                                                     \
            if(!isIdentifiedByTag())                                                                    \
            {                                                                                           \
                arrayXml.setAttribute("name", #name);                                                   \
            }                                                                                           \
            for(int i = 0; i < name.size(); i++)                                                        \
            {                                                                                           \
                itemType item = name.at(i);                                                             \
                QDomElement itemXml = doc.createElement(childNodeTagName());                            \
                itemXml.setAttribute("name", QString("Item%1").arg(i));                                 \
                itemXml.appendChild(doc.createTextNode(QVariant(item).toString()));                     \
                arrayXml.appendChild(itemXml);                                                          \
            }                                                                                           \
            doc.appendChild(arrayXml);                                                                  \
            return  QDomNode(doc);                                                                      \
        }                                                                                               \
        void SET(xml, name)(const QDomNode & node)                                                      \
        {                                                                                               \
            QDomNode domNode = node.firstChild();                                                       \
            name.clear();                                                                               \
            while(!domNode.isNull())                                                                    \
            {                                                                                           \
                if(domNode.isElement())                                                                 \
                {                                                                                       \
                    QDomElement domElement = domNode.toElement();                                       \
                    name.append(QVariant(domElement.text()).value<itemType>());                         \
                }                                                                                       \
                domNode = domNode.nextSibling();                                                        \
            }                                                                                           \
        }
#else
#define QS_XML_ARRAY(itemType, name)
#endif


/* Generate JSON-property and methods for some custom class */
/* Custom type must be provide methods fromJson and toJson or inherit from QSerializer */
#ifdef QS_HAS_JSON
#define QS_JSON_OBJECT(type, name)                                                                      \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                              \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QJsonObject val = name.toJson();                                                                \
        return QJsonValue(val);                                                                         \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        if(!varname.isObject())                                                                         \
        return;                                                                                         \
        name.fromJson(varname);                                                                         \
    }
#else
#define QS_JSON_OBJECT(type, name)
#endif

/* Generate XML-property and methods for some custom class */
/* Custom type must be provide methods fromJson and toJson or inherit from QSerializer */
#ifdef QS_HAS_XML
#define QS_XML_OBJECT(type, name)                                                                       \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
        QDomNode GET(xml, name)() const                                                                 \
        {                                                                                               \
            QDomElement child = name.toXml().firstChildElement();                                       \
            if(!isIdentifiedByTag())                                                                    \
                child.setAttribute("name", #name);                                                      \
            return child;                                                                               \
        }                                                                                               \
        void SET(xml, name)(const QDomNode & node){                                                     \
            name.fromXml(node);                                                                         \
        }
#else
#define QS_XML_OBJECT(type, name)
#endif

/* Generate JSON-property and methods for collection of custom type objects */
/* Custom item type must be provide methods fromJson and toJson or inherit from QSerializer */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#ifdef QS_HAS_JSON
#define QS_JSON_ARRAY_OBJECTS(itemType, name)                                                           \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json, name))                              \
    private:                                                                                            \
        QJsonValue GET(json, name)() const                                                              \
        {                                                                                               \
            QJsonArray val;                                                                             \
            for(int i = 0; i < name.size(); i++)                                                        \
                val.push_back(name.at(i).toJson());                                                     \
            return QJsonValue::fromVariant(val);                                                        \
        }                                                                                               \
        void SET(json, name)(const QJsonValue & varname)                                                \
        {                                                                                               \
            if(!varname.isArray())                                                                      \
                return;                                                                                 \
            name.clear();                                                                               \
            QJsonArray val = varname.toArray();                                                         \
            for(int i = 0; i < val.size(); i++)                                                         \
            {                                                                                           \
                itemType tmp;                                                                           \
                tmp.fromJson(val.at(i));                                                                \
                name.append(tmp);                                                                       \
            }                                                                                           \
        }
#else
#define QS_JSON_ARRAY_OBJECTS(itemType, name)
#endif

/* Generate XML-property and methods for collection of custom type objects  */
/* Custom type must be provide methods fromXml and toXml or inherit from QSerializer */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#ifdef QS_HAS_XML
#define QS_XML_ARRAY_OBJECTS(itemType, name)                                                            \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QDomElement element = doc.createElement(childNodeTagName());                                    \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        element.setAttribute("value", name.size());                                                     \
        element.setAttribute("iskey", "true");                                                          \
        for(int i = 0; i < name.size(); i++)                                                            \
        {                                                                                               \
            QDomElement child = name.at(i).toXml().firstChildElement();                                 \
            child.setAttribute("name", QString("%1%2").arg("Item").arg(i+1));                           \
            element.appendChild(child);                                                                 \
        }                                                                                               \
        doc.appendChild(element);                                                                       \
        return QDomNode(doc);                                                                           \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode & node)                                                          \
    {                                                                                                   \
        name.clear();                                                                                   \
        QDomNodeList nodesList = node.childNodes();                                                     \
        for(int i = 0;  i < nodesList.size(); i++)                                                      \
        {                                                                                               \
            itemType tmp;                                                                               \
            tmp.fromXml(nodesList.at(i));                                                               \
            name.append(tmp);                                                                           \
        }                                                                                               \
    }                                                                                       
#else
#define QS_XML_ARRAY_OBJECTS(itemType, name)
#endif

/* Generate JSON-property and methods for dictionary of simple fields (int, bool, QString, ...)  */
/* Custom type must be inherit from QSerializer */
/* This collection must be provide method insert(KeyT, ValueT) (it's can be QMap, QHash)    */
/* THIS IS FOR QT DICTIONARY TYPES, for example QMap<int, QString>, QMap<int,int>, ...*/
#ifdef QS_HAS_JSON
#define QS_JSON_QT_DICT(map, name)                                                                      \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json,name))                               \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QJsonObject val;                                                                                \
        for(auto p = name.constBegin(); p != name.constEnd(); ++p)                                      \
        {                                                                                               \
            val.insert(                                                                                 \
                QVariant(p.key()).toString(),                                                           \
                QJsonValue::fromVariant(QVariant(p.value())));                                          \
        }                                                                                               \
        return val;                                                                                     \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        QJsonObject val = varname.toObject();                                                           \
        name.clear();                                                                                   \
        for(auto p = val.constBegin() ;p != val.constEnd(); ++p)                                        \
        {                                                                                               \
            name.insert(                                                                                \
                QVariant(p.key()).value<map::key_type>(),                                               \
                QVariant(p.value()).value<map::mapped_type>());                                         \
        }                                                                                               \
    }                                                                                       
#else
#define QS_JSON_QT_DICT(map, name)
#endif

/* Generate XML-property and methods for dictionary of simple fields (int, bool, QString, ...)  */
/* Custom type must be inherit from QSerializer */
/* This collection must be provide method insert(KeyT, ValueT) (it's can be QMap, QHash)    */
/* THIS IS FOR QT DICTIONARY TYPES, for example QMap<int, QString>, QMap<int,int>, ...*/
#ifdef QS_HAS_XML
#define QS_XML_QT_DICT(map, name)                                                                       \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QDomElement element = doc.createElement(nodeTagName());                                         \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        element.setAttribute("iskey", "true");                                                          \
        for(auto p = name.begin(); p != name.end(); ++p)                                                \
        {                                                                                               \
            QDomElement e = doc.createElement(nodeTagName());                                           \
            e.setAttribute("key", QVariant(p.key()).toString());                                        \
            e.setAttribute("value", QVariant(p.value()).toString());                                    \
            element.appendChild(e);                                                                     \
        }                                                                                               \
        doc.appendChild(element);                                                                       \
        return QDomNode(doc);                                                                           \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode & node)                                                          \
    {                                                                                                   \
        if(!node.isNull() && node.isElement())                                                          \
        {                                                                                               \
            QDomElement root = node.toElement();                                                        \
            if(root.attribute("name") == #name)                                                         \
            {                                                                                           \
                QDomNodeList childs = root.childNodes();                                                \
                for(int i = 0; i < childs.size(); ++i)                                                  \
                {                                                                                       \
                    QDomElement item = childs.at(i).toElement();                                        \
                    name.insert(QVariant(item.attributeNode("key").value()).value<map::key_type>(),     \
                                QVariant(item.attributeNode("value").value()).value<map::mapped_type>()); \
                }                                                                                       \
            }                                                                                           \
        }                                                                                               \
    }                                                                                                   
#else
#define QS_XML_QT_DICT(map, name)
#endif


/* Generate JSON-property and methods for dictionary of custom type objects  */
/* Custom type must be inherit from QSerializer */
/* This collection must be provide method inserv(KeyT, ValueT) (it's can be QMap, QHash)    */
/* THIS IS FOR QT DICTIONARY TYPES, for example QMap<int, CustomSerializableType> */
#ifdef QS_HAS_JSON
#define QS_JSON_QT_DICT_OBJECTS(map, name)                                                              \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json,name))                               \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QJsonObject val;                                                                                \
        for(auto p = name.begin(); p != name.end(); ++p)                                                \
        {                                                                                               \
            val.insert(                                                                                 \
                QVariant::fromValue(p.key()).toString(),                                                \
                p.value().toJson());                                                                    \
        }                                                                                               \
        return val;                                                                                     \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        QJsonObject val = varname.toObject();                                                           \
        name.clear();                                                                                   \
        for(auto p = val.constBegin();p != val.constEnd(); ++p)                                         \
        {                                                                                               \
            map::mapped_type tmp;                                                                       \
            tmp.fromJson(p.value());                                                                    \
            name.insert(                                                                                \
                QVariant(p.key()).value<map::key_type>(),                                               \
                tmp);                                                                                   \
        }                                                                                               \
    }                                                                                       
#else
#define QS_JSON_QT_DICT_OBJECTS(map, name)
#endif

/* Generate XML-property and methods for dictionary of custom type objects  */
/* Custom type must be inherit from QSerializer */
/* This collection must be provide method insert(KeyT, ValueT) (it's can be QMap, QHash)    */
/* THIS IS FOR QT DICTIONARY TYPES, for example QMap<int, CustomSerializableType> */
#ifdef QS_HAS_XML
#define QS_XML_QT_DICT_OBJECTS(map, name)                                                               \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QDomElement element = doc.createElement(nodeTagName());                                         \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        for(auto p = name.begin(); p != name.end(); ++p)                                                \
        {                                                                                               \
            QDomElement e = doc.createElement(nodeTagName());                                           \
            e.setAttribute("key", QVariant(p.key()).toString());                                        \
            e.appendChild(p.value().toXml());                                                           \
            element.appendChild(e);                                                                     \
        }                                                                                               \
        doc.appendChild(element);                                                                       \
        return QDomNode(doc);                                                                           \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode & node)                                                          \
    {                                                                                                   \
        if(!node.isNull() && node.isElement())                                                          \
        {                                                                                               \
            QDomElement root = node.toElement();                                                        \
            if(root.attribute("name") == #name)                                                         \
            {                                                                                           \
                QDomNodeList childs = root.childNodes();                                                \
                for(int i = 0; i < childs.size(); ++i)                                                  \
                {                                                                                       \
                QDomElement item = childs.at(i).toElement();                                            \
                map::mapped_type tmp;                                                                   \
                tmp.fromXml(item.firstChild());                                                         \
                name.insert(QVariant(item.attributeNode("key").value()).value<map::key_type>(),         \
                            tmp);                                                                       \
                }                                                                                       \
            }                                                                                           \
        }                                                                                               \
    }                                                                                                   
#else
#define QS_XML_QT_DICT_OBJECTS(map, name)
#endif

/* Generate JSON-property and methods for dictionary of simple fields (int, bool, QString, ...)  */
/* Custom type must be inherit from QSerializer */
/* This collection must be provide method insert(KeyT, ValueT) (it's can be std::map)    */
/* THIS IS FOR STL DICTIONARY TYPES, for example std::map<int, QString>, std::map<int,int>, ...*/
#ifdef QS_HAS_JSON
#define QS_JSON_STL_DICT(map, name)                                                                     \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json,name))                               \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QJsonObject val;                                                                                \
        for(auto p : name){                                                                             \
            val.insert(                                                                                 \
                QVariant::fromValue(p.first).toString(),                                                \
                QJsonValue::fromVariant(QVariant(p.second)));                                           \
        }                                                                                               \
        return val;                                                                                     \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        QJsonObject val = varname.toObject();                                                           \
        name.clear();                                                                                   \
        for(auto p = val.constBegin();p != val.constEnd(); ++p)                                         \
        {                                                                                               \
            name.insert(std::pair<map::key_type, map::mapped_type>(                                     \
                QVariant(p.key()).value<map::key_type>(),                                               \
                QVariant(p.value()).value<map::mapped_type>()));                                        \
        }                                                                                               \
    }                                                                                       
#else
#define QS_JSON_STL_DICT(map, name)
#endif

#ifdef QS_HAS_XML
#define QS_XML_STL_DICT(map, name)                                                                      \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QDomElement element = doc.createElement(nodeTagName());                                         \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        for(auto p : name)                                                                              \
        {                                                                                               \
            QDomElement e = doc.createElement(nodeTagName());                                           \
            e.setAttribute("key", QVariant(p.first).toString());                                        \
            e.setAttribute("value", QVariant(p.second).toString());                                     \
            element.appendChild(e);                                                                     \
        }                                                                                               \
        doc.appendChild(element);                                                                       \
        return QDomNode(doc);                                                                           \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode & node)                                                          \
    {                                                                                                   \
        if(!node.isNull() && node.isElement())                                                          \
        {                                                                                               \
            QDomElement root = node.toElement();                                                        \
            if(root.attribute("name") == #name)                                                         \
            {                                                                                           \
                QDomNodeList childs = root.childNodes();                                                \
                for(int i = 0; i < childs.size(); ++i)                                                  \
                {                                                                                       \
                QDomElement item = childs.at(i).toElement();                                            \
                    name.insert(std::pair<map::key_type, map::mapped_type>(                             \
                        QVariant(item.attributeNode("key").value()).value<map::key_type>(),             \
                        QVariant(item.attributeNode("value").value()).value<map::mapped_type>()));      \
                }                                                                                       \
            }                                                                                           \
        }                                                                                               \
    }                                                                                                   
#else
#define QS_XML_STL_DICT(map, name)                                                                    
#endif

/* Generate JSON-property and methods for dictionary of custom type objects */
/* Custom type must be inherit from QSerializer */
/* This collection must be provide method insert(KeyT, ValueT) (it's can be std::map)    */
/* THIS IS FOR STL DICTIONARY TYPES, for example std::map<int, CustomSerializableType> */
#ifdef QS_HAS_JSON
#define QS_JSON_STL_DICT_OBJECTS(map, name)                                                             \
    Q_PROPERTY(QJsonValue name READ GET(json, name) WRITE SET(json,name))                               \
    private:                                                                                            \
    QJsonValue GET(json, name)() const                                                                  \
    {                                                                                                   \
        QJsonObject val;                                                                                \
        for(auto p : name){                                                                             \
            val.insert(                                                                                 \
                QVariant::fromValue(p.first).toString(),                                                \
                p.second.toJson());                                                                     \
        }                                                                                               \
        return val;                                                                                     \
    }                                                                                                   \
    void SET(json, name)(const QJsonValue & varname)                                                    \
    {                                                                                                   \
        QJsonObject val = varname.toObject();                                                           \
        name.clear();                                                                                   \
        for(auto p = val.constBegin(); p != val.constEnd(); ++p)                                        \
        {                                                                                               \
            map::mapped_type tmp;                                                                       \
            tmp.fromJson(p.value());                                                                    \
            name.insert(std::pair<map::key_type, map::mapped_type>(                                     \
                QVariant(p.key()).value<map::key_type>(),                                               \
                tmp));                                                                                  \
        }                                                                                               \
    }                                                                                       
#else
#define QS_JSON_STL_DICT_OBJECTS(map, name)                               
#endif

/* Generate XML-property and methods for dictionary of custom type objects */
/* Custom type must be inherit from QSerializer */
/* This collection must be provide method insert(KeyT, ValueT) (it's can be std::map)    */
/* THIS IS FOR STL DICTIONARY TYPES, for example std::map<int, CustomSerializableType> */
#ifdef QS_HAS_XML
#define QS_XML_STL_DICT_OBJECTS(map, name)                                                              \
    Q_PROPERTY(QDomNode name READ GET(xml, name) WRITE SET(xml, name))                                  \
    private:                                                                                            \
    QDomNode GET(xml, name)() const                                                                     \
    {                                                                                                   \
        QDomDocument doc;                                                                               \
        QDomElement element = doc.createElement(nodeTagName());                                         \
        if(!isIdentifiedByTag())                                                                        \
        {                                                                                               \
            element.setAttribute("name", #name);                                                        \
        }                                                                                               \
        for(auto p : name)                                                                              \
        {                                                                                               \
            QDomElement e = doc.createElement(nodeTagName());                                           \
            e.setAttribute("key", QVariant(p.first).toString());                                        \
            e.appendChild(p.second.toXml());                                                            \
            element.appendChild(e);                                                                     \
        }                                                                                               \
        doc.appendChild(element);                                                                       \
        return QDomNode(doc);                                                                           \
    }                                                                                                   \
    void SET(xml, name)(const QDomNode & node)                                                          \
    {                                                                                                   \
        if(!node.isNull() && node.isElement())                                                          \
        {                                                                                               \
            QDomElement root = node.toElement();                                                        \
            if(root.attribute("name") == #name)                                                         \
            {                                                                                           \
                QDomNodeList childs = root.childNodes();                                                \
                                                                                                        \
                for(int i = 0; i < childs.size(); ++i)                                                  \
                {                                                                                       \
                QDomElement item = childs.at(i).toElement();                                            \
                map::mapped_type tmp;                                                                   \
                tmp.fromXml(item.firstChild());                                                         \
                name.insert(std::pair<map::key_type, map::mapped_type> (                                \
                            QVariant(item.attributeNode("key").value()).value<map::key_type>(),         \
                            tmp));                                                                      \
                }                                                                                       \
            }                                                                                           \
        }                                                                                               \
    }
#else
#define QS_XML_STL_DICT_OBJECTS(map, name)
#endif


/* BIND: */
/* generate serializable propertyes for XML attribute */
#define QS_BIND_ATTRIBUTE(name)                                                                         \
    QS_XML_ATTRIBUTE(name)

/* BIND: */
/* generate serializable propertyes JSON and XML for primitive type field */
#define QS_BIND_FIELD(type, name)                                                                       \
    QS_JSON_FIELD(type, name)                                                                           \
    QS_XML_FIELD(type, name)

/* BIND: */
/* generate serializable propertyes JSON and XML for primitive type field with asterisk mark */
#define QS_BIND_MARKED_FIELD(type, name)                                                                \
    QS_JSON_MARKED_FIELD(type, name)                                                                    \
    QS_XML_MARKED_FIELD(type, name)

/* BIND: */
/* generate serializable propertyes JSON and XML for paired type fields (QSize, QPoint) */
#define QS_BIND_PAIR_FIELD(type, name)                                                                  \
    QS_JSON_PAIR_FIELD(type, name)                                                                      \
    QS_XML_PAIR_FIELD(type, name)

/* BIND: */
/* generate serializable propertyes JSON and XML for collection of primitive type fields */
#define QS_BIND_COLLECTION(itemType, name)                                                              \
    QS_JSON_ARRAY(itemType, name)                                                                       \
    QS_XML_ARRAY(itemType, name)

/* BIND: */
/* generate serializable propertyes JSON and XML for custom type object */
#define QS_BIND_OBJECT(type, name)                                                                      \
    QS_JSON_OBJECT(type, name)                                                                          \
    QS_XML_OBJECT(type, name)

/* BIND: */
/* generate serializable propertyes JSON and XML for collection of custom type objects */
#define QS_BIND_COLLECTION_OBJECTS(itemType, name)                                                      \
    QS_JSON_ARRAY_OBJECTS(itemType, name)                                                               \
    QS_XML_ARRAY_OBJECTS(itemType, name)

/* BIND: */
/* generate serializable propertyes JSON and XML for dictionary with primitive value type for QT DICTIONARY TYPES */
#define QS_BIND_QT_DICT(map, name)                                                                      \
    QS_JSON_QT_DICT(map, name)                                                                          \
    QS_XML_QT_DICT(map, name)

/* BIND: */
/* generate serializable propertyes JSON and XML for dictionary of custom type objects for QT DICTIONARY TYPES */
#define QS_BIND_QT_DICT_OBJECTS(map, name)                                                              \
    QS_JSON_QT_DICT_OBJECTS(map, name)                                                                  \
    QS_XML_QT_DICT_OBJECTS(map,name)


/* BIND: */
/* generate serializable propertyes JSON and XML for dictionary with primitive value type for STL DICTIONARY TYPES */
#define QS_BIND_STL_DICT(map, name)                                                                     \
    QS_JSON_STL_DICT(map, name)                                                                         \
    QS_XML_STL_DICT(map, name)


/* BIND: */
/* generate serializable propertyes JSON and XML for dictionary of custom type objects for STL DICTIONARY TYPES */
#define QS_BIND_STL_DICT_OBJECTS(map, name)                                                             \
    QS_JSON_STL_DICT_OBJECTS(map, name)                                                                 \
    QS_XML_STL_DICT_OBJECTS(map,name)

/* CREATE AND BIND: */
/* Make xml attribute and generate serializable propertyes */
/* For example: QS_ATTRIBUTE(attribute1, defaultValue) */
#define QS_ATTRIBUTE(name, defaultValue)                                                                \
    QS_DECLARE_ATTRIBUTE(name, defaultValue)                                                            \
    QS_BIND_ATTRIBUTE(name)

/* CREATE AND BIND: */
/* Make primitive field and generate serializable propertyes */
/* For example: QS_FIELD(int, digit), QS_FIELD(bool, flag) */
#define QS_FIELD(type, name)                                                                            \
    QS_DECLARE_MEMBER(type, name)                                                                       \
    QS_BIND_FIELD(type, name)

/* CREATE AND BIND: */
/* Make primitive field with asterisk mark and generate serializable propertyes */
/* For example: QS_MARKED_FIELD(int, digit), QS_MARKED_FIELD(bool, flag) */
#define QS_MARKED_FIELD(type, name)                                                                     \
    QS_DECLARE_MEMBER(type, name)                                                                       \
    QS_DECLARE_MEMBER(bool, name##_marked)                                                              \
    QS_BIND_MARKED_FIELD(type, name)

/* CREATE AND BIND: */
/* Make QSize field and generate serializable propertyes */
/* For example: QS_PAIR_FIELD(QSize, m_size), QS_PAIR_FIELD(QPoint, m_point) */
#define QS_PAIR_FIELD(type, name)                                                                       \
    QS_DECLARE_MEMBER(type, name)                                                                       \
    QS_BIND_PAIR_FIELD(type, name)

/* CREATE AND BIND: */
/* Make collection of primitive type objects [collectionType<itemType> name] and generate serializable propertyes for this collection */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_COLLECTION(collectionType, itemType, name)                                                   \
    QS_DECLARE_MEMBER(collectionType<itemType>, name)                                                   \
    QS_BIND_COLLECTION(itemType, name)

/* CREATE AND BIND: */
/* Make custom class object and bind serializable propertyes */
/* This class must be inherited from QSerializer */
#define QS_OBJECT(type,name)                                                                            \
    QS_DECLARE_MEMBER(type, name)                                                                       \
    QS_BIND_OBJECT(type, name)

/* CREATE AND BIND: */
/* Make collection of custom class objects [collectionType<itemType> name] and bind serializable propertyes */
/* This collection must be provide method append(T) (it's can be QList, QVector)    */
#define QS_COLLECTION_OBJECTS(collectionType, itemType, name)                                           \
    QS_DECLARE_MEMBER(collectionType<itemType>, name)                                                   \
    QS_BIND_COLLECTION_OBJECTS(itemType, name)


/* CREATE AND BIND: */
/* Make dictionary collection of simple types [dictionary<key, itemType> name] and bind serializable propertyes */
/* This collection must be QT DICTIONARY TYPE */
#define QS_QT_DICT(map, first, second, name)                                                            \
    public:                                                                                             \
    typedef map<first,second> dict_##name##_t;                                                          \
    dict_##name##_t name = dict_##name##_t();                                                           \
    QS_BIND_QT_DICT(dict_##name##_t, name)

/* CREATE AND BIND: */
/* Make dictionary collection of custom class objects [dictionary<key, itemType> name] and bind serializable propertyes */
/* This collection must be QT DICTIONARY TYPE */
#define QS_QT_DICT_OBJECTS(map, first, second, name)                                                    \
    public:                                                                                             \
    typedef map<first,second> dict_##name##_t;                                                          \
    dict_##name##_t name = dict_##name##_t();                                                           \
    QS_BIND_QT_DICT_OBJECTS(dict_##name##_t, name)

/* CREATE AND BIND: */
/* Make dictionary collection of simple types [dictionary<key, itemType> name] and bind serializable propertyes */
/* This collection must be STL DICTIONARY TYPE */
#define QS_STL_DICT(map, first, second, name)                                                           \
    public:                                                                                             \
    typedef map<first,second> dict_##name##_t;                                                          \
    dict_##name##_t name = dict_##name##_t();                                                           \
    QS_BIND_STL_DICT(dict_##name##_t, name)

/* CREATE AND BIND: */
/* Make dictionary collection of custom class objects [dictionary<key, itemType> name] and bind serializable propertyes */
/* This collection must be STL DICTIONARY TYPE */
#define QS_STL_DICT_OBJECTS(map, first, second, name)                                                   \
    public:                                                                                             \
    typedef map<first,second> dict_##name##_t;                                                          \
    dict_##name##_t name = dict_##name##_t();                                                           \
    QS_BIND_STL_DICT_OBJECTS(dict_##name##_t, name)

#endif // QSERIALIZER_H

