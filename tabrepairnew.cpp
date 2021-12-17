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
    ui->lineEditSN->setButtons("Clear");

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
    comboboxProblemModel = new QSqlQueryModel();
    ui->comboBoxProblem->setModel(comboboxProblemModel);

    clientAdTypesList = new QSqlQueryModel(this);
    clientAdTypesList->setQuery(QUERY_CLIENT_AD_TYPES, QSqlDatabase::database("connMain"));
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setModelColumn(0);

    clientPhoneTypesList = new QStandardItemModel(this);
    clientPhoneTypesSelector[0] << new QStandardItem("мобильный") << new QStandardItem("1") << new QStandardItem("(+38) 999 999-99-99"); // в ASC формат задаётся нулями, но в поиске совпадающих клиентов  это предусмотрено
    clientPhoneTypesList->appendRow( clientPhoneTypesSelector[0] );
    clientPhoneTypesSelector[1] << new QStandardItem("городской") << new QStandardItem("2") << new QStandardItem("99-99-99");
    clientPhoneTypesList->appendRow( clientPhoneTypesSelector[1] );
    ui->comboBoxClientPhone1Type->setModel(clientPhoneTypesList);
    ui->comboBoxClientPhone1Type->setModelColumn(0);
    ui->comboBoxClientPhone1Type->setCurrentIndex(clientPhoneTypesList->index(0, 0).row());    // устанавливаем первый элемент выпадающего списка
    ui->comboBoxClientPhone2Type->setModel(clientPhoneTypesList);
    ui->comboBoxClientPhone2Type->setModelColumn(0);
    ui->comboBoxClientPhone2Type->setCurrentIndex(clientPhoneTypesList->index(0, 0).row());    // устанавливаем первый элемент выпадающего списка

    clientsMatchTable = new QSqlQueryModel();       // таблица совпадения клиента (по номеру тел. или по фамилии)
    devicesMatchTable = new QSqlQueryModel();       // таблица совпадения устройства (по серийному номеру)

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

    // Заполнение модели выпадающего списка неисправностей
    query = QString("\
        SELECT '' AS 'name'\
        UNION ALL\
        (SELECT\
          TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`fault_list`,',',enumerator.`number`),',',1)) AS 'name'\
        FROM (SELECT `fault_list` FROM `devices` WHERE `id` = %1) AS t1\
        JOIN enumerator\
        ON\
          (LENGTH(REPLACE(t1.`fault_list`, ',', ''))-LENGTH(t1.`fault_list`) <= enumerator.`number`+1));\
    ").arg(comboboxDevicesModel->index(comboBoxDeviceIndex, 1).data().toInt());
    comboboxProblemModel->setQuery(query, QSqlDatabase::database("connMain"));
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
    ui->comboBoxClientPhone1Type->setCurrentIndex(clientPhoneTypesList->index(0, 0).row());     // устанавливаем первый элемент выпадающего списка
    ui->comboBoxClientAdType->setCurrentIndex(0); // Выбираем первый эл-т в списке
    ui->lineEditClientAddress->clear();
    ui->lineEditClientEmail->clear();
    ui->lineEditClientPhone2->clear();
    ui->comboBoxClientPhone2Type->setCurrentIndex(clientPhoneTypesList->index(0, 0).row());     // устанавливаем первый элемент выпадающего списка
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

    clearClientCreds();

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
    if(clientPhonesModel->rowCount() > 1)   // если результат содержит более одной строки, то в поле доп. номера записываем данные из второй строки результатов
    {
        ui->comboBoxClientPhone2Type->setCurrentIndex(clientPhoneTypesList->index((clientPhonesModel->index(1, 1).data().toInt() - 1), 0).row());    // сначала устанавливаем тип в комбобоксе, чтобы применилась маска к полю
        ui->lineEditClientPhone2->setText(clientPhonesModel->index(1, 0).data().toString());    // теперь устанавливаем номер телефона
    }
    if(clientPhonesModel->rowCount() > 0)
    {
        ui->comboBoxClientPhone1Type->setCurrentIndex(clientPhoneTypesList->index((clientPhonesModel->index(0, 1).data().toInt() - 1), 0).row());    //
        ui->lineEditClientPhone1->setText(clientPhonesModel->index(0, 0).data().toString());
    }

    ui->comboBoxProblem->setFocus();    // устанавливаем фокус на полее ввода неисправности
}

