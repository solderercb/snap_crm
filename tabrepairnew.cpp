#include "global.h"
#include "appver.h"
#include "tabrepairnew.h"
#include "ui_tabrepairnew.h"
#include "com_sql_queries.h"

tabRepairNew* tabRepairNew::p_instance = nullptr;

enum addlField {LineEdit = 1, ComboBox, DateEdit, dummy};

tabRepairNew::tabRepairNew(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabRepairNew)
{
    additionalFields = new SFieldsModel(SFieldsModel::Repair);
    repairModel = new SRepairModel(this);
    cashRegister = new SCashRegisterModel();
    comment = new SCommentModel();
    userActivityLog->appendRecord(tr("Navigation Приём в ремонт"));

    ui->setupUi(this);
    this->setWindowTitle("Приём в ремонт");
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillClientCreds(int)));
#ifdef QT_DEBUG
    main_window_test_scheduler = parent->test_scheduler;
    main_window_test_scheduler2 = parent->test_scheduler2;
#endif

    clientModel = new SClientModel();
    ui->phones->setModel(clientModel->phones());
    groupBoxEventFilter = new SGroupBoxEventFilter(this);
    ui->groupBoxDeviceCoincidence->installEventFilter(groupBoxEventFilter);
    ui->groupBoxDeviceCoincidence->hide();  // по умолчанию группу "Совпадение уст-ва" не показываем
    ui->labelPrevRepairFromOldDB->hide();   // по умолчанию поля "Предыдущий ремонт" не показываем
    ui->lineEditPrevRepairFromOldDB->hide();
    ui->lineEditPrevRepair->hide();
//    ui->pushButtonCashReceipt->hide();  // в АСЦ эта кнопка расположена в groupBox'е Финансы, но выполняет ту же функцию, что и кнопка Принять на панели задач внизу; почему так — загадка; прикручу к ней такой же функционал, и если что спрячу от пользователя
    ui->labelDoc->hide();       // хрен знает, что это за поле "Документ", оно не у всех пользователей отображается, а у кого отображается, не содержит ни одного эл-та. Спрячу от пользователя.
    ui->comboBoxDoc->hide();

    ui->pushButtonCreateTabClient->hide();  // кнопка открытия карточки клиента будет отображаться, только когда выбран клиент из базы
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    ui->comboBoxProblem->lineEdit()->setPlaceholderText(tr("неисправность"));
    ui->comboBoxIncomingSet->lineEdit()->setPlaceholderText(tr("комплектность"));
    ui->comboBoxExterior->lineEdit()->setPlaceholderText(tr("внешний вид"));
    ui->comboBoxClientAdType->lineEdit()->setPlaceholderText(tr("источник обращения"));
    ui->comboBoxDeviceClass->setPlaceholderText(tr("устройство"));
    ui->comboBoxDeviceVendor->setPlaceholderText(tr("производитель"));
    ui->comboBoxDevice->lineEdit()->setPlaceholderText(tr("модель"));
    ui->comboBoxPresetEngineer->setPlaceholderText(tr("назначить инженером"));
    ui->comboBoxPresetPlace->setPlaceholderText(tr("ячейка"));
    ui->comboBoxOffice->setPlaceholderText(tr("офис"));
    ui->comboBoxOffice->setEnabled(false);  // TODO: назначение этого эл-та в АСЦ остаётся загадкой; выбор офиса нужно реализовать на глобальном уровне, если пользователь обаладает таким правом (например, пункт в меню)
    ui->comboBoxCompany->setPlaceholderText(tr("организация"));
    ui->comboBoxPresetPaymentAccount->setPlaceholderText(tr("тип оплаты"));
//    ui->comboBoxPrepayAccount->setPlaceholderText(tr("тип оплаты"));
    ui->lineEditPrevRepair->setButtons("Search, DownArrow");
    ui->lineEditPrevRepair->setReadOnly(true);
    ui->lineEditSN->setButtons("Clear");

    setDefaultStyleSheets();

    if(QLineEdit *le = ui->comboBoxPresetEngineer->lineEdit())    // Рисуем кнопку очистки в комбобоксе (это работает)
    {
        le->setClearButtonEnabled(true);
    }

    ui->comboBoxCompany->setModel(companiesModel);
    ui->comboBoxOffice->setModel(officesModel);
    ui->comboBoxOffice->setCurrentIndex(officesModel->rowByDatabaseID(userDbData->value("current_office").toInt()));
    ui->comboBoxPresetEngineer->setModel(engineersModel);
    ui->comboBoxPresetEngineer->setCurrentIndex(-1);
    ui->comboBoxPresetPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxPresetPaymentAccount->setCurrentIndex(-1);
    ui->comboBoxPrepayAccount->setModel(paymentSystemsModel);
    ui->comboBoxPrepayAccount->setCurrentIndex(-1);
    ui->comboBoxPresetPlace->setModel(repairBoxesModel);
    ui->comboBoxPresetPlace->setCurrentIndex(-1);
    ui->spinBoxStickersCount->setValue(comSettings->value("rep_stickers_copy").toInt());

    deviceClassesModel = new SSqlQueryModel();
    ui->comboBoxDeviceClass->setModel(deviceClassesModel);
    deviceVendorsModel = new SSqlQueryModel();
    ui->comboBoxDeviceVendor->setModel(deviceVendorsModel);
    devicesModel = new SSqlQueryModel();
    ui->comboBoxDevice->setModel(devicesModel);
    classProblemsModel = new QSqlQueryModel();
    ui->comboBoxProblem->setModel(classProblemsModel);
    ui->comboBoxProblem->setCurrentIndex(-1);
    classIncomingSetsModel = new QSqlQueryModel();
    ui->comboBoxIncomingSet->setModel(classIncomingSetsModel);
    classExteriorsModel = new QSqlQueryModel();
    ui->comboBoxExterior->setModel(classExteriorsModel);

    connect(ui->lineEditClientLastName,SIGNAL(textEdited(QString)),ui->widgetClientMatch,SLOT(findByLastname(QString)));

    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setModelColumn(0);
    ui->comboBoxClientAdType->setCurrentIndex(-1);

    connect(ui->phones,SIGNAL(primaryPhoneEdited(QString)),this,SLOT(primaryPhoneEdited(QString)));

    QStringList prepayReasonsList = {tr("полная предоплата"), tr("за детали"), tr("за часть стоимости деталей"), tr("за часть стоимости работ"), tr("за диагностику")};
    prepayReasonsModel = new QStandardItemModel();
    for (int i=0; i<prepayReasonsList.size(); i++)
    {
        prepayReason = new QList<QStandardItem*>();
        *prepayReason << new QStandardItem(prepayReasonsList.at(i));
        prepayReason->value(0)->setData(i); // в UserRole+1 храним id типа предоплаты
        prepayReasonsModel->appendRow(*prepayReason);
    }
    ui->comboBoxPrepayReason->setModel(prepayReasonsModel);
    ui->comboBoxPrepayReason->setCurrentIndex(-1);

    clientsMatchTable = new QSqlQueryModel();       // таблица совпадения клиента (по номеру тел. или по фамилии)
    devicesMatchTable = new QSqlQueryModel();       // таблица совпадения устройства (по серийному номеру)

    deviceClassesModel->setQuery(QUERY_SEL_DEVICES, QSqlDatabase::database("connMain"));
    ui->comboBoxDeviceClass->setCurrentIndex(-1);
    msgBox.setText(tr("Ошибка"));
    msgBox.setIcon(QMessageBox::Critical);

