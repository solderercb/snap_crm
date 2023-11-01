#include "global.h"
#include "appver.h"
#include "tabclients.h"
#include "ui_tabclients.h"
#include "mainwindow.h"
#include "com_sql_queries.h"

tabClients* tabClients::p_instance[] = {nullptr,nullptr};

tabClients::tabClients(bool type, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabClients)
{

    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    m_type = type;
    m_tableUpdateDelay = new QTimer();

    clientsTable = new STableBaseModel();
    ui->tableView->setModel(clientsTable);
    if (type == 1)
    {
        ui->buttonPrint->hide();
        ui->buttonClientNew->hide();
        userActivityLog->appendRecord(tr("Navigation Выбор клиента"));
    }
    else
        userActivityLog->appendRecord(tr("Navigation Клиенты"));

    ui->listViewClientsType->setModel(clientsTypesList);
    ui->listViewClientsType->setModelColumn(0);
    if (userDbData->preferRegular)
        ui->listViewClientsType->setCurrentIndex(clientsTypesList->index(4, 0));
    else
        ui->listViewClientsType->setCurrentIndex(clientsTypesList->index(0, 0));    // по умолчанию выбираем одну из категорий; обязательно! иначе будет вылетать при сборке условия в updateTableWidget()

    ui->comboBoxClientAdType->lineEdit()->setPlaceholderText(tr("источник обращения"));
    ui->comboBoxClientAdType->setButtons("Clear");
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setCurrentIndex(-1);

    connect(ui->buttonRefreshTable, &QPushButton::clicked, this, &tabClients::buttonRefreshClicked);
    connect(m_tableUpdateDelay, &QTimer::timeout, this, &tabClients::autorefreshTable);
    m_tableUpdateDelay->setSingleShot(true);

    refreshTable();
}

tabClients::~tabClients()
{
    p_instance[this->m_type] = nullptr;   // Обязательно блять!
    delete ui;
    delete m_tableUpdateDelay;
}

QString tabClients::tabTitle()
{
    if(m_type)
        return tr("Выбрать клиента");
    else
        return tr("Клиенты");
}

tabClients* tabClients::getInstance(bool type, MainWindow *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
    if( !p_instance[type] )
      p_instance[type] = new tabClients(type, parent);
    return p_instance[type];
}

void tabClients::lineEditSearchSetFocus()
{
//    ui->lineEditSearch->setFocusPolicy(Qt::StrongFocus);
    ui->lineEditSearch->setFocus();
}

void tabClients::refreshTable(bool preserveScrollPos, bool preserveSelection)
{
    ui->tableView->setQuery(QUERY_SEL_CLIENTS_STATIC, QSqlDatabase::database("connMain"));
    ui->tableView->setUniqueIdColumn(0);
    FilterList l1;
    l1.op = FilterList::And;

    l1.fields.append(STableViewBase::initFilterField("t1." + clientsTypesList->item(ui->listViewClientsType->currentIndex().row(), 2)->text(), FilterField::NoOp, 1));  // категория клиентов
    if(!ui->checkBoxShowArchived->isChecked())
        l1.fields.append(STableViewBase::initFilterField("t1.`state`", FilterField::Equals, "1"));
    // TODO: создать свой ComboBox с кнопкой 🗙
    if (ui->comboBoxClientAdType->currentIndex() >= 0 )
        l1.fields.append(STableViewBase::initFilterField("`visit_source`", FilterField::Equals, clientAdTypesList->index(ui->comboBoxClientAdType->currentIndex(), 1).data().toString()));


    if (ui->lineEditSearch->text().length() > 0)    // только если строка поиска не пуста
    {
        FilterList l2;
        l2.op = FilterList::Or;
        QString str = ui->lineEditSearch->text();
        l2.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)", FilterField::RegExp, str, Qt::CaseInsensitive));
        l2.fields.append(STableViewBase::initFilterField("t1.`short_name`", FilterField::RegExp, str, Qt::CaseInsensitive));
        l2.fields.append(STableViewBase::initFilterField("t1.`id`", FilterField::Equals, str, Qt::CaseInsensitive));
        l2.fields.append(STableViewBase::initFilterField("t2.`phone`", FilterField::RegExp, str, Qt::CaseInsensitive));
        l2.fields.append(STableViewBase::initFilterField("t2.`phone_clean`", FilterField::RegExp, str, Qt::CaseInsensitive));
        l1.childs.append(l2);
     }

    query_group.clear();
    query_group << "`id`";    // default GROUP part of query

    ui->tableView->setFilter(l1);
    ui->tableView->setGrouping(query_group);
    ui->tableView->refresh(preserveScrollPos, preserveSelection);
    ui->labelClientsCounter->setText(QString::number(clientsTable->rowCount()));

    m_tableUpdateDelay->stop();
    m_tableUpdateDelay->start(10000);
}

void tabClients::clientTypeChanged(QModelIndex)
{
//    qDebug() << "clientTypeChanged(QModelIndex): item1 =" << clientsTypesList->index(index.row(), 0).data() << ", item2 = " << clientsTypesList->index(index.row(), 1).data() << "item3 = " << clientsTypesList->index(index.row(), 2).data();
    refreshTable();
}

void tabClients::clientAdvertisingChanged(int)
{
//    qDebug() << "SLOT clientAdvertisingChanged(int index), index = " << ui->comboBoxClientAdType->currentIndex();
    refreshTable();
}

void tabClients::tableItemDoubleClick(QModelIndex item)
{
    emit doubleClicked(clientsTable->index(item.row(), 0).data().toInt());
    if (m_type == 1)
    {
        emit activateCaller(callerPtr);
        deleteLater();
    }
}

void tabClients::lineEditSearchTextChanged(QString)
{   // задержка поиска; запрос к базе будет выполняться после каждого введённого символа и при быстром наборе текста прога тормозит
//    qDebug() << "SLOT tabClients::lineEditSearchTextChanged(QString search_str), search_str = " << search_str;
    m_tableUpdateDelay->stop();
    m_tableUpdateDelay->start(350);
}

void tabClients::lineEditSearchReturnPressed()
{

}

void tabClients::togglePropertiesPanel()
{

}

void tabClients::buttonRefreshClicked()
{
    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
}

void tabClients::autorefreshTable()
{
    refreshTable(STableViewBase::ScrollPosPreserve, STableViewBase::SelectionPreserve);
}


