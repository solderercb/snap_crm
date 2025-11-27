#ifndef SUSERSETTINGS_H
#define SUSERSETTINGS_H
#include <SSettingsBase>
#include <SSingleRowModelBase>

class SStandardItemModel;
class SSqlQueryModel;

#include "ssettings_predef.h"     // этот файл нужно подключать после SSettingsBase и до списка элементов
#define SETTING_FIELDS                                                                                                 \
    SETTING_FIELD(id, id, int, WidgetTypes::NoWidget)	                                                               \
    SETTING_FIELD(username, username, QString, WidgetTypes::LineEdit, 1)	                                           \
    SETTING_FIELD(name, name, QString, WidgetTypes::LineEdit, 1)	                                                   \
    SETTING_FIELD(surname, surname, QString, WidgetTypes::LineEdit, 1)	                                               \
    SETTING_FIELD(patronymic, patronymic, QString, WidgetTypes::LineEdit, 1)	                                       \
    SETTING_FIELD(IGNORE, roles, QString, WidgetTypes::ComboBox, 1, QString())	                                       \
    SETTING_FIELD(office, office, int, WidgetTypes::ComboBox, 1, 1)	                                                   \
    SETTING_FIELD(address, address, QString, WidgetTypes::LineEdit, 1)	                                               \
    SETTING_FIELD(IGNORE, password, QString, WidgetTypes::LineEdit, 1)	                                               \
    SETTING_FIELD(notes, notes, QString, WidgetTypes::LineEdit, 1)	                                                   \
    SETTING_FIELD(signature, signature, QString, WidgetTypes::LineEdit, 1)	                                           \
    SETTING_FIELD(phone, phone, QString, WidgetTypes::LineEdit, 1)	                                                   \
    SETTING_FIELD(phone_mask, phoneMask, int, WidgetTypes::ComboBox, 1, QString())	                                   \
    SETTING_FIELD(phone2, phone2, QString, WidgetTypes::LineEdit, 1)	                                               \
    SETTING_FIELD(phone2_mask, phone2Mask, int, WidgetTypes::ComboBox, 1, QString())	                               \
    SETTING_FIELD(sip_user_id, sipUserId, QString, WidgetTypes::LineEdit, 1)	                                       \
    SETTING_FIELD(email, email, QString, WidgetTypes::LineEdit, 1)	                                                   \
    SETTING_FIELD(birthday, birthday, QDateTime, WidgetTypes::DateTimeEdit, 1)	                                       \
    SETTING_FIELD(inn, inn, QString, WidgetTypes::LineEdit, 1)	                                                       \
    SETTING_FIELD(kkt, kkt, int, WidgetTypes::ComboBox, 1, QString())	                                               \
    SETTING_FIELD(pinpad, pinpad, int, WidgetTypes::ComboBox, 1, QString())	                                           \
    SETTING_FIELD(card_on_call, cardOnCall, bool, WidgetTypes::CheckBox, 1)	                                           \
    SETTING_FIELD(track_activity, trackActivity, bool, WidgetTypes::CheckBox, 1)	                                   \
    SETTING_FIELD(state, state, int, WidgetTypes::CheckBox, 1)	                                                       \
    SETTING_FIELD(is_bot, isBot, bool, WidgetTypes::CheckBox, 1)	                                                   \
                                                                                                                       \
    SETTING_FIELD(photo, photo, QByteArray, WidgetTypes::NoWidget)	                                                   \
                                                                                                                       \
    /* новые параметры: */                                                                                             \
    SETTING_FIELD(client_user_id, clientUserId, int, WidgetTypes::NoWidget)	                                           \
                                                                                                                       \
    SETTING_FIELD(passport_num, passportNum, QString, WidgetTypes::LineEdit, 2)	                                       \
    SETTING_FIELD(passport_date, passportDate, QDateTime, WidgetTypes::DateTimeEdit, 2)	                               \
    SETTING_FIELD(passport_organ, passportOrgan, QString, WidgetTypes::LineEdit, 2)	                                   \
                                                                                                                       \
    SETTING_FIELD(salary_rate, salaryRate, int, WidgetTypes::SpinBox, 3, 0, 999999)	                                   \
    SETTING_FIELD(pay_day_off, payDayOff, int, WidgetTypes::SpinBox, 3, 0, 999999)	                                   \
    SETTING_FIELD(pay_day, payDay, int, WidgetTypes::SpinBox, 3, 0, 999999)	                                           \
    SETTING_FIELD(pay_device_in, payDeviceIn, int, WidgetTypes::SpinBox, 3, 0, 999999)	                               \
    SETTING_FIELD(pay_device_out, payDeviceOut, int, WidgetTypes::SpinBox, 3, 0, 999999)	                           \
                                                                                                                       \
    SETTING_FIELD(pay_repair, payRepair, int, WidgetTypes::SpinBox, 3, 0, 100)	                                       \
    SETTING_FIELD(pay_repair_quick, payRepairQuick, int, WidgetTypes::SpinBox, 3, 0, 100)	                           \
    SETTING_FIELD(pay_cartridge_refill, payCartridgeRefill, int, WidgetTypes::SpinBox, 3, 0, 100)	                   \
    SETTING_FIELD(pay_sale, paySale, int, WidgetTypes::SpinBox, 3, 0, 100)	                                           \
    SETTING_FIELD(pay_repair_q_sale, payRepairQSale, int, WidgetTypes::SpinBox, 3, 0, 100)	                           \
    SETTING_FIELD(pay_4_sale_in_repair, pay4SaleInRepair, bool, WidgetTypes::CheckBox, 3)	                           \
                                                                                                                       \
    /* служебные параметры уч. записи сотрудника */                                                                    \
    SETTING_FIELD(created, created, QDateTime, WidgetTypes::NoWidget)	                                               \
    SETTING_FIELD(last_login, lastLogin, QDateTime, WidgetTypes::NoWidget)	                                           \
    SETTING_FIELD(last_activity, lastActivity, QDateTime, WidgetTypes::NoWidget)	                                   \
    SETTING_FIELD(IGNORE, currentOffice, int, WidgetTypes::NoWidget)	                                               \
    SETTING_FIELD(IGNORE, company, int, WidgetTypes::NoWidget)	                                                       \
    /* Страница персональных настроек */                                                                               \
    SETTING_FIELD(row_color, rowColor, QString, WidgetTypes::ComboBox, 11, QString("#D8008EA4"))	                   \
    SETTING_FIELD(fontsize, fontSize, int, WidgetTypes::SpinBox, 11, 0, 16)	                                           \
    SETTING_FIELD(rowheight, rowHeight, int, WidgetTypes::SpinBox, 11, 0, 40)	                                       \
    SETTING_FIELD(IGNORE, fontFamily, QString, WidgetTypes::ComboBox, 11, QString("Segoe UI"))	                       \
    SETTING_FIELD(ge_highlight_color, geHighlightColor, QString, WidgetTypes::ComboBox, 11, QString("#FFFFDD70"))	   \
                                                                                                                       \
    SETTING_FIELD(IGNORE, defaultDocumentPrinter, int, WidgetTypes::ComboBox, 12, -1)	                               \
    SETTING_FIELD(IGNORE, defaultStickerPrinter, int, WidgetTypes::ComboBox, 12, -1)	                               \
    SETTING_FIELD(preview_before_print, previewBeforePrint, bool, WidgetTypes::CheckBox, 12)	                       \
    SETTING_FIELD(IGNORE, defaultPosPrinter, int, WidgetTypes::ComboBox, 12, -1)	                                   \
                                                                                                                       \
    SETTING_FIELD(usersParams.alternate_rows_background, alternateRowsBackground, bool, WidgetTypes::CheckBox, 13)	   \
    SETTING_FIELD(display_out, displayOut, bool, WidgetTypes::CheckBox, 13)	                                           \
    SETTING_FIELD(display_complete, displayComplete, bool, WidgetTypes::CheckBox, 13)	                               \
    SETTING_FIELD(new_on_top, newOnTop, bool, WidgetTypes::CheckBox, 13)	                                           \
    SETTING_FIELD(save_state_on_close, saveStateOnClose, bool, WidgetTypes::CheckBox, 13)	                           \
    SETTING_FIELD(auto_refresh_workspace, autoRefreshWorkspace, bool, WidgetTypes::CheckBox, 13)	                   \
    SETTING_FIELD(refresh_time, refreshTime, int, WidgetTypes::SpinBox, 13, 0, 1800)	                               \
    SETTING_FIELD(issued_color, issuedColor, QString, WidgetTypes::ComboBox, 13, QString("#00000000"))	               \
    SETTING_FIELD(def_office, defOffice, int, WidgetTypes::ComboBox, 13, 1)	                                           \
    SETTING_FIELD(def_employee, defEmployee, int, WidgetTypes::ComboBox, 13, id)	                                   \
    SETTING_FIELD(def_status, defStatus, int, WidgetTypes::ComboBox, 13, QString())	                                   \
    SETTING_FIELD(def_ws_filter, defWsFilter, int, WidgetTypes::ComboBox, 13, 0)	                                   \
    /* новые параметры: */                                                                                             \
    SETTING_FIELD(usersParams.autosave_part_list, autosavePartList, int, WidgetTypes::CheckBox, 13)	                   \
    SETTING_FIELD(usersParams.autosave_diag_result, autosaveDiagResult, int, WidgetTypes::CheckBox, 13)	               \
    SETTING_FIELD(usersParams.regexp_search_repair, useRegExpSearch, bool, WidgetTypes::CheckBox, 13)	               \
                                                                                                                       \
    SETTING_FIELD(prefer_regular, preferRegular, bool, WidgetTypes::CheckBox, 14)	                                   \
                                                                                                                       \
    SETTING_FIELD(group_store_items, groupStoreItems, bool, WidgetTypes::CheckBox, 15)	                               \
    SETTING_FIELD(def_store, defStore, int, WidgetTypes::ComboBox, 15, 1)	                                           \
                                                                                                                       \
    SETTING_FIELD(xls_c1, xlsC1, int, WidgetTypes::ComboBox, 16, 0)	                                                   \
    SETTING_FIELD(xls_c2, xlsC2, int, WidgetTypes::ComboBox, 16, 1)	                                                   \
    SETTING_FIELD(xls_c3, xlsC3, int, WidgetTypes::ComboBox, 16, 2)	                                                   \
    SETTING_FIELD(xls_c4, xlsC4, int, WidgetTypes::ComboBox, 16, 3)	                                                   \
    SETTING_FIELD(xls_c14, xlsC14, int, WidgetTypes::ComboBox, 16, 13)	                                               \
    SETTING_FIELD(xls_c6, xlsC6, int, WidgetTypes::ComboBox, 16, 5)	                                                   \
    SETTING_FIELD(xls_c5, xlsC5, int, WidgetTypes::ComboBox, 16, 4)	                                                   \
    SETTING_FIELD(xls_c7, xlsC7, int, WidgetTypes::ComboBox, 16, 6)	                                                   \
    SETTING_FIELD(xls_c8, xlsC8, int, WidgetTypes::ComboBox, 16, 7)	                                                   \
    SETTING_FIELD(xls_c9, xlsC9, int, WidgetTypes::ComboBox, 16, 8)	                                                   \
    SETTING_FIELD(xls_c10, xlsC10, int, WidgetTypes::ComboBox, 16, 9)	                                               \
    SETTING_FIELD(xls_c11, xlsC11, int, WidgetTypes::ComboBox, 16, 10)	                                               \
    SETTING_FIELD(xls_c12, xlsC12, int, WidgetTypes::ComboBox, 16, 11)	                                               \
    SETTING_FIELD(xls_c15, xlsC15, int, WidgetTypes::ComboBox, 16, 14)	                                               \
    SETTING_FIELD(xls_c13, xlsC13, int, WidgetTypes::ComboBox, 16, 12)	                                               \
                                                                                                                       \
    SETTING_FIELD(IGNORE, workspaceRepairs, int, WidgetTypes::RadioButton, 17)	                                       \
    SETTING_FIELD(IGNORE, workspaceItems, int, WidgetTypes::RadioButton, 17)	                                       \
    SETTING_FIELD(workspace_mode, workspaceMode, int, WidgetTypes::NoWidget)	                                       \
                                                                                                                       \
    /* новые параметры: */                                                                                             \
    SETTING_FIELD(usersParams.default_payment_system, defaultPaymentSystem, int, WidgetTypes::ComboBox, 18, 0)         \

    /* не нашел где настраивается */
