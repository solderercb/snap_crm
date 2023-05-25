#include "susermodel.h"

SUserModel::SUserModel(QObject *parent) : SComRecord(parent)
{
    i_tableName = "users";
    i_obligatoryFields << "username" << "name" << "office" << "photo" << "fields_cfg" << "notes" << "signature";
    i_idColumnName = "id";
    i_logRecord->setType(SLogRecordModel::User);
}

SUserModel::~SUserModel()
{
    if(m_clientModel)
        delete m_clientModel;
}

void SUserModel::load(const int id)
{
    if(!id)
        return;

    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QUERY_SEL_USER_DATA2(id));
    if(!record->first())
        return;

    i_id = id;

    m_sipId = record->value("sip_user_id").toInt();
    m_clientUserId = record->value("client_user_id").toInt();
    if(m_clientUserId)
        m_clientModel = new SClientModel(m_clientUserId, this);
    else
        m_clientModel = nullptr;
    m_username = record->value("username").toString();
    m_firstName = record->value("name").toString();
    m_lastName = record->value("surname").toString();
    m_patronymicName = record->value("patronymic").toString();
    m_phone = record->value("phone").toString();
    m_phone2 = record->value("phone2").toString();
    m_phoneMask = record->value("phone_mask").toInt();
    m_phone2Mask = record->value("phone2_mask").toInt();
    m_address = record->value("address").toString();
    m_passportNum = record->value("passport_num").toString();
    m_passportDate = record->value("passport_date").toDate();
    m_passportOrgan = record->value("passport_organ").toString();
    m_state = record->value("state").toInt();
    i_createdUtc = record->value("created").toDateTime();
    m_office = record->value("office").toInt();
    m_currentOffice = m_office; // по умолчанию значение, настроенное администратором
    m_birthday = record->value("birthday").toDateTime();
    m_defOffice = record->value("def_office").toInt();
    m_defStore = record->value("def_store").toInt();
    m_defItemState = record->value("def_item_state").toInt();
    m_defEmployee = record->value("def_employee").toInt();
    m_defStatus = record->value("def_status").toInt();
    m_defWsFilter = record->value("def_ws_filter").toInt();
    m_lastLogin = record->value("last_login").toDateTime();
    m_lastActivity = record->value("last_activity").toDateTime();
    m_email = record->value("email").toString();
    m_sex = record->value("sex").toInt();
    m_photo = record->value("photo").toByteArray();
//    m_salaryRate = record->value("salary_rate").toInt();
    m_payDay = record->value("pay_day").toInt();
    m_payDayOff = record->value("pay_day_off").toInt();
    m_payRepair = record->value("pay_repair").toInt();
    m_payRepairQuick = record->value("pay_repair_quick").toInt();
    m_paySale = record->value("pay_sale").toInt();
    m_payRepairQSale = record->value("pay_repair_q_sale").toInt();
    m_payCartridgeRefill = record->value("pay_cartridge_refill").toInt();
    m_payDeviceIn = record->value("pay_device_in").toInt();
    m_payDeviceOut = record->value("pay_device_out").toInt();
    m_pay4SaleInRepair = record->value("pay_4_sale_in_repair").toBool();
    m_rowColor = record->value("row_color").toString();
    m_geHighlightColor = record->value("ge_highlight_color").toString();
    m_colorLabelWs = record->value("color_label_ws").toString();
    m_workspaceMode = record->value("workspace_mode").toInt();
    m_previewBeforePrint = record->value("preview_before_print").toBool();
    m_newRepDocCopies = record->value("new_rep_doc_copies").toInt();
    m_autoRefreshWorkspace = record->value("auto_refresh_workspace").toBool();
    m_refreshTime = record->value("refresh_time").toInt();
    m_xlsC1 = record->value("xls_c1").toInt();
    m_xlsC2 = record->value("xls_c2").toInt();
    m_xlsC3 = record->value("xls_c3").toInt();
    m_xlsC4 = record->value("xls_c4").toInt();
    m_xlsC5 = record->value("xls_c5").toInt();
    m_xlsC6 = record->value("xls_c6").toInt();
    m_xlsC7 = record->value("xls_c7").toInt();
    m_xlsC8 = record->value("xls_c8").toInt();
    m_xlsC9 = record->value("xls_c9").toInt();
    m_xlsC10 = record->value("xls_c10").toInt();
    m_xlsC11 = record->value("xls_c11").toInt();
    m_xlsC12 = record->value("xls_c12").toInt();
    m_xlsC13 = record->value("xls_c13").toInt();
    m_xlsC14 = record->value("xls_c14").toInt();
    m_xlsC15 = record->value("xls_c15").toInt();
    m_displayOut = record->value("display_out").toBool();
    m_displayComplete = record->value("display_complete").toBool();
    m_isBot = record->value("is_bot").toBool();
    m_newOnTop = record->value("new_on_top").toBool();
    m_issuedColor = record->value("issued_color").toString();
    m_fieldsCfg = record->value("fields_cfg").toString();
    m_saveStateOnClose = record->value("save_state_on_close").toBool();
    m_groupStoreItems = record->value("group_store_items").toBool();
    m_trackActivity = record->value("track_activity").toBool();
    m_cardOnCall = record->value("card_on_call").toBool();
    m_inn = record->value("inn").toString();
    m_informComment = record->value("inform_comment").toBool();
    m_informStatus = record->value("inform_status").toBool();
    m_kkt = record->value("kkt").toInt();
    m_pinpad = record->value("pinpad").toInt();
    m_advanceDisable = record->value("advance_disable").toBool();
    m_salaryDisable = record->value("salary_disable").toBool();
    m_notes = record->value("notes").toString();
    m_signature = record->value("signature").toString();
    m_kkmPwd = record->value("kkm_pass").toInt();
    m_preferRegular = record->value("prefer_regular").toBool();
    m_fontSize = record->value("fontsize").toInt();
    m_rowHeight = record->value("rowheight").toInt();
    m_animation = record->value("animation").toString();

    delete record;

    loadSalaryRate();

    emit sigModelReset();
}

