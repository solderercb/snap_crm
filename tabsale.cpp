#include "global.h"
#include "appver.h"
#include "tabsale.h"
#include "ui_tabsale.h"

QMap<int, tabSale*> tabSale::p_instance;

tabSale::tabSale(int doc, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabSale),
    doc_id(doc)
{
    userActivityLog->appendRecord("Navigation " + tabTitle());

    ui->setupUi(this);
    docModel = new SDocumentModel();
    tableModel = new SSaleTableModel(this);
//    itemDelagates = new SaleTableItemDelegates(ui->tableView);
    clientModel = new SClientModel();
    cashRegister = new SCashRegisterModel();

    tableModel->setTableMode(SSaleTableModel::TablesSet::StoreSale);
//    itemDelagates->setTableModel(tableModel);
    params = new int;
    *params = 0;
    *params |= comSettings->printOutInvoice?tabSaleSettingsMenu::PrintDoc:0;
    *params |= comSettings->printPKO?tabSaleSettingsMenu::PrintCheck:0;
    widgetAction = new tabSaleSettingsMenu(this);
    ui->buttonParams->addAction(widgetAction);
    widgetAction->setParamsModel(params);

    ui->comboBoxPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxMoneyBackAccount->setModel(paymentSystemsModel);
    initPriceColModel();
    ui->comboBoxPriceCol->setModel(m_priceColProxyModel);
    ui->comboBoxCompany->setModel(companiesModel);  // TODO: несколько компаний
    ui->comboBoxClientPhoneType->setModel(clientPhoneTypesModel);
    ui->comboBoxClientPhoneType->setModelColumn(0);
    ui->comboBoxClientPhoneType->setCurrentIndex(0);
    ui->widgetClientMatch->setPhoneMask(0);
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    ui->spinBoxReserve->setMinimum(1);
    ui->lineEditTrack->setButtons("Apply");
    ui->widgetClientMatch->hide();
    connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillClientCreds(int)));

    ui->tableView->setModel(tableModel);

//    connect(ui->lineEditAddByUID, SIGNAL(returnPressed()), this, SLOT(addItemByUID()));    // подключены в дизайнере
//    connect(ui->buttonReserve, SIGNAL(clicked()), this, SLOT(reserveButtonClicked()));    // подключены в дизайнере
//    connect(ui->buttonSale, SIGNAL(clicked()), this, SLOT(saleButtonClicked()));    // подключены в дизайнере
    connect(ui->comboBoxPriceCol, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPriceCol(int)));    // нужно подключать здесь, иначе возникает глюк с доступом к QMap<> fields
    connect(tableModel, SIGNAL(amountChanged(double, double, double)), this, SLOT(updateTotalSumms(double, double, double)));
    if(permissions->viewClients)
    {
        connect(ui->lineEditClientLastName,SIGNAL(textEdited(QString)),ui->widgetClientMatch,SLOT(findByLastname(QString)));
        connect(ui->comboBoxClientPhoneType,SIGNAL(currentIndexChanged(int)),ui->widgetClientMatch,SLOT(setPhoneMask(int)));
        connect(ui->lineEditClientPhone,SIGNAL(textEdited(QString)),this,SLOT(phoneNumberEdited(QString)));
    }


#ifdef QT_DEBUG
    createTestPanel();
#endif

    updateWidgets();
}

tabSale::~tabSale()
{
    delete ui;
    p_instance.remove(doc_id);   // Обязательно блять!
    delete params;
    delete widgetAction;
    delete tableModel;
//    delete itemDelagates;
    delete clientModel;
    delete cashRegister;
    delete docModel;
#ifdef QT_DEBUG
    delete testPushButton;
    delete testBtnAddRandomItem;
    delete testLineEdit;
    delete testPanel;
#endif
}

QString tabSale::tabTitle()
{
    if(doc_id)
        return tr("Расходная накладная %1").arg(doc_id);
    return tr("Продажа");
}

void tabSale::initPriceColModel()
{
    m_priceColProxyModel = new SSortFilterProxyModel();
    m_priceColProxyModel->setSourceModel(priceColModel);
    m_priceColProxyModel->setFilterRegularExpression(QRegularExpression("^(?!(" + QString::number(SStoreItemModel::PriceOptionWarranty) + ")).*$"));
    m_priceColProxyModel->setFilterKeyColumn(1);
}

void tabSale::setDefaultStyleSheets()
{
    ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientFirstName->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditClientPhone->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditTotal->setStyleSheet(commonLineEditStyleSheet);
    ui->lineEditTakeIn->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxClientPhoneType->setStyleSheet(commonLineEditStyleSheet);
    ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxMoneyBackAccount->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxPriceCol->setStyleSheet(commonComboBoxStyleSheet);
    ui->comboBoxCompany->setStyleSheet(commonComboBoxStyleSheet);
}

void tabSale::setBalanceWidgetsVisible(bool state)
{
    ui->labelBalance->setVisible(state);
    ui->lineEditBalance->setVisible(state);
    ui->checkBoxSaleInCredit->setVisible(state);
}

