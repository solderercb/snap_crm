#ifndef TABCLIENT_H
#define TABCLIENT_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlField>
#include <QScrollBar>
#include "tabcommon.h"
#include "tabclientsummary.h"

namespace Ui {
class tabClient;
}

class tabClient : public tabCommon
{
    Q_OBJECT

signals:
    void generatePrintout(QMap<QString,QVariant>);

public:
    explicit tabClient(int rep_id, MainWindow *parent = nullptr);
    static tabClient* getInstance(int id, MainWindow *parent = nullptr);
    ~tabClient();
    QString tabTitle() override;
private:
    Ui::tabClient *ui;
    static QMap<int, tabClient*> p_instance;
    int m_clientId;
private slots:
    void switchPage(const int);
    void buttonSaveClicked();
    void buttonPrintClicked();
#ifdef QT_DEBUG
    void randomFill() override;
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
#endif
};

#endif // TABCLIENT_H