void SUserModel::setSalaryRateStartDate(const QDate date)
{
    m_salaryRateStartDate = date;
}

int SUserModel::salaryRate()
{
    return m_salaryRate;
}

void SUserModel::loadSalaryRate()
{
    m_salaryRate = 0;

    if(m_salaryRateStartDate.isNull())
        return;

    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString("SELECT * FROM `salary_rates` WHERE `user_id` = %1 AND `start_from` <= '%2' ORDER BY `created_at` DESC LIMIT 1;").arg(i_id).arg(m_salaryRateStartDate.toString("yyyy-MM-dd")));
    if(!record->first())
        return;

    m_salaryRate = record->value("value").toDouble();
    m_salaryRateStartDate = QDate();

    delete record;
}

void SUserModel::setSalaryRate(double rate)
{
    i_valuesMap.insert("salary_rate", rate);
}

double SUserModel::balance()
{
    if(!m_clientModel)
        return 0;

    return m_clientModel->balance();
}

int SUserModel::clientUserId()
{
    return m_clientUserId;
}

void SUserModel::setClientUserId(const int id)
{
    m_clientUserId = id;
    i_valuesMap.insert("client_user_id", id);
}

SClientModel *SUserModel::clientModel()
{
    return m_clientModel;
}

void SUserModel::setClientModel(SClientModel *model)
{
    m_clientModel = model;
}

bool SUserModel::commit()
{
    if(i_id)
    {
        if(!update())
            throw 1;
    }
    else
    {
        if(!i_valuesMap.contains("created"))
            i_valuesMap.insert("created", QDateTime::currentDateTime());

        if(!insert())
            throw 1;
    }

    i_logRecord->setClient(i_id);
    if(!commitLogs())
        throw 1;

    return i_nErr;
}

int SUserModel::id()
{
    return i_id;
}

int SUserModel::sipId()
{
    return m_sipId;
}

void SUserModel::setSipId(const int sip_user_id)
{
    i_valuesMap.insert("sip_user_id", sip_user_id);
}

QString SUserModel::username()
{
    return m_username;
}

void SUserModel::setUsername(const QString username)
{
    i_valuesMap.insert("username", username);
}

QString SUserModel::name()
{
    return m_firstName;
}

void SUserModel::setName(const QString name)
{
    i_valuesMap.insert("name", name);
}

QString SUserModel::surname()
{
    return m_lastName;
}

void SUserModel::setSurname(const QString surname)
{
    i_valuesMap.insert("surname", surname);
}

QString SUserModel::patronymic()
{
    return m_patronymicName;
}

void SUserModel::setPatronymic(const QString patronymic)
{
    i_valuesMap.insert("patronymic", patronymic);
}