void tabSale::eventResize(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void tabSale::updateWidgets()
{
    QSqlQuery query;

    setDefaultStyleSheets();
    if(doc_id)
    {
        tableModel->setModelState(SSaleTableModel::StoreSold);
        docModel->load(doc_id);
        if(!docModel->isValid())
            return;
        m_docState = docModel->state();
        client = docModel->client();

        ui->labelDocNum->show();
        ui->lineEditDocNum->show();
        ui->lineEditDocNum->setText(QString::number(doc_id));
        ui->labelDate->show();
        ui->lineEditDate->show();
        ui->lineEditDate->setText(docModel->created());
        ui->comboBoxPaymentAccount->setCurrentIndex(docModel->paymentSystemIndex());
        ui->comboBoxPaymentAccount->setEnabled(false);
        ui->comboBoxMoneyBackAccount->setCurrentIndex(docModel->paymentSystemIndex());  // возврат на тот же счет, куда поступала оплата
        ui->comboBoxCompany->setCurrentIndex(docModel->companyIndex());
        ui->comboBoxCompany->setEnabled(false);
        ui->spinBoxReserve->setValue(docModel->reserveDays());
        ui->spinBoxReserve->setReadOnly(true);
        ui->lineEditTrack->setText(docModel->trackingNumber());
        ui->buttonLog->show();
        ui->buttonReserve->hide();   // не допускается повтоное нажатие "Резерв"
        if(client)
        {
            fillClientCreds(client);
        }
        else
            ui->checkBoxAnonymous->setChecked(true);

        ui->comboBoxPriceCol->setEnabled(false);
        ui->comboBoxPriceCol->setCurrentIndex(docModel->priceOptionIndex());
        ui->checkBoxAnonymous->setEnabled(false);
        ui->lineEditComment->setText(docModel->notes());
        ui->lineEditComment->setReadOnly(true);
        ui->buttonAdd->hide();
        ui->pushButtonClientCredsClearAll->setEnabled(false);
        ui->pushButtonClientFromDB->setEnabled(false);

        if(m_docState == SDocumentModel::ItemsReserved)
        {
            tableModel->setModelState(SSaleTableModel::StoreReserved);
            m_opType = SaleReserved;    // это флаг для функции sale()
            ui->comboBoxPaymentAccount->setEnabled(true);
            ui->comboBoxPriceCol->setEnabled(true);
            ui->lineEditTakeIn->setReadOnly(false);
            ui->lineEditAddByUID->setReadOnly(false);
            ui->labelReserve->hide();
            ui->spinBoxReserve->hide();
            ui->labelReserveProgress->show();
            ui->progressBarReserve->show();
            ui->progressBarReserve->setMaximum(docModel->reserveDays());
            ui->progressBarReserve->setMinimum(0);
            ui->progressBarReserve->setFormat("%v");
            ui->progressBarReserve->setValue( qMax( (int)(docModel->reserveDays() - docModel->createdUtc().daysTo(QDateTime::currentDateTimeUtc())), 0 ));
            ui->groupBoxAdm->hide();
            ui->labelTrack->hide();
            ui->lineEditTrack->hide();
            ui->buttonPrint->hide();
            ui->buttonReserveCancel->show();
            ui->buttonSaleMore->show();
            ui->buttonSale->show();
            tableModel->store_loadTable(doc_id);
#ifdef QT_DEBUG
            ui->lineEditTakeIn->setText(sysLocale.toString(sysLocale.toDouble(ui->lineEditTotal->text()) + QRandomGenerator::global()->bounded(100), 'f', 2)); // это для отладки
#else
            ui->lineEditTakeIn->setText(sysLocale.toString(0.00, 'f', 2));
#endif
        }
        else if(m_docState == SDocumentModel::ReserveCancelled || m_docState == SDocumentModel::OutInvoiceCancelled)   // если открыта РН снятого резерва или распроведённая РН
        {
            ui->lineEditTakeIn->setReadOnly(true);
            ui->lineEditAddByUID->setReadOnly(true);
            ui->labelReserve->hide();
            ui->spinBoxReserve->hide();
            ui->groupBoxAdm->hide();
            ui->labelReserveProgress->hide();
            ui->progressBarReserve->hide();
            ui->groupBoxAdm->hide();
            ui->buttonPrint->hide();
            ui->buttonReserveCancel->hide();
            ui->buttonSaleMore->hide();
            ui->buttonSale->hide();
            ui->labelTrack->hide();
            ui->lineEditTrack->hide();
            tableModel->setModelState(SSaleTableModel::StoreCancelled);
            tableModel->store_loadTable(doc_id);
            ui->lineEditTakeIn->setText(sysLocale.toString(0.00, 'f', 2));
        }
        else    // если открыта проведённая РН
        {
            ui->lineEditAddByUID->setReadOnly(true);
            ui->labelReserve->hide();
            ui->spinBoxReserve->hide();
            ui->labelReserveProgress->hide();
            ui->progressBarReserve->hide();
            ui->checkBoxSaleInCredit->setEnabled(false);
            ui->labelTrack->show();
            ui->lineEditTrack->show();
            ui->buttonPrint->show();            // будут отображаться только кнопки "Печать", "История" и "Параметры"
            ui->buttonReserveCancel->hide();
            ui->buttonSaleMore->hide();
            ui->buttonSale->hide();
            if(permissions->undoOutInvoice)
            {
                ui->groupBoxAdm->show();
            }
            else
            {
                ui->groupBoxAdm->hide();
            }
            tableModel->setModelState(SSaleTableModel::StoreSold);
            tableModel->store_loadTable(doc_id);
            ui->lineEditTotal->setText(docModel->amountLocal());  // устанавливать суммы нужно только после заполнения таблицы
            ui->lineEditTakeIn->setReadOnly(true);
            ui->lineEditTakeIn->setText(tableModel->amountTotalLocale());  // тут применю хитрость: поля будут заполняться из разных "источников" и, в случае возврата товаров, сравниваться
        }
    }
    else    // новая расходная накладная
    {
        clearAll();
        setBalanceWidgetsVisible(false);
        m_opType = Sale;    // это флаг для функции sale()
        ui->lineEditTakeIn->setReadOnly(false);
        ui->labelDate->hide();
        ui->lineEditDate->hide();
        ui->labelDocNum->hide();
        ui->lineEditDocNum->hide();
        ui->checkBoxAnonymous->setChecked(false);
        ui->checkBoxAnonymous->setEnabled(true);
        ui->groupBoxAdm->hide();
        ui->labelTrack->hide();
        ui->labelTrack->setText("");
        ui->textEditRevertReason->setText("");
        ui->lineEditTrack->hide();
        ui->labelReserve->show();
        ui->spinBoxReserve->show();
        ui->labelReserveProgress->hide();
        ui->progressBarReserve->hide();
        ui->comboBoxPaymentAccount->setCurrentText(paymentSystemsModel->getDisplayRole(0, 1));
        ui->comboBoxPaymentAccount->setEnabled(true);
        ui->comboBoxPriceCol->setCurrentText(priceColModel->getDisplayRole(2, 1));  // по умолчанию Розница
        ui->comboBoxPriceCol->setEnabled(true);
        ui->comboBoxCompany->setCurrentText(companiesModel->getDisplayRole(1, 1));  // TODO: несколько компаний
        ui->comboBoxCompany->setEnabled(true);
        ui->spinBoxReserve->setValue(comSettings->defaultItemReserveTime);
        ui->spinBoxReserve->setReadOnly(false);
        ui->lineEditAddByUID->setReadOnly(false);
        ui->buttonPrint->hide();            // будут отображаться только кнопки "Параметры", "Добавить", "Резерв", "Продать ещё" и "Продать"
        ui->buttonLog->hide();
        ui->buttonAdd->show();
        ui->buttonReserve->show();
        ui->buttonReserveCancel->hide();
        ui->buttonSaleMore->show();
        ui->buttonSale->show();
        ui->lineEditComment->setReadOnly(false);
        ui->pushButtonClientCredsClearAll->setEnabled(true);
        ui->pushButtonClientFromDB->setEnabled(permissions->viewClients);

        tableModel->setModelState(SSaleTableModel::StoreNew);

#ifdef QT_DEBUG
        test_scheduler_counter = 0;
        test_scheduler->start(200);
#endif
    }
    ui->widgetClientMatch->hide();
}

bool tabSale::checkInput()
{
    int error = 0;

    // TODO: часть кода взята из класса tabRepairNew; возможно стоит создать общую функцию для проверки введённых данных

    setDefaultStyleSheets();

    if( !ui->checkBoxAnonymous->isChecked() )
    {
        if (ui->lineEditClientLastName->text() == "")       // если не указана фамилия
        {
            ui->lineEditClientLastName->setStyleSheet(commonLineEditStyleSheetRed);
            error = 1;
        }
        if (ui->lineEditClientFirstName->text() == "")     // если не указано имя
        {
            ui->lineEditClientFirstName->setStyleSheet(commonLineEditStyleSheetRed);
            error = 2;
        }
        if (ui->lineEditClientPatronymic->text() == "" && comSettings->isClientPatronymicRequired && !client)   // если не указано отчество и оно обязятельно
        {
            ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheetRed);
            error = 3;
        }
        if (ui->comboBoxClientAdType->currentIndex() < 0 && comSettings->isVisitSourceRequired && !client)        // если не указан источник обращения, а он обязателен и клиент новый
        {
            ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheetRed);
            error = 4;
        }
        if (!ui->lineEditClientPhone->hasAcceptableInput() && comSettings->isClientPhoneRequired && clientModel->isNew())   // если не указан телефон и он обязятелен
        {
            ui->lineEditClientPhone->setStyleSheet(commonLineEditStyleSheetRed);
            error = 5;
        }
    }
    if( (paymentSystemsModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id") == -2) && \
            (!(clientModel->options()&SClientModel::BalanceEnabled) || ui->checkBoxAnonymous->isChecked()) )
    {
        ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 6;
    }

    if (sysLocale.toDouble(ui->lineEditTakeIn->text()) < sysLocale.toDouble(ui->lineEditTotal->text()) )   //
    {
        ui->lineEditTakeIn->setStyleSheet(commonLineEditStyleSheetRed);
        error = 7;
    }
    if (tableModel->rowCount() == 0)   //    в таблице нет строк
    {
        // будет слишкм стрёмно, если окрасить tableView в красный, поэтому просто ничего не делаем.
        error = 9;
    }
    if ( sysLocale.toDouble(ui->lineEditTotal->text()) <= 0 )
    {
        ui->lineEditTotal->setStyleSheet(commonLineEditStyleSheetRed);
        error = 10;
    }
    for(int i = 0; i < tableModel->rowCount(); i++) // TODO: Это нужно перенести в SSaleTableModel
    {
        if(!tableModel->value(i, SStoreItemModel::SaleOpColumns::ColState).toBool())   // только не помеченные на снятие резерва/возврат
        {
            if ( tableModel->value(i, SStoreItemModel::SaleOpColumns::ColPrice).toDouble() <= 0 ) // цена меньше нуля
            {
                tableModel->setData(tableModel->index(i, SStoreItemModel::SaleOpColumns::ColPrice), QColor(255, 209, 209), Qt::BackgroundRole);

                error = 11;
            }
            if( tableModel->value(i, SStoreItemModel::SaleOpColumns::ColPrice).toDouble() < tableModel->value(i, SStoreItemModel::SaleOpColumns::ColInPrice).toDouble() ) // цена меньше закупочной
            {
                QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                            tr("Цена ниже закупочной, продолжить?\n\"%1\"").arg(tableModel->value(i, SStoreItemModel::SaleOpColumns::ColName).toString()),
                                                                            QMessageBox::No | QMessageBox::Yes,
                                                                            QMessageBox::No);
                if (resBtn == QMessageBox::No)
                {
                    error = 12;
                }
            }
        }
    }
//    if ()   //
//    {
//        ui->->setStyleSheet(commonLineEditStyleSheetRed);
//        error = ;
//    }

    if (error)
    {
        qDebug() << "Ошибка создания РН: возможно, не все обязательные поля заполнены (error " << error << ")";
        return false;
    }

    return true;   // return 1 — OK, return 0 - ошибка
}