#ifdef QT_DEBUG
    test_scheduler->start(200);
#endif
}

tabRepairNew::~tabRepairNew()
{
    userActivityLog->updateActivityTimestamp();

    delete ui;
    delete deviceVendorsModel;
    delete devicesModel;
    delete deviceClassesModel;
    delete classProblemsModel;
    delete classIncomingSetsModel;
    delete classExteriorsModel;
    delete clientsMatchTable;
    delete devicesMatchTable;
    delete groupBoxEventFilter;
    delete additionalFields;
    delete clientModel;
    delete cashRegister;
    delete repairModel;
    delete comment;
    p_instance = nullptr;   // Обязательно блять!
}

bool tabRepairNew::tabCloseRequest()
{
    // TODO: сделать проверку не сохранённых данных перед закрытием
    return 1;
}

QString tabRepairNew::tabTitle()
{

}

tabRepairNew* tabRepairNew::getInstance(MainWindow *parent)   // singleton: вкладка приёма в ремонт может быть только одна
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

    if(ui->checkBoxClientType->checkState())
    {
        ui->groupBoxClient->setTitle(tr("Клиент (юридическое лицо)"));
        // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
        ui->lineEditClientFirstName->setPlaceholderText(tr("Название организации"));
        ui->lineEditClientLastName->hide();    // скрываем поле "Фамилия"
        ui->lineEditClientPatronymic->hide();    // скрываем поле "Отчество"
        ui->gridLayoutClient->addWidget(ui->lineEditClientFirstName, 1, 0, 1, 3); // заменяем поле "Фамилия" самим собой, но растягиваем его по ширине на 6 столбцов (занимаем столбцы полей "Имя" и "Отчество")
    }
    else
    {
        ui->groupBoxClient->setTitle("Клиент (частное лицо)");
        ui->gridLayoutClient->addWidget(ui->lineEditClientLastName, 1, 0, 1, 1);
        ui->gridLayoutClient->addWidget(ui->lineEditClientFirstName, 1, 1, 1, 1); // заменяем поле "Фамилия" самим собой, но сжимаем его по ширине до двух столбцов
        ui->gridLayoutClient->addWidget(ui->lineEditClientPatronymic, 1, 2, 1, 1);
        // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
        ui->lineEditClientFirstName->setPlaceholderText(tr("Имя"));
        ui->lineEditClientLastName->show();    // показываем поле "Имя"
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
        ui->lineEditPrevRepairFromOldDB->hide();
    }

}

void tabRepairNew::enablePrepayWidgets(bool state)
{
    ui->comboBoxPrepayReason->setEnabled(state);
    ui->lineEditPrepaySumm->setEnabled(state);
    ui->comboBoxPrepayAccount->setEnabled(state);
    ui->pushButtonCashReceipt->setEnabled(state);
}

void tabRepairNew::changeDeviceClass(int index)
{
    if(index == -1)
    {
        additionalFields->clear();
        return;
    }

    int additionalFieldRow = 2, additionaFieldCol = 0;
    SFieldValueModel *additionalField;
    int classId = 0;
    QString query;
    classId = deviceClassesModel->databaseIDByRow(index);


    query = QUERY_SEL_DEVICE_MAKERS(deviceClassesModel->index(index, 2).data().toString());
    deviceVendorsModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxDeviceVendor->setCurrentIndex(-1);

    // Заполнение модели выпадающего списка неисправностей
    query = QUERY_SEL_DEVICE_FAULTS(classId);
    classProblemsModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxProblem->setCurrentIndex(-1);

    // Заполнение модели выпадающего списка комплектности
    query = QUERY_SEL_DEVICE_SET(classId);
    classIncomingSetsModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxIncomingSet->setCurrentIndex(-1);

    // Заполнение модели выпадающего списка внешнего вида
    query = QUERY_SEL_DEVICE_EXTERIOR(classId);
    classExteriorsModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxExterior->setCurrentIndex(-1);

    additionalFields->init(classId);
    foreach(additionalField, additionalFields->list())
    {
        ui->gridLayoutDeviceDescription->addWidget(additionalField->widget(), additionalFieldRow, additionaFieldCol, 1, 1);

        if (++additionaFieldCol > 5)
        {
            additionalFieldRow++;
            additionaFieldCol = 0;
        }
    }
}

