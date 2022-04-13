#include "global.h"
#include "appver.h"
#include "tabsale.h"
#include "ui_tabsale.h"

QMap<int, tabSale*> tabSale::p_instance;

groupBoxEventFilter3::groupBoxEventFilter3(QObject *parent) :
    QObject(parent)
{
}

bool groupBoxEventFilter3::eventFilter(QObject *watched, QEvent *event)  // TODO: вынести в отдельные файлы
{
//    qDebug() << watched->objectName() << ": viewEventFilter: " << event;

    // когда указатель находится на заголовке (по всей ширине groupBox'а), устанавливаем курсор в виде руки с указательным пальцем
    // TODO: проверить сколько эта метода жрёт ресурсов
    QGroupBox *groupBox = static_cast<QGroupBox*>(watched);
    if (event->type() == QEvent::HoverMove)
    {
        QPoint point = static_cast<QHoverEvent*>(event)->position().toPoint();
        if (point.x() > 0 && point.x() < groupBox->width() && point.y() >0 && point.y() < 20)
            groupBox->setCursor(Qt::PointingHandCursor);
        else
            groupBox->unsetCursor();
    }

    // Нажатие левой кнопкой мыши на заголовке groupBox'а скрывает/показывает таблицу (при скрытии groupBox сжимается по вертикали)
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseButtonPress = static_cast<QMouseEvent*>(event);

        if (mouseButtonPress->button() == Qt::LeftButton)
        {
            if (mouseButtonPress->position().toPoint().x() > 0 && mouseButtonPress->position().toPoint().x() < groupBox->width() && mouseButtonPress->position().toPoint().y() >0 && mouseButtonPress->position().toPoint().y() < 20)
            {
                QTableView *table = groupBox->findChild<QTableView*>();
                table->setVisible(!table->isVisible());
            }
        }
    }
    return false;
}

tabSale::tabSale(int doc, MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabSale),
    doc_id(doc)
{
    ui->setupUi(this);
    clientsMatchTable = new QSqlQueryModel();       // таблица совпадения клиента (по номеру тел. или по фамилии)
    tableModel = new SaleTableModel(this);
    newItemModel = new QSqlQueryModel(this);
    itemDelagates = new SaleTableItemDelegates(tableModel, ui->tableView);
    clientModel = new SSqlQueryModel();
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;

    groupBoxEventFilterObj = new groupBoxEventFilter3(this);
    ui->groupBoxClientCoincidence->installEventFilter(groupBoxEventFilterObj);
    ui->comboBoxPaymentAccount->setModel(paymentSystemsModel);
    ui->comboBoxMoneyBackAccount->setModel(paymentSystemsModel);
    ui->comboBoxPriceCol->setModel(priceColModel);
    ui->comboBoxCompany->setModel(companiesModel);  // TODO: несколько компаний
    ui->comboBoxClientPhoneType->setModel(clientPhoneTypesModel);
    ui->comboBoxClientPhoneType->setModelColumn(0);
    ui->comboBoxClientPhoneType->setCurrentIndex(0);
    ui->comboBoxClientAdType->setModel(clientAdTypesList);
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    ui->spinBoxReserve->setMinimum(1);

    query->exec(QUERY_BEGIN);
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_ACTIVITY(userData->value("id").toString()));

    if(doc_id)
    {
        QUERY_EXEC(query, nDBErr)(QUERY_INS_USER_ACTIVITY(QString("Navigation РН№%1").arg(doc_id)));
    }
    else
    {
        QUERY_EXEC(query, nDBErr)(QUERY_INS_USER_ACTIVITY(QString("Navigation Продажа")));
    }

    QUERY_COMMIT_ROLLBACK(query, nDBErr);

    ui->tableView->setModel(tableModel);
    ui->tableView->setItemDelegateForColumn(0, itemDelagates);
    ui->tableView->setItemDelegate(itemDelagates);

//    connect(ui->lineEditAddByUID, SIGNAL(returnPressed()), this, SLOT(addItemByUID()));    // подключены в дизайнере
//    connect(ui->buttonReserve, SIGNAL(clicked()), this, SLOT(reserveButtonClicked()));    // подключены в дизайнере
//    connect(ui->buttonSale, SIGNAL(clicked()), this, SLOT(saleButtonClicked()));    // подключены в дизайнере
    connect(ui->comboBoxPriceCol, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPriceCol(int)));    // нужно подключать здесь, иначе возникает глюк с доступом к QMap<> fields
    connect(tableModel, SIGNAL(amountChanged(float)), this, SLOT(updateTotalSumms(float)));


#ifdef QT_DEBUG
    testPanel = new QWidget(this, Qt::SplashScreen);
    testPanel->setGeometry(0,0,100, 65);
    testLineEdit = new QLineEdit(testPanel);
    testLineEdit->setGeometry(5,5, 90, 25);
    testPushButton = new QPushButton(testPanel);
    testPushButton->setGeometry(5,35, 90, 25);
    testPushButton->setText("updateWidgets()");
    connect(testPushButton, &QPushButton::clicked, this, &tabSale::test_updateWidgetsWithDocNum);
    testPanel->show();

    test_scheduler = new QTimer();
    test_scheduler->setSingleShot(true);
    test_scheduler2 = new QTimer();
    test_scheduler2->setSingleShot(true);
    QObject::connect(test_scheduler, SIGNAL(timeout()), this, SLOT(test_scheduler_handler()));
    QObject::connect(test_scheduler2, SIGNAL(timeout()), this, SLOT(test_scheduler2_handler()));
#endif

    updateWidgets();

    delete query;
}

