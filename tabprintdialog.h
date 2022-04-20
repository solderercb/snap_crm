#ifndef TABPRINTDIALOG_H
#define TABPRINTDIALOG_H

#include <QWidget>
#include <LimeReport>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QPrinterInfo>
#include <QPageSetupDialog>
#include <QByteArray>
#include <QFile>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "tabcommon.h"
#include "mainwindow.h"
#include <Windows.h>

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
    bool selectTemplateFile();
    bool loadTemplateFromFile();
    bool loadTemplateFromDB();
    bool initReportDataSources();
    bool loadTmpReportTemplate(QString);
    QMap<QString, QVariant> report_vars;
    void setDefaultWidgetFocus();

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

private slots:
    void some_func();
    void on_pushButtonPrint_clicked();
    void on_comboBoxPrinters_currentTextChanged(const QString &arg1);
    void on_labelPrinterSettings_linkActivated(const QString &link);
    void pageSetupAccepted();
};

#endif // TABPRINTDIALOG_H
