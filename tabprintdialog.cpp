#include "global.h"
#include "tabprintdialog.h"
#include "ui_tabprintdialog.h"
#include "com_sql_queries.h"

tabPrintDialog::tabPrintDialog(MainWindow *parent, Global::Reports type) :
    tabCommon(parent),
    ui(new Ui::tabPrintDialog)
{
    m_reportType = type;    // инициализация до вызова любых методов

//    tabCommon::logUserActivity(); // нужно вообще?

    ui->setupUi(this);

    initProgressWidget();
    logRecord = new SLogRecordModel();

    m_printersList = QPrinterInfo::availablePrinterNames();
    ui->comboBoxPrinters->addItems(m_printersList);

    connect(ui->comboBoxPrinters, &QComboBox::currentTextChanged, this, &tabPrintDialog::setPrinter);   // подключение сигнал-слот именно здесь, чтобы избежать лишних вызовов слота при установке принтеров, сохранённых в настройках
}

tabPrintDialog::tabPrintDialog(MainWindow *parent, QMap<QString, QVariant> report_vars):
    tabPrintDialog(parent, (Global::Reports)report_vars.value("type").toInt())
{
    m_reportVars = report_vars;
    startRender();
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
    delete logRecord;
}

void tabPrintDialog::startRender()
{
    renderDelayTimer = new QTimer();
    renderDelayTimer->setSingleShot(true);
    connect(renderDelayTimer, &QTimer::timeout, this, &tabPrintDialog::initReport);
    progressUpdateTimer = new QTimer();

    renderDelayTimer->start(100);
}

tabPrintDialog *tabPrintDialog::create(Global::Reports type)
{
    MainWindow *mw = MainWindow::getInstance();
    QTabWidget *tw = mw->findChild<QTabWidget*>();
    tabPrintDialog *subwindow = new tabPrintDialog(mw, type);
    tw->addTab(subwindow, subwindow->tabTitle());
    tw->setCurrentWidget(subwindow);
    return subwindow;
}

bool tabPrintDialog::tabCloseRequest()
{
    if(m_reportState != ReportState::RenderingFinished)
        return 0;
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
    return tr("Печать %1").arg(tr(Global::staticMetaObject.enumerator(Global::staticMetaObject.indexOfEnumerator("Reports")).valueToKey(m_reportType)));
}

void tabPrintDialog::paintEvent(QPaintEvent *event)
{
    tabCommon::paintEvent(event);

    // если запустить рендеринг отчета отсюда, то обработчик событий будет ругаться на рекурсивный вызов PaintEvent
    // поэтому задействован таймер для отложенного запуска.
    // При отображении вкладки предпросмотра событие PainEvent вызывается не менее трёх раз, но т. к. достоверно неизвестно
    // точное ли это число вызовов, таймер кажется более надёжным; при каждом вызове этого метода таймер перезапускается
    // а спустя заданную задержку после последнего вызова будет произведён запуск рендеринга.
    if(m_reportState == ReportState::New)
    {
        renderDelayTimer->start(20);
    }
}

bool tabPrintDialog::event(QEvent *ev)
{
    bool ret = tabCommon::event(ev);

    if(m_reportState < ReportState::RenderingFinished)
        return ret;

    if(ev->type() == QEvent::ShowToParent)  // при переключении на вкладку нужно установить фокус на виджет (например, при печати документа, нажатием Пробел можно быстро запустить печать и не делать лишних телодвижений мышью)
    {
        setDefaultWidgetFocus();
    }
    return ret;
}

void tabPrintDialog::initDataSources()
{
    SReportsCommonFunctions::initDataSources();

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
    SReportsCommonFunctions::initRepairDataSources();
}

void tabPrintDialog::initRepairStickerDataSources()
{
    SReportsCommonFunctions::initRepairStickerDataSources();
    logRecord->setType(SLogRecordModel::Repair);
    logRecord->setRepairId(m_reportVars.value("repair_id").toInt());
}

void tabPrintDialog::notImplementedReport()
{
    QMessageBox msgBox;

    qDebug().nospace() << "[" << this << "] initDataSources() | not implemented report type: " << m_reportName;
    msgBox.setWindowTitle(tr("Печать"));
    msgBox.setText(tr("Печать <%1> еще не реализована").arg(m_reportName));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    deleteLater();
}