tabSale::~tabSale()
{
    delete ui;
    p_instance.remove(doc_id);   // Обязательно блять!
    delete groupBoxEventFilterObj;
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

void tabSale::updateDocData()
{
    docModel = new QSqlQueryModel();
    docModel->setQuery(QUERY_SEL_DOC(doc_id), QSqlDatabase::database("connMain"));
}

void tabSale::updateWidgets()
{
    QSqlQuery query;

    setDefaultStyleSheets();
    setBalanceWidgetsVisible(false);    // по дефолту не видимы
    if(doc_id)
    {
        updateDocData();
        ui->labelDocNum->show();
        ui->lineEditDocNum->show();
        ui->lineEditDocNum->setText(QString::number(doc_id));
        ui->labelDate->show();
        ui->lineEditDate->show();
        QDateTime date = docModel->record(0).value("created").toDateTime();
        date.setTimeZone(QTimeZone::utc());
        ui->lineEditDate->setText(date.toLocalTime().toString("dd.MM.yyyy hh:mm:ss"));
        ui->comboBoxPaymentAccount->setCurrentText(paymentSystemsModel->getDisplayRole(docModel->record(0).value("payment_system").toInt(), 1));
        ui->comboBoxPaymentAccount->setEnabled(false);
        ui->comboBoxMoneyBackAccount->setCurrentText(paymentSystemsModel->getDisplayRole(docModel->record(0).value("payment_system").toInt(), 1));  // возврат на тот же счет, куда поступала оплата
        ui->comboBoxCompany->setCurrentText(companiesModel->getDisplayRole(docModel->record(0).value("company").toInt(), 1));
        ui->comboBoxCompany->setEnabled(false);
        ui->spinBoxReserve->setValue(docModel->record(0).value("reserve_days").toInt());
        ui->spinBoxReserve->setReadOnly(true);
        ui->lineEditTrack->setText(docModel->record(0).value("track").toString());
        client = docModel->record(0).value("dealer").toInt();
        ui->groupBoxClientCoincidence->hide();
        ui->buttonLog->show();
        ui->buttonReserve->hide();   // не допускается повтоное нажатие "Резерв"
        ui->lineEditAddByUID->setReadOnly(true);    // пока что нельзя будет добавить товары в РН резерва, может позже добавлю такую возможность
        if(client)
        {
            fillClientCreds(client);
            ui->lineEditClientPhone->setReadOnly(true);
            ui->comboBoxClientPhoneType->setEnabled(false);
            ui->comboBoxClientAdType->setEnabled(false);
            ui->pushButtonClientFromDB->setEnabled(false);
            ui->pushButtonClientCredsClearAll->setEnabled(false);
            if(!permissions->value("Х"))     // TODO: в АСЦ чот не нашел разрешения на просмотр карточки клиента, может плохо смотрел
                ui->pushButtonCreateTabClient->setEnabled(false);
            ui->checkBoxAnonymous->setChecked(false);
        }
        else
            ui->checkBoxAnonymous->setChecked(true);

        ui->comboBoxPriceCol->setCurrentText(priceColModel->getDisplayRole(docModel->record(0).value("price_option").toInt(), 1));  // это должно быть после fillClientCreds()
        ui->comboBoxPriceCol->setEnabled(false);
        ui->checkBoxAnonymous->setEnabled(false);
        ui->lineEditComment->setText(docModel->record(0).value("notes").toString());
        ui->lineEditComment->setReadOnly(true);
        ui->buttonAdd->hide();
        if(docModel->record(0).value("state").toInt() == 3)   // если открыта РН резерва
        {
            reserve = 2;    // это флаг для функции sale()
            ui->lineEditTakeIn->setReadOnly(false);
            ui->labelReserve->hide();
            ui->spinBoxReserve->hide();
            ui->labelReserveProgress->show();
            ui->progressBarReserve->show();
            ui->progressBarReserve->setMaximum(docModel->record(0).value("reserve_days").toInt());
            ui->progressBarReserve->setMinimum(0);
            ui->progressBarReserve->setFormat("%v");
            ui->progressBarReserve->setValue( qMax( (docModel->record(0).value("reserve_days").toInt() - docModel->record(0).value("diff_days").toInt()), 0 ));
            ui->groupBoxAdm->hide();
            ui->labelTrack->hide();
            ui->lineEditTrack->hide();
            ui->buttonPrint->hide();
            ui->buttonReserveCancel->show();
            ui->buttonSaleMore->show();
            ui->buttonSale->show();
            tableModel->setQuery(QUERY_SEL_ITEMS_IN_DOC_RSRV(doc_id), QSqlDatabase::database("connMain"));
            tableModel->setModelState(2);
            ui->lineEditTakeIn->setText(sysLocale.toString(0.00, 'f', 2));
        }
        else if(docModel->record(0).value("state").toInt() == 4 || docModel->record(0).value("state").toInt() == 7)   // если открыта РН снятого резерва или распроведённая РН
        {
            ui->lineEditTakeIn->setReadOnly(true);
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
            tableModel->setQuery(QUERY_SEL_ITEMS_IN_DOC_RSRV(doc_id), QSqlDatabase::database("connMain"));
            tableModel->setModelState(3);
            ui->lineEditTakeIn->setText(sysLocale.toString(0.00, 'f', 2));
        }
        else    // если открыта проведённая РН
        {
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
            if(permissions->value("56"))
            {   // удаление строк в проведённой РН будет доступно только для привелегированных пользователей (частичное рапроведение)
                ui->groupBoxAdm->show();
            }
            else
            {
                ui->groupBoxAdm->hide();
            }
            tableModel->setQuery(QUERY_SEL_ITEMS_IN_DOC(doc_id), QSqlDatabase::database("connMain"));
            tableModel->setModelState(1);
            ui->lineEditTotal->setText(sysLocale.toString(docModel->record(0).value("total").toFloat(), 'f', 2));  // устанавливать суммы нужно только после заполнения таблицы
            ui->lineEditTakeIn->setReadOnly(true);
            ui->lineEditTakeIn->setText(sysLocale.toString(tableModel->totalAmount(), 'f', 2));  // тут применю хитрость: поля будут заполняться из разных "источников" и, в случае возврата товаров, сравниваться
        }


    }
    else    // новая расходная накладная
    {
        clearAll();
        reserve = 0;    // это флаг для функции sale()
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
        ui->groupBoxClientCoincidence->hide();
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
        ui->spinBoxReserve->setValue(comSettings->value("default_reserve_days").toInt());
        ui->spinBoxReserve->setReadOnly(false);
        ui->lineEditAddByUID->setReadOnly(false);
        ui->buttonPrint->hide();            // будут отображаться только кнопки "Параметры", "Добавить", "Резерв", "Продать ещё" и "Продать"
        ui->buttonLog->hide();
        ui->buttonAdd->show();
        ui->buttonReserve->show();
        ui->buttonReserveCancel->hide();
        ui->buttonSaleMore->show();
        ui->buttonSale->show();

        tableModel->setModelState(0);

#ifdef QT_DEBUG
        test_scheduler_counter = 0;
        test_scheduler->start(200);
#endif
    }
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
        if (ui->lineEditClientPatronymic->text() == "" && comSettings->value("is_patronymic_required").toBool() && !client)   // если не указано отчество и оно обязятельно
        {
            ui->lineEditClientPatronymic->setStyleSheet(commonLineEditStyleSheetRed);
            error = 3;
        }
        if (ui->comboBoxClientAdType->currentIndex() < 0 && comSettings->value("visit_source_force").toBool() && !client)        // если не указан источник обращения, а он обязателен и клиент новый
        {
            ui->comboBoxClientAdType->setStyleSheet(commonComboBoxStyleSheetRed);
            error = 4;
        }
        if (!ui->lineEditClientPhone->hasAcceptableInput() && comSettings->value("phone_required").toBool())   // если не указан телефон и он обязятелен
        {
            ui->lineEditClientPhone->setStyleSheet(commonLineEditStyleSheetRed);
            error = 5;
        }
    }
    if( paymentSystemsModel->index(ui->comboBoxPaymentAccount->currentIndex(), 1).data().toInt() == -2 && (!clientModel->record(0).value("balance_enable").toBool() || ui->checkBoxAnonymous->isChecked()) )
    {
        ui->comboBoxPaymentAccount->setStyleSheet(commonComboBoxStyleSheetRed);
        error = 6;
    }

    if (sysLocale.toFloat(ui->lineEditTakeIn->text()) < sysLocale.toFloat(ui->lineEditTotal->text()) )   //
    {
        ui->lineEditTakeIn->setStyleSheet(commonLineEditStyleSheetRed);
        error = 7;
    }
    if( ui->checkBoxSaleInCredit->isChecked() || (paymentSystemsModel->index(ui->comboBoxPaymentAccount->currentIndex(), 1).data().toInt() == -2 && clientModel->record(0).value("balance_enable").toBool()) )
    {   // TODO: нужно убедиться, что эта часть кода будет выполняться только при существующем объекте clientModel
        if( sysLocale.toFloat(ui->lineEditTotal->text()) > clientModel->record(0).value("balance").toFloat() )
        {
            QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                        tr("Баланс клиента меньше суммы расходной накладной, продолжить?\n"),
                                                                        QMessageBox::No | QMessageBox::Yes,
                                                                        QMessageBox::No);
            if (resBtn == QMessageBox::No)
            {
                error = 8;
            }
        }
    }
    if (tableModel->rowCount() == 0)   //    в таблице нет строк
    {
        // будет слишкм стрёмно, если окрасить tableView в красный, поэтому просто ничего не делаем.
        error = 9;
    }
    if ( sysLocale.toFloat(ui->lineEditTotal->text()) <= 0 )
    {
        ui->lineEditTotal->setStyleSheet(commonLineEditStyleSheetRed);
        error = 10;
    }
    for(int i = 0; i < tableModel->rowCount(); i++)
    {
        if(!tableModel->value(i, "is_cancellation").toBool())   // только не помеченные на снятие резерва/возврат
        {
            if ( tableModel->value(i, "price").toFloat() <= 0 ) // цена меньше нуля
            {
                tableModel->setData(tableModel->index(i, tableModel->fields->value("price")), QColor(255, 209, 209), Qt::BackgroundRole);

                error = 11;
            }
            if( tableModel->value(i, "price").toFloat() < tableModel->value(i, "in_price").toFloat() ) // цена меньше закупочной
            {
                QMessageBox::StandardButton resBtn = QMessageBox::question( this, "SNAP CRM",
                                                                            tr("Цена ниже закупочной, продолжить?\n\"%1\"").arg(tableModel->value(i, "name").toString()),
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

void tabSale::updateTotalSumms(float amount)
{
//    qDebug() << "tabSale::updateTotalSumms() | amount =" << amount;
#ifdef QT_DEBUG
//    ui->lineEditTakeIn->setText(sysLocale.toString(amount + QRandomGenerator::global()->bounded(100), 'f', 2));
#endif

    float takeInSumm = sysLocale.toFloat(ui->lineEditTakeIn->text());
    ui->lineEditTotal->setText(sysLocale.toString(amount, 'f', 2));
    if(takeInSumm >= amount)
        ui->lineEditCharge->setText(sysLocale.toString(takeInSumm - amount, 'f', 2));
    else
        ui->lineEditCharge->setText(sysLocale.toString(0.00, 'f', 2));
}

void tabSale::findMatchingClient(QString)
{
    QString lineEditClientPhoneDisplayText = ui->lineEditClientPhone->displayText();  // отображаемый текст
    QString currentPhoneInputMask = clientPhoneTypesModel->index(ui->comboBoxClientPhoneType->currentIndex(), 2).data().toString().replace(QRegularExpression("[09]"), "_");   // в маске телефона меняем 0 и 9 на подчеркивание; 0 и 9 — это специальные маскировочные символы (см. справку QLineEdit, inputMask)
    QString enteredByUserDigits;    // строка символов, которые ввёл пользователь (т. е. текст отображаемый в lineEdit над которым выполнена операция т. н. XOR с заданной маской
    QStringList query_where;    // список условий для запроса к БД
    QString query;  // весь запрос к БД
    int i;

    for (i = 0; i < currentPhoneInputMask.length(); i++ )  // определяем какие символы ввёл пользователь
    {
        if(currentPhoneInputMask.at(i) != lineEditClientPhoneDisplayText.at(i))
            enteredByUserDigits.append(lineEditClientPhoneDisplayText.at(i));
    }
    if(ui->lineEditClientLastName->text().length() >= 3 || enteredByUserDigits.length() >= 3 )  // если пользователь ввёл более двух символов в одно из полей
    {
        query_where.clear();
        if (ui->lineEditClientLastName->text().length() >= 3 )
            query_where << QString("LCASE(CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)) REGEXP LCASE('%1')").arg(ui->lineEditClientLastName->text());   // условие поиска по фамилии, имени и отчеству
        if (enteredByUserDigits.length() >= 3 )
            query_where << QString("IFNULL(t2.`phone`, '') LIKE '%1' OR IFNULL(t2.`phone_clean`, '') REGEXP '%2'").arg(lineEditClientPhoneDisplayText, enteredByUserDigits);   // условие поиска по телефонному номеру

        ui->tableViewClientMatch->setModel(clientsMatchTable);  // указываем модель таблицы
        query = QUERY_SEL_CLIENT_MATCH.arg((query_where.count()>0?"AND (" + query_where.join(" OR ") + ")":""));
        clientsMatchTable->setQuery(query, QSqlDatabase::database("connMain"));

        // изменяем размер столбцов под соедржимое.
        // TODO: нужно бы создать свой класс с наследованием QTableView, реализовать в нём пропорциональное изменение ширин столбцов
        // при изменении размера окна и тултип для длинного текста (несколько телефонов в одной ячейке). Этот класс использовать везде
        ui->tableViewClientMatch->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if (clientsMatchTable->rowCount() > 0)
        {
            ui->groupBoxClientCoincidence->show();  // только если возвращены результаты, показываем таблицу совпадения клиента
            ui->tableViewClientMatch->show();       // может случиться ситуация, когда таблица будет скрыта, поэтому принудительно отображаем её
        }
        else
            ui->groupBoxClientCoincidence->hide();  // иначе прячем таблицу
    }
    else
    {
        clientsMatchTable->clear(); // если кол-во введённых пользователем символов меньше трёх, то удаляем результаты предыдущего запроса и прячем таблицу.
        ui->groupBoxClientCoincidence->hide();
    }
}

void tabSale::clearClientCreds(bool hideCoincidence)
{
    setDefaultStyleSheets();
    client = 0;
    price_col = 2;
    ui->comboBoxPriceCol->setCurrentText(priceColModel->getDisplayRole(price_col, 1));
    ui->lineEditClientLastName->setReadOnly(false);     // разрешаем ввод текста (вдруг он был запрещён)
    ui->lineEditClientFirstName->setReadOnly(false);
    ui->lineEditClientPatronymic->setReadOnly(false);
    ui->pushButtonCreateTabClient->setEnabled(false);
    ui->pushButtonClientFromDB->setEnabled(true);
    ui->pushButtonClientCredsClearAll->setEnabled(true);
    ui->lineEditClientLastName->clear();
    ui->lineEditClientFirstName->clear();
    ui->lineEditClientPatronymic->clear();
    ui->lineEditClientPhone->clear();
    ui->comboBoxClientPhoneType->setCurrentIndex(clientPhoneTypesModel->index(0, 0).row());     // устанавливаем первый элемент выпадающего списка
    ui->comboBoxClientAdType->setCurrentIndex(-1);
    ui->checkBoxSaleInCredit->setChecked(false);
    if (hideCoincidence)
        ui->groupBoxClientCoincidence->hide();
}

void tabSale::fillClientCreds(int id)
{
    clientPhonesModel = new QSqlQueryModel();

    clientModel->setQuery(QUERY_SEL_CLIENT(id), QSqlDatabase::database("connMain"));

    clearClientCreds(false);    // очищаем данные клиента, но не прячем таблицу совпадений

    client = id;
    ui->lineEditClientLastName->setReadOnly(true);  // запрет на изменение, если клиент из базы
    ui->lineEditClientFirstName->setReadOnly(true);
    ui->lineEditClientPatronymic->setReadOnly(true);
    ui->pushButtonCreateTabClient->setEnabled(true);
    ui->lineEditClientFirstName->setText(clientModel->record(0).value("name").toString());
    ui->lineEditClientLastName->setText(clientModel->record(0).value("surname").toString());
    ui->lineEditClientPatronymic->setText(clientModel->record(0).value("patronymic").toString());

    clientPhonesModel->setQuery(QUERY_SEL_CLIENT_PHONES(id), QSqlDatabase::database("connMain"));

    // заполняем типы телефонов. Пока так, потом придумаю что-то более элегантное
    // TODO: придумать механизм сокрытия части номера, если у пользователя недостаточно прав
    if(clientPhonesModel->rowCount() > 0)
    {
        ui->comboBoxClientPhoneType->setCurrentIndex(clientPhoneTypesModel->index((clientPhonesModel->index(0, 1).data().toInt() - 1), 0).row());    //
        ui->lineEditClientPhone->setText(clientPhonesModel->index(0, 0).data().toString());
    }

    price_col = clientModel->record(0).value("price_col").toInt();
    if(!price_col)
        price_col = 2;  // на случай ошибок в БД, по умолчанию тип цен — Розница
    ui->comboBoxPriceCol->setCurrentText(priceColModel->getDisplayRole(price_col, 1));
    ui->comboBoxClientAdType->setCurrentText(clientAdTypesList->getDisplayRole(clientModel->record(0).value("visit_source").toInt(), 1));
    if(clientModel->record(0).value("balance_enable").toBool())
    {
        setBalanceWidgetsVisible(true);
        ui->lineEditBalance->setText(sysLocale.toString(clientModel->record(0).value("balance").toFloat(), 'f', 2));
    }
    else
        setBalanceWidgetsVisible(false);

    clientPhonesModel->deleteLater();
}

void tabSale::buttonSelectExistingClientHandler()
{

}

void tabSale::buttonCreateTabClientHandler()
{
    emit createTabClient(client);
}

void tabSale::tableRowDoubleClick(QModelIndex tableIndex)
{
    emit createTabSparePart(tableModel->value(tableIndex.row(), "item_id").toInt());
}

void tabSale::hideGroupBoxClient(bool isAnonymousBuyer)
{
    if(isAnonymousBuyer)
    {
        ui->groupBoxClient->hide();
        ui->buttonReserve->setEnabled(false);   // думаю нельзя резервировать товар на анонимного клиента
    }
    else
    {
        ui->groupBoxClient->show();
        ui->buttonReserve->setEnabled(true);
    }
}

void tabSale::selectPriceCol(int comboBoxIndex)
{
//    qDebug() << "tabSale::selectPriceCol() | priceCol =" << priceColModel->index(comboBoxIndex, 2).data().toString();
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));

    query->prepare(QUERY_SEL_STORE_ITEMS_ITEM_PRICE(priceColModel->index(comboBoxIndex, 2).data().toString()));
    tableModel->setPriceColumn(query);
}

void tabSale::clientMatchTableDoubleClicked(QModelIndex index)
{
    fillClientCreds(clientsMatchTable->index(index.row(), 0).data().toInt());
//    ui->groupBoxClientCoincidence->hide();  // прячем таблицу совпадения клиента
    ui->tableViewClientMatch->hide();   // прячем таблицу, а не весь groupBox (вдруг пользователь промахнётся)
}

void tabSale::addItemByUID()
{
    addItemByUID(ui->lineEditAddByUID->text().toInt());
    ui->lineEditAddByUID->setText("");
}

void tabSale::addItemByUID(int uid)
{
    if(tableModel->getModelState() == 0)
    {
        if(!isItemAlreadyInList(uid))
        {
            newItemModel->setQuery(QUERY_SEL_PART_FOR_SALE(uid, priceColModel->index(ui->comboBoxPriceCol->currentIndex(), 2).data().toString()), QSqlDatabase::database("connMain"));
            for(int i = 0; i < newItemModel->rowCount(); i++)
            {
                if(newItemModel->record(i).value("avail").toInt())
                    tableModel->appendRecord(newItemModel->record(i));
                else
                    qDebug() << QString("Товар UID %1 не доступен").arg(newItemModel->record(i).value("UID").toString());
            }
        }
        else
        {
            qDebug() << "товар уже добавлен";
            // TODO: сообщение, что товар уже добавлен
        }
    }
}

bool tabSale::unSale()  // распроведение
{
    bool nDBErr = 1, nIntegrityErr = 1, balance = 0, lastRow = 1;
    QSqlQuery *query_pre = new QSqlQuery(QSqlDatabase::database("connThird"));
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QMap<int, int> *list;
    QString client_;
    QStringList store_sales_cancel_records;
    int user, office, cash_order_id, paymentAccount = 0;
    float amount;

    user = userData->value("id").toInt();
    office = userData->value("current_office").toInt();

    if( paymentSystemsModel->index(ui->comboBoxMoneyBackAccount->currentIndex(), 1).data().toInt() == -2 && (!clientModel->record(0).value("balance_enable").toBool() || ui->checkBoxAnonymous->isChecked()) )
    {
        ui->comboBoxMoneyBackAccount->setStyleSheet(commonComboBoxStyleSheetRed);
        return true; // return 0 — OK, return 1 - ошибка
    }

#ifdef QT_DEBUG
    queryLog *log = new queryLog(query);
    log->setFile(QApplication::applicationDirPath() + "\\tabSale.sql");
    log->truncateLog();
#endif

    if ( ui->checkBoxAnonymous->isChecked() )
    {
        client_ = "NULL";
    }
    else
    {
        client_ = QString::number(client);
    }

    QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);
    list = tableModel->getPendingRemoveList();
    if(list->count())       // если список не пуст, то это частичный возврат товаров
    {
        QMap<int, int>::iterator i = list->begin();
        while(i != list->end())
        {
            store_sales_cancel_records.append(QString::number(i.value())); // преобразование в QStringList
            i++;
        }

        // нужно проверить остались ли в списке строки, не удалённые раньше или не помеченные на удаление сейчас
        // может так случиться, что пользователь нажмет кнопку Удалить в единственной строке накладной, и чтобы весь документ пометился как распроведённый...
        for(int i = 0; i < tableModel->rowCount(); i++)
        {
            if(!tableModel->value(i, "is_cancellation").toBool())
                lastRow = 0;
        }
        amount = sysLocale.toFloat(ui->lineEditCharge->text()); // при частичном возврате, в поле Сдача будет записана сумма товаров, помеченных на удаление (т. е. сумма возврата)

    }
    else    // если пуст, то возврат всех товаров
    {
        for(int i = 0; i < tableModel->rowCount(); i++)
        {
            if(!tableModel->value(i, "is_cancellation").toBool())     // доп. проверка флага, чтобы ранее возвращенные товары не были еще раз возвращены
                store_sales_cancel_records.append(tableModel->value(i, "id").toString());
        }
        amount = sysLocale.toFloat(ui->lineEditTotal->text()); // при полном возврате, сумма возврата равна сумме накладной
    }

    if(lastRow)
    {
        QUERY_EXEC(query,nDBErr)(QUERY_UPD_SALE_DOC_STATE_N_SUMM(doc_id,  7, 0)); // документ помечается распроведённым, сумма 0.
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",6,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("Расходная накладная №%1 распроведена, основание: %2").arg(doc_id).arg(ui->textEditRevertReason->toPlainText())));
        // TODO: нужно изучить вопрос возврата товара при условии использования РРО, возможно, для этого нужно создавать отдельный документ на возврат
    }
    else
        QUERY_EXEC(query,nDBErr)(QUERY_UPD_SALE_DOC_STATE_N_SUMM(doc_id,  "`state`", tableModel->totalAmount())); // изменяется только сумма

    QUERY_EXEC(query_pre,nDBErr)(QUERY_SEL_PRE_UPD_STORE_ITEMS6(doc_id, store_sales_cancel_records.join(',')));
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS_UNSALE(doc_id, store_sales_cancel_records.join(',')));
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_SALES_UNSALE(doc_id, store_sales_cancel_records.join(',')));
    QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_DOC_RET(userData->value("id").toInt(),userData->value("current_office").toInt(),doc_id,QString("'Возврат товара, основание: %1'").arg(ui->textEditRevertReason->toPlainText()), store_sales_cancel_records.join(',')));
    QUERY_EXEC(query,nDBErr)(QUERY_SEL_PST_UPD_STORE_ITEMS6(doc_id, store_sales_cancel_records.join(',')));

    while(query->next() && query_pre->next() && nIntegrityErr)
    {
        for(int j = 0; j < query->record().count(); j++)   // проверка одинаковости всех полей
        {
            if (query->value(j).toInt() != query_pre->value(j).toInt())
            {
                qDebug() << QString("query->value(%1) =").arg(j) << query->value(j).toInt() << QString("query_pre->value(%1) =").arg(j) << query_pre->value(j).toInt();
                qDebug() << "Ошибка: не совпадают кол-ва товара до/после | `item_id` =" << tableModel->value(j, "item_id").toInt();
                nIntegrityErr = 0;
            }
        }
    }
    delete query_pre;

    // этот код скопирован из метора sale(); нужно подумать над отдельным методом записи кассовых данных
    balance = (paymentSystemsModel->index(ui->comboBoxMoneyBackAccount->currentIndex(), 1).data().toInt() == -2);
    paymentAccount = paymentSystemsModel->index(ui->comboBoxMoneyBackAccount->currentIndex(), 1).data().toInt();

    if(balance)   // если возврат на баланс
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_BALANCE_LOG(client_, amount, 1, QString("Зачисление %1 - возврат за товары по РН №%2").arg(sysLocale.toCurrencyString(amount)).arg(doc_id), office, user));
        QUERY_EXEC(query,nDBErr)(QUERY_UPDATE_BALANCE(client_,amount));
        QUERY_EXEC(query,nDBErr)(QUERY_VRFY_BALANCE(client_));
        QUERY_EXEC_VRFY(query,nIntegrityErr);
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("Баланс клиента пополнен на %1  - возврат за товары по РН №%2").arg(sysLocale.toCurrencyString(amount)).arg(doc_id)));
    }
    else    // если возврат денег
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_CASH(
                                   9,
                                   -amount,
                                   amountToWords(amount).replace('\'',"\\\'"),
                                   client_,
                                   user,
                                   office,
                                   QString("Расход денег в размере %2 - возврат за товары по РН №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)),
                                   "NULL",
                                   doc_id,
                                   paymentAccount
                                   )
                    );
        // финансы — очень важная вещь, поэтому нужно верифицировать записанные данные
        QUERY_EXEC(query,nDBErr)(QUERY_VRFY_CASH(
                                   9,
                                   -amount,
                                   amountToWords(amount).replace('\'',"\\\'"),
                                   "\'"+client_+"\'",
                                   user,
                                   office,
                                   QString("Расход денег в размере %2 - возврат за товары по РН №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)),
                                   "\'NULL\'",
                                   doc_id,
                                   paymentAccount
                                   )
                    );
        QUERY_EXEC_VRFY(query,nIntegrityErr);
        QUERY_LAST_INS_ID(query,nDBErr,cash_order_id);

        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("Расход денег в размере %1 - возврат за товары по РН №%2 (%3).").arg(sysLocale.toCurrencyString(amount)).arg(doc_id).arg(ui->comboBoxPaymentAccount->currentText())));
    }

    if (!nIntegrityErr)
    {
        QMessageBox msgBox;
        msgBox.setText("Ошибка");
        msgBox.setIcon(QMessageBox::Critical);
//            qDebug() << "Ошибка целостности данных (касса/баланс)!";
        msgBox.setText(QString("Ошибка целостности данных!"));
        msgBox.exec();
        nDBErr = 0;
    }

