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
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(int operationType READ operationType)
    Q_PROPERTY(double amount READ amount)
    Q_PROPERTY(QString amountStr READ amountStr)
    Q_PROPERTY(int invoiceId READ invoiceId)
    Q_PROPERTY(int client READ client)
    Q_PROPERTY(QString reason READ reason)
    Q_PROPERTY(int repairId READ repairId)
    Q_PROPERTY(int documentId READ documentId)
    Q_PROPERTY(int systemId READ systemId)
    Q_PROPERTY(int employee READ employee)
    Q_PROPERTY(int user READ user)
    Q_PROPERTY(int company READ company)
    Q_PROPERTY(int office READ office)
    Q_PROPERTY(int img READ img)
    Q_PROPERTY(int cardInfo READ cardInfo)
    Q_PROPERTY(double cardFee READ cardFee)
    Q_PROPERTY(bool isBackdate READ isBackdate)
    Q_PROPERTY(int fdn READ fdn)
    Q_PROPERTY(int paymentItemSign READ paymentItemSign)
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
    void setId(const int);
    void load();
    void load(int);
    bool commit();
    bool commit(double);
    int operationType();
    void setOperationType(int);
    double amount();
    double amountAbs();
    void setAmount(double);
    int client();
    void setClient(int);
    void unsetClient(); // удаление id клиента (анонимный)
    int systemId();
    void setSystemId(int);   // ID платёжной системы (`payment_systems`.`system_id`)
    QString reason();
    void setReason(const QString &);
    int currency();
    void setCurrency(int);
    void setCreated(const QDateTime &timestamp);
    void setCreatedDate(const QDate &date);
    void setCreatedTime(const QTime &time);
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
    QString amountStr();
    void setAmountStr(const QString);//summa_str
    int employee();
    void setEmployee(const int id);//to_user
    int user();
    void setUser(const int);//user
    int company();
    void setCompany(const int);//company
    int office();
    int img();
    void setImg(const int);//img
    int cardInfo();
    void setCardInfo(const int);//card_info
    double cardFee();
    void setCardFee(const double);//card_fee
    bool isBackdate();
    void setIsBackdate(const bool);//is_backdate
    QString customerEmail();
    void setCustomerEmail(const QString &email);
    int fdn();
    void setFdn(const int);//fdn
    int paymentItemSign();
    void setPaymentItemSign(const int);//payment_item_sign
    void initDemo() override;
private:
    void fieldsVerifyFormatter();
    int m_type = -1;
    int m_systemId = userDbData->value("defaultPaymentSystem", 0).toInt();       // ID платёжной системы (DB: payment_systems.system_id)
    int m_client = 0;
    int m_currency = 0;
    QStringList fields_verify;
    double m_amount = 0;
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
    int m_img;
    int m_cardInfo;
    double m_cardFee;
    bool m_isBackdate;
    QString m_customerEmail;
    int m_fdn;
    int m_paymentItemAttribute;
    bool m_skipLogRecording = 0;
signals:
};

#endif // SCASHREGISTERMODEL_H
