#include "tabrepair.h"
#include "ui_tabrepair.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QTableWidget>
#include <QLabel>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlField>
#include <QScrollBar>
#include <QTimeZone>
#include <QDateTime>
#include <QLocale>
#include <QClipboard>
#include <ProjectGlobals>
#include <appVer>
#include <Mainwindow>
#include <tabPrintDialog>
#include <tabRepairs>
#include <SComSettings>
#include <SPermissions>
#include <FieldFactory>
#include <SFieldsModel>
#include <SUserSettings>
#include <SCommentModel>
#include <SPhonesModel>
#include <FlashPopup>
#include <SRepairIssueDialog>
#include <SGroupBoxEventFilter>
#include <SRepairModel>
#include <SClientModel>
#include <SRepairStatusLog>
#include <SSortFilterProxyModel>
#include <SSqlQueryModel>

QMap<int, tabRepair*> tabRepair::p_instance;

tabRepair::tabRepair(int rep_id, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepair),
    repair_id(rep_id)
{
    ui->setupUi(this);
    tabRepair::guiFontChanged();

    repairModel = new SRepairModel();
    repairModel->set_id(repair_id);
    clientModel = new SClientModel();
    if(permissions->printStickers)
    {
        ui->lineEditRepairId->setButtons("Print");
        connect(ui->lineEditRepairId, &SLineEdit::buttonClicked, this, &tabRepair::printStickers);
    }
    if(permissions->moveRepairToOffice)
    {
        ui->lineEditOffice->setButtons("Edit");
        connect(ui->lineEditOffice, &SLineEdit::buttonClicked, this, &tabRepair::changeOffice);
    }
    if(permissions->setRepairManager)
    {
        ui->lineEditManager->setButtons("Edit");
        connect(ui->lineEditManager, &SLineEdit::buttonClicked, this, &tabRepair::changeManager);
    }
    if(permissions->setRepairEngineer)
    {
        ui->lineEditEngineer->setButtons("Edit");
        connect(ui->lineEditEngineer, &SLineEdit::buttonClicked, this, &tabRepair::changeEngineer);
    }
    if(permissions->handleCashlessOrders)
    {
        ui->lineEditInvoice->setButtons("Open");
        connect(ui->lineEditInvoice, &SLineEdit::buttonClicked, this, &tabRepair::openInvoice);
    }
    if(permissions->editRepairIncomingSet)
    {
        ui->lineEditIncomingSet->setButtons("Edit");
        connect(ui->lineEditIncomingSet, &SLineEdit::buttonClicked, this, &tabRepair::editIncomingSet);
    }
    if(userDbData->autosaveDiagResult())
    {
        m_autosaveDiag = 1;
        m_autosaveDiagTimer = new QTimer();
        m_autosaveDiagTimer->setSingleShot(true);
        QObject::connect(m_autosaveDiagTimer, SIGNAL(timeout()), this, SLOT(autosaveDiagAmount()));
    }
    if(userDbData->saveStateOnClose())
    {
        ui->toolButtonSaveState->setHidden(true);
        ui->comboBoxState->disableWheelEvent(true);  // если включено автосохранение статуса ремонта, то нужно игнорировать колёсико мышки
    }
    else
        connect(ui->toolButtonSaveState, &QToolButton::clicked, this, qOverload<>(&tabRepair::saveState));
    connect(ui->comboBoxState, qOverload<int>(&QComboBox::currentIndexChanged), this, &tabRepair::comboBoxStateIndexChanged);

    ui->comboBoxState->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxNotifyStatus->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPlace->setStyleSheet(commonComboBoxStyleSheet);

    additionalFieldsModel = new SFieldsModel(std::make_unique<SRepairField>());
    statusesProxyModel = new SSortFilterProxyModel;
    statusesProxyModel->setSourceModel(comSettings->repairStatusesVariantCopy().Model);
    connect(repairModel, &SRepairModel::modelUpdated, this, &tabRepair::updateWidgets);
    connect(repairModel, &SRepairModel::modelUpdated, this, [=]{modelRO = repairModel->isLock();});

    ui->comboBoxPlace->setButtons("Clear");
    ui->comboBoxPlace->setModel(repairBoxesModel);
    connect(ui->comboBoxPlace, qOverload<int>(&QComboBox::currentIndexChanged), this, &tabRepair::savePlace);
    connect(ui->comboBoxPlace, &SComboBox::buttonClicked, this, &tabRepair::comboBoxPlaceButtonClickHandler);
    ui->comboBoxState->blockSignals(true);
    ui->comboBoxState->setModel(statusesProxyModel);
    statusesProxyModel->setFilterKeyColumn(Global::RepStateHeaders::Id);
    statusesProxyModel->setFilterRegularExpression("");
    ui->comboBoxState->blockSignals(false);
    ui->comboBoxNotifyStatus->disableWheelEvent(true);
    ui->comboBoxNotifyStatus->blockSignals(true);
    ui->comboBoxNotifyStatus->setModel(notifyStatusesModel);
    ui->comboBoxNotifyStatus->blockSignals(false);

    ui->textEditDiagResult->setMinimumHeight(3);
    ui->widgetPartsRequests->setReadOnly(true);
    ui->widgetPartsRequests->setTableLayout(SLocalSettings::RepairPartsRequestsGrid);

    ui->widgetBOQ->linkWithRepairModel(repairModel);
    ui->widgetBOQ->setReadOnly(true);

    ui->widgetComments->setParentTab(this);
    ui->widgetComments->setMode(SCommentModel::Repair);

    // Узкая полоса прокрутки; стиль задаётся здесь, т. к. если задать его в ui-файле он будет унаследован дочерними виджетами
    QString styleSheet = "\
        QScrollBar:vertical{  background-color: rgba(0,0,0,30);  width: 8px;  margin: 21px 0 21px 0;}\
        QScrollBar::handle:vertical{  background-color: rgba(0,0,0,50);  min-height: 25px;}\
        QScrollBar::add-line:vertical{  border: 1px solid rgba(200, 200, 200, 195);	background-color: rgba(200, 200, 200, 127);  height: 20px;  subcontrol-position: bottom;  subcontrol-origin: margin;}\
        QScrollBar::sub-line:vertical{  border: 1px solid rgba(200, 200, 200, 195);  background-color: rgba(200, 200, 200, 127);  height: 20px;  subcontrol-position: top;  subcontrol-origin: margin;}";
    ui->leftScrollArea->verticalScrollBar()->setStyleSheet(styleSheet);
    ui->rightScrollArea->verticalScrollBar()->setStyleSheet(styleSheet);

    // сворачивание групп элементов (ну как в АСЦ чтобы). TODO: Отключено, т. к. требует доработки класса SGroupBoxEventFilter
//    groupBoxEventFilter = new SGroupBoxEventFilter(this);
//    ui->groupBoxDeviceSummary->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxDiagResult->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxWorksAndSpareParts->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxComments->installEventFilter(groupBoxEventFilter);

    this->setAttribute(Qt::WA_DeleteOnClose);

    try
    {
        m_repairLockUpdateTimer = new QTimer(this);
        m_repairLockUpdateTimer->setSingleShot(true);
        connect(m_repairLockUpdateTimer, &QTimer::timeout, this, [=]{setLock();});
        checkViewPermission();
        logUserActivity();
        loadData();
        setLock();
        m_repairLockUpdateTimer->start(repairModel->lockTimeout()*1000);
    }
    catch(Global::ThrowType)
    {
        this->deleteLater();
        return;
    }

#ifdef QT_DEBUG
    if( m_getOutButtonVisible && (repairModel->stateId() == Global::RepStateIds::Ready || repairModel->stateId() == Global::RepStateIds::ReadyNoRepair) )
        createDialogIssue();
    connect(ui->pushButtonManualUpdateRepairData, SIGNAL(clicked()), this, SLOT(reloadData()));
#else
    ui->pushButtonManualUpdateRepairData->setHidden(true);
#endif
}