QString SUserModel::fullLongName()
{
    QString ret;
    ret = m_lastName;
    if(!m_firstName.isEmpty())
        ret.append(" " + m_firstName);
    if(!m_patronymicName.isEmpty())
        ret.append(" " + m_patronymicName);

    return ret;
}

QString SUserModel::fullShortName()
{
    QString ret;
    ret = m_lastName;
    if(!m_firstName.isEmpty())
        ret.append(" ").append(m_firstName.front()).append(".");
    if(!m_patronymicName.isEmpty())
        ret.append(" ").append(m_patronymicName.front()).append(".");

    return ret;
}

QString SUserModel::phone()
{
    return m_phone;
}

void SUserModel::setPhone(const QString phone)
{
    i_valuesMap.insert("phone", phone);
}

QString SUserModel::phone2()
{
    return m_phone2;
}

void SUserModel::setPhone2(const QString phone2)
{
    i_valuesMap.insert("phone2", phone2);
}

int SUserModel::phoneMask()
{
    return m_phoneMask;
}

void SUserModel::setPhoneMask(const int phone_mask)
{
    i_valuesMap.insert("phone_mask", phone_mask);
}

int SUserModel::phone2Mask()
{
    return m_phone2Mask;
}

void SUserModel::setPhone2Mask(const int phone2_mask)
{
    i_valuesMap.insert("phone2_mask", phone2_mask);
}

QString SUserModel::address()
{
    return m_address;
}

void SUserModel::setAddress(const QString address)
{
    i_valuesMap.insert("address", address);
}

QString SUserModel::passportNum()
{
    return m_passportNum;
}

void SUserModel::setPassportNum(const QString passport_num)
{
    i_valuesMap.insert("passport_num", passport_num);
}

QDate SUserModel::passportDate()
{
    return m_passportDate;
}

void SUserModel::setPassportDate(const QDate passport_date)
{
    i_valuesMap.insert("passport_date", passport_date);
}

QString SUserModel::passportOrgan()
{
    return m_passportOrgan;
}

void SUserModel::setPassportOrgan(const QString passport_organ)
{
    i_valuesMap.insert("passport_organ", passport_organ);
}

int SUserModel::state()
{
    return m_state;
}

void SUserModel::setState(const int state)
{
    i_valuesMap.insert("state", state);
}

QDateTime SUserModel::created()
{
    return m_created;
}

void SUserModel::setCreated(const QDateTime created)
{
    i_valuesMap.insert("created", created);
}

int SUserModel::office()
{
    return m_office;
}

int SUserModel::currentOffice()
{
    return m_currentOffice;
}

void SUserModel::setCurrentOffice(const int id)
{
    m_currentOffice = id;
}

int SUserModel::company()
{
    return m_company;
}

void SUserModel::setCompany(const int id)
{
    m_company = id;
}

void SUserModel::setOffice(const int office)
{
    i_valuesMap.insert("office", office);
}

QDateTime SUserModel::birthday()
{
    return m_birthday;
}

void SUserModel::setBirthday(const QDateTime birthday)
{
    i_valuesMap.insert("birthday", birthday);
}

int SUserModel::defOffice()
{
    return m_defOffice;
}

void SUserModel::setDefOffice(const int def_office)
{
    i_valuesMap.insert("def_office", def_office);
}

int SUserModel::defStore()
{
    return m_defStore;
}

void SUserModel::setDefStore(const int def_store)
{
    i_valuesMap.insert("def_store", def_store);
}

int SUserModel::defItemState()
{
    return m_defItemState;
}

void SUserModel::setDefItemState(const int def_item_state)
{
    i_valuesMap.insert("def_item_state", def_item_state);
}

int SUserModel::defEmployee()
{
    return m_defEmployee;
}

void SUserModel::setDefEmployee(const int def_employee)
{
    i_valuesMap.insert("def_employee", def_employee);
}

int SUserModel::defStatus()
{
    return m_defStatus;
}

void SUserModel::setDefStatus(const int def_status)
{
    i_valuesMap.insert("def_status", def_status);
}

int SUserModel::defWsFilter()
{
    return m_defWsFilter;
}

void SUserModel::setDefWsFilter(const int def_ws_filter)
{
    i_valuesMap.insert("def_ws_filter", def_ws_filter);
}

QDateTime SUserModel::lastLogin()
{
    return m_lastLogin;
}

