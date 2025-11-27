#include "swidget.h"
#include <MainWindow>
#include <tabCommon>
#include <ProjectGlobals>
#include <SUserSettings>

SWidget::SWidget(QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent, flags)
{
    connect(userDbData, &SUserSettings::fontSizeChanged, this, &SWidget::guiFontChanged);
}

SWidget::~SWidget()
{
}

/* Возвращающает объект QFont с новым размером (в дальнейшем и типом); предназначен для использования в методе guiFontChanged();
*/
QFont SWidget::guiFont()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize());

    return font;
}

/* Возвращающает указатель на объект вкладки с текущим виджетом; предназначен для связи различных вкладок между собой (например, при выборе клиента на вкладке создания ПКО);
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
