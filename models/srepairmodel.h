#ifndef SREPAIRMODEL_H
#define SREPAIRMODEL_H

#include <SSingleRowJModel>
#include <SCashRegisterModel>

class SCartridgeRepairModel;
class WorkshopSaleModel;
class SRepairStatusLog;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(Hidden, isHidden, bool, 0)                                  \
    TABLE_FIELD(Title, title, QString, 1)                                   \
    TABLE_FIELD(client, clientId, int, 1)                                   \
    TABLE_FIELD(type, classId, int, 0)                                      \
    TABLE_FIELD(maker, vendorId, int, 0)                                    \
    TABLE_FIELD(model, deviceId, int, 0)                                    \
    TABLE_FIELD(serial_number, serialNumber, QString, 1)                    \
    TABLE_FIELD(company, companyId, int, 0)                                 \
    TABLE_FIELD(office, officeId, int, 0)                                   \
    TABLE_FIELD(start_office, startOfficeId, int, 0)                        \
    TABLE_FIELD(manager, managerId, int, 0)                                 \
    TABLE_FIELD(current_manager, currentManagerId, int, 0)                  \
    TABLE_FIELD(master, engineerId, int, 0)                                 \
    TABLE_FIELD(diagnostic_result, diagnosticResult, QString, 1)            \
    TABLE_FIELD(in_date, created, QDateTime, 0)                             \
    TABLE_FIELD(out_date, issued, QDateTime, 0)                             \
    TABLE_FIELD(state, stateId, int, 0)                                     \
    TABLE_FIELD(new_state, newStateId, int, 0)                              \
    TABLE_FIELD(user_lock, userLock, int, 0)                                \
    TABLE_FIELD(lock_datetime, lockDateTime, QDateTime, 0)                  \
    TABLE_FIELD(express_repair, isExpressRepair, bool, 1)                   \
    TABLE_FIELD(quick_repair, isQuickRepair, bool, 1)                       \
    TABLE_FIELD(is_warranty, isWarranty, bool, 1)                           \
    TABLE_FIELD(is_repeat, isRepeat, bool, 1)                               \
    TABLE_FIELD(payment_system, paymentSystem, int, 1)                      \
    TABLE_FIELD(is_card_payment, isCardPayment, bool, 1)                    \
    TABLE_FIELD(can_format, canFormat, bool, 1)                             \
    TABLE_FIELD(print_check, printCheck, bool, 1)                           \
    TABLE_FIELD(box, boxId, int, 0)                                         \
    TABLE_FIELD(warranty_label, warrantyLabel, QString, 1)                  \
    TABLE_FIELD(ext_notes, extNotes, QString, 1)                            \
    TABLE_FIELD(is_prepaid, isPrepaid, bool, 1)                             \
    TABLE_FIELD(prepaid_type, prepaidTypeId, int, 0)                        \
    TABLE_FIELD(prepaid_summ, prepaidSumm, double, 0)                       \
    TABLE_FIELD(prepaid_order, prepaidOrderId, int, 0)                      \
    TABLE_FIELD(is_pre_agreed, isPreAgreed, bool, 1)                        \
    TABLE_FIELD(is_debt, isDebt, bool, 1)                                   \
    TABLE_FIELD(pre_agreed_amount, preAgreedAmount, double, 0)              \
    TABLE_FIELD(repair_cost, repairCost, double, 0)                         \
    TABLE_FIELD(real_repair_cost, realRepairCost, double, 0)                \
    TABLE_FIELD(parts_cost, partsCost, double, 0)                           \
    TABLE_FIELD(fault, fault, QString, 1)                                   \
    TABLE_FIELD(complect, complect, QString, 1)                             \
    TABLE_FIELD(look, look, QString, 1)                                     \
    TABLE_FIELD(thirs_party_sc, thirsPartySc, bool, 1)                      \
    TABLE_FIELD(last_save, lastSave, QDateTime, 0)                          \
    TABLE_FIELD(last_status_changed, lastStatusChanged, QDateTime, 0)       \
    TABLE_FIELD(warranty_days, warrantyDays, int, 1)                        \
    TABLE_FIELD(barcode, barcode, QString, 1)                               \
    TABLE_FIELD(reject_reason, rejectReason, QString, 1)                    \
    TABLE_FIELD(informed_status, informedStatusId, int, 0)                  \
    TABLE_FIELD(image_ids, imageIds, QString, 0)                            \
    TABLE_FIELD(color, color, QString, 1)                                   \
    TABLE_FIELD(order_moving, orderMoving, QString, 0)                      \
    TABLE_FIELD(early, early, int, 1)                                       \
    TABLE_FIELD(ext_early, extEarly, QString, 1)                            \
    TABLE_FIELD(issued_msg, issuedMsg, QString, 1)                          \
    TABLE_FIELD(sms_inform, smsInform, bool, 1)                             \
    TABLE_FIELD(invoice, invoiceId, int, 1)                                 \
    TABLE_FIELD(cartridge, cartridgeId, int, 1)                             \
    TABLE_FIELD(vendor_id, warrantyVendorId, int, 0)                        \
    TABLE_FIELD(termsControl, termsControl, bool, 0)

