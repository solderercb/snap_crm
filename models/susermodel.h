#ifndef SUSERMODEL_H
#define SUSERMODEL_H

#include <SSingleRowJModel>

class SClientModel;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(sip_user_id, sipId, int, 1)                                 \
    TABLE_FIELD(client_user_id, clientUserId, int, 0)                       \
    TABLE_FIELD(username, username, QString, 1)                             \
    TABLE_FIELD(name, name, QString, 1)                                     \
    TABLE_FIELD(surname, surname, QString, 1)                               \
    TABLE_FIELD(patronymic, patronymic, QString, 1)                         \
    TABLE_FIELD(phone, phone, QString, 1)                                   \
    TABLE_FIELD(phone2, phone2, QString, 1)                                 \
    TABLE_FIELD(phone_mask, phoneMask, int, 0)                              \
    TABLE_FIELD(phone2_mask, phone2Mask, int, 0)                            \
    TABLE_FIELD(address, address, QString, 1)                               \
    TABLE_FIELD(passport_num, passportNum, QString, 0)                      \
    TABLE_FIELD(passport_date, passportIssuedDate, QDate, 0)                \
    TABLE_FIELD(passport_organ, passportIssuedBy, QString, 0)               \
    TABLE_FIELD(state, state, int, 0)                                       \
    TABLE_FIELD(created, created, QDateTime, 1)                             \
    TABLE_FIELD(office, office, int, 0)                                     \
    TABLE_FIELD(birthday, birthday, QDateTime, 0)                           \
    TABLE_FIELD(def_office, defOffice, int, 0)                              \
    TABLE_FIELD(def_store, defStore, int, 0)                                \
    TABLE_FIELD(def_item_state, defItemState, int, 0)                       \
    TABLE_FIELD(def_employee, defEmployee, int, 0)                          \
    TABLE_FIELD(def_status, defStatus, int, 0)                              \
    TABLE_FIELD(def_ws_filter, defWsFilter, int, 0)                         \
    TABLE_FIELD(last_login, lastLogin, QDateTime, 0)                        \
    TABLE_FIELD(last_activity, lastActivity, QDateTime, 0)                  \
    TABLE_FIELD(email, email, QString, 1)                                   \
    TABLE_FIELD(sex, sex, int, 0)                                           \
    TABLE_FIELD(photo, photo, QByteArray, 1)                                \
    TABLE_FIELD(salary_rate, salaryRate, int, 0)                            \
    TABLE_FIELD(pay_day, payDay, int, 0)                                    \
    TABLE_FIELD(pay_day_off, payDayOff, int, 0)                             \
    TABLE_FIELD(pay_repair, payRepair, int, 0)                              \
    TABLE_FIELD(pay_repair_quick, payRepairQuick, int, 0)                   \
    TABLE_FIELD(pay_sale, paySale, int, 0)                                  \
    TABLE_FIELD(pay_repair_q_sale, payRepairQuickSale, int, 0)              \
    TABLE_FIELD(pay_cartridge_refill, payCartridgeRefill, int, 0)           \
    TABLE_FIELD(pay_device_in, payDeviceIn, int, 0)                         \
    TABLE_FIELD(pay_device_out, payDeviceOut, int, 0)                       \
    TABLE_FIELD(pay_4_sale_in_repair, payForSaleInRepair, int, 0)           \
    TABLE_FIELD(row_color, rowColor, QString, 0)                            \
    TABLE_FIELD(ge_highlight_color, geHighlightColor, QString, 0)           \
    TABLE_FIELD(color_label_ws, colorLabelWs, QString, 0)                   \
    TABLE_FIELD(workspace_mode, workspaceMode, int, 0)                      \
    TABLE_FIELD(preview_before_print, previewBeforePrint, int, 0)           \
    TABLE_FIELD(new_rep_doc_copies, newRepDocCopies, int, 0)                \
    TABLE_FIELD(auto_refresh_workspace, autoRefreshWorkspace, int, 0)       \
    TABLE_FIELD(refresh_time, refreshTime, int, 0)                          \
    TABLE_FIELD(xls_c1, xlsC1, int, 0)                                      \
    TABLE_FIELD(xls_c2, xlsC2, int, 0)                                      \
    TABLE_FIELD(xls_c3, xlsC3, int, 0)                                      \
    TABLE_FIELD(xls_c4, xlsC4, int, 0)                                      \
    TABLE_FIELD(xls_c5, xlsC5, int, 0)                                      \
    TABLE_FIELD(xls_c6, xlsC6, int, 0)                                      \
    TABLE_FIELD(xls_c7, xlsC7, int, 0)                                      \
    TABLE_FIELD(xls_c8, xlsC8, int, 0)                                      \
    TABLE_FIELD(xls_c9, xlsC9, int, 0)                                      \
    TABLE_FIELD(xls_c10, xlsC10, int, 0)                                    \
    TABLE_FIELD(xls_c11, xlsC11, int, 0)                                    \
    TABLE_FIELD(xls_c12, xlsC12, int, 0)                                    \
    TABLE_FIELD(xls_c13, xlsC13, int, 0)                                    \
    TABLE_FIELD(xls_c14, xlsC14, int, 0)                                    \
    TABLE_FIELD(xls_c15, xlsC15, int, 0)                                    \
    TABLE_FIELD(display_out, displayOut, int, 0)                            \
    TABLE_FIELD(display_complete, displayComplete, int, 0)                  \
    TABLE_FIELD(is_bot, isBot, int, 0)                                      \
    TABLE_FIELD(new_on_top, newOnTop, int, 0)                               \
    TABLE_FIELD(issued_color, issuedColor, QString, 0)                      \
    TABLE_FIELD(fields_cfg, fieldsCfg, QString, 0)                          \
    TABLE_FIELD(save_state_on_close, saveStateOnClose, int, 0)              \
    TABLE_FIELD(group_store_items, groupStoreItems, int, 0)                 \
    TABLE_FIELD(track_activity, trackActivity, int, 0)                      \
    TABLE_FIELD(card_on_call, cardOnCall, int, 0)                           \
    TABLE_FIELD(inn, inn, QString, 1)                                       \
    TABLE_FIELD(inform_comment, informComment, int, 0)                      \
    TABLE_FIELD(inform_status, informStatus, int, 0)                        \
    TABLE_FIELD(kkt, KKT, int, 0)                                           \
    TABLE_FIELD(pinpad, pinpad, int, 0)                                     \
    TABLE_FIELD(advance_disable, subsistenceDisable, int, 0)                \
    TABLE_FIELD(salary_disable, salaryDisable, int, 0)                      \
    TABLE_FIELD(notes, notes, QString, 1)                                   \
    TABLE_FIELD(signature, signature, QString, 0)                           \
    TABLE_FIELD(kkm_pass, kkmPwd, int, 0)                                   \
    TABLE_FIELD(prefer_regular, preferRegular, int, 0)                      \
    TABLE_FIELD(fontsize, fontSize, int, 0)                                 \
    TABLE_FIELD(rowheight, rowHeight, int, 0)                               \
    TABLE_FIELD(animation, animation, QString, 0)

