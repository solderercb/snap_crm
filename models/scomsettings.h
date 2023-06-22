#ifndef SCOMSETTINGS_H
#define SCOMSETTINGS_H
#include <QObject>
#include <QMetaObject>
#include <QAbstractItemModel>
#include "propstruct.h"
#include "models/sdatabaseauxiliary.h"
#include "models/scomsettingstypes.h"

class SComSettings : public SPropertyCollection, public SDatabaseAuxiliary
{
    Q_OBJECT
    PROPSTRUCT_COMBOBOX(int, currencyId, 0, currency, QString())
    PROPSTRUCT_CHECKBOX(int, classicKassa, 0, classic_kassa)
    PROPSTRUCT_COMBOBOX(int, timeZoneId, 0, time_zone, QString())
    PROPSTRUCT_LINEEDIT(QString, ascPhoneMask1, 0, phone_mask1)
    PROPSTRUCT_LINEEDIT(QString, ascPhoneMask2, 0, phone_mask2)

    PROPSTRUCT_CHECKBOX(int, isPriceColOptVisible, 1, it_vis_opt)
    PROPSTRUCT_CHECKBOX(int, isPriceColOpt2Visible, 1, it_vis_opt2)
    PROPSTRUCT_CHECKBOX(int, isPriceColOpt3Visible, 1, it_vis_opt3)
    PROPSTRUCT_CHECKBOX(int, isPriceColRoznVisible, 1, it_vis_rozn)
    PROPSTRUCT_CHECKBOX(int, isPriceColServiceVisible, 1, it_vis_price_for_sc)

    PROPSTRUCT_COMBOBOX(int, voipId, 2, settings.voip, QString())
    PROPSTRUCT_CHECKBOX(int, isRealizatorEnable, 2, realizator_enable)
    PROPSTRUCT_COMBOBOX(int, onlineStoreId, 2, online_store, QString())
    PROPSTRUCT_CHECKBOX(int, isCartridgeRepairEnabled, 2, cartridge_enable)

    PROPSTRUCT_CHECKBOX(int, isEngineerRequiredOnDeviceRecept, 3, is_master_set_on_new)
    PROPSTRUCT_CHECKBOX(int, isVisitSourceRequired, 3, visit_source_force)
    PROPSTRUCT_CHECKBOX(int, isSerialNumberRequired, 3, is_sn_req)
    PROPSTRUCT_CHECKBOX(int, isCartridgeSerialNumberRequired, 3, is_cartridge_sn_req)
    PROPSTRUCT_CHECKBOX(int, isSearchOnlyBySerial, 3, history_by_sn)
    PROPSTRUCT_CHECKBOX(int, isPhotoOnReceptRequired, 3, is_photo_required)
    PROPSTRUCT_CHECKBOX(int, isPhotoOnIssueRequired, 3, is_photo_out_req)
    PROPSTRUCT_CHECKBOX(int, isDiagRequired, 3, diag_required)
    PROPSTRUCT_CHECKBOX(int, isVendorAddingAllowedOnRecept, 3, manual_maker)
    PROPSTRUCT_CHECKBOX(int, isRepairSummSetByManager, 3, rep_price_by_manager)
    PROPSTRUCT_CHECKBOX(int, isPaySalaryForRepairsIssuedInDebt, 3, debt_rep_2_salary)
    PROPSTRUCT_CHECKBOX(int, isAutoSetCompanyOnRepairRecept, 3, rep_auto_company)
    PROPSTRUCT_CHECKBOX(int, isVendorWarrantyEnabled, 3, vw_enable)
    PROPSTRUCT_SPINBOX(int, autoCloseRepairTabTimeout, 3, card_close_time, 0, 32400)
    PROPSTRUCT_SPINBOX(int, repairImagesLimit, 3, rep_img_limit, 0, 20)

    PROPSTRUCT_JSON_ARRAY(t_repairDispatcherUsers, repairDispatcherUsersJson, settings.auto_assign_users)
    PROPSTRUCT_JSON_OBJ(t_repairDispatcherAssignCriteria, repairDispatcherAssignCriteriaJson, settings.auto_assign_criteria)
    PROPSTRUCT_COMBOBOX(QString, repairDispatcherUsers, 4, ignore, "")
    PROPSTRUCT_SPINBOX(int, repairDispatcherDayLimit, 4, ignore, 0, 7)
    PROPSTRUCT_COMBOBOX(QString, repairDispatcherStatuses, 4, ignore, "")

