#ifndef SCARTRIDGEFORM_H
#define SCARTRIDGEFORM_H

#include <SFrame>

class SRepairModel;
class SCartridgeCardModel;
class SCartridgeMaterialModel;
class WorkshopSaleModel;
class SSortFilterProxyModel;

namespace Ui {
class SCartridgeForm;
}

class SCartridgeForm : public SFrame
{
    Q_OBJECT
    friend class tabReceptCartridge;
signals:
    void updateTotalPreagreedAmount();
    void updateParentTab();
    void createTabClient(int id);
    void createTabRepair(int id);
    void createCartridgeCardForm(int id);
public:
    explicit SCartridgeForm(QWidget *parent = nullptr);
    ~SCartridgeForm();
    bool eventFilter(QObject *, QEvent *) override;
    SRepairModel *model();
    void initModels();
    void configureWidgets();
    void init();
    void initCheckBoxWidgets(const QIcon &icon, QPushButton *button, bool isFlat = 0, bool installEventFilter = 0);
    void createRepair();
    int repairId();
    const QString &title() const;
    void setTitle(const QString &title);
    bool isRepeat() const;
    void setIsRepeat(bool isRepeat);
    int deviceClassId() const;
    void setDeviceClassId(int id);
    int deviceVendorId() const;
    void setDeviceVendorId(int id);
    int deviceModelId() const;
    void setDeviceModelId(int id);
    const QString &serialNumber() const;
    void setSerialNumber(const QString &serial);
    int cardId() const;
    void setCardId(int id);
    int clientId() const;
    void setClientId(int clientId);
    int companyIndex() const;
    void setCompanyIndex(int index);
    int officeIndex() const;
    void setOfficeIndex(int index);
    int engineerIndex() const;
    void setEngineerIndex(int index);
    bool isHighPriority() const;
    void setIsHighPriority(bool isHighPriority);
    int paymentSystemIndex() const;
    void setPaymentSystemIndex(int paymentSystemIndex);
    double preargeedAmount();
    int isReady();
    void load();
    void loadCardData();
    void reloadData();
    void updateWidgets();
    void randomFill();
    void setRepair(int id);
private:
    enum OpType {CreateRepair = 0, SaveEngineer, SaveState, AddWorkAndPart, RemoveWorkAndPart, SaveComment, SavePlace, SaveInformedState};
    Ui::SCartridgeForm *ui;
    SRepairModel *m_repair = nullptr;
    SCartridgeCardModel *m_cartridgeCard = nullptr;
    WorkshopSaleModel *m_BOQModel = nullptr;
    SSortFilterProxyModel *statusesProxyModel = nullptr;
    QMap<int, double> preagreedAmounts;
    int m_repairId = 0;
    QString m_title;
    bool m_isRepeat = 0;
    int m_deviceClassId = 0;
    int m_deviceVendorId = 0;
    int m_deviceModelId = 0;
    int m_cardId = 0;
    QString m_serialNumber;
    int m_clientId = 0;
    int m_companyIndex = 0;
    int m_officeIndex = 0;
    int m_engineerIndex = 0;
    bool m_isHighPriority = 0;
    int m_paymentSystemIndex = 0;
    double m_amount = 0;
    bool worksCheckboxesEn = 1;
    bool engineerComboBoxEn = 1;
    bool placeComboBoxEn = 1;
    int m_prevRepairsCount = 0;
    QString m_lineEditStyleSheet;
    QString m_spinBoxStyleSheet;
    bool m_initDone = 0;
    OpType m_opType = CreateRepair;
    int m_workType;
    int m_workMaterialItemId = 0;
    void updateHeader();
    void updateWorksCheckBoxes();
    void hideWidgetsOnReceiptForm();
    void updateLabelRepeatWarranty(const QString text);
    void updateStatesModel();
    void beginSaveState(const int stateId);
    void updatePreagreedAmount(SCartridgeMaterialModel *material, const bool state);
    void setDefaultStyleSheets();
    void updateWidgetsFlagsToState();
    bool checkStateAcl(const int);
    void setPricesToZero();
    void setInformedStatus(int);
    void initEngineer();
    void updateComboBoxEngineer();
    bool addWorkAndPart();
    bool removeWorkAndPart();
    bool workAndPartHandler(const int workType, const int state);
    int prevRepairsCount();
    void saveState();
    void beginCreateRepair();
    bool checkWorkMaterialItemCount();
    int checkInput() override;
    void beginCommit() override;
    int commitStages() override;
    bool skip(const int) override;
    void beginCommit(const int) override;
    void commit(const int) override;
    void throwHandler(int) override;
    void endCommit(const int) override;
    void endCommit() override;
    int checkBeforeStateChange();
    int checkBeforeInformedStatusChange();
    bool checkBeforePlaceChange();
    int checkBeforeRepairCreate();
    void updateComboBoxPlace();
    void updateComboBoxInformedStatus();
    void beginSaveInformedState();
    void beginAddWorkAndPart();
    void beginRemoveWorkAndPart();
    void endBOQCommit();
    void undoBOQModelChanges();
public slots:
    void removeWidget();
private slots:
    void updateTotalSumms(const double, const double, const double);
    void saveTotalSumms();
    void updateLists();
    void setRefill(bool);
    void setChipReplace(bool);
    void setDrumReplace(bool);
    void setBladeReplace(bool);
    void savePlace(int index);
    void comboBoxPlaceButtonClickHandler(int id);
    void comboBoxEngineerChanged(int);
    void stateIndexChanged(int);
    void buttonClientCardClicked();
    void buttonClassicTabClicked();
    void buttonCartridgeCardClicked();
    void updateComment();
    void initWorksMenu();
    void updateWorksMenu();
    void setWorkCheckBoxChecked(const int workType);
    void updateWorksActionsCheckedState();
    void guiFontChanged();
};

#endif // SCARTRIDGEFORM_H