void SUserModel::setLastLogin(const QDateTime last_login)
{
    i_valuesMap.insert("last_login", last_login);
}

QDateTime SUserModel::lastActivity()
{
    return m_lastActivity;
}

void SUserModel::setLastActivity(const QDateTime last_activity)
{
    i_valuesMap.insert("last_activity", last_activity);
}

QString SUserModel::email()
{
    return m_email;
}

void SUserModel::setEmail(const QString email)
{
    i_valuesMap.insert("email", email);
}

int SUserModel::sex()
{
    return m_sex;
}

void SUserModel::setSex(const int sex)
{
    i_valuesMap.insert("sex", sex);
}

QByteArray SUserModel::photo()
{
    return m_photo;
}

void SUserModel::setPhoto(const QByteArray photo)
{
    i_valuesMap.insert("photo", photo);
}

void SUserModel::setSalaryRate(const int salary_rate)
{
    i_valuesMap.insert("salary_rate", salary_rate);
}

int SUserModel::payDay()
{
    return m_payDay;
}

void SUserModel::setPayDay(const int pay_day)
{
    i_valuesMap.insert("pay_day", pay_day);
}

int SUserModel::payDayOff()
{
    return m_payDayOff;
}

void SUserModel::setPayDayOff(const int pay_day_off)
{
    i_valuesMap.insert("pay_day_off", pay_day_off);
}

int SUserModel::payRepair()
{
    return m_payRepair;
}

void SUserModel::setPayRepair(const int pay_repair)
{
    i_valuesMap.insert("pay_repair", pay_repair);
}

int SUserModel::payRepairQuick()
{
    return m_payRepairQuick;
}

void SUserModel::setPayRepairQuick(const int pay_repair_quick)
{
    i_valuesMap.insert("pay_repair_quick", pay_repair_quick);
}

int SUserModel::paySale()
{
    return m_paySale;
}

void SUserModel::setPaySale(const int pay_sale)
{
    i_valuesMap.insert("pay_sale", pay_sale);
}

int SUserModel::payRepairQSale()
{
    return m_payRepairQSale;
}

void SUserModel::setPayRepairQSale(const int pay_repair_q_sale)
{
    i_valuesMap.insert("pay_repair_q_sale", pay_repair_q_sale);
}

int SUserModel::payCartridgeRefill()
{
    return m_payCartridgeRefill;
}

void SUserModel::setPayCartridgeRefill(const int pay_cartridge_refill)
{
    i_valuesMap.insert("pay_cartridge_refill", pay_cartridge_refill);
}

int SUserModel::payDeviceIn()
{
    return m_payDeviceIn;
}

void SUserModel::setPayDeviceIn(const int pay_device_in)
{
    i_valuesMap.insert("pay_device_in", pay_device_in);
}

int SUserModel::payDeviceOut()
{
    return m_payDeviceOut;
}

void SUserModel::setPayDeviceOut(const int pay_device_out)
{
    i_valuesMap.insert("pay_device_out", pay_device_out);
}

bool SUserModel::pay4SaleInRepair()
{
    return m_pay4SaleInRepair;
}

void SUserModel::setPay4SaleInRepair(const bool pay_4_sale_in_repair)
{
    i_valuesMap.insert("pay_4_sale_in_repair", pay_4_sale_in_repair);
}

QString SUserModel::rowColor()
{
    return m_rowColor;
}

void SUserModel::setRowColor(const QString row_color)
{
    i_valuesMap.insert("row_color", row_color);
}

QString SUserModel::geHighlightColor()
{
    return m_geHighlightColor;
}

void SUserModel::setGeHighlightColor(const QString ge_highlight_color)
{
    i_valuesMap.insert("ge_highlight_color", ge_highlight_color);
}

QString SUserModel::colorLabelWs()
{
    return m_colorLabelWs;
}

void SUserModel::setColorLabelWs(const QString color_label_ws)
{
    i_valuesMap.insert("color_label_ws", color_label_ws);
}

int SUserModel::workspaceMode()
{
    return m_workspaceMode;
}

void SUserModel::setWorkspaceMode(const int workspace_mode)
{
    i_valuesMap.insert("workspace_mode", workspace_mode);
}

bool SUserModel::previewBeforePrint()
{
    return m_previewBeforePrint;
}

