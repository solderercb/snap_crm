#include "global.h"
#include "tabprintdialog.h"
#include "ui_tabprintdialog.h"
#include "com_sql_queries.h"

tabPrintDialog::tabPrintDialog(MainWindow *parent, QMap<QString, QVariant> rv):
    tabCommon(parent),
    report_vars(rv),
    ui(new Ui::tabPrintDialog)
{
    ui->setupUi(this);
    printer = new QPrinter(QPrinter::HighResolution);
    report = new LimeReport::ReportEngine();
    report_type = report_vars.value("type").toString();
    if (!loadTemplateFromFile())
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Не удалось найти файл отчета"));
        msgBox.exec();
    }
    if (!initReportDataSources())
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Не удалось иницициализировать модели данных отчета"));
        msgBox.exec();
    }

    ui->comboBoxPrinters->addItems(QPrinterInfo::availablePrinterNames());
    if (userData->contains("DocsPrinter"))
    {
        qDebug() << "DocsPrinter: " << userLocalData->value("DocsPrinter").toString();
        ui->comboBoxPrinters->setCurrentText(userLocalData->value("DocsPrinter").toString());
    }
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
}

tabPrintDialog::~tabPrintDialog()
{
    delete ui;
    delete report;
    delete printer;
}

bool tabPrintDialog::tabCloseRequest()
{
    qDebug() << "(reimplemented) tabCloseRequest()";
    return 1;
}

bool tabPrintDialog::loadReportTemplate(QByteArray *data)
{
    if (!report->loadFromByteArray(data))
        return 1;

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
    return 0;
}

bool tabPrintDialog::loadTmpReportTemplate(QString filename)
{
    if (!report->loadFromFile(filename))
        qDebug() << "Can't open file";
    previewWindow->refreshPages();

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);

    return 1;
}

bool tabPrintDialog::selectTemplateFile()
{
    if (report_type == "new_rep")
        CurrentFile.setFileName(QApplication::applicationDirPath() + "/reports/priemka.lrxml");
    else if (report_type == "sticker1")
        CurrentFile.setFileName(QApplication::applicationDirPath() + "/reports/24x14.lrxml");
//    else if (report_type == "")
//            loadTemplateFromFile(QApplication::applicationDirPath() + "/demo_reports/priemka.lrxml");
//    else if (report_type == "")
//            loadTemplateFromFile(QApplication::applicationDirPath() + "/demo_reports/priemka.lrxml");
//    else if (report_type == "")
//            loadTemplateFromFile(QApplication::applicationDirPath() + "/demo_reports/priemka.lrxml");
    else
        return 0;

    return 1;
}

bool tabPrintDialog::loadTemplateFromFile()
{
    if (!selectTemplateFile())
        return 0;
    if(!report->loadFromFile(CurrentFile.fileName()))
    {
        return 0;
    }
    return 1;
}

bool tabPrintDialog::loadTemplateFromDB()
{

    return 1;
}

void tabPrintDialog::some_func()
{

    if (!report->loadFromFile(QApplication::applicationDirPath() + "/demo_reports/simple_list.lrxml"))
        qDebug() << "Can't open .lrxml";

}

