#ifndef SPHONEMODEL_H
#define SPHONEMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
// Порядок полей должен совпадать с порядком в запросе QUERY_SEL_CLIENT_PHONES
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(phone, phone, QString, 0)                                   \
    TABLE_FIELD(phone_clean, phoneClean, QString, 0)                        \
    TABLE_FIELD(mask, mask, int, 0)                                         \
    TABLE_FIELD(customer, client, int, 0)                                   \
    TABLE_FIELD(type, type, int, 0)                                         \
    TABLE_FIELD(note, note, QString, 0)                                     \
    TABLE_FIELD(viber, viber, bool, 0)                                      \
    TABLE_FIELD(telegram, telegram, bool, 0)                                \
    TABLE_FIELD(whatsapp, whatsapp, bool, 0)                                \
    TABLE_FIELD(notify, receiveSMS, bool, 0)

class SPhoneModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class SPhonesModel;
signals:
    void markedPrimary(SPhoneModel*);
public:
    enum Types {Additional = 0, Primary = 1};
    enum Messengers {Viber = 1, Telegram = 2, Whatsapp = 4};
    Q_ENUM(Messengers);
    explicit SPhoneModel(QObject *parent = nullptr);
    explicit SPhoneModel(const QSqlRecord &record, QObject *parent = nullptr);
    ~SPhoneModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    bool commit() override;
    int maskIndex();
    void set_maskIndex(const int index);
    int messengers();
    void set_messengers(const int opt, bool state = 1);
    bool isNew();
    void set_primary(int primary = Primary);
    bool delDBRecord();
private:
    QString fieldsForSelectQuery() override;
    QString cleanupPhone(const QString &);
    void phoneChanged(const QString &data);
    void typeChanged(const int type);
    void messangersChanged(const int index, bool satate);
    void updateLogAssociatedRecId() override;
public slots:
    void setPrimaryUi();
    void logDataChange(const int, const QVariant &) override;
};

#endif // SPHONEMODEL_H
