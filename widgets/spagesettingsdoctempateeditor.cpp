#include "spagesettingsdoctempateeditor.h"
#include "ui_spagesettingsdoctempateeditor.h"
#include "global.h"
#include "com_sql_queries.h"

SPageSettingsDocTempateEditor::SPageSettingsDocTempateEditor(QWidget *parent) :
    STabPage(parent),
    ui(new Ui::SPageSettingsDocTempateEditor)
{
    ui->setupUi(this);
    ui->pushButtonImport->hide();
    ui->pushButtonExport->hide();
    ui->lineEdit->setButtons("Apply");
    ui->lineEdit->hide();   // TODO: реализовать переименование

    docTemplatesList = new SSqlQueryModel();
    docTemplatesList->setQuery(QUERY_SEL_DOC_TEMPLATES_LIST, QSqlDatabase::database("connMain"));
    docTemplatesList->setObjectName("docTemplates");
//    QMetaEnum docTemplates = Global::staticMetaObject.enumerator(Global::staticMetaObject.indexOfEnumerator("Reports"));
//    docTemplatesList = SStandardItemModel::modelFromEnum(docTemplates, tr);
    ui->comboBox->blockSignals(true);
    ui->comboBox->setModel(docTemplatesList);
//    qobject_cast<QListView *>(ui->comboBox->view())->setRowHidden(docTemplatesList->rowCount() - 1, true);  // скрыть элемент not_impl
    ui->comboBox->setCurrentIndex(-1);
    ui->comboBox->blockSignals(false);
//    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &SPageSettingsDocTempateEditor::selectTemplate); // такой формат здесь почему-то не подходит (ошибка)
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectTemplate(int)));

    m_report = new LimeReport::ReportEngine();
    reportDesigner = m_report->getDesignerWindow();

//    connect(m_report, SIGNAL(saveFinished()), this, SLOT(reportSaved()));
    connect(m_report, SIGNAL(onSave(bool&)), this, SLOT(reportOnSave(bool&)));

    ui->scrollArea->setWidget(reportDesigner);
#ifdef QT_DEBUG
    test_scheduler->start(200);
#endif
}

SPageSettingsDocTempateEditor::~SPageSettingsDocTempateEditor()
{
    delete ui;
    delete reportDesigner;
    delete m_report;
}

bool SPageSettingsDocTempateEditor::pageCloseRequest()
{
    return reportDesigner->checkNeedToSave();
}

void SPageSettingsDocTempateEditor::selectTemplate(int index)
{
    removeReportDataSources();
    m_reportName = docTemplatesList->index(index, 1).data().toString();
    m_reportType = Global::staticMetaObject.enumerator(Global::staticMetaObject.indexOfEnumerator("Reports")).keyToValue(m_reportName.toLocal8Bit());
//    m_reportType = docTemplatesList->index(index, 1).data().toInt();
//    m_reportName = Global::staticMetaObject.enumerator(Global::staticMetaObject.indexOfEnumerator("Reports")).valueToKey(m_reportType);

    setTemplateName(m_reportName);

    initDataSources();
    loadTemplateFromFile();
}

bool SPageSettingsDocTempateEditor::saveTemplateToDB()
{
    QByteArray buffer = m_report->saveToByteArray();
    QByteArray vrfyBuffer;
    QSqlQuery query = QSqlQuery(QSqlDatabase::database("connMain"));
    QFile tmpFile;
    QString hashsum = hash(&buffer);

    query.exec(QUERY_SEL_DOC_TEMPL_CHECKSUM(m_reportName));
    query.first();
    if(query.isValid())
        // вызов метода происходит по сигналу onSave и т. к. сигнал генерируется дважды нет смысла второй раз писать данные
        if(hashsum.toUpper() == query.value(0).toString())
            return 1;

    // временный файл для контроля записанных данных
    tmpFile.setFileName(m_templatesDir + m_reportName + ".lrxml." + QDateTime::currentDateTime().toString("yyMMdd-hhmmss"));
    if(!writeFile(tmpFile, &buffer))
        return 0;

    query.exec("BEGIN;");
    query.exec(QUERY_UPD_DOC_TEMPL_DATA(m_reportName, QString(buffer.toHex()), hashsum));
    if(query.lastError().isValid())
    {
        qDebug().nospace() << "[" << this << "] saveTemplateToDB() | " << query.lastError().text();
        appLog->appendRecord(query.lastError().text());
        return 0;
    }
    query.exec(QUERY_SEL_DOC_TEMPL_DATA(m_reportName));
    query.first();
    if(!query.isValid())
        return 0;

    vrfyBuffer = query.value(0).toByteArray();
    if(hashsum != hash(&vrfyBuffer))
    {
        QString err = "Error occured while saving report template to DB: probably data corruption, hashes mismatch. Queries was rollbacked.";
        qDebug().nospace() << "[" << this << "] saveTemplateToDB() | " << err;
        appLog->appendRecord(err);
        query.exec("ROLLBACK;");
        return 0;
    }
    query.exec("COMMIT;");

    tmpFile.remove();

    if(!writeFile(CurrentFile, &buffer))
        return 0;

    return 1;
}

