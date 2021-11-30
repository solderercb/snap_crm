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

    ui->comboBoxPresetEngineer->addItem("aaaa");
    ui->comboBoxPresetEngineer->addItem("bbbb");
    if(QLineEdit *le = ui->comboBoxPresetEngineer->lineEdit())    // Рисуем кнопку очистки в комбобоксе (это работает)
    {
        le->setClearButtonEnabled(true);
    }

    comboboxDevicesModel = new QStandardItemModel();
    ui->comboBoxDevice->setModel(comboboxDevicesModel);
    comboboxDeviceMakersModel = new QStandardItemModel();
    ui->comboBoxDeviceMaker->setModel(comboboxDeviceMakersModel);
    comboboxDeviceModelsModel = new QStandardItemModel();
    ui->comboBoxDeviceModel->setModel(comboboxDeviceModelsModel);

    QSqlQuery* device_list = new QSqlQuery(QSqlDatabase::database("connMain"));
    QString query;
    QStandardItem *newRow;

    query = QString("SELECT `name`, `id` FROM `devices` WHERE `enable` = 1 AND `refill` = 0 ORDER BY `position`;");
    device_list->exec(query);

//    newRow = new QStandardItem();
//    newRow->setText("");
//    newRow->setData("0",Qt::UserRole+1);
//    comboboxDevicesModel->appendRow(newRow);	// Добавляем пустую строку в модель comboBox (тип уст-ва)

    while(device_list->next())
    {
        newRow = new QStandardItem();
        newRow->setText(device_list->value(0).toString());
        newRow->setData(device_list->value(1).toString(),Qt::UserRole+1);
        comboboxDevicesModel->appendRow(newRow);	// Добавляем строку в модель comboBox (тип уст-ва)
    }

    delete device_list;
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
        ui->lineEditClientLastName->setText("Название организации");
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
        ui->lineEditClientLastName->setText("Фамилия");
        ui->lineEditClientFirstName->show();    // показываем поле "Имя"
        ui->lineEditClientPatronymic->show();    // показываем поле "Отчество"
    }
}

void tabRepairNew::enableLineEditPrevRepair()
{
    if(ui->checkBoxWasEarlier->checkState() || ui->checkBoxIsWarranty->checkState())
    {
        ui->lineEditPrevRepair->setEnabled(true);
    }
    else
    {
        ui->lineEditPrevRepair->setDisabled(true);
    }

}

void tabRepairNew::changeDeviceType()
{
    comboboxDeviceMakersModel->clear();
    int deviceId = ui->comboBoxDevice->currentData(Qt::UserRole+1).toInt();
    QSqlQuery* query_result = new QSqlQuery(QSqlDatabase::database("connMain"));
    QString query, device_makers_list;
    QStandardItem *newRow;

    query = QString("SELECT `company_list` FROM `devices` WHERE `id` = %1;").arg(deviceId);
    query_result->exec(query);
    query_result->first();
    device_makers_list = query_result->value(0).toString();
    query = QString("SELECT `name`, `id` FROM `device_makers` WHERE `id` IN (%1);").arg(device_makers_list);
    query_result->exec(query);

    while(query_result->next())
    {
        newRow = new QStandardItem();
        newRow->setText(query_result->value(0).toString());
        newRow->setData(query_result->value(1).toString(),Qt::UserRole+1);
        comboboxDeviceMakersModel->appendRow(newRow);	// Добавляем строку в модель comboBox (устройство)
    }

    delete query_result;
}

void tabRepairNew::changeDeviceMaker()
{

    comboboxDeviceModelsModel->clear();

    int deviceId = ui->comboBoxDevice->currentData(Qt::UserRole+1).toInt();
    int deviceMakerId = ui->comboBoxDeviceMaker->currentData(Qt::UserRole+1).toInt();
    QSqlQuery* query_result = new QSqlQuery(QSqlDatabase::database("connMain"));
    QString query;
    QStandardItem *newRow;

    newRow = new QStandardItem();
    newRow->setText("");
    comboboxDeviceModelsModel->appendRow(newRow);	// Добавляем пустую строку в модель comboBox (модель устройства)

    query = QString("SELECT `name`, `id` FROM `device_models` WHERE `device` = %1 AND `maker` = %2;").arg(deviceId).arg(deviceMakerId);
    query_result->exec(query);

    while(query_result->next())
    {
        newRow = new QStandardItem();
        newRow->setText(query_result->value(0).toString());
        newRow->setData(query_result->value(1).toString(),Qt::UserRole+1);
        comboboxDeviceModelsModel->appendRow(newRow);	// Добавляем строку в модель comboBox (устройство)
    }

    delete query_result;
}
