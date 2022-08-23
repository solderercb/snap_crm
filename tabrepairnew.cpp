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
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;
    query->exec(QUERY_BEGIN);
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_ACTIVITY(userDbData->value("id").toString()));
    QUERY_EXEC(query, nDBErr)(QUERY_INS_USER_ACTIVITY(QString("Navigation Приём в ремонт")));
    QUERY_COMMIT_ROLLBACK(query, nDBErr);
    delete query;

    ui->setupUi(this);
    this->setWindowTitle("Приём в ремонт");
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillClientCreds(int)));
#ifdef QT_DEBUG
    main_window_test_scheduler = parent->test_scheduler;
    main_window_test_scheduler2 = parent->test_scheduler2;
#endif

    clientModel2 = new SClientModel();
    ui->phones->setModel(clientModel2->phones());
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
    ui->comboBoxDevice->setPlaceholderText(tr("устройство"));
    ui->comboBoxDeviceMaker->setPlaceholderText(tr("производитель"));
    ui->comboBoxDeviceModel->lineEdit()->setPlaceholderText(tr("модель"));
    ui->comboBoxPresetEngineer->setPlaceholderText(tr("назначить инженером"));
    ui->comboBoxPresetBox->setPlaceholderText(tr("ячейка"));
    ui->comboBoxOffice->setPlaceholderText(tr("офис"));
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
    int i = 0;
    while (userDbData->value("current_office").toInt() != officesModel->record(i++).value("id").toInt());

    ui->comboBoxOffice->setCurrentIndex(i-1);
    ui->comboBoxPresetEngineer->setModel(engineersModel);
    ui->comboBoxPresetEngineer->setCurrentIndex(-1);
    ui->comboBoxPresetPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxPresetPaymentAccount->setCurrentIndex(-1);
    ui->comboBoxPrepayAccount->setModel(paymentSystemsModel);
    ui->comboBoxPrepayAccount->setCurrentIndex(-1);
    ui->comboBoxPresetBox->setModel(repairBoxesModel);
    ui->comboBoxPresetBox->setCurrentIndex(-1);
    ui->spinBoxStickersCount->setValue(comSettings->value("rep_stickers_copy").toInt());

    comboboxDevicesModel = new QSqlQueryModel();
    ui->comboBoxDevice->setModel(comboboxDevicesModel);
    comboboxDeviceMakersModel = new QSqlQueryModel();
    ui->comboBoxDeviceMaker->setModel(comboboxDeviceMakersModel);
    comboboxDeviceModelsModel = new QSqlQueryModel();
    ui->comboBoxDeviceModel->setModel(comboboxDeviceModelsModel);
    comboboxProblemModel = new QSqlQueryModel();
    ui->comboBoxProblem->setModel(comboboxProblemModel);
    ui->comboBoxProblem->setCurrentIndex(-1);
    comboBoxIncomingSetModel = new QSqlQueryModel();
    ui->comboBoxIncomingSet->setModel(comboBoxIncomingSetModel);
    comboBoxExteriorModel = new QSqlQueryModel();
    ui->comboBoxExterior->setModel(comboBoxExteriorModel);

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

    comboboxDevicesModel->setQuery(QUERY_SEL_DEVICES, QSqlDatabase::database("connMain"));
    ui->comboBoxDevice->setCurrentIndex(-1);
    msgBox.setText(tr("Ошибка"));
    msgBox.setIcon(QMessageBox::Critical);

#ifdef QT_DEBUG
    test_scheduler = new QTimer();
    test_scheduler->setSingleShot(true);
    test_scheduler2 = new QTimer();
    test_scheduler2->setSingleShot(true);
    QObject::connect(test_scheduler, SIGNAL(timeout()), this, SLOT(test_scheduler_handler()));
    QObject::connect(test_scheduler2, SIGNAL(timeout()), this, SLOT(test_scheduler2_handler()));

    test_scheduler->start(200);
#endif
}

tabRepairNew::~tabRepairNew()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;
    query->exec(QUERY_BEGIN);
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_ACTIVITY(userDbData->value("id").toString()));
    QUERY_COMMIT_ROLLBACK(query, nDBErr);
    delete query;

    delete ui;
    delete comboboxDeviceMakersModel;
    delete comboboxDeviceModelsModel;
    delete comboboxDevicesModel;
    for(int i=additionalFieldsWidgets.size()-1;i>=0;i--)   // удаляем доп. поля
    {
        if ( QString(additionalFieldsWidgets[i]->metaObject()->className()).compare("QComboBox", Qt::CaseSensitive) == 0 )
            delete static_cast<QComboBox*>(additionalFieldsWidgets[i])->model();    // удаляем модель данных, если объект QComboBox
        delete additionalFieldsWidgets[i];
        additionalFieldsWidgets.removeAt(i);
    }
    delete comboboxProblemModel;
    delete comboBoxIncomingSetModel;
    delete comboBoxExteriorModel;
    delete clientsMatchTable;
    delete devicesMatchTable;
    delete groupBoxEventFilter;
    #ifdef QT_DEBUG
        delete test_scheduler;
        delete test_scheduler2;
    #endif
    p_instance = nullptr;   // Обязательно блять!
}