void tabPrintDialog::initReport()
{
    m_reportState = ReportState::Initialized;
    QObject::connect(progressUpdateTimer, &QTimer::timeout, this, &tabPrintDialog::updateProgressWidget);
    progressUpdateTimer->start(250);

    m_report = new LimeReport::ReportEngine(this);
    connect(m_report, &LimeReport::ReportEngine::renderStarted, this, &tabPrintDialog::reportRenderStarted);
    connect(m_report, &LimeReport::ReportEngine::renderFinished, this, &tabPrintDialog::reportRenderFinished);
    m_reportName = Global::staticMetaObject.enumerator(Global::staticMetaObject.indexOfEnumerator("Reports")).valueToKey(m_reportType);

    setTemplateName(m_reportName);
    setProgressText("Initializing datasources");
    initDataSources();
    setProgressText("Loading file");
    loadTemplateFromFile();

    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
    ui->pushButtonPrint->setStyleSheet("QPushButton:hover {border: 1px solid #0078D7; background: solid #E5F1FB;}\n"
                                       "QPushButton:focus {border: 2px solid #0078D7;}");   // чтобы было видно, что кнопка в фокусе

    setProgressText("Renderind report");
    m_report->prepareReportPages();
}

void tabPrintDialog::showPreview()
{
    if(isPagesPrepared())
    {
        previewDelayTimer->stop();
        m_previewWidget = m_report->createPreviewWidget(1);

        ui->gridLayoutTab->replaceWidget(m_progressWidget, m_previewWidget);
        m_progressWidget->hide();
        ui->gridLayoutTab->setColumnStretch(1, 1);
        ui->gridLayoutTab->setColumnMinimumWidth(0, 200);

        // Подключение сигнала только сейчас!
        QObject::connect(ui->pushButtonPrint, &QPushButton::clicked, this, &tabPrintDialog::pushButtonPrintClicked);
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
    int windowWidth = MainWindow::getInstance()->width();
//    int windowWidth = QApplication::activeWindow()->width(); // Оставлю для истории: это приводит к падению, если вызов происходит, когда окно программы не в фокусе
    m_progressWidgetStaticText = text;
    m_progressWidget->setIndent((windowWidth - ui->gridLayoutTab->columnMinimumWidth(0) - fm.size(Qt::TextSingleLine, m_progressWidgetStaticText + "   ").width())/2);
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

bool tabPrintDialog::isPagesPrepared()
{
    // Проверяется размер страницы сгенерированного отчета (при отсутствующих страницах будет возвращён 0).
    // Выглядит так себе, но вроде ничего не падает.
    // TODO: доработать LimeReport: сделать метод count публичным или придумать еще какой-то способ
    if(m_report->preparedPages()->pageProperties(0).geometry.width())
        return 1;

    return 0;
}

void tabPrintDialog::pushButtonPrintClicked()
{
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
    shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Отправлено на печать"), QColor(212,237,242), QColor(229,244,247));
    m_previewWidget->print(m_printer);
    switch(m_reportType)
    {
        case Global::Reports::new_rep: logRecord->setText(tr("Печать квитанции к ремонту №%1").arg(m_reportVars.value("repair_id").toInt())); logRecord->commit(); break;
        case Global::Reports::rep_label: logRecord->setText(tr("Печать стикеров к ремонту №%1 в кол-ве %2шт.").arg(m_reportVars.value("repair_id").toInt()).arg(ui->spinBoxCopies->value())); logRecord->commit(); break;
        default: ;
    }
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
    setProgressText("Initializing printer");
    QString printerName;
    QPrinterInfo pi;
    bool pageSetResult;

    if(m_printer)
        delete m_printer;

    m_printer = new QPrinter(QPrinter::HighResolution);
    printerName = ui->comboBoxPrinters->currentText();
    m_printer->setDocName(m_reportName);
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
    else if(isPagesPrepared())
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

/* Установка принтеров по умолчанию (заданных в настройках пользователя)
*/
void tabPrintDialog::selectPrinter()
{
    QList<int> l;
    // список типов отчетов, печатаемых на принтере стикеров:
    l << Global::Reports::sticker1 << Global::Reports::sticker2 << Global::Reports::sticker3 << Global::Reports::rep_label;
    selectPrinter(InList, l);

    // список типов отчетов, НЕ печатаемых на принтере документов (слип-чек не знаю на каком принтере должен печататься, поэтому тоже исключен):
    l << Global::Reports::slip;
    selectPrinter(NotInList, l);

}

void tabPrintDialog::selectPrinter(const BelongReportsList belong, const QList<int> list)
{
    if(list.contains(m_reportType) == belong)
    {
        QString printerName = belong?userLocalData->StickersPrinter.value:userLocalData->DocsPrinter.value;
        ui->comboBoxPrinters->setCurrentIndex(findPrinterIndex(m_printersList, printerName));
    }
}

int tabPrintDialog::findPrinterIndex(const QStringList &list, const QString &pName)
{
    int rdpKeywordPos = -1;
    QRegularExpression rdpKeywordRegExp(tr("\\(перенаправлено \\d\\)"));  // Microsoft RDP перенаправленным принтерам добавляет "(перенаправлено N)"; от сессии к сессии номер может меняться и чтобы не приходилось как в АСЦ изменять принтеры по умолчанию в настройках ...
    rdpKeywordPos = pName.indexOf(rdpKeywordRegExp);    // вернёт -1 если не найдёт

    if(rdpKeywordPos >= 0)  // если будет найден паттерн rdpKeywordRegExp, то и поиск индекса принтера будет происходить по рег. выражению
        return list.indexOf(QRegularExpression(pName.left(rdpKeywordPos)+
                                                                    rdpKeywordRegExp.pattern()));
    else                    // иначе по простой строке (чтобы в случае содержащихся в названии принтера спец. символов рег. выражений корректно определялся индекс)
        return list.indexOf(pName);
}

void tabPrintDialog::printRepairWorksReports(QList<SRepairModel *> list, bool takeOwn)
{
    if(list.isEmpty())
        return;

    tabPrintDialog *tab;

    tab = new tabPrintDialog(MainWindow::getInstance(), Global::Reports::works);
    tab->setRepairsList(list, takeOwn);
    tab->startRender();
    MainWindow::getInstance()->addTab(tab);
}

void tabPrintDialog::setRepairsList(QList<SRepairModel *> list, bool takeOwn)
{
    if(list.isEmpty())
        return;

    i_clientId = list.at(0)->clientId();    // Печать нескольких актов за раз возможна только при выдаче, а она может быть произведена только с ремонтами от одного клиента
    if(!i_listSourceDataCallbackHandler)
        i_listSourceDataCallbackHandler = new SListSourceDataCallbackHandler();

    i_listSourceDataCallbackHandler->setRepairsList(list, takeOwn);
}

void tabPrintDialog::setTechReportsList(QList<STechReportModel *> list, bool takeOwn)
{
    if(list.isEmpty())
        return;

    if(!i_listSourceDataCallbackHandler)
        i_listSourceDataCallbackHandler = new SListSourceDataCallbackHandler();

    i_listSourceDataCallbackHandler->setTechReportsList(list, takeOwn);
}

/* Чек-квитанция о приёме на заправку и чек-акт отличаются только выводом на печать списка работ и деталей
*/
void tabPrintDialog::printCartridgeWorksReports(QList<SRepairModel *> list, bool takeOwn)
{
    if(list.isEmpty())
        return;

    SPrintPOSReport *report = new SPrintPOSReport();
    SClientModel *client = new SClientModel();
    client->load(list.at(0)->clientId());

    report->setClientModel(client);
    report->openPrinter(userLocalData->PosPrinter.value);
    QList<SRepairModel*>::const_iterator i = list.constBegin();
    while(i != list.constEnd())
    {
        // Если запрошена печать чека-акта, а стоимость ремонта равна 0 (и, соответственно, список работ пуст), то такой чек не печатается вообще
        // во всех остальных случаях производится или печать чека-акта или печать чека-квитанции
        switch( (((*i)->realRepairCost() == 0) << 7) | (*i)->state() )
        {
            case (1 << 7) | Global::RepStateIds::Returned:
            case (1 << 7) | Global::RepStateIds::ReturnedNoRepair:
            case (1 << 7) | Global::RepStateIds::ReturnedInCredit: break;
            default: report->addPrintJob(*i);
        }
        if(takeOwn)
            delete *i;
        i++;
    }
    report->closePrinter();

    delete report;
    delete client;
}

void tabPrintDialog::printTechReports(QList<STechReportModel *> list, bool takeOwn)
{
    if(list.isEmpty())
        return;

    tabPrintDialog *tab;

    tab = new tabPrintDialog(MainWindow::getInstance(), Global::Reports::tech_report);
    tab->setTechReportsList(list, takeOwn);
    tab->startRender();
    MainWindow::getInstance()->addTab(tab);

}

void tabPrintDialog::on_labelPrinterSettings_linkActivated(const QString&)
{
//    initPrinter(true);
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
    m_reportState = ReportState::RenderingInProcess;
}

void tabPrintDialog::reportRenderFinished()
{
    m_reportState = ReportState::RenderingFinished;
    // эмуляция задержки
//    QTime dieTime= QTime::currentTime().addSecs(2);
//    while (QTime::currentTime() < dieTime)
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    initPrinter(false);

    selectPrinter();

    progressUpdateTimer->stop();
    setDefaultWidgetFocus();

    // Сразу после завершения рендеринга страницы не доступны и виджет предпросмотра оказывается пустым.
    // Чтобы этого избежать был придуман костыль. С заданным интервалом производится вызов метода, заменяющего виджет
    // прогресса виджетом предпросмотра.
    previewDelayTimer = new QTimer();
    QObject::connect(previewDelayTimer, &QTimer::timeout, this, &tabPrintDialog::showPreview);
    previewDelayTimer->start(50);

    emit renderFinished();
}

/* Перевод названий отчетов (заголовок вкладки предпросмотра и текст записи в таблице `users_activity`)
 * Подставляемое значение присоединяется к "Печать ...", например "Печать квитанции", "Печать стикера"
 * Желательно использовать короткие названия
 */
void tabPrintDialog::translate()
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

