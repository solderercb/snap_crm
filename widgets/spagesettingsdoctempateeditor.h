#ifndef SPAGESETTINGSDOCTEMPATEEDITOR_H
#define SPAGESETTINGSDOCTEMPATEEDITOR_H

#include <STabPage>
#include <SReportsCommonFunctions>

class QWidget;
class SSqlQueryModel;
class QByteArray;
class QFile;

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
    LimeReport::ReportDesignWindowInterface *reportDesigner;
    std::unique_ptr<QByteArray> m_templateBuffer = nullptr;
    std::unique_ptr<QString> m_templateHash = nullptr;
    std::unique_ptr<QFile> m_temporaryFile = nullptr;
    void removeReportDataSources();
    void translate();
    void initWorksDataSources() override;
    void initTechReportDataSources() override;
    QString queryLogFile() override;
    int checkInput() override;
    void commit(const int) override;
    void throwHandler(int) override;
    void endCommit() override;
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
};

#endif // SPAGESETTINGSDOCTEMPATEEDITOR_H
