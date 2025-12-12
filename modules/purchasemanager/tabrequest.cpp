#include "tabrequest.h"
#include "ui_tabrequest.h"
#include <SAppLog>
#include <Mainwindow>
#include <tabPurchaseManager>
#include <tabRepair>
#include <SUserSettings>
#include <SPermissions>
#include <SPartRequest>
#include <SClientModel>
#include <SCommentModel>
#include <SStandardItemModel>
#include <SSqlQueryModel>
#include <FlashPopup>

QMap<int, tabPartRequest*> tabPartRequest::p_instance;

tabPartRequest *tabPartRequest::getInstance(int id, MainWindow *parent)
{
    if( !p_instance.contains(id) )
      p_instance.insert(id, new tabPartRequest(id, parent));
    return p_instance.value(id);
}

tabPartRequest::tabPartRequest(int id, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabPartRequest),
    m_initialRequestId(id),
    m_requestId(id)
{
    ui->setupUi(this);
    logUserActivity();

    m_partRequest = new SPartRequest(this);
    m_clientModel = new SClientModel();
    m_dealerModel = new SClientModel();

    if(id)
        m_partRequest->setPrimaryKey(id);

    initWidgets();

    if(id)
        load(id);
    else
        updateWidgets();

#ifdef QT_DEBUG
    if(!id)
    {
        test_scheduler->start(300);
    }
#endif
}

tabPartRequest::~tabPartRequest()
{
    delete ui;
    p_instance.remove(m_requestId);   // Обязательно блять!
    if(m_clientModel)
        delete m_clientModel;
    if(m_dealerModel)
        delete m_dealerModel;
}

QString tabPartRequest::tabTitle()
{
    if(m_requestId)
        return tr("Заявка на закупку %1").arg(m_requestId);

    return tr("Новая заявка на закупку");
}

