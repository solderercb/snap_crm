#ifndef SPAGESETTINGSDOCTEMPATEEDITOR_H
#define SPAGESETTINGSDOCTEMPATEEDITOR_H

#include "widgets/stabpage.h"
#include "widgets/sreportscommonfunctions.h"
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QIODevice>

namespace Ui {
class SPageSettingsDocTempateEditor;
}

class SPageSettingsDocTempateEditor : public STabPage, public SReportsCommonFunctions
{
    Q_OBJECT

public:
    explicit SPageSettingsDocTempateEditor(QWidget *parent = nullptr);
    ~SPageSettingsDocTempateEditor();
    bool pageCloseRequest() override;
private:
    Ui::SPageSettingsDocTempateEditor *ui;
    SSqlQueryModel* docTemplatesList;
//    SStandardItemModel* docTemplatesList;
    LimeReport::ReportDesignWindowInterface *reportDesigner;
    bool saveTemplateToDB();
    void removeReportDataSources();
    void translate();
#ifdef QT_DEBUG
    void randomFill() override;
#endif
protected slots:
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
#endif
public slots:
    void selectTemplate(int index);
private slots:
    void reportSaved();
    void reportOnSave(bool&);
};

#endif // SPAGESETTINGSDOCTEMPATEEDITOR_H
