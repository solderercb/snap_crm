#include "tabclient.h"
#include "ui_tabclient.h"
#include <SPermissions>
#include <SClientModel>
#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlField>
#include <QScrollBar>
#include <SClientSummaryPage>

QMap<int, tabClient*> tabClient::p_instance;

tabClient::tabClient(int id, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabClient),
    m_clientId(id),
    m_editStrategy(SClientModel::EditStrategy::OnManualSubmit)
{
    logUserActivity();

    ui->setupUi(this);
    connect(ui->pages, &SPagedInterface::updateBotToolbar, this, &tabClient::updateBotToolbar);
    connect(ui->pages, &SPagedInterface::initPage, this, &tabClient::initPage);
    connect(this, &tabClient::pageInitFinished, ui->pages, &SPagedInterface::addPage);

    ui->buttonPrint->setEnabled(0);
    ui->buttonSave->setEnabled(0);
    ui->buttonRefresh->setEnabled(0);

    ui->pages->switchPage(0);  // по-умолчанию страница 0 ("Сводка")
    if(permissions->editClients)
        ui->pages->addButton(tr("Редактировать"), QIcon(), Page::Edit);
    ui->pages->addButton(tr("История"), QIcon(), Page::History);
    ui->pages->redraw();
}

tabClient::~tabClient()
{
    delete ui;
    p_instance.remove(m_clientId);
}

QString tabClient::tabTitle()
{
    return tr("Клиент %1").arg(m_clientId);
}

void tabClient::updateBotToolbar(const int page)
{
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

    ui->buttonPrint->setVisible(m_buttonPrintVisible);
    ui->buttonSave->setVisible(m_buttonSaveVisible);
    ui->buttonRefresh->setVisible(m_buttonRefreshVisible);
    ui->lineEditFilter->setVisible(m_lineEditFilterVisible);
}

void tabClient::initPage(const int page)
{
    QWidget *widget;
    switch (page)
    {
        case Page::Summary: widget = new SPageClientSummary(m_clientId); break;
        case Page::Edit: widget = new QWidget(); break;
//        case Page::Repairs: ; break;
//        case Page::Purchases: ; break;
//        case Page::Sales: ; break;
//        case Page::Calls: ; break;
//        case Page::Finances: ; break;
//        case Page::Balance: ; break;
        case Page::History: widget = new QWidget(); break;
        default: return;
    }
    emit pageInitFinished(widget, page);
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

