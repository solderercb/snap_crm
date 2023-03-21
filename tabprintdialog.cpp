#include "global.h"
#include "tabprintdialog.h"
#include "ui_tabprintdialog.h"
#include "com_sql_queries.h"

tabPrintDialog::tabPrintDialog(MainWindow *parent, QMap<QString, QVariant> report_vars):
    tabCommon(parent),
    m_reportVars(report_vars),
    ui(new Ui::tabPrintDialog)
{
    ui->setupUi(this);
    initProgressWidget();

    renderDelayTimer = new QTimer();
    renderDelayTimer->setSingleShot(true);
    connect(renderDelayTimer, &QTimer::timeout, this, &tabPrintDialog::initReport);
    progressUpdateTimer = new QTimer();

        m_printersList = QPrinterInfo::availablePrinterNames();
    ui->comboBoxPrinters->addItems(m_printersList);

    QList<int> l;
    // список типов отчетов, печатаемых на принтере стикеров:
    l << Global::Reports::sticker1 << Global::Reports::sticker2 << Global::Reports::sticker3 << Global::Reports::rep_label;
    selectPrinter(InList, l);

    // список типов отчетов, НЕ печатаемых на принтере документов (слип-чек не знаю на каком принтере должен печататься, поэтому тоже исключен):
    l << Global::Reports::slip;
    selectPrinter(NotInList, l);

    connect(ui->comboBoxPrinters, &QComboBox::currentTextChanged, this, &tabPrintDialog::setPrinter);   // подключение сигнал-слот именно здесь, чтобы избежать лишних вызовов слота при установке принтеров, сохранённых в настройках
    initPrinter(false);

    renderDelayTimer->start(100);
}

tabPrintDialog::~tabPrintDialog()
{
    delete ui;
    delete m_progressWidget;
    delete renderDelayTimer;
    delete progressUpdateTimer;
    if(m_report)
    {
        delete m_report;
        m_report = 0;
    }
    if(m_printer)
    {
        delete m_printer;
        m_printer = 0;
    }
    if(previewDelayTimer)
    {
        delete previewDelayTimer;
        previewDelayTimer = 0;
    }
}

bool tabPrintDialog::tabCloseRequest()
{
    return 1;
}

bool tabPrintDialog::loadReportTemplate(QByteArray *data)
{
    if (!m_report->loadFromByteArray(data))
        return 1;

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
    return 0;
}

bool tabPrintDialog::loadTmpReportTemplate(QString filename)
{
    if (!m_report->loadFromFile(filename))
        qDebug() << "Can't open file";
    previewWindow->refreshPages();

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);

    return 1;
}

void tabPrintDialog::setDefaultWidgetFocus()
{
    if (m_reportType == Global::Reports::rep_label) // при печати стикеров логично установить фокус в поле Кол-во
    {
        ui->spinBoxCopies->setFocus();
        ui->spinBoxCopies->selectAll();
    }
    else    // во всех остальных случаях фокус сразу на кнопке Печать
        ui->pushButtonPrint->setFocus();
}

QString tabPrintDialog::tabTitle()
{
    return "Print";
}

void tabPrintDialog::paintEvent(QPaintEvent *event)
{
    tabCommon::paintEvent(event);

    // если запустить рендеринг отчета отсюда, то обработчик событий будет ругаться на рекурсивный вызов PaintEvent
    // поэтому задействован таймер для отложенного запуска.
    // При отображении вкладки предпросмотра событие PainEvent вызывается не менее трёх раз, но т. к. достоверно неизвестно
    // точное ли это число вызовов, таймер кажется более надёжным; при каждом вызове этого метода таймер перезапускается
    // а спустя заданную задержку после последнего вызова будет произведён запуск рендеринга.
    if(!m_isReportRendered)
    {
        renderDelayTimer->start(20);
    }
}

bool tabPrintDialog::event(QEvent *ev)
{
    bool ret = tabCommon::event(ev);
    if(ev->type() == QEvent::ShowToParent)  // при переключении на вкладку нужно установить фокус на виджет (например, при печати документа, нажатием Пробел можно быстро запустить печать и не делать лишних телодвижений мышью)
    {
        setDefaultWidgetFocus();
    }
    return ret;
}

QStandardItemModel* tabPrintDialog::initDemoModel(QStringList &demoHeaders, QList<QVariant> &demoValues)
{
    QStandardItemModel *model = new QStandardItemModel();

    model->setHorizontalHeaderLabels(demoHeaders);
    foreach(QString item, demoHeaders)
    {
        QStandardItem *it = new QStandardItem(demoValues.at(demoHeaders.indexOf(item)).toString());
        model->setItem(0, demoHeaders.indexOf(item), it);
    }

    return model;
}