bool tabSale::createClient()
{
    bool nErr = 1;

    clientModel->setFirstName(ui->lineEditClientFirstName->text());
    clientModel->setLastName(ui->lineEditClientLastName->text());
    clientModel->setPatronymicName(ui->lineEditClientPatronymic->text());
    clientModel->appendLogText(tr("Быстрое создание клиента из формы продажи"));
    clientModel->setAdType(clientAdTypesList->databaseIDByRow(ui->comboBoxClientAdType->currentIndex()));
    if (ui->lineEditClientPhone->hasAcceptableInput())
    {
        clientModel->addPhone(ui->lineEditClientPhone->text(),
                              ui->comboBoxClientPhoneType->currentIndex());
    }
    nErr = clientModel->commit();
    client = clientModel->id();

    return nErr;
}

bool tabSale::createNewDoc()
{
    int reserveDays = 5, priceCol = 2;
    double amount, currency = 0.00 /* TODO: запрос валюты, даже если программа не перезапускалась несколько дней */;

    reserveDays = ui->spinBoxReserve->value();
    priceCol = priceColModel->index(ui->comboBoxPriceCol->currentIndex(), 1).data().toInt();
    amount = tableModel->amountTotal();

    docModel->setType(SDocumentModel::OutInvoice);
    docModel->setState(0);
    docModel->setClient(clientModel->id());
    docModel->setAmount(amount);
    docModel->setNotes(ui->lineEditComment->text());
    docModel->setPriceOption(priceCol);
    docModel->setReserveDays(reserveDays);

    return docModel->commit();
}

