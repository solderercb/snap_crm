#include "global.h"
#include "getOutDialog.h"
#include "ui_getOutDialog.h"
#include "tabrepair.h"

getOutDialog::getOutDialog(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::getOutDialog)
{
    ui->setupUi(this);
    tabRepair *parentTab = static_cast<tabRepair*>(parent);
    if (parentTab->repairModel->record(0).value("state").toInt() != 7)
    {
        ui->comboBoxRejectReason->setHidden(true);   // TODO: нужен более гибкий способ скрытия комбобокса "Причина отказа" (id!=7)
        ui->checkBoxConfirmGetOut->setHidden(true);
    }
    else
    {
        ui->checkBoxPaymentCheckout->setHidden(true);
    }
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->checkBoxCreditPayment->setEnabled(parentTab->clientModel->record(0).value("balance_enable").toBool());
    ui->lineEditPrepay->setText(QString::number(parentTab->repairModel->record(0).value("prepaid_summ").toFloat(), 'f', 2));
    ui->lineEditAgreedAmount->setText(QString::number(parentTab->repairModel->record(0).value("repair_cost").toFloat(), 'f', 2));
    ui->lineEditAlreadyPayed->setText(QString::number(parentTab->repairModel->record(0).value("prepaid_summ").toFloat(), 'f', 2));  // назначение данного поля не понятно, оно вроде бы, всегда равно полю Предоплата
    ui->lineEditTotalAmount->setText(QString::number(parentTab->total_sum, 'f', 2));
    ui->lineEditCurrentPaymentAmount->setText(QString::number(parentTab->total_sum - parentTab->repairModel->record(0).value("prepaid_summ").toFloat(), 'f', 2));
    ui->comboBoxPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxPaymentAccount->setCurrentText("Наличные");
    if(parentTab->repairModel->record(0).value("repair_cost").toFloat() > parentTab->total_sum) // если сумма работ и деталей меньше согласованной, отобразится сообщение
        summsNotEq = 1;
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
    if( ui->checkBoxPaymentCheckout->isVisible() && !ui->checkBoxPaymentCheckout->isChecked())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Подтвердите правильность ввода данных", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
    }
    if( ui->checkBoxConfirmGetOut->isVisible() && !ui->checkBoxConfirmGetOut->isChecked())
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Подтвердите выдачу", QColor("#FFC7AD"), QColor("#FFA477"));
        newPopup->setVisible(true);
    }
    if(summsNotEq)
    {
        //TODO: здесь нужно показать диалог: "Сумма ремонта меньше согласованной. Всё ОК?" с кнопками Да/Нет; а пока предупреждалка...
        shortlivedNotification *newPopup = new shortlivedNotification(this, "Ой-ай", "Согласованная стоимость больше суммы работ и деталей", QColor("#FFC7AD"), QColor("#FFA477"));
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
