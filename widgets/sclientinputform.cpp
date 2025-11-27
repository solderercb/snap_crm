#include "sclientinputform.h"
#include "ui_sclientinputform.h"
#include <Mainwindow>
#include <tabClients>
#include <SPermissions>
#include <SUserSettings>
#include <SComSettings>
#include <SClientModel>
#include <SSqlQueryModel>
#include <SPhone>
#include <SPhonesModel>

SClientInputForm::SClientInputForm(QWidget *parent) :
    SClientInputFormBase(parent),
    ui(new Ui::SClientInputForm)
{
    ui->setupUi(this);
    guiFontChanged();
    QWidget::setFocusProxy(ui->lineEditClientLastName);
    clientModel = new SClientModel();

    initWidgets();
    connect(userDbData, &SUserSettings::fontSizeChanged, this, &SClientInputForm::guiFontChanged);
}

SClientInputForm::~SClientInputForm()
{
    delete ui;
    delete clientModel;
}

void SClientInputForm::initWidgets()
{
    ui->pushButtonCreateTabClient->hide();  // кнопка открытия карточки клиента будет отображаться, только когда выбран клиент из базы
    ui->comboBoxClientAdType->setPlaceholderText(tr("источник обращения"));
    ui->comboBoxClientAdType->setButtons("Clear");

    setDefaultStyleSheets();

    // установка моделей данных
    ui->phones->setModel(clientModel->phones());
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    if(permissions->viewClients)
    {
        connect(ui->lineEditClientFirstName, &QLineEdit::textEdited, this, &SClientInputForm::firstNameChanged);
        connect(ui->lineEditClientLastName, &QLineEdit::textEdited, this, &SClientInputForm::lastNameChanged);
        connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillClientCreds(int)));
        connect(ui->phones,SIGNAL(primaryPhoneEdited(QString)),this,SLOT(primaryPhoneEdited(QString)));
    }

    connect(ui->pushButtonCreateTabClient, &QPushButton::clicked, this, &SClientInputForm::buttonCreateTabClientClicked);
    connect(ui->pushButtonSelectExistingClient, &QPushButton::clicked, this, &SClientInputForm::buttonSelectExistingClientClicked);
    connect(ui->checkBoxClientType, &QCheckBox::toggled, this, &SClientInputForm::changeClientType);
    connect(ui->pushButtonClientCredsClearAll, &QPushButton::clicked, this, &SClientInputForm::clearClientCreds);
    ui->pushButtonSelectExistingClient->setVisible(permissions->viewClients);
}

void SClientInputForm::changeClientType()
{
    if(ui->checkBoxClientType->checkState())
    {
        ui->groupBoxClient->setTitle(tr("Клиент (юридическое лицо)"));
        // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
        ui->lineEditClientFirstName->setPlaceholderText(tr("Название организации"));
        ui->lineEditClientLastName->hide();    // скрываем поле "Фамилия"
        ui->lineEditClientPatronymic->hide();    // скрываем поле "Отчество"
        ui->gridLayoutClient->addWidget(ui->lineEditClientFirstName, 1, 0, 1, 3); // заменяем поле "Фамилия" самим собой, но растягиваем его по ширине на 6 столбцов (занимаем столбцы полей "Имя" и "Отчество")
        ui->widgetClientMatch->setClientType(SClientMatch::NameSearchScope::Name);
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
        ui->widgetClientMatch->setClientType(SClientMatch::NameSearchScope::LastName);
    }
}

void SClientInputForm::lastNameChanged(const QString &text)
{
    if(ui->checkBoxClientType->checkState())
        return;

    ui->widgetClientMatch->findByName(text);
}

void SClientInputForm::firstNameChanged(const QString &text)
{
    if(!ui->checkBoxClientType->checkState())
        return;

    ui->widgetClientMatch->findByName(text);
}

void SClientInputForm::clearClientCreds(bool keepCoincidenceShown)
{
    setDefaultStyleSheets();
    m_client = 0;
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
    ui->pushButtonCreateTabClient->hide();
    ui->listWidgetClientOptions->clear();
    ui->widgetClientMatch->clear();

    if (!keepCoincidenceShown)
        ui->widgetClientMatch->clear();
}

void SClientInputForm::fillClientCreds(int id)
{
    clearClientCreds();    // очищаем данные клиента, но не прячем таблицу совпадений
    m_client = id;

    clientModel->load(m_client);
    ui->checkBoxClientType->setChecked(clientModel->isCompany());
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
//    if(permissions->viewClients)
        ui->phones->setModel(clientModel->phones());
//    else
//        ui->phones->setEnabled(false);
    ui->lineEditClientAddress->setText(clientModel->address());
    ui->lineEditClientEmail->setText(clientModel->email());
    ui->pushButtonCreateTabClient->setVisible(permissions->viewClients);
    ui->listWidgetClientOptions->addItems(clientModel->optionsList(SClientModel::OptionsOutputForm::Short));
    if(clientModel->isBalanceEnabled())
        ui->listWidgetClientOptions->addItems(QStringList("бал. "+sysLocale.toString(clientModel->balance(), 'f', 2)));

}

