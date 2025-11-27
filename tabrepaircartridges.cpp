#include "tabrepaircartridges.h"
#include "ui_tabrepaircartridges.h"
#include <ProjectGlobals>
#include <tabRepairs>
#include <tabPrintDialog>
#include <SPermissions>
#include <SComSettings>
#include <SRepairModel>
#include <SRepairIssueDialog>
#include <SCartridgeForm>
#include <SCartridgeCard>

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
    connect(ui->buttonSetReadyToIssue, &QPushButton::clicked, this, &tabRepairCartridges::manualSubmit);
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
    QList<SCartridgeForm*> list = existentForms();
    QList<SCartridgeForm*>::const_iterator it = list.constBegin();
    while(it != list.constEnd())
    {
        (*it)->removeWidget();
        it++;
    }
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

void tabRepairCartridges::initRepairCartridgeForm(SCartridgeForm *&form, const int repairId)
{
    form = new SCartridgeForm(this);
    form->setRepair(repairId);
    form->init();
}

void tabRepairCartridges::loadForms(QList<int> *list)
{
    SCartridgeForm *form = nullptr;
    QList<int> currentList;

    for(auto form : existentForms())
    {
        currentList.append(form->repairId());
    }

    for(int i = 0; i < list->count(); i++)
    {
        if(currentList.contains(list->at(i)))
            continue;

        try
        {
            initRepairCartridgeForm(form, list->at(i));
            appendToList(form);
        }
        catch (Global::ThrowType)
        {
            delete form;
        }
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

void tabRepairCartridges::appendToList(SCartridgeForm *form)
{
    if(!form)
        return;

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
    m_issueButtonVisible = permissions->issueDevices;
    bool formReady;
    bool nEmpty = 0;

    if(comSettings->useSimplifiedCartridgeRepair())
        m_readyButtonVisible = 1;        // кнопка видна только в упрощенном режиме работы с картриджами

    for(auto form : existentForms())
    {
        nEmpty = 1;
        formReady = form->isReady();
        m_readyButtonVisible &= formReady;
        m_issueButtonVisible &= formReady;
        if(client && client != form->clientId())
            m_issueButtonVisible &= 0;
        client = form->clientId();
    }
    ui->buttonSetReadyToIssue->setVisible(m_readyButtonVisible && nEmpty);
    ui->buttonIssue->setVisible(m_issueButtonVisible && nEmpty);
}

const QList<SCartridgeForm *> tabRepairCartridges::existentForms()
{
    return ui->scrollAreaWidgetContents->findChildren<SCartridgeForm *>();
}

/* обновление виджетов на формах
*/
void tabRepairCartridges::updateForms()
{
    for(auto form : existentForms())
    {
        form->updateWidgets();
    }
}

void tabRepairCartridges::createCartridgeCardForm(const int id)
{
    try
    {
        SCartridgeCard *cardForm = new SCartridgeCard(id, 0, Qt::SplashScreen, this);
        connect(cardForm, &SCartridgeCard::cardModified, this, &tabRepairCartridges::reloadCardModel);
        connect(cardForm, &SCartridgeCard::onDelete, this, &tabRepairCartridges::closeCartridgeCardForm);
    }
    catch (Global::ThrowType)
    {
    }
}

void tabRepairCartridges::closeCartridgeCardForm()
{
}

void tabRepairCartridges::reloadCardModel(int)
{
    for(auto form : existentForms())
    {
        try
        {
            form->loadCardData();
        }
        catch(Global::ThrowType)
        {
        }
    }
}

void tabRepairCartridges::beginCommit()
{
    m_existentForms = existentForms();
}

int tabRepairCartridges::commitStages()
{
    if(m_existentForms.isEmpty()) // на всякий случай
        Global::throwError(Global::ThrowType::ConditionsError, tr("Не удалось переключить статусы картриджей на Готов; список QList<SCartridgeForm*> пуст"));

    return m_existentForms.count();
}

bool tabRepairCartridges::skip(const int i)
{
    return (m_existentForms.at(i)->model()->stateId() == Global::RepStateIds::Ready);
}

void tabRepairCartridges::commit(const int i)
{
    SRepairModel *repair = m_existentForms.at(i)->model();

    switch(repair->stateId())
    {
        case Global::RepStateIds::InWork: {
            repair->set_stateId(Global::RepStateIds::Ready);
            repair->commit();
            break;
        }
        default: ;
    }
#ifdef QT_DEBUG
//    if(i > 2)
//        Global::throwDebug();
#endif
}

void tabRepairCartridges::endCommit(const int i)
{
    m_existentForms.at(i)->model()->SSingleRowModelBase::load();
}

void tabRepairCartridges::endCommit()
{
    m_existentForms = QList<SCartridgeForm*>();
    closeTab();
}

void tabRepairCartridges::throwHandler(int)
{
    for(auto form : existentForms())
    {
        form->model()->setFieldsFailed();
    }

    updateForms();
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

    if(list.isEmpty())
        return;

    m_dialogIssue = new SDialogIssueRepair(list, Qt::SplashScreen, this);
    connect(m_dialogIssue, &SDialogIssueRepair::printWorksLists, [=](){tabPrintDialog::printCartridgeWorksReports(list, false);});
    connect(m_dialogIssue, &SDialogIssueRepair::issueSuccessfull, this, &tabRepairCartridges::closeTab);
    connect(m_dialogIssue, &SDialogIssueRepair::issueFailed, this, &tabRepairCartridges::updateForms);
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
