#include "global.h"
#include "appver.h"
#include "tabrepair.h"
#include "ui_tabrepair.h"

QMap<int, tabRepair*> tabRepair::p_instance;

tabRepair::tabRepair(int rep_id, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepair),
    repair_id(rep_id)
{
    query = new QSqlQuery(QSqlDatabase::database("connThird"));

    ui->setupUi(this);
    i_tabTitle = tr("Ремонт", "repair tab title") + " " + QString::number(repair_id);
    userActivityLog->appendRecord(tr("Navigation %1").arg(i_tabTitle));
    repairModel = new SRepairModel();
    repairModel->setId(repair_id);
    setLock(1);
    clientModel = new SClientModel();
    if(permissions->contains("3"))  // Печатать ценники и стикеры
    {
        ui->lineEditRepairId->setButtons("Print");
        connect(ui->lineEditRepairId, &SLineEdit::buttonClicked, this, &tabRepair::printStickers);
    }
    if(permissions->contains("72"))  // Изменять офис ремонта
    {
        ui->lineEditOffice->setButtons("Edit");
        connect(ui->lineEditOffice, &SLineEdit::buttonClicked, this, &tabRepair::changeOffice);
    }
    if(permissions->contains("76"))  // Назначать ответственного менеджера
    {
        ui->lineEditManager->setButtons("Edit");
        connect(ui->lineEditManager, &SLineEdit::buttonClicked, this, &tabRepair::changeManager);
    }
    if(permissions->contains("60"))  // Назначать ответственного инженера
    {
        ui->lineEditEngineer->setButtons("Edit");
        connect(ui->lineEditEngineer, &SLineEdit::buttonClicked, this, &tabRepair::changeEngineer);
    }
    if(permissions->contains("65"))  // Работать с безналичными счетами
    {
        ui->lineEditInvoice->setButtons("Open");
        connect(ui->lineEditInvoice, &SLineEdit::buttonClicked, this, &tabRepair::openInvoice);
    }
//    if(permissions->contains("X"))  // TODO: разрешение Редактировать комплектность
    {
        ui->lineEditIncomingSet->setButtons("Edit");
        connect(ui->lineEditIncomingSet, &SLineEdit::buttonClicked, this, &tabRepair::editIncomingSet);
    }
    if(permissions->contains("69"))  // Устанавливать детали со склада
    {
        ui->lineEditQuickAddSparePartByUID->setButtons("Apply");
        connect(ui->lineEditQuickAddSparePartByUID, &SLineEdit::buttonClicked, this, &tabRepair::quickAddSparePartByUID);
    }
    if(permissions->contains("29"))  // Статус: Согласовано с клиентом; TODO: подумать над этим разрешением; посмотреть как в АСЦ; достаточно ли использовать его или нужно создать своё
    {
        ui->lineEditAgreedAmount->setButtons("Apply");
        connect(ui->lineEditAgreedAmount, &SLineEdit::buttonClicked, this, &tabRepair::setAgreedAmount);
    }
    save_state_on_close = userDbDataModel->record(0).value("save_state_on_close").toBool();
    if(save_state_on_close)
    {
        ui->toolButtonSaveStatus->setHidden(true);
        ui->comboBoxStatus->disableWheelEvent(true);  // если включено автосохранение статуса ремонта, то нужно игнорировать колёсико мышки
        connect(ui->comboBoxStatus, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIndexChanged(int)));
    }

    ui->comboBoxStatus->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxNotifyStatus->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPlace->setStyleSheet(commonComboBoxStyleSheet);
    connect(ui->toolButtonSaveStatus, SIGNAL(clicked()), this, SLOT(saveStatus()));

    additionalFieldsModel = new SFieldsModel(SFieldsModel::Repair);
    statusesProxyModel = new SSortFilterProxyModel;
    statusesProxyModel->setSourceModel(statusesModel);
    worksAndPartsModel = new worksAndSparePartsDataModel;
    connect(worksAndPartsModel, SIGNAL(modelReset()), this, SLOT(updateTotalSumms()));  // TODO: уточнить генерируется ли сигнал при изменении существующих данных
    commentsModel = new commentsDataModel();
