#include "spagesettingsdoctempateeditor.h"
#include "ui_spagesettingsdoctempateeditor.h"
#include <QFileInfo>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SAppLog>
#include <FlashPopup>
#include <SSqlQueryModel>

SPageSettingsDocTempateEditor::SPageSettingsDocTempateEditor(QWidget *parent) :
    STabPage(parent),
    ui(new Ui::SPageSettingsDocTempateEditor)
{
    ui->setupUi(this);
    if(!m_parentTab)
        m_parentTab = findParentTab();

    ui->pushButtonImport->hide();
    ui->pushButtonExport->hide();
    ui->lineEdit->setButtons("Apply");
    ui->lineEdit->hide();   // TODO: реализовать переименование

    docTemplatesList = new SSqlQueryModel();
    docTemplatesList->setQuery(QUERY_SEL_DOC_TEMPLATES_LIST, QSqlDatabase::database(TdConn::main()));
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
    connect(m_report, &LimeReport::ReportEngine::onSave, this, [=]{manualSubmit();});

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
    // Редактор не знает о результатах сохранения и после сбоя операции запрос подтверждения выведен не будет.
    // Однако, ситуация из категории очень редких и делать её обработчик смысла мало.
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

QString SPageSettingsDocTempateEditor::queryLogFile()
{
    return m_parentTab->metaObject()->className();
}

int SPageSettingsDocTempateEditor::checkInput()
{
    m_templateBuffer = std::make_unique<QByteArray>(m_report->saveToByteArray());
    auto tmp = std::make_unique<QString>(hash(m_templateBuffer.get()));

    // сигнал onSave генерируется дважды, вторая итерация будет пропущена
    if(m_templateHash.get() && m_templateHash->compare(*tmp) == 0)
    {
        m_templateHash.reset(); // нужно игнорировать только повторно сгенерированный сигнал, но после сбоя должна производиться попытка сохранения
        return 1;
    }

    m_templateHash = std::move(tmp);

    // Сравнение хэшей и пропуск операции если они совпадают
    // Примечание: при первом открытии редактора шаблонов нажатие кнопки Сохранить приведёт к
    // перезаписи данных в БД (выведется сообщение) даже если не вносились изменения. Это происходит
    // потому, что механизм (де-)сериализации данных не сохраняет порядок аттрибутов в тэгах XML
    // при открытии/сохранении шаблона. Т. е. дизайн шаблона не изменяется, но хэш будет отличаться.
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    query->exec(QUERY_SEL_DOC_TEMPL_CHECKSUM(m_reportName));
    query->first();
    if(query->isValid())
    {
        if(m_templateHash->compare(query->value(0).toString(), Qt::CaseInsensitive) == 0)
            return 1;
    }

    // Временный файл для контроля записанных данных. Запись должна производиться именно сейчас.
    m_temporaryFile = std::make_unique<QFile>();
    m_temporaryFile->setFileName(m_templatesDir + m_reportName + ".lrxml." + QDateTime::currentDateTime().toString("yyMMdd-hhmmss"));
    writeFile(*m_temporaryFile, m_templateBuffer.get());

    return 0;
}

void SPageSettingsDocTempateEditor::commit(const int)
{
    auto query = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::session()));

    QUERY_EXEC_TH(query, 1, QUERY_UPD_DOC_TEMPL_DATA(m_reportName, QString(m_templateBuffer->toHex()), *m_templateHash));
    QUERY_EXEC_TH(query, 1, QUERY_SEL_DOC_TEMPL_DATA(m_reportName));

    query->first();
    auto vrfyBuffer  = std::make_unique<QByteArray>(query->value(0).toByteArray());
    if(m_templateHash.get() != hash(vrfyBuffer.get()))
    {
        Global::throwError(Global::ThrowType::IntegrityError, tr("Error occurred while saving report template to DB: hashes mismatch detected"));
    }
}

void SPageSettingsDocTempateEditor::throwHandler(int)
{
    appLog->appendRecord(tr("   Changes saved to temporary file %1").arg(QFileInfo(m_temporaryFile->fileName()).fileName()));
}

void SPageSettingsDocTempateEditor::endCommit()
{
    new shortlivedNotification(this, tr("Успешно"), tr("Шаблон сохранен в БД"), QColor(214,239,220), QColor(229,245,234));

    if(CurrentFile.exists())
        CurrentFile.remove();
    if(!m_temporaryFile->rename(CurrentFile.fileName()))
        Global::errorMsg(Global::ThrowType::DiskWriteError, tr("Шаблон отчёта"));

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

/* Дэмо-данные
*/
void SPageSettingsDocTempateEditor::initWorksDataSources()
{
    if(!i_listSourceDataCallbackHandler)
        i_listSourceDataCallbackHandler = new SListSourceDataCallbackHandler();

    SReportsCommonFunctions::initWorksDataSources();
}

void SPageSettingsDocTempateEditor::initTechReportDataSources()
{
    if(!i_listSourceDataCallbackHandler)
        i_listSourceDataCallbackHandler = new SListSourceDataCallbackHandler();

    SReportsCommonFunctions::initTechReportDataSources();
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
