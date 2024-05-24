#ifndef TABTECHREPORTS_H
#define TABTECHREPORTS_H

#include "tabcommon.h"
#include "widgets/stableviewbase.h"
#include "models/stechreportmodel.h"
#include "models/stabletechreportsmodel.h"

namespace Ui {
class tabTechReports;
}

class tabTechReports : public tabCommon
{
    Q_OBJECT

public:
    static tabTechReports* getInstance(MainWindow *parent = nullptr);
    explicit tabTechReports(MainWindow *parent = nullptr);
    ~tabTechReports();
    QString tabTitle() override;
private:
    static tabTechReports* p_instance;
    Ui::tabTechReports *ui;
    STableTechReportsModel *m_tableModel;
    QTimer *tableUpdateDelay;
    QStringList query_group;
    QList<STechReportModel*> reportsListFromSelection();
    void constructQueryClause();
private slots:
    void buttonRefreshClicked();
    void lineEditSearchTextChanged(QString);
public slots:
    void refreshTable(bool preserveScrollPos = STableViewBase::ScrollPosPreserve, bool preserveSelection = STableViewBase::SelectionReset);
    void buttonPrintClicked();
};

#endif // TABTECHREPORTS_H