//    reloadRepairData();

    ui->comboBoxPlace->setModel(repairBoxesModel);
    ui->comboBoxStatus->blockSignals(true);
    ui->comboBoxStatus->setModel(statusesProxyModel);
    ui->comboBoxNotifyStatus->setModel(notifyStatusesModel);
    statusesProxyModel->setFilterKeyColumn(1);
    statusesProxyModel->setFilterRegularExpression("");
    ui->comboBoxStatus->blockSignals(false);

    ui->tableViewComments->setModel(commentsModel);
    ui->tableViewComments->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewComments->verticalHeader()->hide();
    ui->tableViewComments->horizontalHeader()->hide();

    ui->tableViewWorksAndSpareParts->setModel(worksAndPartsModel);
    ui->tableViewWorksAndSpareParts->verticalHeader()->hide();

    // сворачивание групп элементов (ну как в АСЦ чтобы). TODO: Отключено, т. к. требует доработки класса SGroupBoxEventFilter
//    groupBoxEventFilter = new SGroupBoxEventFilter(this);
//    ui->groupBoxDeviceSummary->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxDiagResult->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxWorksAndSpareParts->installEventFilter(groupBoxEventFilter);
//    ui->groupBoxComments->installEventFilter(groupBoxEventFilter);

    this->setAttribute(Qt::WA_DeleteOnClose);

    reloadRepairData();

#ifdef QT_DEBUG
//    createGetOutDialog();
    connect(ui->pushButtonManualUpdateRepairData, SIGNAL(clicked()), this, SLOT(updateWidgets()));
#else
    ui->pushButtonManualUpdateRepairData->setHidden(true);
#endif
}

tabRepair::~tabRepair()
{
    userActivityLog->updateActivityTimestamp();

    setLock(0);
    delAdditionalFieldsWidgets();
    delete additionalFieldsModel;
    delete ui;
    delete repairModel;
    delete clientModel;
    delete commentsModel;
    delete worksAndPartsModel;
    delete query;
    p_instance.remove(repair_id);   // Обязательно блять!
}

QString tabRepair::tabTitle()
{
    return i_tabTitle;
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
    worksAndPartsModel->setQuery(QUERY_SEL_REPAIR_WORKS_AND_PARTS(repair_id));
    commentsModel->setQuery(QUERY_SEL_REPAIR_COMMENTS(repair_id));

    updateStatesModel(repairModel->stateIndex());
    updateWidgets();
}