void SPageSettingsDocTempateEditor::removeReportDataSources()
{
    for(QString &ds : m_reportDatasouces)
    {
        if(!m_report->dataManager()->containsDatasource(ds))
            continue;

        m_report->dataManager()->removeModel(ds);
        m_reportDatasouces.removeAt(m_reportDatasouces.indexOf(ds));
    }
}

/* Перевод названий шаблонов (Настройки -> Шаблоны документов)
 * Полные названия шаблонов отчетов, например, "Приходный кассовый ордер", "Акт выполненных работ"
 * TODO: Требуется доработка, учитывающая переименования
 */
void SPageSettingsDocTempateEditor::translate()
{
    tr("new_rep");
    tr("pko");
    tr("reject");
    tr("rko");
    tr("sticker1");
    tr("sticker2");
    tr("sticker3");
    tr("rep_label");
    tr("warranty");
    tr("works");
    tr("diag");
    tr("pn");
    tr("rn");
    tr("lost");
    tr("price1");
    tr("invoice0");
    tr("invoice1");
    tr("invoice2");
    tr("invoice3");
    tr("vatinvoice0");
    tr("p_list0");
    tr("w_list0");
    tr("new_cartridge");
    tr("sticker_cartridge");
    tr("issue_cartridge");
    tr("slip");
    tr("move");
    tr("buyout");
    tr("repairs");
    tr("cartridges");
}

void SPageSettingsDocTempateEditor::reportSaved()
{   // Этот сигнал посылается когда файл отчета уже сохранён (перезаписан) (контролировал путём сравнения контрольной суммы с оригинальным)
//    qDebug().nospace() << "[" << this << "] reportSaved()";
}

void SPageSettingsDocTempateEditor::reportOnSave(bool&)
{   // Этот сигнал посылается при нажатии кнопки "Сохранить", но до того как файл будет перезаписан (контролировал путём сравнения контрольной суммы с оригинальным)
    // оставлю для истории
//    qDebug().nospace() << "[" << this << "] reportOnSave() | saved = " << saved;

    // здесь хитро получилось...  поскольку дизайнер загружает шаблон из QByteArray, то при сохранении сигнал OnSave генерируется дважды.
    // А т. к. после первого раза я сохранил на диске копию, то диалог сохранения файла не отображается.
    // ВНИМАНИЕ! Также, после сохранения файла на диск с пом. Save as... (т. е. экспорта) и до закрытия вкладки настроек, любое внесенное изменение сохранится и в БД и в экспортированный файл.
    // В таком поведении есть и плюсы и минусы. Например, если стоит цель создать резервную копию перед выполнением пробных изменений, нужно переименовать файл на диске для защиты от перезаписи.
    // TODO: реализовать в LimeReport Disigner функцию Save as copy...
    saveTemplateToDB();
}

#ifdef QT_DEBUG
void SPageSettingsDocTempateEditor::randomFill()
{

}

void SPageSettingsDocTempateEditor::test_scheduler_handler()
{
    ui->comboBox->setCurrentIndex(0);
}

void SPageSettingsDocTempateEditor::test_scheduler2_handler()
{

}
#endif