//    nDBErr = 0; // это временное для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
    QUERY_COMMIT_ROLLBACK(query,nDBErr);

#ifdef QT_DEBUG
    log->saveLog();
    delete log;
#endif

    if(nDBErr)
        return false;
    else
        return true; // return 0 — OK, return 1 - ошибка
}

// https://stackoverflow.com/a/18866593
QString tabSale::genUserWebPass() const // TODO: это копия метода из класса tabRepairNew; нужно вынести его в отдельный класс
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

/* На вкладке продажа задумка с кнопками "Продать ещё" и "Продать" несколько другая, чем аналогичные кнопки
 * на вкладке приёма в ремонт: при нажатии "Продать" производится продажа, вкладка остаётся открытой (это для
 * того, чтобы пользователь мог сначала набросать список товаров, а потом собрать товары и выдать покупателю);
 * нажатие "Продать ещё" приведёт к проводке накладной и очистке списка товаров и данных клиента (этот режим
 * будет удобен при большом кол-ве продаж — не придётся вызывать вкладку продажи вручную).
 */
void tabSale::clearAll()
{
    clearClientCreds(1);
    ui->checkBoxAnonymous->setChecked(false);
    ui->lineEditTotal->setText(sysLocale.toString(0.00, 'f', 2));
    ui->lineEditTakeIn->setText("");
    tableModel->removeRows(0, tableModel->rowCount());
    newItemModel->clear();
    clientModel->clear();
}