/* Загрузка ранее кэшированного файла отчета
 * Возвращает 1 в случае успеха
 */
bool tabPrintDialog::loadTemplateFromFile()
{
    QString fileName;
    QFile file;
    QByteArray fileContent;
    QSqlQuery query = QSqlQuery(QSqlDatabase::database("connMain"));

    if(!QFile::exists(fileName = QApplication::applicationDirPath() + "/reports/" + m_reportName + ".lrxml"))
    {
        if(!loadTemplateFromDB())
            return 0;
    }
    else    // если файл отчета кэширован ранее, нужно сравнить контрольные суммы
    {
        query.exec(QUERY_SEL_DOC_TEMPL_CHECKSUM(m_reportName));
        query.first();
        if(query.isValid())
        {
            file.setFileName(fileName);
            file.open(QIODevice::ReadOnly);
            fileContent = file.readAll();
            file.close();
            QCryptographicHash hash(QCryptographicHash::Sha1);
            hash.addData(fileContent);
            if(hash.result().toHex().toUpper() != query.value(0).toString())
            {
                if(!loadTemplateFromDB())
                    return 0;
            }
            else
            {
                if(!m_report->loadFromFile(fileName))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/* Загрузка бланка отчета из БД и кэширование
 * Возвращает 1 в случае успеха
 */
bool tabPrintDialog::loadTemplateFromDB()
{
    QString filePath = QApplication::applicationDirPath() + "/reports/";
    QString fileName = filePath + m_reportName + ".lrxml";
    QDir path;
    QFile file;
    QByteArray fileContent;
    QSqlQuery query = QSqlQuery(QSqlDatabase::database("connMain"));

    query.exec(QUERY_SEL_DOC_TEMPL_DATA(m_reportName));
    query.first();
    if(query.isValid())
    {
        fileContent = query.value(0).toByteArray();
        path.setPath(filePath);
        if (!path.exists())
            path.mkpath(filePath);
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadWrite))
        {
            qDebug() << "Не удалось сохранить файл бланка отчета на диск";
            return 0;
        }
        file.resize(fileContent.size());
        file.write(fileContent);
        file.close();
        if(!m_report->loadFromFile(fileName))
        {
            return 0;
        }
    }

    return 1;
}

void tabPrintDialog::initDataSources()
{
    try
    {
        switch(m_reportType)
        {
            case Global::Reports::new_rep: initRepairDataSources(); break;
            case Global::Reports::rep_label: initRepairStickerDataSources(); break;
            case Global::Reports::sticker1: initItemStickerDataSources(); break;
            case Global::Reports::pko: initPKODataSources(); break;
            default: return;
        }

        m_printer->setDocName(m_reportName);
    }
    catch (const QString &err)
    {
        errorHandler(err);
//        errorHandler(tr("Не удалось иницициализировать модели данных отчета"));
    }

#ifdef PRINT_DEBUG_PAGE_INFO
    qDebug() << "";
    qDebug() << "========== report page properties ==========";
    LimeReport::IPreparedPages::PageProps pageProperties = m_report->preparedPages()->pageProperties(0);
    qDebug() << "page->pageSize():" << pageProperties.pageSize;
    qDebug() << "page->geometry():" << pageProperties.geometry;
    qDebug() << "left:" << pageProperties.leftMargin << "; top:" << pageProperties.topMargin << "; right:" << pageProperties.rightMargin << "; bottom:" << pageProperties.bottomMargin;
#endif
}

void tabPrintDialog::initRepairDataSources()
{
    // TODO: реализовать дэмо-данные, как для sticker1
    // TODO: подумать над вариантом модели данных ремонта в виде списка полей со столбцами "название поля", "значение";
    // в этом случае шаблон отчета будет построен полностью на dataBand, а строки dataBand'а могут автоматичеки увеличиваться по высоте и,
    // в случае большого кол-ва данных, не придётся уменьшать размерт шрифта
    QSqlQueryModel *repairModel = new QSqlQueryModel();
    repairModel->setQuery(QUERY_SEL_REPAIR_RPRT(m_reportVars.value("repair_id").toString()), QSqlDatabase::database("connMain"));
    if(repairModel->lastError().isValid())
        throw tr("Не удалось иницициализировать модель данных ремонта");
    m_report->dataManager()->addModel("repair", repairModel, true);
    client_id = repairModel->record(0).value("client").toInt();

    QSqlQueryModel *customerModel = new QSqlQueryModel();
    customerModel->setQuery(QUERY_SEL_CLIENT_RPRT(repairModel->record(0).value("client").toInt()));
    if(customerModel->lastError().isValid())
        throw tr("Не удалось иницициализировать модель данных клиента");
    m_report->dataManager()->addModel("customer", customerModel, true);

    // из всех параметров для отёта пригодится только валюта
    // TODO: заменить системное обозначение валюты на валюту заданную в таблице БД config
    QStandardItemModel *configModel = new QStandardItemModel();
    QStringList headers = {"currency"};
    configModel->setHorizontalHeaderLabels(headers);
//        qDebug() << "sysLocale.currencySymbol: " << sysLocale.currencySymbol(QLocale::CurrencySymbol);  // QLocale::CurrencyDisplayName почему-то возвращает "рубль": язык интерф. Win 10 Русский (Россия), ден. ед. в настройках ОС - "грн.", Qt v6.2.1
    configModel->appendRow(new QStandardItem(sysLocale.currencySymbol(QLocale::CurrencySymbol)));
    m_report->dataManager()->addModel("config", configModel, true);

    QSqlQueryModel *fieldsModel = new QSqlQueryModel();
    fieldsModel->setQuery(QUERY_SEL_REP_FIELDS_RPRT(m_reportVars.value("repair_id").toString()));
    if(fieldsModel->lastError().isValid())
        throw tr("Не удалось иницициализировать модель данных дополнительных полей");
    m_report->dataManager()->addModel("additionalFields", fieldsModel, true);

    m_report->dataManager()->addModel("user", userDbDataModel, false);
    m_report->dataManager()->addModel("company", companiesModel, false);
    m_report->dataManager()->addModel("office", officesModel, false);
}

void tabPrintDialog::initRepairStickerDataSources()
{
    if (m_reportVars.contains("repair_id"))
    {
        QSqlQueryModel *repairModel = new QSqlQueryModel();
        repairModel->setQuery(QUERY_SEL_REPAIR_RPRT(m_reportVars.value("repair_id").toString()), QSqlDatabase::database("connMain"));
        if(repairModel->lastError().isValid())
            throw tr("Не удалось иницициализировать модель данных ремонта");
        m_report->dataManager()->addModel("repair", repairModel, true);
        client_id = repairModel->record(0).value("client").toInt();

        QSqlQueryModel *customerModel = new QSqlQueryModel();
        customerModel->setQuery(QUERY_SEL_CLIENT_RPRT(client_id));
        if(customerModel->lastError().isValid())
            throw tr("Не удалось иницициализировать модель данных клиента");
        m_report->dataManager()->addModel("customer", customerModel, true);

        m_report->dataManager()->addModel("user", userDbDataModel, false);
        m_report->dataManager()->addModel("company", companiesModel, false);
        m_report->dataManager()->addModel("office", officesModel, false);
    }
    else
    {   // Дэмо значения для стикера (если не задано значение repair_id)
        QStringList demoHeaders = {"id", "Title", "client", "serial_number", "company", "office", "start_office", "manager", "master", "in_date", "express_repair", "is_warranty", "is_repeat", "box", "box_name", "barcode", "early", "ext_early"};
        QList<QVariant> demoValues = {12345, "Моноблок (All-in-One PC) Apple iMac12,1  Mid 2011  A1311 (EMC 2428)", 6325, "C02POIWERUJD", 1, 1, 1, 33, 33, "2022-01-25 10:26:32", 0, 0, 0, "NULL", "NULL", "012000123452", "NULL", "NULL"};
        m_report->dataManager()->addModel("repair", initDemoModel(demoHeaders, demoValues), true);
        client_id = 6325;
        demoHeaders = QStringList{"id", "name", "surname", "patronymic", "type", "is_regular", "is_dealer"};
        demoValues = {6325, "Андрей", "Андреев", "Андреевич", 0, 0, 0};
        m_report->dataManager()->addModel("customer", initDemoModel(demoHeaders, demoValues), true);
        demoHeaders = QStringList{"id", "username", "name", "surname", "patronymic", "phone", "phone2", "email"};
        demoValues = {32, "manager", "Менеджер", "", "", "", "", ""};
        m_report->dataManager()->addModel("user", initDemoModel(demoHeaders, demoValues), true);
        demoHeaders = QStringList{"id", "type", "name", "inn", "kpp", "ogrn", "ur_address", "site", "email", "logo"};
        demoValues = {1, 1, "ЧП Рога и копыта", "1234567890", "1234", "5678", "туманность Андромеды, 1", "rik.com", "pr@rik.com", "NULL"};
        m_report->dataManager()->addModel("company", initDemoModel(demoHeaders, demoValues), true);
        demoHeaders = QStringList{"id", "name", "address", "phone", "phone2", "logo"};
        demoValues = {1, "Главный", "туманность Андромеды, 1", "123 456-78-90", "", "NULL"};
        m_report->dataManager()->addModel("office", initDemoModel(demoHeaders, demoValues), true);
    }

    if (m_reportVars.contains("copies"))
    {
        ui->spinBoxCopies->setValue(m_reportVars.value("copies").toInt());
    }
    else
    {
        ui->spinBoxCopies->setValue(comSettings->value("rep_stickers_copy").toInt());
    }
}

void tabPrintDialog::initItemStickerDataSources()
{
    if (m_reportVars.contains("ids_list"))
    {
        QSqlQueryModel *itemsModel = new QSqlQueryModel();
        itemsModel->setQuery(QString("SELECT CONCAT(LPAD(store_items.`id`, 6, '0'), '-', LPAD(store_items.`articul`, 6, '0')) AS 'UID', store_items.* FROM store_items WHERE `id` IN (%1);").arg(m_reportVars.value("ids_list").toString()), QSqlDatabase::database("connMain"));
        if(itemsModel->lastError().isValid())
            throw tr("Не удалось иницициализировать модель данных");
        m_report->dataManager()->addModel("items", itemsModel, true);
    }
    else
    {   // Дэмо значения для стикера (если не задано значение ids_list)
        QStringList demoHeaders = {"UID", "id", "Hidden", "articul", "dealer", "is_realization", "dealer_lock", "name", "state", "category", "store", "created", "updated", "count", "reserved", "units", "box", "box_name", "price_option", "custom_price_option", "currency_rate", "in_price", "in_price_base", "price", "price_base", "price2", "price2_base", "price3", "price3_base", "price4", "price4_base", "price5", "price5_base", "document", "part_request", "shop_title", "shop_description", "SN", "PN", "description", "shop_enable", "int_barcode", "ext_barcode", "in_count", "in_summ", "notes", "img1", "img2", "img3", "img4", "img5", "minimum_in_stock", "sold", "return_percent", "warranty", "warranty_dealer", "not_for_sale", "st_state", "st_notes", "ge_highlight", "last_stocktaking_date"};
        QList<QVariant> demoValues = {"016466-003790", 16466, 0, 3790, 1, 0, 0, "Гнездо micro USB, 2.0, на плату, Арт. 3790", 1, 82, 1, "2018-06-28 09:23:06", "NULL", 1, 0, 0, "NULL", "NULL", 1, 2, 26.19, 10.7629, 9.95, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 2535, "NULL", "", "", "", "Бахчев: тип 45", "Прежняя ПН: 2535\r\n", "NULL", "011000164662", "NULL", 2, 20, "Похож на Бахчев P/N тип 47\r\nарт. 3392 (без \"юбки\").\r\nПохож на Бахчев P/N тип 54 (без \"юбки\").\r\nПохож на Бахчев P/N тип 106\r\nарт. 4922 (без \"юбки\").\r\nПохож на Бахчев P/N тип 124 (без \"юбки\").\r\nВозможна замена на:", "NULL", "NULL", "NULL", "NULL", "NULL", 0, 1, 0, 0, 0, 0, 0, "NULL", 0, "NULL"};
        m_report->dataManager()->addModel("items", initDemoModel(demoHeaders, demoValues), true);
    }
}

void tabPrintDialog::initPKODataSources()
{
    QMessageBox msgBox;

//    if(->lastError().isValid())
//        throw tr("Не удалось иницициализировать модель данных ");
    msgBox.setWindowTitle(tr("Печать"));
    msgBox.setText(tr("Печать ПКО еще не реализована"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    deleteLater();
}

void tabPrintDialog::initReport()
{
    m_isReportRendered = 1;
    QObject::connect(progressUpdateTimer, &QTimer::timeout, this, &tabPrintDialog::updateProgressWidget);
    progressUpdateTimer->start(250);

    m_printer = new QPrinter(QPrinter::HighResolution);
    m_report = new LimeReport::ReportEngine();
    connect(m_report, &LimeReport::ReportEngine::renderStarted, this, &tabPrintDialog::reportRenderStarted);
    connect(m_report, &LimeReport::ReportEngine::renderFinished, this, &tabPrintDialog::reportRenderFinished);
    m_reportType = m_reportVars.value("type").toInt();
    m_reportName = Global::staticMetaObject.enumerator(Global::staticMetaObject.indexOfEnumerator("Reports")).valueToKey(m_reportType);

    setProgressText("Loading file");
    if (!loadTemplateFromFile())
        errorHandler(tr("Не удалось найти файл отчета"));
    setProgressText("Initializing datasources");
    initDataSources();

    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
    ui->pushButtonPrint->setStyleSheet("QPushButton:hover {border: 1px solid #0078D7; background: solid #E5F1FB;}\n"
                                       "QPushButton:focus {border: 2px solid #0078D7;}");   // чтобы было видно, что кнопка в фокусе

    setProgressText("Renderind report");
    m_report->prepareReportPages();
}

void tabPrintDialog::showPreview()
{
    // Проверяется размер страницы сгенерированного отчета (при отсутствующих страницах будет возвращён 0).
    // Выглядит так себе, но вроде ничего не падает.
    // TODO: доработать LimeReport: сделать метод count публичным или придумать еще какой-то способ
    if(m_report->preparedPages()->pageProperties(0).geometry.width())
    {
        previewDelayTimer->stop();
        m_previewWidget = m_report->createPreviewWidget(1);

        ui->gridLayoutTab->replaceWidget(m_progressWidget, m_previewWidget);
        m_progressWidget->hide();
        ui->gridLayoutTab->setColumnStretch(1, 1);
        ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
    }
}

void tabPrintDialog::initProgressWidget()
{
    m_progressWidget = new QLabel(this);
    QFont f;
    f.setFamily("Calibri");
    f.setPointSize(24);
    f.setBold(true);
    m_progressWidget->setFont(f);
    ui->gridLayoutTab->addWidget(m_progressWidget, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
    setProgressText("Wait");
}

void tabPrintDialog::setProgressText(const QString &text)
{
    if(text.isEmpty())
        return;

    QFontMetrics fm(m_progressWidget->font());
    m_progressWidgetStaticText = text;
    m_progressWidget->setIndent((QApplication::activeWindow()->width() - ui->gridLayoutTab->columnMinimumWidth(0) - fm.size(Qt::TextSingleLine, m_progressWidgetStaticText + "   ").width())/2);
    updateProgressWidget();
}

void tabPrintDialog::updateProgressWidget()
{
    QString currentText = m_progressWidget->text().right(3);
    int i = QString(currentText).trimmed().length();

    if(i >= 3)
        i = -1;

    QString newText = m_progressWidgetStaticText + QString(".").repeated(i+1).leftJustified(3, ' ');
    m_progressWidget->setText(newText);
}

void tabPrintDialog::on_pushButtonPrint_clicked()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;

    if (!ui->lineEditPagesToPrint->text().isEmpty())
    {
        m_printer->setPrintRange(QPrinter::PrintRange(2));
        QRegularExpression rx("\\d+-?(\\d)*");
        QValidator *validator = new QRegularExpressionValidator (rx, this);
        ui->lineEditPagesToPrint->setValidator(validator);
        if(ui->lineEditPagesToPrint->hasAcceptableInput())
        {
            QStringList pages = ui->lineEditPagesToPrint->text().split('-');
            if (pages.count() == 1)
            {
                m_printer->setFromTo(pages.at(0).toInt(), pages.at(0).toInt());
            }
            else
            {
                m_printer->setFromTo(pages.at(0).toInt(), pages.at(1).toInt());
            }
        }
        else
            qDebug() << "unacceptable input";

        delete validator;
    }

    m_printer->setCopyCount(ui->spinBoxCopies->value());
    previewWindow->print(m_printer);
    if (m_reportType == Global::Reports::new_rep)
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,userDbData->value("id").toInt(),userDbData->value("current_office").toInt(),client_id,m_reportVars.value("repair_id").toInt(),"NULL","NULL","NULL",tr("Печать квитанции к ремонту №%1").arg(m_reportVars.value("repair_id").toInt())));
    }
    else if (m_reportType == Global::Reports::rep_label)
    {
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,userDbData->value("id").toInt(),userDbData->value("current_office").toInt(),client_id,m_reportVars.value("repair_id").toInt(),"NULL","NULL","NULL",tr("Печать стикеров к ремонту №%1 в кол-ве %2шт.").arg(m_reportVars.value("repair_id").toInt()).arg(ui->spinBoxCopies->value())));
    }

    delete query;
}

