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
    enum PaymentType{ExpSimple = 1, ExpInvoice = 2, ExpZ = 3, ExpBalance = 4, ExpSubsist = 5,
                     ExpSalary = 6, AddSubCash = 7, ExpRepair = 8, ExpGoods = 9, RecptSimple = 11,
                     RecptPrepayRepair = 12, RecptBalance = 13, RecptGoods = 14, RecptRepair = 15,
                     ExpInvoiceUndo = 16, RecptInvoice = 17, MoveCash = 18, ExpDealer = 19, ExpRevert = 20,
                     ExpCustom = 50};
    explicit SCashRegisterModel(QObject *parent = nullptr);
    explicit SCashRegisterModel(int systemId, QObject *parent = nullptr);
    ~SCashRegisterModel();
    int id();
    void setId(int);
    void load();
    void load(int);
    bool commit();
    bool commit(float);
    void setOperationType(int);
    float amount();
    float amountAbs();
    void setAmount(float);
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
    int invoiceId();
    void setInvoiceId(int);
    void setLogText(const QString &);
    QString constructReason(const QString&);
    QString constructReason(int);
    void setSkipLogRecording(bool state = true);
    void setOffice(const int);
    void setOfficeIndex(const int);

    // методы, в необходимости которых я пока не увеврен
    int client();
    int systemId();
    QString reason();
    int currency();
    int operationType();

private:
    void fieldsVerifyFormatter();
    int m_type = -1;
    int m_systemId = userDbData->value("defaultPaymentSystem", 0).toInt();       // ID платёжной системы (DB: payment_systems.system_id)
    int m_client = 0;
    int m_currency = 0;
    QStringList fields_verify;
    float m_amount = 0;
    QString m_amount_str;
    QString m_amount_words;
    QString m_reason;
    int m_toUser;
    int m_user;
    int m_company;
    int m_office;
    int m_invoice = 0;
    int m_repair = 0;
    int m_document = 0;
    QByteArray m_img;
    float m_cardFee;
    bool m_isBackdate;
    int m_cardInfo;
    QString m_customerEmail;
    int m_fdn;
    int m_paymentItemAttribute;
    bool m_skipLogRecording = 0;

signals:

};

#endif // SCASHREGISTERMODEL_H
