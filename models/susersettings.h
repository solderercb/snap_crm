#ifndef SUSERSETTINGS_H
#define SUSERSETTINGS_H
#include <QPrinterInfo>
#include "propstruct.h"
#include "models/sdatabaseauxiliary.h"
#include "models/susersettingstypes.h"
#include "ssqlquerymodel.h"

class SUserSettings : public SPropertyCollection, public SDatabaseAuxiliary
{
    Q_OBJECT
    // Страница настроек уч. записи сотрудника
    PROPSTRUCT_LINEEDIT(QString, username, 1, username)
    PROPSTRUCT_LINEEDIT(QString, name, 1, name)
    PROPSTRUCT_LINEEDIT(QString, surname, 1, surname)
    PROPSTRUCT_LINEEDIT(QString, patronymic, 1, patronymic)
    PROPSTRUCT_COMBOBOX(QString, roles, 1, ignore, QString())
    PROPSTRUCT_COMBOBOX(int, office, 1, office, 1)
    PROPSTRUCT_LINEEDIT(QString, address, 1, address)
    PROPSTRUCT_LINEEDIT(QString, password, 1, ignore)
    PROPSTRUCT_LINEEDIT(QString, notes, 1, notes)
    PROPSTRUCT_LINEEDIT(QString, signature, 1, signature)
    PROPSTRUCT_LINEEDIT(QString, phone, 1, phone)
    PROPSTRUCT_COMBOBOX(int, phoneMask, 1, phone_mask, QString())
    PROPSTRUCT_LINEEDIT(QString, phone2, 1, phone2)
    PROPSTRUCT_COMBOBOX(int, phone2Mask, 1, phone2_mask, QString())
    PROPSTRUCT_LINEEDIT(QString, sipUserId, 1, sip_user_id)
    PROPSTRUCT_LINEEDIT(QString, email, 1, email)
    PROPSTRUCT_DATETIMEEDIT(QDateTime, birthday, 1, birthday)
    PROPSTRUCT_LINEEDIT(QString, inn, 1, inn)
    PROPSTRUCT_COMBOBOX(int, kkt, 1, kkt, QString())
    PROPSTRUCT_COMBOBOX(int, pinpad, 1, pinpad, QString())
    PROPSTRUCT_CHECKBOX(bool, cardOnCall, 1, card_on_call)
    PROPSTRUCT_CHECKBOX(bool, trackActivity, 1, track_activity)
    PROPSTRUCT_CHECKBOX(int, state, 1, state)
    PROPSTRUCT_CHECKBOX(bool, isBot, 1, is_bot)

    PROPSTRUCT_FIELD(QByteArray, photo, photo)

    // новые параметры:
    PROPSTRUCT_FIELD(int, clientUserId, client_user_id)

    PROPSTRUCT_LINEEDIT(QString, passportNum, 2, passport_num)
    PROPSTRUCT_DATETIMEEDIT(QDateTime, passportDate, 2, passport_date)
    PROPSTRUCT_LINEEDIT(QString, passportOrgan, 2, passport_organ)

    PROPSTRUCT_SPINBOX(int, salaryRate, 3, salary_rate, 0, 999999)
    PROPSTRUCT_SPINBOX(int, payDayOff, 3, pay_day_off, 0, 999999)
    PROPSTRUCT_SPINBOX(int, payDay, 3, pay_day, 0, 999999)
    PROPSTRUCT_SPINBOX(int, payDeviceIn, 3, pay_device_in, 0, 999999)
    PROPSTRUCT_SPINBOX(int, payDeviceOut, 3, pay_device_out, 0, 999999)

    PROPSTRUCT_SPINBOX(int, payRepair, 3, pay_repair, 0, 100)
    PROPSTRUCT_SPINBOX(int, payRepairQuick, 3, pay_repair_quick, 0, 100)
    PROPSTRUCT_SPINBOX(int, payCartridgeRefill, 3, pay_cartridge_refill, 0, 100)
    PROPSTRUCT_SPINBOX(int, paySale, 3, pay_sale, 0, 100)
    PROPSTRUCT_SPINBOX(int, payRepairQSale, 3, pay_repair_q_sale, 0, 100)
    PROPSTRUCT_CHECKBOX(bool, pay4SaleInRepair, 3, pay_4_sale_in_repair)

    // служебные параметры уч. записи сотрудника
    PROPSTRUCT_FIELD(int, id, id)
    PROPSTRUCT_FIELD(QDateTime, created, created)
    PROPSTRUCT_FIELD(QDateTime, lastLogin, last_login)
    PROPSTRUCT_FIELD(QDateTime, lastActivity, last_activity)
    PROPSTRUCT_FIELD(int, currentOffice, ignore)
    PROPSTRUCT_FIELD(int, company, ignore)
    // Страница персональных настроек
    PROPSTRUCT_COMBOBOX(QString, rowColor, 11, row_color, QString("#D8008EA4"))
    PROPSTRUCT_COMBOBOX(QString, colorLabelWs, 11, color_label_ws, QString("#FF000000"))
    PROPSTRUCT_SPINBOX(int, fontSize, 11, fontsize, 0, 16)
    PROPSTRUCT_SPINBOX(int, rowHeight, 11, rowheight, 0, 40)
    PROPSTRUCT_COMBOBOX(QString, geHighlightColor, 11, ge_highlight_color, QString("#FFFFDD70"))

    PROPSTRUCT_COMBOBOX(int, defaultDocumentPrinter, 12, ignore, 0)
    PROPSTRUCT_COMBOBOX(int, defaultStickerPrinter, 12, ignore, 0)
    PROPSTRUCT_CHECKBOX(bool, previewBeforePrint, 12, preview_before_print)
    PROPSTRUCT_COMBOBOX(int, defaultPosPrinter, 12, ignore, 0)

