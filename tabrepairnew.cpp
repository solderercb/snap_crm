#include "tabrepairnew.h"
#include "ui_tabrepairnew.h"

tabRepairNew* tabRepairNew::p_instance = nullptr;

tabRepairNew::tabRepairNew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabRepairNew)
{
    ui->setupUi(this);
    this->setWindowTitle("Приём в ремонт");
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->groupBoxDeviceCoincidence->hide();  // по умолчанию группу "Совпадение уст-ва" не показываем
    ui->groupBoxClientCoincidence->hide();  // по умолчанию группу "Совпадение клиента" не показываем
    ui->labelPrevRepairFromOldDB->hide();   // по умолчанию поля "Предыдущий ремонт" не показываем
    ui->lineEditPrevRepairFromOldDB->hide();
    ui->lineEditPrevRepair->hide();

    ui->lineEditEstPrice->setDisabled(true);
    ui->lineEditPrepaySumm->setDisabled(true);
    ui->comboBoxPrepayAccount->setDisabled(true);
    ui->pushButtonCashReceipt->setDisabled(true);
    ui->comboBoxProblem->lineEdit()->setPlaceholderText("неисправность");
    ui->comboBoxIncomingSet->lineEdit()->setPlaceholderText("комплектность");
    ui->comboBoxExterior->lineEdit()->setPlaceholderText("внешний вид");
    ui->comboBoxClientAd->lineEdit()->setPlaceholderText("источник обращения");
    ui->comboBoxDevice->lineEdit()->setPlaceholderText("устройство");
    ui->comboBoxDeviceMaker->lineEdit()->setPlaceholderText("производитель");
    ui->comboBoxDeviceModel->lineEdit()->setPlaceholderText("модель");
    ui->comboBoxPresetEngineer->lineEdit()->setPlaceholderText("назначить инженером");
    ui->comboBoxPresetBox->lineEdit()->setPlaceholderText("ячейка");
    ui->comboBoxOffice->lineEdit()->setPlaceholderText("офис");
    ui->comboBoxCompany->lineEdit()->setPlaceholderText("организация");
    ui->comboBoxPresetPaymentAccount->lineEdit()->setPlaceholderText("тип оплаты");
    ui->comboBoxPrepayAccount->lineEdit()->setPlaceholderText("тип оплаты");
    ui->lineEditPrevRepair->setButtons("Search, DownArrow");
    ui->lineEditPrevRepair->setReadOnly(true);
    ui->lineEditPrevRepair->setText("23812");

    ui->comboBoxPresetEngineer->addItem("aaaa");
    ui->comboBoxPresetEngineer->addItem("bbbb");
    if(QLineEdit *le = ui->comboBoxPresetEngineer->lineEdit())    // Рисуем кнопку очистки в комбобоксе (это работает)
    {
        le->setClearButtonEnabled(true);
    }

    comboboxDevicesModel = new QSqlQueryModel();
    ui->comboBoxDevice->setModel(comboboxDevicesModel);
    comboboxDeviceMakersModel = new QSqlQueryModel();
    ui->comboBoxDeviceMaker->setModel(comboboxDeviceMakersModel);
    comboboxDeviceModelsModel = new QSqlQueryModel();
    ui->comboBoxDeviceModel->setModel(comboboxDeviceModelsModel);

    QString query;

    query = QString("SELECT '' AS 'name', '' AS 'id', '' AS 'company_list' UNION ALL (SELECT `name`, `id`, `company_list` FROM `devices` WHERE `enable` = 1 AND `refill` = 0 ORDER BY `position`);");
    comboboxDevicesModel->setQuery(query, QSqlDatabase::database("connMain"));
}

tabRepairNew::~tabRepairNew()
{
    delete comboboxDeviceMakersModel;
    delete comboboxDeviceModelsModel;
    delete comboboxDevicesModel;
    delete ui;
    p_instance = nullptr;   // Обязательно блять!
}

tabRepairNew* tabRepairNew::getInstance(QWidget *parent)   // singleton: вкладка приёма в ремонт может быть только одна
{
if( !p_instance )
  p_instance = new tabRepairNew(parent);
return p_instance;
}

void tabRepairNew::getDevices()
{
//    if (!DBConnectionOK)
//        return;

}