bool tabRepairNew::tabCloseRequest()
{
    // TODO: сделать проверку не сохранённых данных перед закрытием
    return 1;
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

// https://stackoverflow.com/a/18866593
QString tabRepairNew::genUserWebPass() const
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
   const int randomStringLength = 6; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
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

void tabRepairNew::changeDeviceType()
{

    int comboBoxDeviceIndex = ui->comboBoxDevice->currentIndex();
    int additionalFieldRow = 2, additionaFieldCol = 0;
    int additionalFieldType = 0;
    QSizePolicy *additionalFieldsSizePolicy = new QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    QString query;

//    qDebug() << "tabRepairNew::changeDeviceType: additionalFieldsWidgets.size() =" << additionalFieldsWidgets.size() << "(before)";
    for(int i=additionalFieldsWidgets.size()-1;i>=0;i--)   // в случае ошибочного выбора категории уст-ва, нужно удалить ранее добавленные виджеты доп. полей
    {
//        qDebug() << "Removing widget; fieldId = " << additionalFieldsWidgets[i]->property("fieldId").toInt() << "; fieldType = " << additionalFieldsWidgets[i]->property("fieldType").toInt();
        delete additionalFieldsWidgets[i];
        additionalFieldsWidgets.removeAt(i);
    }
//    qDebug() << "tabRepairNew::changeDeviceType: additionalFieldsWidgets.size() =" << additionalFieldsWidgets.size() << "(after)";

    query = QUERY_SEL_DEVICE_MAKERS(comboboxDevicesModel->index(comboBoxDeviceIndex, 2).data().toString());
    comboboxDeviceMakersModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxDeviceMaker->setCurrentIndex(-1);

    // Заполнение модели выпадающего списка неисправностей
    query = QUERY_SEL_DEVICE_FAULTS(comboboxDevicesModel->index(comboBoxDeviceIndex, 1).data().toInt());
    comboboxProblemModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxProblem->setCurrentIndex(-1);


    // Заполнение модели выпадающего списка комплектности
    query = QUERY_SEL_DEVICE_SET(comboboxDevicesModel->index(comboBoxDeviceIndex, 1).data().toInt());
    comboBoxIncomingSetModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxIncomingSet->setCurrentIndex(-1);

    // Заполнение модели выпадающего списка внешнего вида
    query = QUERY_SEL_DEVICE_EXTERIOR(comboboxDevicesModel->index(comboBoxDeviceIndex, 1).data().toInt());
    comboBoxExteriorModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxExterior->setCurrentIndex(-1);

    // создание доп. полей для выбранной категории уст-ва
    QSqlQuery* additionalFieldsList = new QSqlQuery(QSqlDatabase::database("connMain"));

    query = QUERY_SEL_DEVICE_ADD_FIELDS(comboboxDevicesModel->index(comboBoxDeviceIndex, 1).data().toInt());
    additionalFieldsList->exec(query);

    while(additionalFieldsList->next())
    {
        additionalFieldType = additionalFieldsList->value(2).toInt();
        QWidget *additionalFieldWidget;
        if (additionalFieldType == addlField::LineEdit)
        {
            QLineEdit *additionalFieldLineEdit = new QLineEdit(this);
            additionalFieldWidget = additionalFieldLineEdit;
            // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
            additionalFieldLineEdit->setPlaceholderText(additionalFieldsList->value(0).toString());
        } else if (additionalFieldType == addlField::ComboBox)
        {
            QComboBox *additionalFieldComboBox = new QComboBox(this);
            additionalFieldWidget = additionalFieldComboBox;
            QStandardItemModel* additionalFieldComboBoxModel = new QStandardItemModel();
            // TODO: нужно проверить удаляется ли модель при удалении виджета
            QStringList additionalFieldComboBoxItems = additionalFieldsList->value(1).toString().split('\n');
            QStandardItem *newRow;
            QFontMetrics *fm = new QFontMetrics(this->font());
            int itemTextWidth, dropDownListWidth = 0;

            additionalFieldComboBox->setModel(additionalFieldComboBoxModel);
//            if (additionalFieldComboBoxItems.at(0) != "")   // Принудительно добавляем пустую строку
//            {
//                newRow = new QStandardItem();
//                newRow->setText("");
//                additionalFieldComboBoxModel->appendRow(newRow);
//            }
            for (int i=0; i<additionalFieldComboBoxItems.size(); i++)
            {
                newRow = new QStandardItem();
                newRow->setText(additionalFieldComboBoxItems.at(i));
                additionalFieldComboBoxModel->appendRow(newRow);

                // определяем наибольшую длину текста в списке элементов
                itemTextWidth = fm->size(Qt::TextSingleLine, additionalFieldComboBoxItems.at(i)).width() + 10;
                if (itemTextWidth > dropDownListWidth)
                    dropDownListWidth = itemTextWidth;
            }
            delete fm;  // больше не нужен
            additionalFieldComboBox->setMinimumWidth(100);
            additionalFieldComboBox->view()->setMinimumWidth(dropDownListWidth);
            additionalFieldComboBox->setCurrentIndex(-1);
            additionalFieldComboBox->setEditable(true);
            // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
            additionalFieldComboBox->lineEdit()->setPlaceholderText(additionalFieldsList->value(0).toString());
        } else if (additionalFieldType == addlField::DateEdit)
        {
            QDateEdit *additionalFieldDateTimeEdit = new QDateEdit(QDate::currentDate());
            additionalFieldWidget = additionalFieldDateTimeEdit;
            additionalFieldDateTimeEdit->setCalendarPopup(true);
        } else if (additionalFieldType == addlField::dummy)
        {
            // TODO: В АСЦ не реализовано, поэтому используем заглушку из лайнэдита
            QLineEdit *additionalFieldLineEdit = new QLineEdit(this);
            additionalFieldWidget = additionalFieldLineEdit;
            // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
            additionalFieldLineEdit->setPlaceholderText(additionalFieldsList->value(0).toString());
            additionalFieldLineEdit->setEnabled(false);
        }
        else
            additionalFieldWidget = new QWidget();  // на случай ошибок в БД

        additionalFieldWidget->setSizePolicy(*additionalFieldsSizePolicy);
        additionalFieldsWidgets.append(additionalFieldWidget);
        ui->gridLayoutDeviceDescription->addWidget(additionalFieldWidget, additionalFieldRow, additionaFieldCol, 1, 1);
        additionalFieldWidget->setProperty("fieldId", additionalFieldsList->value(3).toInt());
        additionalFieldWidget->setProperty("fieldType", additionalFieldType);
        additionalFieldWidget->setProperty("fieldRequired", additionalFieldsList->value(4).toBool());
        additionalFieldWidget->setProperty("fieldPrintable", additionalFieldsList->value(5).toBool());

        if (++additionaFieldCol > 5)
        {
            additionalFieldRow++;
            additionaFieldCol = 0;
        }
    }

//    qDebug() << "tabRepairNew::changeDeviceType: additionalFieldsWidgets.size() =" << additionalFieldsWidgets.size() << "(end of function)";
//    delete additionalFieldsList;
    delete additionalFieldsSizePolicy;

}

void tabRepairNew::changeDeviceMaker()
{
    int comboBoxDeviceIndex = ui->comboBoxDevice->currentIndex();
    int comboBoxDeviceMakerIndex = ui->comboBoxDeviceMaker->currentIndex();
    int deviceId = comboboxDevicesModel->index(comboBoxDeviceIndex, 1).data().toInt();
    int deviceMakerId = comboboxDeviceMakersModel->index(comboBoxDeviceMakerIndex, 1).data().toInt();
    QString query;

    query = QUERY_SEL_DEVICE_MODELS.arg(deviceId).arg(deviceMakerId);
    comboboxDeviceModelsModel->setQuery(query, QSqlDatabase::database("connMain"));
    ui->comboBoxDeviceModel->setCurrentIndex(-1);
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

    delete clientModel2;
    clientModel2 = new SClientModel();
    ui->phones->setModel(clientModel2->phones());

    if (hideCoincidence)
        ui->widgetClientMatch->hide();
}

void tabRepairNew::lineEditPrevRepairButtonsHandler(int button)
{
    if (button == SLineEdit::Search)
        emit createTabSelectPrevRepair(1);
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
    clientModel = new QSqlQueryModel();
    clientPhonesModel = new QSqlQueryModel();

    QString query;
    query = QUERY_SEL_CLIENT(id);
    clientModel->setQuery(query, QSqlDatabase::database("connMain"));

    clearClientCreds(false);    // очищаем данные клиента, но не прячем таблицу совпадений
    if(clientModel->record(0).value("notes").toString() != "")
    {
        msgBox.setText(clientModel->record(0).value("notes").toString());
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }

    if (clientModel->record(0).value("type").toBool())
        ui->checkBoxClientType->setChecked(true);
    else
        ui->checkBoxClientType->setChecked(false);
    changeClientType();

    client = id;
    clientModel2->load(client);
    ui->checkBoxClientType->setEnabled(false);
    ui->lineEditClientLastName->setReadOnly(true);  // запрет на изменение, если клиент из базы
    ui->lineEditClientFirstName->setReadOnly(true);
    ui->lineEditClientPatronymic->setReadOnly(true);
    ui->comboBoxClientAdType->setEnabled(false);
    ui->lineEditClientAddress->setReadOnly(true);
    ui->lineEditClientEmail->setReadOnly(true);

    ui->pushButtonCreateTabClient->setEnabled(true);
    ui->lineEditClientFirstName->setText(clientModel->record(0).value("name").toString());
    ui->lineEditClientLastName->setText(clientModel->record(0).value("surname").toString());
    ui->lineEditClientPatronymic->setText(clientModel->record(0).value("patronymic").toString());
    ui->phones->setModel(clientModel2->phones());
    ui->lineEditClientAddress->setText(clientModel->record(0).value("address").toString());
    ui->lineEditClientEmail->setText(clientModel->record(0).value("email").toString());

    ui->comboBoxProblem->setFocus();    // устанавливаем фокус на полее ввода неисправности

    clientPhonesModel->deleteLater();
    clientModel->deleteLater();
}

void tabRepairNew::fillDeviceCreds(int id)
{
    int i;
    int repair, device, deviceMaker, deviceModel, client;
    queryDevice = new QSqlQueryModel();

    QString query;
    query = QUERY_SEL_DEVICE(id);
    queryDevice->setQuery(query, QSqlDatabase::database("connMain"));

    repair = queryDevice->record(0).value("id").toInt();
    device = queryDevice->record(0).value("type").toInt();
    deviceMaker = queryDevice->record(0).value("maker").toInt();
    deviceModel = queryDevice->record(0).value("model").toInt();
    client = queryDevice->record(0).value("client").toInt();

    fillClientCreds(client);
//    if (ui->comboBoxDevice->currentIndex() < 0)        // только если пользователь не выбрал тип уст-ва
    {
        for (i = 0; i < comboboxDevicesModel->rowCount(); i++)  // перебираем все типы уст-в в поисках нужного id
        {
            if (comboboxDevicesModel->index(i, 1).data().toInt() == device)
                break;
        }
        ui->comboBoxDevice->setCurrentIndex(i);
        // TODO: сделать запросы к БД асинхронными. Готовые классы https://github.com/micjabbour/MSqlQuery
        // на данный момент по локальной сети всё работает быстро, поэтому не буду заморачиваться

        for (i = 0; i < comboboxDeviceMakersModel->rowCount(); i++)  // перебираем всех производителей уст-в в поисках нужного id
        {
            if (comboboxDeviceMakersModel->index(i, 1).data().toInt() == deviceMaker)
                break;
        }
        ui->comboBoxDeviceMaker->setCurrentIndex(i);
        for (i = 0; i < comboboxDeviceModelsModel->rowCount(); i++)  // перебираем все модели уст-в в поисках нужной id
        {
            if (comboboxDeviceModelsModel->index(i, 1).data().toInt() == deviceModel)
                break;
        }
        ui->comboBoxDeviceModel->setCurrentIndex(i);
        ui->lineEditSN->setText(queryDevice->record(0).value("serial_number").toString());
    }
    ui->lineEditPrevRepair->setText(QString::number(repair));  // устанавливаем номер предыдущего ремонта в соотв. поле
    ui->checkBoxWasEarlier->setCheckState(Qt::Checked);     // ставим галочку "Ранее было в ремонте" чтобы поле с номером ремонта отобразилось

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
    ui->comboBoxDevice->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxDeviceMaker->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxDeviceModel->setStyleSheet(commonComboBoxStyleSheet);
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
    ui->comboBoxPresetBox->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxOffice->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPresetPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPresetEngineer->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditEstPrice->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxPrepayReason->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditPrepaySumm->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxPrepayAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditPrevRepair->setStyleSheet(commonLineEditStyleSheet);

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

}

bool tabRepairNew::checkInput()
{
    int error = 0;

    if ( ui->comboBoxDevice->currentIndex() < 0 )        // если не выбран тип уст-ва
    {
        ui->comboBoxDevice->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 1;
    }
    if ( ui->comboBoxDeviceMaker->currentIndex() < 0 )        // если не выбран производителя
    {
        ui->comboBoxDeviceMaker->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 2;
    }
    if ( ui->comboBoxDeviceModel->currentIndex() < 0 && ui->comboBoxDeviceModel->currentText() == "" )        // если не выбрана или не написана модель
    {
        ui->comboBoxDeviceModel->setStyleSheet(commonComboBoxStyleSheetRed);
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
    for(int i=0; i< additionalFieldsWidgets.size(); i++)   // установка стилей доп. полей
    {
        if ( additionalFieldsWidgets[i]->property("fieldRequired").toBool() )
        {
            // TODO: добавить в БД поле для задания маски ввода
            if ( additionalFieldsWidgets[i]->property("fieldType").toInt() == addlField::ComboBox )
            {
                if (static_cast<QComboBox*>(additionalFieldsWidgets[i])->currentText() == "")
                {
                    additionalFieldsWidgets[i]->setStyleSheet(commonComboBoxStyleSheetRed);
                    error = 16;
                }
            }
            else if ( additionalFieldsWidgets[i]->property("fieldType").toInt() == addlField::LineEdit )
            {
                if (static_cast<QLineEdit*>(additionalFieldsWidgets[i])->text() == "")
                {
                    additionalFieldsWidgets[i]->setStyleSheet(commonLineEditStyleSheetRed);
                    error = 16;
                }
            }
            // Дату не окрашиваем, даже если она обязательна, т. к. не продуман механизм проверки
//            else if ( additionalFieldsWidgets[i]->property("fieldType").toInt() == addlField::DateEdit )
//            {
//                if (static_cast<QDateEdit>(additionalFieldsWidgets[i]).text() == "")
//                {
//                    additionalFieldsWidgets[i]->setStyleSheet(commonDateEditStyleSheetRed);
//                    error = 16;
//                }
//            }
            else if ( additionalFieldsWidgets[i]->property("fieldType").toInt() == addlField::dummy )
            {
                if (static_cast<QLineEdit*>(additionalFieldsWidgets[i])->text() == "")
                {
                    additionalFieldsWidgets[i]->setStyleSheet(commonLineEditStyleSheetRed);
                    error = 16;
                }
            }
        }
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
    bool nDBErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);
    clientModel2->setFirstName(ui->lineEditClientFirstName->text());
    clientModel2->setLastName(ui->lineEditClientLastName->text());
    clientModel2->setPatronymicName(ui->lineEditClientPatronymic->text());
    clientModel2->appendLogText(tr("Быстрое создание клиента из формы приёма в ремонт"));
    clientModel2->setAdType(clientAdTypesList->databaseIDByRow(ui->comboBoxClientAdType->currentIndex()));
    clientModel2->setAddress(ui->lineEditClientAddress->text());
    clientModel2->setType(ui->checkBoxClientType->isChecked());
//    if (ui->lineEditClientPhone->hasAcceptableInput())
//    {
//        clientModel2->addPhone(ui->lineEditClientPhone->text(),
//                              ui->comboBoxClientPhoneType->currentIndex());
//    }
    nDBErr = clientModel2->commit();

    QUERY_COMMIT_ROLLBACK(query,nDBErr);

    delete query;
    return nDBErr;
}

bool tabRepairNew::createRepair()
{
    if(!checkInput())
        return true; // return 0 — OK, return 1 - ошибка

    int error = 0;
    int deviceTypeIndex, deviceMakerIndex, deviceModelIndex;    // это currentIndex'ы combobox'ов, а не id записей в соответствующих таблицах БД
    int deviceModel, user, office, company, repair, engineer, payment_system, prepaySumm, box;
    bool isPrepay, isEstPrice, isWarranty, isRepeat, nDBErr = 1, nIntegrityErr = 1;
    QString prevRepair = "NULL", prevRepairFromOldDB = "NULL", phone1, phone1c, phone2, phone2c;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    setDefaultStyleSheets();
    deviceTypeIndex = ui->comboBoxDevice->currentIndex();
    deviceMakerIndex = ui->comboBoxDeviceMaker->currentIndex();
    deviceModelIndex = ui->comboBoxDeviceModel->currentIndex();
    isEstPrice = ui->checkBoxIsEstPrice->isChecked();
    isPrepay = ui->checkBoxIsPrepay->isChecked();
    prepaySumm = ui->lineEditPrepaySumm->text().toInt();
    isRepeat = ui->checkBoxWasEarlier->isChecked();
    prevRepair = ui->lineEditPrevRepair->text();
    prevRepairFromOldDB = ui->lineEditPrevRepairFromOldDB->text();
    if ( prevRepair == "" )
        prevRepair = "NULL";
    if( prevRepairFromOldDB == "" )
        prevRepairFromOldDB = "NULL";
    else
        prevRepairFromOldDB = "\'" + prevRepairFromOldDB + "\'";

    user = userDbData->value("id").toInt();
    office = officesModel->record(ui->comboBoxOffice->currentIndex()).value("id").toInt();
    company = companiesModel->record(ui->comboBoxCompany->currentIndex()).value("id").toInt();
    engineer = (ui->comboBoxPresetEngineer->currentIndex() < 0)?0:engineersModel->record(ui->comboBoxPresetEngineer->currentIndex()).value("id").toInt();
    payment_system = paymentSystemsModel->record(ui->comboBoxPresetPaymentAccount->currentIndex()).value("system_id").toInt();
    box = (ui->comboBoxPresetBox->currentIndex() < 0)?0:repairBoxesModel->record(ui->comboBoxPresetBox->currentIndex()).value("id").toInt();
    isWarranty = ui->checkBoxIsWarranty->isChecked();

#ifdef QT_DEBUG
    queryLog *sqlLog = new queryLog(query);
    sqlLog->setFile(QApplication::applicationDirPath() + "\\tabRepairNew.sql");
    sqlLog->truncateLog();
#endif

    if (clientModel2->isClear())
        createClient();
    client= clientModel2->id();
    if(clientModel2->phones()->isUpdated())
        clientModel2->phones()->commit();

    QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);

    // пользователь мог выбрать модель из списка, а затем отредаткировать её, поэтому пробуем найти модель в списке
    deviceModelIndex = comboboxDeviceModelsModel->rowCount();
    while (--deviceModelIndex >= 0 && comboboxDeviceModelsModel->record(deviceModelIndex).value("name").toString() != ui->comboBoxDeviceModel->currentText());
    // если не выбрана модель из списка, создаём новую. Или если была выбрана модель из списка, а затем текст был отредактирован, тоже записываем в таблицу новую модель.
    if ( deviceModelIndex < 0 ||  (comboboxDeviceModelsModel->record(deviceModelIndex).value("name").toString() != ui->comboBoxDeviceModel->currentText()) )
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_DEVICE_MODEL
                    .arg(ui->comboBoxDeviceModel->currentText())
                    .arg("NULL")
                    .arg(comboboxDeviceMakersModel->record(deviceMakerIndex).value("id").toInt())
                    .arg(comboboxDevicesModel->record(deviceTypeIndex).value("id").toInt()));
        QUERY_LAST_INS_ID(query,nDBErr,deviceModel);
        qDebug() << "Новая модель уст-ва: LAST_INSERT_ID() = " << deviceModel;
    }
    else
    {
        deviceModel = comboboxDeviceModelsModel->record(deviceModelIndex).value("id").toInt();
//        qDebug() << "model already exists";
    }

