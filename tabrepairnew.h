#ifndef TABREPAIRNEW_H
#define TABREPAIRNEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QToolButton>
#include <QStyle>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QDateEdit>
#include <QRandomGenerator>
#ifdef QT_DEBUG
#include <QTimer>
#endif
#include "mainwindow.h"
#include "tabcommon.h"
#include "amountToWords.h"

namespace Ui {
    class tabRepairNew;
}

class groupBoxEventFilter : public QObject
{
    Q_OBJECT
signals:

public:
    groupBoxEventFilter(QObject*);
private:
protected:
    bool eventFilter(QObject*, QEvent*) override;
};

class tabRepairNew : public tabCommon
{
    Q_OBJECT

signals:
    void createTabSelectPrevRepair(int);
    void createTabSelectExistingClient(int);
    void createTabClient(int);
    void generatePrintout(QMap<QString, QVariant>);

public:
    explicit tabRepairNew(MainWindow *parent = nullptr);
    static tabRepairNew* getInstance(MainWindow *parent = nullptr);
    ~tabRepairNew();
    virtual bool tabCloseRequest();
#ifdef QT_DEBUG
    void randomFill();
#endif

private:
    Ui::tabRepairNew *ui;
    static tabRepairNew* p_instance;
    MainWindow *parent;
    void setDefaultStyleSheets();
    void getDevices();
    QString genUserWebPass() const;
//    QStandardItemModel* comboboxDevicesModel;
    QSqlQueryModel* comboboxDevicesModel;
    QSqlQueryModel* comboboxDeviceMakersModel;
    QSqlQueryModel* comboboxDeviceModelsModel;
    QSqlQueryModel* comboboxProblemModel;
    QSqlQueryModel* comboBoxIncomingSetModel;
    QSqlQueryModel* comboBoxExteriorModel;
    QSqlQueryModel* clientModel;
    QSqlQueryModel* queryDevice;
    QSqlQueryModel* clientPhonesModel;
    QStandardItemModel* prepayReasonsModel;
    QList<QStandardItem*> *prepayReason;
    QSqlQueryModel* clientsMatchTable;
    QSqlQueryModel* devicesMatchTable;
    QVector<QWidget*> additionalFieldsWidgets;
    int client = 0;
    void phoneTypeChanged(int, int);
    bool checkInput();
    QMessageBox msgBox;
#ifdef QT_DEBUG
    QTimer *test_scheduler, *test_scheduler2, *main_window_test_scheduler, *main_window_test_scheduler2;
    uint test_scheduler_counter = 0;
#endif

    groupBoxEventFilter *groupBoxEventFilterObj;

private slots:
    void changeClientType();
    void showLineEditPrevRepair();
    void enablePrepayWidgets(bool);
    void changeDeviceType();
    void changeDeviceMaker();
    void clearClientCreds(bool hideCoincidence = true);
    void lineEditPrevRepairButtonsHandler(int);
    void fillClientCreds(int);
    void fillDeviceCreds(int);
    void buttonSelectExistingClientHandler();
    void buttonCreateTabClientHandler();
    void findMatchingClient(QString);
    void findMatchingDevice(QString);
    void phone1TypeChanged(int);
    void phone2TypeChanged(int);
    void deviceMatchTableDoubleClicked(QModelIndex);
    void clientMatchTableDoubleClicked(QModelIndex);
    void lineEditSNClearHandler(int);
    bool createRepair();
    void createRepairClose();
#ifdef QT_DEBUG
    void test_scheduler_handler();
    void test_scheduler2_handler();
#endif
};

#endif // TABREPAIRNEW_H