    PROPSTRUCT_CHECKBOX(int, isClientPatronymicRequired, 5, is_patronymic_required)
    PROPSTRUCT_CHECKBOX(int, isClientEmailRequired, 5, email_required)
    PROPSTRUCT_CHECKBOX(int, isClientAddressRequired, 5, address_required)
    PROPSTRUCT_CHECKBOX(int, isClientPhoneRequired, 5, phone_required)
    PROPSTRUCT_CHECKBOX(int, isAnyClientDealer, 5, clients_are_dealers)

    PROPSTRUCT_SPINBOX(int, timeoutForItemsRequestsHandling, 6, give2user_time, 0, 72)
    PROPSTRUCT_SPINBOX(int, timeoutForDiagnosisConfirmation, 6, diag_accept_time, 0, 72)

    PROPSTRUCT_SPINBOX(int, defaultItemReserveTime, 7, default_reserve_days, 0, 30)
    PROPSTRUCT_SPINBOX(int, itemImagesLimit, 7, item_img_limit, 0, 20)
    PROPSTRUCT_CHECKBOX(int, isReasonForItemIncomeRequired, 7, is_reason_req)

    PROPSTRUCT_COMBOBOX(int, defaultWorksWarranty, 8, default_works_warranty, 31)
    PROPSTRUCT_COMBOBOX(int, defaultNewItemsWarranty, 8, default_items_warranty, 31)
    PROPSTRUCT_COMBOBOX(int, defaultUsedItemsWarranty, 8, default_items_used_warranty, 7)
    PROPSTRUCT_COMBOBOX(int, defaultRefItemsWarranty, 8, default_items_rep_warranty, 31)
    PROPSTRUCT_COMBOBOX(int, defaultDisasmItemsWarranty, 8, default_items_razb_warranty, 7)
    PROPSTRUCT_COMBOBOX(int, defaultOtherItemsWarranty, 8, default_items_other_warranty, 0)

    PROPSTRUCT_CHECKBOX(int, notifyNewComment, 9, settings.inform_comment)   // все настройки группы 9 хранятся в табл. settings
    PROPSTRUCT_COMBOBOX(int, notifyNewCommentColor, 9, settings.inform_comment_color, QString("#FFFFFFFF"))
    PROPSTRUCT_COMBOBOX(int, notifyRepairStatusUpdateColor, 9, settings.inform_status_color, QString("#FFFFFFFF"))
    PROPSTRUCT_CHECKBOX(int, notifyIncomingSMS, 9, settings.inform_sms)
    PROPSTRUCT_COMBOBOX(int, notifyIncomingSMSColor, 9, settings.inform_sms_color, QString("#FFFFFFFF"))
    PROPSTRUCT_COMBOBOX(int, notifyOutOfTermAlarmColor, 9, settings.inform_terms_color, QString("#FFFFFFFF"))
    PROPSTRUCT_CHECKBOX(int, notifyDeviceMatch, 9, settings.inform_task_match)
    PROPSTRUCT_COMBOBOX(int, notifyDeviceMatchColor, 9, settings.inform_task_match_color, QString("#FFFFFFFF"))
    PROPSTRUCT_CHECKBOX(int, notifyCustomTask, 9, settings.inform_task_custom)
    PROPSTRUCT_COMBOBOX(int, notifyCustomTaskColor, 9, settings.inform_task_custom_color, QString("#FFFFFFFF"))
    PROPSTRUCT_CHECKBOX(int, notifyItemRequest, 9, settings.inform_int_request)
    PROPSTRUCT_COMBOBOX(int, notifyItemRequestColor, 9, settings.inform_int_request_color, QString("#FFFFFFFF"))
    PROPSTRUCT_CHECKBOX(int, notifyOrderFromOnlineStore, 9, settings.inform_task_request)
    PROPSTRUCT_COMBOBOX(int, notifyOrderFromOnlineStoreColor, 9, settings.inform_task_request_color, QString("#FFFFFFFF"))
    PROPSTRUCT_CHECKBOX(int, notifyIncomingCall, 9, settings.inform_call)
    PROPSTRUCT_COMBOBOX(int, notifyIncomingCallColor, 9, settings.inform_call_color, QString("#FFFFFFFF"))
    PROPSTRUCT_CHECKBOX(int, notifyItemPurchaseRequest, 9, settings.inform_part_request)
    PROPSTRUCT_COMBOBOX(int, notifyItemPurchaseRequestColor, 9, settings.inform_part_request_color, QString("#FFFFFFFF"))

