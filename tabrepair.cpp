#include "global.h"
#include "appver.h"
#include "tabrepair.h"
#include "ui_tabrepair.h"
#include "tabprintdialog.h"
#include "tabrepairs.h"

QMap<int, tabRepair*> tabRepair::p_instance;

tabRepair::tabRepair(int rep_id, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepair),
    repair_id(rep_id)
{
    ui->setupUi(this);
    tabRepair::guiFontChanged();

    repairModel = new SRepairModel();
    repairModel->setId(repair_id);
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
    if(userDbData->autosaveDiagResult)
    {
        m_autosaveDiag = 1;
        m_autosaveDiagTimer = new QTimer();
        m_autosaveDiagTimer->setSingleShot(true);
        QObject::connect(m_autosaveDiagTimer, SIGNAL(timeout()), this, SLOT(autosaveDiagAmount()));
    }
    if(userDbData->saveStateOnClose)
    {
        ui->toolButtonSaveState->setHidden(true);
        ui->comboBoxState->disableWheelEvent(true);  // если включено автосохранение статуса ремонта, то нужно игнорировать колёсико мышки
        connect(ui->comboBoxState, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxStateIndexChanged(int)));
    }

    ui->comboBoxState->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxNotifyStatus->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPlace->setStyleSheet(commonComboBoxStyleSheet);
    connect(ui->toolButtonSaveState, SIGNAL(clicked()), this, SLOT(saveState()));

    additionalFieldsModel = new SFieldsModel(SFieldsModel::Repair);
    statusesProxyModel = new SSortFilterProxyModel;
    statusesProxyModel->setSourceModel(comSettings->repairStatuses.Model);
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

    ui->widgetBOQ->linkWithRepairModel(repairModel);
    ui->widgetBOQ->setReadOnly(true);

    ui->widgetComments->setParentTab(this);
    ui->widgetComments->setMode(SCommentModel::Repair);

    // сворачивание групп элементов (ну как в АСЦ чтобы). TODO: Отключено, т. к. требует доработки класса SGroupBoxEventFilter
//    groupBoxEventFilter = new SGroupBoxEventFilter(this);
//    ui->groupBoxDeviceSummary->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxDiagResult->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxWorksAndSpareParts->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxComments->installEventFilter(groupBoxEventFilter);

    this->setAttribute(Qt::WA_DeleteOnClose);

    if(!checkViewPermission())
    {
        logUserActivity();
        reloadRepairData();
        setLock();
        m_repairLockUpdateTimer = new QTimer(this);
        connect(m_repairLockUpdateTimer, &QTimer::timeout, [=]{setLock();});
        m_repairLockUpdateTimer->setSingleShot(false);
        m_repairLockUpdateTimer->start(10000);
    }
    else
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Не достаточно прав для открытия карточки чужого ремонта"), QColor(255,255,255), QColor(245,245,245));
        this->deleteLater();
        return;
    }

