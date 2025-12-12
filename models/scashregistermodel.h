/*  Модель кассы
 *  Содержит все методы, необходимые для работы с платежами.
 *  Первоначальный план не предусматривает работу с РРО/ККТ.
 */
#ifndef SCASHREGISTERMODEL_H
#define SCASHREGISTERMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(created, created, QDateTime, 1)                             \
    TABLE_FIELD(type, operationType, int, 1)                                \
    TABLE_FIELD(summa, amount, double, 1)                                   \
    TABLE_FIELD(summa_str, amountStr, QString, 1)                           \
    TABLE_FIELD(invoice, invoice, int, 1)                                   \
    TABLE_FIELD(client, client, int, 1)                                     \
    TABLE_FIELD(to_user, employee, int, 1)                                  \
    TABLE_FIELD(user, user, int, 1)                                         \
    TABLE_FIELD(company, company, int, 1)                                   \
    TABLE_FIELD(office, office, int, 1)                                     \
    TABLE_FIELD(notes, reason, QString, 1)                                  \
    TABLE_FIELD(repair, repair, int, 1)                                     \
    TABLE_FIELD(document, document, int, 1)                                 \
    TABLE_FIELD(img, imgId, int, 1)                                         \
    TABLE_FIELD(payment_system, systemId, int, 1)                           \
    TABLE_FIELD(card_fee, cardFee, double, 1)                               \
    TABLE_FIELD(is_backdate, isBackdate, bool, 1)                           \
    TABLE_FIELD(card_info, cardInfo, int, 1)                                \
    TABLE_FIELD(customer_email, customerEmail, QString, 1)                  \
    TABLE_FIELD(fdn, fiscalDocNum, int, 1)                                  \
    TABLE_FIELD(payment_item_sign, paymentItemSign, int, 1)

// Список дополнительных полей для отчетов
#undef ADDITIONAL_REPORT_FIELDS
#define ADDITIONAL_REPORT_FIELDS                                            \
    ADDITIONAL_REPORT_FIELD(invoiceNum, invoiceNum)                         \
    ADDITIONAL_REPORT_FIELD(haveCardFee, haveCardFee)                       \
    ADDITIONAL_REPORT_FIELD(haveFiscalDocumentNumber, haveFiscalDocumentNumber) \
    ADDITIONAL_REPORT_FIELD(img, img)                                       \
    ADDITIONAL_REPORT_FIELD(cardAmount, cardAmount)                         \
    ADDITIONAL_REPORT_FIELD(cardName, cardName)                             \
    ADDITIONAL_REPORT_FIELD(card, card)                                     \
    ADDITIONAL_REPORT_FIELD(cardExpiryDate, cardExpiryDate)                 \
    ADDITIONAL_REPORT_FIELD(cardAuthCode, cardAuthCode)                     \
    ADDITIONAL_REPORT_FIELD(pinpadId, pinpadId)                             \
    ADDITIONAL_REPORT_FIELD(terminalNum, terminalNum)

class SCashRegisterModel : public SSingleRowJModel
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
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load() override;
    void load(int);
    bool commit() override;
    bool commit(double);
    double amountAbs();
    void unsetClient(); // удаление id клиента (анонимный)
    void set_createdDate(const QDate &date);
    void set_createdTime(const QTime &time);
    void set_logText(const QString &);
    QString constructReason(const QString&);
    QString constructReason(int);
    void set_skipLogRecording(bool state = true);
    void set_officeIndex(const int);
    int companyIndex();
    void set_companyIndex(const int index);
    void initDemo() override;
private:
    QStringList fieldsVerifyFormatter();
    bool m_skipLogRecording = 0;
    void amountChanged(double amount);
    void reasonChanged(const QString &text);
    void employeeChanged(const int id);
    void documentChanged(int id);
    void repairChanged(int id);
    void invoiceChanged(int id);
public Q_SLOTS:
    // эти методы возвращают данные дополнительных полей в отчетах
    QVariant invoiceNum(){return "not_implemented";};
    QVariant haveCardFee(){return (cardFee() > 0);};
    QVariant haveFiscalDocumentNumber(){return (fiscalDocNum() > 0);};
    QVariant img(){return "not_implemented";};
    QVariant cardAmount(){return 0;};  // в АСЦ всегда выводится 0
    QVariant cardName(){return "not_implemented";};
    QVariant card(){return "not_implemented";};
    QVariant cardExpiryDate(){return "not_implemented";};
    QVariant cardAuthCode(){return "not_implemented";};
    QVariant pinpadId(){return 0;};  // в АСЦ всегда выводится 0
    QVariant terminalNum(){return "not_implemented";};
private slots:
    void setDataRework(const int index, QVariant &data);
    void logDataChange(const int index, const QVariant &data) override;
};

#endif // SCASHREGISTERMODEL_H
