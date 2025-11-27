#ifndef TABRECEPTCARTRIDGE_H
#define TABRECEPTCARTRIDGE_H

#include <tabCommon>

class QWidget;
class MainWindow;
class SClientModel;
class SSqlQueryModel;
class SCartridgeForm;

namespace Ui {
class tabReceptCartridge;
}

class tabReceptCartridge : public tabCommon
{
    Q_OBJECT
signals:
    void generatePrintout(QMap<QString, QVariant>);
public:
    enum EndCommitOp {CloseTab, PrepareRepeat};
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
    bool m_printReport = 0;
    int m_endCommitOp = CloseTab;
    void setDefaultStyleSheets();
    void initWidgets();
    void clearWidgets();
    int checkInput() override;
    bool checkInputBeforeAdd();
    void initReceiptCartridgeForm(SCartridgeForm *&form, const QSqlRecord &record = QSqlRecord());
    void appendToList(SCartridgeForm *form);
    void print();
    const QList<SCartridgeForm*> existentForms();
    int commitStages() override;
    void beginCommit() override;
    bool skip(const int) override;
    void commitClient();
    void commitRepairs();
    void commit(const int stage) override;
    void throwHandler(int) override;
    void endCommit(const int) override;
    void endCommit() override;
    void prepareForRepeatedOp();
#ifdef QT_DEBUG
    void randomFill() override;
#endif
public slots:
    void changeVendor(int index);
    void serialTextEdited(QString text);
    void findAndAddBySerial();
private slots:
    void createRepairs();
    void createRepairsAndClose();
    void comboBoxModelButtonClickHandler(int);
    void appendToList();
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
