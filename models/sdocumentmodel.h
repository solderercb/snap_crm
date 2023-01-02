#ifndef SDOCUMENTMODEL_H
#define SDOCUMENTMODEL_H

#include <QObject>
#include "global.h"
#include "scomrecord.h"

class SDocumentModel : public SComRecord
{
    Q_OBJECT
public:
    // 1 Приходная накладная\r\n2 Расходная накладная\r\n3 Списание товара\r\n4 Накладные перемещения\r\n5 счета\r\n6 резерв товара\r\n8 выкуп техники
    enum Type {InInvoice = 1, OutInvoice = 2, WriteOff = 3, MoveInvoice = 4, Bill = 5, ReserveInvoice = 6, Buyout = 8};
    // 0 не активна\r\n1 активна\r\n2 ПН проведена (создан РКО)\r\n3 Резерв товара\r\n4 резерв снят\r\n5 РН проведена (создан ПКО)\r\n6 списание товара\r\n6 Списание товара ОК\r\n7 РН распроведена
    enum State {Draft = 0, Active, InInvoicePayed, ItemsReserved, ReserveCancelled, OutInvoicePayed, ItemsWrittenOff, OutInvoiceCancelled};
    explicit SDocumentModel(QObject *parent = nullptr);
    explicit SDocumentModel(int id, QObject *parent = nullptr);
    ~SDocumentModel();
    int id();
    void setId(int);
    void load();
    void load(int);
    int type();
    void setType(int);
    int state();
    void setState(int);
    int client();
    void setClient(int);  // установка id клиента
    void unsetClient();
    int paymentSystem();
    int paymentSystemIndex();
    void setPaymentSystem(int);
    double amount();
    QString amountLocal();
    void setAmount(double);
    double currencyRate();
    QString currencyRateLocal();
    void setCurrencyRate(double);
    int company();
    int companyIndex();
    void setCompany(int company);
    int priceOption();
    int priceOptionIndex();
    void setPriceOption(int);
    QString notes();
    void setNotes(QString);
    QString reason();
    void setReason(QString);
    int orderId();
    void setOrderId(const int);
    int reserveDays();
    void setReserveDays(int);
    QString trackingNumber();
    void setTrackingNumber(const QString&);
    bool isValid();
    bool commit();
    QString title();

private:
    int m_id = 0;
    int m_type = 0;
    int m_state = 0;
    bool m_realization = 0;
    int m_paymentSystem = 0;
    int m_company = 0;
    int m_office = 0;
    int m_store = 0;
    int m_user = 0;
    double m_amount = 0;
    QString m_notes;
    QDateTime m_updated;
    int m_client = 0;
    double m_currencyRate = 1.00;
    QString m_reason;
    int m_cashOrder = 0;
    int m_priceOption = 0;
    int m_returnPercent = 0;
    int m_reserveDays = 0;
    int m_master = 0;
    int m_repair = 0;
    bool m_worksIncluded = 0;
    int m_bill = 0;
    QString m_trackingNumber;
    int m_d_store = 0;
    bool m_d_pay = 0;
};

#endif // SDOCUMENTMODEL_H
