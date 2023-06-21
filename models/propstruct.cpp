#include "propstruct.h"
#include "global.h"

QWidget *SPropertyCollection::widget(const int propId, const WidgetType type)
{
    QString wName = metaObject()->property(propId + metaObject()->propertyOffset()).name();
    QWidget *w;
    if(type == WidgetType::Label)
    {
        w = i_labelWidgets.value(wName);
    }
    else
    {
        w = i_editorWidgets.value(wName);
    }

    return w;
}

int SPropertyCollection::propertyGroup(const int propId)
{
    QString wName = metaObject()->property(propId + metaObject()->propertyOffset()).name();
    return i_propertyGroup.value(wName);
}

int SPropertyCollection::propertyId(const char *name) const
{
    return (metaObject()->indexOfProperty(name) - metaObject()->propertyOffset());
}

int SPropertyCollection::fieldToPropertyId(const QString &fieldName)
{
    QString propertyName = i_fieldNames.key(fieldName);
    if(propertyName.isEmpty())
    {
        appLog->appendRecord(QString("Unknown setting \"%1\"").arg(fieldName));
        return -1;
    }

    return metaObject()->indexOfProperty((&propertyName)->toLocal8Bit());
}

void SPropertyCollection::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{

}

void SPropertyCollection::reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{

}

int SPropertyCollection::count()
{
    return metaObject()->propertyCount() - metaObject()->propertyOffset();
}

QStringList SPropertyCollection::keys()
{
    QStringList list;
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        list.append(metaObject()->property(i).name());

    return list;
}

QList<QVariant> SPropertyCollection::values()
{
    QList<QVariant> list;
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); i++)
        list.append(metaObject()->property(i).read(this));

    return list;
}

void SPropertyCollection::deleteWidgets()
{
    QWidget *w;
    QString k;
    while(i_labelWidgets.count())
    {
        w = i_labelWidgets.last();
        k = i_labelWidgets.lastKey();
        i_labelWidgets.remove(k);
        delete w;
    }

    while(i_editorWidgets.count())
    {
        w = i_editorWidgets.last();
        k = i_editorWidgets.lastKey();
        i_editorWidgets.remove(k);
        delete w;
    }
}