void SUserModel::setPreviewBeforePrint(const bool preview_before_print)
{
    i_valuesMap.insert("preview_before_print", preview_before_print);
}

int SUserModel::newRepDocCopies()
{
    return m_newRepDocCopies;
}

void SUserModel::setNewRepDocCopies(const int new_rep_doc_copies)
{
    i_valuesMap.insert("new_rep_doc_copies", new_rep_doc_copies);
}

bool SUserModel::autoRefreshWorkspace()
{
    return m_autoRefreshWorkspace;
}

void SUserModel::setAutoRefreshWorkspace(const bool auto_refresh_workspace)
{
    i_valuesMap.insert("auto_refresh_workspace", auto_refresh_workspace);
}

int SUserModel::refreshTime()
{
    return m_refreshTime;
}

void SUserModel::setRefreshTime(const int refresh_time)
{
    i_valuesMap.insert("refresh_time", refresh_time);
}

int SUserModel::xlsC1()
{
    return m_xlsC1;
}

void SUserModel::setXlsC1(const int xls_c1)
{
    i_valuesMap.insert("xls_c1", xls_c1);
}

int SUserModel::xlsC2()
{
    return m_xlsC2;
}

void SUserModel::setXlsC2(const int xls_c2)
{
    i_valuesMap.insert("xls_c2", xls_c2);
}

int SUserModel::xlsC3()
{
    return m_xlsC3;
}

void SUserModel::setXlsC3(const int xls_c3)
{
    i_valuesMap.insert("xls_c3", xls_c3);
}

int SUserModel::xlsC4()
{
    return m_xlsC4;
}

void SUserModel::setXlsC4(const int xls_c4)
{
    i_valuesMap.insert("xls_c4", xls_c4);
}

int SUserModel::xlsC5()
{
    return m_xlsC5;
}

void SUserModel::setXlsC5(const int xls_c5)
{
    i_valuesMap.insert("xls_c5", xls_c5);
}

int SUserModel::xlsC6()
{
    return m_xlsC6;
}

void SUserModel::setXlsC6(const int xls_c6)
{
    i_valuesMap.insert("xls_c6", xls_c6);
}

int SUserModel::xlsC7()
{
    return m_xlsC7;
}

void SUserModel::setXlsC7(const int xls_c7)
{
    i_valuesMap.insert("xls_c7", xls_c7);
}

int SUserModel::xlsC8()
{
    return m_xlsC8;
}

void SUserModel::setXlsC8(const int xls_c8)
{
    i_valuesMap.insert("xls_c8", xls_c8);
}

int SUserModel::xlsC9()
{
    return m_xlsC9;
}

void SUserModel::setXlsC9(const int xls_c9)
{
    i_valuesMap.insert("xls_c9", xls_c9);
}

int SUserModel::xlsC10()
{
    return m_xlsC10;
}

void SUserModel::setXlsC10(const int xls_c10)
{
    i_valuesMap.insert("xls_c10", xls_c10);
}

int SUserModel::xlsC11()
{
    return m_xlsC11;
}

void SUserModel::setXlsC11(const int xls_c11)
{
    i_valuesMap.insert("xls_c11", xls_c11);
}

int SUserModel::xlsC12()
{
    return m_xlsC12;
}

void SUserModel::setXlsC12(const int xls_c12)
{
    i_valuesMap.insert("xls_c12", xls_c12);
}

int SUserModel::xlsC13()
{
    return m_xlsC13;
}

void SUserModel::setXlsC13(const int xls_c13)
{
    i_valuesMap.insert("xls_c13", xls_c13);
}

int SUserModel::xlsC14()
{
    return m_xlsC14;
}

void SUserModel::setXlsC14(const int xls_c14)
{
    i_valuesMap.insert("xls_c14", xls_c14);
}

int SUserModel::xlsC15()
{
    return m_xlsC15;
}

void SUserModel::setXlsC15(const int xls_c15)
{
    i_valuesMap.insert("xls_c15", xls_c15);
}

bool SUserModel::displayOut()
{
    return m_displayOut;
}

void SUserModel::setDisplayOut(const bool display_out)
{
    i_valuesMap.insert("display_out", display_out);
}

bool SUserModel::displayComplete()
{
    return m_displayComplete;
}

void SUserModel::setDisplayComplete(const bool display_complete)
{
    i_valuesMap.insert("display_complete", display_complete);
}

bool SUserModel::isBot()
{
    return m_isBot;
}