tabRepair::~tabRepair()
{
    delAdditionalFieldsWidgets();
    delete statusesProxyModel;
    delete additionalFieldsModel;
    delete ui;
    delete repairModel;
    delete clientModel;
    if(m_autosaveDiag)
    {
        delete m_autosaveDiagTimer;
    }
    if(i_tabIcon)
        delete i_tabIcon;
    if(m_repairLockUpdateTimer)
        delete m_repairLockUpdateTimer;
    p_instance.remove(repair_id);   // Обязательно блять!
}

QString tabRepair::tabTitle()
{
    return tr("Ремонт", "repair tab title") + " " + QString::number(repair_id);
}

bool tabRepair::tabCloseRequest()
{
    // Проверка на несохранённые статус ремонта (если в настройках не включено автосохранение), результат диагностики или список работ и деталей
    // TODO: Проверка несохранённого комментария
    if((!userDbData->saveStateOnClose() && (ui->comboBoxState->currentIndex() >= 0)) || (!m_autosaveDiag && (m_diagChanged || m_spinBoxAmountChanged)) || ui->widgetBOQ->isDirty())
    {
        auto result = QMessageBox::question(this, tr("Данные не сохранены"), tr("Статус, результат диагностики, согласованная сумма или список работ и деталей не сохранены!\nСохранить перед закрытием?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (result == QMessageBox::Cancel)
        {
            return 0;
        }
        else if (result == QMessageBox::Yes)
        {
            m_opType = SaveBeforeClose;
            if(!manualSubmit())
                return 0;
        }
    }

    m_repairLockUpdateTimer->stop();
    setLock(0);
    return 1;
}

void tabRepair::refreshIfTabExists(const int repairId)
{
    QList<tabRepair*> repairTabs;

    repairTabs = MainWindow::getInstance()->findChildren<tabRepair*>();
    if(repairTabs.count())
    {
        foreach (auto tab, repairTabs)
        {
            if(tab->repair_id != repairId)
                continue;

            tab->reloadData();
            return;
        }
    }
}

void tabRepair::loadData()
{
    repairModel->load(repair_id);
    if(repairModel->clientId() != m_clientId)  // перезагрузка данных клиента только при первом вызове метода или если был изменён клиент
    {
        m_clientId = repairModel->clientId();
        clientModel->load(m_clientId);
    }

    additionalFieldsModel->load(repair_id);

    reloadRequestsList();

    ui->widgetComments->load(repair_id);

    // Перезагрузка таблицы работ и деталей (а также обновление статуса) только если она сохранена
    if(!ui->widgetBOQ->isDirty())
    {
        ui->widgetBOQ->load(repair_id);
        statusesProxyModel->setFilterRegularExpression("");
        updateStatesModel();
        setWidgetsParams(repairModel->stateId());
        updateWidgets();
    }

}

void tabRepair::reloadData()
{
    try
    {
        loadData();
    }
    catch(Global::ThrowType)
    {
        // TODO: выключение всех виджетов, чтобы спровоцировать пользователя закрыть вкладку
        //       или создание виджета, перекрывающего элементы вкладки и отображающего информацию о сбое
    }

}

void tabRepair::reloadRequestsList()
{
    FilterList filter;
    filter.op = FilterList::And;
    filter.fields.append(STableViewBase::initFilterField("t1.`repair`", FilterField::Equals, repair_id));
    filter.fields.append(STableViewBase::initFilterField("t1.`state`", FilterField::NotMark|FilterField::InSet, SPartRequest::State::Cancelled));
    ui->widgetPartsRequests->setFilter(filter);
    ui->widgetPartsRequests->refresh();
}

void tabRepair::updateWidgets()
{
    ui->lineEditRepairId->setText(QString::number(repair_id));
    ui->lineEditDevice->setText(repairModel->title());
    ui->lineEditSN->setText(repairModel->serialNumber());
    ui->textEditClient->setPlainText(clientModel->fullLongName().toString());
    ui->labelPrimaryPhone->setVisible(permissions->viewClients);
    ui->lineEditPrimaryPhone->setVisible(permissions->viewClients);
    ui->lineEditPrimaryPhone->setText(clientModel->phones()->primaryStr());
    ui->lineEditInDate->setText(repairModel->createdStr());
    setInfoWidgetVisible(ui->lineEditOutDate, m_outDateVisible);
    ui->lineEditOutDate->setText(repairModel->outDateTime().toString());
    ui->pushButtonGetout->setVisible(m_getOutButtonVisible && !modelRO);
    ui->buttonClientCard->setVisible(permissions->viewClients);
    ui->buttonCall->setVisible(permissions->useTelephony);
    ui->buttonAdm->setVisible(permissions->advEditRepair);
    setInfoWidgetVisible(ui->lineEditExtPrevRepair, !repairModel->extEarly().isEmpty());
    ui->lineEditExtPrevRepair->setText(repairModel->extEarly());
    ui->lineEditOffice->setText(officesModel->getDisplayRole(repairModel->officeId()));
    ui->lineEditManager->setText(allUsersModel->value(repairModel->currentManagerId(), "id", "username").toString());
    ui->lineEditEngineer->setText(allUsersModel->value(repairModel->engineerId(), "id", "username").toString());
    setInfoWidgetVisible(ui->lineEditPreagreedAmount, repairModel->isPreAgreed());
    ui->lineEditPreagreedAmount->setText(sysLocale.toCurrencyString(repairModel->preAgreedAmount()));        // TODO: заменить системное обозначение валюты на валюту заданную в таблице БД config

    updateComboBoxPlace();
    ui->comboBoxPlace->setEnabled(!modelRO);
    ui->lineEditColor->setStyleSheet(QString("background-color: %1;").arg(repairModel->color()));
    ui->lineEditWarrantyLabel->setText(repairModel->warrantyLabel());

    if(repairModel->early())
        ui->lineEditPrevRepair->setText(QString::number(repairModel->early()));

    fillExtraInfo();
    if( repairModel->paymentSystem() == 1 && permissions->handleCashlessOrders )    // указана Безналичная оплата и есть разрешение Работать с безналичными счетами
    { // TODO: нужен более гибкий способ определения безналичного рассчета

        ui->groupBoxCashless->setHidden(false);
        if(repairModel->invoiceId()) // если уже выставлен счет
        {
            ui->lineEditInvoiceAmount->setText("TODO:");
            ui->lineEditInvoice->setText(QString("id=%1; TODO:").arg(repairModel->invoiceId()));
            ui->labelInvoice->setHidden(false);
            ui->lineEditInvoice->setHidden(false);
            ui->lineEditInvoicePaymentDate->setText("TODO:");
            ui->labelInvoicePaymentDate->setHidden(false);
            ui->lineEditInvoicePaymentDate->setHidden(false);
            ui->pushButtonCreateInvoice->setHidden(true);
            ui->labelInvoiceAmount->setHidden(false);
            ui->lineEditInvoiceAmount->setHidden(false);
        }
        else
        {
            ui->labelInvoice->setHidden(true);
            ui->lineEditInvoice->setHidden(true);
            ui->labelInvoicePaymentDate->setHidden(true);
            ui->lineEditInvoicePaymentDate->setHidden(true);
            ui->labelInvoiceAmount->setHidden(true);
            ui->lineEditInvoiceAmount->setHidden(true);
            ui->pushButtonCreateInvoice->setHidden(false);
        }
        ui->pushButtonCreatePrepayOrder->setHidden(true);
    }
    else
    {
        ui->pushButtonCreatePrepayOrder->setHidden(false);
        ui->pushButtonCreateInvoice->setHidden(true);
        ui->groupBoxCashless->setHidden(true);
    }
    ui->pushButtonDebtReceived->setVisible(m_buttonDebtReceivedVisible);

    updateComboBoxInformedStatus();
    ui->comboBoxNotifyStatus->setEnabled(m_comboBoxNotifyStatusEnabled && !modelRO);
    if(repairModel->cartridge())
    {
        ui->groupBoxDeviceSummary->setVisible(false);
        ui->groupBoxDiagResult->setVisible(false);
    }
    ui->lineEditProblem->setText(repairModel->fault());
    ui->lineEditIncomingSet->setText(repairModel->complect());
    ui->lineEditExterior->setText(repairModel->look());

    for(int i = ui->gridLayoutDeviceSummary->rowCount() - 1; i > 2; i-- )
    {
        ui->gridLayoutDeviceSummary->itemAtPosition(i, 1)->widget()->deleteLater();
        ui->gridLayoutDeviceSummary->itemAtPosition(i, 0)->widget()->deleteLater();
    }
    createAdditionalFieldsWidgets();
    ui->textEditDiagResult->blockSignals(true);
    ui->textEditDiagResult->setText(repairModel->diagnosticResult());
    ui->textEditDiagResult->setReadOnly(m_diagRO || modelRO);
    ui->textEditDiagResult->blockSignals(false);
    ui->doubleSpinBoxAmount->blockSignals(true);
    ui->doubleSpinBoxAmount->setDecimals(comSettings->classicKassa()?2:0);
    ui->doubleSpinBoxAmount->setValue(repairModel->repairCost());
    ui->doubleSpinBoxAmount->setReadOnly(m_summRO || modelRO);
    ui->doubleSpinBoxAmount->blockSignals(false);
    ui->pushButtonSaveDiagAmount->setEnabled(!m_summRO && !modelRO);
//    ui->pushButtonCreateInvoice->setEnabled(!modelRO);
//    ui->pushButtonCreatePrepayOrder->setEnabled(!modelRO);
//    ui->lineEditManager->setButtonsVisible(!modelRO);
//    ui->lineEditEngineer->setButtonsVisible(!modelRO);
//    ui->lineEditOffice->setButtonsVisible(!modelRO);
//    ui->lineEditIncomingSet->setButtonsVisible(!modelRO);
//    ui->lineEditWarrantyLabel->setEnabled(!modelRO);
//    ui->lineEditColor->setEnabled(!modelRO);
    ui->comboBoxState->setEnabled(m_comboBoxStateEnabled && !modelRO);
    ui->toolButtonSaveState->setEnabled(m_buttonSaveStateEnabled && !modelRO);

    ui->widgetBOQ->setReadOnly(m_BOQModelRO);
    ui->widgetBOQ->updateWidgets();
    ui->widgetPartsRequests->setVisible(ui->widgetPartsRequests->requestCount() > 0);
}

void tabRepair::fillExtraInfo()
{
    ui->listWidgetExtraInfo->setHidden(true);
    ui->listWidgetExtraInfo->clear();
    ui->listWidgetExtraInfo->addItems(clientModel->optionsList());
    if(repairModel->thirsPartySc())
        ui->listWidgetExtraInfo->addItem(tr("было в другом СЦ"));
    if(repairModel->canFormat())
        ui->listWidgetExtraInfo->addItem(tr("данные не важны"));
    if(repairModel->isExpressRepair())
        ui->listWidgetExtraInfo->addItem(tr("срочный"));
    if(repairModel->isRepeat())
        ui->listWidgetExtraInfo->addItem(tr("повтор"));
    if(repairModel->isWarranty())
        ui->listWidgetExtraInfo->addItem(tr("гарантия"));
    if(repairModel->printCheck())
        ui->listWidgetExtraInfo->addItem(tr("чек при выдаче"));
    if(repairModel->isPrepaid())
        ui->listWidgetExtraInfo->addItem(QString(tr("предоплата: %1")).arg(sysLocale.toCurrencyString(repairModel->paymentsAmount())));
//    if(repairModel->isDebt())  // используется вместе со статусом "Выдано в долг", но не отображается в карте ремонта
//        ui->listWidgetExtraInfo->addItem("");
    if(ui->listWidgetExtraInfo->count())
        ui->listWidgetExtraInfo->setHidden(false);
}

void tabRepair::setLock(bool state)
{
    if(repairModel->isDirty())   // таймер может сработать во время выполнения какой-то операции; результат не предсказуем
    {
        m_repairLockUpdateTimer->start(1000);  // откладываем блокировку на x миллисекунд
        return;
    }

    if(state && modelRO)
    {
        i_tabIcon = new QIcon(":/icons/light/1F512_32.png");
        return;
    }

    if(!state && i_tabIcon)
    {
        delete i_tabIcon;
        i_tabIcon = nullptr;
    }

    if(modelRO)
        return;

    repairModel->lock(state);
    m_repairLockUpdateTimer->start(repairModel->lockTimeout()*1000);
}

void tabRepair::createAdditionalFieldsWidgets()
{
    delAdditionalFieldsWidgets();
    int i;

    i = 0;
    foreach(auto field, additionalFieldsModel->valuesList())
    {
        QLabel *label = new QLabel(field->name());  // эти объекты удаляются в методе delAdditionalFieldsWidgets()
        QLineEdit *lineEdit = new QLineEdit();
        additionalFieldsWidgets.append(label);
        additionalFieldsWidgets.append(lineEdit);
        lineEdit->setText(field->value());
        lineEdit->setReadOnly(true);
        ui->gridLayoutDeviceSummary->addWidget(label, i + 3, 0 );
        ui->gridLayoutDeviceSummary->addWidget(lineEdit, i + 3, 1);
        i++;
    }
}

void tabRepair::delAdditionalFieldsWidgets()
{
    QWidget *w;

    while(!additionalFieldsWidgets.isEmpty())
    {
        w = additionalFieldsWidgets.last();
        additionalFieldsWidgets.removeLast();
        ui->gridLayoutDeviceSummary->removeWidget(w);
        delete w;
    }
}

/*  Скрытие/отображение пары виджетов в левой верхней части вкладки (дата выдачи, предварительная стоимость и др.)
*/
void tabRepair::setInfoWidgetVisible(QWidget *field, bool state)
{
    // TODO: нужно сделать скрытие пары виджетов с удалением их из layout, т. к. пустая строка увеличивает зазор между видимыми виджетами
    QWidget *label;
    label = ui->leftScrollAreaFormLayout->labelForField(field);
    field->setVisible(state);
    label->setVisible(state);
}

void tabRepair::setWidgetsParams(const int stateId)
{
    m_getOutButtonVisible = 0;
    m_comboBoxStateEnabled = 1;
    m_buttonSaveStateEnabled = 0;
    m_BOQModelRO = 1;
    m_diagRO = 1;
    m_summRO = 0;   // При наличии пользовательского статуса "Ожидание решения" логика данной опции не работает


    if( stateId == Global::RepStateIds::Ready || stateId == Global::RepStateIds::ReadyNoRepair )
    {
        for(const int nextState : (const QList<int>)comSettings->repairStatusesVariantCopy()[stateId].Contains)
        {
            switch (nextState)
            {
                case Global::RepStateIds::Returned:
                case Global::RepStateIds::ReturnedNoRepair:
                case Global::RepStateIds::ReturnedInCredit: m_getOutButtonVisible = checkStateAcl(nextState); break;
            }

            if(m_getOutButtonVisible)
                break;
        }
    }

    switch (stateId)
    {
        case Global::RepStateIds::Negotiation: m_summRO = 0; break;
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair: m_comboBoxStateEnabled = 0; m_comboBoxNotifyStatusEnabled = 0; Q_FALLTHROUGH();
        case Global::RepStateIds::ReturnedInCredit: m_buttonDebtReceivedVisible = 1; m_outDateVisible = 1; break;
    }

    for(const int action : (const QList<int>)comSettings->repairStatusesVariantCopy()[stateId].Actions)
    {
        switch (action)
        {
            case Global::RepStateActions::EditWorksParts: m_BOQModelRO = 0; break;
            case Global::RepStateActions::EditDiagSumm: m_diagRO = 0; m_summRO = 0; break;
        }
    }
}

/*  Проверка разрешений
 */
bool tabRepair::checkStateAcl(const int stateId)
{
    // проверка разрешенённых для роли статусов (предустановленных) (имеет приоритет над разрешёнными для статуса ролями)
    switch(stateId)
    {
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair:
        case Global::RepStateIds::ReturnedInCredit: return permissions->issueDevices;
    }

    // проверка разрешенённых для статуса ролей (Настройки -» Статусы ремонта)
    const QString allowedForRoles = comSettings->repairStatusesVariantCopy()[stateId].Roles.join('|');
    if(userDbData->roles().contains(QRegularExpression(QString("\\b(%1)\\b").arg(allowedForRoles))))
    {
        return 1;
    }

    return 0;
}

void tabRepair::createDialogIssue()
{
    if(!m_autosaveDiag)
    {
        m_opType = SaveDiagAmount;
        manualSubmit();
    }

    m_opType = SaveBOQ;
    manualSubmit();

    QList<SRepairModel*> list;

    list.append(repairModel);
    m_dialogIssue = new SDialogIssueRepair(list, Qt::SplashScreen, this);
    connect(m_dialogIssue, &SDialogIssueRepair::printWorksLists, [=](){tabPrintDialog::printRepairWorksReports(list, false);});
    connect(m_dialogIssue, &SDialogIssueRepair::issueSuccessfull, this, [=]{clientModel->SSingleRowModelBase::load(); repairModel->SSingleRowModel::load(); endStateChange();});
    connect(m_dialogIssue, &SDialogIssueRepair::issueFailed, this, &tabRepair::updateWidgets);
}

void tabRepair::openPrevRepair()
{
    emit createTabPrevRepair(ui->lineEditPrevRepair->text().toInt());
}

void tabRepair::printStickers(int)
{
    QMap<QString, QVariant> report_vars;
    report_vars.insert("type", Global::Reports::rep_label);
    report_vars.insert("repair_id", repair_id);
    emit generatePrintout(report_vars);
}

void tabRepair::changeOffice(int)
{

}

void tabRepair::changeManager(int)
{

}

void tabRepair::changeEngineer(int index)
{
    Q_UNUSED(index);
//    repairModel->setEngineerIndex(index);
}

void tabRepair::initEngineer()
{
    if(repairModel->engineerId())
        return;

    repairModel->set_engineerId(userDbData->id());
}

/*  Проверка разрешения на открытие чужих карт ремонта
 *  Если пользователю не обладает такими правами, будет сгенерировано исключение
*/
void tabRepair::checkViewPermission()
{
    if(permissions->viewAnyRepair)
    {
        return;
    }

    bool ret = 1;
    int role;
    int repManager = 0;
    int repEngineer = 0;
    QStringList userRoles = userDbData->roles().split(',');
    QSqlQuery query(QSqlDatabase::database("connMain"));

    query.exec(QUERY_SEL_REPAIR_MNGR_ENGR(repair_id));
    if(query.first())
    {
        repManager = query.value(0).toInt();
        repEngineer = query.value(1).toInt();

        foreach(auto r, userRoles)
        {
            role = r.toInt();
            switch (role)
            {
                case Global::UserRoles::Engineer:
                case Global::UserRoles::SeniorEngineer: ret &= (repEngineer != 0) && (repEngineer != userDbData->id()); break;    // инженер может открывать свободные ремонты и свои
                case Global::UserRoles::Manager:
                case Global::UserRoles::SeniorManager: ret &= (repManager != 0) && (repManager != userDbData->id()); break;    // менеджер может открывать только свои ремонты
                case Global::UserRoles::Director: ret = 0; break;
            }

            if(!ret)
                break;
        }
    }

    if(ret)
        Global::throwError(Global::ThrowType::AccessDenied);
}

void tabRepair::openInvoice(int)
{

}

void tabRepair::editIncomingSet(int)
{

}

void tabRepair::setAgreedAmount(int)
{

}

void tabRepair::buttonClientClicked()
{
    emit createTabClient(m_clientId);
}

void tabRepair::updateStatesModel()
{
    int stateId = repairModel->stateId();
    QString allowedStates = comSettings->repairStatusesVariantCopy()[stateId].ContainsStr.join('|');
    ui->comboBoxState->blockSignals(true);
    statusesProxyModel->setFilterRegularExpression(QString("\\b(%1)\\b").arg(allowedStates));
    ui->comboBoxState->setCurrentIndex(-1);
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    ui->comboBoxState->setPlaceholderText(comSettings->repairStatusesVariantCopy()[stateId].Name);
    ui->comboBoxState->blockSignals(false);
}

void tabRepair::doStateActions(const int stateId)
{
    QList<int> stateActions = comSettings->repairStatusesVariantCopy()[stateId].Actions;

    // В АСЦ установка инженера происходит при переключении с "Приём в ремонт" на любой другой.
    // Здесь реализация более гибкая — через действие "Назначить инициатора инженером" в настройках статусов; это позволит
    // создать пользовательские статусы, при включении которых не нужно задавать инженера ремонта.
    // Например, в мастерской разборкой занимается помощник мастеров.
    // Для статуса "Проведение диагностики" действие включено принудительно и от настроек пользователя не зависит
    if(stateId == Global::RepStateIds::Diag && permissions->beginUnengagedRepair)
    {
        stateActions << Global::RepStateActions::SetEngineer;
    }

    // TODO: перенести обработку действий при смене статуса в SRepairModel::doStateActions(QList<int>); список действий передавать в качестве параметра;
    // в ведении классов вкладок доложны остаться манипуляции со списком действий (например, добавление действия RepStateActions::SetEngineer,
    // или удаление ненужных действий при административной правке карточки ремонта)
    for(const int &action : qAsConst(stateActions))
        switch (action)
        {
            case Global::RepStateActions::NoPayDiag: setPricesToZero(); break;
            case Global::RepStateActions::ResetInformedStatus: if(repairModel->informedStatusIndex()) repairModel->setInformedStatusIndex(0); break;
            case Global::RepStateActions::SetEngineer: initEngineer(); break;
            case Global::RepStateActions::InformManager: /*TODO: notifications->create(caption, text);*/; break;
            case Global::RepStateActions::InformEngineer: /*TODO: notifications->create(caption, text);*/; break;
        }
}

void tabRepair::setPricesToZero()
{
//    tableWorksParts->setPricesToZero();
    repairModel->set_repairCost(0);
}

void tabRepair::saveState()
{
    saveState(ui->comboBoxState->currentIndex());
}

void tabRepair::endStateChange()
{
    updateStatesModel();
    setWidgetsParams(repairModel->stateId());
    updateWidgets();
}

void tabRepair::saveState(int)
{
    m_opType = SaveState;
    manualSubmit();
}

void tabRepair::updateComboBoxInformedStatus()
{
    ui->comboBoxNotifyStatus->blockSignals(true);
    ui->comboBoxNotifyStatus->setCurrentIndex(repairModel->informedStatusIndex());
    ui->comboBoxNotifyStatus->blockSignals(false);
}

void tabRepair::setInformedStatus(int)
{
    m_opType = SaveInformedState;
    manualSubmit();
}

void tabRepair::diagChanged()
{
    m_diagChanged = 1;
    ui->pushButtonSaveDiagAmount->setEnabled(true);
    if(m_autosaveDiag)
    {
        m_autosaveDiagTimer->start(10000);
    }
}

void tabRepair::diagEditFinished()  // слот вызывается при потере фокуса
{
    if(m_autosaveDiag)
        autosaveDiagAmount();
}

void tabRepair::spinBoxAmountChanged(double)
{
    m_spinBoxAmountChanged = 1;
    ui->pushButtonSaveDiagAmount->setEnabled(true);
    if(m_autosaveDiag)
    {
        m_autosaveDiagTimer->start(10000);
    }
}

void tabRepair::spinBoxAmountEditingFinished()  // слот вызывается при потере фокуса или нажатии Enter
{
    if(m_autosaveDiag)
        autosaveDiagAmount();
}

void tabRepair::setModelDiagAmount()
{
    if(m_spinBoxAmountChanged)
    {
        repairModel->set_repairCost(ui->doubleSpinBoxAmount->value());
    }
    if(m_diagChanged)
    {
        repairModel->set_diagnosticResult(ui->textEditDiagResult->toPlainText());
    }
}

void tabRepair::saveDiagAmount()
{
    m_opType = SaveDiagAmount;
    manualSubmit();
}

void tabRepair::autosaveDiagAmount()
{
    QTextCursor tc = ui->textEditDiagResult->textCursor();
    int pos = tc.position();
    saveDiagAmount();
    tc.setPosition(pos);    // после сохранения позиция курсора в tc почему-то изменяется (хотя это копия объекта)
    ui->textEditDiagResult->setTextCursor(tc);
}

void tabRepair::diagAmountSaved()
{
    m_diagChanged = 0;
    m_spinBoxAmountChanged = 0;
    ui->pushButtonSaveDiagAmount->setEnabled(false);
    if(m_autosaveDiag)
        m_autosaveDiagTimer->stop();
}

void tabRepair::updateComboBoxPlace()
{
    ui->comboBoxPlace->blockSignals(true);
    ui->comboBoxPlace->setCurrentIndex(repairModel->boxIndex());
    ui->comboBoxPlace->blockSignals(false);
}

void tabRepair::savePlace(int)
{
    m_opType = SavePlace;
    manualSubmit();
}

void tabRepair::comboBoxPlaceButtonClickHandler(int id)
{
    if(id == SLineEdit::Clear)
        savePlace(-1);
}

void tabRepair::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize());

    ui->comboBoxState->setFont(font);
    ui->comboBoxNotifyStatus->setFont(font);
    ui->comboBoxPlace->setFont(font);
}

