#include "spagedinterface.h"

SPagedInterface::SPagedInterface(QWidget *parent) : QWidget(parent)
{
    tabLayout = new QGridLayout(this);
    tabLayout->setObjectName(QString::fromUtf8("tabLayout"));
    tabLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    toolboxSwitchPanel = new SToolboxSwitchPanel(this);
    toolboxSwitchPanel->setObjectName(QString::fromUtf8("toolboxSwitchPanel"));
    toolboxSwitchPanel->setMinimumSize(QSize(200, 200));
    line = new QFrame(this);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    tabLayout->addWidget(toolboxSwitchPanel, 0, 0, 1, 1);
    tabLayout->addWidget(line, 0, 1, 1, 1);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));

    tabLayout->addWidget(stackedWidget, 0, 2, 1, 1);


    connect(toolboxSwitchPanel, SIGNAL(buttonToggled(int)), this, SLOT(switchPage(int)));
}

SPagedInterface::~SPagedInterface()
{
    delPages();
}

void SPagedInterface::addButton(const QString &caption, QIcon icon, const int id)
{
    toolboxSwitchPanel->addButton(caption, icon, id);
}

void SPagedInterface::redraw()
{
    toolboxSwitchPanel->redraw();
}

void SPagedInterface::addPage(QWidget *widget, const int page)
{
    i_pages.insert(page, widget);
    stackedWidget->addWidget(widget);
    switchPage(page);
}

void SPagedInterface::switchPage(const int page)
{
    int pageIndex = 0;
    if(!i_pages.contains(page))
    {
        emit initPage(page);
        return;
    }
    pageIndex = stackedWidget->indexOf(i_pages.value(page));
    stackedWidget->setCurrentIndex(pageIndex);

    emit updateBotToolbar(page);
}

void SPagedInterface::delPages()
{
    int key;
    QWidget *w;
    while(!i_pages.isEmpty())
    {
        key = i_pages.lastKey();
        w = i_pages.last();
        i_pages.remove(key);
        stackedWidget->removeWidget(w);
        delete w;
    }
}