bool tabSale::isItemAlreadyInList(int id)
{
    for(int i = 0; i < tableModel->rowCount(); i++)
    {
        if(tableModel->value(i, "item_id").toInt() == id)
            return true;
    }
    return false;
}

bool tabSale::sale()
{
    if(!checkInput())
        return true; // return 0 — OK, return 1 - ошибка

    bool nDBErr = 1, nIntegrityErr = 1, balance = 0, isAnonBuyer = 0;
    int user, office, cash_order_id, paymentAccount = 0, reserveDays = 5, priceCol = 2;
    float amount, currency = 29.2549 /* TODO: запрос валюты, даже если программа не перезапускалась несколько дней */;
    QString phone1, phone1c, client_;
    QStringList store_sales_cancel_records;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QSqlQuery *query_pre = new QSqlQuery(QSqlDatabase::database("connThird"));

#ifdef QT_DEBUG
    queryLog *log = new queryLog(query);
    log->setFile(QApplication::applicationDirPath() + "\\tabSale.sql");
    log->truncateLog();
#endif

    user = userData->value("id").toInt();
    office = userData->value("current_office").toInt();
    amount = tableModel->totalAmount();
    reserveDays = ui->spinBoxReserve->value();
    priceCol = priceColModel->index(ui->comboBoxPriceCol->currentIndex(), 1).data().toInt();
    isAnonBuyer = ui->checkBoxAnonymous->isChecked();

    // в этом месте у АСЦ недоработка: не зависимо от установки галочки "с баланса", код `payment_system` в табл. docs записывается соответствующий выбранному счету в комбобоксе
    balance = ui->checkBoxSaleInCredit->isChecked() || paymentSystemsModel->index(ui->comboBoxPaymentAccount->currentIndex(), 1).data().toInt() == -2;
    if(balance)
        paymentAccount = -2;    // TODO: наверное правильно будет при установке галочки "с баланса" автоматически переключить комбобокс на соотв. индекс
    else
        paymentAccount = paymentSystemsModel->index(ui->comboBoxPaymentAccount->currentIndex(), 1).data().toInt();

    if (client == 0 && !isAnonBuyer )    // TODO: это копия кода (с небольшими дополнениями) из класса tabRepairNew; нужно вынести его в отдельный метод отдельного класса
    {
        QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);
        QUERY_EXEC(query,nDBErr)(QUERY_INS_CLIENT
                    .arg(user)
                    .arg(ui->lineEditClientFirstName->text())
                    .arg(ui->lineEditClientLastName->text())
                    .arg(ui->lineEditClientPatronymic->text())
                    .arg("")
                    .arg(0)
                    .arg(clientAdTypesList->record(ui->comboBoxClientAdType->currentIndex()).value("id").toInt())
                    .arg(genUserWebPass())
                    .arg(""));
        QUERY_LAST_INS_ID(query,nDBErr,client);
//        qDebug() << "Новый клиент: LAST_INSERT_ID() = " << client;
//        qDebug() << QUERY_INS_LOG("NULL",2,user,office,client,"NULL","NULL","NULL","NULL","Быстрое создание клиента из формы приёма в ремонт");

        // запись телефонных номеров в таблицу tel
        if (ui->lineEditClientPhone->hasAcceptableInput())
        {
            phone1 = ui->lineEditClientPhone->text();
            phone1c = "";
            for (int i = 0; i < phone1.length(); i++ )  // переписываем "чистый" номер
            {
                if(phone1.at(i).isDigit())
                    phone1c.append(phone1.at(i));
            }
//            qDebug() << "phone1c: " << phone1c;
            QUERY_EXEC(query,nDBErr)(QUERY_INS_PHONE(phone1,phone1c,clientPhoneTypesModel->index(ui->comboBoxClientPhoneType->currentIndex(), 1).data().toInt(),client,1,""));
        }

        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",2,user,office,client,"NULL","NULL","NULL","NULL","Быстрое создание клиента из формы продажи"));
        QUERY_COMMIT_ROLLBACK(query,nDBErr);
    }

    if (isAnonBuyer)
    {
        client_ = "NULL";
    }
    else
    {
        client_ = QString::number(client);
    }


    QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);
    if(reserve < 2) // простая продажа и резерв; при проведении РН ранее зарезервированных товаров документ создавать не нужно
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_DOCS( reserve?6:2, 0, 0, paymentAccount, 1, "NULL", user, amount,\
                                                QString("\'%1\'").arg(ui->lineEditComment->text()), 1, client_,\
                                                currency, "\'Empty\'", priceCol, reserveDays ));
        QUERY_LAST_INS_ID(query,nDBErr,doc_id);
    }


    if(reserve == 2)    // при проведении РН резерва, сначала обновляем данные товаров, помеченных на удаление
    {                           //
        QMap<int, int>* list = tableModel->getPendingRemoveList();
        if(list->count())
        {
            QMap<int, int>::iterator i = list->begin();
            while(i != list->end())
            {
                store_sales_cancel_records.append(QString::number(i.value())); // преобразование в QStringList
                i++;
            }
            QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS_RESERVE_CANCELLATION(doc_id, store_sales_cancel_records.join(','))); // TODO: нужно проверить порядок этих запросов
            QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_SALES_RESERVE_CANCELLATION(doc_id, store_sales_cancel_records.join(',')));
        }
    }

    for(int i = 0; i < tableModel->rowCount(); i++)
    {

        if(reserve == 0)        // продажа
        {
            QUERY_EXEC(query_pre,nDBErr)(QUERY_SEL_PRE_UPD_STORE_ITEMS3(tableModel->value(i, "count").toInt(), tableModel->value(i, "item_id").toInt()));    // кол-ва товара, которое должно получиться после обновления
        }
        else if(reserve == 1)    // резерв
        {
            QUERY_EXEC(query_pre,nDBErr)(QUERY_SEL_PRE_UPD_STORE_ITEMS2(tableModel->value(i, "count").toInt(), tableModel->value(i, "item_id").toInt()));    // кол-ва товара, которое должно получиться после обновления
        }
        else if(reserve == 2)    // продажа ранее зарезервированного
        {
            QUERY_EXEC(query_pre,nDBErr)(QUERY_SEL_PRE_UPD_STORE_ITEMS4(tableModel->value(i, "count").toInt(), tableModel->value(i, "id").toInt()));    // кол-ва товара, которое должно получиться после обновления
        }

        if(reserve < 2)     // продажа и резерв
        {
            QUERY_EXEC(query,nDBErr)(QUERY_INS_STORE_SALES( tableModel->value(i, "dealer").toInt(),
                                                            tableModel->value(i, "item_id").toInt(),
                                                            doc_id,
                                                            client_,
                                                            tableModel->value(i, "count").toInt(),
                                                            tableModel->value(i, "in_price").toFloat(),
                                                            tableModel->value(i, "price").toFloat(),
                                                            tableModel->value(i, "warranty").toInt(),
                                                            tableModel->value(i, "is_realization").toBool(),
                                                            tableModel->value(i, "return_percent").toInt(),
                                                            user,
                                                            tableModel->value(i, "sn").toString()));
            if(reserve == 0)    // продажа
            {
                QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS3(tableModel->value(i, "count").toInt(), tableModel->value(i, "item_id").toInt()));
                QUERY_EXEC(query,nDBErr)(QUERY_SEL_PST_UPD_STORE_ITEMS3(doc_id, tableModel->value(i, "item_id").toInt()));     // кол-ва товара после обновления
            }
            else if(reserve == 1)        // резерв
            {
                QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS2(tableModel->value(i, "count").toInt(), tableModel->value(i, "item_id").toInt()));
                QUERY_EXEC(query,nDBErr)(QUERY_SEL_PST_UPD_STORE_ITEMS2(doc_id, tableModel->value(i, "item_id").toInt()));     // кол-ва товара после обновления
            }
        }
        else    // при проведении РН ранее зарезервированных товаров нужно обновить данные в таблице store_sales: логично предположить, что, например, серийный номер будет вводиться только при проведении документа; также может измениться кол-во
        {
            QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS4(tableModel->value(i, "count").toInt(), tableModel->value(i, "id").toInt())); // этот запрос должен выполняться до обновления store_sales
            QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_SALES(tableModel->value(i, "id").toInt(),
                                                           client_,
                                                           tableModel->value(i, "count").toInt(),
                                                           tableModel->value(i, "price").toFloat(),
                                                           tableModel->value(i, "warranty").toInt(),
                                                           user,
                                                           "`is_cancellation`", // остаются без изменений
                                                           "`cancellation_reason`",
                                                           tableModel->value(i, "sn").toString()));
            QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS4_1(tableModel->value(i, "count").toInt(), tableModel->value(i, "id").toInt()));   // это специфические запросы и просматривая жунрал сервера можно запутаться, но они такие для механизма верификации
            QUERY_EXEC(query,nDBErr)(QUERY_SEL_PST_UPD_STORE_ITEMS4(tableModel->value(i, "id").toInt()));     // кол-ва товара после обновления
        }

        if(query->next() && query_pre->next() && nIntegrityErr)        // запросы для всех случаев должны быть составлены так, чтобы проверка была одинаковой
        {
//            qDebug() << "Доступно (на момент проведения док-та):" << (query_pre->value(0).toInt() - query_pre->value(4).toInt()) << "; требуется: " << tableModel->value(i, "count").toInt();
            if(tableModel->value(i, "count").toInt() > (query_pre->value(0).toInt() - query_pre->value(4).toInt()) )
            {
                qDebug() << "Qt::BackgroundRole =" << tableModel->index(i, 3).data(Qt::BackgroundRole);
                qDebug() << "Ошибка: на момент проведения док-та доступное кол-во товара изменилось | `item_id` =" << tableModel->value(i, "item_id").toInt();
                tableModel->setData(tableModel->index(i, 3), QColor("#FFD1D1"), Qt::BackgroundRole);
                tableModel->setData(tableModel->index(i, 4), QColor("#FFD1D1"), Qt::BackgroundRole);
                nIntegrityErr = 0;
            }

            for(int j = 0; j < query->record().count(); j++)   // проверка одинаковости всех полей
            {
                if (query->value(j).toInt() != query_pre->value(j).toInt())
                {
                    qDebug() << QString("query->value(%1) =").arg(j) << query->value(j).toInt() << QString("query_pre->value(%1) =").arg(j) << query_pre->value(j).toInt();
                    qDebug() << "Ошибка: не совпадают кол-ва товара до/после | `item_id` =" << tableModel->value(j, "item_id").toInt();
                    nIntegrityErr = 0;
                }
            }
        }
        // TODO: если последняя единица товара, нужно удалить ячейку и сделать соотв. запись в журнал (но! нужно чтобы при вызове updateWidgets() в таблице место не обновлялось)
        // а может и не нужно; в АСЦ удалялась, но за годы пользования я так и не понял зачем это сделано
    }
    delete query_pre;

    // в АСЦ с кол-вом покупок то ли глюк, то ли странная задумка: в таблице клиентов отображается кол-во РН, а в самой карте клиента кол-во записей в таблице store_sales
    // TODO: разобраться с этим
    if(reserve != 1) // статистику клиента обновляем только при простой продаже и проведении РН резерва
        QUERY_EXEC(query,nDBErr)(QUERY_UPD_CLIENT_PURCHASES(client_, tableModel->rowCount()));

    if(reserve == 1)
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",6,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("Расходная накладная №%1 создана (резерв)").arg(doc_id)));
        QUERY_EXEC(query,nDBErr)(QUERY_UPD_SALE_DOC_TYPE_N_STATE(doc_id, 6, 3));
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_DOC(user,office,doc_id,QString("CONCAT('Резерв ', `count`, 'ед. товара на срок (дней): %1')").arg(reserveDays)));
    }
    else
    {
        if(reserve == 2)    // оформление продажи ранее зарезервированного товара
        {
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",6,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("Расходная накладная №%1 проведена").arg(doc_id)));
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_DOC_RSRV(user,office,doc_id,"'Снятие резерва'", 1));  // запись о снятии с резерва для удалённых товаров
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_DOC_RSRV(user,office,doc_id,"'Снятие резерва перед проведением РН'", 0));  // т. к. кол-во может измениться, сначала запись о снятии с резерва
            QUERY_EXEC(query,nDBErr)(QUERY_UPD_SALE_DOC_TYPE_STATE_SUMM(doc_id, 2, 5, amount));
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_DOC_RSRV(user,office,doc_id,"CONCAT('Продажа товара в кол-ве ', `count`, 'шт. (кол-во может отличаться от ранее зарезервированного)')", 0));  // а теперь запись о продаже, но уже с правильным кол-вом // TODO: единицы измерения кол-ва товара; нужна или отдельная таблица (а это, в свою очередь,  требует разработки механизма проверки schemaversion и автообновления) либо изменить запросы записи в журнал
        }
        else                // простая продажа
        {
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",6,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("Расходная накладная №%1 создана").arg(doc_id)));
            QUERY_EXEC(query,nDBErr)(QUERY_UPD_SALE_DOC_TYPE_N_STATE(doc_id, 2, 5));
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_DOC(user,office,doc_id,"CONCAT('Продажа товара в кол-ве ', `count`, 'шт.')"));
        }


        if(balance)   // если продажа в долг          // TODO: это копия кода из класса tabRepairNew; нужно вынести его в отдельный метод отдельного класса
        {
            QUERY_EXEC(query,nDBErr)(QUERY_INS_BALANCE_LOG(client_, -amount, 0, QString("Списание %1 за товары по РН №%2").arg(sysLocale.toCurrencyString(amount)).arg(doc_id), office, user));
            QUERY_EXEC(query,nDBErr)(QUERY_UPDATE_BALANCE(client_,-amount));
            QUERY_EXEC(query,nDBErr)(QUERY_VRFY_BALANCE(client_));
            QUERY_EXEC_VRFY(query,nIntegrityErr);
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("С баланса клиента списано %1 за товары по РН №%2.").arg(sysLocale.toCurrencyString(amount)).arg(doc_id)));
        }
        else    // если продажа с оплатой
        {
            QUERY_EXEC(query,nDBErr)(QUERY_INS_CASH(
                                       14,
                                       amount,
                                       amountToWords(amount).replace('\'',"\\\'"),
                                       client_,
                                       user,
                                       office,
                                       QString("Поступление денег в размере %2 по расходной накладной №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)),
                                       "NULL",
                                       doc_id,
                                       paymentAccount
                                       )
                        );
            // финансы — очень важная вещь, поэтому нужно верифицировать записанные данные
            QUERY_EXEC(query,nDBErr)(QUERY_VRFY_CASH(
                                       14,
                                       amount,
                                       amountToWords(amount).replace('\'',"\\\'"),
                                       "\'"+client_+"\'",
                                       user,
                                       office,
                                       QString("Поступление денег в размере %2 по расходной накладной №%1").arg(doc_id).arg(sysLocale.toCurrencyString(amount)),
                                       "\'NULL\'",
                                       doc_id,
                                       paymentAccount
                                       )
                        );
            QUERY_EXEC_VRFY(query,nIntegrityErr);
            QUERY_LAST_INS_ID(query,nDBErr,cash_order_id);

            // TODO: Признак предмета расчета
            QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,user,office,client_,"NULL","NULL",doc_id,"NULL",QString("Поступление %1 за товары по РН №%2 (%3).").arg(sysLocale.toCurrencyString(amount)).arg(doc_id).arg(ui->comboBoxPaymentAccount->currentText())));
        }
    }

    if (!nIntegrityErr)
    {
        QMessageBox msgBox;
        msgBox.setText("Ошибка");
        msgBox.setIcon(QMessageBox::Critical);
    //            qDebug() << "Ошибка целостности данных (касса/баланс)!";
        msgBox.setText(QString("Ошибка целостности данных!"));
        msgBox.exec();
        nDBErr = 0;
    }

