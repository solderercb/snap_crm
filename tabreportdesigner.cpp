#include "tabreportdesigner.h"
#include "ui_tabreportdesigner.h"

tabReportDesigner::tabReportDesigner(MainWindow *parent, QString type) :
    tabCommon(parent),
    ui(new Ui::tabReportDesigner),
    report_type(type)
{
    ui->setupUi(this);
    tmpFile.setFileName(QApplication::applicationDirPath() + "\\template.tmp");         // временный файл для редактирования (для того, чтобы работала кнопка Save)
    dataSources = new QVector<QSqlQueryModel>;
    report = new LimeReport::ReportEngine;
    reportDesigner = report->getDesignerWindow();

    connect(report, SIGNAL(saveFinished()), this, SLOT(reportSaved()));
//    connect(report, SIGNAL(onSave(bool&)), this, SLOT(reportOnSave(bool&)));

    initReportDataSources();
    loadTemplateFromFile();

    ui->gridLayoutTab->addWidget(reportDesigner,0,0);
}

tabReportDesigner::~tabReportDesigner()
{
    delete ui;
//    delete fileBuf;
    delete report;
}

bool tabReportDesigner::tabCloseRequest()
{
    qDebug() << "(reimplemented) tabCloseRequest()";
    return reportDesigner->checkNeedToSave();
}

bool tabReportDesigner::selectTemplateFile()
{
    if (report_type == "new_rep")
        CurrentFile.setFileName(QApplication::applicationDirPath() + "\\reports\\priemka.lrxml");
    else if (report_type == "sticker1")
        CurrentFile.setFileName(QApplication::applicationDirPath() + "\\reports\\24x14.lrxml");
//    else if (report_type == "")
//            loadTemplateFromFile(QApplication::applicationDirPath() + "\\reports\\priemka.lrxml");
//    else if (report_type == "")
//            loadTemplateFromFile(QApplication::applicationDirPath() + "\\reports\\priemka.lrxml");
//    else if (report_type == "")
//            loadTemplateFromFile(QApplication::applicationDirPath() + "\\reports\\priemka.lrxml");
}