//    qDebug() << (QUERY_INS_WORKSHOP
    QUERY_EXEC(query,nDBErr)(QUERY_INS_WORKSHOP
                .arg(ui->comboBoxDevice->currentText() + " " + ui->comboBoxDeviceMaker->currentText() + " " + ui->comboBoxDeviceModel->currentText())
                .arg(client)
                .arg(comboboxDevicesModel->record(deviceTypeIndex).value("id").toInt())
                .arg(comboboxDeviceMakersModel->record(deviceMakerIndex).value("id").toInt())
                .arg(deviceModel)
                .arg(ui->lineEditSN->text())
                .arg(company)
                .arg(office)
                .arg(user)
                .arg((engineer == 0)?"NULL":QString::number(engineer))
                .arg(ui->checkBoxIsHighPriority->isChecked())
                .arg(ui->checkBoxIsQuick->isChecked())
                .arg(isWarranty)
                .arg(isRepeat)
                .arg(payment_system)
                .arg((paymentSystemsModel->record(ui->comboBoxPresetPaymentAccount->currentIndex()).value("system_id").toInt() == -1)?1:0)
                .arg(ui->checkBoxIsNonImportantData->isChecked())
                .arg(ui->checkBoxIsCheckNeeded->isChecked())
                .arg((box == 0)?"NULL":QString::number(box))
                .arg(ui->lineEditExtNotes->text())
                .arg(isPrepay)
                .arg((isPrepay)?ui->comboBoxPrepayReason->currentData(Qt::UserRole+1).toInt():0)
                .arg((isPrepay)?ui->lineEditPrepaySumm->text().toInt():0)
                .arg(isEstPrice)
                .arg((isEstPrice)?ui->lineEditEstPrice->text().toInt():0)
                .arg(ui->comboBoxProblem->currentText())
                .arg(ui->comboBoxIncomingSet->currentText())
                .arg(ui->comboBoxExterior->currentText())
                .arg(ui->checkBoxWasInOtherWorkshop->isChecked())
                .arg("TODO: ")
                .arg(prevRepair)
                .arg(prevRepairFromOldDB));
    QUERY_LAST_INS_ID(query,nDBErr,repair);