bool tabPrintDialog::initReportDataSources()
{
//    QString report_type = report_vars.value("type").toString();
    printer->setDocName(report_type);

    if (report_type == "new_rep")
    {   // TODO: реализовать дэмо-данные, как для sticker1
        // TODO: подумать над вариантом модели данных ремонта в виде списка полей со столбцами "название поля", "значение";
        // в этом случае шаблон отчета будет построен полностью на dataBand, а строки dataBand'а могут автоматичеки увеличиваться по высоте и,
        // в случае большого кол-ва данных, не придётся уменьшать размерт шрифта
        QSqlQueryModel *repairModel = new QSqlQueryModel();
        repairModel->setQuery(QUERY_SEL_REPAIR_RPRT(report_vars.value("repair_id").toString()));
        report->dataManager()->addModel("repair", repairModel, true);
        client_id = repairModel->record(0).value("client").toInt();

        QSqlQueryModel *customerModel = new QSqlQueryModel();
        customerModel->setQuery(QUERY_SEL_CLIENT_RPRT(repairModel->record(0).value("client").toInt()));
        report->dataManager()->addModel("customer", customerModel, true);

        // из всех параметров для отёта пригодится только валюта
        // TODO: заменить системное обозначение валюты на валюту заданную в таблице БД config
        QStandardItemModel *configModel = new QStandardItemModel();
        QStringList headers = {"currency"};
        configModel->setHorizontalHeaderLabels(headers);
//        qDebug() << "sysLocale.currencySymbol: " << sysLocale.currencySymbol(QLocale::CurrencySymbol);  // QLocale::CurrencyDisplayName почему-то возвращает "рубль": язык интерф. Win 10 Русский (Россия), ден. ед. в настройках ОС - "грн.", Qt v6.2.1
        configModel->appendRow(new QStandardItem(sysLocale.currencySymbol(QLocale::CurrencySymbol)));
        report->dataManager()->addModel("config", configModel, true);

        QSqlQueryModel *fieldsModel = new QSqlQueryModel();
        fieldsModel->setQuery(QUERY_SEL_REP_FIELDS_RPRT(report_vars.value("repair_id").toString()));
        report->dataManager()->addModel("additionalFields", fieldsModel, true);

        report->dataManager()->addModel("user", userDataModel, false);
        report->dataManager()->addModel("company", companiesModel, false);
        report->dataManager()->addModel("office", officesModel, false);

//        QSqlQueryModel *itemsModel = new QSqlQueryModel();
//        itemsModel->setQuery("", QSqlDatabase::database("connMain"));
//        report->dataManager()->addModel("", itemsModel, true);

    }
    else if (report_type == "sticker1")
    {
        if (report_vars.contains("ids_list"))
        {
            QSqlQueryModel *itemsModel = new QSqlQueryModel();
            itemsModel->setQuery(QString("SELECT CONCAT(LPAD(store_items.`id`, 6, '0'), '-', LPAD(store_items.`articul`, 6, '0')) AS 'UID', store_items.* FROM store_items WHERE `id` IN (%1);").arg(report_vars.value("ids_list").toString()), QSqlDatabase::database("connMain"));
            report->dataManager()->addModel("items", itemsModel, true);
        }
        else
        {   // Дэмо значения для стикера (если не задано значение ids_list)
            QStandardItemModel *itemsModel = new QStandardItemModel();
            // назвиния полей:
            QStringList demoHeaders = {"UID", "id", "Hidden", "articul", "dealer", "is_realization", "dealer_lock", "name", "state", "category", "store", "created", "updated", "count", "reserved", "units", "box", "box_name", "price_option", "custom_price_option", "currency_rate", "in_price", "in_price_base", "price", "price_base", "price2", "price2_base", "price3", "price3_base", "price4", "price4_base", "price5", "price5_base", "document", "part_request", "shop_title", "shop_description", "SN", "PN", "description", "shop_enable", "int_barcode", "ext_barcode", "in_count", "in_summ", "notes", "img1", "img2", "img3", "img4", "img5", "minimum_in_stock", "sold", "return_percent", "warranty", "warranty_dealer", "not_for_sale", "st_state", "st_notes", "ge_highlight", "last_stocktaking_date"};
            // значения полей:
            QList<QVariant> demoValues = {"016466-003790", 16466, 0, 3790, 1, 0, 0, "Гнездо micro USB, 2.0, на плату, Арт. 3790", 1, 82, 1, "2018-06-28 09:23:06", "NULL", 1, 0, 0, "NULL", "NULL", 1, 2, 26.19, 10.7629, 9.95, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 2535, "NULL", "", "", "", "Бахчев: тип 45", "Прежняя ПН: 2535\r\n", "NULL", "011000164662", "NULL", 2, 20, "Похож на Бахчев P/N тип 47\r\nарт. 3392 (без \"юбки\").\r\nПохож на Бахчев P/N тип 54 (без \"юбки\").\r\nПохож на Бахчев P/N тип 106\r\nарт. 4922 (без \"юбки\").\r\nПохож на Бахчев P/N тип 124 (без \"юбки\").\r\nВозможна замена на:", "NULL", "NULL", "NULL", "NULL", "NULL", 0, 1, 0, 0, 0, 0, 0, "NULL", 0, "NULL"};
            itemsModel->setHorizontalHeaderLabels(demoHeaders);
            QVariant tmp = 123;
            foreach(QString item, demoHeaders)
            {
                QStandardItem *it = new QStandardItem(demoValues.at(demoHeaders.indexOf(item)).toString());
                itemsModel->setItem(0, demoHeaders.indexOf(item), it);
            }
            report->dataManager()->addModel("items", itemsModel, true);
        }
    }
//    else if (report_type == "")
//    {
//    }
    else
        return 0;

    previewWindow =  report->createPreviewWidget();

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);

    return 1;
}

void tabPrintDialog::on_pushButtonPrint_clicked()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;

    if (!ui->lineEditPagesToPrint->text().isEmpty())
    {
        printer->setPrintRange(QPrinter::PrintRange(2));
        QRegularExpression rx("\\d+-?(\\d)*");
        QValidator *validator = new QRegularExpressionValidator (rx, this);
        ui->lineEditPagesToPrint->setValidator(validator);
        if(ui->lineEditPagesToPrint->hasAcceptableInput())
        {
            QStringList pages = ui->lineEditPagesToPrint->text().split('-');
            if (pages.count() == 1)
            {
                printer->setFromTo(pages.at(0).toInt(), pages.at(0).toInt());
            }
            else
            {
                printer->setFromTo(pages.at(0).toInt(), pages.at(1).toInt());
            }
        }
        else
            qDebug() << "unacceptable input";

        delete validator;
    }

    printer->setCopyCount(ui->spinBoxCopies->value());
    report->printReport(printer);   // TODO: разобраться, что возвращает данная функция
    if (report_type == "new_rep")
        QUERY_EXEC(query,nDBErr)(QUERY_INS_LOG("NULL",3,userData->value("id").toInt(),userData->value("current_office").toInt(),client_id,report_vars.value("repair_id").toInt(),"NULL","NULL","NULL","Печать квитанции к ремонту №"+report_vars.value("repair_id").toString()));

    delete query;
}


void tabPrintDialog::on_comboBoxPrinters_currentTextChanged(const QString &arg1)
{
    QPrinterInfo pi = QPrinterInfo::printerInfo(arg1);
    printer->setPrinterName(arg1);
    printer->setPageSize(pi.defaultPageSize()); // дефолтный размер страницы выбранного принтера, без этой установки размер страницы может остаться от предыдущего принтера и отчет отрисуется неправильно
    printer->setDuplex(pi.defaultDuplexMode());
    printer->setColorMode(pi.defaultColorMode());
}


void tabPrintDialog::on_labelPrinterSettings_linkActivated(const QString &link)
{
    QPageSetupDialog dialog(printer);
    dialog.open(this, SLOT(pageSetupAccepted()));
}

void tabPrintDialog::pageSetupAccepted()
{
}
