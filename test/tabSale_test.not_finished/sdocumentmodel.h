#ifndef SDOCUMENTMODEL_H
#define SDOCUMENTMODEL_H

#include <QObject>
#include <QDate>
#include <QTime>
#include <QDateTime>

class SDocumentModel
{
public:
    enum Columns{C_cashOrder};
    enum State {Draft = 0, Active, InInvoicePayed, ItemsReserved, ReserveCancelled, OutInvoicePayed, ItemsWrittenOff, OutInvoiceCancelled};
    enum Type {InInvoice = 1, OutInvoice = 2, WriteOff = 3, MoveInvoice = 4, Bill = 5, ReserveInvoice = 6, Buyout = 8};
    explicit SDocumentModel(QObject *parent = nullptr);
    int id(){return 398465;};
    void load(const int);
    bool isPrimaryKeyValid(){return 1;};
    int state(){return 0;};
    int client(){return 1952;};
    int company(){return 1;};
    QString amountLocal(){return "1 024,00 ₽";};
    QDateTime createdUtc(){return QDateTime(QDate(2025,1,1), QTime(3,0,0));};
    QString createdStr(){return "01.01.2025 03:00:00";};
    int paymentSystemIndex(){return 0;};
    int companyIndex(){return 0;};
    int priceOptionIndex(){return 0;};
    int reserveDays(){return 5;};
    QString trackingNumber(){return "CN01234567789SU";};
    void set_trackingNumber(const QString&);
    QString notes(){return "примечание";};
    void set_client(int);
    void unsetClient();
    void set_amount(double);
    void set_state(int);
    void appendLogText(const QString&);
    bool commit(){return 1;};
    void setCompanyIndex(int);
    void set_paymentSystem(int);
    void set_priceOption(int);
    void set_reserveDays(int);
    void set_type(int);
    void set_notes(const QString&);
    void set_cashOrder(int);
    void setAllState(int);
    bool setData(const int &index, const QVariant &data);
};

#endif // SDOCUMENTMODEL_H
