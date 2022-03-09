#include "appver.h"
#include "tabrepair.h"
#include "ui_tabrepair.h"

tabRepair* tabRepair::p_instance = nullptr;

tabRepair::tabRepair(int repair_id, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepair)
{
    ui->setupUi(this);
    this->setWindowTitle("Ремонт " + QString::number(repair_id));
    this->setAttribute(Qt::WA_DeleteOnClose);

    works_table = new QSqlTableModel();
    ui->tableView->setModel(works_table);
    updateTableWidget();
}

tabRepair::~tabRepair()
{
    delete ui;
    p_instance = nullptr;   // Обязательно блять!
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

//void tabRepair::lineEditSearchTextChanged(QString)
//{

//}

//void tabRepair::lineEditSearchReturnPressed()
//{

//}

tabRepair* tabRepair::getInstance(MainWindow *parent)   // singleton: одна вкладка для ремонта
{
if( !p_instance )
  p_instance = new tabRepair(0, parent);
return p_instance;
}

