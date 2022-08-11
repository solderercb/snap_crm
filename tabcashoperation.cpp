#include "global.h"
#include "tabcashoperation.h"
#include "ui_tabcashoperation.h"

QMap<int, tabCashOperation*> tabCashOperation::p_instance;

tabCashOperation::tabCashOperation(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabCashOperation)
{
    ui->setupUi(this);
    connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillClientCreds(int)));
    updateWidgets();
}

tabCashOperation::tabCashOperation(int order, MainWindow *parent) :
    tabCashOperation(parent)
{

}

tabCashOperation::~tabCashOperation()
{
    delete ui;
}

void tabCashOperation::updateWidgets()
{
    if(order_id)
    {
        ui->buttonPrint->show();
        ui->buttonSave->hide();
    }
    else
    {
        ui->buttonSave->show();
        ui->buttonPrint->hide();
    }
}

void tabCashOperation::clearClientCreds(bool)
{

}

void tabCashOperation::fillClientCreds(int)
{

}

void tabCashOperation::orderTypeChanged(int)
{

}

void tabCashOperation::dateChanged(QDate)
{

}

void tabCashOperation::findClientByLastname(QString)
{

}

void tabCashOperation::openDocument()
{

}

void tabCashOperation::amountChanged(QString)
{

}

void tabCashOperation::constructReason(int)
{

}

void tabCashOperation::paymentSystemChanged(int)
{

}
