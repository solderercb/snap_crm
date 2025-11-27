#ifndef SDOCUMENTMODEL_H
#define SDOCUMENTMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(type, type, int, 0)                                         \
    TABLE_FIELD(state, state, int, 0)                                       \
    TABLE_FIELD(is_realization, isRealization, bool, 0)                     \
    TABLE_FIELD(payment_system, paymentSystem, int, 0)                      \
    TABLE_FIELD(company, company, int, 0)                                   \
    TABLE_FIELD(store, store, int, 0)                                       \
    TABLE_FIELD(user, user, int, 0)                                         \
    TABLE_FIELD(total, amount, double, 0)                                   \
    TABLE_FIELD(notes, notes, QString, 0)                                   \
    TABLE_FIELD(created, created, QDateTime, 0)                             \
    TABLE_FIELD(updated_at, updated, QDateTime, 0)                          \
    TABLE_FIELD(office, office, int, 0)                                     \
    TABLE_FIELD(dealer, client, int, 0)                                     \
    TABLE_FIELD(currency_rate, currencyRate, double, 0)                     \
    TABLE_FIELD(img1, img1, QByteArray, 0)                                  \
    TABLE_FIELD(img2, img2, QByteArray, 0)                                  \
    TABLE_FIELD(img3, img3, QByteArray, 0)                                  \
    TABLE_FIELD(reason, reason, QString, 0)                                 \
    TABLE_FIELD(order_id, cashOrder, int, 0)                                \
    TABLE_FIELD(price_option, priceOption, int, 0)                          \
    TABLE_FIELD(return_percent, returnPercent, int, 0)                      \
    TABLE_FIELD(reserve_days, reserveDays, int, 0)                          \
    TABLE_FIELD(master_id, employee, int, 0)                                \
    TABLE_FIELD(repair_id, repair, int, 0)                                  \
    TABLE_FIELD(works_included, worksIncluded, bool, 0)                     \
    TABLE_FIELD(invoice, invoice, int, 0)                                   \
    TABLE_FIELD(track, trackingNumber, QString, 0)                          \
    TABLE_FIELD(d_store, dStore, int, 0)                                    \
    TABLE_FIELD(d_pay, dPay, bool, 0)

class SDocumentModel : public SSingleRowJModel
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
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load(int id);
    void setOfficeIndex(int index);
    void unsetClient();
    int paymentSystemIndex();
    QString amountLocal();
    QString currencyRateLocal();
    int companyIndex();
    void setCompanyIndex(int company);
    int priceOptionIndex();
    bool commit() override;
    QString title();
private:
    void updateLogAssociatedRecId() override;
    void trackingNumberChanged(const QString &track);
protected slots:
    void setDataRework(const int index, QVariant &data);
    void logDataChange(const int index, const QVariant &data) override;
};

#endif // SDOCUMENTMODEL_H
