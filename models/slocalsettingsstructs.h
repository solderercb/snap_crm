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
    QS_XML_FIELD(bool, value)
};

class t_int : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("setting")
    OVERRIDE_CHILD_NODE_TAG_NAME("value")
    IDENTIFIED_BY_TAG(1)
    QS_XML_FIELD(int, value)
};

class t_string : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("setting")
    OVERRIDE_CHILD_NODE_TAG_NAME("value")
    IDENTIFIED_BY_TAG(1)
    QS_XML_FIELD(QString, value)
};

class t_userSettings : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("userSettings")
    OVERRIDE_CHILD_NODE_TAG_NAME("setting")
    QS_XML_OBJECT(t_string, dbHost)
    QS_XML_OBJECT(t_string, dbPort)
    QS_XML_OBJECT(t_string, dbName)
    QS_XML_OBJECT(t_string, lastLogin)
    QS_XML_OBJECT(t_int, StoreLastCat)
    QS_XML_OBJECT(t_int, WorkspaceTop)
    QS_XML_OBJECT(t_int, WorkspaceLeft)
    QS_XML_OBJECT(t_int, WorkspaceWidth)
    QS_XML_OBJECT(t_int, WorkspaceHeight)
    QS_XML_OBJECT(t_string, WorkspaceState)
    QS_XML_OBJECT(t_bool, ACPDisplayArhiveUsers)
    QS_XML_OBJECT(t_string, language)
    QS_XML_OBJECT(t_int, PreviewTop)
    QS_XML_OBJECT(t_int, PreviewLeft)
    QS_XML_OBJECT(t_int, PreviewWidth)
    QS_XML_OBJECT(t_int, PreviewHeight)
    QS_XML_OBJECT(t_string, PreviewMaximized)
    QS_XML_OBJECT(t_string, LastCamera)
    QS_XML_OBJECT(t_bool, LastCameraSet)
    QS_XML_OBJECT(t_int, LookLeft)
    QS_XML_OBJECT(t_int, LookTop)
    QS_XML_OBJECT(t_int, WorkspaceBackground)
    QS_XML_OBJECT(t_int, LastCompany)
    QS_XML_OBJECT(t_int, koLeft)
    QS_XML_OBJECT(t_int, koTop)
    QS_XML_OBJECT(t_int, PhotoAddTop)
    QS_XML_OBJECT(t_int, PhotoAddLeft)
    QS_XML_OBJECT(t_int, PhotoAddWidth)
    QS_XML_OBJECT(t_int, PhotoAddHeight)
    QS_XML_OBJECT(t_int, StoreCatsWidth)
    QS_XML_OBJECT(t_string, StoreArticulHeight) // В АСЦ это поле может содержать символ *, означающий, что ширина столбца изменяется автоматически под ширину окна (увеличивается, если есть место после крайнего правого столбца или уменьшается, если ширина всех столбцов больше ширины виджета
    QS_XML_OBJECT(t_string, StoreDetailHeight) // В АСЦ это поле может содержать символ *, означающий, что ширина столбца изменяется автоматически под ширину окна (увеличивается, если есть место после крайнего правого столбца или уменьшается, если ширина всех столбцов больше ширины виджета
    QS_XML_OBJECT(t_string, PhotoAddState)
    QS_XML_OBJECT(t_int, ArticulTop)
    QS_XML_OBJECT(t_int, ArticulLeft)
    QS_XML_OBJECT(t_int, ArticulWidth)
    QS_XML_OBJECT(t_int, ArticulHeight)
    QS_XML_OBJECT(t_string, ArticulState)
    QS_XML_OBJECT(t_int, ExportLastStore)
    QS_XML_OBJECT(t_int, StoreLastStore)
    QS_XML_OBJECT(t_int, WebCamCurrentResolution)
    QS_XML_OBJECT(t_int, RealizPayTop)
    QS_XML_OBJECT(t_int, RealizPayLeft)
    QS_XML_OBJECT(t_int, RealizPayHeight)
    QS_XML_OBJECT(t_int, RealizPayWidth)
    QS_XML_OBJECT(t_string, RealizPayState)
    QS_XML_OBJECT(t_int, RpLastUserFilter)
    QS_XML_OBJECT(t_bool, SettingsUpgradeRequired)
    QS_XML_OBJECT(t_string, OnlyMyOrders)
    QS_XML_OBJECT(t_string, Theme)
    QS_XML_OBJECT(t_int, NewRepairLastCompany)
    QS_XML_OBJECT(t_string, BackupPath)
    QS_XML_OBJECT(t_int, PriceEditorTop)
    QS_XML_OBJECT(t_int, PriceEditorLeft)
    QS_XML_OBJECT(t_int, PriceEditorWidth)
    QS_XML_OBJECT(t_int, PriceEditorHeight)
    QS_XML_OBJECT(t_string, PriceEditorState)
    QS_XML_OBJECT(t_int, TMTop)
    QS_XML_OBJECT(t_int, TMLeft)
    QS_XML_OBJECT(t_int, TMWidth)
    QS_XML_OBJECT(t_int, TMHeight)
    QS_XML_OBJECT(t_string, TMState)
    QS_XML_OBJECT(t_int, PlayerTop)
    QS_XML_OBJECT(t_int, PlayerLeft)
    QS_XML_OBJECT(t_int, PlayerWidth)
    QS_XML_OBJECT(t_int, PlayerHeight)
    QS_XML_OBJECT(t_bool, IncludePn)
    QS_XML_OBJECT(t_bool, IncludeDescription)
    QS_XML_OBJECT(t_bool, IncludePrice)
    QS_XML_OBJECT(t_bool, IncludePrice4Sc)
    QS_XML_OBJECT(t_bool, IncludePriceOpt)
    QS_XML_OBJECT(t_bool, IncludePriceOpt2)
    QS_XML_OBJECT(t_bool, IncludePriceOpt3)
    QS_XML_OBJECT(t_bool, IncludePhoto)
    QS_XML_OBJECT(t_string, LastSticker)
    QS_XML_OBJECT(t_bool, UserFieldsSearch)
    QS_XML_OBJECT(t_string, CategoriesState)
    QS_XML_OBJECT(t_bool, OnlyInMyOffice)
    QS_XML_OBJECT(t_int, RcardRow3)
    QS_XML_OBJECT(t_int, RcardRow1)
    QS_XML_OBJECT(t_string, FontFamily)
    QS_XML_OBJECT(t_string, RpFilter)
    QS_XML_OBJECT(t_bool, ClShowArchive)
    QS_XML_OBJECT(t_string, DocsPrinter)
    QS_XML_OBJECT(t_string, StickersPrinter)
    QS_XML_OBJECT(t_string, PosPrinter)
    QS_XML_OBJECT(t_bool, TasksVisible)
    QS_XML_OBJECT(t_bool, StockTakingPhotoVis)
    QS_XML_OBJECT(t_bool, NewRepLeftPanel)
    QS_XML_OBJECT(t_bool, KassaSidebarExpanded)
    QS_XML_OBJECT(t_bool, DocsSidebarExpanded)
    QS_XML_OBJECT(t_bool, CartridgesOnly)
    QS_XML_OBJECT(t_bool, ArrivalSidebarExpanded)
    QS_XML_OBJECT(t_bool, StoreSidebarExpanded)
    QS_XML_OBJECT(t_int, IgnoreAfter)
    QS_XML_OBJECT(t_int, TasksPanelLeft)
    QS_XML_OBJECT(t_int, TasksPanelTop)
    QS_XML_OBJECT(t_int, StoreLastOption)
    QS_XML_OBJECT(t_string, ExportPath)
    QS_XML_OBJECT(t_string, PathOfImages)
    QS_XML_OBJECT(t_bool, ExportSidebarExpanded)
    QS_XML_OBJECT(t_bool, SearchArticul)
    QS_XML_OBJECT(t_string, StickerVariant)
    QS_XML_OBJECT(t_int, ExportProductAvailability)
    QS_XML_OBJECT(t_string, SkipUpdate)
    QS_XML_OBJECT(t_string, PriceWorkCategoriesState)
    QS_XML_OBJECT(t_bool, SSLConnection)
    QS_XML_OBJECT(t_string, SSLKey)
    QS_XML_OBJECT(t_string, SSLCert)
    QS_XML_OBJECT(t_string, SSLCA)
    QS_XML_OBJECT(t_string, SSLCAPath)
    QS_XML_OBJECT(t_string, SSLCipher)
    QS_XML_OBJECT(t_string, ASCExecutablePath)
};

class t_asc_userSettings : public QSerializer
{
    Q_GADGET
    QS_SERIALIZABLE
    OVERRIDE_TAG_NAME("userSettings")
    OVERRIDE_CHILD_NODE_TAG_NAME("ASC.Properties.Settings")
    IDENTIFIED_BY_TAG(1)
    QS_XML_OBJECT(t_userSettings, ascPropertiesSettings)
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
    QS_XML_OBJECT(t_asc_userSettings, userSettings)
};

#endif // SLOCALSETTINGSSTRUCTS_H