void tabRepairNew::changeDeviceVendor(int index)
{
    int comboBoxDeviceIndex = ui->comboBoxDeviceClass->currentIndex();
    int deviceClassId = deviceClassesModel->databaseIDByRow(comboBoxDeviceIndex);
    int deviceVendorId = deviceVendorsModel->databaseIDByRow(index);
    QString query;

    query = QUERY_SEL_DEVICE_MODELS.arg(deviceClassId).arg(deviceVendorId);
    devicesModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxDevice->setCurrentIndex(-1);
}

void tabRepairNew::clearClientCreds(bool hideCoincidence)
{
    setDefaultStyleSheets();
    client = 0;
    ui->checkBoxClientType->setEnabled(true);
    ui->lineEditClientLastName->setReadOnly(false);  // разрешение редактирования
    ui->lineEditClientFirstName->setReadOnly(false);
    ui->lineEditClientPatronymic->setReadOnly(false);
    ui->comboBoxClientAdType->setEnabled(true);
    ui->lineEditClientAddress->setReadOnly(false);
    ui->lineEditClientEmail->setReadOnly(false);

    ui->pushButtonCreateTabClient->setEnabled(false);
    ui->lineEditClientLastName->clear();
    ui->lineEditClientFirstName->clear();
    ui->lineEditClientPatronymic->clear();
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    ui->lineEditClientAddress->clear();
    ui->lineEditClientEmail->clear();

    clientModel->clear();

    if (hideCoincidence)
        ui->widgetClientMatch->hide();
}

void tabRepairNew::lineEditPrevRepairButtonsHandler(int button)
{
    if (button == SLineEdit::Search)
        emit createTabSelectPrevRepair(1, this);
    else if (button == SLineEdit::DownArrow)
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

void tabRepairNew::fillClientCreds(int id)
{
    clearClientCreds(false);    // очищаем данные клиента, но не прячем таблицу совпадений
    client = id;

    clientModel->load(client);
    ui->checkBoxClientType->setChecked(clientModel->type());
    changeClientType();
    ui->checkBoxClientType->setEnabled(false);
    ui->lineEditClientLastName->setReadOnly(true);  // запрет на изменение, если клиент из базы
    ui->lineEditClientFirstName->setReadOnly(true);
    ui->lineEditClientPatronymic->setReadOnly(true);
    ui->comboBoxClientAdType->setEnabled(false);
    ui->lineEditClientAddress->setReadOnly(true);
    ui->lineEditClientEmail->setReadOnly(true);

    ui->pushButtonCreateTabClient->setEnabled(true);
    ui->lineEditClientFirstName->setText(clientModel->firstName());
    ui->lineEditClientLastName->setText(clientModel->lastName());
    ui->lineEditClientPatronymic->setText(clientModel->patronymicName());
    ui->phones->setModel(clientModel->phones());
    ui->lineEditClientAddress->setText(clientModel->address());
    ui->lineEditClientEmail->setText(clientModel->email());

    ui->comboBoxProblem->setFocus();    // устанавливаем фокус на полее ввода неисправности
}

void tabRepairNew::fillDeviceCreds(int id)
{
    int i;
    int repair, deviceClass, deviceVendor, device, client;
    queryDevice = new QSqlQueryModel();

    QString query;
    query = QUERY_SEL_DEVICE(id);
    queryDevice->setQuery(query, QSqlDatabase::database("connMain"));

    repair = queryDevice->record(0).value("id").toInt();
    deviceClass = queryDevice->record(0).value("type").toInt();
    deviceVendor = queryDevice->record(0).value("maker").toInt();
    device = queryDevice->record(0).value("model").toInt();
    client = queryDevice->record(0).value("client").toInt();

    fillClientCreds(client);
//    if (ui->comboBoxDeviceClass->currentIndex() < 0)        // только если пользователь не выбрал тип уст-ва
    {
        for (i = 0; i < deviceClassesModel->rowCount(); i++)  // перебираем все типы уст-в в поисках нужного id
        {
            if (deviceClassesModel->index(i, 1).data().toInt() == deviceClass)
                break;
        }
        ui->comboBoxDeviceClass->setCurrentIndex(i);
        // TODO: сделать запросы к БД асинхронными. Готовые классы https://github.com/micjabbour/MSqlQuery
        // на данный момент по локальной сети всё работает быстро, поэтому не буду заморачиваться

        for (i = 0; i < deviceVendorsModel->rowCount(); i++)  // перебираем всех производителей уст-в в поисках нужного id
        {
            if (deviceVendorsModel->index(i, 1).data().toInt() == deviceVendor)
                break;
        }
        ui->comboBoxDeviceVendor->setCurrentIndex(i);
        for (i = 0; i < devicesModel->rowCount(); i++)  // перебираем все модели уст-в в поисках нужной id
        {
            if (devicesModel->index(i, 1).data().toInt() == device)
                break;
        }
        ui->comboBoxDevice->setCurrentIndex(i);
        ui->lineEditSN->setText(queryDevice->record(0).value("serial_number").toString());
    }
    ui->lineEditPrevRepair->setText(QString::number(repair));  // устанавливаем номер предыдущего ремонта в соотв. поле
    ui->checkBoxWasEarlier->setCheckState(Qt::Checked);     // ставим галочку "Ранее было в ремонте" чтобы поле с номером ремонта отобразилось
    repairModel->setEarly(repair);
    repairModel->setIsRepeat(1);

    ui->comboBoxProblem->setFocus();    // устанавливаем фокус на полее ввода неисправности

    delete queryDevice;
}

void tabRepairNew::buttonSelectExistingClientHandler()
{
    emit createTabSelectExistingClient(1, this);
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
        query = QUERY_SEL_DEVICE_MATCH(text);
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
        {
            ui->groupBoxDeviceCoincidence->show();  // только если возвращены результаты, показываем таблицу совпадения устройства
            ui->tableViewDeviceMatch->show();       // может случиться ситуация, когда таблица будет скрыта, поэтому принудительно отображаем её
        }
        else
            ui->groupBoxDeviceCoincidence->hide();  // иначе прячем таблицу
    }
    else
    {
        devicesMatchTable->clear(); // если кол-во введённых пользователем символов меньше трёх, то удаляем результаты предыдущего запроса и прячем таблицу.
        ui->groupBoxDeviceCoincidence->hide();
    }
}

