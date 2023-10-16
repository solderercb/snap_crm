/*  Классы параметров, используемые для сохранения настроек пользователя.
 *  Используются совместно с QSerializer
*/
#ifndef STABLEVIEWGRIDLAYOUT_H
#define STABLEVIEWGRIDLAYOUT_H
#include <3rdparty/QSerializer/QSerializer>
class t_GroupSummary : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(int, Item)
};

class t_SortInfo : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(QString, FieldName)
    QS_XML_FIELD(QString, SortOrder)
    QS_XML_FIELD(bool, MergeWithPreviousGroup)
};

class t_Columns : public QSerializer {
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    QS_XML_ATTRIBUTE(isnull, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(int, GridRow)
    QS_XML_FIELD(QString, FieldName)
    QS_XML_FIELD(QString, Name)
    QS_XML_FIELD(bool, Visible)
    QS_XML_FIELD(int, VisibleIndex)
    QS_XML_MARKED_FIELD(double, Width)    // В АСЦ это поле может содержать символ *, означающий, что ширина столбца изменяется автоматически под ширину окна (увеличивается, если есть место после крайнего правого столбца или уменьшается, если ширина всех столбцов больше ширины виджета
    QS_XML_FIELD(double, ActualWidth)
};

class t_FormatConditions : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(int, Item)
};

class t_ColumnChooserState : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_PAIR_FIELD(QPoint, Location)
    QS_XML_PAIR_FIELD(QSize, Size)
};

class t_View : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    QS_XML_ATTRIBUTE(isnull, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_COLLECTION_OBJECTS(QVector, t_FormatConditions, FormatConditions)
    QS_XML_FIELD(bool, ShowGroupPanel)
    QS_XML_OBJECT(t_ColumnChooserState, ColumnChooserState)
    QS_XML_FIELD(QString, SearchString)
};

class t_TotalSummary : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(int, Item)
};

class t_MRUFilters : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(QString, FilterString)
    QS_XML_FIELD(QString, FilterText)
};

class t_GroupSummarySortInfo : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(int, Item)
};

class GridControl : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(iskey, "true")
    QS_XML_ATTRIBUTE(value, "GridControl")
    OVERRIDE_TAG_NAME("property")
    QS_XML_FIELD(bool, UseFieldNameForSerialization)
    QS_XML_FIELD(int, GroupCount)
    QS_XML_COLLECTION_OBJECTS(QVector, t_GroupSummary, GroupSummary)
    QS_XML_COLLECTION_OBJECTS(QVector, t_SortInfo, SortInfo)
    QS_XML_COLLECTION_OBJECTS(QVector, t_Columns, Columns)
    QS_XML_OBJECT(t_View, View)
    QS_XML_COLLECTION_OBJECTS(QVector, t_TotalSummary, TotalSummary)
    QS_XML_COLLECTION_OBJECTS(QVector, t_MRUFilters, MRUFilters)
    QS_XML_FIELD(QString, FilterString)
    QS_XML_FIELD(bool, IsFilterEnabled)
    QS_XML_COLLECTION_OBJECTS(QVector, t_GroupSummarySortInfo, GroupSummarySortInfo)
};

class XtraSerializer : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_XML_ATTRIBUTE(version, "1.2.1")
    QS_XML_ATTRIBUTE(application, "SNAP CRM")
    OVERRIDE_TAG_NAME("XtraSerializer")
    QS_XML_FIELD(int, LayoutVersion)    // в АСЦ в имени этого поля используется префикс из символа #; в целом, версия не так уж важна, поэтому специальный обработчик для такого случая не создавался.
    QS_XML_OBJECT(GridControl, $GridControl) // префикс из символа $ используется в АСЦ
};

#endif // STABLEVIEWGRIDLAYOUT_H
