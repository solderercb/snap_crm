#include "tabrepairnew.h"
#include "ui_tabrepairnew.h"
#include "bottoolbarwidget.h"

tabRepairNew::tabRepairNew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabRepairNew)
{
    enum {store_cats_id, store_cats_parent, store_cats_name, store_cats_position};

    ui->setupUi(this);
//    BotToolbarWidget* botToolbarWidget_ = new BotToolbarWidget(ui->botToolbar);
//    ui->gridLayout->addWidget(botToolbarWidget_, 30, 0, 1, 5, Qt::AlignBottom);
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

    QSqlQuery* device_list = new QSqlQuery(QSqlDatabase::database("connMain"));
    QString query;
    QStandardItem *newRow;

    query = QString("SELECT `name`, `id` FROM `devices` WHERE `enable` = 1 AND `refill` = 0 ORDER BY `position`;");
    device_list->exec(query);

//    newRow = new QStandardItem();
//    newRow->setText("All");
//    newRow->setData("0",Qt::UserRole+1);
//    comboboxDevicesModel->appendRow(newRow);	// Добавляем строку в модель comboBox (склад-источник)

    while(device_list->next())
    {
        newRow = new QStandardItem();
        newRow->setText(device_list->value(0).toString());
        newRow->setData(device_list->value(1).toString(),Qt::UserRole+1);
        comboboxDevicesModel->appendRow(newRow);	// Добавляем строку в модель comboBox (устройство)
    }

//    ui->comboBoxDevice->setCurrentIndex(0); // По умолчанию выбираем первое уст-во в списке

    delete device_list;
}

tabRepairNew::~tabRepairNew()
{
    delete ui;
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