//    qDebug() << "Новый ремонт: LAST_INSERT_ID() = " << repair;
//    qDebug() << QUERY_INS_LOG("NULL",3,user,office,client,repair,"NULL","NULL","NULL","Устройство принято в ремонт №"+QString::number(repair));
    QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,client,repair,"NULL","NULL","NULL","Устройство принято в ремонт №"+QString::number(repair)));
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_CLIENT_REPAIRS(client));

    if (ui->lineEditInsideComment->text() != "" )
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_REPAIR_COMMENT
                .arg(ui->lineEditInsideComment->text())
                .arg(user)
                .arg(repair));
//        qDebug() << QUERY_INS_LOG("NULL",3,user,office,"NULL",repair,"NULL","NULL","NULL","Добавлен внутренний комментарий к ремонту №"+QString::number(repair));
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,"NULL",repair,"NULL","NULL","NULL","Добавлен внутренний комментарий к ремонту №"+QString::number(repair)));
    }

    if (isPrepay)
    {
//        qDebug() << (QUERY_INS_CASH(
        QUERY_EXEC(query,nDBErr)(QUERY_INS_CASH(
                                   12,
                                   ui->lineEditPrepaySumm->text(),
                                   amountToWords(prepaySumm).replace('\'',"\\\'"),
                                   client,
                                   user,
                                   office,
                                   QString("Предоплата за ремонт №%1 в размере %2").arg(repair).arg(sysLocale.toCurrencyString(prepaySumm)),
                                   repair,
                                   "NULL",
                                   paymentSystemsModel->record(ui->comboBoxPrepayAccount->currentIndex()).value("system_id").toInt()
                                   )
                    );
        // финансы — очень важная вещь, поэтому нужно верифицировать записанные данные
        QUERY_EXEC(query,nDBErr)(QUERY_VRFY_CASH(
                                   12,
                                   ui->lineEditPrepaySumm->text(),
                                   amountToWords(prepaySumm).replace('\'',"\\\'"),
                                   client,
                                   user,
                                   office,
                                   QString("Предоплата за ремонт №%1 в размере %2").arg(repair).arg(sysLocale.toCurrencyString(prepaySumm)),
                                   repair,
                                   "\'NULL\'",
                                   paymentSystemsModel->record(ui->comboBoxPrepayAccount->currentIndex()).value("system_id").toInt()
                                   )
                    );

        QUERY_EXEC_VRFY(query,nIntegrityErr);
        if (!nIntegrityErr)
        {
//            qDebug() << "Ошибка целостности данных!";
            msgBox.setText(QString("Ошибка целостности данных!"));
            msgBox.exec();
            nDBErr = 0;
        }
//        else
//            qDebug() << "Верификация данных: ОК";

        // TODO: Признак предмета расчета
        // В журнале делаю запись о предоплате, думаю это будет удобно; ASC CRM такую запись вроде бы не делает :-(
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,"NULL",repair,"NULL","NULL","NULL",QString("Внесена предоплата за ремонт №%2 в размере %3 (%1)").arg(ui->comboBoxPrepayReason->currentText()).arg(repair).arg(sysLocale.toCurrencyString(prepaySumm))));
    }

    for(int i=0; i< additionalFieldsWidgets.size(); i++)   // запись значений доп. полей
    {
        QString text = "";
        switch (additionalFieldsWidgets[i]->property("fieldType").toInt())  {
            case addlField::ComboBox: {
                QComboBox *qw = static_cast<QComboBox*>(additionalFieldsWidgets[i]);
                text = qw->currentText();
                break;}
            case addlField::LineEdit: {
                QLineEdit *qw = static_cast<QLineEdit*>(additionalFieldsWidgets[i]);
                text = qw->text();
                break;}
            case addlField::DateEdit: {
                QDateEdit *qw = static_cast<QDateEdit*>(additionalFieldsWidgets[i]);
                text = qw->text();
                break;}
            case addlField::dummy: {
                QLineEdit *qw = static_cast<QLineEdit*>(additionalFieldsWidgets[i]);
                text = qw->text();
                break;}
        }
        if (text.length() > 0)
        {
//            qDebug() << "additional field text:" << text << "; text.length() =" << text.length();
            QUERY_EXEC(query,nDBErr)(QUERY_INS_FIELDS(additionalFieldsWidgets[i]->property("fieldId").toInt(), repair, "NULL", text));
        }
    }

    QUERY_COMMIT_ROLLBACK(query, nDBErr);