void tabSale::updateTotalSumms(const double amountTotal, const double, const double)
{
    double takeInSumm = sysLocale.toDouble(ui->lineEditTakeIn->text());
    ui->lineEditTotal->setText(sysLocale.toString(amountTotal, 'f', 2));
    if(takeInSumm >= amountTotal)
        ui->lineEditCharge->setText(sysLocale.toString(takeInSumm - amountTotal, 'f', 2));
    else
        ui->lineEditCharge->setText(sysLocale.toString(0.00, 'f', 2));
}

void tabSale::phoneNumberEdited(QString)
{
    if(permissions->viewClients)
        ui->widgetClientMatch->findByPhone(ui->lineEditClientPhone->displayText(), ui->comboBoxClientPhoneType->currentIndex());
}

void tabSale::clearClientCreds(bool hideCoincidence)
{
    setDefaultStyleSheets();
    clientModel->clear();
    client = 0;
    price_col = 2;
    ui->comboBoxPriceCol->setCurrentText(priceColModel->getDisplayRole(price_col, 1));
    ui->lineEditClientLastName->setReadOnly(false);     // разрешаем ввод текста (вдруг он был запрещён)
    ui->lineEditClientFirstName->setReadOnly(false);
    ui->lineEditClientPatronymic->setReadOnly(false);
    ui->lineEditClientPhone->setReadOnly(false);
    ui->pushButtonCreateTabClient->setEnabled(false);
    ui->pushButtonClientFromDB->setEnabled(permissions->viewClients);
    ui->pushButtonClientCredsClearAll->setEnabled(true);
    ui->lineEditClientLastName->clear();
    ui->lineEditClientFirstName->clear();
    ui->lineEditClientPatronymic->clear();
    ui->lineEditClientPhone->clear();
    ui->lineEditClientPhone->setReadOnly(false);
    ui->comboBoxClientPhoneType->setEnabled(true);
    ui->lineEditBalance->clear();
    setBalanceWidgetsVisible(false);
    ui->comboBoxClientPhoneType->setCurrentIndex(clientPhoneTypesModel->index(0, 0).row());     // устанавливаем первый элемент выпадающего списка
    ui->comboBoxClientAdType->setEnabled(true);
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    ui->checkBoxSaleInCredit->setChecked(false);
    if (hideCoincidence)
        ui->widgetClientMatch->hide();
}

void tabSale::fillClientCreds(int id)
{
    clearClientCreds(false);    // очищаем данные клиента, но не прячем таблицу совпадений
    client = id;                // установка нового значения должна выполняться после очистки
    clientModel->load(client);
    tableModel->setClient(client);
    cashRegister->setClient(client);
    ui->lineEditClientFirstName->setText(permissions->viewClients?clientModel->firstName():tr("no permissions"));
    ui->lineEditClientLastName->setText(permissions->viewClients?clientModel->lastName():tr("no permissions"));
    ui->lineEditClientPatronymic->setText(permissions->viewClients?clientModel->patronymicName():tr("no permissions"));
    ui->lineEditClientFirstName->setReadOnly(true);
    ui->lineEditClientLastName->setReadOnly(true);  // запрет на изменение, если клиент из базы
    ui->lineEditClientPatronymic->setReadOnly(true);

    ui->comboBoxClientPhoneType->setCurrentIndex(permissions->viewClients?clientModel->phones()->primary()->maskIndex():-1);
    ui->lineEditClientPhone->setText(permissions->viewClients?clientModel->phones()->primary()->phone():tr("no permissions"));
    ui->lineEditClientPhone->setReadOnly(true);
    ui->comboBoxClientPhoneType->setEnabled(false);

    ui->pushButtonCreateTabClient->setEnabled(permissions->viewClients);
    ui->comboBoxClientAdType->setEnabled(false);
    ui->checkBoxAnonymous->setChecked(false);

    if(tableModel->modelState() == SSaleTableModel::StoreNew)
        ui->comboBoxPriceCol->setCurrentIndex(clientModel->priceColumnIndex());
    ui->comboBoxClientAdType->setCurrentIndex(clientModel->adTypeIndex());
    if(clientModel->options() & SClientModel::BalanceEnabled)
    {
        setBalanceWidgetsVisible(true);
        ui->checkBoxSaleInCredit->setEnabled(true);
        ui->lineEditBalance->setText(sysLocale.toString(clientModel->balance(), 'f', 2));
    }
    else
        setBalanceWidgetsVisible(false);
}