void tabRepairNew::fillDeviceCreds(QModelIndex index)
{
    int i;
    int repair, device, deviceMaker, deviceModel, client;

    // пример запроса:
    // SELECT   t1.`id`,   CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',   `fault`,   `serial_number`,   CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client',   t1.`type`,   t1.`maker`,   t1.`model`,   t1.`client` AS 'client_id'   FROM `workshop` AS t1 LEFT JOIN `devices` AS t2 ON t1.`type` = t2.`id` LEFT JOIN `device_makers` AS t3 ON t1.maker = t3.`id` LEFT JOIN `device_models` AS t4 ON t1.model = t4.`id` LEFT JOIN `clients` AS t5 ON t1.`client` = t5.`id` WHERE `serial_number` REGEXP '.*' ORDER BY `id` DESC;
    repair = devicesMatchTable->index(index.row(), 0).data().toInt();
    device = devicesMatchTable->index(index.row(), 5).data().toInt();
    deviceMaker = devicesMatchTable->index(index.row(), 6).data().toInt();
    deviceModel = devicesMatchTable->index(index.row(), 7).data().toInt();
    client = devicesMatchTable->index(index.row(), 8).data().toInt();

    fillClientCreds(client);
    if (ui->comboBoxDevice->currentIndex() == 0)        // только если пользователь не выбрал тип уст-ва
    {
        for (i = 1; i < comboboxDevicesModel->rowCount(); i++)  // перебираем все типы уст-в в поисках нужного id (начинаем с 1, т. к. первый эл-т — пустышка)
        {
            if (comboboxDevicesModel->index(i, 1).data().toInt() == device)
                break;
        }
        ui->comboBoxDevice->setCurrentIndex(i);
        // TODO: сделать запросы к БД асинхронными. Готовые классы https://github.com/micjabbour/MSqlQuery
        // на данный момент по локальной сети всё работает быстро, поэтому не буду заморачиваться

        for (i = 0; i < comboboxDeviceMakersModel->rowCount(); i++)  // перебираем всех производителей уст-в в поисках нужного id (здесь начинаем с 0, т. к. элемент-пустышка принудительно не добавляется)
        {
            if (comboboxDeviceMakersModel->index(i, 1).data().toInt() == deviceMaker)
                break;
        }
        ui->comboBoxDeviceMaker->setCurrentIndex(i);
        for (i = 0; i < comboboxDeviceModelsModel->rowCount(); i++)  // перебираем все модели уст-в в поисках нужной id (здесь тоже начинаем с 0, т. к. элемент-пустышка принудительно не добавляется)
        {
            if (comboboxDeviceModelsModel->index(i, 1).data().toInt() == deviceModel)
                break;
        }
        ui->comboBoxDeviceModel->setCurrentIndex(i);
    }
    setPrevRepair(repair);  // устанавливаем номер предыдущего ремонта в соотв. поле
    ui->checkBoxWasEarlier->setCheckState(Qt::Checked);     // ставим галочку "Ранее было в ремонте" чтобы поле с номером ремонта отобразилось

    devicesMatchTable->clear(); // если кол-во введённых пользователем символов меньше трёх, то удаляем результаты предыдущего запроса и прячем таблицу.
    ui->groupBoxDeviceCoincidence->hide();
    ui->comboBoxProblem->setFocus();    // устанавливаем фокус на полее ввода неисправности
}

void tabRepairNew::buttonSelectExistingClientHandler()
{
    emit createTabSelectExistingClient(1);
}

