#ifndef SUSERMODEL_H
#define SUSERMODEL_H

#include "scomrecord.h"
#include <QObject>
#include "models/sclientmodel.h"

class SUserModel : public SComRecord
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(int sipId READ sipId)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString surname READ surname)
    Q_PROPERTY(QString patronymic READ patronymic)
    Q_PROPERTY(QString fullLongName READ fullLongName)
    Q_PROPERTY(QString fullShortName READ fullShortName)
    Q_PROPERTY(QString phone READ phone)
    Q_PROPERTY(QString phone2 READ phone2)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(QDateTime created READ created)
    Q_PROPERTY(QString email READ email)
    Q_PROPERTY(QByteArray photo READ photo)
    Q_PROPERTY(QString inn READ inn)
    Q_PROPERTY(QString notes READ notes)
signals:
    void sigModelReset();
public:
    explicit SUserModel(QObject *parent = nullptr);
    ~SUserModel();
    void load(const int id);
    int id();
    int sipId();
    void setSipId(const int);//sip_user_id
    QString username();
    void setUsername(const QString);//username
    QString name();
    void setName(const QString);//name
    QString surname();
    void setSurname(const QString);//surname
    QString patronymic();
    void setPatronymic(const QString);//patronymic
    QString fullLongName();   // Фамилия Имя Отчество
    QString fullShortName();  // Фамилия И. О.
    QString phone();
    void setPhone(const QString);//phone
    QString phone2();
    void setPhone2(const QString);//phone2
    int phoneMask();
    void setPhoneMask(const int);//phone_mask
    int phone2Mask();
    void setPhone2Mask(const int);//phone2_mask
    QString address();
    void setAddress(const QString);//address
    QString passportNum();
    void setPassportNum(const QString);//passport_num
    QDate passportDate();
    void setPassportDate(const QDate);//passport_date
    QString passportOrgan();
    void setPassportOrgan(const QString);//passport_organ
    int state();
    void setState(const int);//state
    QDateTime created();
    void setCreated(const QDateTime);//created
    int office();
    void setOffice(const int);//office
    int currentOffice();
    void setCurrentOffice(const int id);
    int company();
    void setCompany(const int id);
//    int currentCompany();
//    void setCurrentCompany(const int id);
    QDateTime birthday();
    void setBirthday(const QDateTime);//birthday
    int defOffice();
    void setDefOffice(const int);//def_office
    int defStore();
    void setDefStore(const int);//def_store
    int defItemState();
    void setDefItemState(const int);//def_item_state
    int defEmployee();
    void setDefEmployee(const int);//def_employee
    int defStatus();
    void setDefStatus(const int);//def_status
    int defWsFilter();
    void setDefWsFilter(const int);//def_ws_filter
    QDateTime lastLogin();
    void setLastLogin(const QDateTime);//last_login
    QDateTime lastActivity();
    void setLastActivity(const QDateTime);//last_activity
    QString email();
    void setEmail(const QString);//email
    int sex();
    void setSex(const int);//sex
    QByteArray photo();
    void setPhoto(const QByteArray);//photo
    int salaryRate();
    void setSalaryRate(const int);//salary_rate
    int payDay();
    void setPayDay(const int);//pay_day
    int payDayOff();
    void setPayDayOff(const int);//pay_day_off
    int payRepair();
    void setPayRepair(const int);//pay_repair
    int payRepairQuick();
    void setPayRepairQuick(const int);//pay_repair_quick
    int paySale();
    void setPaySale(const int);//pay_sale
    int payRepairQSale();
    void setPayRepairQSale(const int);//pay_repair_q_sale
    int payCartridgeRefill();
    void setPayCartridgeRefill(const int);//pay_cartridge_refill
    int payDeviceIn();
    void setPayDeviceIn(const int);//pay_device_in
    int payDeviceOut();
    void setPayDeviceOut(const int);//pay_device_out
    bool pay4SaleInRepair();
    void setPay4SaleInRepair(const bool);//pay_4_sale_in_repair
    QString rowColor();
    void setRowColor(const QString);//row_color
    QString geHighlightColor();
    void setGeHighlightColor(const QString);//ge_highlight_color
    QString colorLabelWs();
    void setColorLabelWs(const QString);//color_label_ws
    int workspaceMode();
    void setWorkspaceMode(const int);//workspace_mode
    bool previewBeforePrint();
    void setPreviewBeforePrint(const bool);//preview_before_print
    int newRepDocCopies();
    void setNewRepDocCopies(const int);//new_rep_doc_copies
    bool autoRefreshWorkspace();
    void setAutoRefreshWorkspace(const bool);//auto_refresh_workspace
    int refreshTime();
    void setRefreshTime(const int);//refresh_time
    int xlsC1();
    void setXlsC1(const int);//xls_c1
    int xlsC2();
    void setXlsC2(const int);//xls_c2
    int xlsC3();
    void setXlsC3(const int);//xls_c3
    int xlsC4();
    void setXlsC4(const int);//xls_c4
    int xlsC5();
    void setXlsC5(const int);//xls_c5
    int xlsC6();
    void setXlsC6(const int);//xls_c6
    int xlsC7();
    void setXlsC7(const int);//xls_c7
    int xlsC8();
    void setXlsC8(const int);//xls_c8
    int xlsC9();
    void setXlsC9(const int);//xls_c9
    int xlsC10();
    void setXlsC10(const int);//xls_c10
    int xlsC11();
    void setXlsC11(const int);//xls_c11
    int xlsC12();
    void setXlsC12(const int);//xls_c12
    int xlsC13();
    void setXlsC13(const int);//xls_c13
    int xlsC14();
    void setXlsC14(const int);//xls_c14
    int xlsC15();
    void setXlsC15(const int);//xls_c15
    bool displayOut();
    void setDisplayOut(const bool);//display_out
    bool displayComplete();
    void setDisplayComplete(const bool);//display_complete
    bool isBot();
    void setIsBot(const bool);//is_bot
    bool newOnTop();
    void setNewOnTop(const bool);//new_on_top
    QString issuedColor();
    void setIssuedColor(const QString);//issued_color
    QString fieldsCfg();
    void setFieldsCfg(const QString);//fields_cfg
    bool saveStateOnClose();
    void setSaveStateOnClose(const bool);//save_state_on_close
    bool groupStoreItems();
    void setGroupStoreItems(const bool);//group_store_items
    bool trackActivity();
    void setTrackActivity(const bool);//track_activity
    bool cardOnCall();
    void setCardOnCall(const bool);//card_on_call
    QString inn();
    void setInn(const QString);//inn
    bool informComment();
    void setInformComment(const bool);//inform_comment
    bool informStatus();
    void setInformStatus(const bool);//inform_status
    int kkt();
    void setKkt(const int);//kkt
    int pinpad();
    void setPinpad(const int);//pinpad
    bool advanceDisable();
    void setAdvanceDisable(const bool);//advance_disable
    bool salaryDisable();
    void setSalaryDisable(const bool);//salary_disable
    QString notes();
    void setNotes(const QString);//notes
    QString signature();
    void setSignature(const QString);//signature
    int kkmPwd();
    void setKkmPwd(const int);//kkm_pass
    bool preferRegular();
    void setPreferRegular(const bool);//prefer_regular
    int fontSize();
    void setFontSize(const int);//fontsize
    int rowHeight();
    void setRowHeight(const int);//rowheight
    QString animation();
    void setAnimation(const QString);//animation
    void loadSalaryRate();
    void setSalaryRate(double rate);
    void setSalaryRateStartDate(const QDate date);
    double balance();
    int clientUserId();
    void setClientUserId(const int id);
    int m_clientUserId = 0;
    SClientModel* clientModel();
    void setClientModel(SClientModel *model);
    bool commit();
