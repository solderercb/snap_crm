#include "getOutDialog.h"
#include "ui_getOutDialog.h"

getOutDialog::getOutDialog(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::getOutDialog)
{
    ui->setupUi(this);
//    shortlivedNotification::setSize(385, 90);
    shortlivedNotification::setAppearance(this, shortlivedNotification::bottomRight);
}

getOutDialog::~getOutDialog()
{
    delete ui;
    emit close();
}

void getOutDialog::getOut()
{
    if(!ui->checkBoxPaymentCheckout->isChecked())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Подтвердите правильность ввода данных", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
    }
}

void getOutDialog::cancel()
{
    emit close();
//    this->deleteLater();
}

void getOutDialog::createLooseDoc()
{

}