#ifdef QT_DEBUG
    if( m_getOutButtonVisible && (repairModel->state() == Global::RepStateIds::Ready || repairModel->state() == Global::RepStateIds::ReadyNoRepair) )
        createDialogIssue();
    connect(ui->pushButtonManualUpdateRepairData, SIGNAL(clicked()), this, SLOT(reloadRepairData()));
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
    if((!m_autosaveDiag && (m_diagChanged || m_spinBoxAmountChanged)) || (ui->widgetBOQ->isCommitted()))
    {
        auto result = QMessageBox::question(this, tr("Данные не сохранены"), tr("Результат диагностики, согласованная сумма или список работ и деталей не сохранены!\nСохранить перед закрытием?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (result == QMessageBox::Cancel)
        {
            return 0;
        }
        else if (result == QMessageBox::Yes)
        {
            saveDiagAmount();
            ui->widgetBOQ->commit();
        }
    }
    m_repairLockUpdateTimer->stop();
    setLock(0);
    return 1;
}

void tabRepair::reloadRepairData()
{
    repairModel->load(repair_id);
    if(repairModel->clientId() != m_clientId)  // перезагрузка данных клиента только при первом вызове метода или если был изменён клиент
    {
        m_clientId = repairModel->clientId();
        clientModel->load(m_clientId);
    }
    additionalFieldsModel->load(repair_id);
    ui->widgetBOQ->load(repair_id);
    ui->widgetComments->load(repair_id);

    statusesProxyModel->setFilterRegularExpression("");
    updateStatesModel(repairModel->state());
    setWidgetsParams(repairModel->state());
    updateWidgets();
}

void tabRepair::updateWidgets()
{
    ui->lineEditRepairId->setText(QString::number(repair_id));
    ui->lineEditDevice->setText(repairModel->title());
    ui->lineEditSN->setText(repairModel->serialNumber());
    ui->lineEditClient->setText(permissions->viewClients?clientModel->fullLongName():tr("no permissions"));
    ui->labelPrimaryPhone->setVisible(permissions->viewClients);
    ui->lineEditPrimaryPhone->setVisible(permissions->viewClients);
    ui->lineEditPrimaryPhone->setText(clientModel->phones()->primaryStr());
    ui->lineEditInDate->setText(repairModel->created());
    setInfoWidgetVisible(ui->lineEditOutDate, m_outDateVisible);
    ui->lineEditOutDate->setText(repairModel->outDateTime());
    ui->pushButtonGetout->setVisible(m_getOutButtonVisible && !modelRO);
    ui->buttonClientCard->setVisible(permissions->viewClients);
    ui->buttonCall->setVisible(permissions->useTelephony);
    ui->buttonAdm->setVisible(permissions->advEditRepair);
    setInfoWidgetVisible(ui->lineEditExtPrevRepair, !repairModel->extEarly().isEmpty());
    ui->lineEditExtPrevRepair->setText(repairModel->extEarly());
    ui->lineEditOffice->setText(officesModel->getDisplayRole(repairModel->office()));
    ui->lineEditManager->setText(allUsersModel->value(repairModel->currentManager(), "id", "username").toString());
    ui->lineEditEngineer->setText(allUsersModel->value(repairModel->engineer(), "id", "username").toString());
    setInfoWidgetVisible(ui->lineEditPreagreedAmount, repairModel->isPreAgreed());
    ui->lineEditPreagreedAmount->setText(sysLocale.toCurrencyString(repairModel->preAgreedAmount()));        // TODO: заменить системное обозначение валюты на валюту заданную в таблице БД config

    ui->comboBoxPlace->setCurrentIndex(repairModel->boxIndex());
    ui->comboBoxPlace->setEnabled(!modelRO);
    ui->lineEditColor->setStyleSheet(QString("background-color: %1;").arg(repairModel->color()));
    ui->lineEditWarrantyLabel->setText(repairModel->warrantyLabel());

    if(repairModel->early())
        ui->lineEditPrevRepair->setText(QString::number(repairModel->early()));

    fillExtraInfo();
    if( repairModel->paymentSystem() == 1 && permissions->handleCashlessOrders )    // указана Безналичная оплата и есть разрешение Работать с безналичными счетами
    { // TODO: нужен более гибкий способ определения безналичного рассчета

        ui->groupBoxCashless->setHidden(false);
        if(repairModel->invoice()) // если уже выставлен счет
        {
            ui->lineEditInvoiceAmount->setText("TODO:");
            ui->lineEditInvoice->setText(QString("id=%1; TODO:").arg(repairModel->invoice()));
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

    ui->comboBoxNotifyStatus->blockSignals(true);
    ui->comboBoxNotifyStatus->setCurrentIndex(repairModel->informedStatusIndex());
    ui->comboBoxNotifyStatus->blockSignals(false);
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
    ui->doubleSpinBoxAmount->setDecimals(comSettings->classicKassa?2:0);
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
    ui->toolButtonSaveState->setEnabled(!modelRO);

    ui->widgetBOQ->updateWidgets();
    ui->toolButtonSaveState->setEnabled(m_buttonSaveStateEnabled);
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
    if(repairModel->expressRepair())
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

    if(!modelRO)
        repairModel->lock(state);
}

void tabRepair::createAdditionalFieldsWidgets()
{
    delAdditionalFieldsWidgets();
    int i;
    SFieldValueModel *field;

    i = 0;
    foreach(field, additionalFieldsModel->list())
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
    label = ui->formLayout->labelForField(field);
    field->setVisible(state);
    label->setVisible(state);
}

bool tabRepair::setWidgetsParams(const int stateId)
{
    m_getOutButtonVisible = 0;
    m_comboBoxStateEnabled = 1;
    m_BOQModelRO = 1;
    m_diagRO = 1;
    m_summRO = 0;   // При наличии пользовательского статуса "Ожидание решения" логика данной опции не работает

    if( stateId == Global::RepStateIds::Ready || stateId == Global::RepStateIds::ReadyNoRepair )
    {
        for(const int nextState : (const QList<int>)comSettings->repairStatuses[stateId].Contains)
        {
            switch (nextState)
            {
                case Global::RepStateIds::Returned:
                case Global::RepStateIds::ReturnedNoRepair:
                case Global::RepStateIds::ReturnedInCredit: m_getOutButtonVisible = permissions->issueDevices && checkStateAcl(nextState);
            }
        }
    }

    switch (stateId)
    {
        case Global::RepStateIds::Negotiation: m_summRO = 0; break;
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair: m_comboBoxStateEnabled = 0; m_comboBoxNotifyStatusEnabled = 0; Q_FALLTHROUGH();
        case Global::RepStateIds::ReturnedInCredit: m_buttonDebtReceivedVisible = 1; m_outDateVisible = 1; break;
    }

    for(const int action : (const QList<int>)comSettings->repairStatuses[stateId].Actions)
    {
        switch (action)
        {
            case Global::RepStateActions::EditWorksParts: m_BOQModelRO = 0; break;
            case Global::RepStateActions::EditDiagSumm: m_diagRO = 0; m_summRO = 0; break;
        }
    }

    ui->widgetBOQ->setReadOnly(m_BOQModelRO);

    return 1;
}

/*  Проверка разрешений
 */
bool tabRepair::checkStateAcl(const int stateId)
{
    if(repairModel->state() == Global::RepStateIds::GetIn && !repairModel->engineer() && !permissions->beginUnengagedRepair)
        return 0;

    const QString allowedForRoles = comSettings->repairStatuses[stateId].Roles.join('|');

    if(userDbData->roles.contains(QRegularExpression(QString("\\b(%1)\\b").arg(allowedForRoles))))
    {
        return 1;
    }
    return 0;
}

/* Проверка данных перед сменой статуса или выдачей
 * Возвращает 1, если всё ОК
*/
bool tabRepair::checkData(const int stateId)
{
    int ret = 0;

    switch(stateId)
    {
        case Global::RepStateIds::DiagFinished:
        case Global::RepStateIds::OnApprovement:
        case Global::RepStateIds::Negotiation: if( ui->textEditDiagResult->toPlainText().isEmpty() || ui->widgetBOQ->isEmpty() ) ret = 1; break;
        case Global::RepStateIds::IssueNotAppeared: break;
        case Global::RepStateIds::Agreed: if(ui->doubleSpinBoxAmount->value() == 0 && !repairModel->isRepeat() && !repairModel->isWarranty()) ret = 2; break;
    }

    if(ret)
    {
        QString msg;
        switch (ret)
        {
            case 1: msg = tr("Поле с результатом диагностики не может быть пустым"); break;
            case 2: msg = tr("Не установлена согласованная сумма"); break;
        }
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), msg, QColor(255,164,119), QColor(255,199,173));
        throw 0;
    }

    return !ret;
}

void tabRepair::createDialogIssue()
{
    QList<SRepairModel*> list;

    list.append(repairModel);
    m_dialogIssue = new SDialogIssueRepair(list, Qt::SplashScreen, this);
    connect(m_dialogIssue, &SDialogIssueRepair::printWorksLists, [=](){tabPrintDialog::printRepairWorksReports(list, false);});
    connect(m_dialogIssue, &SDialogIssueRepair::issueSuccessfull, this, &tabRepair::reloadRepairData);
    connect(m_dialogIssue, &SDialogIssueRepair::issueFailed, this, &tabRepair::reloadRepairData);
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
    repairModel->setEngineerIndex(index);
}

void tabRepair::initEngineer()
{
    if(repairModel->engineer())
        return;

    repairModel->setEngineer(userDbData->id);
}

/*  Проверка разрешения на открытие чужих карт ремонта
 *  Возвращает 1 если пользователь не обладает таким правом
*/
bool tabRepair::checkViewPermission()
{
    if(permissions->viewAnyRepair)
    {
        return 0;
    }

    bool ret = 1;
    int role;
    int repManager = 0;
    int repEngineer = 0;
    QStringList userRoles = userDbData->roles.split(',');
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));

    query->exec(QUERY_SEL_REPAIR_MNGR_ENGR(repair_id));
    if(query->first())
    {
        repManager = query->value(0).toInt();
        repEngineer = query->value(1).toInt();

        foreach(auto r, userRoles)
        {
            role = r.toInt();
            switch (role)
            {
                case Global::UserRoles::Engineer:
                case Global::UserRoles::SeniorEngineer: ret &= (repEngineer != 0) && (repEngineer != userDbData->id); break;    // инженер может открывать свободные ремонты и свои
                case Global::UserRoles::Manager:
                case Global::UserRoles::SeniorManager: ret &= (repManager != 0) && (repManager != userDbData->id); break;    // менеджер может открывать только свои ремонты
                case Global::UserRoles::Director: ret = 0; break;
            }

            if(!ret)
                break;
        }
    }

    delete query;
    return ret;
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

