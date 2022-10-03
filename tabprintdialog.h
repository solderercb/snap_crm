#ifndef TABPRINTDIALOG_H
#define TABPRINTDIALOG_H

#include <QWidget>
#include <LimeReport>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPageSetupDialog>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "tabcommon.h"
#include "mainwindow.h"
#include <Windows.h>
#include <QMessageBox>
#include <QMetaEnum>
#include <QCryptographicHash>
//#define PRINT_DEBUG_PAGE_INFO

namespace Ui {
class tabPrintDialog;
}

class tabPrintDialog : public tabCommon
{
    Q_OBJECT


public:
    explicit tabPrintDialog(MainWindow *parent, QMap<QString, QVariant> rv = {{"type","dummy"}});
    ~tabPrintDialog();
    virtual bool tabCloseRequest();
    bool loadReportTemplate(QByteArray*);
    bool loadTemplateFromFile();
    bool loadTemplateFromDB();
    bool initReportDataSources();
    bool loadTmpReportTemplate(QString);
    QMap<QString, QVariant> report_vars;
    void setDefaultWidgetFocus();
    QString tabTitle() override;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
    QMap<int, int> PageSizeMap = {
        {QPageSize::Letter, DMPAPER_LETTER}, {QPageSize::LetterSmall, DMPAPER_LETTERSMALL},
        {QPageSize::Tabloid, DMPAPER_TABLOID}, {QPageSize::Ledger, DMPAPER_LEDGER}, {QPageSize::Legal, DMPAPER_LEGAL},
        {QPageSize::Statement, DMPAPER_STATEMENT}, {QPageSize::Executive, DMPAPER_EXECUTIVE}, {QPageSize::A3, DMPAPER_A3},
        {QPageSize::A4, DMPAPER_A4}, {QPageSize::A4Small, DMPAPER_A4SMALL}, {QPageSize::A5, DMPAPER_A5},
        {QPageSize::B4, DMPAPER_B4}, {QPageSize::B5, DMPAPER_B5}, {QPageSize::Folio, DMPAPER_FOLIO},
        {QPageSize::Quarto, DMPAPER_QUARTO}, {QPageSize::Imperial10x14, DMPAPER_10X14}, {QPageSize::Note, DMPAPER_NOTE},
        {QPageSize::Envelope9, DMPAPER_ENV_9}, {QPageSize::Comm10E, DMPAPER_ENV_10}, {QPageSize::Envelope11, DMPAPER_ENV_11},
        {QPageSize::Envelope12, DMPAPER_ENV_12}, {QPageSize::Envelope14, DMPAPER_ENV_14}, {QPageSize::DLE, DMPAPER_ENV_DL},
        {QPageSize::C5E, DMPAPER_ENV_C5}, {QPageSize::EnvelopeC3, DMPAPER_ENV_C3}, {QPageSize::EnvelopeC4, DMPAPER_ENV_C4},
        {QPageSize::EnvelopeC6, DMPAPER_ENV_C6}, {QPageSize::EnvelopeC65, DMPAPER_ENV_C65},
        {QPageSize::EnvelopeB4, DMPAPER_ENV_B4}, {QPageSize::EnvelopeB5, DMPAPER_ENV_B5},
        {QPageSize::EnvelopeB6, DMPAPER_ENV_B6}, {QPageSize::EnvelopeItalian, DMPAPER_ENV_ITALY},
        {QPageSize::EnvelopeMonarch, DMPAPER_ENV_MONARCH}, {QPageSize::EnvelopePersonal, DMPAPER_ENV_PERSONAL},
        {QPageSize::FanFoldUS, DMPAPER_FANFOLD_US}, {QPageSize::FanFoldGerman, DMPAPER_FANFOLD_STD_GERMAN},
        {QPageSize::FanFoldGermanLegal, DMPAPER_FANFOLD_LGL_GERMAN}, {QPageSize::Imperial10x11, DMPAPER_10X11},
        {QPageSize::Imperial15x11, DMPAPER_15X11}, {QPageSize::EnvelopeInvite, DMPAPER_ENV_INVITE},
        {QPageSize::LetterExtra, DMPAPER_LETTER_EXTRA}, {QPageSize::LegalExtra, DMPAPER_LEGAL_EXTRA},
        {QPageSize::TabloidExtra, DMPAPER_TABLOID_EXTRA}, {QPageSize::A4Extra, DMPAPER_A4_EXTRA},
        {QPageSize::LetterPlus, DMPAPER_LETTER_PLUS}, {QPageSize::A4Plus, DMPAPER_A4_PLUS},
        {QPageSize::A3Extra, DMPAPER_A3_EXTRA}, {QPageSize::A5Extra, DMPAPER_A5_EXTRA}, {QPageSize::B5Extra, DMPAPER_B5_EXTRA},
        {QPageSize::A2, DMPAPER_A2}, {QPageSize::A6, DMPAPER_A6}, {QPageSize::EnvelopeKaku2, DMPAPER_JENV_KAKU2},
        {QPageSize::EnvelopeKaku3, DMPAPER_JENV_KAKU3}, {QPageSize::EnvelopeChou3, DMPAPER_JENV_CHOU3},
        {QPageSize::EnvelopeChou4, DMPAPER_JENV_CHOU4}, {QPageSize::JisB6, DMPAPER_B6_JIS},
        {QPageSize::Imperial12x11, DMPAPER_12X11}, {QPageSize::EnvelopeYou4, DMPAPER_JENV_YOU4},
        {QPageSize::Prc16K, DMPAPER_P16K}, {QPageSize::Prc32K, DMPAPER_P32K}, {QPageSize::Prc32KBig, DMPAPER_P32KBIG},
        {QPageSize::EnvelopePrc1, DMPAPER_PENV_1}, {QPageSize::EnvelopePrc2, DMPAPER_PENV_2},
        {QPageSize::EnvelopePrc3, DMPAPER_PENV_3}, {QPageSize::EnvelopePrc4, DMPAPER_PENV_4},
        {QPageSize::EnvelopePrc5, DMPAPER_PENV_5}, {QPageSize::EnvelopePrc6, DMPAPER_PENV_6},
        {QPageSize::EnvelopePrc7, DMPAPER_PENV_7}, {QPageSize::EnvelopePrc8, DMPAPER_PENV_8},
        {QPageSize::EnvelopePrc9, DMPAPER_PENV_9}, {QPageSize::EnvelopePrc10, DMPAPER_PENV_10}, {QPageSize::Custom, DMPAPER_USER} };
#else
    QMap<int, int> PageSizeMap = {
        {QPrinter::Letter, DMPAPER_LETTER}, {QPrinter::Tabloid, DMPAPER_TABLOID}, {QPrinter::Ledger, DMPAPER_LEDGER},
        {QPrinter::Legal, DMPAPER_LEGAL}, {QPrinter::Executive, DMPAPER_EXECUTIVE}, {QPrinter::A4, DMPAPER_A4},
        {QPrinter::A5, DMPAPER_A5}, {QPrinter::B4, DMPAPER_B4}, {QPrinter::B5, DMPAPER_B5}, {QPrinter::Folio, DMPAPER_FOLIO},
        {QPrinter::B6, DMPAPER_B5_TRANSVERSE}, {QPrinter::A6, DMPAPER_A6} };
#endif
private:
    Ui::tabPrintDialog *ui;
    QString report_type;
    int client_id;
    LimeReport::ReportEngine *report;
    LimeReport::ReportDesignWindowInterface* designerWindow;
    QByteArray *DataFile;
    QFile CurrentFile;
    LimeReport::PreviewReportWidget *previewWindow;
    QPrinter *printer;
    QStandardItemModel* initDemoModel(QStringList &, QList<QVariant> &);
    bool event(QEvent*);
    void printerAdvSettings(QPrinter *printer = nullptr);
    void fillDebugData();
    void initPrinter(bool showSettings = false);
#ifdef QT_DEBUG
    void randomFill(){};
#endif
#ifdef Q_OS_WINDOWS
    unsigned char *driverExtraData = nullptr;     // проприетарные данные драйвера
    int driverExtraDataSize = 0;
#endif

private slots:
    void some_func();
    void on_pushButtonPrint_clicked();
    void setPrinter(const QString &printerName);
    void on_labelPrinterSettings_linkActivated(const QString &link);
    void pageSetupAccepted();
#ifdef QT_DEBUG
    void test_scheduler_handler(){};
    void test_scheduler2_handler(){};
#endif
};

#endif // TABPRINTDIALOG_H