void tabRepairNew::deviceMatchTableDoubleClicked(QModelIndex item)
{
    fillDeviceCreds(devicesMatchTable->index(item.row(), 0).data().toInt());
    ui->tableViewDeviceMatch->hide();   // прячем таблицу, а не весь groupBox (вдруг пользователь промахнётся)
    ui->widgetClientMatch->hide();  // прячем таблицу совпадения клиента (если она по какой-то причине отображается)
}

void tabRepairNew::clientMatchTableDoubleClicked(QModelIndex item)
{
    fillClientCreds(clientsMatchTable->index(item.row(), 0).data().toInt());
//    ui->groupBoxClientCoincidence->hide();  // прячем таблицу совпадения клиента
    ui->widgetClientMatch->hide();   // прячем таблицу, а не весь groupBox (вдруг пользователь промахнётся)
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

void tabRepairNew::setDefaultStyleSheets()
{
    ui->comboBoxDeviceClass->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxDeviceVendor->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxDevice->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditSN->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientFirstName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditClientAddress->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientEmail->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxProblem->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxIncomingSet->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxExterior->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPresetEngineer->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPresetPlace->setStyleSheet(commonComboBoxStyleSheet);
//    ui->comboBoxOffice->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPresetPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPresetEngineer->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditEstPrice->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxPrepayReason->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditPrepaySumm->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxPrepayAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditPrevRepair->setStyleSheet(commonLineEditStyleSheet);
/*
    for(int i=0; i< additionalFieldsWidgets.size(); i++)   // установка стилей доп. полей
    {
        if ( additionalFieldsWidgets[i]->property("fieldRequired").toBool() )
        {
            if ( QString(additionalFieldsWidgets[i]->metaObject()->className()).compare("QComboBox", Qt::CaseSensitive) == 0 )
            {
                additionalFieldsWidgets[i]->setStyleSheet(commonComboBoxStyleSheet);
            }
            else if ( QString(additionalFieldsWidgets[i]->metaObject()->className()).compare("QLineEdit", Qt::CaseSensitive) == 0 )
                additionalFieldsWidgets[i]->setStyleSheet(commonLineEditStyleSheet);
            // Дату не окрашиваем, даже если она обязательна, т. к. не продуман механизм проверки
//            else if ( QString(additionalFieldsWidgets[i]->metaObject()->className()).compare("QDateEdit", Qt::CaseSensitive) == 0 )
//                    additionalFieldsWidgets[i]->setStyleSheet(commonDateEditStyleSheet);   // Дату не окрашиваем, даже если она обязательна, т. к. не продуман механизм проверки
        }
    }
*/
}

bool tabRepairNew::checkInput()
{
    int error = 0;
    setDefaultStyleSheets();

    if ( ui->comboBoxDeviceClass->currentIndex() < 0 )        // если не выбран тип уст-ва
    {
        ui->comboBoxDeviceClass->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 1;
    }
    if ( ui->comboBoxDeviceVendor->currentIndex() < 0 )        // если не выбран производителя
    {
        ui->comboBoxDeviceVendor->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 2;
    }
    if ( ui->comboBoxDevice->currentIndex() < 0 && ui->comboBoxDevice->currentText() == "" )        // если не выбрана или не написана модель
    {
        ui->comboBoxDevice->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 3;
    }
    if (ui->lineEditSN->text() == "" && comSettings->value("is_sn_req").toBool())   // если не записан серийный номер, а он обязателен
    {
        ui->lineEditSN->setStyleSheet(commonLineEditStyleSheetRed);
        error = 4;
    }
    if (ui->lineEditClientLastName->text() == "" && ui->checkBoxClientType->checkState() == 0)       // если не указана фамилия (только для физ. лиц)
    {
        ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheetRed);
        error = 5;
    }
    if (ui->lineEditClientFirstName->text() == "")     // если не указано имя (или название организации)
    {
        ui->lineEditClientFirstName->setStyleSheet(commonLineEditStyleSheetRed);
        error = 6;
    }
    if (ui->lineEditClientPatronymic->text() == "" && ui->checkBoxClientType->checkState() == 0 && comSettings->value("is_patronymic_required").toBool() && !client)   // если не указано отчество и оно обязятельно (только для физ. лиц и только для новых клиентов)
    {
        ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheetRed);
        error = 7;
    }
    if (ui->comboBoxClientAdType->currentIndex() < 0 && comSettings->value("visit_source_force").toBool() && !client)        // если не указан источник обращения, а он обязателен и клиент новый
    {
        ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 8;
    }
    if (ui->lineEditClientAddress->text() == "" && comSettings->value("address_required").toBool() && !client)   // если не указан адрес, а он обязателен и клиент новый
    {
        ui->lineEditClientAddress->setStyleSheet(commonLineEditStyleSheetRed);
        error = 9;
    }
    if (ui->lineEditClientEmail->text() == "" && comSettings->value("email_required").toBool() && !client)   // если не указан email, а он обязателен и клиент новый
    {
        ui->lineEditClientEmail->setStyleSheet(commonLineEditStyleSheetRed);
        error = 10;
    }
    if (!ui->phones->isValid())
        error = 11;
    if (ui->comboBoxProblem->text() == "")        // если не указана(ы) неисправность(и)
    {
        ui->comboBoxProblem->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 12;
    }
    if (ui->comboBoxIncomingSet->text() == "")        // если не указана комплектность
    {
        ui->comboBoxIncomingSet->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 13;
    }
    if (ui->comboBoxExterior->text() == "")        // если не указано состояние (внешинй вид)
    {
        ui->comboBoxExterior->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 14;
    }
    if (ui->comboBoxPresetEngineer->currentIndex() < 0 && comSettings->value("is_master_set_on_new").toBool())        // если не выбран инженер, а он обязателен
    {
        ui->comboBoxPresetEngineer->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 15;
    }
    if(!additionalFields->validate())
    {
        error = 16;
    }
    if ( ui->checkBoxIsEstPrice->isChecked() )   // если установлен флаг "Клиент ознакомлен с возможной стоимостью"
        if (ui->lineEditEstPrice->text() == "" || ui->lineEditEstPrice->text().toInt() == 0)   // соответствует ли норме введённая сумма
        {
            ui->lineEditEstPrice->setStyleSheet(commonLineEditStyleSheetRed);
            error = 17;
        }
    if ( ui->checkBoxIsPrepay->isChecked() )       // если установлен флаг "Клиент вносит предоплату"
    {
        if (ui->comboBoxPrepayReason->currentIndex() < 0)       // обязательно должно быть указано основание
            ui->comboBoxPrepayReason->setStyleSheet(commonComboBoxStyleSheetRed);
        if (ui->comboBoxPrepayAccount->currentIndex() < 0)      // опязательно должен быть выбран счёт
            ui->comboBoxPrepayAccount->setStyleSheet(commonComboBoxStyleSheetRed);
        if ( ui->lineEditPrepaySumm->text().toInt() == 0 )   // соответствует ли норме введённая сумма
        {
            ui->lineEditPrepaySumm->setStyleSheet(commonLineEditStyleSheetRed);
            error = 18;
        }
    }
    if ( ui->checkBoxWasEarlier->isChecked() ) // если установлен флаг "Ранее было в ремонте", то обязательно должен быть указан номер ремонта; однако, это не касается гарантийного ремонта
        if ( ui->lineEditPrevRepair->text() == "" )
        {
            if ( ui->lineEditPrevRepairFromOldDB->text() == "" )  // также допускается не выбирать предыдущий ремонт, если указан номер ремонта в старой системе CRM
            {
                ui->lineEditPrevRepair->setStyleSheet(commonLineEditStyleSheetRed);
                error = 19;
            }
        }

    if (error)
    {
        qDebug() << "Ошибка создания карты ремонта: не все обязательные поля заполнены (error " << error << ")";
        return false;
    }

    return true;
}