void SUserModel::setIsBot(const bool is_bot)
{
    i_valuesMap.insert("is_bot", is_bot);
}

bool SUserModel::newOnTop()
{
    return m_newOnTop;
}

void SUserModel::setNewOnTop(const bool new_on_top)
{
    i_valuesMap.insert("new_on_top", new_on_top);
}

QString SUserModel::issuedColor()
{
    return m_issuedColor;
}

void SUserModel::setIssuedColor(const QString issued_color)
{
    i_valuesMap.insert("issued_color", issued_color);
}

QString SUserModel::fieldsCfg()
{
    return m_fieldsCfg;
}

void SUserModel::setFieldsCfg(const QString fields_cfg)
{
    i_valuesMap.insert("fields_cfg", fields_cfg);
}

bool SUserModel::saveStateOnClose()
{
    return m_saveStateOnClose;
}

void SUserModel::setSaveStateOnClose(const bool save_state_on_close)
{
    i_valuesMap.insert("save_state_on_close", save_state_on_close);
}

bool SUserModel::groupStoreItems()
{
    return m_groupStoreItems;
}

void SUserModel::setGroupStoreItems(const bool group_store_items)
{
    i_valuesMap.insert("group_store_items", group_store_items);
}

bool SUserModel::trackActivity()
{
    return m_trackActivity;
}

void SUserModel::setTrackActivity(const bool track_activity)
{
    i_valuesMap.insert("track_activity", track_activity);
}

bool SUserModel::cardOnCall()
{
    return m_cardOnCall;
}

void SUserModel::setCardOnCall(const bool card_on_call)
{
    i_valuesMap.insert("card_on_call", card_on_call);
}

QString SUserModel::inn()
{
    return m_inn;
}

void SUserModel::setInn(const QString inn)
{
    i_valuesMap.insert("inn", inn);
}

bool SUserModel::informComment()
{
    return m_informComment;
}

void SUserModel::setInformComment(const bool inform_comment)
{
    i_valuesMap.insert("inform_comment", inform_comment);
}

bool SUserModel::informStatus()
{
    return m_informStatus;
}

void SUserModel::setInformStatus(const bool inform_status)
{
    i_valuesMap.insert("inform_status", inform_status);
}

int SUserModel::kkt()
{
    return m_kkt;
}

void SUserModel::setKkt(const int kkt)
{
    i_valuesMap.insert("kkt", kkt);
}

int SUserModel::pinpad()
{
    return m_pinpad;
}

void SUserModel::setPinpad(const int pinpad)
{
    i_valuesMap.insert("pinpad", pinpad);
}

bool SUserModel::advanceDisable()
{
    return m_advanceDisable;
}

void SUserModel::setAdvanceDisable(const bool advance_disable)
{
    i_valuesMap.insert("advance_disable", advance_disable);
}

bool SUserModel::salaryDisable()
{
    return m_salaryDisable;
}

void SUserModel::setSalaryDisable(const bool salary_disable)
{
    i_valuesMap.insert("salary_disable", salary_disable);
}

QString SUserModel::notes()
{
    return m_notes;
}

void SUserModel::setNotes(const QString notes)
{
    i_valuesMap.insert("notes", notes);
}

QString SUserModel::signature()
{
    return m_signature;
}

void SUserModel::setSignature(const QString signature)
{
    i_valuesMap.insert("signature", signature);
}

int SUserModel::kkmPwd()
{
    return m_kkmPwd;
}

void SUserModel::setKkmPwd(const int kkm_pass)
{
    i_valuesMap.insert("kkm_pass", kkm_pass);
}

bool SUserModel::preferRegular()
{
    return m_preferRegular;
}

void SUserModel::setPreferRegular(const bool prefer_regular)
{
    i_valuesMap.insert("prefer_regular", prefer_regular);
}

int SUserModel::fontSize()
{
    return m_fontSize;
}

void SUserModel::setFontSize(const int fontsize)
{
    i_valuesMap.insert("fontsize", fontsize);
}

int SUserModel::rowHeight()
{
    return m_rowHeight;
}

void SUserModel::setRowHeight(const int rowheight)
{
    i_valuesMap.insert("rowheight", rowheight);
}

QString SUserModel::animation()
{
    return m_animation;
}

void SUserModel::setAnimation(const QString animation)
{
    i_valuesMap.insert("animation", animation);
}