    PROPSTRUCT_CHECKBOX(int, printPKO, 10, qs_print_pko)
    PROPSTRUCT_CHECKBOX(int, printOutInvoice, 10, qs_print_rn)
    PROPSTRUCT_CHECKBOX(int, printInInvoice, 10, qs_print_pn)
    PROPSTRUCT_CHECKBOX(int, printRKO, 10, qs_print_rko)
    PROPSTRUCT_CHECKBOX(int, printWarrantyDoc, 10, print_warranty)
    PROPSTRUCT_CHECKBOX(int, printWorksList, 10, print_works)
    PROPSTRUCT_CHECKBOX(int, printDiagResult, 10, print_diagnostic)
    PROPSTRUCT_CHECKBOX(int, printRepairRejectDoc, 10, print_reject)
    PROPSTRUCT_CHECKBOX(int, printRKOOnItemIncome, 10, rko_on_pn)
    PROPSTRUCT_CHECKBOX(int, printCheck, 10, print_check)
    PROPSTRUCT_CHECKBOX(int, printRepairReceptDoc, 10, settings.print_new_repair_report)    // таблица settings!
    PROPSTRUCT_CHECKBOX(int, printRepairStickers, 10, settings.print_rep_stickers)    // таблица settings!
    PROPSTRUCT_CHECKBOX(int, printCartridgeReceptDoc, 10, settings.print_new_cartridge_report)    // таблица settings!
    PROPSTRUCT_CHECKBOX(int, printCartridgeStickers, 10, settings.print_cartridge_stickers)    // таблица settings!
    PROPSTRUCT_SPINBOX(int, defaultRepairStickersQty, 10, rep_stickers_copy, 0, 10)

    PROPSTRUCT_CHECKBOX(int, autoSwitchKeyboardLayout, 11, auto_switch_layout)

    PROPSTRUCT_LINEEDIT(QString, voipAsteriskHost, 12, aster_host)   // только для asterisk
    PROPSTRUCT_SPINBOX(int, voipAsteriskPort, 12, aster_port, 0, 65535)
    PROPSTRUCT_LINEEDIT(QString, voipAsteriskLogin, 12, aster_login)
    PROPSTRUCT_LINEEDIT(QString, voipAsteriskPassword, 12, aster_password)
    PROPSTRUCT_SPINBOX(int, voipAsteriskWebPort, 12, aster_web_port, 0, 65535)
    PROPSTRUCT_LINEEDIT(QString, voipPrefix, 12, settings.voip_prefix)    // используется только для Asterisk
    PROPSTRUCT_LINEEDIT(QString, voipEndpoint, 12, settings.voip_endpoint)    // Используется только при выборе оператора Megafon
    PROPSTRUCT_LINEEDIT(QString, voipKey, 12, settings.voip_key)  // используется для операторов Zadarma, Rostelecom, Mango Telecom, Megafon
    PROPSTRUCT_LINEEDIT(QString, voipSecret, 12, settings.voip_secret)  // используется для операторов Zadarma, Rostelecom, Mango Telecom

    PROPSTRUCT_LINEEDIT(QString, onlineStoreUrl, 13, online_store_api)
    PROPSTRUCT_LINEEDIT(QString, onlineStoreKey, 13, online_store_key)

    // параметры таблицы settings, добавленные в этой программе
//    PROPSTRUCT_CHECKBOX(int, autosave_part_list, , autosave_part_list) // это должно быть в персональных настройка пользователя
    PROPSTRUCT_CHECKBOX(int, salaryClassic, 14, settings.classic_salary)
    PROPSTRUCT_CHECKBOX(int, salaryIncludeNotIssuedByDefault, 14, settings.salary_include_not_issued_by_default)
    PROPSTRUCT_CHECKBOX(int, newClientSmsEnabled, 14, settings.new_client_sms_enabled)

    PROPSTRUCT_JSON_ARRAY(t_repairStatuses, statusesJson, statuses)

