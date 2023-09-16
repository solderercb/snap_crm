#ifndef SLOCALSETTINGSSTRUCTS_H
#define SLOCALSETTINGSSTRUCTS_H
#include <3rdparty/QSerializer/QSerializer>
#include <QQueue>
#include <QStack>
#include <QSize>
#include <QPoint>

class t_bool : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("setting")
    OVERRIDE_CHILD_NODE_TAG_NAME("value")
    IDENTIFIED_BY_TAG(1)
    QS_FIELD(bool, value)
};

class t_int : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("setting")
    OVERRIDE_CHILD_NODE_TAG_NAME("value")
    IDENTIFIED_BY_TAG(1)
    QS_FIELD(int, value)
};

class t_string : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("setting")
    OVERRIDE_CHILD_NODE_TAG_NAME("value")
    IDENTIFIED_BY_TAG(1)
    QS_FIELD(QString, value)
};

class t_userSettings : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("userSettings")
    OVERRIDE_CHILD_NODE_TAG_NAME("setting")
    QS_OBJECT(t_string, dbHost)
    QS_OBJECT(t_string, dbPort)
    QS_OBJECT(t_string, dbName)
    QS_OBJECT(t_string, lastLogin)
    QS_OBJECT(t_int, StoreLastCat)
    QS_OBJECT(t_int, WorkspaceTop)
    QS_OBJECT(t_int, WorkspaceLeft)
    QS_OBJECT(t_int, WorkspaceWidth)
    QS_OBJECT(t_int, WorkspaceHeight)
    QS_OBJECT(t_string, WorkspaceState)
    QS_OBJECT(t_bool, ACPDisplayArhiveUsers)
    QS_OBJECT(t_string, language)
    QS_OBJECT(t_int, PreviewTop)
    QS_OBJECT(t_int, PreviewLeft)
    QS_OBJECT(t_int, PreviewWidth)
    QS_OBJECT(t_int, PreviewHeight)
    QS_OBJECT(t_string, PreviewMaximized)
    QS_OBJECT(t_string, LastCamera)
    QS_OBJECT(t_bool, LastCameraSet)
    QS_OBJECT(t_int, LookLeft)
    QS_OBJECT(t_int, LookTop)
    QS_OBJECT(t_int, WorkspaceBackground)
    QS_OBJECT(t_int, LastCompany)
    QS_OBJECT(t_int, koLeft)
    QS_OBJECT(t_int, koTop)
    QS_OBJECT(t_int, PhotoAddTop)
    QS_OBJECT(t_int, PhotoAddLeft)
    QS_OBJECT(t_int, PhotoAddWidth)
    QS_OBJECT(t_int, PhotoAddHeight)
    QS_OBJECT(t_int, StoreCatsWidth)
    QS_OBJECT(t_string, StoreArticulHeight) // В АСЦ это поле может содержать символ *, означающий, что ширина столбца изменяется автоматически под ширину окна (увеличивается, если есть место после крайнего правого столбца или уменьшается, если ширина всех столбцов больше ширины виджета
    QS_OBJECT(t_string, StoreDetailHeight) // В АСЦ это поле может содержать символ *, означающий, что ширина столбца изменяется автоматически под ширину окна (увеличивается, если есть место после крайнего правого столбца или уменьшается, если ширина всех столбцов больше ширины виджета
    QS_OBJECT(t_string, PhotoAddState)
    QS_OBJECT(t_int, ArticulTop)
    QS_OBJECT(t_int, ArticulLeft)
    QS_OBJECT(t_int, ArticulWidth)
    QS_OBJECT(t_int, ArticulHeight)
    QS_OBJECT(t_string, ArticulState)
    QS_OBJECT(t_int, ExportLastStore)
    QS_OBJECT(t_int, StoreLastStore)
    QS_OBJECT(t_int, WebCamCurrentResolution)
    QS_OBJECT(t_int, RealizPayTop)
    QS_OBJECT(t_int, RealizPayLeft)
    QS_OBJECT(t_int, RealizPayHeight)
    QS_OBJECT(t_int, RealizPayWidth)
    QS_OBJECT(t_string, RealizPayState)
    QS_OBJECT(t_int, RpLastUserFilter)
    QS_OBJECT(t_bool, SettingsUpgradeRequired)
    QS_OBJECT(t_string, OnlyMyOrders)
    QS_OBJECT(t_string, Theme)
    QS_OBJECT(t_int, NewRepairLastCompany)
    QS_OBJECT(t_string, BackupPath)
    QS_OBJECT(t_int, PriceEditorTop)
    QS_OBJECT(t_int, PriceEditorLeft)
    QS_OBJECT(t_int, PriceEditorWidth)
    QS_OBJECT(t_int, PriceEditorHeight)
    QS_OBJECT(t_string, PriceEditorState)
    QS_OBJECT(t_int, TMTop)
    QS_OBJECT(t_int, TMLeft)
    QS_OBJECT(t_int, TMWidth)
    QS_OBJECT(t_int, TMHeight)
    QS_OBJECT(t_string, TMState)
    QS_OBJECT(t_int, PlayerTop)
    QS_OBJECT(t_int, PlayerLeft)
    QS_OBJECT(t_int, PlayerWidth)
    QS_OBJECT(t_int, PlayerHeight)
    QS_OBJECT(t_bool, IncludePn)
    QS_OBJECT(t_bool, IncludeDescription)
    QS_OBJECT(t_bool, IncludePrice)
    QS_OBJECT(t_bool, IncludePrice4Sc)
    QS_OBJECT(t_bool, IncludePriceOpt)
    QS_OBJECT(t_bool, IncludePriceOpt2)
    QS_OBJECT(t_bool, IncludePriceOpt3)
    QS_OBJECT(t_bool, IncludePhoto)
    QS_OBJECT(t_string, LastSticker)
    QS_OBJECT(t_bool, UserFieldsSearch)
    QS_OBJECT(t_string, CategoriesState)
    QS_OBJECT(t_bool, OnlyInMyOffice)
    QS_OBJECT(t_int, RcardRow3)
    QS_OBJECT(t_int, RcardRow1)
    QS_OBJECT(t_string, FontFamily)
    QS_OBJECT(t_string, RpFilter)
    QS_OBJECT(t_bool, ClShowArchive)
    QS_OBJECT(t_string, DocsPrinter)
    QS_OBJECT(t_string, StickersPrinter)
    QS_OBJECT(t_bool, TasksVisible)
    QS_OBJECT(t_bool, StockTakingPhotoVis)
    QS_OBJECT(t_bool, NewRepLeftPanel)
    QS_OBJECT(t_bool, KassaSidebarExpanded)
    QS_OBJECT(t_bool, DocsSidebarExpanded)
    QS_OBJECT(t_bool, CartridgesOnly)
    QS_OBJECT(t_bool, ArrivalSidebarExpanded)
    QS_OBJECT(t_bool, StoreSidebarExpanded)
    QS_OBJECT(t_int, IgnoreAfter)
    QS_OBJECT(t_int, TasksPanelLeft)
    QS_OBJECT(t_int, TasksPanelTop)
    QS_OBJECT(t_int, StoreLastOption)
    QS_OBJECT(t_string, ExportPath)
    QS_OBJECT(t_string, PathOfImages)
    QS_OBJECT(t_bool, ExportSidebarExpanded)
    QS_OBJECT(t_bool, SearchArticul)
    QS_OBJECT(t_string, StickerVariant)
    QS_OBJECT(t_int, ExportProductAvailability)
    QS_OBJECT(t_string, SkipUpdate)
    QS_OBJECT(t_string, PriceWorkCategoriesState)
    QS_OBJECT(t_bool, SSLConnection)
    QS_OBJECT(t_string, SSLKey)
    QS_OBJECT(t_string, SSLCert)
    QS_OBJECT(t_string, SSLCA)
    QS_OBJECT(t_string, SSLCAPath)
    QS_OBJECT(t_string, SSLCipher)
};

class t_asc_userSettings : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("userSettings")
    OVERRIDE_CHILD_NODE_TAG_NAME("ASC.Properties.Settings")
    IDENTIFIED_BY_TAG(1)
    QS_OBJECT(t_userSettings, ascPropertiesSettings)
};

/*  */
class t_asc_configuration : public QSerializer
{
public:
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("configuration")
    OVERRIDE_CHILD_NODE_TAG_NAME("userSettings")
    IDENTIFIED_BY_TAG(1)
    QS_OBJECT(t_asc_userSettings, userSettings)
};

#endif // SLOCALSETTINGSSTRUCTS_H
