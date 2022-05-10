#ifndef TABREPORTDESIGNER_H
#define TABREPORTDESIGNER_H

#include <LimeReport>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "tabcommon.h"
#include "mainwindow.h"
#include <QCryptographicHash>

namespace Ui {
class tabReportDesigner;
}

class tabReportDesigner : public tabCommon
{
    Q_OBJECT

public:
    explicit tabReportDesigner(MainWindow *parent = nullptr, QString type = "");
    ~tabReportDesigner();
    virtual bool tabCloseRequest();
    bool selectTemplateFile();
    bool loadTemplateFromFile();
    bool saveTemplateToFile();
    bool initReportDataSources();
    bool loadTemplateFromDB();
    bool saveTemplateToDB();
    QVector<QSqlQueryModel> *dataSources;
    QStandardItemModel *test_model;
    QSqlQueryModel *itemsModel;
    LimeReport::ReportEngine *report;
    LimeReport::ReportDesignWindowInterface *reportDesigner;

private:
    Ui::tabReportDesigner *ui;
    tabCommon *d_ptr;
    QByteArray *fileBuf;
    QString report_type;
    QFile CurrentFile;
    QFile tmpFile;

private slots:
    void reportSaved();
    void reportOnSave(bool&);
};

#endif // TABREPORTDESIGNER_H