void tabRepair::comboBoxStateIndexChanged(int index)
{
    if(userDbData->saveStateOnClose())
    {
        saveState(index);
        return;
    }
    else
    {
        m_buttonSaveStateEnabled = 1;
        ui->toolButtonSaveState->setEnabled(true); // нет смысла вызывать метод updateWidgets() ради одной кнопки
    }
}

tabRepair* tabRepair::getInstance(int rep_id, MainWindow *parent)   // singleton: одна вкладка для ремонта
{
    if( !p_instance.contains(rep_id) )
      p_instance.insert(rep_id, new tabRepair(rep_id, parent));
    return p_instance.value(rep_id);
}

bool tabRepair::checkBeforeInformedStatusChange()
{
    int stateId = notifyStatusesModel->databaseIDByRow(ui->comboBoxNotifyStatus->currentIndex());
    if(stateId != Global::ClientInformStateIds::NotSet && !permissions->setNotificationState)
    {
        Global::errorPopupMsg(Global::ThrowType::AccessDenied);
        updateComboBoxInformedStatus();
        return 1;
    }

    return 0;
}

bool tabRepair::checkBeforePlaceChange()
{
    return (repairModel->boxIndex() == ui->comboBoxPlace->currentIndex());
}

bool tabRepair::checkBeforeStateChange()
{
    if(ui->comboBoxState->currentIndex() == -1) // SAbstractItemModel::databaseIDByRow() для -1 вернёт 0, соответствующий статусу Приём в ремонт
        return 1;

    int stateId = statusesProxyModel->databaseIDByRow(ui->comboBoxState->currentIndex());
    try
    {
        if(!checkStateAcl(stateId))
        {
            Global::errorPopupMsg(Global::ThrowType::AccessDenied);
            throw 0;
        }

        int err = 0;

        // TODO: Необходим механизм настраиваемых проверок при включении статуса.
        // Проблема частного случая: предопределённый статус Ожидание решения клиента
        // был заменён на пользовательский чтобы изменить порядок строк в выпадающем
        // списке ComboBox'а. Однако, при этом не будет работать проверка Обязательное
        // указание диагностики (Настройки -» Основные, группа Ремонты).
        switch(stateId)
        {
            case Global::RepStateIds::Returned:
            case Global::RepStateIds::ReturnedNoRepair:
            case Global::RepStateIds::ReturnedInCredit: createDialogIssue(); throw 0;
            case Global::RepStateIds::DiagFinished:
            case Global::RepStateIds::OnApprovement:
            case Global::RepStateIds::Negotiation: if( ui->textEditDiagResult->toPlainText().isEmpty() || ui->widgetBOQ->isEmpty() ) err = 1; break;
            case Global::RepStateIds::IssueNotAppeared: break;
            case Global::RepStateIds::Agreed: if(ui->doubleSpinBoxAmount->value() == 0 && !repairModel->isRepeat() && !repairModel->isWarranty()) err = 2; break;
        }

        switch (err)
        {
            case 1: Global::errorPopupMsg(tr("Поле с результатом диагностики не может быть пустым")); throw 0;
            case 2: Global::errorPopupMsg(tr("Не установлена согласованная сумма")); throw 0;
            default: ;
        }
    }
    catch(...)
    {
        updateStatesModel();
        return 1;
    }

    return 0;
}