    PROPSTRUCT_CHECKBOX(bool, alternateRowsBackground, 13, usersParams.alternate_rows_background)
    PROPSTRUCT_CHECKBOX(bool, displayOut, 13, display_out)
    PROPSTRUCT_CHECKBOX(bool, displayComplete, 13, display_complete)
    PROPSTRUCT_CHECKBOX(bool, newOnTop, 13, new_on_top)
    PROPSTRUCT_CHECKBOX(bool, saveStateOnClose, 13, save_state_on_close)
    PROPSTRUCT_CHECKBOX(bool, autoRefreshWorkspace, 13, auto_refresh_workspace)
    PROPSTRUCT_SPINBOX(int, refreshTime, 13, refresh_time, 0, 1800)
    PROPSTRUCT_COMBOBOX(QString, issuedColor, 13, issued_color, QString("#00000000"))
    PROPSTRUCT_COMBOBOX(int, defOffice, 13, def_office, 1)
    PROPSTRUCT_COMBOBOX(int, defEmployee, 13, def_employee, id)
    PROPSTRUCT_COMBOBOX(int, defStatus, 13, def_status, QString())
    PROPSTRUCT_COMBOBOX(int, defWsFilter, 13, def_ws_filter, 0)
    // новые параметры:
    PROPSTRUCT_CHECKBOX(int, autosavePartList, 13, usersParams.autosave_part_list)
    PROPSTRUCT_CHECKBOX(int, autosaveDiagResult, 13, usersParams.autosave_diag_result)

    PROPSTRUCT_CHECKBOX(bool, preferRegular, 14, prefer_regular)

    PROPSTRUCT_CHECKBOX(bool, groupStoreItems, 15, group_store_items)
    PROPSTRUCT_COMBOBOX(int, defStore, 15, def_store, 1)

    PROPSTRUCT_COMBOBOX(int, xlsC1, 16, xls_c1, 0)
    PROPSTRUCT_COMBOBOX(int, xlsC2, 16, xls_c2, 1)
    PROPSTRUCT_COMBOBOX(int, xlsC3, 16, xls_c3, 2)
    PROPSTRUCT_COMBOBOX(int, xlsC4, 16, xls_c4, 3)
    PROPSTRUCT_COMBOBOX(int, xlsC14, 16, xls_c14, 13)
    PROPSTRUCT_COMBOBOX(int, xlsC6, 16, xls_c6, 5)
    PROPSTRUCT_COMBOBOX(int, xlsC5, 16, xls_c5, 4)
    PROPSTRUCT_COMBOBOX(int, xlsC7, 16, xls_c7, 6)
    PROPSTRUCT_COMBOBOX(int, xlsC8, 16, xls_c8, 7)
    PROPSTRUCT_COMBOBOX(int, xlsC9, 16, xls_c9, 8)
    PROPSTRUCT_COMBOBOX(int, xlsC10, 16, xls_c10, 9)
    PROPSTRUCT_COMBOBOX(int, xlsC11, 16, xls_c11, 10)
    PROPSTRUCT_COMBOBOX(int, xlsC12, 16, xls_c12, 11)
    PROPSTRUCT_COMBOBOX(int, xlsC15, 16, xls_c15, 14)
    PROPSTRUCT_COMBOBOX(int, xlsC13, 16, xls_c13, 12)

    PROPSTRUCT_RADIOBTN(int, workspaceRepairs, 17, ignore)
    PROPSTRUCT_RADIOBTN(int, workspaceItems, 17, ignore)
    PROPSTRUCT_FIELD(int, workspaceMode, workspace_mode)

    // новые параметры:
    PROPSTRUCT_COMBOBOX(int, defaultPaymentSystem, 18, usersParams.default_payment_system, 0)

    // не нашел где настраивается
//    PROPSTRUCT_CHECKBOX(bool, informComment, inform_comment)
//    PROPSTRUCT_CHECKBOX(bool, informStatus, inform_status)
//    PROPSTRUCT_CHECKBOX(bool, advanceDisable, advance_disable)
//    PROPSTRUCT_CHECKBOX(bool, salaryDisable, salary_disable)
//    PROPSTRUCT_FIELD(int, kkmPass, kkm_pass)
//    PROPSTRUCT_CHECKBOX(int, sex, sex)
//    PROPSTRUCT_COMBOBOX(int, defItemState, def_item_state)
//    PROPSTRUCT_SPINBOX(int, newRepDocCopies, new_rep_doc_copies)
//    PROPSTRUCT_JSON_ARRAY(QString, fieldsCfg, fields_cfg) // похоже, не используется
//    QString, animation, animation

public:
    enum Table{Users = 0, UsersParams = 1};
    enum WidgetType{Label, Editor};
    enum XlsColumns{ColumnA = 0, ColumnB, ColumnC, ColumnD, ColumnE, ColumnF, ColumnG, ColumnH, ColumnI, ColumnJ, ColumnK, ColumnL, ColumnM, ColumnN, ColumnO, NoColumn = 99};
    Q_ENUM(XlsColumns)
    explicit SUserSettings();
    ~SUserSettings();
    void initWidgets();
    void load(const QString &username);
    void loadFromUsersParams();
    void loadPrinterSettings();
    bool commit();
    void save();
    void saveToUsersParams();
    void savePrinterSettings();
    void updateLoginTimestamp();
    void updateActivityTimestamp(const bool standalone = true);
private:
    QMap<QString, int> m_params;
    QStringList m_printersList;
    SStandardItemModel *xlsColumnsList = nullptr;
    void setComboBoxModel(const QString propertyName, SSqlQueryModel* model);
    void disableWidget(const QString propertyName);
    void prepareUpdateList(Table table);
    void translate();
};

#endif // SUSERSETTINGS_H)
