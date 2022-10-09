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
    connect(ui->toolboxSwitchPanel, SIGNAL(buttonToggled(int)), this, SLOT(switchPage(int)));
    ui->toolboxSwitchPanel->addButton(tr("Редактировать"), QIcon(), 1);
    ui->toolboxSwitchPanel->addButton(tr("История"), QIcon(), 2);
    ui->toolboxSwitchPanel->redraw();
    switchPage(0);  // по-умолчанию страница 0 ("Сводка")
}

tabClient::~tabClient()
{
    delete ui;
    p_instance.remove(m_clientId);   // Обязательно блять!
}

QString tabClient::tabTitle()
{
    return QString();
}

void tabClient::switchPage(const int page)
{
    qDebug().nospace() << "[" << this << "] switchPage() | page = " << page;

    ui->stackedWidget->setCurrentIndex(page);
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