void tabRepairNew::findMatchingClient(QString text)
{
    QString lineEditClientPhone1DisplayText = ui->lineEditClientPhone1->displayText();  // отображаемый текст
    QString currentPhone1InputMask = clientPhoneTypesList->index(ui->comboBoxClientPhone1Type->currentIndex(), 2).data().toString().replace(QRegularExpression("[09]"), "_");   // в маске телефона меняем 0 и 9 на подчеркивание; 0 и 9 — это специальные маскировочные символы (см. справку QLineEdit, inputMask)
    QString enteredByUserDigits;    // строка символов, которые ввёл пользователь (т. е. текст отображаемый в lineEdit над которым выполнена операция т. н. XOR с заданной маской
    QStringList query_where;    // список условий для запроса к БД
    QString query;  // весь запрос к БД
    int i;

    //    qDebug() << "currentPhone1InputMask " << currentPhone1InputMask << " lineEditClientPhone1DisplayText " << lineEditClientPhone1DisplayText;
    for (i = 0; i < currentPhone1InputMask.length(); i++ )  // определяем какие символы ввёл пользователь
    {
        if(currentPhone1InputMask.at(i) != lineEditClientPhone1DisplayText.at(i))
            enteredByUserDigits.append(lineEditClientPhone1DisplayText.at(i));
    }

//    qDebug() << "enteredByUserDigits " << enteredByUserDigits;
    if(ui->lineEditClientLastName->text().length() >= 3 || enteredByUserDigits.length() >= 3 )  // если пользователь ввёл более двух символов в одно из полей
    {
        query_where.clear();
        if (ui->lineEditClientLastName->text().length() > 3 )
            query_where << QString("LCASE(CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)) REGEXP LCASE('%1')").arg(ui->lineEditClientLastName->text());   // условие поиска по фамилии, имени и отчеству
        if (enteredByUserDigits.length() > 3 )
            query_where << QString("IFNULL(t2.`phone`, '') LIKE '%1' OR IFNULL(t2.`phone_clean`, '') REGEXP '%2'").arg(lineEditClientPhone1DisplayText, enteredByUserDigits);   // условие поиска по телефонному номеру

        ui->tableViewClientMatch->setModel(clientsMatchTable);  // указываем модель таблицы
        query = QString("SELECT t1.`id`, CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO', t1.`balance`, t1.`repairs`, t1.`purchases`, IF(t1.`type` = 1, 'Ю', '') AS 'type', GROUP_CONCAT(IFNULL(t2.`phone`, '') ORDER BY t2.`type` DESC, t2.`id` DESC SEPARATOR '\r\n')  AS 'phone' FROM `clients` AS t1 LEFT JOIN `tel` AS t2 ON t1.`id` = t2.`customer` WHERE `state` = 1 %1 GROUP BY t1.`id`;").arg((query_where.count()>0?"AND (" + query_where.join(" OR ") + ")":""));
        clientsMatchTable->setQuery(query, QSqlDatabase::database("connMain"));

        // изменяем размер столбцов под соедржимое.
        // TODO: нужно бы создать свой класс с наследованием QTableView, реализовать в нём пропорциональное изменение ширин столбцов
        // при изменении размера окна и тултип для длинного текста (несколько телефонов в одной ячейке). Этот класс использовать везде
        ui->tableViewClientMatch->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if (clientsMatchTable->rowCount() > 0)
            ui->groupBoxClientCoincidence->show();  // только если возвращены результаты, показываем таблицу совпадения клиента
        else
            ui->groupBoxClientCoincidence->hide();  // иначе прячем таблицу
    }
    else
    {
        clientsMatchTable->clear(); // если кол-во введённых пользователем символов меньше трёх, то удаляем результаты предыдущего запроса и прячем таблицу.
        ui->groupBoxClientCoincidence->hide();
    }
}

