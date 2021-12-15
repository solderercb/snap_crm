#include "tabrepairnew.h"
#include "ui_tabrepairnew.h"
#include "com_sql_queries.h"

tabRepairNew* tabRepairNew::p_instance = nullptr;

tabRepairNew::tabRepairNew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabRepairNew)
{
    QString query;

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
    ui->comboBoxClientAdType->lineEdit()->setPlaceholderText("источник обращения");
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

    clientAdTypesList = new QSqlQueryModel(this);
    clientAdTypesList->setQuery(QUERY_CLIENT_AD_TYPES, QSqlDatabase::database("connMain"));
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setModelColumn(0);

    clientPhoneTypesList = new QStandardItemModel(this);
    clientPhoneTypesSelector[0] << new QStandardItem("мобильный") << new QStandardItem("1");
    clientPhoneTypesList->appendRow( clientPhoneTypesSelector[0] );
    clientPhoneTypesSelector[1] << new QStandardItem("городской") << new QStandardItem("2");
    clientPhoneTypesList->appendRow( clientPhoneTypesSelector[1] );
    ui->comboBoxClientPhone1Type->setModel(clientPhoneTypesList);
    ui->comboBoxClientPhone1Type->setModelColumn(0);
    ui->comboBoxClientPhone1Type->setCurrentIndex(clientPhoneTypesList->index(0, 0).row());    // по умолчанию выбираем "мобильный"
    ui->comboBoxClientPhone2Type->setModel(clientPhoneTypesList);
    ui->comboBoxClientPhone2Type->setModelColumn(0);
    ui->comboBoxClientPhone2Type->setCurrentIndex(clientPhoneTypesList->index(0, 0).row());    // по умолчанию выбираем "мобильный"

    clientsMatchTable = new QSqlQueryModel();       // таблица совпадения клиента (по номеру тел. или по фамилии)

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
    ui->comboBoxClientAdType->setCurrentIndex(0); // Выбираем первый эл-т в списке
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

void tabRepairNew::fillClientCreds(int id)
{
    clientModel = new QSqlQueryModel();
    clientPhonesModel = new QSqlQueryModel();

    QString query;
    query = QString("SELECT  `id`,  `name`,  `surname`,  `patronymic`,  `agent_phone_mask`,  `agent2_phone_mask`,  `address`,  `post_index`,  `type`,  `memorial`,  `notes`,  `prefer_cashless`,  `visit_source`,  `ur_name`,  `email`,  `balance`,  `price_col`,  `repairs`,  `purchases` FROM `clients` WHERE `id` = %1").arg(id);
    clientModel->setQuery(query, QSqlDatabase::database("connMain"));

    exist_client_id = id;
    ui->lineEditClientFirstName->setText(clientModel->index(0, 1).data().toString());
    ui->lineEditClientLastName->setText(clientModel->index(0, 2).data().toString());
    ui->lineEditClientPatronymic->setText(clientModel->index(0, 3).data().toString());
    ui->lineEditClientAddress->setText(clientModel->index(0, 6).data().toString());
    ui->lineEditClientEmail->setText(clientModel->index(0, 14).data().toString());
    ui->lineEditClientEmail->setText(clientModel->index(0, 14).data().toString());

    query = QString(QUERY_CLIENT_PHONES(id));
    clientPhonesModel->setQuery(query, QSqlDatabase::database("connMain"));

    // заполняем типы телефонов. Пока так, потом придумаю что-то более элегантное
    if(clientPhonesModel->rowCount() > 1)
    {
        ui->lineEditClientPhone2->setText(clientPhonesModel->index(1, 0).data().toString());
        ui->comboBoxClientPhone2Type->setCurrentIndex(clientPhoneTypesList->index((clientPhonesModel->index(1, 1).data().toInt() - 1), 0).row());    // по умолчанию выбираем "мобильный"
    }
    if(clientPhonesModel->rowCount() > 0)
    {
        ui->lineEditClientPhone1->setText(clientPhonesModel->index(0, 0).data().toString());
        ui->comboBoxClientPhone1Type->setCurrentIndex(clientPhoneTypesList->index((clientPhonesModel->index(0, 1).data().toInt() - 1), 0).row());    // по умолчанию выбираем "мобильный"
    }

}

void tabRepairNew::buttonSelectExistingClientHandler()
{
    emit createTabSelectExistingClient(1);
}

void tabRepairNew::findMatchingCllient()
{
    if(ui->lineEditClientLastName->text().length() > 3 || ui->lineEditClientPhone1->text().length() > 3)
    {
        QString query;
        ui->tableViewClientMatch->setModel(clientsMatchTable);
        query = "SELECT t1.`id`, CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO', t1.`balance`, t1.`repairs`, t1.`purchases`, IF(t1.`type` = 1, 'Ю', '') AS 'type', GROUP_CONCAT(IFNULL(t2.`phone`, '') ORDER BY t2.`type` DESC, t2.`id` DESC SEPARATOR '\r\n')  AS 'phone' FROM `clients` AS t1 LEFT JOIN `tel` AS t2 ON t1.`id` = t2.`customer` GROUP BY t1.`id`";
        clientsMatchTable->setQuery(query, QSqlDatabase::database("connMain"));
        if (clientsMatchTable->rowCount() > 0)
            ui->groupBoxClientCoincidence->show();
        else
            ui->groupBoxClientCoincidence->hide();
    }
    else
    {
        clientsMatchTable->clear();
        ui->groupBoxClientCoincidence->hide();
    }
}