void tabPrintDialog::setPrinter(const QString &)
{
    // очистка проприетарных данных драйвера принтера перед инициализацией
    if(driverExtraData)
        memset(driverExtraData, 0xFF, driverExtraDataSize);
    driverExtraData = nullptr;
    initPrinter(false);
}

/* https://stackoverflow.com/questions/40043507/qt-5-8-windows-printer-api-invalid-handle-error */
void tabPrintDialog::initPrinter(bool showSettings)
{
    QString printerName;
    QPrinterInfo pi;
    bool pageSetResult;

    printerName = ui->comboBoxPrinters->currentText();
    pi = QPrinterInfo::printerInfo(printerName);
    if(pi.isNull())
        return;

    m_printer->setPrinterName(printerName);

#ifdef Q_OS_WINDOWS
    printerName = printerName+"\0";
    LPWSTR pname = reinterpret_cast<wchar_t *>(printerName.data());
    HANDLE hPrinter = NULL;
    LPDEVMODE pDevMode;
    DWORD dwNeeded = 0, dwRet;
    if(!OpenPrinter(pname, &hPrinter, NULL))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Не удалось вызвать драйвер принтера"));
        msgBox.exec();
    }
    /* Step 1:
    * Allocate a buffer of the correct size.
    */
    dwNeeded = DocumentProperties((HWND)effectiveWinId(),
        hPrinter, /* Handle to our printer. */
        pname, /* Name of the printer. */
        NULL, /* Asking for size, so */
        NULL, /* these are not used. */
        0); /* Zero returns buffer size. */

    pDevMode = (LPDEVMODE)malloc(dwNeeded);

    /* Step 2:
    * Get the default DevMode for the printer and
    * modify it for your needs.
    */
    dwRet = DocumentProperties( (HWND)effectiveWinId(),
                                hPrinter,
                                pname,
                                pDevMode, /* The address of the buffer to fill. */
                                NULL, /* Not using the input buffer. */
                                DM_OUT_BUFFER); /* Have the output buffer filled. */
    if (dwRet != IDOK)
    {
        /* If failure, cleanup and return failure. */
        free(pDevMode);
        ClosePrinter(hPrinter);
        return;
    }

    // вызов окна расширенной настройки параметров печати; это диалоговое окно, оно блокирует работу всей ОС (в т. ч. такое же поведение при вызове этого диалога, например, в Word)
    // если пользователь нажмёт "Отмена", то функция вернёт 0
    if(showSettings)
    {
        pDevMode->dmPaperWidth = m_printer->pageRect(QPrinter::Millimeter).width() * 10;
        pDevMode->dmPaperLength = m_printer->pageRect(QPrinter::Millimeter).height() * 10;
        pDevMode->dmOrientation = (m_printer->pageLayout().orientation() == QPageLayout::Portrait)?DMORIENT_PORTRAIT:DMORIENT_LANDSCAPE;
        if(m_printer->pageLayout().pageSize().id() != QPageSize::Custom)    // попытка установить размер листа по id
        {
            pDevMode->dmPaperSize = PageSizeMap.value(m_printer->pageLayout().pageSize().id());
        }
        else
        {
            pDevMode->dmPaperSize = DMPAPER_USER;
        }
        pDevMode->dmPrintQuality = m_printer->resolution();
        memcpy(((unsigned char *)(pDevMode) + pDevMode->dmSize), driverExtraData, driverExtraDataSize); // восстановление данных из буфера

        // размер страницы принтера по умолчанию. Например, для Rongta RP400 размер по умолчанию 600 900 (или наоборот), а id страницы - 9 (т. е. A4).
        qDebug() << "[before] LxW =" << pDevMode->dmPaperLength << pDevMode->dmPaperWidth << "; id=" << pDevMode->dmPaperSize;
        AdvancedDocumentProperties( (HWND)effectiveWinId(),
                                        hPrinter,
                                        pname,
                                        pDevMode,
                                        pDevMode);
    }
    else
    {
        pDevMode->dmOrientation = (m_report->preparedPages()->pageProperties().orientation == QPageLayout::Portrait)?DMORIENT_PORTRAIT:DMORIENT_LANDSCAPE;   // ориентация по умолчанию в соответствии с параметрами страницы отчета
    }

    if(pDevMode->dmPaperSize < DMPAPER_USER)    // попытка установить размер листа по id
        pageSetResult = m_printer->setPageSize(QPageSize((QPageSize::PageSizeId)PageSizeMap.key(pDevMode->dmPaperSize)));
    if(pDevMode->dmPaperSize == DMPAPER_USER || !pageSetResult) // если размер изначально пользовательский или не удалось установить по id
        m_printer->setPageSize( \
                              QPageSize(
                                    ((pDevMode->dmOrientation==DMORIENT_PORTRAIT)?\
                                        QSizeF(
                                            (pDevMode->dmPaperWidth / 10),
                                            (pDevMode->dmPaperLength / 10)):\
                                        QSizeF(
                                            (pDevMode->dmPaperLength / 10),
                                            (pDevMode->dmPaperWidth / 10))),
                                    QPageSize::Millimeter,
                                    QString(),
                                    QPageSize::ExactMatch)); // изменённый размер страницы
    m_printer->setPageOrientation((pDevMode->dmOrientation==DMORIENT_PORTRAIT)?QPageLayout::Portrait:QPageLayout::Landscape);       // ориентация в соответствии с параметрами драйвера
    m_printer->setResolution(pDevMode->dmPrintQuality);
    driverExtraDataSize = (int)pDevMode->dmDriverExtra;

    // инициализация буфера проприетарных данных драйвера
    if(driverExtraData == nullptr)
        driverExtraData = (unsigned char*)malloc(driverExtraDataSize);
    memcpy(driverExtraData, ((unsigned char *)(pDevMode) + pDevMode->dmSize), driverExtraDataSize);

    if( m_report->preparedPages()->pageProperties(0).widthMM > m_printer->paperRect(QPrinter::Millimeter).width() ||
        m_report->preparedPages()->pageProperties(0).heightMM > m_printer->paperRect(QPrinter::Millimeter).height() )
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Размер страницы отчета больше размера страницы принтера"));
        msgBox.exec();
    }

    free(pDevMode);
    if(!ClosePrinter(hPrinter))
    {
        qDebug() << "ClosePrinter failed";
        return;
    }
    CloseHandle(hPrinter);