int tabRepair::checkInput()
{
    switch(m_opType)
    {
        case SaveState: return checkBeforeStateChange();
        case SaveInformedState: return checkBeforeInformedStatusChange();
        case SaveDiagAmount: return (!m_diagChanged && !m_spinBoxAmountChanged);
        case SaveBOQ: return 0;
        case SavePlace: return checkBeforePlaceChange();
        case SaveBeforeClose: return 0;
        default: ;
    }

    return 0;
}

/* Проверка и выполнение необходимых подготовительных операций со статусом перед закрытием вкладки ремонта
 * Актуально только если отключеном автосохранение статуса ремонта
*/
void tabRepair::doStateActionsBeforeClose()
{
    if(userDbData->saveStateOnClose() || ui->comboBoxState->currentIndex() == -1)
        return;

    int stateId = statusesProxyModel->databaseIDByRow(ui->comboBoxState->currentIndex());

    switch(stateId)
    {
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair:
        case Global::RepStateIds::ReturnedInCredit: return; // создание диалога не предусмотрено
        default: ;
    }

    if(!checkStateAcl(stateId))
    {
        Global::errorPopupMsg(tr("Статус ремонта не сохранён, недостаточно прав"));
        return;
    }

    doStateActions(stateId);
    repairModel->set_stateId(stateId);
}