    PROPSTRUCT_RADIOBTN(bool, exchangeTypeAuto, 21, ignore) // Настройки-»Финансы-»Операции с валютой, exchange_type
    PROPSTRUCT_COMBOBOX(int, exchangeSourceId, 21, exchange_source, 1) // Настройки-»Финансы-»Операции с валютой
    PROPSTRUCT_RADIOBTN(bool, exchangeTypeManual, 21, ignore)
    PROPSTRUCT_DSPINBOX(double, exchangeRate, 21, exchange_rate, 0, 9999) // Настройки-»Финансы-»Операции с валютой
    PROPSTRUCT_CHECKBOX(int, backupEnable, 31, backup_enable) // Настройки-»Резервное копирование-»Расписание
    PROPSTRUCT_CHECKBOX(int, backupImages, 31, backup_images) // Настройки-»Резервное копирование-»Расписание
    PROPSTRUCT_DATETIMEEDIT(QDateTime, backupTime, 31, backup_time) // Настройки-»Резервное копирование-»Расписание

    PROPSTRUCT_JSON_OBJ(t_smsConfig, smsConfigJson, sms_config)    // Настройки-»Оповещения-»Настройки SMS-»Настройки соединения, JSON
    PROPSTRUCT_COMBOBOX(int, smsProvider, 41, ignore, QString())
    PROPSTRUCT_COMBOBOX(int, smsAuthType, 41, ignore, QString())
    PROPSTRUCT_LINEEDIT(QString, smsApiId, 41, ignore)
    PROPSTRUCT_LINEEDIT(QString, smsLogin, 41, ignore)
    PROPSTRUCT_LINEEDIT(QString, smsPassword, 41, ignore)
    PROPSTRUCT_LINEEDIT(QString, smsSender, 41, ignore)

    PROPSTRUCT_JSON_OBJ(t_emailConfig, emailConfigJson, email_config)    // Настройки-»Оповещения-»Настройки SMS-»Настройки соединения, JSON
    PROPSTRUCT_LINEEDIT(QString, emailServer, 41, ignore)
    PROPSTRUCT_SPINBOX(int, emailPort, 41, ignore, 0, 65535)
    PROPSTRUCT_LINEEDIT(QString, emailLogin, 41, ignore)
    PROPSTRUCT_LINEEDIT(QString, emailPassword, 41, ignore)
    PROPSTRUCT_SPINBOX(int, emailTimeout, 41, ignore, 0, 300)
    PROPSTRUCT_CHECKBOX(bool, emailEnableSsl, 41, ignore)
    PROPSTRUCT_CHECKBOX(bool, emailEnableImplicitSsl, 41, ignore)
    PROPSTRUCT_LINEEDIT(QString, emailTemplate, 41, ignore)

    // другие параметры таблицы config
//    (int, id, , id)
//    (varchar, configcol, , configcol) // не нашел что это
//    PROPSTRUCT_FIELD(QDateTime, config_modified, , config_modified)   // похоже, не используется
//    (tinyint, print_rep_stickers, , print_rep_stickers)   // не используется, заменено значением в табл. settings
//    (tinyint, close_clients, , close_clients)   // похоже, не используется
//    (int, images_width, , images_width)   // похоже, не используется
//    (int, salary_day, , salary_day)   // похоже, не используется
//    (tinyint, parts_included, , parts_included)   // похоже, не используется
//    (text, key, , key)    // ключ лицензии АСЦ
//    (int, video_codec, , video_codec)     // Давным давно видеозапись отключена (говорили, что временно) и, похоже, насовсем
//    (tinyint, print_new_repair_report, , print_new_repair_report)   // не используется, заменено значением в табл. settings
//    (int, regular_customer_step, , regular_customer_step)     // похоже, не используется
//    PROPSTRUCT_FIELD(QString, TimeOfWork, , TimeOfWork)     // похоже, не используется; должно быть в настройках офиса; JSON
//    (tinyint, termsControl, , termsControl)     // похоже, не используется; параметр задаётся отдельно для каждого статусаы
//    PROPSTRUCT_FIELD(QString, version)      // здесь это не нужно
//    PROPSTRUCT_FIELD(QString, version_snap, , version_snap)      // здесь это не нужно

public:
    enum Table{Config = 0, Settings = 1};
    explicit SComSettings();
    void initWidgets();
    void load();
    void loadFromTableSettings();
    void loadFromJson();
    void save();
    void saveToTableSettings();
    void updateJson();
    bool isDirty();
private:
    void prepareUpdateList(Table table);
    void translate();
};

#endif // SCOMSETTINGS_H