#ifdef QT_DEBUG
//    nDBErr = 0; // это временное для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif

    QUERY_COMMIT_ROLLBACK(query,nDBErr);

#ifdef QT_DEBUG
//    nDBErr = 1; // и это временное для отладки (чтобы проверить работу дальше)

    log->saveLog();
    delete log;
#endif

    delete query;
    if(nDBErr)
    {
        QMap<QString, QVariant> report_vars;
        report_vars.insert("type", "rn");
        report_vars.insert("doc_id", doc_id);
        emit generatePrintout(report_vars);

        // TODO: печать чека
        return false;
    }
    else
        return true; // return 0 — OK, return 1 - ошибка
}

void tabSale::saleButtonClicked()
{
//    qDebug() << "tabSale::saleButtonClicked() | reserve =" << reserve;
    if (sale())
        return;

    updateWidgets();
}

void tabSale::saleMoreButtonClicked()
{
    qDebug() << "reserve =" << reserve;
    if (sale())
        return;

    doc_id = 0;
    reserve = 0;
    updateWidgets();
}

/* При нажатии "Резерв" вызывается тот же метод, что и при продаже; для изменения набора запросов к БД используется
 * флаг reserve.
 */
void tabSale::reserveButtonClicked()
{
    reserve = 1;

    if(sale())
        return;

    updateWidgets();
}