void tabRepair::beginCommit()
{
    if(m_opType == SaveState)
    {
        if(ui->comboBoxState->currentIndex() == -1) // SAbstractItemModel::databaseIDByRow() для -1 вернёт 0, соответствующий статусу Приём в ремонт
            return;

        int stateId = statusesProxyModel->databaseIDByRow(ui->comboBoxState->currentIndex());

        doStateActions(stateId);
        repairModel->set_stateId(stateId);
    }
    else if(m_opType == SaveInformedState)
    {
        int stateId = notifyStatusesModel->databaseIDByRow(ui->comboBoxNotifyStatus->currentIndex());
        repairModel->setInformedStatusIndex(stateId);
    }
    else if(m_opType == SaveBOQ)
    {
        return;
    }
    else if(m_opType == SavePlace)
    {
        repairModel->setBoxIndex(ui->comboBoxPlace->currentIndex());
        return;
    }
    else if(m_opType == SaveBeforeClose)
    {
        doStateActionsBeforeClose();
    }

    setModelDiagAmount();
}

int tabRepair::commitStages()
{
    switch(m_opType)
    {
        case SaveState:
        case SaveBeforeClose: return 2;   // сохранение списка работ и деталей, затем сохранение данных модели ремонта
        case SaveInformedState:
        case SaveDiagAmount:
        case SavePlace:
        case SaveBOQ: return 1;
        default: ;
    }

    return 0;
}