bool tabRepairNew::createClient()
{
    bool nErr = 1;

    clientModel->setFirstName(ui->lineEditClientFirstName->text());
    clientModel->setLastName(ui->lineEditClientLastName->text());
    clientModel->setPatronymicName(ui->lineEditClientPatronymic->text());
    clientModel->appendLogText(tr("Быстрое создание клиента из формы приёма в ремонт"));
    clientModel->setAdType(clientAdTypesList->databaseIDByRow(ui->comboBoxClientAdType->currentIndex()));
    clientModel->setAddress(ui->lineEditClientAddress->text());
    clientModel->setType(ui->checkBoxClientType->isChecked());
    nErr = clientModel->commit();
    client = clientModel->id();

    return nErr;
}

bool tabRepairNew::createRepair()
{
    if(!checkInput())
        return true; // return 0 — OK, return 1 - ошибка

    bool nErr = 1;
    int deviceClassIndex, deviceVendorIndex, deviceIndex;    // это currentIndex'ы combobox'ов, а не id записей в соответствующих таблицах БД
    int device, user, repair, preferredPaymentAccIndex, prepaySumm;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    setDefaultStyleSheets();
    deviceClassIndex = ui->comboBoxDeviceClass->currentIndex();
    deviceVendorIndex = ui->comboBoxDeviceVendor->currentIndex();

    user = userDbData->value("id").toInt();
    preferredPaymentAccIndex = ui->comboBoxPresetPaymentAccount->currentIndex();

#ifdef QT_DEBUG
    queryLog *sqlLog = new queryLog(query);
    sqlLog->setFile(QApplication::applicationDirPath() + "\\tabRepairNew.sql");
    sqlLog->truncateLog();
#endif

    try
    {
        if (clientModel->isNew() || clientModel->phones()->isUpdated())
        {
            QUERY_EXEC(query,nErr)(QUERY_BEGIN);
            if(clientModel->isNew())
                nErr = createClient();
            if(clientModel->phones()->isUpdated())
                nErr = clientModel->phones()->commit();
            QUERY_COMMIT_ROLLBACK(query,nErr);
            fillClientCreds(client);
        }

        QUERY_EXEC(query,nErr)(QUERY_BEGIN);

        // пользователь мог выбрать модель из списка, а затем отредаткировать её, поэтому пробуем найти модель в списке
        deviceIndex = devicesModel->findIndex(ui->comboBoxDevice->currentText());
        // если модель не найдена, создаём новую
        if ( deviceIndex < 0 )
        {
            SDevMdlModel *devMdl = new SDevMdlModel(this);
            devMdl->setName(ui->comboBoxDevice->currentText());
            devMdl->setDevice(deviceClassesModel->record(deviceClassIndex).value("id").toInt());
            devMdl->setMaker(deviceVendorsModel->record(deviceVendorIndex).value("id").toInt());
            nErr = devMdl->commit();
            device = devMdl->id();
            delete devMdl;
        }
        else
        {
            device = devicesModel->databaseIDByRow(deviceIndex);
        }

        repairModel->setClassId(deviceClassesModel->databaseIDByRow(ui->comboBoxDeviceClass->currentIndex()));
        repairModel->setVendorId(deviceVendorsModel->databaseIDByRow(ui->comboBoxDeviceVendor->currentIndex()));
        repairModel->setTitle(ui->comboBoxDeviceClass->currentText() + " " + ui->comboBoxDeviceVendor->currentText() + " " + ui->comboBoxDevice->currentText());
        repairModel->setDeviceId(device);
        repairModel->setClientId(clientModel->id());
        repairModel->setSerialNumber(ui->lineEditSN->text());
        repairModel->setCompanyIndex(ui->comboBoxCompany->currentIndex());
        repairModel->setOffice(userDbData->value("current_office").toInt());
        repairModel->setStartOffice(userDbData->value("current_office").toInt());
        repairModel->setManager(user);
        repairModel->setCurrentManager(user);
        repairModel->setMasterIndex(ui->comboBoxPresetEngineer->currentIndex());
        repairModel->setFault(ui->comboBoxProblem->currentText());
        repairModel->setComplect(ui->comboBoxIncomingSet->currentText());
        repairModel->setLook(ui->comboBoxExterior->currentText());
        repairModel->setExtNotes(ui->lineEditExtNotes->text());
        if(ui->checkBoxIsHighPriority->isChecked())
            repairModel->setExpressRepair(1);
        if(ui->checkBoxIsQuick->isChecked())
            repairModel->setQuickRepair(1);
        if(ui->checkBoxIsWarranty->isChecked())
            repairModel->setIsWarranty(1);
        if(ui->checkBoxWasEarlier->isChecked())
            repairModel->setIsRepeat(1);
        if(ui->checkBoxIsEstPrice->isChecked())
        {
            repairModel->setIsPreAgreed(1);
            repairModel->setPreAgreedAmount(sysLocale.toFloat(ui->lineEditEstPrice->text()));
        }
        if(ui->checkBoxIsNonImportantData->isChecked())
            repairModel->setCanFormat(1);
        if(ui->checkBoxIsCheckNeeded->isChecked())
            repairModel->setPrintCheck(1);
        if(ui->comboBoxPresetPlace->currentIndex() >= 0)
            repairModel->setBoxIndex(ui->comboBoxPresetPlace->currentIndex());
        if(ui->checkBoxIsPrepay->isChecked())
        {
            prepaySumm = sysLocale.toFloat(ui->lineEditPrepaySumm->text());
            repairModel->addPrepay(prepaySumm, ui->comboBoxPrepayReason->currentText());
        }
        if(ui->checkBoxWasInOtherWorkshop->isChecked())
            repairModel->setThirsPartySc(1);
        if(ui->lineEditPrevRepairFromOldDB->text().length())
            repairModel->setExtEarly(ui->lineEditPrevRepairFromOldDB->text());
        if( preferredPaymentAccIndex >= 0)
        {
            repairModel->setPaymentSystemIndex(preferredPaymentAccIndex);
            if(paymentSystemsModel->databaseIDByRow(preferredPaymentAccIndex, "system_id") == -1)
                repairModel->setIsCardPayment(1);
        }

        nErr = repairModel->commit();
        nErr = clientModel->updateRepairs();
        repair = repairModel->id();

        // запись значений доп. полей
        additionalFields->setRepair(repair);
        nErr = additionalFields->commit();

        if (ui->lineEditInsideComment->text() != "" )
        {
            comment->setRepair(repair);
            comment->setText(ui->lineEditInsideComment->text());
            nErr = comment->commit();
        }

        if (ui->checkBoxIsPrepay->isChecked())
        {
            cashRegister->setId(0);
            cashRegister->setClient(clientModel->id());
            cashRegister->setOperationType(SCashRegisterModel::RecptPrepayRepair);
            cashRegister->setRepairId(repair);
            cashRegister->setAmount(prepaySumm);
            cashRegister->setReason(QString("%1 (%2)").arg(cashRegister->constructReason(repair)).arg(ui->comboBoxPrepayReason->currentText()));
            cashRegister->setSkipLogRecording(true);
            nErr = cashRegister->commit();

            // TODO: Признак предмета расчета
        }

#ifdef QT_DEBUG
//        throw 0; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query, nErr);
    }
    catch (int type)
    {
        nErr = 0;
        repairModel->setId(0);
        additionalFields->resetIds();
        if(type == 0)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }
#ifdef QT_DEBUG
//    nErr = 1; // и это для отладки (чтобы проверить работу дальше)
    sqlLog->saveLog();
    delete sqlLog;
#endif

    if (nErr)   // если все запросы выполнены без ошибок, очистить всё, кроме данных клиента
    {
        print(repair);

        ui->comboBoxProblem->clearEditText();
        ui->comboBoxIncomingSet->clearEditText();
        ui->comboBoxExterior->clearEditText();
        ui->lineEditSN->setText("");
        ui->lineEditPrevRepair->setText("");
        ui->lineEditPrevRepairFromOldDB->setText("");
        ui->checkBoxIsQuick->setCheckState(Qt::Unchecked);
        ui->checkBoxWasInOtherWorkshop->setCheckState(Qt::Unchecked);
        ui->checkBoxIsHighPriority->setCheckState(Qt::Unchecked);
        ui->checkBoxIsNonImportantData->setCheckState(Qt::Unchecked);
        ui->checkBoxWasEarlier->setCheckState(Qt::Unchecked);
        ui->checkBoxIsWarranty->setCheckState(Qt::Unchecked);
        ui->checkBoxIsCashReceiptDocNeeded->setCheckState(Qt::Unchecked);
        ui->checkBoxIsCheckNeeded->setCheckState(Qt::Unchecked);
        ui->comboBoxPresetEngineer->setCurrentIndex(-1);
        ui->comboBoxPresetPlace->setCurrentIndex(-1);
        ui->spinBoxStickersCount->setValue(comSettings->value("rep_stickers_copy").toInt());
        ui->lineEditExtNotes->setText("");
        ui->lineEditInsideComment->setText("");
        ui->checkBoxIsEstPrice->setCheckState(Qt::Unchecked);
        ui->checkBoxIsPrepay->setCheckState(Qt::Unchecked);
        ui->comboBoxPrepayReason->setCurrentIndex(-1);
        ui->lineEditPrepaySumm->setText("0");
        ui->comboBoxPrepayAccount->setCurrentIndex(-1);
        ui->lineEditEstPrice->setText("0");
        ui->comboBoxDeviceClass->setCurrentIndex(-1);
        ui->comboBoxDeviceVendor->setCurrentIndex(-1);
        ui->comboBoxDevice->setCurrentIndex(-1);
    }

    delete query;
    return !nErr; // return 0 — OK, return 1 - ошибка
}

