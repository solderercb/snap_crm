/*  Классы параметров, используемые для сохранения настроек пользователя.
 *  Используются совместно с QSerializer
*/
#ifndef STABLEVIEWGRIDLAYOUT_H
#define STABLEVIEWGRIDLAYOUT_H
#include <3rdparty/QSerializer/QSerializer>
class t_GroupSummary : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(int, Item)
};

class t_SortInfo : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(QString, FieldName)
    QS_FIELD(QString, SortOrder)
    QS_FIELD(bool, MergeWithPreviousGroup)
};

class t_Columns : public QSerializer {
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    QS_ATTRIBUTE(isnull, "true")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(int, GridRow)
    QS_FIELD(QString, FieldName)
    QS_FIELD(QString, Name)
    QS_FIELD(bool, Visible)
    QS_FIELD(int, VisibleIndex)
    QS_MARKED_FIELD(int, Width)    // В АСЦ это поле может содержать символ *, означающий, что ширина столбца изменяется автоматически под ширину окна (увеличивается, если есть место после крайнего правого столбца или уменьшается, если ширина всех столбцов больше ширины виджета
    QS_FIELD(double, ActualWidth)
};

class t_FormatConditions : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(int, Item)
};

class t_ColumnChooserState : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_PAIR_FIELD(QPoint, Location)
    QS_PAIR_FIELD(QSize, Size)
};

class t_View : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    QS_ATTRIBUTE(isnull, "true")
    OVERRIDE_TAG_NAME("property")
    QS_COLLECTION_OBJECTS(QVector, t_FormatConditions, FormatConditions)
    QS_FIELD(bool, ShowGroupPanel)
    QS_OBJECT(t_ColumnChooserState, ColumnChooserState)
    QS_FIELD(QString, SearchString)
};

class t_TotalSummary : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(int, Item)
};

class t_MRUFilters : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(QString, FilterString)
    QS_FIELD(QString, FilterText)
};

class t_GroupSummarySortInfo : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(int, Item)
};

class GridControl : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(iskey, "true")
    QS_ATTRIBUTE(value, "GridControl")
    OVERRIDE_TAG_NAME("property")
    QS_FIELD(bool, UseFieldNameForSerialization)
    QS_FIELD(int, GroupCount)
    QS_COLLECTION_OBJECTS(QVector, t_GroupSummary, GroupSummary)
    QS_COLLECTION_OBJECTS(QVector, t_SortInfo, SortInfo)
    QS_COLLECTION_OBJECTS(QVector, t_Columns, Columns)
    QS_OBJECT(t_View, View)
    QS_COLLECTION_OBJECTS(QVector, t_TotalSummary, TotalSummary)
    QS_COLLECTION_OBJECTS(QVector, t_MRUFilters, MRUFilters)
    QS_FIELD(QString, FilterString)
    QS_FIELD(bool, IsFilterEnabled)
    QS_COLLECTION_OBJECTS(QVector, t_GroupSummarySortInfo, GroupSummarySortInfo)
};

class XtraSerializer : public QSerializer{
    Q_GADGET
    QS_SERIALIZABLE
    QS_ATTRIBUTE(version, "1.2.1")
    QS_ATTRIBUTE(application, "SNAP CRM")
    OVERRIDE_TAG_NAME("XtraSerializer")
    QS_FIELD(int, LayoutVersion)    // в АСЦ в имени этого поля используется префикс из символа #; в целом, версия не так уж важна, поэтому специальный обработчик для такого случая не создавался.
    QS_OBJECT(GridControl, $GridControl) // префикс из символа $ используется в АСЦ
};

#endif // STABLEVIEWGRIDLAYOUT_H
