#ifndef TABREPAIR_H
#define TABREPAIR_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QTableWidget>
#include <QLabel>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlField>
#include <QScrollBar>
#include <QTimeZone>
#include <QDateTime>
#include <QLocale>
#include <QClipboard>
#include "tabcommon.h"
#include "widgets/sdialogissuerepair.h"
#include "widgets/sgroupboxeventfilter.h"
#include "widgets/stableviewbase.h"
#include "models/srepairmodel.h"
#include "models/srepairstatuslog.h"
#include "models/sfieldsmodel.h"
#include "models/ssortfilterproxymodel.h"
#include "models/ssaletablemodel.h"
#include "widgets/saletableitemdelegates.h"

namespace Ui {
class tabRepair;
}

class tabRepair : public tabCommon
{
    Q_OBJECT

    friend class SDialogIssueRepair;

signals:
    void worksTreeDoubleClicked(int);
    void createTabPrevRepair(int);
    void generatePrintout(QMap<QString,QVariant>);
    void createTabClient(int);
    void createTabSparePart(int);
    void createTabSparePartReserve(int);
    void createTabSelectItem(int, QWidget*);

public:
    explicit tabRepair(int rep_id, MainWindow *parent = nullptr);
    static tabRepair* getInstance(int rep_id, MainWindow *parent = nullptr);
    ~tabRepair();
    QString tabTitle() override;
    bool tabCloseRequest() override;
private:
    Ui::tabRepair *ui;
    static QMap<int, tabRepair*> p_instance;
    int repair_id;
    SRepairModel *repairModel;
    SClientModel *clientModel;
    SFieldsModel *additionalFieldsModel;
    SSortFilterProxyModel *statusesProxyModel = nullptr;
    SSaleTableModel *m_BOQModel;    // bill of quantities
    bool m_statusUpdateInProgress = 0;
    double works_sum, parts_sum, total_sum;
    QString box_name;
    bool modelRO = 0;   // признак блокировки карты ремонта
    SDialogIssueRepair *m_dialogIssue;
    bool m_BOQModelRO = 1;
    bool m_diagRO = 1;
    bool m_summRO = 1;
    bool m_getOutButtonVisible = 0;
    bool m_comboBoxStateEnabled = 1;
    bool m_comboBoxNotifyStatusEnabled = 1;
    bool m_outDateVisible = 0;
    bool m_buttonDebtReceivedVisible = 0;
    SGroupBoxEventFilter *groupBoxEventFilter;
    int m_clientId = 0;
    QList<QWidget*> additionalFieldsWidgets;
    bool m_autosaveDiag = 0;
    QTimer *m_autosaveDiagTimer;
    bool m_diagChanged = 0;
    bool m_spinBoxAmountChanged = 0;
    bool m_groupUpdate = 0;
    bool m_buttonSaveStateEnabled = 0;
    QTimer *m_repairLockUpdateTimer = nullptr;
    int getFieldIdByName(const QString &, QSqlQueryModel *);
    QString getDisplayRoleById(int, QAbstractItemModel*, int column = 0);
    void eventResize(QResizeEvent *);
    void fillExtraInfo();
    void setLock(bool state = 1);
    void createAdditionalFieldsWidgets();
    void delAdditionalFieldsWidgets();
    void setInfoWidgetVisible(QWidget *w, bool state = 1);
    bool setWidgetsParams(const int);
    bool checkStateAcl(const int);
    bool checkData(const int);
    void updateStatesModel(const int statusId);
    void doStateActions(const int);
    void setPricesToZero();
    bool commit(const QString &notificationCaption = tr("Успешно"), const QString &notificationText = tr("Данные сохранены"));
    void initEngineer();
    bool checkViewPermission();
#ifdef QT_DEBUG
    void randomFill() override {};
#endif

private slots:
    void reloadRepairData();
    void updateWidgets();
    void saveState();
    void saveState(int);
    void comboBoxStateIndexChanged(int);
    void createDialogIssue();
    void openPrevRepair();
    void printStickers(int);
    void changeOffice(int);
    void changeManager(int);
    void changeEngineer(int);
    void openInvoice(int);
    void editIncomingSet(int);
    void setAgreedAmount(int);
    void buttonClientClicked();
    void setInformedStatus(int);
    void diagChanged();
    void diagEditFinished();
    void spinBoxAmountChanged(double);
    void spinBoxAmountEditingFinished();
    void saveDiagAmount();
    void autosaveTimeout();
    void diagAmountSaved();
    void savePlace(int index);
    void comboBoxPlaceButtonClickHandler(int id);
    void guiFontChanged() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override{};
    void test_scheduler2_handler() override{};
#endif
};

#endif // TABREPAIR_H