#ifdef QT_DEBUG
//    nDBErr = 1; // и это для отладки (чтобы проверить работу дальше)
    sqlLog->saveLog();
    delete sqlLog;
#endif

    if (nDBErr)   // если все запросы выполнены без ошибок, очистить всё, кроме данных клиента
    {
        // печать квитанции
        if(comSettings->value("print_new_repair_report").toBool())
        {
            QMap<QString, QVariant> report_vars;
            report_vars.insert("type", "new_rep");
            report_vars.insert("repair_id", repair);
            emit generatePrintout(report_vars);
        }

        // печать стикеров
        if(comSettings->value("print_rep_stickers").toBool())
        {
            QMap<QString, QVariant> report_vars;
            report_vars.insert("type", "rep_label");
            report_vars.insert("repair_id", repair);
            report_vars.insert("copies", ui->spinBoxStickersCount->value());
            emit generatePrintout(report_vars);
        }

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
        ui->comboBoxPresetBox->setCurrentIndex(-1);
        ui->spinBoxStickersCount->setValue(comSettings->value("rep_stickers_copy").toInt());
        ui->lineEditExtNotes->setText("");
        ui->lineEditInsideComment->setText("");
        ui->checkBoxIsEstPrice->setCheckState(Qt::Unchecked);
        ui->checkBoxIsPrepay->setCheckState(Qt::Unchecked);
        ui->comboBoxPrepayReason->setCurrentIndex(-1);
        ui->lineEditPrepaySumm->setText("0");
        ui->comboBoxPrepayAccount->setCurrentIndex(-1);
        ui->lineEditEstPrice->setText("0");
        ui->comboBoxDevice->setCurrentIndex(-1);
        ui->comboBoxDeviceMaker->setCurrentIndex(-1);
        ui->comboBoxDeviceModel->setCurrentIndex(-1);
    }

    delete query;
    return !nDBErr; // return 0 — OK, return 1 - ошибка
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
        fillClientCreds(257);
        test_scheduler_counter++;
        test_scheduler->start(400);    //  (пере-)запускаем таймер
        return;
