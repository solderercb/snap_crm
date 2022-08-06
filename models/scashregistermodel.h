/*  Модель кассы
 *  Содержит все методы, необходимые для работы с платежами.
 *  Первоначальный план не предусматривает работу с РРО/ККТ.
 */
#ifndef SCASHREGISTERMODEL_H
#define SCASHREGISTERMODEL_H

#include <QObject>
#include "global.h"
#include "amountToWords.h"
#include "com_sql_queries.h"
#include "models/scomrecord.h"
#include "models/slogrecordmodel.h"

class SCashRegisterModel : public SComRecord
{
    Q_OBJECT
public:
    enum PaymentType{PaymentSimple = 1, PaymentInvoice = 2, PaymentZ = 3, PaymentBalance = 4, PaymentSubsist = 5,
                     PaymentSalary = 6, AddSubCash = 7, MoneybackRepair = 8, MoneybackGoods = 9, ReceiptSimple = 11,
                     ReceiptPrepayRepair = 12, ReceiptBalance = 13, ReceiptGoods = 14, ReceiptRepair = 15, ReceiptInvoice = 17,
                     MoveCash = 18, PaymentDealer = 19, PaymentCustom = 50};
    explicit SCashRegisterModel(QObject *parent = nullptr);
    explicit SCashRegisterModel(int systemId, QObject *parent = nullptr);
    ~SCashRegisterModel();
    void setId(int);
    void load();
    void load(int);
    bool commit();
    bool commit(float);
    void setOperationType(int);
    void setClient(int);
    void unsetClient(); // удаление id клиента (анонимный)
    void setSystemId(int);   // ID платёжной системы (`payment_systems`.`system_id`)
    void setReason(const QString &);
    void setCurrency(int);
    void setCreated(QDateTime);
    int documentId();
    void setDocumentId(int);
    int repairId();
    void setRepairId(int);
    void setLogText(const QString &);

    // методы, в необходимости которых я пока не увеврен
    int client();
    int systemId();
    QString reason();
    int currency();
    int operationType();

private:
    void fieldsVerifyFormatter();
    int m_systemId = userDbData->value("defaultPaymentSystem", 0).toInt();       // ID платёжной системы (DB: payment_systems.system_id)
    int m_client = 0;
    int m_operationType = -1;
    QString m_reason;
    int m_currency = 0;
    QStringList fields_verify;
    int m_id;
    int m_type;
    float m_amount;
    QString m_amount_str;
    int m_invoice;
    int m_toUser;
    int m_user;
    int m_company;
    int m_office;
    QString m_notes;
    int m_repair;
    int m_document;
    QByteArray m_img;
    float m_cardFee;
    bool m_isBackdate;
    int m_cardInfo;
    QString m_customerEmail;
    int m_fdn;
    int m_paymentItemAttribute;

signals:

};

#endif // SCASHREGISTERMODEL_H
