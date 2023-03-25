#ifndef SPAGESETTINGSDOCTEMPATEEDITOR_H
#define SPAGESETTINGSDOCTEMPATEEDITOR_H

#include "widgets/stabpage.h"
#include <LimeReport>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QCryptographicHash>
#include <QStringListModel>
#include <QIODevice>
#include "models/ssqlquerymodel.h"
#include "models/sclientmodel.h"

namespace Ui {
class SPageSettingsDocTempateEditor;
}

class SPageSettingsDocTempateEditor : public STabPage
{
    Q_OBJECT

public:
    explicit SPageSettingsDocTempateEditor(QWidget *parent = nullptr);
    ~SPageSettingsDocTempateEditor();
    bool pageCloseRequest() override;
private:
    Ui::SPageSettingsDocTempateEditor *ui;
    QString m_templatesDir;
    SSqlQueryModel* docTemplatesList;
    QByteArray *fileBuf;
//    QString report_type;
    QFile CurrentFile;
    QFile tmpFile;
    QVector<QSqlQueryModel> *dataSources;
    QStandardItemModel *test_model;
    QSqlQueryModel *itemsModel;
    LimeReport::ReportEngine *m_report;
    LimeReport::ReportDesignWindowInterface *reportDesigner;
    int m_reportType;
    QString m_reportName;
    bool loadTemplateFromFile();
    bool saveTemplateToFile();
    bool loadTemplateFromDB();
    bool saveTemplateToDB();
    QByteArray* readFile(QFile &file);
    bool writeFile(QFile &file, QByteArray *data = nullptr);
    QString hash(QByteArray *data);
    QStandardItemModel* initDemoModel(const QStringList &demoHeaders, const QList<QVariant> &demoValues);
    void initDataSources();
    void initRepairDataSources();
    void initRepairStickerDataSources();
    void initItemStickerDataSources();
#ifdef QT_DEBUG
    void randomFill();
#endif
public slots:
    void selectTemplate(int index);
protected slots:
#ifdef QT_DEBUG
    void test_scheduler_handler();
    void test_scheduler2_handler();
#endif
private slots:
    void reportSaved();
    void reportOnSave(bool&);
};

#endif // SPAGESETTINGSDOCTEMPATEEDITOR_H