void SClientInputForm::fillCreds(int id)
{
    fillClientCreds(id);    // TODO: требуется переработать механизм выбора клиента на новый вариант, аналогичный разработанному на вкладке создания кассового ордера
}

int SClientInputForm::checkInput()
{
    int error = 0;
    setDefaultStyleSheets();

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
    if (ui->lineEditClientPatronymic->text() == "" && ui->checkBoxClientType->checkState() == 0 && comSettings->isClientPatronymicRequired() && !m_client)   // если не указано отчество и оно обязятельно (только для физ. лиц и только для новых клиентов)
    {
        ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheetRed);
        error = 7;
    }
    if (ui->comboBoxClientAdType->currentIndex() < 0 && comSettings->isVisitSourceRequired() && !m_client)        // если не указан источник обращения, а он обязателен и клиент новый
    {
        ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 8;
    }
    if (ui->lineEditClientAddress->text() == "" && comSettings->isClientAddressRequired() && !m_client)   // если не указан адрес, а он обязателен и клиент новый
    {
        ui->lineEditClientAddress->setStyleSheet(commonLineEditStyleSheetRed);
        error = 9;
    }
    if (ui->lineEditClientEmail->text() == "" && comSettings->isClientEmailRequired() && !m_client)   // если не указан email, а он обязателен и клиент новый
    {
        ui->lineEditClientEmail->setStyleSheet(commonLineEditStyleSheetRed);
        error = 10;
    }
    if (clientModel->isNew() && !ui->phones->isValid())
        error = 11;

    return error;
}

SClientModel *SClientInputForm::model()
{
    return clientModel;
}

void SClientInputForm::createClient()
{
    clientModel->set_firstName(ui->lineEditClientFirstName->text());
    clientModel->set_lastName(ui->lineEditClientLastName->text());
    clientModel->set_patronymicName(ui->lineEditClientPatronymic->text());
    clientModel->appendLogText(tr("Быстрое создание клиента из формы приёма в ремонт"));
    clientModel->setAdTypeIndex(ui->comboBoxClientAdType->currentIndex());
    clientModel->set_address(ui->lineEditClientAddress->text());
    clientModel->set_isCompany(ui->checkBoxClientType->isChecked());
    if(ui->checkBoxClientType->isChecked())
        clientModel->set_urName(ui->lineEditClientFirstName->text());
    clientModel->commit();

    m_client = clientModel->id();
}

void SClientInputForm::commit()
{
    if (clientModel->isNew() || clientModel->phones()->isUpdated())
    {
        if(clientModel->isNew())
            createClient();
        if(clientModel->phones()->isUpdated())
            clientModel->phones()->commit();
    }
}

void SClientInputForm::primaryPhoneEdited(QString number)
{
    ui->widgetClientMatch->findByPhone(number, ui->phones->primary()->maskIndex());
}

void SClientInputForm::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize());

    ui->lineEditClientAddress->setFont(font);
    ui->lineEditClientEmail->setFont(font);
    ui->lineEditClientFirstName->setFont(font);
    ui->lineEditClientLastName->setFont(font);
    ui->lineEditClientPatronymic->setFont(font);
    ui->comboBoxClientAdType->setFont(font);
}

void SClientInputForm::buttonCreateTabClientClicked()
{
    MainWindow *mw = MainWindow::getInstance();
    mw->createTabClient(m_client);
}

void SClientInputForm::buttonSelectExistingClientClicked()
{
    MainWindow *mw = MainWindow::getInstance();
    tabClients *clientsTab = nullptr;
    SWidget *parentTab = findParentTab();
    clientsTab = static_cast<tabClients *>(mw->createTabClients(parentTab));
    if(clientsTab)
    {
        QObject::connect(clientsTab, &tabClients::doubleClicked, this, [=](int id){fillClientCreds(id);});
        QObject::connect(clientsTab, &tabClients::activateCaller, mw, &MainWindow::reactivateCallerTab);
        ui->widgetClientMatch->clear();
        clientsTab->setTabTitle(i_clientsTabTitle);
        clientsTab->setCategory(i_clientsTabDefaultCategory);
        clientsTab->setCallerPtr(parentTab);
    }
}

#ifdef QT_DEBUG
void SClientInputForm::randomFill()
{
    int i;
//        fillClientCreds(33);
//        return;
    if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 или выбираем из уже имеющихся клиентов или создаём нового
    {
        fillClientCreds(QRandomGenerator::global()->bounded(randomClientIdFromDB()));
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
                ui->phones->forms().at(1)->testFill(2, clients4Test->value(i)->at(4));
            }
            else
            {
                ui->phones->forms().at(1)->testFill(0, clients4Test->value(i)->at(3));
            }
        }
        i = QRandomGenerator::global()->bounded(clientAdTypesList->rowCount()); // если клиент новый, то случайно выбираем источник обращения
        ui->comboBoxClientAdType->setCurrentIndex(i);

    }
}
#endif

void SClientInputForm::setDefaultStyleSheets()
{
    ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientFirstName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditClientAddress->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientEmail->setStyleSheet(commonLineEditStyleSheet);
}

void SClientInputForm::setMatchWidgetFilter(int category)
{
    ui->widgetClientMatch->setCategory(category);
}