void tabSale::addItemButtonClicked()
{

}

void tabSale::paramsButtonClicked()
{

}

void tabSale::updateChargeAmount(QString text)
{
    // TODO: применить стиль по умолчанию, если поле было красным
    float takeInSumm = sysLocale.toFloat(text);
    float amount = sysLocale.toFloat(ui->lineEditTotal->text());
    if(amount <= takeInSumm )
        ui->lineEditCharge->setText(sysLocale.toString(takeInSumm - amount, 'f', 2));
    else
        ui->lineEditCharge->setText(sysLocale.toString(0.00, 'f', 2));

}

void tabSale::comboBoxIndexChanged(int index)
{
}

void tabSale::phoneTypeChanged(int index)
{
    ui->lineEditClientPhone->setInputMask(""); ui->lineEditClientPhone->setInputMask(clientPhoneTypesModel->index(index, 2).data().toString() + ";_");  // Here ";_" for filling blank characters with underscore
}

void tabSale::setTrackNum()     // слот вызываемый по нажатию Return
{
    bool nDBErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QString client_;


    if (ui->checkBoxAnonymous->isChecked())
    {
        client_ = "NULL";
    }
    else
    {
        client_ = QString::number(client);
    }

    QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_SALE_DOC_SET_TRACK(doc_id, ui->lineEditTrack->text()));
    QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,userData->value("id").toInt(),userData->value("current_office").toInt(),client_,"NULL","NULL",doc_id,"NULL",QString("РН №%1 присвоен номер ТТН: %2").arg(doc_id).arg(ui->lineEditTrack->text())));   // TODO: экранировать спец символы, мало ли что введёт пользователь
    QUERY_COMMIT_ROLLBACK(query,nDBErr);
    if(!nDBErr);
        //TODO: сообщение об ошибке
    delete query;
}