void tabRepair::updateStatesModel(const int stateId)
{
    QString allowedStates = comSettings->repairStatuses[stateId].ContainsStr.join('|');
    ui->comboBoxState->blockSignals(true);
    statusesProxyModel->setFilterRegularExpression(QString("\\b(%1)\\b").arg(allowedStates));
    ui->comboBoxState->setCurrentIndex(-1);
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    ui->comboBoxState->setPlaceholderText(comSettings->repairStatuses[stateId].Name);
    ui->comboBoxState->blockSignals(false);
}

void tabRepair::doStateActions(const int stateId)
{
    QList<int> stateActions = comSettings->repairStatuses[stateId].Actions;
    switch (stateId)
    {
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair:
        case Global::RepStateIds::ReturnedInCredit: createDialogIssue(); throw 0;
    }

    // В АСЦ установка инженера происходит при переключении с "Приём в ремонт" на любой другой.
    // Здесь реализация более гибкая — через действие "Назначить инициатора инженером" в настройках статусов; это позволит
    // создать пользовательские статусы, при включении которых не нужно задавать инженера ремонта.
    // Например, в мастерской разборкой занимается помощник мастеров.
    // Для статуса "Проведение диагностики" действие включено принудительно и от настроек пользователя не зависит
    if(stateId == Global::RepStateIds::Diag)
    {
        stateActions << Global::RepStateActions::SetEngineer;
    }

    for(const int &action : qAsConst(stateActions))
        switch (action)
        {
            case Global::RepStateActions::NoPayDiag: setPricesToZero(); break;
            case Global::RepStateActions::ResetInformedStatus: if(ui->comboBoxNotifyStatus->currentIndex()) setInformedStatus(0); break;
            case Global::RepStateActions::SetEngineer: initEngineer(); break;
            case Global::RepStateActions::InformManager: /*TODO: notifications->create(caption, text);*/; break;
            case Global::RepStateActions::InformEngineer: /*TODO: notifications->create(caption, text);*/; break;
        }
}

