#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoginWindow)
{
	ui->setupUi(this);
	QObject::connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(ConnectToDB()));
	QObject::connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(slotCancelClick()));
}

LoginWindow::~LoginWindow()
{
	delete ui;
}

// SLOT
void LoginWindow::ConnectToDB()
{
    emit this->ConnectToDB(ui->editLogin->text(), ui->editPassword->text(), ui->editIPaddr->text(), ui->editPort->text().toUInt(), ui->editDBName->text(), "connectionDefault");
}

// SLOT
void LoginWindow::slotCancelClick()
{
	qDebug("Application close.");
	emit this->btnCancelClick();
}

void LoginWindow::DBConnectErr(const QString &message)
{
	ui->labelStatus->setText(message);
}