void tabSale::reserveCancelButtonClicked()
{
    if(docModel->record(0).value("state").toInt() != 3)   // проверка на всякий случай, вдруг накосячу с отображением кнопок в разных режимах
        return;

    bool nDBErr = 1, nIntegrityErr = 1;
    QSqlQuery *query_pre = new QSqlQuery(QSqlDatabase::database("connThird"));
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    QStringList store_sales_records;
    QString client_;

    if ( ui->checkBoxAnonymous->isChecked() )
    {
        client_ = "NULL";
    }
    else
    {
        client_ = QString::number(client);
    }

#ifdef QT_DEBUG
    queryLog *log = new queryLog(query);
    log->setFile(QApplication::applicationDirPath() + "\\tabSale.sql");
    log->truncateLog();
#endif

    for(int i = 0; i < tableModel->rowCount(); i++)
    {
        store_sales_records.append(tableModel->value(i, "id").toString());
    }

    QUERY_EXEC(query,nDBErr)(QUERY_BEGIN);
    QUERY_EXEC(query_pre,nDBErr)(QUERY_SEL_PRE_UPD_STORE_ITEMS5(doc_id));
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_SALE_DOC_STATE_N_SUMM(doc_id, 4, 0));
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_ITEMS_RESERVE_CANCELLATION(doc_id, store_sales_records.join(',')));
    QUERY_EXEC(query,nDBErr)(QUERY_UPD_STORE_SALES_RESERVE_CANCELLATION(doc_id, store_sales_records.join(',')));
    QUERY_EXEC(query,nDBErr)(QUERY_SEL_PST_UPD_STORE_ITEMS5(doc_id));
    QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG_PARTS_IN_DOC_RSRV(userData->value("id").toInt(),userData->value("current_office").toInt(),doc_id,"'Снятие резерва'", 1));
    QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",6,userData->value("id").toInt(),userData->value("current_office").toInt(),client_,"NULL","NULL",doc_id,"NULL",QString("Резерв товара по РН№%1 снят").arg(doc_id)));

    while(query->next() && query_pre->next() && nIntegrityErr)
    {
        for(int j = 0; j < query->record().count(); j++)   // проверка одинаковости всех полей
        {
            if (query->value(j).toInt() != query_pre->value(j).toInt())
            {
                qDebug() << QString("query->value(%1) =").arg(j) << query->value(j).toInt() << QString("query_pre->value(%1) =").arg(j) << query_pre->value(j).toInt();
                qDebug() << "Ошибка: не совпадают кол-ва товара до/после | `item_id` =" << tableModel->value(j, "item_id").toInt();
                nIntegrityErr = 0;
            }
        }
    }