void tabRepairNew::findMatchingDevice(QString text)
{
//    qDebug() << "findMatchingDevice(QString text)";
    QString query;

    if( text.length() >= 3 )  // если пользователь ввёл более двух символов
    {
        ui->tableViewDeviceMatch->setModel(devicesMatchTable);  // указываем модель таблицы

        // TODO: сейчас regexp будет работать неправильно, т. к. производится преобразование регистра для расширения диапазона поиска
        // Возможно нужно доработать этот механизм, чтобы рег. выражения работали полноценно
        query = QString("\
            SELECT\
              t1.`id`,\
              CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\
              `fault`,\
              `serial_number`,\
              CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client',\
              t1.`type`,\
              t1.`maker`,\
              t1.`model`,\
              t1.`client` AS 'client_id'\
            FROM `workshop` AS t1 LEFT JOIN `devices` AS t2 ON t1.`type` = t2.`id` LEFT JOIN `device_makers` AS t3 ON t1.maker = t3.`id` LEFT JOIN `device_models` AS t4 ON t1.model = t4.`id` LEFT JOIN `clients` AS t5 ON t1.`client` = t5.`id` WHERE LCASE(`serial_number`) REGEXP LCASE('%1') ORDER BY `id` DESC;\
        ").arg(text);
        devicesMatchTable->setQuery(query, QSqlDatabase::database("connMain"));

        // прячем столбцы с кодами типа уст-ва, производителем, моделью и клиентом (они запрашиваются для автозаполнения полей при выборе совпадающего)
        ui->tableViewDeviceMatch->hideColumn(5);
        ui->tableViewDeviceMatch->hideColumn(6);
        ui->tableViewDeviceMatch->hideColumn(7);
        ui->tableViewDeviceMatch->hideColumn(8);

        // изменяем размер столбцов под соедржимое.
        // TODO: заменить QTableView на свой класс и реализовать в нём пропорциональное изменение ширин столбцов
        // при изменении размера окна и тултип для длинного текста (неисправность).
        ui->tableViewDeviceMatch->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        if (devicesMatchTable->rowCount() > 0)
            ui->groupBoxDeviceCoincidence->show();  // только если возвращены результаты, показываем таблицу совпадения устройства
        else
            ui->groupBoxDeviceCoincidence->hide();  // иначе прячем таблицу
    }
    else
    {
        devicesMatchTable->clear(); // если кол-во введённых пользователем символов меньше трёх, то удаляем результаты предыдущего запроса и прячем таблицу.
        ui->groupBoxDeviceCoincidence->hide();
    }
}

void tabRepairNew::phone1TypeChanged(int index)
{
    phoneTypeChanged(1, index);
}

void tabRepairNew::phone2TypeChanged(int index)
{
    phoneTypeChanged(2, index);
}

void tabRepairNew::clientMatchTableDoubleClicked(QModelIndex item)
{
    fillClientCreds(clientsMatchTable->index(item.row(), 0).data().toInt());
    ui->groupBoxClientCoincidence->hide();  // прячем таблицу совпадения клиента
}

void tabRepairNew::lineEditSNClearHandler(int)
{
    if(ui->lineEditSN->text().length())
        ui->lineEditSN->clear();
    else
        ui->lineEditSN->setText("НЕТ S/N");

    devicesMatchTable->clear(); // то удаляем результаты предыдущего запроса и прячем таблицу.
    ui->groupBoxDeviceCoincidence->hide();
}

void tabRepairNew::phoneTypeChanged(int type, int index)
{
    switch (type)
    {
        // т. к. полей для телефона всего два, то нет смысла городить какую-то универсальную конструкцию.
        case 1: ui->lineEditClientPhone1->setInputMask(""); ui->lineEditClientPhone1->setInputMask(clientPhoneTypesList->index(index, 2).data().toString() + ";_"); break;  // Here ";_" for filling blank characters with underscore
        case 2: ui->lineEditClientPhone2->setInputMask(""); ui->lineEditClientPhone2->setInputMask(clientPhoneTypesList->index(index, 2).data().toString() + ";_"); break;
    }
}