void tabRepair::setPricesToZero()
{
//    tableWorksParts->setPricesToZero();
    repairModel->setRepairCost(0);
}

bool tabRepair::commit(const QString &notificationCaption, const QString &notificationText)
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    QUERY_LOG_START(metaObject()->className());
    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        repairModel->updateLastSave();
        nErr = repairModel->commit();
        shortlivedNotification *newPopup = new shortlivedNotification(this, notificationCaption, notificationText, QColor(214,239,220), QColor(229,245,234));
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }
    QUERY_LOG_STOP;

    delete query;

    return nErr;
}

void tabRepair::saveState()
{
    saveState(ui->comboBoxState->currentIndex());
}

void tabRepair::saveState(int index)
{
    if (index < 0)
        return;

    m_groupUpdate = 1;

    if(!m_autosaveDiag)
        saveDiagAmount();

    ui->widgetBOQ->commit();

    int newStateId = statusesProxyModel->databaseIDByRow(index);

    try
    {
        if(!checkStateAcl(newStateId))
        {
            shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Проверьте права доступа или обратитесь к администратору"), QColor(212,237,242), QColor(229,244,247));
            throw 0;
        }
        checkData(newStateId);
        doStateActions(newStateId);
        repairModel->setState(newStateId);
    }
    catch (int type)
    {
        m_groupUpdate = 0;
        ui->comboBoxState->blockSignals(true);
        ui->comboBoxState->setCurrentIndex(-1);
        ui->comboBoxState->blockSignals(false);
        return;
    }
    updateStatesModel(newStateId);
    setWidgetsParams(newStateId);

    if(!commit())
        return;

    if(!m_autosaveDiag)
        diagAmountSaved();
    m_groupUpdate = 0;
    m_buttonSaveStateEnabled = 0;

    updateWidgets();
    tabRepairs::refreshIfTabExists();
}

