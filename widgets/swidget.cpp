#include "swidget.h"
#include "mainwindow.h"
#include "tabcommon.h"
#include "global.h"
#include "models/susersettings.h"

SWidget::SWidget(QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent, flags)
{
    connect(userDbData, &SUserSettings::fontSizeChanged, this, &SWidget::guiFontChanged);
#ifdef QT_DEBUG
    i_queryLog = new SQueryLog();
#endif
}

SWidget::~SWidget()
{
#ifdef QT_DEBUG
    delete i_queryLog;
#endif
}

/* Возвращающает объект QFont с новым размером (в дальнейшем и типом); предназначен для использования в методе guiFontChanged();
*/
QFont SWidget::guiFont()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize);

    return font;
}

/* Dозвращающает указатель на объект вкладки с текущим виджетом; предназначен для связи различных вкладок между собой (например, при выборе клиента на вкладке создания ПКО);
*/
SWidget *SWidget::findParentTab()
{
    MainWindow *mw = MainWindow::getInstance();
    QList<tabCommon*> tabs;
    QList<SWidget*> widgets;

    tabs = mw->findChildren<tabCommon*>();
    for(auto tab : qAsConst(tabs))
    {
        widgets = tab->findChildren<SWidget*>();
        for(auto widget : qAsConst(widgets))
        {
            if(widget == this)
            {
                return tab;
            }
        }
    }

    return nullptr;
}
