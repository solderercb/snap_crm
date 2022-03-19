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
    qDebug() << "tabRepair constructor";
    ui->setupUi(this);
    getRepairData();
    connect(ui->comboBoxStatus, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIndexChanged(int)));

//    this->setWindowTitle("Ремонт " + QString::number(repair_id));
    ui->lineEditRepairId->setText(QString::number(repair_id));
    ui->lineEditDevice->setText(repairModel->record(0).value("Title").toString());
    ui->lineEditSN->setText(repairModel->record(0).value("serial_number").toString());
    ui->lineEditClient->setText(clientModel->record(0).value("FioOrUrName").toString());
    ui->lineEditInDate->setText(repairModel->record(0).value("in_date").toDateTime().toLocalTime().toString());
    if (repairModel->record(0).value("state").toInt() != 8)
    {
        ui->lineEditOutDate->setHidden(true);   // TODO: нужен более гибкий способ скрытия поля с датой выдачи ремонта, если статус не "Выдано клиенту" (id!=8)
        ui->labelOutDate->setHidden(true);
    }
    else
        ui->lineEditOutDate->setText(repairModel->record(0).value("out_date").toDateTime().toLocalTime().toString());

    ui->lineEditOffice->setText(getDisplayRoleById(repairModel->record(0).value("office").toInt(), officesModel, getFieldIdByName("id", officesModel)));
    ui->lineEditManager->setText(getDisplayRoleById(repairModel->record(0).value("manager").toInt(), allUsersModel, getFieldIdByName("id", allUsersModel)));
    ui->lineEditEngineer->setText(getDisplayRoleById(repairModel->record(0).value("master").toInt(), allUsersModel, getFieldIdByName("id", allUsersModel)));
    if (!repairModel->record(0).value("is_pre_agreed").toBool())
    {
        ui->lineEditPreagreedAmount->setHidden(true);
        ui->labelPreagreedAmount->setHidden(true);
    }
    else
        ui->lineEditPreagreedAmount->setText(repairModel->record(0).value("pre_agreed_amount").toString() + comSettings->value("currency").toString());        // TODO: изменить банковское обозначение валюты на локализованное сокращение или символ

    ui->lineEditBox->setText(getDisplayRoleById(repairModel->record(0).value("box").toInt(), repairBoxesModel, getFieldIdByName("id", repairBoxesModel)));
    ui->lineEditColor->setText(repairModel->record(0).value("color").toString());
    ui->lineEditWarrantyLabel->setText(repairModel->record(0).value("warranty_label").toString());

    // TODO: добавить игнор колёсика мышки для комбобоксов статусов.
    ui->comboBoxStatus->setModel(statusesProxyModel);
    ui->comboBoxNotifyStatus->setModel(notifyStatusesModel);
    statusesProxyModel->setFilterKeyColumn(1);
    statusesProxyModel->setFilterRegularExpression("");
    statusUpdateFlag = 0;
    ui->comboBoxStatus->setCurrentText(getDisplayRoleById(repairModel->record(0).value("state").toInt(), statusesModel, 1));
    ui->comboBoxNotifyStatus->setCurrentText(getDisplayRoleById(repairModel->record(0).value("informed_status").toInt(), notifyStatusesModel, 1));
    ui->lineEditProblem->setText(repairModel->record(0).value("fault").toString());
    ui->lineEditIncomingSet->setText(repairModel->record(0).value("complect").toString());
    ui->lineEditExterior->setText(repairModel->record(0).value("look").toString());
    ui->textEditDiagResult->setText(repairModel->record(0).value("diagnostic_result").toString());
    ui->lineEditAgreedAmount->setText(repairModel->record(0).value("repair_cost").toString());
    ui->tableViewComments->setModel(commentsModel);
    ui->tableViewComments->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewComments->verticalHeader()->hide();
    ui->tableViewComments->horizontalHeader()->hide();

    this->setAttribute(Qt::WA_DeleteOnClose);

    works_table = new QSqlTableModel();
    ui->tableViewWorksAndSpareParts->setModel(works_table);
    updateTableWidget();
}

tabRepair::~tabRepair()
{
    delete ui;
    p_instance.remove(repair_id);   // Обязательно блять!
}

void tabRepair::getRepairData()
{
    repairModel = new QSqlQueryModel();
    repairModel->setQuery(QUERY_SEL_REPAIR_RPRT(repair_id));

    clientModel = new QSqlQueryModel();
    clientModel->setQuery(QUERY_SEL_CLIENT_RPRT(repairModel->record(0).value("client").toInt()));

    // из всех параметров для отёта пригодится только валюта
    // TODO: изменить банковское обозначение валюты на локализованное сокращение или символ
    QStandardItemModel *configModel = new QStandardItemModel();
    QStringList headers = {"currency"};
    configModel->setHorizontalHeaderLabels(headers);
    configModel->appendRow(new QStandardItem(comSettings->value("currency").toString()));

    fieldsModel = new QSqlQueryModel();
    fieldsModel->setQuery(QUERY_SEL_REP_FIELDS_RPRT(repair_id));

    statusesProxyModel = new QSortFilterProxyModel;
    statusesProxyModel->setSourceModel(statusesModel);

    commentsModel = new commentsDataModel();
    commentsModel->setQuery(QUERY_SEL_REPAIR_DATA(repair_id));
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

void tabRepair::updateTableWidget()
{
    works_table->setQuery("SELECT 1;", QSqlDatabase::database("connMain"));

//    repairs_table->setHeaderData(0, Qt::Horizontal, tr("Name"));
//    repairs_table->setHeaderData(1, Qt::Horizontal, tr("Salary"));
//    ui->tableView->show();
}

void tabRepair::worksTreeDoubleClicked(QModelIndex item)
{
    //    emit this->worksTreeDoubleClicked(ui->tableWidget->item(item->row(), item->column())->text().toInt());
}

void tabRepair::comboBoxIndexChanged(int index)
{
    if (index >= 0)
    {
        if (!statusUpdateFlag)
        {
            QString activeStatus = statusesProxyModel->index(index, 0).data().toString();
            statusUpdateFlag = 1; // защита от зацикливания: при обновлении фильтра прокси модели происходит установка текущего индекса равным 0 и генерируется сигнал currentIndexChanged()
            statusesProxyModel->setFilterRegularExpression(QString("\\b(") + statusesProxyModel->index(index, 4).data().toString() + QString(")\\b"));
            ui->comboBoxStatus->setCurrentIndex(-1);
            ui->comboBoxStatus->setPlaceholderText(activeStatus);
            statusUpdateFlag = 0;
        }
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
//            date = QSqlQueryModel::data(index, role).toDateTime();
            date.setTimeZone(QTimeZone::utc());
//            qDebug() << date.toLocalTime().toString("dd.MM.yyyy hh:mm:dd");
            return date.toLocalTime().toString("dd.MM.yyyy hh:mm:dd");
        }
        if (index.column() == 1)    // имя пользователя
            return allUsersMap->value(QSqlQueryModel::data(index, role).toInt());
    }
    return QSqlQueryModel::data(index, role);   // или если просто возвращать данные наследуемого объекта, то тоже всё ОК
}