void tabRepair::updateWidgets()
{
    ui->lineEditRepairId->setText(QString::number(repair_id));
    ui->lineEditDevice->setText(repairModel->title());
    ui->lineEditSN->setText(repairModel->serialNumber());
    ui->lineEditClient->setText(clientModel->fullLongName());
    ui->lineEditInDate->setText(repairModel->created());
    setInfoWidgetVisible(ui->lineEditOutDate, repairModel->state() == 8 || repairModel->state() == 12); // TODO: нужен более гибкий способ скрытия поля с датой выдачи ремонта, если статус не "Выдано клиенту" или "Готово к выдаче без ремонта" (id!=8, id!=12)
    ui->lineEditOutDate->setText(repairModel->outDate());
    ui->pushButtonAdmEditWorks->setVisible(!m_worksRO && permissions->contains("TODO: разреш. на адм. правку списка раб. и дет."));
    ui->pushButtonGetout->setVisible(m_getOutButtonVisible);
    setInfoWidgetVisible(ui->lineEditExtPrevRepair, !repairModel->extEarly().isEmpty());
    ui->lineEditExtPrevRepair->setText(repairModel->extEarly());
    ui->lineEditOffice->setText(officesModel->getDisplayRole(repairModel->office()));
    ui->lineEditManager->setText(allUsersModel->value(repairModel->manager(), "id", "username").toString());
    ui->lineEditEngineer->setText(allUsersModel->value(repairModel->engineer(), "id", "username").toString());
    setInfoWidgetVisible(ui->lineEditPreagreedAmount, repairModel->isPreAgreed());
    ui->lineEditPreagreedAmount->setText(sysLocale.toCurrencyString(repairModel->preAgreedAmount()));        // TODO: заменить системное обозначение валюты на валюту заданную в таблице БД config

    ui->comboBoxPlace->setCurrentIndex(repairModel->boxIndex());
    box_name = ui->comboBoxPlace->currentText();
    ui->lineEditColor->setStyleSheet(QString("background-color: %1;").arg(repairModel->color()));
    ui->lineEditWarrantyLabel->setText(repairModel->warrantyLabel());
    if(repairModel->early())
        ui->lineEditPrevRepair->setText(QString::number(repairModel->early()));

    fillExtraInfo();
    if( repairModel->paymentSystem() == 1 && permissions->contains("65") )    // указана Безналичная оплата и есть разрешение Работать с безналичными счетами
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

    ui->comboBoxNotifyStatus->setCurrentIndex(repairModel->informedStatusIndex());
    ui->lineEditProblem->setText(repairModel->fault());
    ui->lineEditIncomingSet->setText(repairModel->complect());
    ui->lineEditExterior->setText(repairModel->look());

    for(int i = ui->gridLayoutDeviceSummary->rowCount() - 1; i > 2; i-- )
    {
        ui->gridLayoutDeviceSummary->itemAtPosition(i, 1)->widget()->deleteLater();
        ui->gridLayoutDeviceSummary->itemAtPosition(i, 0)->widget()->deleteLater();
    }
    createAdditionalFieldsWidgets();
    ui->textEditDiagResult->setText(repairModel->diagnosticResult());
    ui->lineEditAgreedAmount->setText(sysLocale.toString(repairModel->repairCost(), 'f', 2));

    ui->tableViewWorksAndSpareParts->resizeRowsToContents();
    ui->tableViewComments->resizeRowsToContents();
    ui->toolButtonSaveStatus->setEnabled(m_buttonSaveStateEnabled);
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
        ui->listWidgetExtraInfo->addItem(QString(tr("предоплата: %1")).arg(sysLocale.toCurrencyString(repairModel->prepaidSumm())));
//    if(repairModel->isDebt())  // похоже не используется в АЦС
//        ui->listWidgetExtraInfo->addItem("");
    if(ui->listWidgetExtraInfo->count())
        ui->listWidgetExtraInfo->setHidden(false);
}

void tabRepair::setLock(bool state)
{
    if(repairModel->isLock())
    {
        modelRO = 1;
        i_tabIcon = new QIcon(":/icons/light/1F512_32.png");
    }
    else
    {
        modelRO = 0;
        if(i_tabIcon)
            delete i_tabIcon;
        i_tabIcon = nullptr;
        repairModel->lock(state);
    }
}

