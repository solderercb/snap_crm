#ifndef SCARTRIDGEFORM_H
#define SCARTRIDGEFORM_H

#include <QWidget>
#include <QMenu>
#include "widgets/swidget.h"
#include "models/srepairmodel.h"
#include "models/scartridgecardmodel.h"
#include "models/ssortfilterproxymodel.h"
#include "models/sstoreitemmodel.h"

namespace Ui {
class SCartridgeForm;
}

class SCartridgeForm : public QFrame
{
    Q_OBJECT
signals:
    void updateTotalPreagreedAmount();
    void updateParentTab();
    void createTabClient(int id);
    void createTabRepair(int id);
    void createCartridgeCardForm(int id);
public:
    explicit SCartridgeForm(QWidget *parent = nullptr);
    explicit SCartridgeForm(const int repairId, QWidget *parent = nullptr);
    ~SCartridgeForm();
    bool eventFilter(QObject *, QEvent *);
    SRepairModel *model();
    void initModels();
    void initWidgets();
    void initCheckBoxWidgets(const QIcon &icon, QPushButton *button, bool isFlat = 0, bool installEventFilter = 0);
    bool createRepair();
    bool updateRepair();
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
    int checkInput();
    int isReady();
    void updateModels();
    void randomFill();
private:
    Ui::SCartridgeForm *ui;
    SQueryLog *i_queryLog;
    SRepairModel *m_repair = nullptr;
    SCartridgeCardModel *m_cartridgeCard = nullptr;
    SSaleTableModel *m_BOQModel = nullptr;
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
    bool m_groupUpdate = 0;
    int m_prevRepairsCount = 0;
    QString m_lineEditStyleSheet = commonLineEditStyleSheet;
    QString m_spinBoxStyleSheet = commonSpinBoxStyleSheet;
    void updateHeader();
    void updateWidgets();
    void hideWidgetsOnReceiptForm();
    void updateLabelRepeatWarranty(const QString text);
    void updateStatesModel(const int statusId);
    void updateStateWidget(const int statusId);
    bool checkData(const int);
    void doStateActions(const int);
    void updatePreagreedAmount(SCartridgeMaterialModel *material, const bool state);
    void setDefaultStyleSheets();
    void setWidgetsParams(const int);
    bool checkStateAcl(const int);
    void setPricesToZero();
    void setInformedStatus(int);
    void initEngineer();
    void updateComboBoxEngineer(const int id);
    bool commit(const QString &notificationCaption = tr("Успешно"), const QString &notificationText = tr("Данные сохранены"));
    bool addWorkAndPart(const int workType);
    bool removeWorkAndPart(const int workType);
    bool workAndPartHandler(const int workType, const int state);
    int prevRepairsCount();
    void saveState(int);
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
    void removeWidget();
    void buttonClientCardClicked();
    void buttonClassicTabClicked();
    void buttonCartridgeCardClicked();
    void updateComment();
    void initWorksMenu();
    void setWorkCheckBoxChecked(const int workType);
    void updateWorksActionsCheckedState();
    void guiFontChanged();
};

#endif // SCARTRIDGEFORM_H
