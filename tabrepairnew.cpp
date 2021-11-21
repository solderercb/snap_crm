#include "tabrepairnew.h"
#include "ui_tabrepairnew.h"

tabRepairNew::tabRepairNew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabRepairNew)
{
    ui->setupUi(this);
}

tabRepairNew::~tabRepairNew()
{
    delete ui;
}
