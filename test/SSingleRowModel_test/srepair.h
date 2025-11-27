#ifndef SREPAIR_H
#define SREPAIR_H

#include "../../models/ssinglerowjmodel.h"

#include "../../models/ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов

// Список элементов
/* Формат декларации
    - <имя поля в таблице БД>,
    - <имя поля, соответствующее синтаксису C>,
    - <тип>,
    - <0|1 - добавить элемент в список полей, доступных в отчетах)
*/
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(Hidden, hidden, bool, 0)                                    \
    TABLE_FIELD(Title, title, QString, 1)                                   \
    TABLE_FIELD(client, clientId, int, 1)                                   \
    TABLE_FIELD(type, devClass, int, 1)                                     \
    TABLE_FIELD(maker, vendor, int, 1)                                      \
    TABLE_FIELD(model, device, int, 1)                                      \
    TABLE_FIELD(serial_number, serialNumber, QString, 1)                    \
    TABLE_FIELD(company, company, int, 1)                                   \
    TABLE_FIELD(office, office, int, 1)                                     \
    TABLE_FIELD(start_office, startOffice, int, 1)                          \
    TABLE_FIELD(manager, manager, int, 1)                                   \
    TABLE_FIELD(current_manager, currentManager, int, 1)                    \
    TABLE_FIELD(master, engineer, int, 1)                                   \
    TABLE_FIELD(diagnostic_result, diagnosticResult, QString, 1)            \
    TABLE_FIELD(in_date, inDate, QDateTime, 1)                              \
    TABLE_FIELD(out_date, outDate, QDateTime, 1)                            \
    TABLE_FIELD(state, state, int, 1)                                       \
    TABLE_FIELD(new_state, newState, int, 0)                                \
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
    TABLE_FIELD(box, box, int, 1)                                           \
    TABLE_FIELD(warranty_label, warrantyLlabel, QString, 1)                 \
    TABLE_FIELD(ext_notes, extNotes, QString, 1)                            \
    TABLE_FIELD(is_prepaid, isPrepaid, bool, 1)                             \
    TABLE_FIELD(prepaid_type, prepaidType, int, 1)                          \
    TABLE_FIELD(prepaid_summ, prepaidSumm, double, 1)                       \
    TABLE_FIELD(prepaid_order, prepaidOrder, int, 1)                        \
    TABLE_FIELD(is_pre_agreed, isPreAgreed, bool, 1)                        \
    TABLE_FIELD(is_debt, isDebt, bool, 1)                                   \
    TABLE_FIELD(pre_agreed_amount, preAgreedAmount, double, 1)              \
    TABLE_FIELD(repair_cost, repairCost, double, 1)                         \
    TABLE_FIELD(real_repair_cost, realRepairCost, double, 1)                \
    TABLE_FIELD(parts_cost, partsCost, double, 1)                           \
    TABLE_FIELD(fault, fault, QString, 1)                                   \
    TABLE_FIELD(complect, complect, QString, 1)                             \
    TABLE_FIELD(look, look, QString, 1)                                     \
    TABLE_FIELD(thirs_party_sc, thirsPartySc, bool, 1)                      \
    TABLE_FIELD(last_save, lastSave, QDateTime, 0)                          \
    TABLE_FIELD(last_status_changed, lastStatusChanged, QDateTime, 0)       \
    TABLE_FIELD(warranty_days, warranty, int, 1)                            \
    TABLE_FIELD(barcode, barcode, QString, 1)                               \
    TABLE_FIELD(reject_reason, rejectReason, QString, 1)                    \
    TABLE_FIELD(informed_status, informedStatus, int, 0)                    \
    TABLE_FIELD(image_ids, imageIds, QString, 0)                            \
    TABLE_FIELD(color, color, QString, 1)                                   \
    TABLE_FIELD(order_moving, orderMoving, QString, 0)                      \
    TABLE_FIELD(early, early, int, 1)                                       \
    TABLE_FIELD(ext_early, extEarly, QString, 1)                            \
    TABLE_FIELD(issued_msg, issuedMsg, QString, 1)                          \
    TABLE_FIELD(sms_inform, smsInform, bool, 1)                             \
    TABLE_FIELD(invoice, invoice, int, 1)                                   \
    TABLE_FIELD(cartridge, isCartridge, int, 1)                             \
    TABLE_FIELD(vendor_id, vendorId, int, 0)                                \
    TABLE_FIELD(termsControl, termsControl, bool, 1)

// Список дополнительных полей для отчетов
// Описание смотри в ssinglerowmodel.h
#undef ADDITIONAL_REPORT_FIELDS
#define ADDITIONAL_REPORT_FIELDS                                            \
    ADDITIONAL_REPORT_FIELD(inDateTime, inDateTime)                         \
    ADDITIONAL_REPORT_FIELD(outDateTime, outDateTime)

class SRepair : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
public:
    SRepair(QObject *parent = nullptr);
#include "../../models/ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
private:
    void updateLogAssociatedRecId() override;
    void loadError(const int type, const QString &text = QString());
public Q_SLOTS:
    QVariant inDateTime();
    QVariant outDateTime();
private slots:
    void logDataChange(const int index, const QVariant &data) override;
};

#endif // SREPAIR_H
