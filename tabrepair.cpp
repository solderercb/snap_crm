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
    repairModel2 = new SRepairModel();
    repairModel2->load(rep_id);
    setLock(1);
    ui->setupUi(this);
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

    ui->comboBoxStatus->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxNotifyStatus->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPlace->setStyleSheet(commonComboBoxStyleSheet);
    connect(ui->comboBoxStatus, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIndexChanged(int)));
    connect(ui->toolButtonSaveStatus, SIGNAL(clicked()), this, SLOT(saveStatus()));

    repairModel = new QSqlQueryModel();
    clientModel = new QSqlQueryModel();
    if (repairModel->record(0).value("user_lock").toInt());
        // TODO: Добавлять символ 🔒 в название вкладки
    fieldsModel = new QSqlQueryModel();
    statusesProxyModel = new QSortFilterProxyModel;
    statusesProxyModel->setSourceModel(statusesModel);
    worksAndPartsModel = new worksAndSparePartsDataModel;
    connect(worksAndPartsModel, SIGNAL(modelReset()), this, SLOT(updateTotalSumms()));  // TODO: уточнить генерируется ли сигнал при изменении существующих данных
    commentsModel = new commentsDataModel();
//    updateRepairData();

    ui->comboBoxPlace->setModel(repairBoxesModel);
    ui->comboBoxStatus->setModel(statusesProxyModel);
    ui->comboBoxNotifyStatus->setModel(notifyStatusesModel);
    statusesProxyModel->setFilterKeyColumn(1);
    statusesProxyModel->setFilterRegularExpression("");
    statusUpdateFlag = 0;

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

    connect(ui->pushButtonManualUpdateRepairData, SIGNAL(clicked()), this, SLOT(updateWidgets()));

    updateWidgets();


#ifdef QT_DEBUG
//    createGetOutDialog();
#else
    ui->pushButtonManualUpdateRepairData->setHidden(true);
#endif
}

tabRepair::~tabRepair()
{
    userActivityLog->updateActivityTimestamp();

    setLock(0);
    delete ui;
    p_instance.remove(repair_id);   // Обязательно блять!
}

QString tabRepair::tabTitle()
{

}

void tabRepair::updateRepairData()
{
    repairModel->setQuery(QUERY_SEL_REPAIR_RPRT(repair_id), QSqlDatabase::database("connMain"));
    clientModel->setQuery(QUERY_SEL_CLIENT_RPRT(repairModel->record(0).value("client").toInt()));
    fieldsModel->setQuery(QUERY_SEL_REPAIR_ADD_FIELDS(repair_id), QSqlDatabase::database("connMain"));
    worksAndPartsModel->setQuery(QUERY_SEL_REPAIR_WORKS_AND_PARTS(repair_id));
    commentsModel->setQuery(QUERY_SEL_REPAIR_COMMENTS(repair_id));
}

