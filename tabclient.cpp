#include "global.h"
#include "appver.h"
#include "tabclient.h"
#include "ui_tabclient.h"

QMap<int, tabClient*> tabClient::p_instance;

tabClient::tabClient(int id, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabClient),
    m_clientId(id)
{
    ui->setupUi(this);

    ui->buttonPrint->setEnabled(0);
    ui->buttonSave->setEnabled(0);
    ui->buttonRefresh->setEnabled(0);

    connect(ui->toolboxSwitchPanel, SIGNAL(buttonToggled(int)), this, SLOT(switchPage(int)));
    ui->toolboxSwitchPanel->addButton(tr("Редактировать"), QIcon(), Page::Edit);
    ui->toolboxSwitchPanel->addButton(tr("История"), QIcon(), Page::History);
    ui->toolboxSwitchPanel->redraw();
    switchPage(0);  // по-умолчанию страница 0 ("Сводка")
}

tabClient::~tabClient()
{
    delPages();
    delete ui;
    p_instance.remove(m_clientId);   // Обязательно блять!
}

QString tabClient::tabTitle()
{
    return QString();
}

bool tabClient::initPage(const int page)
{
    QWidget *widget;
    switch(page)
    {
        case Page::Summary: widget = new tabClientSummary(m_clientId); break;
        case Page::Edit: if(!permissions->value("11")) return 0; widget = new QWidget(); break;
//        case Page::Repairs: ; break;
//        case Page::Purchases: ; break;
//        case Page::Sales: ; break;
//        case Page::Calls: ; break;
//        case Page::Finances: ; break;
//        case Page::Balance: ; break;
        case Page::History: widget = new QWidget(); break;
        default: return 0;
    }
    m_pages.insert(page, widget);
    ui->stackedWidget->addWidget(widget);
    return 1;
}

void tabClient::delPages()
{
    int key;
    QWidget *w;
    while(!m_pages.isEmpty())
    {
        key = m_pages.lastKey();
        w = m_pages.last();
        m_pages.remove(key);
        ui->stackedWidget->removeWidget(w);
        delete w;
    }
}

void tabClient::updateWidgets()
{
    ui->buttonPrint->setVisible(m_buttonPrintVisible);
    ui->buttonSave->setVisible(m_buttonSaveVisible);
    ui->buttonRefresh->setVisible(m_buttonRefreshVisible);
    ui->lineEditFilter->setVisible(m_lineEditFilterVisible);
}

void tabClient::switchPage(const int page)
{
    int pageIndex = 0;
    if(!m_pages.contains(page))
        if(!initPage(page))
            return;
    pageIndex = ui->stackedWidget->indexOf(m_pages.value(page));
    ui->stackedWidget->setCurrentIndex(pageIndex);

    m_buttonPrintVisible = 0;
    m_buttonSaveVisible = 1;
    m_buttonRefreshVisible = 0;
    m_lineEditFilterVisible = 0;
    switch (page)
    {
        case Page::Summary: m_buttonSaveVisible = 0; break;
        case Page::Edit: if(!m_editStrategy) m_buttonSaveVisible = 0; break;
        default: m_buttonPrintVisible = 1; m_buttonRefreshVisible = 1; m_buttonSaveVisible = 0; m_lineEditFilterVisible = 1;
    }
    updateWidgets();
}

void tabClient::buttonSaveClicked()
{

}

void tabClient::buttonPrintClicked()
{

}

tabClient* tabClient::getInstance(int id, MainWindow *parent)   // singleton: одна вкладка для клиента
{
  if( !p_instance.contains(id) )
    p_instance.insert(id, new tabClient(id, parent));
  return p_instance.value(id);
}

#ifdef QT_DEBUG
void tabClient::randomFill()
{

}

void tabClient::test_scheduler_handler()
{

}

void tabClient::test_scheduler2_handler()
{

}
#endif