//        if(0)
        if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 или выбираем из уже имеющихся клиентов или создаём нового
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
        i = comboboxDevicesModel->rowCount();
        ui->comboBoxDevice->setCurrentIndex(QRandomGenerator::global()->bounded(i));
    }
    else if (test_scheduler_counter == 2)
    {
        i = comboboxDeviceMakersModel->rowCount();
        ui->comboBoxDeviceMaker->setCurrentIndex(QRandomGenerator::global()->bounded(i));
    }
    else if (test_scheduler_counter == 3)   // модель
    {
        i = comboboxDeviceModelsModel->rowCount();
        if (QRandomGenerator::global()->bounded(100) > 50 && i)  // 50/50 или выбираем из уже имеющихся моделей или случайное число (проверка i нужна, т. к. список может быть вообще пуст)
        {
            ui->comboBoxDeviceModel->setCurrentIndex(QRandomGenerator::global()->bounded(i));
        }
        else
        {
//            qDebug() << "генерирую случайное число в качестве модели уст-ва, i = " << i;
            ui->comboBoxDeviceModel->setCurrentText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
        }
    }
    else if (test_scheduler_counter == 4)   // серийный номер
    {
        ui->lineEditSN->setText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
    }
    else if (test_scheduler_counter == 5)   // неисправность
    {
        ui->comboBoxProblem->setFocus();
        i = comboboxProblemModel->rowCount();
        ui->comboBoxProblem->setCurrentText(comboboxProblemModel->record(QRandomGenerator::global()->bounded(i)).value("name").toString());
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
        i = comboBoxIncomingSetModel->rowCount();
        ui->comboBoxIncomingSet->setCurrentText(comboBoxIncomingSetModel->record(QRandomGenerator::global()->bounded(i)).value("name").toString());
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
        i = comboBoxIncomingSetModel->rowCount();
        ui->comboBoxExterior->setCurrentText(comboBoxExteriorModel->record(QRandomGenerator::global()->bounded(i)).value("name").toString());
        if (ui->comboBoxExterior->currentText() == "")
            ui->comboBoxExterior->setCurrentText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
        QKeyEvent* newEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier,"\t");
        QCoreApplication::postEvent(ui->comboBoxExterior->lineEdit(), newEvent);
        QKeyEvent* newEvent2 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QCoreApplication::postEvent(ui->comboBoxExterior->view(), newEvent2);
    }
    else if (test_scheduler_counter == 8)
    {
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