private:
    int m_sipId;
    QString m_username;
    QString m_firstName;
    QString m_lastName;
    QString m_patronymicName;
    QString m_phone;
    QString m_phone2;
    int m_phoneMask;
    int m_phone2Mask;
    QString m_address;
    QString m_passportNum;
    QDate m_passportDate;
    QString m_passportOrgan;
    int m_state;
    QDateTime m_created;
    int m_office;
    int m_currentOffice;
    int m_company = 1;
//    int m_currentCompany;
    QDateTime m_birthday;
    int m_defOffice;
    int m_defStore;
    int m_defItemState;
    int m_defEmployee;
    int m_defStatus;
    int m_defWsFilter;
    QDateTime m_lastLogin;
    QDateTime m_lastActivity;
    QString m_email;
    int m_sex;
    QByteArray m_photo;
    int m_salaryRate;
    int m_payDay;
    int m_payDayOff;
    int m_payRepair;
    int m_payRepairQuick;
    int m_paySale;
    int m_payRepairQSale;
    int m_payCartridgeRefill;
    int m_payDeviceIn;
    int m_payDeviceOut;
    bool m_pay4SaleInRepair;
    QString m_rowColor;
    QString m_geHighlightColor;
    QString m_colorLabelWs;
    int m_workspaceMode;
    bool m_previewBeforePrint;
    int m_newRepDocCopies;
    bool m_autoRefreshWorkspace;
    int m_refreshTime;
    int m_xlsC1;
    int m_xlsC2;
    int m_xlsC3;
    int m_xlsC4;
    int m_xlsC5;
    int m_xlsC6;
    int m_xlsC7;
    int m_xlsC8;
    int m_xlsC9;
    int m_xlsC10;
    int m_xlsC11;
    int m_xlsC12;
    int m_xlsC13;
    int m_xlsC14;
    int m_xlsC15;
    bool m_displayOut;
    bool m_displayComplete;
    bool m_isBot;
    bool m_newOnTop;
    QString m_issuedColor;
    QString m_fieldsCfg;
    bool m_saveStateOnClose;
    bool m_groupStoreItems;
    bool m_trackActivity;
    bool m_cardOnCall;
    QString m_inn;
    bool m_informComment;
    bool m_informStatus;
    int m_kkt;
    int m_pinpad;
    bool m_advanceDisable;
    bool m_salaryDisable;
    QString m_notes;
    QString m_signature;
    int m_kkmPwd;
    bool m_preferRegular;
    int m_fontSize;
    int m_rowHeight;
    QString m_animation;
    QDate m_salaryRateStartDate;
    SClientModel* m_clientModel = nullptr;
};

#endif // SUSERMODEL_H