bool tabRepair::skip(const int stage)
{
    if(stage == 0)
    {
        switch (m_opType)
        {
            case SaveInformedState:
            case SavePlace:
            case SaveDiagAmount: return 0;
            case SaveBOQ:
            case SaveState:
            case SaveBeforeClose: return !ui->widgetBOQ->isDirty();
            default: break;
        }
    }

    return 0;
}

void tabRepair::commit(const int stage)
{
    if(stage == 0)
    {
        switch (m_opType)
        {
            case SaveInformedState:
            case SaveDiagAmount:
            case SavePlace: break;
            case SaveBOQ:
            case SaveState:
            case SaveBeforeClose: ui->widgetBOQ->submit(); return;
            default: break;
        }
    }

    repairModel->updateLastSave();
    repairModel->commit();
}

void tabRepair::throwHandler(int type)
{
    if(type == Global::ThrowType::ConnLost)
        return;

    repairModel->setFieldsFailed();
    switch (m_opType)
    {
        case SaveState: updateStatesModel(); break;
        case SaveInformedState: updateComboBoxInformedStatus(); break;
        case SavePlace: updateComboBoxPlace(); break;
        default: break;
    }
}

void tabRepair::endCommit()
{
    repairModel->SSingleRowModelBase::load(); // "тихая" перезагрузка данных модели

    // т. к. перезагрузка данных происходит без сигнала modelUpdated, то и корректировки, вносимые возможными
    // триггерами в базе, не будут отражаться в интерфейсе; по хорошему триггеров не должно быть вообще, но это
    // очень далёкое будущее; при необходимости нужно заменить метод загрузки данных или добавить эмит сигнала для
    // отдельных типов операций
    QString msg;
    switch (m_opType)
    {
        case SaveState: msg = tr("Статус изменён"); break;
        case SaveInformedState: msg = tr("Статус информирования клиента сохранён"); break;
        case SavePlace: msg = tr("Место хранения сохранено"); break;
        case SaveBeforeClose:
        case SaveDiagAmount: msg = tr("Данные сохранены"); break;
        case SaveBOQ: return;
        default: ;
    }
    switch (m_opType)
    {
        case SaveState: endStateChange(); break;
        case SaveInformedState:
        case SaveDiagAmount: diagAmountSaved(); break;
        default: ;
    }

    new shortlivedNotification(this, tr("Успешно"), msg, QColor(214,239,220), QColor(229,245,234));
    tabRepairs::refreshIfTabExists();
}