void tabRepairNew::createRepairClose()
{
    if (!createRepair())
        this->deleteLater();
}

void tabRepairNew::primaryPhoneEdited(QString number)
{
    ui->widgetClientMatch->findByPhone(number, ui->phones->primary()->maskIndex());
}

void tabRepairNew::preferredPaymentSystemChanged(int index)
{
    // при установке модели данных автоматически устанавливается индекс 0 и в модель ремонта устанавливается предпочитаемый тип оплаты, что есть неправильно
}

void tabRepairNew::prepayPaymentSystemChanged(int index)
{
    int sysId = paymentSystemsModel->databaseIDByRow(index, "system_id");
    cashRegister->setSystemId(sysId);
}

void tabRepairNew::print(int repair)
{
    QMap<QString, QVariant> report_vars;
    // печать квитанции
    if(comSettings->value("print_new_repair_report").toBool())
    {
        report_vars.insert("type", "new_rep");
        report_vars.insert("repair_id", repair);
        emit generatePrintout(report_vars);
        report_vars.clear();
    }

    // печать стикеров
    if(comSettings->value("print_rep_stickers").toBool())
    {
        report_vars.insert("type", "rep_label");
        report_vars.insert("repair_id", repair);
        report_vars.insert("copies", ui->spinBoxStickersCount->value());
        emit generatePrintout(report_vars);
        report_vars.clear();
    }
    // печать ПКО
    if(ui->checkBoxIsCashReceiptDocNeeded->isChecked())
    {
        report_vars.insert("type", "pko");
        emit generatePrintout(report_vars);
        report_vars.clear();
    }
}