void tabSale::buttonSelectExistingClientHandler()
{
    emit createTabSelectExistingClient(1, this);
}

void tabSale::buttonCreateTabClientHandler()
{
    emit createTabClient(client);
}

void tabSale::tableRowDoubleClick(QModelIndex tableIndex)
{
    emit createTabSparePart(tableModel->value(tableIndex.row(), SStoreItemModel::SaleOpColumns::ColItemId).toInt());
}

void tabSale::hideGroupBoxClient(bool isAnonymousBuyer)
{
    if(isAnonymousBuyer)
    {
        ui->groupBoxClient->hide();
        ui->buttonReserve->setEnabled(false);   // думаю нельзя резервировать товар на анонимного клиента
        cashRegister->unsetClient();
        tableModel->unsetClient();
        docModel->unsetClient();
    }
    else
    {
        ui->groupBoxClient->show();
        ui->buttonReserve->setEnabled(true);
        if(client)
        {
            cashRegister->setClient(client);
            tableModel->setClient(client);
            docModel->setClient(client);
        }
    }
}

void tabSale::selectPriceCol(int index)
{
    if(tableModel->modelState() == SSaleTableModel::State::StoreNew || tableModel->modelState() == SSaleTableModel::State::StoreReserved)
        tableModel->setPriceColumn((SStoreItemModel::PriceOption)m_priceColProxyModel->databaseIDByRow(index));
}

void tabSale::addItemByUID()
{
    tableModel->addItemByUID(ui->lineEditAddByUID->text().toInt());
    ui->lineEditAddByUID->setText("");
}

void tabSale::print()
{
    if(*params & tabSaleSettingsMenu::PrintDoc)     // печать РН
    {
        QMap<QString, QVariant> report_vars;
        report_vars.insert("type", Global::Reports::rn);
        report_vars.insert("doc_id", doc_id);
        emit generatePrintout(report_vars);
    }
    if(*params & tabSaleSettingsMenu::PrintCheck && cashRegister->lastInsertId())   // печать ПКО
    {
        QMap<QString, QVariant> report_vars;
        report_vars.insert("type", Global::Reports::pko);
        report_vars.insert("order_id", cashRegister->lastInsertId());
        emit generatePrintout(report_vars);
    }

    // TODO: печать чека с пом. РРО
}

