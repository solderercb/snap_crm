#ifndef TABTECHREPORTS_H
#define TABTECHREPORTS_H

#include <tabCommon>

class QTimer;
class STechReportModel;
class STableTechReportsModel;

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
    void refreshTable(bool preserveScrollPos, bool preserveSelection) override;
    void buttonPrintClicked();
};

#endif // TABTECHREPORTS_H
