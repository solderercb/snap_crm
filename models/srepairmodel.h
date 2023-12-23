#ifndef SREPAIRMODEL_H
#define SREPAIRMODEL_H

#include "scomrecord.h"
#include <QObject>
#include <QString>
#include <QDate>
#include "sdevmdlmodel.h"
#include "sclientmodel.h"
#include "ssaletablemodel.h"
#include "srepairstatuslog.h"
#include "scartridgerepairmodel.h"
#include "scartridgematerialmodel.h"

class SRepairModel : public SComRecord
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(int clientId READ clientId)
    Q_PROPERTY(QString class READ devClass)
    Q_PROPERTY(QString vendor READ vendor)
    Q_PROPERTY(QString device READ device)
    Q_PROPERTY(QString serialNumber READ serialNumber)
    Q_PROPERTY(QString company READ company)
    Q_PROPERTY(QString office READ officeStr)
    Q_PROPERTY(QString startOffice READ startOfficeStr)
    Q_PROPERTY(QString manager READ managerFioShort)
    Q_PROPERTY(QString currentManager READ currentManagerFioShort)
    Q_PROPERTY(QString engineer READ engineerFioShort)
    Q_PROPERTY(QString diagnosticResult READ diagnosticResult)
    Q_PROPERTY(QString inDateTime READ created)
    Q_PROPERTY(QString inDate READ inDate)
    Q_PROPERTY(QString outDate READ outDate)
    Q_PROPERTY(QString outDateTime READ outDateTime)
    Q_PROPERTY(int state READ state)
    Q_PROPERTY(bool isExpressRepair READ expressRepair)
    Q_PROPERTY(bool isQuickRepair READ quickRepair)
    Q_PROPERTY(bool isWarranty READ isWarranty)
    Q_PROPERTY(bool isRepeat READ isRepeat)
    Q_PROPERTY(int paymentSystem READ paymentSystem)
    Q_PROPERTY(bool isCardPayment READ isCardPayment)
    Q_PROPERTY(bool canFormat READ canFormat)
    Q_PROPERTY(bool printCheck READ printCheck)
    Q_PROPERTY(QString box READ box)
    Q_PROPERTY(QString warrantyLabel READ warrantyLabel)
    Q_PROPERTY(QString extNotes READ extNotes)
    Q_PROPERTY(bool isPrepaid READ isPrepaid)
    Q_PROPERTY(QString prepaidType READ prepaidTypeStr)
    Q_PROPERTY(QString prepaidSumm READ prepaidSummStr)
    Q_PROPERTY(QString realPrepaidSumm READ realPrepaidSummStr)
    Q_PROPERTY(int prepaidOrder READ prepaidOrder)
    Q_PROPERTY(bool isPreAgreed READ isPreAgreed)
    Q_PROPERTY(bool isDebt READ isDebt)
    Q_PROPERTY(QString preAgreedAmount READ preAgreedAmountStr)
    Q_PROPERTY(QString repairCost READ repairCostStr)
    Q_PROPERTY(QString realRepairCost READ realRepairCostStr)
    Q_PROPERTY(QString realRepairCostWords READ realRepairCostInWords)
    Q_PROPERTY(QString partsCost READ partsCostStr)
    Q_PROPERTY(QString realWorksCost READ realWorksCostStr)
    Q_PROPERTY(QString realPartsCost READ realPartsCostStr)
    Q_PROPERTY(QString fault READ fault)
    Q_PROPERTY(QString complect READ complect)
    Q_PROPERTY(QString look READ look)
    Q_PROPERTY(bool thirsPartySc READ thirsPartySc)
    Q_PROPERTY(QString warranty READ warranty)
    Q_PROPERTY(QString barcode READ barcode)
    Q_PROPERTY(QString rejectReason READ rejectReason)
    Q_PROPERTY(QString color READ color)
    Q_PROPERTY(int early READ early)
    Q_PROPERTY(QString extEarly READ extEarly)
    Q_PROPERTY(QString issuedMsg READ issuedMsg)
    Q_PROPERTY(bool smsInform READ smsInform)
    Q_PROPERTY(int invoice READ invoice)