bool tabSale::unSale()  // распроведение
{
    // TODO: нужно изучить вопрос возврата товара при условии использования РРО, возможно, для этого нужно создавать отдельный документ на возврат
    bool nErr = 1, balance = 0;
    QSqlQuery *query;
    int paymentAccount = 0;
    double amount;

    tableModel->setClient(clientModel->id());
    paymentAccount = paymentSystemsModel->databaseIDByRow(ui->comboBoxMoneyBackAccount->currentIndex(), "system_id");
    balance = (paymentAccount == -2);
    if( balance && (!(clientModel->options() & SClientModel::BalanceEnabled) || ui->checkBoxAnonymous->isChecked()) )
    {
        ui->comboBoxMoneyBackAccount->setStyleSheet(commonComboBoxStyleSheetRed);
        return true; // return 0 — OK, return 1 - ошибка
    }

    query = new QSqlQuery(QSqlDatabase::database("connThird"));

    QUERY_LOG_START(metaObject()->className());

    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);

        tableModel->unsaleItems(ui->textEditRevertReason->toPlainText());
        clientModel->updatePurchases(-tableModel->itemsAffected());

        // если в следствие (нескольких) частичных распроведений были возвращены все товары, то нужно изменить статус документа
        if(tableModel->amountTotal() == 0)
        {
            QStringList logText;
            logText << tr("Расходная накладная №%1 распроведена").arg(doc_id);
            QString reason = ui->textEditRevertReason->toPlainText();
            if(reason.length())
                logText << tr("основание: ", "основание распроведения РН") + reason;
            docModel->setAmount(0.00);
            docModel->setState(SDocumentModel::OutInvoiceCancelled);
            docModel->appendLogText(logText.join(", "));
        }
        else
            docModel->setAmount(tableModel->amountTotal());

        amount = sysLocale.toDouble(ui->lineEditCharge->text()); // в поле Сдача будет записана сумма товаров, помеченных на удаление (т. е. сумма возврата)

        docModel->commit();

        if(balance)
        {
            clientModel->updateBalance(amount, QString("Зачисление %1 - возврат за товары по РН №%2").arg(sysLocale.toCurrencyString(amount)).arg(doc_id), SBalanceLogRecordModel::RoyaltyReason::Document, doc_id);
        }
        else
        {
            cashRegister->setId(0); // обнулить Id, иначе будет произведён update, а не insert
            cashRegister->setOperationType(SCashRegisterModel::ExpGoods);
            cashRegister->setDocumentId(doc_id);
            cashRegister->setReason(QString("Расход денег в размере %2 - возврат за товары по РН №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)));
            cashRegister->commit(-amount);
        }

#ifdef QT_DEBUG
//    throw Global::ThrowType::Debug; // это временное для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }

    QUERY_LOG_STOP;

    delete query;

    if(nErr)
        return false;
    else
        return true; // return 0 — OK, return 1 - ошибка
}

void tabSale::clearAll()
{
    clearClientCreds(1);
    ui->checkBoxAnonymous->setChecked(false);
    ui->lineEditTotal->setText(sysLocale.toString(0.00, 'f', 2));
    ui->lineEditTakeIn->setText("");
    ui->lineEditComment->setText("");
    tableModel->removeRows(0, tableModel->rowCount());
    delete docModel;
    delete clientModel;
    docModel = new SDocumentModel();
    clientModel = new SClientModel();

}

bool tabSale::sale()
{
    if(!checkInput())
        return true; // return 0 — OK, return 1 - ошибка

    bool nErr = 1, balance = 0, isAnonBuyer = 0;
    int paymentAccount = 0, initial_doc_id = doc_id;
    double amount;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));

    amount = tableModel->amountTotal();

    QUERY_LOG_START(metaObject()->className());

    isAnonBuyer = ui->checkBoxAnonymous->isChecked();

    // в этом месте у АСЦ недоработка: не зависимо от установки галочки "с баланса", код `payment_system` в табл. docs записывается соответствующий выбранному счету в комбобоксе
    if(ui->checkBoxSaleInCredit->isChecked())
    {
        paymentAccount = -2;
        balance = 1;
    }
    else
    {
        paymentAccount = paymentSystemsModel->databaseIDByRow(ui->comboBoxPaymentAccount->currentIndex(), "system_id");
        balance = paymentAccount == -2;
    }

    try
    {
        if(!isAnonBuyer)
        {
            if (clientModel->isNew())
            {
                QUERY_EXEC(query,nErr)(QUERY_BEGIN);
                nErr = createClient();
                QUERY_COMMIT_ROLLBACK(query,nErr);
                fillClientCreds(client);
            }
        }

        tableModel->setClient(client);

        QUERY_EXEC(query,nErr)(QUERY_BEGIN);

        if(balance)
            if(!clientModel->balanceEnough(-amount))
                throw Global::ThrowType::UserCanceled;

        if(m_opType == Sale || m_opType == Reserve)
        {
            createNewDoc();
            doc_id = docModel->id();
            tableModel->setDocumentId(doc_id);
        }

        docModel->setPaymentSystem(paymentAccount);

        if(m_opType == Reserve)
        {
            tableModel->store_saveTables(SSaleTableModel::StoreOpType::Reserve);
            docModel->setType(SDocumentModel::ReserveInvoice);
            docModel->appendLogText(QString("Расходная накладная №%1 создана (резерв)").arg(doc_id));
            docModel->setState(SDocumentModel::ItemsReserved);
        }
        else
        {
            tableModel->store_saveTables(SSaleTableModel::StoreOpType::Sale);
            if(m_opType == SaleReserved)
            {
                docModel->setAmount(amount);
                docModel->appendLogText(QString("Расходная накладная №%1 проведена").arg(doc_id));
                docModel->setType(SDocumentModel::OutInvoice);
                docModel->setState(SDocumentModel::OutInvoicePayed);
            }
            else                // простая продажа
            {
                docModel->appendLogText(QString("Расходная накладная №%1 создана").arg(doc_id));
                docModel->setState(SDocumentModel::OutInvoicePayed);
            }

            if(balance)   // если продажа в долг
            {
                clientModel->updateBalance(-amount, QString("Списание %1 за товары по РН №%2").arg(sysLocale.toCurrencyString(amount)).arg(doc_id), SBalanceLogRecordModel::RoyaltyReason::Document, doc_id);
            }
            else
            {
                cashRegister->setId(0); // обнулить Id, иначе будет произведён update, а не insert
                cashRegister->setClient(clientModel->id());
                cashRegister->setOperationType(SCashRegisterModel::RecptGoods);
                cashRegister->setDocumentId(doc_id);
                cashRegister->setReason(QString("Поступление денег в размере %2 по расходной накладной №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)));
                cashRegister->commit(amount);
            }

            clientModel->updatePurchases(tableModel->itemsAffected());
        }
        docModel->commit();

#ifdef QT_DEBUG
//        throw Global::ThrowType::Debug; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif

        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if(m_opType == Sale || m_opType == Reserve)
        {
            doc_id = initial_doc_id;
            docModel->setId(0);
        }
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }

#ifdef QT_DEBUG
//    nErr = 1; // и это для отладки (чтобы проверить работу дальше)
#endif

    QUERY_LOG_STOP;

    delete query;

    if(nErr)
    {
        p_instance.remove(initial_doc_id);   // Если всё ОК, то нужно заменить указатель
        p_instance.insert(doc_id, this);    // иначе будет падать при попытке создать новую вкладку продажи

        emit updateTabTitle(this);
        print();
        return false;
    }
    else
    {
        return true; // return 0 — OK, return 1 - ошибка
    }
}

void tabSale::saleButtonClicked()
{
    if (sale())
        return;

    updateWidgets();
}

/* На вкладке продажа задумка с кнопками "Продать ещё" и "Продать" несколько другая, чем аналогичные кнопки
 * на вкладке приёма в ремонт: при нажатии "Продать" производится продажа, вкладка остаётся открытой (это для
 * того, чтобы пользователь мог сначала набросать список товаров, а потом собрать товары и выдать покупателю);
 * нажатие "Продать ещё" приведёт к проводке накладной и очистке списка товаров и данных клиента (этот режим
 * будет удобен при большом кол-ве продаж — не придётся вызывать вкладку продажи вручную).
 */
void tabSale::saleMoreButtonClicked()
{
    if(sale())
        return;

    shortlivedNotification *newPopup = new shortlivedNotification(this, "Успешно", "Расходная накладная проведена", QColor(214,239,220), QColor(229,245,234));
    doc_id = 0;
    m_opType = Sale;
    updateWidgets();
}

/* При нажатии "Резерв" вызывается тот же метод, что и при продаже; для изменения набора запросов к БД используется
 * флаг m_opType.
 */
void tabSale::reserveButtonClicked()
{
    m_opType = Reserve;

    if(sale())
        return;

    updateWidgets();
}

void tabSale::addItemButtonClicked()
{
    emit createTabSelectItem(1, this);
}

void tabSale::paramsButtonClicked()
{

}

void tabSale::updateChargeAmount(QString text)
{
    // TODO: применить стиль по умолчанию, если поле было красным
    double takeInSumm = sysLocale.toDouble(text);
    double amount = sysLocale.toDouble(ui->lineEditTotal->text());
    if(amount <= takeInSumm )
        ui->lineEditCharge->setText(sysLocale.toString(takeInSumm - amount, 'f', 2));
    else
        ui->lineEditCharge->setText(sysLocale.toString(0.00, 'f', 2));

}

void tabSale::comboBoxIndexChanged(int)
{
}

void tabSale::phoneTypeChanged(int index)
{
    ui->lineEditClientPhone->setInputMask("");
    ui->lineEditClientPhone->setInputMask(clientPhoneTypesModel->index(index, 2).data().toString() + ";_");  // Here ";_" for filling blank characters with underscore
}

void tabSale::setTrackNum()     // слот вызываемый по нажатию Return
{
    bool nErr = 1;
    QSqlQuery *query;

    if( QString::compare(ui->lineEditTrack->text(), docModel->trackingNumber()) == 0 )
        return;

    query = new QSqlQuery(QSqlDatabase::database("connThird"));

    QUERY_EXEC(query,nErr)(QUERY_BEGIN);
    docModel->setTrackingNumber(ui->lineEditTrack->text());
    nErr = docModel->commit();
    QUERY_COMMIT_ROLLBACK(query,nErr);
    shortlivedNotification *newPopup;
    if(nErr)
        newPopup = new shortlivedNotification(this, "Успешно", "ТТН задана", QColor(214,239,220), QColor(229,245,234));
    else
        newPopup = new shortlivedNotification(this, "Неудача", "Ошибка установки ТТН", QColor(255,199,173), QColor(255,164,119));
    delete query;
}

void tabSale::setTrackNum(int)
{
    setTrackNum();
}

void tabSale::reserveCancelButtonClicked()
{
    if(m_docState != SDocumentModel::ItemsReserved)   // проверка на всякий случай, вдруг накосячу с отображением кнопок в разных режимах
        return;

    bool nErr = 1;
    QSqlQuery *query;

    query = new QSqlQuery(QSqlDatabase::database("connThird"));

    QUERY_LOG_START(metaObject()->className());

    QUERY_EXEC(query,nErr)(QUERY_BEGIN);
    try
    {
        nErr = tableModel->freeItems();


        // если в следствие (нескольких) частичных распроведений были возвращены все товары, то нужно изменить статус документа
        if(tableModel->amountTotal() == 0)
        {
            docModel->setAmount(0.00);
            docModel->setState(SDocumentModel::ReserveCancelled);
            docModel->appendLogText(tr("Резерв товара по РН №%1 снят").arg(doc_id));
        }
        else
            docModel->setAmount(tableModel->amountTotal());

        docModel->commit();

#ifdef QT_DEBUG
//    throw Global::ThrowType::Debug; // это временное для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }

    QUERY_LOG_STOP;

    updateWidgets();
    delete query;
}

void tabSale::printButtonClicked()
{

}

void tabSale::logButtonClicked()
{

}

void tabSale::unSaleButtonClicked()
{
    if(m_docState != SDocumentModel::OutInvoicePayed)   // проверка на всякий случай, вдруг накосячу с отображением кнопок в разных режимах
        return;

    QMessageBox resBtn( QMessageBox::Question, "SNAP CRM",
                        tr("Распровести расходную накладную и вернуть средства покупателю?"),
                        QMessageBox::No | QMessageBox::Yes);
    QAbstractButton *noButton = resBtn.button(QMessageBox::No);
    resBtn.setDefaultButton(QMessageBox::No);
    resBtn.setWindowModality(Qt::ApplicationModal);
    resBtn.exec();
    if (resBtn.clickedButton() == noButton)
        return;

    if(unSale())
        return;
    updateWidgets();
}

void tabSale::paymentSystemChanged(int index)
{
    int sysId = paymentSystemsModel->databaseIDByRow(index, "system_id");
    cashRegister->setSystemId(sysId);
}

tabSale* tabSale::getInstance(int doc_id, MainWindow *parent)
{
    if( !p_instance.contains(doc_id) )
      p_instance.insert(doc_id, new tabSale(doc_id, parent));
    return p_instance.value(doc_id);
}

/******************************************************************************************************************************************************
 *
 */
sparePartsTable::sparePartsTable(QWidget *parent) :
    STableViewBase(SLocalSettings::SaleItemsGrid, parent)
{
    i_defaultColumnsWidths = {{0, 60},{1, 90},{2, 270},{3, 45},{4, 60},{5, 70},{6, 70},{7, 120},{8, 120},{9, 80}};
    i_defaultHeaderLabels << tr("") << tr("UID") << tr("Наименование") << tr("Кол-во") << tr("Доступно") << tr("Цена") << tr("Сумма") << tr("Место") << tr("Серийный номер") << tr("Гарантия");
    readLayout();
    i_gridLayout->$GridControl.Columns[2].Width_marked = true;  // по умолчанию автоширина столбца с наименованием
}

sparePartsTable::~sparePartsTable()
{
}

void sparePartsTable::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SSaleTableModel*>(model);
    STableViewBase::setModel(model);
    m_itemDelagates = new SaleTableItemDelegates(m_model, this);
    setItemDelegate(m_itemDelagates);
}

void sparePartsTable::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    int row = currentIndex().row();
    if(m_model->index(row, SStoreItemModel::SaleOpColumns::ColRecordType).data().toBool())
    {
        clearSelection();
        selectionModel()->select(currentIndex(), QItemSelectionModel::Select);
        emit createTabSparePart(m_model->index(row, SStoreItemModel::SaleOpColumns::ColItemId).data().toInt());
    }
}