#else
    m_printer->setPageSize(pi.defaultPageSize());   // defaultPageSize() по сути создаёт новый объект QPageSize, не привязанный ни к какому принтеру. В Win API вообще не замечено элементов, обозначающих  отступы страницы
    m_printer->setDuplex(pi.defaultDuplexMode());   // TODO: dmDuplex
    m_printer->setColorMode(pi.defaultColorMode()); // TODO: dmColor
#endif

    if(1)   // TODO: если dropPrinterMargins == 1; фактически это нужно только для отображения корректной отладочной информации, т. к. установка отступов при установленно флаге выполняется в LimeReport
    {
        m_printer->setFullPage(true); // без этой настройки установить отступы равными 0 у некоторых принтеров нельзя (Send to OneNote 2013 и Bullzip PDF...)
        if( !m_printer->setPageMargins(QMarginsF(0,0,0,0)) )
        {
            qDebug() << "m_printer->setPageMargins failed.";
        }
    }

#ifdef PRINT_DEBUG_PAGE_INFO
    qDebug() << "";
    if(showSettings)
        qDebug() << "=========== Changed printer params ==========";
    else
        qDebug() << "=========== Selected printer params ==========";
//    qDebug() << "supportedPageSizes" << pi.supportedPageSizes();
#endif
    fillDebugData();
}