//    Q_PROPERTY(int isCartridge READ cartridge)
    Q_PROPERTY(bool termsControl READ termsControl)
public:
    explicit SRepairModel(QObject *parent = nullptr);
    explicit SRepairModel(const int, QObject *parent = nullptr);
    ~SRepairModel();
    int id();
    void setId(const int);
    void load(const int);
    void reload();
    bool isHidden();
    void setHidden(const bool);
    QString title();
    void setTitle(const QString);
    int clientId();
    void setClientId(const int);
    SClientModel *clientModel();
    int classId();
    void setClassId(const int);
    int vendorId();
    void setVendorId(const int);
    int deviceId();
    void setDeviceId(const int);
    QString serialNumber();
    void setSerialNumber(const QString);
    int companyIndex();
    void setCompanyIndex(const int);
    int office();
    int officeIndex();
    void setOffice(const int);
    void setOfficeIndex(const int);
    int startOfficeIndex();
    void setStartOffice(const int);
    void setStartOfficeIndex(const int);
    int manager();
    int managerIndex();
    void setManager(const int);
    void setManagerIndex(const int);
    int currentManager();
    int currentManagerIndex();
    void setCurrentManager(const int);
    void setCurrentManagerIndex(const int);
    int engineer();
    int engineerIndex();
    void setEngineer(const int);
    void setEngineerIndex(const int);
    QString diagnosticResult();
    void setDiagnosticResult(const QVariant);
    void setDiagnosticResult(const QString);
    QString inDate();
    void setInDate(const QDateTime);
    QString outDate();
    QString outDateTime();
    void setOutDate(const QDateTime);
    int state();
    void setState(const int);
    int newState();
    void setNewState(const int);
    int userLock();
    void setUserLock(const QVariant);
    QDateTime lockDatetime();
    void setLockDatetime(const QVariant);
    bool expressRepair();
    void setExpressRepair(const bool);
    bool quickRepair();
    void setQuickRepair(const bool);
    bool isWarranty();
    void setIsWarranty(const bool);
    bool isRepeat();
    void setIsRepeat(const bool);
    int paymentSystem();
    int paymentSystemIndex();
    void setPaymentSystem(const int);
    void setPaymentSystemIndex(const int);
    bool isCardPayment();
    void setIsCardPayment(const bool);
    bool canFormat();
    void setCanFormat(const bool);
    bool printCheck();
    void setPrintCheck(const bool);
    int boxIndex();
    void setBoxIndex(const int);
    QString warrantyLabel();
    void setWarrantyLabel(const QString);
    QString extNotes();
    void setExtNotes(const QString);
    bool isPrepaid();
    void setIsPrepaid(const bool state = true);
    int prepaidType();
    void setPrepaidType(const int);
    double prepaidSumm();
    void setPrepaidSumm(const double);
    double paymentsAmount(const SCashRegisterModel::PaymentType type = (SCashRegisterModel::PaymentType)0);
    int prepaidOrder();
    void setPrepaidOrder(const int);
    void addPrepay(double amount, QString reason = QString());
    bool isPreAgreed();
    void setIsPreAgreed(const bool);
    bool isDebt();
    void setIsDebt(const bool);
    double preAgreedAmount();
    void setPreAgreedAmount(const double);
    double repairCost();
    void setRepairCost(const double);
    double realRepairCost();
    void setRealRepairCost(const double);
    double partsCost();
    void setPartsCost(const double);
    double realWorksCost();
    double realPartsCost();
    QString fault();
    void setFault(const QString);
    QString complect();
    void setComplect(const QString);
    QString look();
    void setLook(const QString);
    bool thirsPartySc();
    void setThirsPartySc(const bool);
    QDateTime lastSave();
    void setLastSave(const QDateTime);
    void updateLastSave();
    QDateTime lastStatusChanged();
    void setLastStatusChanged(const QDateTime);
    void updateLastStatusChanged();
    int warrantyDays();
    void setWarrantyDays(const int);
    QString barcode();
    void setBarcode(const QString);
    QString rejectReason();
    void setRejectReason(const QString);
    int informedStatusIndex();
    void setInformedStatusIndex(const int);
    QString imageIds();
    void setImageIds(const QString);
    QString color();
    void setColor(const QColor);
    QString orderMoving();
    void setOrderMoving(const QString);
    int early();
    void setEarly(const int);
    QString extEarly();
    void setExtEarly(const QString);
    QString issuedMsg();
    void setIssuedMsg(const QString);
    bool smsInform();
    void setSmsInform(const bool);
    int invoice();
    void setInvoice(const int);
    SCartridgeRepairModel* cartridge();
    void setCartridge(SCartridgeRepairModel *model);
    bool termsControl();
    void setTermsControl(const bool);
    bool commit();
    bool lock(bool state = 1);
    bool isLock();
    void initDemo() override;
    void initCartridgeRepairModel(const int id);
    bool isNew();
    void setBOQModel(SSaleTableModel *model);
    SSaleTableModel *BOQModel() const;

