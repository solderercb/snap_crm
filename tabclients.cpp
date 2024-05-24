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
    m_type = type;    // инициализация до вызова любых методов

    logUserActivity();

    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    clientsTable = new STableBaseModel(this);
    ui->tableView->setModel(clientsTable);
    ui->tableView->setQuery(QUERY_SEL_CLIENTS_STATIC, QSqlDatabase::database("connMain"));
    ui->tableView->setUniqueIdColumn(0);
    ui->tableView->enableAutorefresh(userDbData->refreshTime*1000);
    if (type == 1)
    {
        ui->buttonPrint->hide();
        ui->buttonClientNew->hide();
    }

    ui->listViewClientsType->setModel(clientsTypesList);
    ui->listViewClientsType->setModelColumn(0);
    if (userDbData->preferRegular)
        ui->listViewClientsType->setCurrentIndex(clientsTypesList->index(4, 0));
    else
        ui->listViewClientsType->setCurrentIndex(clientsTypesList->index(0, 0));    // по умолчанию выбираем одну из категорий; обязательно! иначе будет вылетать при сборке условия в updateTableWidget()

    ui->comboBoxClientAdType->setPlaceholderText(tr("источник обращения"));
    ui->comboBoxClientAdType->setButtons("Clear");
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setCurrentIndex(-1);

    connect(ui->buttonRefreshTable, &QPushButton::clicked, this, &tabClients::buttonRefreshClicked);

    refreshTable();
}

tabClients::~tabClients()
{
    p_instance[this->m_type] = nullptr;   // Обязательно блять!
    delete ui;
}

QString tabClients::tabTitle()
{
    if(m_type)
        return tr("Выбрать клиента");
    else
        return tr("Клиенты");
}

bool tabClients::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride)
    {
        // TODO: настраиваемые горячие клавиши
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier))
            switch(keyEvent->key())
            {
                // вызов метода accept() останавливает распространение события на родительские объекты (MainWindow не получит ShortcutOverride),
                // однако, следующие за ним события keyPress и keyRelease распространяются; TODO: разобраться нужно ли их фильтровать тоже
                case Qt::Key_F: setFocusSearchField(); event->accept(); break;
                case Qt::Key_N: createNewClient(); event->accept(); break;
                default: ;
            }
    }
    return tabCommon::event(event);
}

tabClients* tabClients::getInstance(bool type, MainWindow *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
    if( !p_instance[type] )
      p_instance[type] = new tabClients(type, parent);
    return p_instance[type];
}

void tabClients::setFocusSearchField()
{
    ui->lineEditSearch->setFocus(Qt::MouseFocusReason);
    ui->lineEditSearch->selectAll();
}

void tabClients::constructQueryClause()
{
    FilterList list;
    list.op = FilterList::And;

    list.fields.append(STableViewBase::initFilterField("t1." + clientsTypesList->item(ui->listViewClientsType->currentIndex().row(), 2)->text(), FilterField::NoOp, 1));  // категория клиентов
    if(!ui->checkBoxShowArchived->isChecked())
      list.fields.append(STableViewBase::initFilterField("t1.`state`", FilterField::Equals, "1"));
    if (ui->comboBoxClientAdType->currentIndex() >= 0 )
      list.fields.append(STableViewBase::initFilterField("`visit_source`", FilterField::Equals, clientAdTypesList->index(ui->comboBoxClientAdType->currentIndex(), 1).data().toString()));

    if (ui->lineEditSearch->text().length() > 0)    // только если строка поиска не пуста
    {
      FilterList searchText;
      searchText.op = FilterList::Or;
      FilterField::Op matchFlag;
      if(userDbData->useRegExpSearch)
            matchFlag = FilterField::RegExp;
      else
            matchFlag = FilterField::Contains;
      QString str = ui->lineEditSearch->text();
      searchText.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)", matchFlag, str, Qt::CaseInsensitive));
      searchText.fields.append(STableViewBase::initFilterField("t1.`short_name`", matchFlag, str, Qt::CaseInsensitive));
      searchText.fields.append(STableViewBase::initFilterField("t1.`id`", FilterField::Equals, str, Qt::CaseInsensitive));
      searchText.fields.append(STableViewBase::initFilterField("t2.`phone`", matchFlag, str, Qt::CaseInsensitive));
      searchText.fields.append(STableViewBase::initFilterField("t2.`phone_clean`", matchFlag, str, Qt::CaseInsensitive));
      list.childs.append(searchText);
    }

    query_group.clear();
    query_group << "`id`";    // default GROUP part of query

    ui->tableView->setFilter(list);
    ui->tableView->setGrouping(query_group);

}

void tabClients::refreshTable(bool preserveScrollPos, bool preserveSelection)
{
    constructQueryClause();

    ui->tableView->refresh(preserveScrollPos, preserveSelection);
    ui->labelClientsCounter->setText(QString::number(clientsTable->rowCount()));
}

void tabClients::clientTypeChanged(QModelIndex)
{
//    qDebug() << "clientTypeChanged(QModelIndex): item1 =" << clientsTypesList->index(index.row(), 0).data() << ", item2 = " << clientsTypesList->index(index.row(), 1).data() << "item3 = " << clientsTypesList->index(index.row(), 2).data();
    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
}

void tabClients::clientAdvertisingChanged(int)
{
//    qDebug() << "SLOT clientAdvertisingChanged(int index), index = " << ui->comboBoxClientAdType->currentIndex();
    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
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
    ui->tableView->clearSelection();
    ui->tableView->resetVScrollPos();
    constructQueryClause();
    ui->tableView->delayedRefresh(350);
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

void tabClients::createNewClient()
{

}