bool tabPartRequest::tabCloseRequest()
{
    try
    {
        if(m_partRequest->isDirty())    throw 1;
        if(ui->lineEditName->text().compare(m_partRequest->name()))    throw 2;
        if(ui->widgetSuppliers->isDirty())     throw 3;
        if(ui->textEditNotes->toPlainText().compare(m_partRequest->notes()))    throw 4;
        if(m_partRequest->id())
        {
            if(m_partRequest->dealer() != m_dealerModel->id())  throw 5;
            if(isManagersChanged()) throw 6;
            if(m_partRequest->planEndDate() != ui->dateEditPlanEndDate->date()) throw 8;
            if(m_partRequest->state() != partRequestStatesListModel->databaseIDByRow(ui->comboBoxState->currentIndex()))    throw 9;
            if(m_partRequest->priority() != partRequestPrioritiesListModel->databaseIDByRow(ui->comboBoxPriority->currentIndex()))  throw 10;
            if(ui->lineEditTrack->text().compare(m_partRequest->tracking()))    throw 11;
            if(m_partRequest->amount() != ui->doubleSpinBoxAmount->value()) throw 12;
            if(m_partRequest->count() != ui->spinBoxCount->value()) throw 13;
        }
    }
    catch (int step)
    {
        int result = QMessageBox::question(this,
                                           tr("Данные не сохранены"),
                                           tr("Сохранить?"),
                                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        switch(result)
        {
            case QMessageBox::Yes: save(); break;
            case QMessageBox::Cancel: return 0;
        }
    }

    return 1;
}

void tabPartRequest::copyRequest(const int id)
{
    SPartRequest *old = new SPartRequest;
    old->load(id);
    ui->comboBoxManagers->resetChecked();
    ui->comboBoxManagers->setChecked(old->managers());
    ui->lineEditName->setText(old->name());
    m_dealerModel->blockSignals(true);
    ui->widgetDealerInput->fillCreds(old->dealer());
    m_dealerModel->blockSignals(false);
    if(old->client())
        setClient(old->client());
    ui->spinBoxCount->setValue(old->count());
    ui->widgetSuppliers->setPredefSupplierId(old->dealer());
    ui->widgetSuppliers->copyLinks(id);

    delete old;
}

void tabPartRequest::setRepair(const int id)
{
    bool repairVisible = id > 0;
    ui->labelRepair->setVisible(repairVisible);
    ui->lineEditRepair->setVisible(repairVisible);
    ui->pushButtonRepair->setVisible(repairVisible);

    if(ui->lineEditRepair->text().toInt() == id)
        return;

    if(repairVisible)
        ui->lineEditRepair->setText(QString::number(id));
    ui->lineEditRepair->setReadOnly(true);
}

void tabPartRequest::setClient(const int id)
{
    ui->widgetClientInput->setVisible(id > 0);

    if(m_clientModel->id() == id)
        return;

    ui->widgetClientInput->fillCreds(id);
    ui->widgetClientInput->setHandlingButtons(SClientSelectForm::AccessMode::ViewCard);
}

void tabPartRequest::load(const int id)
{
    updateTabPtr(m_initialRequestId, id);
    m_requestId = id;
    m_partRequest->load(id);
    ui->comboBoxCreator->setCurrentIndex(usersModel->rowByDatabaseID(m_partRequest->employee()));
    ui->widgetDealerInput->fillCreds(m_partRequest->dealer());
    ui->widgetDealerInput->setHandlingButtons(m_dealerRO);
    ui->widgetSuppliers->select(id);
    ui->widgetComments->load(id);
    emit updateTabTitle(this);
}

void tabPartRequest::initWidgets()
{
    ui->comboBoxCreator->setModel(usersModel);
    ui->comboBoxCreator->setCurrentIndex(usersModel->rowByDatabaseID(userDbData->id()));
    ui->comboBoxCreator->setEnabled(false);
    ui->comboBoxManagers->setModel(partRequestManagersModel);
    ui->comboBoxManagers->showSelectAllRow(true);
    ui->comboBoxManagers->applyGuiSettings();
    ui->dateEditBeginDate->setAllowFutureDate(true);
    ui->dateEditBeginDate->setDateTrackingEnabled(true);
    ui->dateEditPlanEndDate->clear();
    ui->dateEditPlanEndDate->setAllowFutureDate(true);
    ui->dateEditEndDate->clear();
    ui->dateEditEndDate->setAllowBackDate(true);
    ui->dateEditEndDate->setVisible(m_endDateVisible);

    ui->comboBoxState->setModel(partRequestStatesListModel);
    ui->comboBoxState->hideRow(partRequestStatesListModel->rowByDatabaseID(SPartRequest::NewUncommitted));

    ui->comboBoxPriority->setModel(partRequestPrioritiesListModel);
    ui->comboBoxPriority->setCurrentIndex(partRequestPrioritiesListModel->rowByDatabaseID(SPartRequest::Priority::Regular));

//    ui->lineEditName->setButtons("Search"); // когда будет готова вкладка Товары
//    ui->lineEditRepair->setButtons("Search");
    ui->spinBoxCount->setMinimum(1);
    ui->spinBoxCount->setMaximum(99999);
    connect(ui->pushButtonRepair, &QPushButton::clicked, this, [=]{MainWindow::getInstance()->createTabRepair(m_partRequest->repair());});
    connect(ui->pushButtonCreate, &QPushButton::clicked, this, &tabPartRequest::create);
    connect(ui->pushButtonCreateMore, &QPushButton::clicked, this, &tabPartRequest::createAndRepeat);
    connect(ui->pushButtonCopy, &QPushButton::clicked, this, &tabPartRequest::createCopyTab);
    connect(ui->pushButtonSave, &QPushButton::clicked, this, &tabPartRequest::save);

    ui->widgetDealerInput->setClientModel(m_dealerModel);
    ui->widgetDealerInput->setLayoutRefWidget(0, ui->labelName);
    ui->widgetDealerInput->setLayoutRefWidget(1, ui->spinBoxCount);
    ui->widgetDealerInput->setLabelText(tr("Поставщик"));
    ui->widgetDealerInput->setClientsTabTitle(tr("Выбрать поставщика"));
    ui->widgetDealerInput->setBalanceVisible(false);
    ui->widgetDealerInput->setClientsTabDefaultCategory(SClientModel::Categories::Suppliers);

    ui->widgetClientInput->setClientModel(m_clientModel);
    ui->widgetClientInput->setLayoutRefWidget(0, ui->labelName);
    ui->widgetClientInput->setLayoutRefWidget(1, ui->spinBoxCount);
    ui->widgetClientInput->setClientsTabDefaultCategory(SClientModel::Categories::Regulars);
    ui->widgetClientInput->setBalanceVisible(false);

    ui->widgetSuppliers->setSaveButtonVisible(false);   // кнопка Сохранить не отображается никогда, т. к. есть кнопки на нижней панели

    ui->widgetComments->setParentTab(this);
    ui->widgetComments->setMode(SCommentModel::Mode::PartRequest);

    connect(m_dealerModel, &SClientModel::modelUpdated, this, [=]{ui->widgetSuppliers->addSupplierRecord(m_dealerModel->id());});
    connect(m_partRequest, &SPartRequest::modelUpdated, this, &tabPartRequest::updateWidgets);
}

void tabPartRequest::setDefaultStyleSheets()
{
    ui->lineEditName->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxCreator->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxManagers->setStyleSheet(commonComboBoxStyleSheet);
    ui->dateEditBeginDate->setStyleSheet(commonDateEditStyleSheet);
    ui->dateEditPlanEndDate->setStyleSheet(commonDateEditStyleSheet);
    ui->dateEditEndDate->setStyleSheet(commonDateEditStyleSheet);
    ui->comboBoxState->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPriority->setStyleSheet(commonComboBoxStyleSheet);
    ui->spinBoxCount->setStyleSheet(commonLineEditStyleSheet);
}

int tabPartRequest::checkInput()
{
    int error = 0;
    int step = 0;
    setDefaultStyleSheets();

    if ( ++step && ui->comboBoxCreator->currentIndex() < 0 )
    {
        ui->comboBoxCreator->setStyleSheet(commonComboBoxStyleSheetRed);
        error |= 1<<step;
    }
    if ( ++step && ui->comboBoxManagers->checked().count() <= 0 )
    {
        ui->comboBoxManagers->setStyleSheet(commonComboBoxStyleSheetRed);
        error |= 1<<step;
    }
    if( ++step && (m_partRequest->state() == SPartRequest::NewUncommitted && ui->dateEditBeginDate->date().daysTo(ui->dateEditPlanEndDate->date()) < 0) )
    {
        ui->dateEditPlanEndDate->setStyleSheet(commonDateEditStyleSheetRed);
        error |= 1<<step;
    }
    if ( ++step && (!m_newRequest && ui->comboBoxState->currentIndex() < 0) )
    {
        ui->comboBoxState->setStyleSheet(commonComboBoxStyleSheetRed);
        error |= 1<<step;
    }
    if ( ++step && ui->comboBoxPriority->currentIndex() < 0 )
    {
        ui->comboBoxPriority->setStyleSheet(commonComboBoxStyleSheetRed);
        error |= 1<<step;
    }
    if ( ++step && ui->lineEditName->text().isEmpty() )
    {
        ui->lineEditName->setStyleSheet(commonLineEditStyleSheetRed);
        error |= 1<<step;
    }
//    if ( ++step && ui->widgetClientInput->checkInput() )
//    {
//        error |= 1<<step;
//    }
    if ( ++step && ui->spinBoxCount->value() == 0 )
    {
        ui->spinBoxCount->setStyleSheet(commonLineEditStyleSheetRed);
        error |= 1<<step;
    }

    if (error)
    {
        qDebug() << QString("Ошибка создания заявки: возможно, не все обязательные поля заполнены (error 0b%1)").arg(error, 0, 2);
        appLog->appendRecord(QString("Error while creating part request: probably some obligatory fields empty (error 0b%1)").arg(error, 0, 2));
        return 1;
    }
    return 0;
}

/* Проверка изменений отмеченных менеджеров
 * Возвращает 1 если есть изменения
*/
bool tabPartRequest::isManagersChanged()
{
    if(m_partRequest->isManagersModelDirty())
        return true;

    foreach (auto mgr, m_partRequest->managers())
    {
        // Право использовать менеджер закупок может быть отозвано и некоторые заявки всегда будут считаться изменёнными
        if(partRequestManagersModel->rowByDatabaseID(mgr) == -1)
            continue;

        if(!ui->comboBoxManagers->checked().contains(mgr))
            return 1;
    }
    foreach (auto mgr, ui->comboBoxManagers->checked())
    {
        if(!m_partRequest->managers().contains(mgr))
            return 1;
    }

    return 0;
}

void tabPartRequest::setModelData()
{
    QDate plannedEnd = ui->dateEditPlanEndDate->date();
    int newState;

    m_partRequest->set_employee(usersModel->databaseIDByRow(ui->comboBoxCreator->currentIndex()));
    // Обновление списка ответственных сотрудников должно производиться только после записи данных в БД
    if(m_newRequest)
    {
        newState = SPartRequest::State::Created;
        if(QDate::currentDate().daysTo(ui->dateEditBeginDate->date()) > 0)
            m_partRequest->set_created(ui->dateEditBeginDate->dateTime());
        else
            m_partRequest->set_created(QDateTime::currentDateTime());
    }
    else
    {
        newState = partRequestStatesListModel->databaseIDByRow(ui->comboBoxState->currentIndex());
        m_partRequest->set_tracking(ui->lineEditTrack->text());
        m_partRequest->set_amount(ui->doubleSpinBoxAmount->value());
    }

    if(plannedEnd.isValid())
        m_partRequest->set_planEndDate(plannedEnd);
    m_partRequest->set_state(newState);
    m_partRequest->set_priority(partRequestPrioritiesListModel->databaseIDByRow(ui->comboBoxPriority->currentIndex()));

    m_partRequest->set_name(ui->lineEditName->text());
    m_partRequest->set_dealer(m_dealerModel->id());
    m_partRequest->set_client(m_clientModel->id());
    m_partRequest->set_repair(ui->lineEditRepair->text().toInt());
    m_partRequest->set_count(ui->spinBoxCount->value());
    m_partRequest->set_notes(ui->textEditNotes->toPlainText());
}

void tabPartRequest::beginCommit()
{
    setModelData();

    setDefaultStyleSheets();
}

void tabPartRequest::commit(const int stage)
{
    Q_UNUSED(stage)
    m_partRequest->commit();
    if(m_newRequest)
    {
        m_requestId = m_partRequest->id();
        ui->widgetSuppliers->setRequestId(m_requestId);
        m_partRequest->updateAdditionalModelsFilters(m_requestId);
    }

    if(isManagersChanged() || m_newRequest)
    {
        m_partRequest->setManagers(ui->comboBoxManagers->checked());
        m_partRequest->commitManagers();
    }
    ui->widgetSuppliers->commit();
}

void tabPartRequest::throwHandler(int)
{
    if(m_newRequest)
    {
        m_requestId = 0;
    }
    m_partRequest->setFieldsFailed();
}

void tabPartRequest::endCommit()
{
    QString message;
    if(m_newRequest)
        message = tr("Успешно сохранёна");
    else
        message = tr("Изменения успешно сохранёны");
    auto *p = new shortlivedNotification(this, tr("Заявка на закупку"), message, QColor(214,239,220), QColor(229,245,234));
    Q_UNUSED(p);

    m_partRequest->SSingleRowModelBase::load();

    int rep = m_partRequest->repair();
    if(rep)
        tabRepair::refreshIfTabExists(rep);
    tabPurchaseManager::refreshIfTabExists();

    switch(m_endCommitOp)
    {
        case SwitchToViewMode: switchTabToViewMode(); break;
        case PrepareRepeat: prepareForRepeatedOp(); break;
        case UpdateWidgets: m_partRequest->load(m_requestId); return;   // return т.к. updateWidgets() вызовется по сигналу modelReset()
        default: break;
    }

    updateWidgets();
}

void tabPartRequest::switchTabToViewMode()
{
    if(m_newRequest)
    {
        updateTabPtr(m_initialRequestId, m_requestId);
        emit updateTabTitle(this);
    }

    ui->widgetClientInput->setHandlingButtons(SClientSelectForm::ViewCard);
    m_partRequest->load(m_requestId);
    ui->widgetSuppliers->setRequestState(m_partRequest->state());
    ui->widgetSuppliers->select(m_requestId);
}

void tabPartRequest::prepareForRepeatedOp()
{
    m_requestId = m_initialRequestId;
    m_partRequest->deleteLater();
    m_partRequest = new SPartRequest(this);
    m_partRequest->set_repair(m_partRequest->repair());
    ui->widgetSuppliers->clearModel();
    ui->widgetSuppliers->addSupplierRecord(m_dealerModel->id());
}

void tabPartRequest::stateHandler(const int state)
{
    m_newRequest = 0;
    m_commentsVisible = 1;
    m_endDateVisible = 0;
    m_requestRO = permissions->enableSparepartRequestManager?0:1;
    m_dealerRO = m_requestRO?SClientSelectForm::ViewCard:SClientSelectForm::Full;
    m_clientRO = SClientSelectForm::ViewCard;
    m_deadlineVisible = true;

    switch (state)
    {
        case SPartRequest::State::NewUncommitted: m_newRequest = 1; m_requestRO = 0; m_dealerRO = SClientSelectForm::Full; m_clientRO = SClientSelectForm::Full; m_commentsVisible = 0; break;
        case SPartRequest::State::Created: break;
        case SPartRequest::State::Cancelled:
        case SPartRequest::State::Finished:
            m_requestRO = 1;
            m_dealerRO = SClientSelectForm::ViewCard;
            m_deadlineVisible &= m_partRequest->planEndDate().isValid();
            m_endDateVisible = 1;
            break;
        default: break;
    }
}

/* Замена ключа в списке singletone-вкладок
*/
// TODO: идентичный метод есть в классе tabCashOperation; возможно стоит его перенести в tabCommon
void tabPartRequest::updateTabPtr(const int oldId, const int newId)
{
    if(oldId == newId)
        return;

    p_instance.remove(oldId);
    p_instance.insert(newId, this);
}

void tabPartRequest::updateWidgets()
{
    setDefaultStyleSheets();
    stateHandler(m_partRequest->state());

    ui->comboBoxManagers->setChecked(m_partRequest->managers());
    ui->comboBoxManagers->setEnabled(!m_requestRO);

    ui->dateEditBeginDate->setAllowBackDate(!m_newRequest);
    if(!m_newRequest)
    {
        ui->dateEditBeginDate->setDateTime(m_partRequest->created());
        ui->comboBoxPriority->setCurrentIndex(partRequestPrioritiesListModel->rowByDatabaseID(m_partRequest->priority()));
    }
    ui->dateEditBeginDate->setReadOnly(!m_newRequest);

    ui->labelPlanEndDate->setVisible(m_deadlineVisible);
    ui->dateEditPlanEndDate->setVisible(m_deadlineVisible);
    ui->dateEditPlanEndDate->setAllowBackDate(!m_newRequest);
    if(m_deadlineVisible)
        ui->dateEditPlanEndDate->setDate(m_partRequest->planEndDate());
    ui->dateEditPlanEndDate->setReadOnly(m_requestRO);

    ui->labelEndDate->setVisible(m_endDateVisible);
    ui->dateEditEndDate->setVisible(m_endDateVisible);
    ui->dateEditEndDate->setDate(m_partRequest->endDate());
    ui->dateEditEndDate->setReadOnly(m_requestRO);

    ui->comboBoxState->setCurrentIndex(partRequestStatesListModel->rowByDatabaseID(m_partRequest->state()));
    ui->comboBoxState->setDisabled(m_newRequest || m_requestRO);

    ui->comboBoxPriority->setEnabled(!m_requestRO);

    ui->labelTrack->setVisible(m_commentsVisible);
    ui->lineEditTrack->setVisible(m_commentsVisible);
    ui->lineEditTrack->setText(m_partRequest->tracking());
    ui->lineEditTrack->setReadOnly(m_requestRO);

    ui->labelAmount->setVisible(m_commentsVisible);
    ui->doubleSpinBoxAmount->setVisible(m_commentsVisible);
    ui->doubleSpinBoxAmount->setValue(m_partRequest->amount());
    ui->doubleSpinBoxAmount->setReadOnly(m_requestRO);

    ui->lineEditName->setText(m_partRequest->name());
    ui->lineEditName->setReadOnly(m_requestRO);

    ui->widgetDealerInput->setHandlingButtons(m_dealerRO);

    setClient(m_partRequest->client());
    setRepair(m_partRequest->repair());

    ui->spinBoxCount->setValue(m_partRequest->count());
    ui->spinBoxCount->setReadOnly(m_requestRO);

    ui->textEditNotes->setText(m_partRequest->notes());
    ui->textEditNotes->setReadOnly(m_requestRO);

    // режим RO может быть автивен, если у пользователя нет права Использовать менеджер; открыть карточку заявки при этом он может из карточки ремонта
    ui->widgetSuppliers->setRequestState(m_requestRO?SPartRequest::Finished:m_partRequest->state());

    ui->widgetComments->setVisible(m_commentsVisible);

    ui->pushButtonCreate->setVisible(!m_requestRO && m_newRequest);
    ui->pushButtonCreateMore->setVisible(!m_requestRO && m_newRequest);
    ui->pushButtonCopy->setVisible(!m_newRequest);
    ui->pushButtonSave->setVisible(!m_requestRO && !m_newRequest);
    ui->pushButtonLog->setVisible(!m_newRequest);
}

void tabPartRequest::create()
{
    m_endCommitOp = EndCommitOp::SwitchToViewMode;
    manualSubmit();
}

void tabPartRequest::createAndRepeat()
{
    m_endCommitOp = EndCommitOp::PrepareRepeat;
    manualSubmit();
}

void tabPartRequest::createCopyTab()
{
    MainWindow *mw = MainWindow::getInstance();
    tabPartRequest *newTab = nullptr;

    newTab = static_cast<tabPartRequest*>(mw->createTabPartRequest(0));
    newTab->copyRequest(m_requestId);
}

void tabPartRequest::save()
{
    m_endCommitOp = EndCommitOp::UpdateWidgets;
    manualSubmit();
}

void tabPartRequest::guiFontChanged()
{
    QFont font(guiFont());

    ui->comboBoxManagers->setFont(font);
    ui->comboBoxState->setFont(font);
}

#ifdef QT_DEBUG
void tabPartRequest::randomFill()
{
    int i;

    i = QRandomGenerator::global()->bounded(partRequestPrioritiesListModel->rowCount());
    ui->comboBoxPriority->setCurrentIndex(i);

    if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50
        ui->widgetDealerInput->fillCreds(SClientInputFormBase::randomClientIdFromDB("is_dealer", 1));
    if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50
    {
        ui->widgetClientInput->fillCreds(SClientInputFormBase::randomClientIdFromDB("is_dealer", 0));
        ui->widgetClientInput->setVisible(true);
    }

    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    for(int j = 0; j < 3; j++)
    {
        query->exec(QString("SELECT `name` FROM (SELECT ROUND(@i * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id`, t1.`name` FROM store_items AS t1 CROSS JOIN (SELECT @i := 0) AS dummy WHERE t1.`count` - t1.`reserved` > 0 AND t1.`is_realization` = 1) AS t1 ON t1.`num` = `rand`.`rand`;"));
        if(j<2)
            continue;   // после обновления сервера на mysql 5.6.51 (win) пришлось чуть-чуть изменить запрос для случайного товара; также в только что открытой сессии результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их

        query->first();
        if(query->isValid())
            ui->lineEditName->setText(query->value(0).toString());
    }

    i = QRandomGenerator::global()->bounded(10);
    if(i)
        i *= 5;
    else
        i += 1;
    ui->spinBoxCount->setValue(i);
}

void tabPartRequest::test_scheduler_handler()
{
//    randomFill();
//    ui->widgetDealerInput->setLineEditText("tag");
//    ui->widgetClientInput->setLineEditText("чер");
//    copyRequest(217);
//    load(217);
}
#endif