void tabPrintDialog::errorHandler(const QString &msg)
{
    appLog->appendRecord(msg);
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void tabPrintDialog::selectPrinter(const BelongReportsList belong, const QList<int> list)
{
    if(list.contains(m_reportType) == belong)
    {
        QString printerName = belong?userLocalData->StickersPrinter.value:userLocalData->DocsPrinter.value;
        int rdpKeywordPos = -1;
        QRegularExpression rdpKeywordRegExp(tr("\\(перенаправлено \\d\\)"));  // Microsoft RDP перенаправленным принтерам добавляет "(перенаправлено N)"; от сессии к сессии номер может меняться и чтобы не приходилось как в АСЦ изменять принтеры по умолчанию в настройках ...
        rdpKeywordPos = userLocalData->StickersPrinter.value.indexOf(rdpKeywordRegExp);    // вернёт -1 если не найдёт
        if(rdpKeywordPos >= 0)  // если будет найден паттерн rdpKeywordRegExp, то и поиск индекса принтера будет происходить по рег. выражению
            ui->comboBoxPrinters->setCurrentIndex( m_printersList.indexOf(QRegularExpression(printerName.left(rdpKeywordPos)+
                                                                        rdpKeywordRegExp.pattern())) );
        else                    // иначе по простой строке (чтобы в случае содержащихся в названии принтера спец. символов рег. выражений корректно определялся индекс)
            ui->comboBoxPrinters->setCurrentIndex( m_printersList.indexOf(printerName));
    }
}

void tabPrintDialog::on_labelPrinterSettings_linkActivated(const QString&)
{
    initPrinter(true);
}

void tabPrintDialog::fillDebugData()
{
    // Вывод отладочной инфы в TextEdit
    ui->textEdit->clear();
    ui->textEdit->append(QString("==== report params ===="));
//    ui->textEdit->append(QString("dropPrinterMargins = %1 | fullPage = %2").arg(m_report->preparedPages()->pageProperties().dropPrinterMargins).arg(m_report->preparedPages()->pageProperties().fullPage));
//    ui->textEdit->append(QString("oldPrintMode = %1 | endlessHeight = %2").arg(m_report->preparedPages()->pageProperties().oldPrintMode).arg(m_report->preparedPages()->pageProperties().endlessHeight));
    ui->textEdit->append(QString("pageSize = %1x%2 | orientation %3")\
                         .arg(m_report->preparedPages()->pageProperties().widthMM)\
                         .arg(m_report->preparedPages()->pageProperties().heightMM)\
                         .arg(m_report->preparedPages()->pageProperties().orientation));
    ui->textEdit->append(QString(" setPageSizeToPrinter = %1 | printBehavior = %2")\
                         .arg(m_report->preparedPages()->pageProperties().isSetPageSizeToPrinter)\
                         .arg("N/A"/*m_report->preparedPages()->pageProperties().printBehavior*/));
    ui->textEdit->append(QString("==== printer params ===="));
    ui->textEdit->append(QString("paperSize: QRectF(%1, %2, %3x%4 | orientation %5")\
                         .arg(m_printer->paperRect(QPrinter::Millimeter).left())\
                         .arg(m_printer->paperRect(QPrinter::Millimeter).top())\
                         .arg(m_printer->paperRect(QPrinter::Millimeter).width())\
                         .arg(m_printer->paperRect(QPrinter::Millimeter).height())\
                         .arg((QPageLayout::Orientation)m_printer->pageLayout().orientation()));
    ui->textEdit->append(QString("pageSize: QRectF(%1, %2, %3x%4)")\
                         .arg(m_printer->pageRect(QPrinter::Millimeter).left())\
                         .arg(m_printer->pageRect(QPrinter::Millimeter).top())\
                         .arg(m_printer->pageRect(QPrinter::Millimeter).width())\
                         .arg(m_printer->pageRect(QPrinter::Millimeter).height())\
                         );

#ifdef PRINT_DEBUG_PAGE_INFO
    qDebug() << "printerName():                     " << m_printer->printerName();
    qDebug() << "pageLayout():                      " << m_printer->pageLayout();
    qDebug() << "outputFormat() :                   " << m_printer->outputFormat();
    qDebug() << "pageRect(QPrinter::Millimeter):    " << m_printer->pageRect(QPrinter::Millimeter);
    qDebug() << "paperRect(QPrinter::Millimeter):   " << m_printer->paperRect(QPrinter::Millimeter);
    qDebug() << "resolution():                      " << m_printer->resolution();
//    qDebug() << "supportedPageSizes():              " << pi.supportedPageSizes();
//    if(pi.supportsCustomPageSizes()) (qDebug() << "supportsCustomPageSizes");
#endif
}

void tabPrintDialog::pageSetupAccepted()
{
}

void tabPrintDialog::reportRenderStarted()
{
}

void tabPrintDialog::reportRenderFinished()
{
    // эмуляция задержки
//    QTime dieTime= QTime::currentTime().addSecs(2);
//    while (QTime::currentTime() < dieTime)
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    progressUpdateTimer->stop();

    // Сразу после завершения рендеринга страницы не доступны и виджет предпросмотра оказывается пустым.
    // Чтобы этого избежать был придуман костыль. С заданным интервалом производится вызов метода, заменяющего виджет
    // прогресса виджетом предпросмотра.
    previewDelayTimer = new QTimer();
    QObject::connect(previewDelayTimer, &QTimer::timeout, this, &tabPrintDialog::showPreview);
    previewDelayTimer->start(50);
}