//    SETTING_FIELD(color_label_ws, colorLabelWs, QString, WidgetTypes::NoWidget)            /* в АСЦ не обнаружен */
//    SETTING_FIELD(inform_comment, informComment, bool, WidgetTypes::CheckBox)
//    SETTING_FIELD(inform_status, informStatus, bool, WidgetTypes::CheckBox)
//    SETTING_FIELD(advance_disable, advanceDisable, bool, WidgetTypes::CheckBox)
//    SETTING_FIELD(salary_disable, salaryDisable, bool, WidgetTypes::CheckBox)
//    SETTING_FIELD(kkm_pass, kkmPass, int, WidgetTypes::NoWidget)
//    SETTING_FIELD(sex, sex, int, WidgetTypes::CheckBox)
//    SETTING_FIELD(def_item_state, defItemState, int, WidgetTypes::ComboBox)
//    SETTING_FIELD(new_rep_doc_copies, newRepDocCopies, int, WidgetTypes::SpinBox)
//    SETTING_FIELD(fields_cfg, fieldsCfg, QString, WidgetTypes::NoWidget)	/* похоже, не используется */
//    QString, animation, animation

class SUserSettings : public SSettingsBase
{
    Q_OBJECT
signals:
    void fontSizeChanged();
    void rowHeightChanged();
public:
    enum Table{Users = 0, UsersParams = 0x200};
    enum XlsColumns{ColumnA = 0, ColumnB, ColumnC, ColumnD, ColumnE, ColumnF, ColumnG, ColumnH, ColumnI, ColumnJ, ColumnK, ColumnL, ColumnM, ColumnN, ColumnO, NoColumn = 99};
    Q_ENUM(XlsColumns)
    explicit SUserSettings();
    ~SUserSettings();
#include "ssettings_init.h"
public:
    void load() override;
    void load(const QString &username);
    void save() override;
    void updateLoginTimestamp();
    void updateActivityTimestamp();
protected:
private:
    QMap<int, int> m_params;    // соответствие id поля класса id параметра в таблице `user_params`; используется при записи изменений
    QStringList m_printersList;
    int i_primaryKeyIndex = 0;
    std::unique_ptr<QMap<int, int>> m_classDbMap;       // соответствие индексов полей класса полям таблицы
    std::shared_ptr<QSqlRecord> m_record_settings;      // данные таблицы `users_params` (m_record содержит данные таблицы `users`)
    SStandardItemModel *xlsColumnsList = nullptr;
    void configureWidgets() override;
    QString table() override;
    QString dbFieldName(const int classIndex) const override;
    void loadPermissions();
    void loadFromUsersParams();
    void configureWidgetsForLocalSettings();
    QVariant loadedValue(const int dbTableIndex) const override;
    void updateJson(const int index, const QVariant &value);
    int dbFieldIndex(const int classIndex) const override;
    void insertNewField(const int index, const QString &name) override;
    int primaryKeyIndex() override;
    QVariant primaryKeyData() override;
    void deserializeData();
    void loadFromJson(const int index, bool init = 0);
    void updateWorkspaceSwitch();
    void updateWorkspaceField();
    int targetTable(const int targetTableIndex) override;
    void saveToUsersParams();
    void savePrinterSettings();
    void translate();
protected slots:
    void dataChangeHandler(const int index, const QVariant &data) override;
    void setComboBoxModel(const int index, SSqlQueryModel* model);
};

inline int SUserSettings::primaryKeyIndex()
{
    return i_primaryKeyIndex;
}

inline QVariant SUserSettings::primaryKeyData()
{
    return data(i_primaryKeyIndex);
}

#endif // SUSERSETTINGS_H