bool tabReportDesigner::loadTemplateFromFile()
{
    fileBuf = new QByteArray;

    selectTemplateFile();

    if(!CurrentFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open .lrxml";
    *fileBuf = CurrentFile.readAll();
    CurrentFile.close();
    if (!tmpFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Can't open new file";
    }
    tmpFile.write(*fileBuf);
    tmpFile.close();
    report->loadFromFile(tmpFile.fileName());
}

bool tabReportDesigner::saveTemplateToFile()
{
    fileBuf = new QByteArray;
    if(!tmpFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open temp file";
    *fileBuf = tmpFile.readAll();
    tmpFile.close();

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(*fileBuf);
    qDebug() << hash.result().toHex();

    selectTemplateFile();

    if (!CurrentFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Can't open .lrxml";
    }
    CurrentFile.write(*fileBuf);
    CurrentFile.close();
}

bool tabReportDesigner::initReportDataSources()
{
    if (report_type == "new_rep")
    {
        int repair = 24996;
        QStandardItemModel *model = new QStandardItemModel();
        // назвиния полей:
        QStringList demoHeaders = {"UID", "id", "Hidden", "articul", "dealer", "is_realization", "dealer_lock", "name", "state", "category", "store", "created", "updated", "count", "reserved", "units", "box", "box_name", "price_option", "custom_price_option", "currency_rate", "in_price", "in_price_base", "price", "price_base", "price2", "price2_base", "price3", "price3_base", "price4", "price4_base", "price5", "price5_base", "document", "part_request", "shop_title", "shop_description", "SN", "PN", "description", "shop_enable", "int_barcode", "ext_barcode", "in_count", "in_summ", "notes", "img1", "img2", "img3", "img4", "img5", "minimum_in_stock", "sold", "return_percent", "warranty", "warranty_dealer", "not_for_sale", "st_state", "st_notes", "ge_highlight", "last_stocktaking_date"};
        // значения полей:
        QList<QVariant> demoValues = {"016466-003790", 16466, 0, 3790, 1, 0, 0, "Гнездо micro USB, 2.0, на плату, Арт. 3790", 1, 82, 1, "2018-06-28 09:23:06", NULL, 1, 0, 0, NULL, NULL, 1, 2, 26.19, 10.7629, 9.95, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 22, 19.9, 2535, NULL, "", "", "", "Бахчев: тип 45", "Прежняя ПН: 2535\r\n", NULL, "011000164662", NULL, 2, 20, "Похож на Бахчев P/N тип 47\r\nарт. 3392 (без \"юбки\").\r\nПохож на Бахчев P/N тип 54 (без \"юбки\").\r\nПохож на Бахчев P/N тип 106\r\nарт. 4922 (без \"юбки\").\r\nПохож на Бахчев P/N тип 124 (без \"юбки\").\r\nВозможна замена на:", NULL, NULL, NULL, NULL, NULL, 0, 1, 0, 0, 0, 0, 0, NULL, 0, NULL};
        model->setHorizontalHeaderLabels(demoHeaders);
        foreach(QString item, demoHeaders)
        {
            QStandardItem *it = new QStandardItem(demoValues.at(demoHeaders.indexOf(item)).toString());
            model->setItem(0, demoHeaders.indexOf(item), it);
        }
        report->dataManager()->addModel("string_list2", model, true);

        QStringList simpleData;
        simpleData << "value1" << "value2" << "value3";

        QStringListModel* stringListModel = new QStringListModel();
        stringListModel->setStringList(simpleData);
        stringListModel->setHeaderData(0, Qt::Vertical, 111);
        report->dataManager()->addModel("string_list",stringListModel,true);

        QSqlQueryModel *repairModel = new QSqlQueryModel();
        repairModel->setQuery(QString("SELECT t2.PrepaidTypeStr, workshop.* FROM workshop LEFT JOIN (SELECT \"полная предоплата\" AS 'PrepaidTypeStr', 0 AS 'id' UNION SELECT \"за детали\", 1 UNION SELECT \"за часть стоимости деталей\", 2 UNION SELECT \"за часть стоимости работ\", 3 UNION SELECT \"за диагностику\", 4 ) AS `t2` ON workshop.prepaid_type = t2.`id` WHERE workshop.`id` = %1;").arg(repair), QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("repair", repairModel, true);

        QSqlQueryModel *customerModel = new QSqlQueryModel();
        customerModel->setQuery(QString("SELECT t1.*, GROUP_CONCAT(t2.phone ORDER BY t2.`type` DESC SEPARATOR ',') AS 'phone', IF(t1.`type` = 1, t1.`ur_name`, CONCAT(t1.`surname`, ' ', t1.`name`, ' ', t1.`patronymic`)) AS 'FioOrUrName' FROM clients as t1 LEFT JOIN tel AS t2 ON t1.id = t2.customer WHERE t1.`id` = %1 GROUP BY t1.`id`;").arg(repairModel->record(0).value("client").toInt()), QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("customer", customerModel, true);

        QSqlQueryModel *userModel = new QSqlQueryModel();
        userModel->setQuery(QString("SELECT CONCAT(IF(users.`name` IS NULL OR users.`name` = '', '', users.surname), IF(users.`name` IS NULL OR users.`name` = '', '', CONCAT(LEFT(users.`name`, 1), '.')), IF(users.`patronymic` IS NULL OR users.`patronymic` = '', '', CONCAT(LEFT(users.`name`, 1), '.'))) AS 'fio', users.* FROM users WHERE `id` = %1;").arg(32), QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("user", userModel, true);

        QSqlQueryModel *companyModel = new QSqlQueryModel();
        companyModel->setQuery("SELECT * FROM companies WHERE `id` = 1;", QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("company", companyModel, true);

        QSqlQueryModel *officeModel = new QSqlQueryModel();
        officeModel->setQuery("SELECT * FROM offices WHERE `id` = 1;", QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("office", officeModel, true);

        QSqlQueryModel *configModel = new QSqlQueryModel();
        configModel->setQuery("SELECT * FROM config WHERE `id` = 1;", QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("config", configModel, true);

        QSqlQueryModel *fieldsModel = new QSqlQueryModel();
        fieldsModel->setQuery(QString("(SELECT 'Предоплата' AS 'name', CONCAT(ROUND(`prepaid_summ`, 0), '$D{config.currency}') AS 'value', `prepaid_type` AS 'comment' FROM workshop WHERE `id` = %1 AND `is_prepaid` = 1)\
                              UNION ALL\
                              (SELECT 'Ориентировочная стоимость ремонта', CONCAT(ROUND(`pre_agreed_amount`, 0), '$D{config.currency}'), '' FROM workshop WHERE `id` = %1 AND `is_pre_agreed` = 1)\
                              UNION ALL\
                              (SELECT '', 'Уст-во было в другом СЦ', '' FROM workshop WHERE `id` = %1 AND `thirs_party_sc` = 1)\
                              UNION ALL\
                              (SELECT t2.`name`, t1.`value`, '' AS 'comment' FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t2.`printable` = 1 AND t2.`archive` = 0 AND t1.`repair_id` = %1 ORDER BY t1.`field_id` ASC);\
                              ").arg(repair), QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("additionalFields", fieldsModel, true);

//        itemsModel = new QSqlQueryModel();
//        itemsModel->setQuery("", QSqlDatabase::database("connMain"));
//        report->dataManager()->addModel("", itemsModel, true);

    }
    else if (report_type == "sticker1")
    {
        itemsModel = new QSqlQueryModel();
        itemsModel->setQuery("SELECT CONCAT(LPAD(store_items.`id`, 6, '0'), '-', LPAD(store_items.`articul`, 6, '0')) AS 'UID', store_items.* FROM store_items WHERE `id` IN (16466,23452);", QSqlDatabase::database("connMain"));
        report->dataManager()->addModel("items", itemsModel, true);
    }
//    else if (report_type == "")
//    {
//    }

}

bool tabReportDesigner::loadTemplateFromDB()
{

}

bool tabReportDesigner::saveTemplateToDB()
{

}

void tabReportDesigner::reportSaved()
{   // Этот сигнал посылается когда файл отчета уже сохранён (перезаписан) (контролировал путём сравнения контрольной суммы с оригинальным)
    qDebug() << "received SIGNAL \"saveFinished()\"";
    saveTemplateToFile();
}

void tabReportDesigner::reportOnSave(bool &saved)
{   // Этот сигнал посылается при нажатии кнопки "Сохранить", но до того как файл будет перезаписан (контролировал путём сравнения контрольной суммы с оригинальным)
    // оставлю для истории
    qDebug() << "received SIGNAL \"onSave(bool&)\"";
    fileBuf = new QByteArray;
    if(!tmpFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open temp file";
    *fileBuf = tmpFile.readAll();
    tmpFile.close();
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(*fileBuf);
    qDebug() << hash.result().toHex();
}