private:
    SClientModel *m_clientModel;
    SRepairStatusLog *m_repairStatusLog;
    SCartridgeRepairModel *m_cartridgeRepair = nullptr;
    bool m_isHidden = 0;
    QString m_title;
    int m_clientId = 0;
    int m_type = 0;
    int m_maker = 0;
    int m_model = 0;
    QString m_serialNumber;
    int m_company = 0;
    int m_office = 0;
    int m_startOffice = 0;
    int m_manager = 0;
    int m_currentManager = 0;
    int m_master = 0;
    QString m_diagnosticResult;
    QDateTime m_inDate;
    QDateTime m_outDate;
    int m_state = 0;
    int m_newState = 0;
    int m_userLock = 0;
    QDateTime m_lockDatetime;
    bool m_expressRepair = 0;
    bool m_quickRepair = 0;
    bool m_isWarranty = 0;
    bool m_isRepeat = 0;
    int m_paymentSystem = 0;
    bool m_isCardPayment = 0;
    bool m_canFormat = 0;
    bool m_printCheck = 0;
    int m_box = 0;
    QString m_warrantyLabel;
    QString m_extNotes;
    bool m_isPrepaid = 0;
    int m_prepaidType = 0;
    double m_prepaidSumm = 0;
    int m_prepaidOrder = 0;
    bool m_isPreAgreed = 0;
    bool m_isDebt = 0;
    double m_preAgreedAmount = 0;
    double m_repairCost = 0;
    double m_realRepairCost = 0;
    double m_partsCost = 0;
    QString m_fault;
    QString m_complect;
    QString m_look;
    bool m_thirsPartySc = 0;
    QDateTime m_lastSave;
    QDateTime m_lastStatusChanged;
    int m_warrantyDays = 0;
    QString m_barcode;
    QString m_rejectReason;
    int m_informedStatus = 0;
    QString m_imageIds;
    QString m_color;
    QString m_orderMoving;
    int m_early = 0;
    QString m_extEarly;
    QString m_issuedMsg;
    bool m_smsInform = 0;
    int m_invoice = 0;
    int m_cartridge = 0;
    int m_vendorId = 0;
    bool m_termsControl = 0;
    SSaleTableModel *m_BOQModel = nullptr;  // модель данных таблицы работ и деталей
    QString devClass();
    QString vendor();
    QString device();
    QString company();
    QString officeStr();
    QString startOfficeStr();
    QString managerFioShort();
    QString currentManagerFioShort();
    QString engineerFioShort();
    QString box();
    QString prepaidTypeStr();
    QString prepaidSummStr();
    QString realPrepaidSummStr();
    QString preAgreedAmountStr();
    QString repairCostStr();
    QString realRepairCostStr();
    QString realRepairCostInWords();
    QString partsCostStr();
    QString realWorksCostStr();
    QString realPartsCostStr();
    QString warranty();
    bool commitCartridge();
};

#endif // SREPAIRMODEL_H
