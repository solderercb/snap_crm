#include "global.h"
#include "ssloptionsdialog.h"
#include "ui_ssloptionsdialog.h"

SSLOptionsDialog::SSLOptionsDialog(QWidget *p, Qt::WindowFlags f) :
    QFrame(p, f),
    ui(new Ui::SSLOptionsDialog),
    parent(p)
{
    ui->setupUi(this);
    directory = QDir::homePath();
    signalMapper = new QSignalMapper(this);
    QObject::connect(signalMapper, SIGNAL(mappedInt(int)), this, SLOT(browse(int)));

    signalMapper->setMapping(ui->toolButtonSSLKey, 0);
    QObject::connect(ui->toolButtonSSLKey, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->toolButtonSSLCert, 1);
    QObject::connect(ui->toolButtonSSLCert, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->toolButtonSSLCA, 2);
    QObject::connect(ui->toolButtonSSLCA, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->toolButtonSSLCAPath, 3);
    QObject::connect(ui->toolButtonSSLCAPath, SIGNAL(clicked()), signalMapper, SLOT(map()));

    ui->lineEditSSLKey->setText(userLocalData->SSLKey.value);
    ui->lineEditSSLCert->setText(userLocalData->SSLCert.value);
    ui->lineEditSSLCA->setText(userLocalData->SSLCA.value);

    /* MySQL 5.7.38, похоже, не дружит с CAPATH, поэтому выключено (хотя я сильно не вникал)
     * если указать SSL_CAPATH вместо SSL_CA, то драйвер ругается:
     *      SSL connection error: error:00000001:lib(0):func(0):reason(1)
     */
//    ui->lineEditSSLCAPath->setText(userLocalData->SSLCAPath.value);
    ui->labelSSLCAPath->setEnabled(false);
    ui->lineEditSSLCAPath->setEnabled(false);
    ui->toolButtonSSLCAPath->setEnabled(false);

    ui->lineEditSSLCipher->setText(userLocalData->SSLCipher.value);
}

SSLOptionsDialog::~SSLOptionsDialog()
{
    delete ui;
//    emit close();
}

void SSLOptionsDialog::browse(int buttonIndex)
{
    QString file;

    if(buttonIndex < 3)
    {
        QFileDialog fileDlg(this, tr("Select Key file"), directory);
        file = fileDlg.getOpenFileName();
        directory = QFileInfo(file).absolutePath();
    }
    switch (buttonIndex)
    {
    case 0: ui->lineEditSSLKey->setText(file); break;
    case 1: ui->lineEditSSLCert->setText(file); break;
    case 2: ui->lineEditSSLCA->setText(file); break;
    case 3: {directory = QFileDialog::getExistingDirectory(this, tr("Select CA Path"), directory); ui->lineEditSSLCAPath->setText(directory); break;}
    }
}

void SSLOptionsDialog::cancel()
{
    emit close();
}

void SSLOptionsDialog::ok()
{
    userLocalData->SSLKey.value = ui->lineEditSSLKey->text();
    userLocalData->SSLCert.value = ui->lineEditSSLCert->text();
    userLocalData->SSLCA.value = ui->lineEditSSLCA->text();
    userLocalData->SSLCAPath.value = ui->lineEditSSLCAPath->text();
    userLocalData->SSLCipher.value = ui->lineEditSSLCipher->text();
    emit close();
}