void tabRepair::createAdditionalFieldsWidgets()
{
    delAdditionalFieldsWidgets();
    int i;
    SFieldValueModel *field;

    i = 0;
    foreach(field, additionalFieldsModel->list())
    {
        QLabel *label = new QLabel(field->name());
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

bool tabRepair::checkStatus(const int index)
{
    bool ret = 0;

    QString allowedForRoles = statusesProxyModel->index(index, 6).data().toString();

    if(userDbData->value("roles").toString().contains(QRegularExpression(QString("\\b(%1)\\b").arg(allowedForRoles))))
        ret = 1;
    else
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Проверьте права доступа или обратитесь к администратору"), QColor(212,237,242), QColor(229,244,247));

    if(!ret)
        throw 2;

    return ret;
}

/* Проверка данных перед сметой статуса или выдачей
*/
bool tabRepair::checkData()
{
    bool ret = 1;

    // проверка данных...

    if(!ret)
        throw 2;

    return ret;
}

void tabRepair::updateTotalSumms()
{
    works_sum = 0;
    parts_sum = 0;
    total_sum = 0;
    for(int i=0;i<worksAndPartsModel->rowCount();i++)
    {
        if(worksAndPartsModel->record(i).value(worksAndSparePartsDataModel::item_rsrv_id).toInt())
            parts_sum += worksAndPartsModel->record(i).value(worksAndSparePartsDataModel::summ).toFloat();
        else
            works_sum += worksAndPartsModel->record(i).value(worksAndSparePartsDataModel::summ).toFloat();

        total_sum +=  worksAndPartsModel->record(i).value(worksAndSparePartsDataModel::summ).toFloat();
    }
    ui->lineEditWorksAmount->setText(sysLocale.toString(works_sum, 'f', 2));
    ui->lineEditSparePartsAmount->setText(sysLocale.toString(parts_sum, 'f', 2));
    ui->lineEditTotalAmount->setText(sysLocale.toString(total_sum, 'f', 2));
}

void tabRepair::createGetOutDialog()
{
    overlay = new QWidget(this);
    overlay->setStyleSheet("QWidget { background: rgba(154, 154, 154, 128);}");
    overlay->resize(size());
    overlay->setVisible(true);

    modalWidget = new getOutDialog(this, Qt::SplashScreen);
    QObject::connect(modalWidget, SIGNAL(close()), this, SLOT(closeGetOutDialog()));
    QObject::connect(modalWidget, SIGNAL(getOutOk()), this, SLOT(updateWidgets()));

    modalWidget ->setWindowModality(Qt::WindowModal);
    modalWidget ->show();
}

void tabRepair::closeGetOutDialog()
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

void tabRepair::openPrevRepair()
{
    emit createTabPrevRepair(ui->lineEditPrevRepair->text().toInt());
}

void tabRepair::printStickers(int)
{
    QMap<QString, QVariant> report_vars;
    report_vars.insert("type", "rep_label");
    report_vars.insert("repair_id", repair_id);
    emit generatePrintout(report_vars);
}

void tabRepair::changeOffice(int)
{

}

void tabRepair::changeManager(int)
{

}

void tabRepair::changeEngineer(int)
{

}

void tabRepair::openInvoice(int)
{

}

void tabRepair::quickAddSparePartByUID(int)
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

void tabRepair::worksTreeDoubleClicked(QModelIndex item)
{
    //    emit this->worksTreeDoubleClicked(ui->tableWidget->item(item->row(), item->column())->text().toInt());
}

void tabRepair::saveStatus()
{
    saveStatus(ui->comboBoxStatus->currentIndex());
}

void tabRepair::updateStatesModel(int index)
{
    int statusId = statusesProxyModel->databaseIDByRow(index);
    QString allowedStates = statusesProxyModel->index(index, 4).data().toString();
    QString activeStatus = statusesProxyModel->getDisplayRole(statusId);
    ui->comboBoxStatus->blockSignals(true);
    statusesProxyModel->setFilterRegularExpression(QString("\\b(%1)\\b").arg(allowedStates));
    ui->comboBoxStatus->setCurrentIndex(-1);
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    ui->comboBoxStatus->setPlaceholderText(activeStatus);
    ui->comboBoxStatus->blockSignals(false);
}

void tabRepair::saveStatus(int index)
{
    if (index < 0)
        return;

    m_getOutButtonVisible = 0;
    try
    {
        checkStatus(index);
        checkData();
        repairModel->setState(statusesProxyModel->databaseIDByRow(index));
        QUERY_LOG_START(metaObject()->className());
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        nErr = repairModel->commit();
        QUERY_COMMIT_ROLLBACK(query,nErr);
        QUERY_LOG_STOP;
        updateStatesModel(index);
    }
    catch (int type)
    {
        nErr = 0;
        ui->comboBoxStatus->blockSignals(true);
        ui->comboBoxStatus->setCurrentIndex(-1);
        ui->comboBoxStatus->blockSignals(false);
        if(type == 0)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
        return;
    }

    m_buttonSaveStateEnabled = 0;
    if((repairModel->state() == 6 || repairModel->state() == 7) && permissions->contains("4"))
        m_getOutButtonVisible = 1;

    updateWidgets();
}

void tabRepair::comboBoxIndexChanged(int index)
{
    if(save_state_on_close)
    {
        saveStatus(index);
        return;
    }
    else
    {
        m_buttonSaveStateEnabled = 1;
        ui->toolButtonSaveStatus->setEnabled(true); // нет смысла вызывать метод updateWidgets() ради одной кнопки
    }
}

//void tabRepair::lineEditSearchTextChanged(QString)
//{

//}

//void tabRepair::lineEditSearchReturnPressed()
//{

//}

tabRepair* tabRepair::getInstance(int rep_id, MainWindow *parent)   // singleton: одна вкладка для ремонта
{
if( !p_instance.contains(rep_id) )
  p_instance.insert(rep_id, new tabRepair(rep_id, parent));
return p_instance.value(rep_id);
}


commentsTable::commentsTable(QWidget *parent) :
    QTableView(parent)
{
    setShowGrid(false);
}

commentsTable::~commentsTable()
{

}

void commentsTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
//    setColumnWidth(0, 120);
//    setColumnWidth(1, 100);
    resizeColumnToContents(0);
    resizeColumnToContents(1);
    if (verticalScrollBar()->isVisible())
        setColumnWidth(2, geometry().width() - verticalScrollBar()->width() - columnWidth(0) - columnWidth(1) - 2);
    else
        setColumnWidth(2, geometry().width() - columnWidth(0) - columnWidth(1) - 2);
    resizeRowsToContents();
}

#if QT_VERSION >= 0x060000
void commentsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void commentsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    qDebug() << "commentsTable::dataChanged()"; // TODO: разообраться, почему этот слот не вызывается при обновлении модели.
    QTableView::dataChanged(topLeft,bottomRight,roles);
    resizeRowsToContents();
}

