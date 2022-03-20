#include "getOutDialog.h"
#include "ui_getOutDialog.h"

getOutDialog::getOutDialog(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::getOutDialog)
{
    ui->setupUi(this);
}

getOutDialog::~getOutDialog()
{
    delete ui;
    emit close();
}

void getOutDialog::getOut()
{

}

void getOutDialog::cancel()
{
    emit close();
//    this->deleteLater();
}

void getOutDialog::createLooseDoc()
{

}