#ifdef QT_DEBUG
void tabSale::randomFill()
{
    int i;

    if (test_scheduler_counter == 1)   // клиент
    {
        int val = QRandomGenerator::global()->bounded(100);
        if (val > 66)  // или выбираем из уже имеющихся клиентов или создаём нового или включаем галочку анонимный
        {
//            fillClientCreds(143);
            fillClientCreds(QRandomGenerator::global()->bounded(7538)); // пытаемся заполнить данные уже имеющимся клиентом
            if (ui->lineEditClientLastName->text() == "")
                fillClientCreds(QRandomGenerator::global()->bounded(7538)); // если попался id несуществующего клиета
            else if (ui->lineEditClientLastName->text() == "")
                fillClientCreds(QRandomGenerator::global()->bounded(7538)); // и еще раз, на всякий пожарный
        }
        else if (val > 33)    // ввод данных из тестового списка
        {
            i = QRandomGenerator::global()->bounded(clients4Test->size());
            ui->lineEditClientLastName->setText(clients4Test->value(i)->at(0));
            ui->lineEditClientFirstName->setText(clients4Test->value(i)->at(1));
            ui->lineEditClientPatronymic->setText(clients4Test->value(i)->at(2));
            if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50 или телефон мобильный или городской (это для проверки масок)
            {
                ui->comboBoxClientPhoneType->setCurrentIndex(1);
                ui->lineEditClientPhone->setText(clients4Test->value(i)->at(4));
            }
            else
            {
                ui->comboBoxClientPhoneType->setCurrentIndex(0);
                ui->lineEditClientPhone->setText(clients4Test->value(i)->at(3));
            }

            i = QRandomGenerator::global()->bounded(clientAdTypesList->rowCount()); // если клиент новый, то случайно выбираем источник обращения
            ui->comboBoxClientAdType->setCurrentIndex(i);
        }
        else
            ui->checkBoxAnonymous->setChecked(true);
    }
    else if (test_scheduler_counter == 2)   // товары
    {
        tableModel->dbgAddRandomItem();
    }
    else if (test_scheduler_counter == 3)   // продажа в долг или с оплатой
    {
    {
        if(ui->checkBoxSaleInCredit->isVisible())
            if (QRandomGenerator::global()->bounded(100) > 50)  // 50/50
                ui->checkBoxSaleInCredit->setChecked(true);
            }
    }
    else if (test_scheduler_counter == 4)   // поле полученной суммы
    {
        ui->lineEditTakeIn->setText(sysLocale.toString(sysLocale.toDouble(ui->lineEditTotal->text()) + QRandomGenerator::global()->bounded(100), 'f', 2));
    }
    else if (test_scheduler_counter == 5)  // платёжная система
    {
        ui->comboBoxPaymentAccount->setCurrentIndex(QRandomGenerator::global()->bounded(paymentSystemsModel->rowCount()));
    }
    else if (test_scheduler_counter == 6)   // тип цен
    {
        ui->comboBoxPriceCol->setCurrentIndex(QRandomGenerator::global()->bounded(priceColModel->rowCount()));
    }
    else if (test_scheduler_counter == 11)
    {
//        saleButtonClicked();
//        main_window_test_scheduler2->start(500);    // запускаем таймер закрытия вкладки приёма в ремонт
    }
    if (test_scheduler_counter < 11)
    {
        test_scheduler_counter++;
        test_scheduler->start(400);    //  (пере-)запускаем таймер
    }
//    qDebug() << "test_scheduler_handler(), test_scheduler_counter = " << test_scheduler_counter++;

}

