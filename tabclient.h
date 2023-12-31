#ifndef TABCLIENT_H
#define TABCLIENT_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlField>
#include <QScrollBar>
#include "tabcommon.h"
#include "widgets/spageclientsummary.h"

namespace Ui {
class tabClient;
}

class tabClient : public tabCommon
{
    Q_OBJECT

signals:
    void generatePrintout(QMap<QString,QVariant>);
    void pageInitFinished(QWidget* widget, const int page);

public:
    enum Page{Summary, Edit, Repairs, Purchases, Sales, Calls, Finances, Balance, History};
    explicit tabClient(int rep_id, MainWindow *parent = nullptr);
    static tabClient* getInstance(int id, MainWindow *parent = nullptr);
    ~tabClient();
    QString tabTitle() override;
private:
    Ui::tabClient *ui;
    static QMap<int, tabClient*> p_instance;
    int m_clientId;
    QMap<int, QWidget*> m_pages;
    bool m_buttonPrintVisible = 0;
    bool m_buttonSaveVisible = 1;
    bool m_buttonRefreshVisible = 0;
    bool m_lineEditFilterVisible = 0;
    bool m_editStrategy = SClientModel::EditStrategy::OnManualSubmit;
private slots:
    void buttonSaveClicked();
    void buttonPrintClicked();
    void updateBotToolbar(const int page);
    void initPage(const int page);
#ifdef QT_DEBUG
    void randomFill() override;
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
#endif
};

#endif // TABCLIENT_H
