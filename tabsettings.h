#ifndef TABSETTINGS_H
#define TABSETTINGS_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlField>
#include <QScrollBar>
#include "tabcommon.h"

namespace Ui {
class tabSettings;
}

class tabSettings : public tabCommon
{
    Q_OBJECT

signals:
    void generatePrintout(QMap<QString,QVariant>);
    void pageInitFinished(QWidget* widget, const int page);

public:
    enum Page{User, Global, Company, Employees, UserRoles, Warehouses, Finances, Devices, RepairStatuses, AdditionalFieldsAndAdSources, DocTemplates, Notifications, Backup};
    explicit tabSettings(MainWindow *parent = nullptr);
    static tabSettings* getInstance(MainWindow *parent = nullptr);
    ~tabSettings();
    QString tabTitle() override;
    bool tabCloseRequest() override;
private:
    Ui::tabSettings *ui;
    static tabSettings* p_instance;
    QMap<int, QWidget*> m_pages;
    bool m_buttonSaveVisible = 1;
    bool m_buttonRefreshVisible = 0;
    bool m_lineEditSearchVisible = 0;
    bool m_editStrategy = 0;
private slots:
    void buttonSaveClicked();
    void updateBotToolbar(const int page);
    void initPage(const int page);
#ifdef QT_DEBUG
    void randomFill() override;
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
#endif
};

#endif // TABSETTINGS_H
