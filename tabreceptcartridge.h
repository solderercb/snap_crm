#ifndef TABRECEPTCARTRIDGE_H
#define TABRECEPTCARTRIDGE_H

#include <QWidget>
#include <QComboBox>
#include "mainwindow.h"
#include "tabcommon.h"
#include "models/ssqlquerymodel.h"
#include "models/srepairmodel.h"
#include "widgets/scartridgeform.h"
#include "widgets/scartridgecard.h"

namespace Ui {
class tabReceptCartridge;
}

class tabReceptCartridge : public tabCommon
{
    Q_OBJECT
signals:
    void generatePrintout(QMap<QString, QVariant>);
public:
    explicit tabReceptCartridge(MainWindow *parent = nullptr);
    ~tabReceptCartridge();
    static tabReceptCartridge* getInstance(MainWindow *parent = nullptr);
    bool tabCloseRequest() override;
    QString tabTitle() override;
    void setFocusSerialNumber();
private:
    Ui::tabReceptCartridge *ui;
    static tabReceptCartridge* p_instance;
    int m_deviceClassId = 0;
    QStandardItemModel *m_tableModel;
    SSqlQueryModel* m_vendorsModel;
    SSqlQueryModel* m_cartridgesModel;
    QTimer *m_findBySerialDelay;
    bool m_closePending = 0;
    SClientModel *m_client = nullptr;
    SCartridgeCard *m_cartridgeCardForm;
    bool m_printReport = 0;
    void setDefaultStyleSheets();
    void initWidgets();
    void clearWidgets();
    bool checkInput();
    bool checkInputBeforeAdd();
    void appendToReceptList(SCartridgeForm *form);
    void print();
    const QList<SCartridgeForm*> existentForms();
#ifdef QT_DEBUG
    void randomFill() override;
#endif
public slots:
    void changeVendor(int index);
    void serialTextEdited(QString text);
    void findAndAddBySerial();
private slots:
    bool createRepairs();
    void createRepairsAndClose();
    void comboBoxModelButtonClickHandler(int);
    void appendToReceptList();
    void updateTotalPreagreedAmount();
    void createCartridgeCardForm(const int id);
    void closeCartridgeCardForm();
    void updateDevicesModel(const int id = 0);
    void guiFontChanged() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
    void dbgRandomFillClicked();
#endif
};

#endif // TABRECEPTCARTRIDGE_H
