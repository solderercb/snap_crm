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
    connect(ui->buttonIssue, &QPushButton::clicked, this, &tabRepairCartridges::createDialogIssue);
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
    return tabCommon::eventFilter(watched, event);
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
    else
        p_instance->loadForms(list);
    return p_instance;
}

void tabRepairCartridges::loadForms(QList<int> *list)
{
    SCartridgeForm *form;
    QList<int> currentList;
    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        currentList.append(form->repairId());
    }

    for(int i = 0; i < list->count(); i++)
    {
        if(currentList.contains(list->at(i)))
            continue;
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
    connect(form, &SCartridgeForm::createCartridgeCardForm, this, &tabRepairCartridges::createCartridgeCardForm);
    connect(form, &SCartridgeForm::updateParentTab, this, &tabRepairCartridges::updateWidgets);
    updateWidgets();
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

void tabRepairCartridges::createCartridgeCardForm(const int id)
{
    m_cartridgeCardForm = new SCartridgeCard(id, 0, Qt::SplashScreen, this);
    connect(m_cartridgeCardForm, &SCartridgeCard::cardModified, this, &tabRepairCartridges::reloadCardModel);
    connect(m_cartridgeCardForm, &SCartridgeCard::onDelete, this, &tabRepairCartridges::closeCartridgeCardForm);
}

void tabRepairCartridges::closeCartridgeCardForm()
{
}

void tabRepairCartridges::reloadCardModel(int id)
{
    SCartridgeForm *form;
    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        form->updateModels();
    }
}

void tabRepairCartridges::createDialogIssue()
{
    QList<SRepairModel*> list;
    SCartridgeForm *form;

    for(int i = 0; i < ui->verticalLayoutCartridges->count(); i++)
    {
        if(ui->verticalLayoutCartridges->itemAt(i)->widget() == nullptr)
            continue;

        form = static_cast<SCartridgeForm *>(ui->verticalLayoutCartridges->itemAt(i)->widget());
        list.append(form->model());
    }

    m_dialogIssue = new SDialogIssueRepair(list, Qt::SplashScreen, this);
    connect(m_dialogIssue, &SDialogIssueRepair::onDelete, this, &tabRepairCartridges::closeDialogIssue);
    connect(m_dialogIssue, &SDialogIssueRepair::issueFailed, this, &tabRepairCartridges::reloadRepairData);
}

void tabRepairCartridges::closeDialogIssue()
{
}

#ifdef QT_DEBUG
void tabRepairCartridges::randomFill()
{

}

void tabRepairCartridges::test_scheduler_handler()
{

}

void tabRepairCartridges::test_scheduler2_handler()
{

}
#endif

