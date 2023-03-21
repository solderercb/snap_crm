#include "widgets/spageclientsummary.h"
#include "ui_spageclientsummary.h"

SPageClientSummary::SPageClientSummary(QWidget *parent) :
    STabPage(parent),
    ui(new Ui::SPageClientSummary)
{
    ui->setupUi(this);
}

SPageClientSummary::SPageClientSummary(int id, QWidget *parent) :
    SPageClientSummary(parent)
{
    m_client = new SClientModel();
    m_client->load(id);
    ui->phones->setEditable(SPhone::NotEditable);
    ui->phones->setModel(m_client->phones());
    ui->labelFullName->setText(m_client->fullLongName());
    ui->lineEditId->setText(QString::number(id));
    ui->lineEditCreated->setText(m_client->created());

    if(m_client->balanceEnabled())
    {
        ui->lineEditBalance->setText(sysLocale.toCurrencyString(m_client->balance()));
    }
    else
    {
        ui->formLayoutUnderPhoto->takeRow(ui->labelBalance);
        ui->lineEditBalance->hide();
        ui->labelBalance->hide();
    }
    ui->lineEditRepairs->setText(QString::number(m_client->repairs()));
    ui->lineEditPurchases->setText(QString::number(m_client->purchases()));
    ui->lineEditPriceColumn->setText(priceColModel->getDisplayRole(m_client->priceColumn()));
    ui->lineEditVisitSource->setText(clientAdTypesList->getDisplayRole(m_client->adType()));
    if(!m_client->address().isEmpty())
        ui->lineEditAddress->setText(QString("%1, %2").arg(m_client->postIndex()).arg(m_client->address()));
    else
    {
        ui->formLayoutRightToPhotoLong->takeRow(ui->labelAddress);
        ui->labelAddress->hide();
        ui->lineEditAddress->hide();
    }
    ui->lineEditNotes->setText(m_client->notes());
    if(!m_client->email().isEmpty())
        ui->lineEditEmail->setText(m_client->email());
    else
    {
        ui->formLayoutRightToPhotoShort->takeRow(ui->labelEmail);
        ui->labelEmail->hide();
        ui->lineEditEmail->hide();
    }
    ui->lineEditPassword->setText(m_client->web_password());
    fillBinaryProperties();
//    if(!permissions->value("X"))    // TODO: разрешение видеть реквизиты налогоплательщика и банковские
//    {
//        ui->groupBoxBanks->hide();
//    }
//    else
        fillBanks();

//    ui->lineEdi->setText();

    setCursorPositionsToZero();
}

SPageClientSummary::~SPageClientSummary()
{
    delete m_client;
    delete ui;
}

SClientModel *SPageClientSummary::model()
{
    return m_client;
}

void SPageClientSummary::fillBanks()
{
    if(!m_client->INN().isEmpty())
        ui->lineEditTaxField1->setText(m_client->INN());
    else
    {
        ui->formLayoutRightToPhotoShort->takeRow(ui->lineEditTaxField1);
        ui->labelTaxField1->hide();
        ui->lineEditTaxField1->hide();
    }
    if(!m_client->OGRN().isEmpty())
        ui->lineEditTaxField2->setText(m_client->OGRN());
    else
    {
        ui->formLayoutRightToPhotoShort->takeRow(ui->lineEditTaxField2);
        ui->labelTaxField2->hide();
        ui->lineEditTaxField2->hide();
    }
    if(!m_client->KPP().isEmpty())
        ui->lineEditTaxField3->setText(m_client->KPP());
    else
    {
        ui->formLayoutRightToPhotoShort->takeRow(ui->lineEditTaxField3);
        ui->labelTaxField3->hide();
        ui->lineEditTaxField3->hide();
    }

}

void SPageClientSummary::fillBinaryProperties()
{
    int properties = m_client->options();
    for(int i = 0; i < clientBinaryProperties->rowCount(); i++)
    {
        if(clientBinaryProperties->index(i, 1).data().toInt() == SClientModel::BalanceEnabled)  // Флаг о включенном балансе отображать не нужно
            continue;
        if(properties&(1<<i))
            ui->listInfo->addItem(clientBinaryProperties->index(i, 0).data().toString());
    }
}

//void SPageClientSummary::setCursorPositionsToZero()
//{
//    QLineEdit *le;
//    QList<QLineEdit*> list = this->findChildren<QLineEdit*>(QRegularExpression(".*"));
//    foreach(le, list)
//        le->setCursorPosition(0);
//}