void tabRepair::updateWidgets()
{
    updateRepairData();
    ui->lineEditRepairId->setText(QString::number(repair_id));
    ui->lineEditDevice->setText(repairModel->record(0).value("Title").toString());
    ui->lineEditSN->setText(repairModel->record(0).value("serial_number").toString());
    ui->lineEditClient->setText(clientModel->record(0).value("FioOrUrName").toString());
    QDateTime date = repairModel->record(0).value("in_date").toDateTime();
    date.setTimeZone(QTimeZone::utc());
    ui->lineEditInDate->setText(date.toLocalTime().toString("dd.MM.yyyy hh:mm:ss"));
    if (repairModel->record(0).value("state").toInt() != 8 && repairModel->record(0).value("state").toInt() != 12)
    {
        ui->lineEditOutDate->setHidden(true);   // TODO: нужен более гибкий способ скрытия поля с датой выдачи ремонта, если статус не "Выдано клиенту" или "Готово к выдаче без ремонта" (id!=8, id!=12)
        ui->labelOutDate->setHidden(true);
    }
    else
    {
        date = repairModel->record(0).value("out_date").toDateTime();
        date.setTimeZone(QTimeZone::utc());
        ui->lineEditOutDate->setText(date.toLocalTime().toString("dd.MM.yyyy hh:mm:ss"));
        ui->lineEditOutDate->setHidden(false);
        ui->labelOutDate->setHidden(false);
    }
    if ( worksAndPartsEditEnabled || !permissions->contains("TODO: разреш. на адм. правку списка раб. и дет."))
    {
        ui->pushButtonAdmEditWorks->setHidden(true);
    }
    else
    {
        ui->pushButtonAdmEditWorks->setHidden(false);
    }
    if (repairModel->record(0).value("state").toInt() != 6 && repairModel->record(0).value("state").toInt() != 7 || !permissions->contains("4"))
    {
        ui->pushButtonGetout->setHidden(true);   // TODO: нужен более гибкий способ скрытия кнопки "Выдать", если статус не "Готово к выдаче" или "Готово к выдаче без ремонта" (id!=6, id!=7)
    }
    else
    {
        ui->pushButtonGetout->setHidden(false);
    }

    if( repairModel->record(0).value("ext_early").toString() != "" )
    {
        ui->labelExtPrevRepair->show();
        ui->lineEditExtPrevRepair->show();
        ui->lineEditExtPrevRepair->setText(repairModel->record(0).value("ext_early").toString());
    }
    else
    {
        ui->labelExtPrevRepair->hide();
        ui->lineEditExtPrevRepair->hide();
    }
    ui->lineEditOffice->setText(getDisplayRoleById(repairModel->record(0).value("office").toInt(), officesModel, getFieldIdByName("id", officesModel)));
    ui->lineEditManager->setText(getDisplayRoleById(repairModel->record(0).value("manager").toInt(), allUsersModel, getFieldIdByName("id", allUsersModel)));
    ui->lineEditEngineer->setText(getDisplayRoleById(repairModel->record(0).value("master").toInt(), allUsersModel, getFieldIdByName("id", allUsersModel)));
    if (!repairModel->record(0).value("is_pre_agreed").toBool())
    {
        ui->lineEditPreagreedAmount->setHidden(true);
        ui->labelPreagreedAmount->setHidden(true);
    }
    else
        ui->lineEditPreagreedAmount->setText(sysLocale.toCurrencyString(repairModel->record(0).value("pre_agreed_amount").toFloat()));        // TODO: заменить системное обозначение валюты на валюту заданную в таблице БД config

    ui->comboBoxPlace->setCurrentIndex(-1);
    ui->comboBoxPlace->setCurrentText(getDisplayRoleById(repairModel->record(0).value("box").toInt(), repairBoxesModel, getFieldIdByName("id", repairBoxesModel)));
    box_name = ui->comboBoxPlace->currentText();
    ui->lineEditColor->setText(repairModel->record(0).value("color").toString());
    ui->lineEditWarrantyLabel->setText(repairModel->record(0).value("warranty_label").toString());
    if(repairModel->record(0).value("early").toInt())
        ui->lineEditPrevRepair->setText(repairModel->record(0).value("early").toString());

    ui->listWidgetExtraInfo->setHidden(true);
    ui->listWidgetExtraInfo->clear();
    if(clientModel->record(0).value("is_regular").toBool())
        ui->listWidgetExtraInfo->addItem(tr("постоянный клиент"));
    if(repairModel->record(0).value("thirs_party_sc").toBool())
        ui->listWidgetExtraInfo->addItem(tr("было в другом СЦ"));
    if(repairModel->record(0).value("can_format").toBool())
        ui->listWidgetExtraInfo->addItem(tr("данные не важны"));
    if(repairModel->record(0).value("express_repair").toBool())
        ui->listWidgetExtraInfo->addItem(tr("срочный"));
    if(clientModel->record(0).value("is_agent").toBool())
        ui->listWidgetExtraInfo->addItem(tr("посредник"));
//    if(clientModel->record(0).value("is_bad").toBool())
//        ui->listWidgetExtraInfo->addItem(tr("проблемный"));
//    if(repairModel->record(0).value("").toBool())
//        ui->listWidgetExtraInfo->addItem("");
//    if(repairModel->record(0).value("is_card_payment").toBool())
//        ui->listWidgetExtraInfo->addItem("");
    if(repairModel->record(0).value("is_repeat").toBool())
        ui->listWidgetExtraInfo->addItem(tr("повтор"));
    if(repairModel->record(0).value("is_warranty").toBool())
        ui->listWidgetExtraInfo->addItem(tr("гарантия"));
//    if(repairModel->record(0).value("is_pre_agreed").toBool())    // предварительная стоимость в отдельном lineEdit
//        ui->listWidgetExtraInfo->addItem(QString(tr("предварительная стоимость: %1")).arg(sysLocale.toCurrencyString(repairModel->record(0).value("pre_agreed_amount").toFloat())));
    if(repairModel->record(0).value("print_check").toBool())
        ui->listWidgetExtraInfo->addItem(tr("чек при выдаче"));
    if(repairModel->record(0).value("is_prepaid").toBool())
        ui->listWidgetExtraInfo->addItem(QString(tr("предоплата: %1")).arg(sysLocale.toCurrencyString(repairModel->record(0).value("prepaid_summ").toFloat())));
//    if(repairModel->record(0).value("is_debt").toBool())  // похоже не используется в АЦС
//        ui->listWidgetExtraInfo->addItem("");
    if(ui->listWidgetExtraInfo->count())
        ui->listWidgetExtraInfo->setHidden(false);

    if( repairModel->record(0).value("payment_system").toInt() == 1 && permissions->contains("65") )    // указана Безналичная оплата и есть разрешение Работать с безналичными счетами
    { // TODO: нужен более гибкий способ определения безналичного рассчета

        ui->groupBoxCashless->setHidden(false);
        if(repairModel->record(0).value("invoice").toInt()) // если уже выставлен счет
        {
            ui->lineEditInvoiceAmount->setText("TODO:");
            ui->lineEditInvoice->setText(QString("id=%1; TODO:").arg(repairModel->record(0).value("invoice").toString()));
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

    save_state_on_close = userDbDataModel->record(0).value("save_state_on_close").toBool();
    ui->comboBoxStatus->setCurrentText(getDisplayRoleById(repairModel->record(0).value("state").toInt(), statusesModel, 1));
    if(save_state_on_close)
            {
        ui->toolButtonSaveStatus->setHidden(true);
        ui->comboBoxStatus->disableWheelEvent(true);  // если включено автосохранение статуса ремонта, то нужно игнорировать колёсико мышки
    }
    else
    {
        saveStatus();
    }
    ui->comboBoxNotifyStatus->setCurrentText(getDisplayRoleById(repairModel->record(0).value("informed_status").toInt(), notifyStatusesModel, 1));
    ui->lineEditProblem->setText(repairModel->record(0).value("fault").toString());
    ui->lineEditIncomingSet->setText(repairModel->record(0).value("complect").toString());
    ui->lineEditExterior->setText(repairModel->record(0).value("look").toString());

    for(int i = ui->gridLayoutDeviceSummary->rowCount() - 1; i > 2; i-- )
    {
        ui->gridLayoutDeviceSummary->itemAtPosition(i, 1)->widget()->deleteLater();
        ui->gridLayoutDeviceSummary->itemAtPosition(i, 0)->widget()->deleteLater();
    }
    for(int i = 0; i < fieldsModel->rowCount(); i ++)
    {
        QLabel *label = new QLabel(fieldsModel->record(i).value("name").toString());
        QLineEdit *lineEdit = new QLineEdit();
        lineEdit->setText(fieldsModel->record(i).value("value").toString());
        lineEdit->setReadOnly(true);
        ui->gridLayoutDeviceSummary->addWidget(label, i + 3, 0 );
        ui->gridLayoutDeviceSummary->addWidget(lineEdit, i + 3, 1);
    }
    ui->textEditDiagResult->setText(repairModel->record(0).value("diagnostic_result").toString());
    ui->lineEditAgreedAmount->setText(sysLocale.toString(repairModel->record(0).value("repair_cost").toFloat(), 'f', 2));

    ui->tableViewWorksAndSpareParts->resizeRowsToContents();
    ui->tableViewComments->resizeRowsToContents();
}

int tabRepair::getFieldIdByName(const QString &field, QSqlQueryModel *model)
{
    for (int i = 0; i<model->columnCount(); i++)
    {
        if (model->record(0).field(i).name() == field)
        {
            return i;
        }
    }
    return -1;
}

QString tabRepair::getDisplayRoleById(int id, QAbstractItemModel *model, int column)
{
    for(int i=0; i<model->rowCount(); i++)
    {
        if(model->index(i, column).data().toInt() == id)
        {
            return model->index(i, 0).data().toString();
        }
    }
    return NULL;
}

void tabRepair::eventResize(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void tabRepair::addItemToListViewExtraInfo(QString, QString)
{
    if(ui->listWidgetExtraInfo->isHidden())
        ui->listWidgetExtraInfo->setHidden(false);
}

void tabRepair::setLock(bool lock)
{
    repairModel2->lock();
    if(lock)
        userActivityLog->appendRecord(tr("Navigation Ремонт %1").arg(repair_id));
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

void tabRepair::worksTreeDoubleClicked(QModelIndex item)
{
    //    emit this->worksTreeDoubleClicked(ui->tableWidget->item(item->row(), item->column())->text().toInt());
}

void tabRepair::saveStatus()
{
    saveStatus(ui->comboBoxStatus->currentIndex());
}

void tabRepair::saveStatus(int index)
{
    if (index >= 0)
    {
        if (!statusUpdateFlag)
        {
            QString activeStatus = statusesProxyModel->index(index, 0).data().toString();
            statusUpdateFlag = 1; // защита от зацикливания: при обновлении фильтра прокси модели происходит установка текущего индекса равным 0 и генерируется сигнал currentIndexChanged()
            statusesProxyModel->setFilterRegularExpression(QString("\\b(") + statusesProxyModel->index(index, 4).data().toString() + QString(")\\b"));
            ui->comboBoxStatus->setCurrentIndex(-1);
            // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
            ui->comboBoxStatus->setPlaceholderText(activeStatus);
            statusUpdateFlag = 0;
        }
    }
}

void tabRepair::comboBoxIndexChanged(int index)
{
    if(save_state_on_close)
    {
        saveStatus(index);
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