void tabSale::createTestPanel()
{
    testPanel = new QWidget(this, Qt::SplashScreen);
    testPanel->setGeometry(0,0,190, 60);
    testLineEdit = new QLineEdit(testPanel);
    testLineEdit->setGeometry(5,5, 90, 20);
    testPushButton = new QPushButton(testPanel);
    testPushButton->setGeometry(5,30, 90, 20);
    testPushButton->setText("updateWidgets()");
//    connect(testPushButton, &QPushButton::clicked, this, &tabSale::test_updateWidgetsWithDocNum);
    connect(testPushButton, SIGNAL(clicked()), this, SLOT(test_updateWidgetsWithDocNum()));
    testBtnAddRandomItem = new QPushButton(testPanel);
    testBtnAddRandomItem->setGeometry(95,30, 90, 20);
    testBtnAddRandomItem->setText("AddRandomItem");
    connect(testBtnAddRandomItem, &QPushButton::clicked, tableModel, &SSaleTableModel::dbgAddRandomItem);
    testPanel->show();
}

void tabSale::test_scheduler_handler()  //
{
    if(tableModel->modelState() == 0)
        randomFill();
}

void tabSale::test_scheduler2_handler()  // обработик таймера закрытия вкладки
{
//    closeTab(ui->tabWidget->indexOf(tabClients::getInstance(0)));
    //    test_scheduler->start(1000);    //  перезапускаем таймер открытия вкладки
}

void tabSale::test_updateWidgetsWithDocNum()
{
    p_instance.remove(doc_id);   // заменить указатель
    doc_id = testLineEdit->text().toInt();
    p_instance.insert(doc_id, this);    // иначе будет падать при попытке создать новую вкладку продажи
    emit updateTabTitle(this);
    updateWidgets();
}

#endif