// Список дополнительных полей для отчетов
#undef ADDITIONAL_REPORT_FIELDS
#define ADDITIONAL_REPORT_FIELDS                                            \
    ADDITIONAL_REPORT_FIELD(class, devClass)                                \
    ADDITIONAL_REPORT_FIELD(vendor, vendor)                                 \
    ADDITIONAL_REPORT_FIELD(device, device)                                 \
    ADDITIONAL_REPORT_FIELD(company, company)                               \
    ADDITIONAL_REPORT_FIELD(office, office)                                 \
    ADDITIONAL_REPORT_FIELD(startOffice, startOffice)                       \
    ADDITIONAL_REPORT_FIELD(manager, manager)                               \
    ADDITIONAL_REPORT_FIELD(currentManager, currentManager)                 \
    ADDITIONAL_REPORT_FIELD(engineer, engineer)                             \
    ADDITIONAL_REPORT_FIELD(box, box)                                       \
    ADDITIONAL_REPORT_FIELD(prepaidType, prepaidType)                       \
    ADDITIONAL_REPORT_FIELD(prepaidSumm, prepaidSummStr)                    \
    ADDITIONAL_REPORT_FIELD(realPrepaidSumm, realPrepaidSummStr)            \
    ADDITIONAL_REPORT_FIELD(preAgreedAmount, preAgreedAmountStr)            \
    ADDITIONAL_REPORT_FIELD(repairCost, repairCostStr)                      \
    ADDITIONAL_REPORT_FIELD(realRepairCost, realRepairCostStr)              \
    ADDITIONAL_REPORT_FIELD(realRepairCostWords, realRepairCostWords)       \
    ADDITIONAL_REPORT_FIELD(partsCost, partsCostStr)                        \
    ADDITIONAL_REPORT_FIELD(realWorksCost, realWorksCostStr)                \
    ADDITIONAL_REPORT_FIELD(realPartsCost, realPartsCostStr)                \
    ADDITIONAL_REPORT_FIELD(inDate, inDate)                                 \
    ADDITIONAL_REPORT_FIELD(inDateTime, inDateTime)                         \
    ADDITIONAL_REPORT_FIELD(outDate, outDate)                               \
    ADDITIONAL_REPORT_FIELD(outDateTime, outDateTime)                       \
    ADDITIONAL_REPORT_FIELD(warranty, warranty)

class SRepairModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
    friend class SListSourceDataCallbackHandler;
public:
    explicit SRepairModel(QObject *parent = nullptr);
    ~SRepairModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load() override;
    void load(const int);
    int companyIndex();
    void setCompanyIndex(const int);
    int officeIndex();
    void setOfficeIndex(const int);
    int startOfficeIndex();
    void setStartOfficeIndex(const int);
    int managerIndex();
    void setManagerIndex(const int);
    void managerChanged(const int id);
    int currentManagerIndex();
    void setCurrentManagerIndex(const int);
    void currentManagerChanged(const int id);
    int engineerIndex();
    void setEngineerIndex(const int);
    void engineerChanged(const int id);
    void stateChanged(const int id);
    int paymentSystemIndex();
    void setPaymentSystemIndex(const int index);
    int boxIndex();
    void setBoxIndex(const int index);
    void boxChanged(const int newId);
    double paymentsAmount(const SCashRegisterModel::PaymentType type = (SCashRegisterModel::PaymentType)0);
    void addPrepay(double amount, QString reason = QString());
    double realWorksCost();
    double realPartsCost();
    void updateLastSave();
    void updateLastStatusChanged();
    int informedStatusIndex();
    void setInformedStatusIndex(const int);
    SCartridgeRepairModel* cartridge();
    void setCartridge(SCartridgeRepairModel *model);
    bool commit() override;
    bool lock(bool state = 1);
    bool isLock();
    void initDemo() override;
    void initCartridgeRepairModel(const int id);
    bool isNew();
    void setBOQModel(WorkshopSaleModel *model);
    WorkshopSaleModel *BOQModel() const;
    int lockTimeout();
    void setFieldsFailed() override;
private:
    SRepairStatusLog *m_repairStatusLog;
    SCartridgeRepairModel *m_cartridgeRepair = nullptr;
    WorkshopSaleModel *m_BOQModel = nullptr;  // модель данных таблицы работ и деталей
    int indexOfCreated() override;
    void convertTimestampFieldsToLocal() override;
    void genBarcode();
    bool commitCartridge();
    void updateLogAssociatedRecId() override;
    void loadError(const int type) override;
    QString constructSelectQuery() override;
public Q_SLOTS:
    QVariant devClass();
    QVariant vendor();
    QVariant device();
    QVariant company();
    QVariant office();
    QVariant startOffice();
    QVariant manager();
    QVariant currentManager();
    QVariant engineer();
    QVariant box();
    QVariant prepaidType();
    QVariant prepaidSummStr();
    QVariant realPrepaidSummStr();
    QVariant preAgreedAmountStr();
    QVariant repairCostStr();
    QVariant realRepairCostStr();
    QVariant realRepairCostWords();
    QVariant partsCostStr();
    QVariant realWorksCostStr();
    QVariant realPartsCostStr();
    QVariant warranty();
    QVariant inDateTime();
    QVariant inDate();
    QVariant outDateTime();
    QVariant outDate();
protected slots:
    void setDataRework(const int index, QVariant &data);
    void logDataChange(const int index, const QVariant &data) override;
};

#endif // SREPAIRMODEL_H
