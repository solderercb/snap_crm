#ifndef SCOMSETTINGS_H
#define SCOMSETTINGS_H

#include <SSettingsBase>
#ifndef SKIP_INCLUDES_IN_PREPROCESSING_EVALUATION
#include <SComSettingsTypes>
#include <SCheckComboBox>
#endif // SKIP_INCLUDES_IN_PREPROCESSING_EVALUATION
class SStandardItemModel;

#include "ssettings_predef.h"     // этот файл нужно подключать после SSettingsBase и до списка элементов
#define SETTING_FIELDS                                                                                                                                                       \
    SETTING_FIELD(id, id, int, WidgetTypes::NoWidget)                                                                                                                        \
    SETTING_FIELD(currency, currencyId, QString, WidgetTypes::ComboBox, 0, QString())                                                                                        \
    SETTING_FIELD(classic_kassa, classicKassa, int, WidgetTypes::CheckBox, 0)                                                                                                \
    SETTING_FIELD(time_zone, timeZoneId, int, WidgetTypes::ComboBox, 0, QString())                                                                                           \
    SETTING_FIELD(phone_mask1, ascPhoneMask1, QString, WidgetTypes::LineEdit, 0)                                                                                             \
    SETTING_FIELD(phone_mask2, ascPhoneMask2, QString, WidgetTypes::LineEdit, 0)                                                                                             \
    SETTING_FIELD(settings.update_channel, updateChannel, QString, WidgetTypes::LineEdit, 0)                                                                                 \
                                                                                                                                                                             \
    SETTING_FIELD(it_vis_opt, isPriceColOptVisible, int, WidgetTypes::CheckBox, 1)                                                                                           \
    SETTING_FIELD(it_vis_opt2, isPriceColOpt2Visible, int, WidgetTypes::CheckBox, 1)                                                                                         \
    SETTING_FIELD(it_vis_opt3, isPriceColOpt3Visible, int, WidgetTypes::CheckBox, 1)                                                                                         \
    SETTING_FIELD(it_vis_rozn, isPriceColRoznVisible, int, WidgetTypes::CheckBox, 1)                                                                                         \
    SETTING_FIELD(it_vis_price_for_sc, isPriceColServiceVisible, int, WidgetTypes::CheckBox, 1)                                                                              \
                                                                                                                                                                             \
    SETTING_FIELD(settings.voip, voipId, int, WidgetTypes::ComboBox, 2, QString())                                                                                           \
    SETTING_FIELD(realizator_enable, isRealizatorEnable, int, WidgetTypes::CheckBox, 2)                                                                                      \
    SETTING_FIELD(online_store, onlineStoreId, int, WidgetTypes::ComboBox, 2, QString())                                                                                     \
    SETTING_FIELD(cartridge_enable, isCartridgeRepairEnabled, int, WidgetTypes::CheckBox, 2)                                                                                 \
                                                                                                                                                                             \
    SETTING_FIELD(is_master_set_on_new, isEngineerRequiredOnDeviceRecept, int, WidgetTypes::CheckBox, 3)                                                                     \
    SETTING_FIELD(visit_source_force, isVisitSourceRequired, int, WidgetTypes::CheckBox, 3)                                                                                  \
    SETTING_FIELD(is_sn_req, isSerialNumberRequired, int, WidgetTypes::CheckBox, 3)                                                                                          \
    SETTING_FIELD(is_cartridge_sn_req, isCartridgeSerialNumberRequired, int, WidgetTypes::CheckBox, 3)                                                                       \
    SETTING_FIELD(history_by_sn, isSearchOnlyBySerial, int, WidgetTypes::CheckBox, 3)                                                                                        \
    SETTING_FIELD(is_photo_required, isPhotoOnReceptRequired, int, WidgetTypes::CheckBox, 3)                                                                                 \
    SETTING_FIELD(is_photo_out_req, isPhotoOnIssueRequired, int, WidgetTypes::CheckBox, 3)                                                                                   \
    SETTING_FIELD(diag_required, isDiagRequired, int, WidgetTypes::CheckBox, 3)                                                                                              \
    SETTING_FIELD(manual_maker, isVendorAddingAllowedOnRecept, int, WidgetTypes::CheckBox, 3)                                                                                \
    SETTING_FIELD(rep_price_by_manager, isRepairSummSetByManager, int, WidgetTypes::CheckBox, 3)                                                                             \
    SETTING_FIELD(debt_rep_2_salary, isPaySalaryForRepairsIssuedInDebt, int, WidgetTypes::CheckBox, 3)                                                                       \
    SETTING_FIELD(rep_auto_company, isAutoSetCompanyOnRepairRecept, int, WidgetTypes::CheckBox, 3)                                                                           \
    SETTING_FIELD(vw_enable, isVendorWarrantyEnabled, int, WidgetTypes::CheckBox, 3)                                                                                         \
    SETTING_FIELD(settings.use_simplified_cartridge_repair, useSimplifiedCartridgeRepair, int, WidgetTypes::CheckBox, 3)                                                     \
    SETTING_FIELD(card_close_time, autoCloseRepairTabTimeout, int, WidgetTypes::SpinBox, 3, 0, 32400)                                                                        \
    SETTING_FIELD(rep_img_limit, repairImagesLimit, int, WidgetTypes::SpinBox, 3, 0, 20)                                                                                     \
                                                                                                                                                                             \
    SETTING_FIELD_VARIANT_COPY(settings.auto_assign_users, repairDispatcherUsersJson, t_repairDispatcherUsers, WidgetTypes::NoWidget)                                        \
    SETTING_FIELD_VARIANT_COPY(settings.auto_assign_criteria, repairDispatcherAssignCriteriaJson, t_repairDispatcherAssignCriteria, WidgetTypes::NoWidget)                   \
    SETTING_FIELD(IGNORE, repairDispatcherUsers, QString, WidgetTypes::SCheckableComboBox, 4)                                                                                \
    SETTING_FIELD(IGNORE, repairDispatcherDayLimit, int, WidgetTypes::SpinBox, 4, 0, 7)                                                                                      \
    SETTING_FIELD(IGNORE, repairDispatcherStatuses, QString, WidgetTypes::SCheckableComboBox, 4)                                                                             \
                                                                                                                                                                             \
    SETTING_FIELD(is_patronymic_required, isClientPatronymicRequired, int, WidgetTypes::CheckBox, 5)                                                                         \
    SETTING_FIELD(email_required, isClientEmailRequired, int, WidgetTypes::CheckBox, 5)                                                                                      \
    SETTING_FIELD(address_required, isClientAddressRequired, int, WidgetTypes::CheckBox, 5)                                                                                  \
    SETTING_FIELD(phone_required, isClientPhoneRequired, int, WidgetTypes::CheckBox, 5)                                                                                      \
    SETTING_FIELD(clients_are_dealers, isAnyClientDealer, int, WidgetTypes::CheckBox, 5)                                                                                     \
                                                                                                                                                                             \
    SETTING_FIELD(give2user_time, timeoutForItemsRequestsHandling, int, WidgetTypes::SpinBox, 6, 0, 72)                                                                      \
    SETTING_FIELD(diag_accept_time, timeoutForDiagnosisConfirmation, int, WidgetTypes::SpinBox, 6, 0, 72)                                                                    \
                                                                                                                                                                             \
    SETTING_FIELD(default_reserve_days, defaultItemReserveTime, int, WidgetTypes::SpinBox, 7, 0, 30)                                                                         \
    SETTING_FIELD(item_img_limit, itemImagesLimit, int, WidgetTypes::SpinBox, 7, 0, 20)                                                                                      \
    SETTING_FIELD(is_reason_req, isReasonForItemIncomeRequired, int, WidgetTypes::CheckBox, 7)                                                                               \
                                                                                                                                                                             \
    SETTING_FIELD(default_works_warranty, defaultWorksWarranty, int, WidgetTypes::ComboBox, 8, 31)                                                                           \
    SETTING_FIELD(default_items_warranty, defaultNewItemsWarranty, int, WidgetTypes::ComboBox, 8, 31)                                                                        \
    SETTING_FIELD(default_items_used_warranty, defaultUsedItemsWarranty, int, WidgetTypes::ComboBox, 8, 7)                                                                   \
    SETTING_FIELD(default_items_rep_warranty, defaultRefItemsWarranty, int, WidgetTypes::ComboBox, 8, 31)                                                                    \
    SETTING_FIELD(default_items_razb_warranty, defaultDisasmItemsWarranty, int, WidgetTypes::ComboBox, 8, 7)                                                                 \
    SETTING_FIELD(default_items_other_warranty, defaultOtherItemsWarranty, int, WidgetTypes::ComboBox, 8, 0)                                                                 \
                                                                                                                                                                             \
    SETTING_FIELD(settings.inform_comment, notifyNewComment, int, WidgetTypes::CheckBox, 9)	/* все настройки группы 9 хранятся в табл. settings */                           \
    SETTING_FIELD(settings.inform_comment_color, notifyNewCommentColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                                 \
    SETTING_FIELD(settings.inform_status_color, notifyRepairStatusUpdateColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                          \
    SETTING_FIELD(settings.inform_sms, notifyIncomingSMS, int, WidgetTypes::CheckBox, 9)                                                                                     \
    SETTING_FIELD(settings.inform_sms_color, notifyIncomingSMSColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                                    \
    SETTING_FIELD(settings.inform_terms_color, notifyOutOfTermAlarmColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                               \
    SETTING_FIELD(settings.inform_task_match, notifyDeviceMatch, int, WidgetTypes::CheckBox, 9)                                                                              \
    SETTING_FIELD(settings.inform_task_match_color, notifyDeviceMatchColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                             \
    SETTING_FIELD(settings.inform_task_custom, notifyCustomTask, int, WidgetTypes::CheckBox, 9)                                                                              \
    SETTING_FIELD(settings.inform_task_custom_color, notifyCustomTaskColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                             \
    SETTING_FIELD(settings.inform_int_request, notifyItemRequest, int, WidgetTypes::CheckBox, 9)                                                                             \
    SETTING_FIELD(settings.inform_int_request_color, notifyItemRequestColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                            \
    SETTING_FIELD(settings.inform_task_request, notifyOrderFromOnlineStore, int, WidgetTypes::CheckBox, 9)                                                                   \
    SETTING_FIELD(settings.inform_task_request_color, notifyOrderFromOnlineStoreColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                  \
    SETTING_FIELD(settings.inform_call, notifyIncomingCall, int, WidgetTypes::CheckBox, 9)                                                                                   \
    SETTING_FIELD(settings.inform_call_color, notifyIncomingCallColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                                  \
    SETTING_FIELD(settings.inform_part_request, notifyItemPurchaseRequest, int, WidgetTypes::CheckBox, 9)                                                                    \
    SETTING_FIELD(settings.inform_part_request_color, notifyItemPurchaseRequestColor, int, WidgetTypes::ComboBox, 9, QString("#FFFFFFFF"))                                   \
                                                                                                                                                                             \
    SETTING_FIELD(qs_print_pko, printPKO, int, WidgetTypes::CheckBox, 10)                                                                                                    \
    SETTING_FIELD(qs_print_rn, printOutInvoice, int, WidgetTypes::CheckBox, 10)                                                                                              \
    SETTING_FIELD(qs_print_pn, printInInvoice, int, WidgetTypes::CheckBox, 10)                                                                                               \
    SETTING_FIELD(qs_print_rko, printRKO, int, WidgetTypes::CheckBox, 10)                                                                                                    \
    SETTING_FIELD(print_warranty, printWarrantyDoc, int, WidgetTypes::CheckBox, 10)                                                                                          \
    SETTING_FIELD(print_works, printWorksList, int, WidgetTypes::CheckBox, 10)                                                                                               \
    SETTING_FIELD(print_diagnostic, printDiagResult, int, WidgetTypes::CheckBox, 10)                                                                                         \
    SETTING_FIELD(print_reject, printRepairRejectDoc, int, WidgetTypes::CheckBox, 10)                                                                                        \
    SETTING_FIELD(rko_on_pn, printRKOOnItemIncome, int, WidgetTypes::CheckBox, 10)                                                                                           \
    SETTING_FIELD(print_check, printCheck, int, WidgetTypes::CheckBox, 10)                                                                                                   \
    SETTING_FIELD(settings.print_new_repair_report, printRepairReceptDoc, int, WidgetTypes::CheckBox, 10)                                                                    \
    SETTING_FIELD(settings.print_rep_stickers, printRepairStickers, int, WidgetTypes::CheckBox, 10)                                                                          \
    SETTING_FIELD(settings.print_new_cartridge_report, printCartridgeReceptDoc, int, WidgetTypes::CheckBox, 10)                                                              \
    SETTING_FIELD(settings.print_cartridge_stickers, printCartridgeStickers, int, WidgetTypes::CheckBox, 10)                                                                 \
    SETTING_FIELD(rep_stickers_copy, defaultRepairStickersQty, int, WidgetTypes::SpinBox, 10, 0, 10)                                                                         \
                                                                                                                                                                             \
    SETTING_FIELD(auto_switch_layout, autoSwitchKeyboardLayout, int, WidgetTypes::CheckBox, 11)                                                                              \
                                                                                                                                                                             \
    SETTING_FIELD(aster_host, voipAsteriskHost, QString, WidgetTypes::LineEdit, 12)	/* только для asterisk */                                                                \
    SETTING_FIELD(aster_port, voipAsteriskPort, int, WidgetTypes::SpinBox, 12, 0, 65535)                                                                                     \
    SETTING_FIELD(aster_login, voipAsteriskLogin, QString, WidgetTypes::LineEdit, 12)                                                                                        \
    SETTING_FIELD(aster_password, voipAsteriskPassword, QString, WidgetTypes::LineEdit, 12)                                                                                  \
    SETTING_FIELD(aster_web_port, voipAsteriskWebPort, int, WidgetTypes::SpinBox, 12, 0, 65535)                                                                              \
    SETTING_FIELD(settings.voip_prefix, voipPrefix, QString, WidgetTypes::LineEdit, 12)	/* используется только для Asterisk */                                               \
    SETTING_FIELD(settings.voip_endpoint, voipEndpoint, QString, WidgetTypes::LineEdit, 12)	/* Используется только при выборе оператора Megafon */                           \
    SETTING_FIELD(settings.voip_key, voipKey, QString, WidgetTypes::LineEdit, 12)	/* используется для операторов Zadarma, Rostelecom, Mango Telecom, Megafon */            \
    SETTING_FIELD(settings.voip_secret, voipSecret, QString, WidgetTypes::LineEdit, 12)	/* используется для операторов Zadarma, Rostelecom, Mango Telecom */                 \
                                                                                                                                                                             \
    SETTING_FIELD(online_store_api, onlineStoreUrl, QString, WidgetTypes::LineEdit, 13)                                                                                      \
    SETTING_FIELD(online_store_key, onlineStoreKey, QString, WidgetTypes::LineEdit, 13)                                                                                      \
                                                                                                                                                                             \
    SETTING_FIELD(settings.classic_salary, salaryClassic, int, WidgetTypes::CheckBox, 14)                                                                                    \
    SETTING_FIELD(settings.salary_include_not_issued_by_default, salaryIncludeNotIssuedByDefault, int, WidgetTypes::CheckBox, 14)                                            \
    SETTING_FIELD(settings.new_client_sms_enabled, newClientSmsEnabled, int, WidgetTypes::CheckBox, 14)                                                                      \
                                                                                                                                                                             \
    SETTING_FIELD_VARIANT_COPY(statuses, repairStatuses, t_repairStatuses, WidgetTypes::NoWidget)                                                                            \
                                                                                                                                                                             \
    SETTING_FIELD(IGNORE, exchangeTypeAuto, bool, WidgetTypes::RadioButton, 21)	/* Настройки-»Финансы-»Операции с валютой, exchange_type */                                  \
    SETTING_FIELD(exchange_source, exchangeSourceId, int, WidgetTypes::ComboBox, 21, 1)	/* Настройки-»Финансы-»Операции с валютой */                                         \
    SETTING_FIELD(IGNORE, exchangeTypeManual, bool, WidgetTypes::RadioButton, 21)                                                                                            \
    SETTING_FIELD(exchange_rate, exchangeRate, double, WidgetTypes::DoubleSpinBox, 21, 0.00, 9999.99)	/* Настройки-»Финансы-»Операции с валютой */                         \
    SETTING_FIELD(backup_enable, backupEnable, int, WidgetTypes::CheckBox, 31)	/* Настройки-»Резервное копирование-»Расписание */                                           \
    SETTING_FIELD(backup_images, backupImages, int, WidgetTypes::CheckBox, 31)	/* Настройки-»Резервное копирование-»Расписание */                                           \
    SETTING_FIELD(backup_time, backupTime, QDateTime, WidgetTypes::DateTimeEdit, 31)	/* Настройки-»Резервное копирование-»Расписание */                                   \
                                                                                                                                                                             \
    SETTING_FIELD_VARIANT_COPY(sms_config, smsConfigJson, t_smsConfig, WidgetTypes::NoWidget)	/* Настройки-»Оповещения-»Настройки SMS-»Настройки соединения, JSON */       \
    SETTING_FIELD(IGNORE, smsProvider, int, WidgetTypes::ComboBox, 41, QString())                                                                                            \
    SETTING_FIELD(IGNORE, smsAuthType, int, WidgetTypes::ComboBox, 41, QString())                                                                                            \
    SETTING_FIELD(IGNORE, smsApiId, QString, WidgetTypes::LineEdit, 41)                                                                                                      \
    SETTING_FIELD(IGNORE, smsLogin, QString, WidgetTypes::LineEdit, 41)                                                                                                      \
    SETTING_FIELD(IGNORE, smsPassword, QString, WidgetTypes::LineEdit, 41)                                                                                                   \
    SETTING_FIELD(IGNORE, smsSender, QString, WidgetTypes::LineEdit, 41)                                                                                                     \
                                                                                                                                                                             \
    SETTING_FIELD_VARIANT_COPY(email_config, emailConfigJson, t_emailConfig, WidgetTypes::NoWidget)	/* Настройки-»Оповещения-»Настройка почтовых уведомлений-»Настройки соединения, JSON */      \
    SETTING_FIELD(IGNORE, emailServer, QString, WidgetTypes::LineEdit, 41)                                                                                                   \
    SETTING_FIELD(IGNORE, emailPort, int, WidgetTypes::SpinBox, 41, 0, 65535)                                                                                                \
    SETTING_FIELD(IGNORE, emailLogin, QString, WidgetTypes::LineEdit, 41)                                                                                                    \
    SETTING_FIELD(IGNORE, emailPassword, QString, WidgetTypes::LineEdit, 41)                                                                                                 \
    SETTING_FIELD(IGNORE, emailTimeout, int, WidgetTypes::SpinBox, 41, 0, 300)                                                                                               \
    SETTING_FIELD(IGNORE, emailEnableSsl, bool, WidgetTypes::CheckBox, 41)                                                                                                   \
    SETTING_FIELD(IGNORE, emailEnableImplicitSsl, bool, WidgetTypes::CheckBox, 41)                                                                                           \
    SETTING_FIELD(IGNORE, emailTemplate, QString, WidgetTypes::LineEdit, 41)                                                                                                 \

class SComSettings : public SSettingsBase
{
    Q_OBJECT
    friend class TClassTest;
public:
    enum Table{Config = 0, Settings = 0x200};   // для таблицы `config` в m_classDbMap зарезервированы индексы 0..511; для `settings` — 512..1023
    explicit SComSettings();
#include "ssettings_init.h"
public:
    void load() override;
    void save() override;
private:
    std::unique_ptr<QMap<int, int>> m_classDbMap;       // соответствие индексов полей класса полям таблицы
    std::shared_ptr<QSqlRecord> m_record_settings;      // данные таблицы `settings` (m_record содержит данные таблицы `config`)
    int dbFieldIndex(const int classIndex) const override;
    void insertNewField(const int index, const QString &name) override;
    int primaryKeyIndex() override {return C_id;};
    QVariant primaryKeyData() override {return 1;};
protected:
    void configureWidgets() override;
    QString table() override;
    QString dbFieldName(const int classIndex) const override;
    void loadFromTableSettings();
    void deserializeData();
    void loadFromJson(const int index, bool init = 0);
    QVariant loadedValue(const int dbTableIndex) const override;
    void updateJson(const int index, const QVariant &value);
    void saveToTableSettings();
    int targetTable(const int targetTableIndex) override;
    void translate();
protected slots:
    void setDataRework(const int index, QVariant &data);
    void dataChangeHandler(const int index, const QVariant &data) override;
};

#endif // SCOMSETTINGS_H