// Список дополнительных полей для отчетов
#undef ADDITIONAL_REPORT_FIELDS
#define ADDITIONAL_REPORT_FIELDS                                            \
    ADDITIONAL_REPORT_FIELD(photo, photo)                                   \
    ADDITIONAL_REPORT_FIELD(fullLongName, fullLongName)                     \
    ADDITIONAL_REPORT_FIELD(fullShortName, fullShortName)

class SUserModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
signals:
    void sigModelReset();
public:
    explicit SUserModel(QObject *parent = nullptr);
    ~SUserModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load() override;
    void load(const int id);
    void setSalaryRateStartDate(const QDate date);
    double balance();
    SClientModel* clientModel();
    void setClientModel(SClientModel *model);
    bool commit() override;
    static QString fullLongName(const QString &surname, const QString &name, const QString &patronymic);   // Фамилия Имя Отчество
    static QString fullShortName(const QString &surname, const QString &name, const QString &patronymic);  // Фамилия И. О.
private:
    QDate m_salaryRateStartDate;
    SClientModel* m_clientModel = nullptr;
    QString constructSelectQuery() override;
    void loadSalaryRate();
    void updateLogAssociatedRecId() override;
    void overrideSalaryRate(const int rate);
public Q_SLOTS:
    QString fullLongName();   // Фамилия Имя Отчество
    QString fullShortName();  // Фамилия И. О.
};

#endif // SUSERMODEL_H
