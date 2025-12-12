#ifndef SCOMPANYMODEL_H
#define SCOMPANYMODEL_H

#include <SSingleRowJModel>

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(type, type, int, 0)                                         \
    TABLE_FIELD(name, name, QString, 1)                                     \
    TABLE_FIELD(inn, inn, QString, 1)                                       \
    TABLE_FIELD(kpp, kpp, QString, 1)                                       \
    TABLE_FIELD(ogrn, ogrn, QString, 1)                                     \
    TABLE_FIELD(ur_address, address, QString, 1)                            \
    TABLE_FIELD(site, site, QString, 1)                                     \
    TABLE_FIELD(email, email, QString, 1)                                   \
    TABLE_FIELD(logo, logo, QByteArray, 1)                                  \
    TABLE_FIELD(banks, banks, QString, 0)                                   \
    TABLE_FIELD(is_default, isDefault, bool, 0)                             \
    TABLE_FIELD(status, status, bool, 0)                                    \
    TABLE_FIELD(director, ceo, int, 0)                                      \
    TABLE_FIELD(accountant, cao, int, 0)                                    \
    TABLE_FIELD(tax_form, taxForm, int, 0)                                  \
    TABLE_FIELD(description, description, QString, 0)

// Список дополнительных полей для отчетов
#undef ADDITIONAL_REPORT_FIELDS
#define ADDITIONAL_REPORT_FIELDS                                            \
    ADDITIONAL_REPORT_FIELD(ceoLongName, ceoLongName)                       \
    ADDITIONAL_REPORT_FIELD(ceoShortName, ceoShortName)                     \
    ADDITIONAL_REPORT_FIELD(caoLongName, caoLongName)                       \
    ADDITIONAL_REPORT_FIELD(caoShortName, caoShortName)                     \

class SCompanyModel : public SSingleRowJModel
{
    Q_OBJECT
public:
    explicit SCompanyModel(QObject *parent = nullptr);
    ~SCompanyModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    static SCompanyModel* current(QObject *parent = nullptr);
    void load(const int &id);
private:
    static SCompanyModel* p_current;
    void userNames(const int id, QString &surname, QString &name, QString &patronymic);
public Q_SLOTS:
    // эти методы возвращают данные дополнительных полей в отчетах
    QVariant ceoLongName();
    QVariant ceoShortName();
    QVariant caoLongName();
    QVariant caoShortName();
};

#endif // SCOMPANYMODEL_H
