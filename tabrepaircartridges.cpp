#include "global.h"
#include "tabrepaircartridges.h"
#include "ui_tabrepaircartridges.h"

tabRepairCartridges* tabRepairCartridges::p_instance = nullptr;

tabRepairCartridges::tabRepairCartridges(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepairCartridges)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->installEventFilter(this);

    ui->scrollAreaWidgetContents->setStyleSheet("#scrollAreaWidgetContents {\
                                                    background-color: rgb(255, 255, 255);\
                                                }");
    connect(ui->buttonIssue, &QPushButton::clicked, this, &tabRepairCartridges::createGetOutDialog);
}

bool tabRepairCartridges::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->scrollAreaWidgetContents)
    {
        if(event->type() == QEvent::ChildRemoved)
        {
            updateWidgets();
        }
    }
}

tabRepairCartridges::tabRepairCartridges(QList<int> *list, MainWindow *parent) :
    tabRepairCartridges(parent)
{
    loadForms(list);
}

tabRepairCartridges::~tabRepairCartridges()
{
    delete ui;
    p_instance = nullptr;   // Обязательно блять!
}

tabRepairCartridges *tabRepairCartridges::getInstance(QList<int> *list, MainWindow *parent)
{
    if( !p_instance )
        p_instance = new tabRepairCartridges(list, parent);
    return p_instance;
}

void tabRepairCartridges::loadForms(QList<int> *list)
{
    SCartridgeForm *form;
    for(int i = 0; i < list->count(); i++)
    {
        form = new SCartridgeForm(list->at(i));
        appendToReceptList(form);
    }
}

bool tabRepairCartridges::tabCloseRequest()
{
    return 1;
}

QString tabRepairCartridges::tabTitle()
{
    return tr("Заправка картриджей");
}

void tabRepairCartridges::appendToReceptList(SCartridgeForm *form)
{
    ui->verticalLayoutCartridges->addWidget(form);
    connect(form, &SCartridgeForm::createTabClient, this, &tabRepairCartridges::createTabClient);
    connect(form, &SCartridgeForm::createTabRepair, this, &tabRepairCartridges::createTabRepair);
    connect(form, &SCartridgeForm::updateParentTab, this, &tabRepairCartridges::updateWidgets);
    form->initWidgets();
}

/* Обновление состояния кнопок вкладки
*/
void tabRepairCartridges::updateWidgets()
{
    SCartridgeForm *form;
    int client = 0;

    m_readyButtonVisible = 1;
    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        m_readyButtonVisible &= form->isReady();
        if(client && client != form->clientId())
            m_readyButtonVisible &= 0;
        client = form->clientId();
    }
    ui->buttonIssue->setVisible(m_readyButtonVisible);
}

/* Перезагрузка данных модели из БД
*/
void tabRepairCartridges::reloadRepairData()
{
    SCartridgeForm *form;
    SRepairModel *repairModel;
    SSaleTableModel *worksAndPartsModel;
    int repairId;

    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        repairModel = form->model();
        repairId = repairModel->id();
        worksAndPartsModel = repairModel->worksAndPartsModel();
        repairModel->load(repairId);
        worksAndPartsModel->repair_loadTable(repairId);
    }

}

void tabRepairCartridges::randomFill()
{

}

void tabRepairCartridges::createGetOutDialog()
{
    QList<SRepairModel*> list;
    SCartridgeForm *form;
    overlay = new QWidget(this);
    overlay->setStyleSheet("QWidget { background: rgba(154, 154, 154, 128);}");
    overlay->resize(size());
    overlay->setVisible(true);

    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        list.append(form->model());
    }

    modalWidget = new getOutDialog(list, Qt::SplashScreen, this);
    connect(modalWidget, SIGNAL(close()), this, SLOT(closeGetOutDialog()));
    connect(modalWidget, &getOutDialog::issueFailed, this, &tabRepairCartridges::reloadRepairData);
}

void tabRepairCartridges::closeGetOutDialog()
{
    if(modalWidget != nullptr)
    {
        modalWidget->deleteLater();
        modalWidget = nullptr;
    }
    if (overlay != nullptr)
    {
        overlay->deleteLater();
        overlay = nullptr;
    }
}

void tabRepairCartridges::test_scheduler_handler()
{

}

void tabRepairCartridges::test_scheduler2_handler()
{

}