void tabRepair::setInformedStatus(int status)
{
    if(status != Global::ClientInformStateIds::NotSet && !permissions->setNotificationState)
    {
        ui->comboBoxNotifyStatus->blockSignals(true);
        ui->comboBoxNotifyStatus->setCurrentIndex(Global::ClientInformStateIds::NotSet);
        ui->comboBoxNotifyStatus->blockSignals(false);
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Проверьте права доступа или обратитесь к администратору"), QColor(212,237,242), QColor(229,244,247));
        return;
    }

    repairModel->setInformedStatusIndex(status);

    if(m_groupUpdate)
        return;

    if(!m_autosaveDiag)
    {
        m_groupUpdate = 1;
        saveDiagAmount();
    }

    commit(tr("Успешно"), tr("Статус информирования клиента обновлён"));
    if(!m_autosaveDiag)
    {
        m_groupUpdate = 0;
        diagAmountSaved();
    }
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

void tabRepair::saveDiagAmount()
{
    if(!m_diagChanged && !m_spinBoxAmountChanged)
        return;

    if(m_spinBoxAmountChanged)
    {
        repairModel->setRepairCost(ui->doubleSpinBoxAmount->value());
    }
    if(m_diagChanged)
    {
        repairModel->setDiagnosticResult(ui->textEditDiagResult->toPlainText());
    }

    if(m_groupUpdate)
        return;

    if(!commit())
        return;

    diagAmountSaved();
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

void tabRepair::savePlace(int index)
{
    int currentPlace = repairModel->boxIndex();

    if(currentPlace == index)
        return;

    repairModel->setBoxIndex(index);
    if(!commit())
    {
        ui->comboBoxPlace->blockSignals(true);
        ui->comboBoxPlace->setCurrentIndex(currentPlace);
        ui->comboBoxPlace->blockSignals(false);
    }
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
    font.setPixelSize(userDbData->fontSize);

    ui->comboBoxState->setFont(font);
    ui->comboBoxNotifyStatus->setFont(font);
    ui->comboBoxPlace->setFont(font);
}

void tabRepair::comboBoxStateIndexChanged(int index)
{
    if(userDbData->saveStateOnClose)
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