commentsDataModel::commentsDataModel(QWidget *parent) :
    QSqlQueryModel(parent)
{

}

commentsDataModel::~commentsDataModel()
{

}

QVariant commentsDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    // FIXME: Implement me!
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)    // преобразование времени в локальное
        {
            QDateTime date = QSqlQueryModel::data(index, role).toDateTime();
            date.setTimeZone(QTimeZone::utc());
            return date.toLocalTime().toString("dd.MM.yyyy hh:mm:ss");
        }
        if (index.column() == 1)    // имя пользователя
            return allUsersMap->value(QSqlQueryModel::data(index, role).toInt());
    }
    return QSqlQueryModel::data(index, role);   // или если просто возвращать данные наследуемого объекта, то тоже всё ОК
}

// =====================================
worksAndSparePartsTable::worksAndSparePartsTable(QWidget *parent) :
    QTableView(parent)
{
}

worksAndSparePartsTable::~worksAndSparePartsTable()
{
}

void worksAndSparePartsTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    int i;
    int colWidths[] = {30,0,30,50,50,60,60,100};
    int colNameWidth = 0;

    horizontalHeader()->hideSection(10); // прячем служебные столбцы
    horizontalHeader()->hideSection(9);
    horizontalHeader()->hideSection(8);

    for (i = 0; i < 8; i++)
    {
        colNameWidth += colWidths[i];
        setColumnWidth(i, colWidths[i]);
    }
    colNameWidth = geometry().width() - verticalScrollBar()->width() - colNameWidth;
    if (verticalScrollBar()->isVisible())
        setColumnWidth(1, colNameWidth - verticalScrollBar()->width());
    else
        setColumnWidth(1, colNameWidth);
    resizeRowsToContents();
}

#if QT_VERSION >= 0x060000
void worksAndSparePartsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void worksAndSparePartsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    QTableView::dataChanged(topLeft,bottomRight,roles);
    resizeRowsToContents();
}

worksAndSparePartsDataModel::worksAndSparePartsDataModel(QWidget *parent) :
    QSqlQueryModel(parent)
{

}

worksAndSparePartsDataModel::~worksAndSparePartsDataModel()
{

}

QVariant worksAndSparePartsDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return false;

    // FIXME: Implement me!
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
            case actions: return QSqlQueryModel::data(index, role);
            case price: return sysLocale.toString(QSqlQueryModel::data(index, role).toFloat(), 'f', 2);
            case summ: return sysLocale.toString(QSqlQueryModel::data(index, role).toFloat(), 'f', 2);
            case user: return allUsersMap->value(QSqlQueryModel::data(index, role).toInt());
            case warranty: return warrantyTermsMap->value(QSqlQueryModel::data(index, role).toInt());
        }
    }
    return QSqlQueryModel::data(index, role);   // или если просто возвращать данные наследуемого объекта, то тоже всё ОК
}
