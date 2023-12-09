#include "global.h"
#include "tabrepairs.h"
#include "tabrepaircartridges.h"
#include "ui_tabrepaircartridges.h"
#include "tabprintdialog.h"

tabRepairCartridges* tabRepairCartridges::p_instance = nullptr;

tabRepairCartridges::tabRepairCartridges(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepairCartridges)
{
    logUserActivity();

    ui->setupUi(this);
    ui->scrollAreaWidgetContents->installEventFilter(this);

    ui->scrollAreaWidgetContents->setStyleSheet("#scrollAreaWidgetContents {\
                                                    background-color: rgb(255, 255, 255);\
                                                }");
    connect(ui->buttonIssue, &QPushButton::clicked, this, &tabRepairCartridges::createDialogIssue);
    connect(ui->buttonSetReadyToIssue, &QPushButton::clicked, this, &tabRepairCartridges::setReadyToIssue);
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

    for(auto form : existentForms())
    {
        currentList.append(form->repairId());
    }

    for(int i = 0; i < list->count(); i++)
    {
        if(currentList.contains(list->at(i)))
            continue;
        form = new SCartridgeForm(list->at(i));

        if(!form->repairId())   // если не удалось загрузить данные модели ремонта (например, ремонт с таким номером не существует)
        {
            delete form;
            continue;
        }
        appendToReceptList(form);
    }
    delete list;    // этот объект больше не нужен
    if(ui->verticalLayoutCartridges->count() == 0)
        this->deleteLater();
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
    int client = 0;

    m_readyButtonVisible = 0;
    m_issueButtonVisible = 1;
    bool formReady;

    if(comSettings->useSimplifiedCartridgeRepair)
        m_readyButtonVisible = 1;        // кнопка видна только в упрощенном режиме работы с картриджами

    for(auto form : existentForms())
    {
        formReady = form->isReady();
        m_readyButtonVisible &= formReady;
        m_issueButtonVisible &= formReady;
        if(client && client != form->clientId())
            m_issueButtonVisible &= 0;
        client = form->clientId();
    }
    ui->buttonSetReadyToIssue->setVisible(m_readyButtonVisible);
    ui->buttonIssue->setVisible(m_issueButtonVisible);
}

const QList<SCartridgeForm *> tabRepairCartridges::existentForms()
{
    return ui->scrollAreaWidgetContents->findChildren<SCartridgeForm *>();
}

/* Перезагрузка данных модели из БД
*/
void tabRepairCartridges::reloadRepairData()
{
    SRepairModel *repairModel;
    SSaleTableModel *worksAndPartsModel;
    int repairId;

    for(auto form : existentForms())
    {
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

void tabRepairCartridges::reloadCardModel(int)
{
    for(auto form : existentForms())
    {
        form->updateModels();
    }
}

void tabRepairCartridges::setReadyToIssue()
{
    bool ret = 1;

    for(auto form : existentForms())
    {
        form->model()->setState(Global::RepStateIds::Ready);
        ret &= form->model()->commit();
    }

    tabRepairs::refreshIfTabExists();
    if(ret)
    {
        closeTab();
    }
}

void tabRepairCartridges::closeTab()
{
    tabRepairs::refreshIfTabExists();
    this->deleteLater();
}

void tabRepairCartridges::createDialogIssue()
{
    QList<SRepairModel*> list;

    for(auto form : existentForms())
    {
        list.append(form->model());
    }

    m_dialogIssue = new SDialogIssueRepair(list, Qt::SplashScreen, this);
    connect(m_dialogIssue, &SDialogIssueRepair::printWorksLists, [=](){tabPrintDialog::printCartridgeWorksReports(list, false);});
    connect(m_dialogIssue, &SDialogIssueRepair::issueSuccessfull, this, &tabRepairCartridges::closeTab);
    connect(m_dialogIssue, &SDialogIssueRepair::issueFailed, this, &tabRepairCartridges::reloadRepairData);
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