void tabRepairNew::changeClientType()
{
    int index, row, col, span_row, span_col;

    if(ui->checkBoxClientType->checkState())
    {
        ui->groupBoxClient->setTitle("Клиент (юридическое лицо)");
        ui->lineEditClientFirstName->hide();    // скрываем поле "Имя"
        ui->lineEditClientPatronymic->hide();    // скрываем поле "Отчество"
        ui->lineEditClientLastName->setPlaceholderText("Название организации");
        index = ui->gridLayoutClient->indexOf(ui->lineEditClientLastName);  // получаем индекс поля "Фамилия" в объекте компоновки
        ui->gridLayoutClient->getItemPosition(index, &row, &col, &span_row, &span_col); // получаем координаты поля
        ui->gridLayoutClient->addWidget(ui->lineEditClientLastName, row, col, span_row, 6); // заменяем поле "Фамилия" самим собой, но растягиваем его по ширине на 6 столбцов (занимаем столбцы полей "Имя" и "Отчество")
    }
    else
    {
        ui->groupBoxClient->setTitle("Клиент (частное лицо)");
        index = ui->gridLayoutClient->indexOf(ui->lineEditClientLastName);  // получаем индекс поля "Фамилия" в объекте компоновки
        ui->gridLayoutClient->getItemPosition(index, &row, &col, &span_row, &span_col); // получаем координаты поля
        ui->gridLayoutClient->addWidget(ui->lineEditClientLastName, row, col, span_row, 2); // заменяем поле "Фамилия" самим собой, но сжимаем его по ширине до двух столбцов
        ui->lineEditClientLastName->setPlaceholderText("Фамилия");
        ui->lineEditClientFirstName->show();    // показываем поле "Имя"
        ui->lineEditClientPatronymic->show();    // показываем поле "Отчество"
    }
}

void tabRepairNew::showLineEditPrevRepair()
{
    if(ui->checkBoxWasEarlier->checkState() || ui->checkBoxIsWarranty->checkState())
    {
        ui->lineEditPrevRepair->show();
    }
    else
    {
        ui->lineEditPrevRepair->hide();
    }

}

void tabRepairNew::changeDeviceType()
{

    int comboBoxDeviceIndex = ui->comboBoxDevice->currentIndex();
    QString query;

    query = QString("SELECT `name`, `id` FROM `device_makers` WHERE `id` IN (%1);").arg(comboboxDevicesModel->index(comboBoxDeviceIndex, 2).data().toString());
    comboboxDeviceMakersModel->setQuery(query, QSqlDatabase::database("connMain"));
}

void tabRepairNew::changeDeviceMaker()
{
    int comboBoxDeviceIndex = ui->comboBoxDevice->currentIndex();
    int comboBoxDeviceMakerIndex = ui->comboBoxDeviceMaker->currentIndex();
    int deviceId = comboboxDevicesModel->index(comboBoxDeviceIndex, 1).data().toInt();
    int deviceMakerId = comboboxDeviceMakersModel->index(comboBoxDeviceMakerIndex, 1).data().toInt();
    QString query;

    query = QString("SELECT '' AS 'name', '' AS 'id' UNION ALL (SELECT `name`, `id` FROM `device_models` WHERE `device` = %1 AND `maker` = %2);").arg(deviceId).arg(deviceMakerId);
    comboboxDeviceModelsModel->setQuery(query, QSqlDatabase::database("connMain"));
}

void tabRepairNew::clearClientCreds()
{
    ui->lineEditClientLastName->clear();
    ui->lineEditClientFirstName->clear();
    ui->lineEditClientPatronymic->clear();
    ui->lineEditClientPhone1->clear();
    ui->comboBoxClientAd->setCurrentIndex(0); // Выбираем первый эл-т в списке
    ui->lineEditClientAddress->clear();
    ui->lineEditClientEmail->clear();
    ui->lineEditClientPhone2->clear();
}

void tabRepairNew::lineEditPrevRepairButtonsHandler(int button)
{
    if (button == 0)
        emit createTabSelectPrevRepair(1);
    else if (button ==1)
    {
        if (!ui->lineEditPrevRepairFromOldDB->isVisible())
        {
            ui->lineEditPrevRepairFromOldDB->show();
            ui->labelPrevRepairFromOldDB->show();
        }
        else
        {
            ui->lineEditPrevRepairFromOldDB->hide();
            ui->labelPrevRepairFromOldDB->hide();
        }
    }
}

void tabRepairNew::setPrevRepair(int repairNum)
{
    ui->lineEditPrevRepair->setText(QString::number(repairNum));
}

void tabRepairNew::fillClientCreds(int)
{

}

void tabRepairNew::buttonSelectExistingClientHandler()
{
    emit createTabSelectExistingClient(1);
}