#ifdef QT_DEBUG
//    nDBErr = 0; // это временное для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif

    QUERY_COMMIT_ROLLBACK(query,nDBErr);
    updateWidgets();
    delete query;
    delete query_pre;

#ifdef QT_DEBUG
    log->saveLog();
    delete log;
#endif
}

void tabSale::printButtonClicked()
{

}

void tabSale::logButtonClicked()
{

}

void tabSale::unSaleButtonClicked()
{
    if(docModel->record(0).value("state").toInt() != 5)   // проверка на всякий случай, вдруг накосячу с отображением кнопок в разных режимах
        return;

    if(unSale())
        return;
    updateWidgets();
}

tabSale* tabSale::getInstance(int doc_id, MainWindow *parent)   // singleton: одна вкладка для ремонта
{
    if( !p_instance.contains(doc_id) )
      p_instance.insert(doc_id, new tabSale(doc_id, parent));
    return p_instance.value(doc_id);
}

/******************************************************************************************************************************************************
 *
 */
sparePartsTable::sparePartsTable(QWidget *parent) :
    QTableView(parent)
{
}

sparePartsTable::~sparePartsTable()
{
}

void sparePartsTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    int i;
    int colWidths[] = {30,100,0,45,45,70,70,120,120,80};
    int colNameWidth = 0;

    verticalHeader()->hide();
    hideColumn(10); // прячем служебные столбцы
    hideColumn(11);
    hideColumn(12);
    hideColumn(13);
    hideColumn(14);
    hideColumn(15);
    hideColumn(16);

    for (i = 0; i < 10; i++)
    {
        colNameWidth += colWidths[i];
        setColumnWidth(i, colWidths[i]);
    }
    colNameWidth = geometry().width() /*- verticalScrollBar()->width()*/ - colNameWidth - 10;
    if (verticalScrollBar()->isVisible())
        setColumnWidth(2, colNameWidth - verticalScrollBar()->width());
    else
        setColumnWidth(2, colNameWidth);
    resizeRowsToContents();
}

void sparePartsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
{
    QTableView::dataChanged(topLeft,bottomRight,roles);
    resizeRowsToContents();
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
        QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));
        i = QRandomGenerator::global()->bounded(10); // кол-во товаров от 1 до 10
        if(i < 3)
            i += 3;

        for(int j = 0; j < i; j++)
        {
            query->exec(QString("SELECT `id` FROM (SELECT ROUND(@i * RAND(), 0) AS 'rand') AS `rand` LEFT JOIN (SELECT @i := @i + 1 AS 'num', t1.`id` FROM store_items AS t1 CROSS JOIN (SELECT @i := 0) AS dummy WHERE t1.`count` - t1.`reserved` > 0) AS t1 ON t1.`num` = `rand`.`rand`;"));
            if(j<2)
                continue;   // после обновления сервера на mysql 5.6.51 (win) пришлось чуть-чуть изменить запрос для случайного товара; также в только что открытой сессии результаты первых двух запросов будут состоять из NULL, поэтому пропускаем их

            query->first();
            if(query->isValid())
                addItemByUID(query->record().value(0).toInt());
        }

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
        ui->lineEditTakeIn->setText(sysLocale.toString(sysLocale.toFloat(ui->lineEditTotal->text()) + QRandomGenerator::global()->bounded(100), 'f', 2));
    }
//    ...

    else if (test_scheduler_counter == 11)
    {
//        saleButtonClicked();
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

void tabSale::test_scheduler_handler()  //
{
    if(tableModel->getModelState() == 0)
        randomFill();
}

void tabSale::test_scheduler2_handler()  // обработик таймера закрытия вкладки
{
//    qDebug() << "test_scheduler2_handler(), clientTabId = " << ui->tabWidget->indexOf(tabClients::getInstance(0));
//    closeTab(ui->tabWidget->indexOf(tabClients::getInstance(0)));
    //    test_scheduler->start(1000);    //  перезапускаем таймер открытия вкладки
}

void tabSale::test_updateWidgetsWithDocNum()
{
    doc_id = testLineEdit->text().toInt();
    updateWidgets();
}
#endif