void tabRepairNew::buttonCreateTabClientHandler()
{
    emit createTabClient(client);
}

#ifdef QT_DEBUG
void tabRepairNew::randomFill()
{
    int i;
    if (test_scheduler_counter == 0)   // клиент
//    if (1)
    {
//        fillClientCreds(257);
//        test_scheduler_counter++;
//        test_scheduler->start(400);    //  (пере-)запускаем таймер
//        return;
        if(0)
//        if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 или выбираем из уже имеющихся клиентов или создаём нового
        {
            fillClientCreds(QRandomGenerator::global()->bounded(7538)); // пытаемся заполнить данные уже имеющимся клиентом
            if (ui->lineEditClientLastName->text() == "")
                fillClientCreds(QRandomGenerator::global()->bounded(7538)); // если попался id несуществующего клиета
            else if (ui->lineEditClientLastName->text() == "")
                fillClientCreds(QRandomGenerator::global()->bounded(7538)); // и еще раз, на всякий пожарный
        }
        else    // ввод данных из тестового списка
        {
            i = QRandomGenerator::global()->bounded(clients4Test->size());
            ui->lineEditClientLastName->setText(clients4Test->value(i)->at(0));
            ui->lineEditClientFirstName->setText(clients4Test->value(i)->at(1));
            ui->lineEditClientPatronymic->setText(clients4Test->value(i)->at(2));
            if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 или телефон мобильный или городской (это для проверки масок)
            {
                ui->phones->primary()->testFill(2, clients4Test->value(i)->at(4));
            }
            else
            {
                ui->phones->primary()->testFill(1, clients4Test->value(i)->at(3));
            }
            if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 заполняем доп. номер или нет и аналогично 50/50 тип доп. номера
            {
                ui->phones->addPhone();
                if (QRandomGenerator::global()->bounded(100) > 50)
                {
                    ui->phones->forms().last()->testFill(2, clients4Test->value(i)->at(4));
                }
                else
                {
                    ui->phones->forms().last()->testFill(0, clients4Test->value(i)->at(3));
                }
            }
            i = QRandomGenerator::global()->bounded(clientAdTypesList->rowCount()); // если клиент новый, то случайно выбираем источник обращения
            ui->comboBoxClientAdType->setCurrentIndex(i);

        }
    }
    else if (test_scheduler_counter == 1)
    {
        i = deviceClassesModel->rowCount();
        ui->comboBoxDeviceClass->setCurrentIndex(QRandomGenerator::global()->bounded(i));
    }
    else if (test_scheduler_counter == 2)
    {
        i = deviceVendorsModel->rowCount();
        ui->comboBoxDeviceVendor->setCurrentIndex(QRandomGenerator::global()->bounded(i));
    }
    else if (test_scheduler_counter == 3)   // модель
    {
        i = devicesModel->rowCount();
        if (QRandomGenerator::global()->bounded(100) > 50 && i)  // 50/50 или выбираем из уже имеющихся моделей или случайное число (проверка i нужна, т. к. список может быть вообще пуст)
        {
            ui->comboBoxDevice->setCurrentIndex(QRandomGenerator::global()->bounded(i));
        }
        else
        {
//            qDebug() << "генерирую случайное число в качестве модели уст-ва, i = " << i;
            ui->comboBoxDevice->setCurrentText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
        }
    }
    else if (test_scheduler_counter == 4)   // серийный номер
    {
        ui->lineEditSN->setText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
    }
    else if (test_scheduler_counter == 5)   // неисправность
    {
        ui->comboBoxProblem->setFocus();
        i = classProblemsModel->rowCount();
        ui->comboBoxProblem->setCurrentText(classProblemsModel->record(QRandomGenerator::global()->bounded(i)).value("name").toString());
        if (ui->comboBoxProblem->currentText() == "")
            ui->comboBoxProblem->setCurrentText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
        QKeyEvent* newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier,"\t");
        QCoreApplication::postEvent(ui->comboBoxProblem->lineEdit(), newEvent);
        QKeyEvent* newEvent2 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QCoreApplication::postEvent(ui->comboBoxProblem->view(), newEvent2);
    }
    else if (test_scheduler_counter == 6)   // космолектность
    {
        ui->comboBoxIncomingSet->setFocus();
        i = classIncomingSetsModel->rowCount();
        ui->comboBoxIncomingSet->setCurrentText(classIncomingSetsModel->record(QRandomGenerator::global()->bounded(i)).value("name").toString());
        if (ui->comboBoxIncomingSet->currentText() == "")
            ui->comboBoxIncomingSet->setCurrentText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
        QKeyEvent* newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier,"\t");
        QCoreApplication::postEvent(ui->comboBoxIncomingSet->lineEdit(), newEvent);
        QKeyEvent* newEvent2 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QCoreApplication::postEvent(ui->comboBoxIncomingSet->view(), newEvent2);
    }
    else if (test_scheduler_counter == 7)   // внешний вид
    {
        ui->comboBoxExterior->setFocus();
        i = classIncomingSetsModel->rowCount();
        ui->comboBoxExterior->setCurrentText(classExteriorsModel->record(QRandomGenerator::global()->bounded(i)).value("name").toString());
        if (ui->comboBoxExterior->currentText() == "")
            ui->comboBoxExterior->setCurrentText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
        QKeyEvent* newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier,"\t");
        QCoreApplication::postEvent(ui->comboBoxExterior->lineEdit(), newEvent);
        QKeyEvent* newEvent2 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QCoreApplication::postEvent(ui->comboBoxExterior->view(), newEvent2);
    }
    else if (test_scheduler_counter == 8)
    {
/*
        for(int j=0; j< additionalFieldsWidgets.size(); j++)   // автозаполнение обязательных доп. полей
        {
            if ( additionalFieldsWidgets[j]->property("fieldRequired").toBool() )
            {
                if ( QString(additionalFieldsWidgets[j]->metaObject()->className()).compare("QComboBox", Qt::CaseSensitive) == 0 )
                {
                    QAbstractItemModel *addFldTmpModel = (static_cast<QComboBox*>(additionalFieldsWidgets[j]))->model();
                    i = addFldTmpModel->rowCount();
                    (static_cast<QComboBox*>(additionalFieldsWidgets[j]))->setCurrentIndex(QRandomGenerator::global()->bounded(i));
                }
                else if ( QString(additionalFieldsWidgets[j]->metaObject()->className()).compare("QLineEdit", Qt::CaseSensitive) == 0 )
                {
                    (static_cast<QLineEdit*>(additionalFieldsWidgets[j]))->setText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
                }
            }
        }
*/
    }
    else if (test_scheduler_counter == 9)   // инженер
    {
        i = engineersModel->rowCount();
        ui->comboBoxPresetEngineer->setCurrentIndex(QRandomGenerator::global()->bounded(i));
    }
    else if (test_scheduler_counter == 10)
    {
        // в случайном порядке включаем разные флаги
        i = QRandomGenerator::global()->bounded(16384);
        ui->checkBoxIsQuick->setChecked((i>>0)&0x01);
        ui->checkBoxWasInOtherWorkshop->setChecked((i>>1)&0x01);
        ui->checkBoxIsHighPriority->setChecked((i>>2)&0x01);
        ui->checkBoxIsNonImportantData->setChecked((i>>3)&0x01);
        ui->checkBoxWasEarlier->setChecked((i>>4)&0x01);
        ui->checkBoxIsWarranty->setChecked((i>>5)&0x01);
        ui->checkBoxIsEstPrice->setChecked((i>>6)&0x01);
        ui->checkBoxIsPrepay->setChecked((i>>7)&0x01);
        ui->checkBoxIsCashReceiptDocNeeded->setChecked((i>>8)&0x01);
        ui->checkBoxIsCheckNeeded->setChecked((i>>9)&0x01);
        if (ui->checkBoxWasEarlier->isChecked())
        {
            if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 или берём ремонт из базы или записываем значение "из другой БД"
            { // в данном случае это просто чтобы проверка обязательных полей проходила успешно; вызова функции заполнения полей значениями предыдущего ремонта не будет (пока)
                QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
                query->exec(QString("SELECT `id` FROM `workshop` WHERE `state` IN (8,12,16) AND `id` < %1 ORDER BY `id` DESC LIMIT 1 ").arg(QRandomGenerator::global()->bounded(24900)));
                query->first();
                if(query->isValid())
                    ui->lineEditPrevRepair->setText(query->value(0).toString());
                else
                    ui->checkBoxWasEarlier->setChecked(0);  // это если вдруг не выбран номер
            }
            else
            {
                ui->lineEditPrevRepairFromOldDB->show();
                ui->lineEditPrevRepairFromOldDB->setText(QString::number(QRandomGenerator::global()->bounded(1000000)));
            }
        }
        if (ui->checkBoxIsEstPrice->isChecked())
        {
            ui->lineEditEstPrice->setText(QString::number(QRandomGenerator::global()->bounded(100)*50));

        }
        if (ui->checkBoxIsPrepay->isChecked())
        {
            i = prepayReasonsModel->rowCount();
            ui->comboBoxPrepayReason->setCurrentIndex(QRandomGenerator::global()->bounded(i));
            ui->lineEditPrepaySumm->setText(QString::number(QRandomGenerator::global()->bounded(100)*50));
            i = paymentSystemsModel->rowCount();
            ui->comboBoxPrepayAccount->setCurrentIndex(QRandomGenerator::global()->bounded(i));
        }

    }
    else if (test_scheduler_counter == 11)
    {
//        createRepair();
//        createRepairClose();
//        main_window_test_scheduler2->start(500);    // запускаем таймер закрытия вкладки приёма в ремонт
    }
    else if (test_scheduler_counter == 12)
    {}
    else if (test_scheduler_counter == 13)
    {}
    if (test_scheduler_counter < 11)
    {
        test_scheduler_counter++;
        test_scheduler->start(400);    //  (пере-)запускаем таймер
    }
//    qDebug() << "test_scheduler_handler(), test_scheduler_counter = " << test_scheduler_counter++;

}

void tabRepairNew::test_scheduler_handler()  //
{
    randomFill();
}

void tabRepairNew::test_scheduler2_handler()  // обработик таймера закрытия вкладки
{
//    qDebug() << "test_scheduler2_handler(), clientTabId = " << ui->tabWidget->indexOf(tabClients::getInstance(0));
//    closeTab(ui->tabWidget->indexOf(tabClients::getInstance(0)));
//    test_scheduler->start(1000);    //  перезапускаем таймер открытия вкладки
}
#endif
