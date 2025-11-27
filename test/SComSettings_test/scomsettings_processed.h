#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <memory>
#include <type_traits>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QDateTimeEdit>
#include <QLabel>
#include <SComSettingsTypes>
#include <LimeReport>
#include <SCheckComboBox>





class SSingleRowModelBase
{
    friend class TClassTest;
public:
    class ModifiedField;
    int size() const;
    bool isIndexValid(int index) const;
    virtual QVariant data(const int index, const int state = (ModifiedField::Updated|ModifiedField::Executed|ModifiedField::Commited)) const;
    std::optional<QVariant> commitedData(const int &index) const;
    virtual bool setData(const int &index, const QVariant &data);
    virtual void signalDataToBeChanged(const int index, QVariant &data) = 0;
    virtual void signalDataChanged(const int index, QVariant &data) = 0;
    bool isFieldDirty(const int index);
    virtual bool isDirty();
    static QDate localDate(const QDate &utcDate);
    static QDate localDate(const QVariant &utcDate);
    static QDate utcDate(const QDate &localDate);
    static QDate utcDate(const QVariant &localDate);
    static QDateTime localDateTime(const QDateTime &utcDateTime);
    static QDateTime localDateTime(const QVariant &utcDateTime);
    static QDateTime utcDateTime(QDateTime localDateTime);
    static QDateTime utcDateTime(const QVariant &localDateTime);
    static QString fieldValueHandler(const QVariant &value);
    virtual void load();

    class ModifiedField{
        friend class TClassTest;
    public:
        enum State {None = 1, Updated = 2, Executed = 4, Failed = 8, Commited = 16};
        inline ModifiedField(State s = None, std::shared_ptr<ModifiedField> prev = nullptr) :
            m_state(None), m_prev_data(prev)
        {
            Q_ASSERT_X(s != Executed, "ModifiedField", "creating an object with such status makes no sense");
            setState(s);
        }
        inline State state() const {
            return m_state;
        }
        inline void setState(State s){
            if(m_state == s)
                return;
            m_state = s;
        }
        inline QVariant data() const {
            return m_data;
        }
        inline void setData(QVariant data){
            m_data = data;
        }
        inline std::shared_ptr<ModifiedField>& prev(){
            return m_prev_data;
        }
    private:
        State m_state = None;
        QVariant m_data;
        std::shared_ptr<ModifiedField> m_prev_data;
    };

    typedef QMap<int, std::shared_ptr<ModifiedField>> CacheMap;
    virtual void setState(const int index, ModifiedField::State state);
    virtual void setAllState(ModifiedField::State state);
    virtual void setFieldsCommited();
    virtual void setFieldsFailed();
protected:
    int sz = 0;
    std::unique_ptr<QSqlQuery> i_query;
    std::shared_ptr<QSqlDatabase> m_database;
    std::shared_ptr<QSqlRecord> m_record;
    std::unique_ptr<QMap<int, QString>> m_namesMap;
    std::unique_ptr<QHash<QString, QString>> m_reportFieldsMap;
    CacheMap cache;
    virtual void insertNewField(const int index, const QString &name);
    int fieldIndex(const QString &name) const;
    QString fieldName(const int index) const;
    virtual int indexOfCreated();
    virtual int dbFieldIndex(const int classIndex) const;
    virtual QString dbFieldName(const int classIndex) const;
    std::shared_ptr<QSqlDatabase> database() const;
    virtual void setDatabase(const QSqlDatabase &database);
    void initSqlQuery();
    virtual void initSqlQuery(QSqlDatabase database);
    QString fieldForSelectQuery(const int index);
    QString restrictedFieldForSelectQuery(const int index);
    virtual QString fieldsForSelectQuery();
    QString wrapSelectQueryFields(const QString &fields);
    virtual QString constructSelectQuery();
    virtual void loadError(const int type);
    virtual void convertTimestampFieldsToLocal();
    QString constructInsertQuery();
    QString fieldForUpdateQuery(const int index);
    QString prepareQueryUpdatePairs();
    QString wrapUpdateQueryPairs(const QString &setPairs);
    QString constructUpdateQuery();
    void setCacheData(const int index, const QVariant &value, ModifiedField::State newState = ModifiedField::Updated);
    virtual void setState(std::shared_ptr<ModifiedField> field, ModifiedField::State state);
    virtual bool skipField(CacheMap::ConstIterator f);
    bool isFieldDirty(CacheMap::ConstIterator f);
    virtual void setFieldsExecuted();
    virtual int primaryKeyIndex() = 0;
    virtual QVariant primaryKeyData() = 0;
    virtual bool isModelLoaded() const = 0;
    virtual QString table() = 0;
    virtual std::shared_ptr<QSqlRecord> rec() const = 0;
    virtual QVariant loadedValue(const int dbTableIndex) const;
    std::shared_ptr<ModifiedField>& cacheRef(const int index);
    bool isEqual(const QVariant& newData, const QVariant& oldData);
    int reportColumnCount();
    QVariant reportColumnHeader(const int index) const;
    void reportData(const int index, QVariant &data);
    void reportData(const QString &field, QVariant &data);
    virtual void reportMetaData(const QString &field, QVariant &data);
};

inline int SSingleRowModelBase::size() const
{
    return sz;
}

inline bool SSingleRowModelBase::isIndexValid(int index) const
{
    return index >= 0 && index < size();
}

inline int SSingleRowModelBase::fieldIndex(const QString &name) const
{
    return m_namesMap->key(name, -1);
}

inline QString SSingleRowModelBase::fieldName(const int index) const
{
    return m_namesMap->value(index);
}

inline int SSingleRowModelBase::dbFieldIndex(const int classIndex) const
{
    return classIndex;
}

inline QString SSingleRowModelBase::dbFieldName(const int classIndex) const
{
    return rec()->fieldName(dbFieldIndex(classIndex));
}

inline QString SSingleRowModelBase::fieldForSelectQuery(const int index)
{
    return QString("  `%1`").arg(rec()->fieldName(index));
}

inline QString SSingleRowModelBase::restrictedFieldForSelectQuery(const int index)
{
    return QString("  '%1' AS '%2'").arg(QObject::tr("no permissions"), rec()->fieldName(index));
}

inline QString SSingleRowModelBase::fieldForUpdateQuery(const int index)
{
    return QString("  `%1` = ").arg(rec()->fieldName(index));
}

inline bool SSingleRowModelBase::isFieldDirty(CacheMap::ConstIterator f)
{
    return ((*f)->state() & (ModifiedField::Updated | ModifiedField::Failed)) != 0;
}





inline QVariant SSingleRowModelBase::loadedValue(const int dbTableIndex) const
{
    return rec()->value(dbTableIndex);
}

inline int SSingleRowModelBase::reportColumnCount()
{
    return m_reportFieldsMap->count();
}

inline QVariant SSingleRowModelBase::reportColumnHeader(const int index) const
{
    return m_reportFieldsMap->keys().value(index);
}
class SSettingsBase : public QObject, public SSingleRowModelBase
{
    Q_OBJECT
    friend class TClassTest;
signals:
    void beginDataChange(const int index, QVariant &data);
    void dataChanged(const int index, QVariant &data);
public:
    enum WidgetTypes {NoWidget, CheckBox, ComboBox, SCheckableComboBox, SColorPickBox, DateTimeEdit, LineEdit, RadioButton, SpinBox, DoubleSpinBox};
    explicit SSettingsBase();
    void setPrimaryKey(const QVariant &data);
    void deleteWidgets();
    int propertyGroup(const int index) const;
    QLabel *label(const int index);
    QWidget *widget(const int index);
    void load() override;
protected:
    QMap<int, QLabel*> i_labelWidgets;
    QMap<int, QWidget*> i_editorWidgets;
    QMap<int, int> i_editorWidgetsTypes;
    QMap<int, QVariant> i_editorWidgetsDefaults;
    QMap<int, int> i_propertyGroup;
    virtual void mapFields() = 0;
    virtual void registerReportFields() = 0;
    virtual void initWidgets() = 0;
    virtual void configureWidgets(){};
    void disableWidget(const int index);
    int widgetGroup(const int index);
    void signalDataToBeChanged(const int index, QVariant &data) override;
    void signalDataChanged(const int index, QVariant &data) override;
    bool isModelLoaded() const override;
    virtual std::shared_ptr<QSqlRecord> rec() const override;
    void setComboBoxIndex(QComboBox *cb, const QVariant &dbValue);
    QVariant dbValueFromComboBox(QComboBox *cb, const int index);
    virtual void setWidgetsData();
    virtual int targetTable(const int index);
    bool skipField(CacheMap::ConstIterator f) override;
    bool skipField(CacheMap::ConstIterator f, const int targetTable);
    bool isDirty() override;
    bool isDirty(const int targetTable);
    void collectWidgetsData();
    void setCacheData(const int index, const QVariant &value, ModifiedField::State newState = ModifiedField::Updated);
    virtual void save();
    template<typename T>
    void setData(int index, const T& value){
        SSingleRowModelBase::setData(index, QVariant::fromValue(value));
    };

    template<typename... Args>
    QWidget* initLineEdit(int, Args&&...) {
        QLineEdit *w = new QLineEdit();
        return w;
    };

    template<typename T>
    QWidget* initComboBox(int index, T defaultValue) {
        QComboBox *w = new QComboBox();
        i_editorWidgetsDefaults.insert(index, defaultValue);
        return w;
    };
    template<typename... Args>
    QWidget* initComboBox(int, Args&&...) {
        QComboBox *w = new QComboBox();
        return w;
    };

    template<typename... Args>
    QWidget* initCheckableComboBox(int, Args&&...) {
        SCheckComboBox *w = new SCheckComboBox();
        return w;
    };

    template<typename... Args>
    QWidget* initColorPickBox(int, Args&&...) {


        return new QComboBox();
    };

    template<typename... Args>
    QWidget* initCheckBox(int, Args&&...) {
        QCheckBox *w = new QCheckBox();
        w->setText("");
        w->setLayoutDirection(Qt::RightToLeft);
        return w;
    };

    template<typename... Args>
    QWidget* initSpinBox(int, Args&&...) {
        Q_ASSERT_X(0, "Can't initialize QSpinBox", " wrong arguments types");
        return nullptr;
    }
    template<typename T1, typename T2, typename... Args,
             typename = std::enable_if_t<
                 std::is_same<std::decay_t<T1>, int>::value &&
                 std::is_same<std::decay_t<T2>, int>::value
             >>
    QWidget* initSpinBox(int, T1&& min, T2&& max, Args&&...) {
        QSpinBox *w = new QSpinBox();
        w->setRange(min, max);
        return w;
    };

    template<typename... Args>
    QWidget* initDSpinBox(int, Args&&...) {
        Q_ASSERT_X(0, "Can't initialize QDoubleSpinBox", " wrong arguments types");
        return nullptr;
    }
    template<typename T1, typename T2, typename... Args,
             typename = std::enable_if_t<
                 std::is_same<std::decay_t<T1>, double>::value &&
                 std::is_same<std::decay_t<T2>, double>::value
             >>
    QWidget* initDSpinBox(int, T1&& min, T2&& max, Args&&...) {
        QDoubleSpinBox *w = new QDoubleSpinBox();
        w->setRange(min, max);
        return w;
    };

    template<typename... Args>
    QWidget* initRadioButton(int, Args&&...) {
        QRadioButton *w = new QRadioButton();
        w->setText("");
        w->setLayoutDirection(Qt::RightToLeft);
        return w;
    };

    template<typename... Args>
    QWidget* initDateTimeEdit(int, Args&&...) {
        QDateTimeEdit*w = new QDateTimeEdit();
        return w;
    };

    template<typename... Args>
    void initWidget(int, const QString &, int type, Args&&...) {







    };

    template<typename... Args>
    void initWidget(int index, const QString &label, int type, int group, Args&&... args) {
      QWidget *w;
      switch(type) {
          case WidgetTypes::LineEdit: w = initLineEdit(index, std::forward<Args>(args)...); break;
          case WidgetTypes::ComboBox: w = initComboBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::SCheckableComboBox: w = initCheckableComboBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::SColorPickBox: w = initColorPickBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::CheckBox: w = initCheckBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::SpinBox: w = initSpinBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::DoubleSpinBox: w = initDSpinBox(index, std::forward<Args>(args)...); break;
          case WidgetTypes::RadioButton: w = initRadioButton(index, std::forward<Args>(args)...); break;
          case WidgetTypes::DateTimeEdit: w = initDateTimeEdit(index, std::forward<Args>(args)...); break;
          default: break;
      }

      QLabel *l = new QLabel(label);
      i_labelWidgets.insert(index, l);
      i_editorWidgets.insert(index, w);
      i_editorWidgetsTypes.insert(index, type);
      i_propertyGroup.insert(index, group);
    };

    void registerReportField(const QString &name, int widgetType);
    void reportMetaData(const QString &field, QVariant &data) override;
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
protected slots:
    virtual void dataChangeHandler(const int, const QVariant &){};
};

inline void SSettingsBase::signalDataToBeChanged(const int index, QVariant &data)
{
    emit beginDataChange(index, data);
}

inline void SSettingsBase::signalDataChanged(const int index, QVariant &data)
{
    emit dataChanged(index, data);
}

inline bool SSettingsBase::isModelLoaded() const
{
    return m_record->value(0).isValid();
}

inline std::shared_ptr<QSqlRecord> SSettingsBase::rec() const
{
    return m_record;
}




inline int SSettingsBase::targetTable(const int)
{
    return 0;
}




class SStandardItemModel;

class SComSettings : public SSettingsBase
{
    Q_OBJECT
    friend class TClassTest;
public:
    enum Table{Config = 0, Settings = 0x200};
    explicit SComSettings();




public:
    enum Fields {C_id, C_currencyId, C_classicKassa, C_timeZoneId, C_ascPhoneMask1, C_ascPhoneMask2, C_updateChannel, C_isPriceColOptVisible, C_isPriceColOpt2Visible, C_isPriceColOpt3Visible, C_isPriceColRoznVisible, C_isPriceColServiceVisible, C_voipId, C_isRealizatorEnable, C_onlineStoreId, C_isCartridgeRepairEnabled, C_isEngineerRequiredOnDeviceRecept, C_isVisitSourceRequired, C_isSerialNumberRequired, C_isCartridgeSerialNumberRequired, C_isSearchOnlyBySerial, C_isPhotoOnReceptRequired, C_isPhotoOnIssueRequired, C_isDiagRequired, C_isVendorAddingAllowedOnRecept, C_isRepairSummSetByManager, C_isPaySalaryForRepairsIssuedInDebt, C_isAutoSetCompanyOnRepairRecept, C_isVendorWarrantyEnabled, C_useSimplifiedCartridgeRepair, C_autoCloseRepairTabTimeout, C_repairImagesLimit, C_repairDispatcherUsersJson, C_repairDispatcherAssignCriteriaJson, C_repairDispatcherUsers, C_repairDispatcherDayLimit, C_repairDispatcherStatuses, C_isClientPatronymicRequired, C_isClientEmailRequired, C_isClientAddressRequired, C_isClientPhoneRequired, C_isAnyClientDealer, C_timeoutForItemsRequestsHandling, C_timeoutForDiagnosisConfirmation, C_defaultItemReserveTime, C_itemImagesLimit, C_isReasonForItemIncomeRequired, C_defaultWorksWarranty, C_defaultNewItemsWarranty, C_defaultUsedItemsWarranty, C_defaultRefItemsWarranty, C_defaultDisasmItemsWarranty, C_defaultOtherItemsWarranty, C_notifyNewComment, C_notifyNewCommentColor, C_notifyRepairStatusUpdateColor, C_notifyIncomingSMS, C_notifyIncomingSMSColor, C_notifyOutOfTermAlarmColor, C_notifyDeviceMatch, C_notifyDeviceMatchColor, C_notifyCustomTask, C_notifyCustomTaskColor, C_notifyItemRequest, C_notifyItemRequestColor, C_notifyOrderFromOnlineStore, C_notifyOrderFromOnlineStoreColor, C_notifyIncomingCall, C_notifyIncomingCallColor, C_notifyItemPurchaseRequest, C_notifyItemPurchaseRequestColor, C_printPKO, C_printOutInvoice, C_printInInvoice, C_printRKO, C_printWarrantyDoc, C_printWorksList, C_printDiagResult, C_printRepairRejectDoc, C_printRKOOnItemIncome, C_printCheck, C_printRepairReceptDoc, C_printRepairStickers, C_printCartridgeReceptDoc, C_printCartridgeStickers, C_defaultRepairStickersQty, C_autoSwitchKeyboardLayout, C_voipAsteriskHost, C_voipAsteriskPort, C_voipAsteriskLogin, C_voipAsteriskPassword, C_voipAsteriskWebPort, C_voipPrefix, C_voipEndpoint, C_voipKey, C_voipSecret, C_onlineStoreUrl, C_onlineStoreKey, C_salaryClassic, C_salaryIncludeNotIssuedByDefault, C_newClientSmsEnabled, C_repairStatuses, C_exchangeTypeAuto, C_exchangeSourceId, C_exchangeTypeManual, C_exchangeRate, C_backupEnable, C_backupImages, C_backupTime, C_smsConfigJson, C_smsProvider, C_smsAuthType, C_smsApiId, C_smsLogin, C_smsPassword, C_smsSender, C_emailConfigJson, C_emailServer, C_emailPort, C_emailLogin, C_emailPassword, C_emailTimeout, C_emailEnableSsl, C_emailEnableImplicitSsl, C_emailTemplate,};







    inline int id() const { return data(C_id).value<int>(); } inline void set_id(const int &data) { setData(C_id, data); } inline QString currencyId() const { return data(C_currencyId).value<QString>(); } inline void set_currencyId(const QString &data) { setData(C_currencyId, data); } inline int classicKassa() const { return data(C_classicKassa).value<int>(); } inline void set_classicKassa(const int &data) { setData(C_classicKassa, data); } inline int timeZoneId() const { return data(C_timeZoneId).value<int>(); } inline void set_timeZoneId(const int &data) { setData(C_timeZoneId, data); } inline QString ascPhoneMask1() const { return data(C_ascPhoneMask1).value<QString>(); } inline void set_ascPhoneMask1(const QString &data) { setData(C_ascPhoneMask1, data); } inline QString ascPhoneMask2() const { return data(C_ascPhoneMask2).value<QString>(); } inline void set_ascPhoneMask2(const QString &data) { setData(C_ascPhoneMask2, data); } inline QString updateChannel() const { return data(C_updateChannel).value<QString>(); } inline void set_updateChannel(const QString &data) { setData(C_updateChannel, data); } inline int isPriceColOptVisible() const { return data(C_isPriceColOptVisible).value<int>(); } inline void set_isPriceColOptVisible(const int &data) { setData(C_isPriceColOptVisible, data); } inline int isPriceColOpt2Visible() const { return data(C_isPriceColOpt2Visible).value<int>(); } inline void set_isPriceColOpt2Visible(const int &data) { setData(C_isPriceColOpt2Visible, data); } inline int isPriceColOpt3Visible() const { return data(C_isPriceColOpt3Visible).value<int>(); } inline void set_isPriceColOpt3Visible(const int &data) { setData(C_isPriceColOpt3Visible, data); } inline int isPriceColRoznVisible() const { return data(C_isPriceColRoznVisible).value<int>(); } inline void set_isPriceColRoznVisible(const int &data) { setData(C_isPriceColRoznVisible, data); } inline int isPriceColServiceVisible() const { return data(C_isPriceColServiceVisible).value<int>(); } inline void set_isPriceColServiceVisible(const int &data) { setData(C_isPriceColServiceVisible, data); } inline int voipId() const { return data(C_voipId).value<int>(); } inline void set_voipId(const int &data) { setData(C_voipId, data); } inline int isRealizatorEnable() const { return data(C_isRealizatorEnable).value<int>(); } inline void set_isRealizatorEnable(const int &data) { setData(C_isRealizatorEnable, data); } inline int onlineStoreId() const { return data(C_onlineStoreId).value<int>(); } inline void set_onlineStoreId(const int &data) { setData(C_onlineStoreId, data); } inline int isCartridgeRepairEnabled() const { return data(C_isCartridgeRepairEnabled).value<int>(); } inline void set_isCartridgeRepairEnabled(const int &data) { setData(C_isCartridgeRepairEnabled, data); } inline int isEngineerRequiredOnDeviceRecept() const { return data(C_isEngineerRequiredOnDeviceRecept).value<int>(); } inline void set_isEngineerRequiredOnDeviceRecept(const int &data) { setData(C_isEngineerRequiredOnDeviceRecept, data); } inline int isVisitSourceRequired() const { return data(C_isVisitSourceRequired).value<int>(); } inline void set_isVisitSourceRequired(const int &data) { setData(C_isVisitSourceRequired, data); } inline int isSerialNumberRequired() const { return data(C_isSerialNumberRequired).value<int>(); } inline void set_isSerialNumberRequired(const int &data) { setData(C_isSerialNumberRequired, data); } inline int isCartridgeSerialNumberRequired() const { return data(C_isCartridgeSerialNumberRequired).value<int>(); } inline void set_isCartridgeSerialNumberRequired(const int &data) { setData(C_isCartridgeSerialNumberRequired, data); } inline int isSearchOnlyBySerial() const { return data(C_isSearchOnlyBySerial).value<int>(); } inline void set_isSearchOnlyBySerial(const int &data) { setData(C_isSearchOnlyBySerial, data); } inline int isPhotoOnReceptRequired() const { return data(C_isPhotoOnReceptRequired).value<int>(); } inline void set_isPhotoOnReceptRequired(const int &data) { setData(C_isPhotoOnReceptRequired, data); } inline int isPhotoOnIssueRequired() const { return data(C_isPhotoOnIssueRequired).value<int>(); } inline void set_isPhotoOnIssueRequired(const int &data) { setData(C_isPhotoOnIssueRequired, data); } inline int isDiagRequired() const { return data(C_isDiagRequired).value<int>(); } inline void set_isDiagRequired(const int &data) { setData(C_isDiagRequired, data); } inline int isVendorAddingAllowedOnRecept() const { return data(C_isVendorAddingAllowedOnRecept).value<int>(); } inline void set_isVendorAddingAllowedOnRecept(const int &data) { setData(C_isVendorAddingAllowedOnRecept, data); } inline int isRepairSummSetByManager() const { return data(C_isRepairSummSetByManager).value<int>(); } inline void set_isRepairSummSetByManager(const int &data) { setData(C_isRepairSummSetByManager, data); } inline int isPaySalaryForRepairsIssuedInDebt() const { return data(C_isPaySalaryForRepairsIssuedInDebt).value<int>(); } inline void set_isPaySalaryForRepairsIssuedInDebt(const int &data) { setData(C_isPaySalaryForRepairsIssuedInDebt, data); } inline int isAutoSetCompanyOnRepairRecept() const { return data(C_isAutoSetCompanyOnRepairRecept).value<int>(); } inline void set_isAutoSetCompanyOnRepairRecept(const int &data) { setData(C_isAutoSetCompanyOnRepairRecept, data); } inline int isVendorWarrantyEnabled() const { return data(C_isVendorWarrantyEnabled).value<int>(); } inline void set_isVendorWarrantyEnabled(const int &data) { setData(C_isVendorWarrantyEnabled, data); } inline int useSimplifiedCartridgeRepair() const { return data(C_useSimplifiedCartridgeRepair).value<int>(); } inline void set_useSimplifiedCartridgeRepair(const int &data) { setData(C_useSimplifiedCartridgeRepair, data); } inline int autoCloseRepairTabTimeout() const { return data(C_autoCloseRepairTabTimeout).value<int>(); } inline void set_autoCloseRepairTabTimeout(const int &data) { setData(C_autoCloseRepairTabTimeout, data); } inline int repairImagesLimit() const { return data(C_repairImagesLimit).value<int>(); } inline void set_repairImagesLimit(const int &data) { setData(C_repairImagesLimit, data); } inline t_repairDispatcherUsers repairDispatcherUsersJsonVariantCopy() const { return data(C_repairDispatcherUsersJson).value<t_repairDispatcherUsers>(); } inline void set_repairDispatcherUsersJson(const t_repairDispatcherUsers &data) { setData(C_repairDispatcherUsersJson, data); } inline t_repairDispatcherAssignCriteria repairDispatcherAssignCriteriaJsonVariantCopy() const { return data(C_repairDispatcherAssignCriteriaJson).value<t_repairDispatcherAssignCriteria>(); } inline void set_repairDispatcherAssignCriteriaJson(const t_repairDispatcherAssignCriteria &data) { setData(C_repairDispatcherAssignCriteriaJson, data); } inline QString repairDispatcherUsers() const { return data(C_repairDispatcherUsers).value<QString>(); } inline void set_repairDispatcherUsers(const QString &data) { setData(C_repairDispatcherUsers, data); } inline int repairDispatcherDayLimit() const { return data(C_repairDispatcherDayLimit).value<int>(); } inline void set_repairDispatcherDayLimit(const int &data) { setData(C_repairDispatcherDayLimit, data); } inline QString repairDispatcherStatuses() const { return data(C_repairDispatcherStatuses).value<QString>(); } inline void set_repairDispatcherStatuses(const QString &data) { setData(C_repairDispatcherStatuses, data); } inline int isClientPatronymicRequired() const { return data(C_isClientPatronymicRequired).value<int>(); } inline void set_isClientPatronymicRequired(const int &data) { setData(C_isClientPatronymicRequired, data); } inline int isClientEmailRequired() const { return data(C_isClientEmailRequired).value<int>(); } inline void set_isClientEmailRequired(const int &data) { setData(C_isClientEmailRequired, data); } inline int isClientAddressRequired() const { return data(C_isClientAddressRequired).value<int>(); } inline void set_isClientAddressRequired(const int &data) { setData(C_isClientAddressRequired, data); } inline int isClientPhoneRequired() const { return data(C_isClientPhoneRequired).value<int>(); } inline void set_isClientPhoneRequired(const int &data) { setData(C_isClientPhoneRequired, data); } inline int isAnyClientDealer() const { return data(C_isAnyClientDealer).value<int>(); } inline void set_isAnyClientDealer(const int &data) { setData(C_isAnyClientDealer, data); } inline int timeoutForItemsRequestsHandling() const { return data(C_timeoutForItemsRequestsHandling).value<int>(); } inline void set_timeoutForItemsRequestsHandling(const int &data) { setData(C_timeoutForItemsRequestsHandling, data); } inline int timeoutForDiagnosisConfirmation() const { return data(C_timeoutForDiagnosisConfirmation).value<int>(); } inline void set_timeoutForDiagnosisConfirmation(const int &data) { setData(C_timeoutForDiagnosisConfirmation, data); } inline int defaultItemReserveTime() const { return data(C_defaultItemReserveTime).value<int>(); } inline void set_defaultItemReserveTime(const int &data) { setData(C_defaultItemReserveTime, data); } inline int itemImagesLimit() const { return data(C_itemImagesLimit).value<int>(); } inline void set_itemImagesLimit(const int &data) { setData(C_itemImagesLimit, data); } inline int isReasonForItemIncomeRequired() const { return data(C_isReasonForItemIncomeRequired).value<int>(); } inline void set_isReasonForItemIncomeRequired(const int &data) { setData(C_isReasonForItemIncomeRequired, data); } inline int defaultWorksWarranty() const { return data(C_defaultWorksWarranty).value<int>(); } inline void set_defaultWorksWarranty(const int &data) { setData(C_defaultWorksWarranty, data); } inline int defaultNewItemsWarranty() const { return data(C_defaultNewItemsWarranty).value<int>(); } inline void set_defaultNewItemsWarranty(const int &data) { setData(C_defaultNewItemsWarranty, data); } inline int defaultUsedItemsWarranty() const { return data(C_defaultUsedItemsWarranty).value<int>(); } inline void set_defaultUsedItemsWarranty(const int &data) { setData(C_defaultUsedItemsWarranty, data); } inline int defaultRefItemsWarranty() const { return data(C_defaultRefItemsWarranty).value<int>(); } inline void set_defaultRefItemsWarranty(const int &data) { setData(C_defaultRefItemsWarranty, data); } inline int defaultDisasmItemsWarranty() const { return data(C_defaultDisasmItemsWarranty).value<int>(); } inline void set_defaultDisasmItemsWarranty(const int &data) { setData(C_defaultDisasmItemsWarranty, data); } inline int defaultOtherItemsWarranty() const { return data(C_defaultOtherItemsWarranty).value<int>(); } inline void set_defaultOtherItemsWarranty(const int &data) { setData(C_defaultOtherItemsWarranty, data); } inline int notifyNewComment() const { return data(C_notifyNewComment).value<int>(); } inline void set_notifyNewComment(const int &data) { setData(C_notifyNewComment, data); } inline int notifyNewCommentColor() const { return data(C_notifyNewCommentColor).value<int>(); } inline void set_notifyNewCommentColor(const int &data) { setData(C_notifyNewCommentColor, data); } inline int notifyRepairStatusUpdateColor() const { return data(C_notifyRepairStatusUpdateColor).value<int>(); } inline void set_notifyRepairStatusUpdateColor(const int &data) { setData(C_notifyRepairStatusUpdateColor, data); } inline int notifyIncomingSMS() const { return data(C_notifyIncomingSMS).value<int>(); } inline void set_notifyIncomingSMS(const int &data) { setData(C_notifyIncomingSMS, data); } inline int notifyIncomingSMSColor() const { return data(C_notifyIncomingSMSColor).value<int>(); } inline void set_notifyIncomingSMSColor(const int &data) { setData(C_notifyIncomingSMSColor, data); } inline int notifyOutOfTermAlarmColor() const { return data(C_notifyOutOfTermAlarmColor).value<int>(); } inline void set_notifyOutOfTermAlarmColor(const int &data) { setData(C_notifyOutOfTermAlarmColor, data); } inline int notifyDeviceMatch() const { return data(C_notifyDeviceMatch).value<int>(); } inline void set_notifyDeviceMatch(const int &data) { setData(C_notifyDeviceMatch, data); } inline int notifyDeviceMatchColor() const { return data(C_notifyDeviceMatchColor).value<int>(); } inline void set_notifyDeviceMatchColor(const int &data) { setData(C_notifyDeviceMatchColor, data); } inline int notifyCustomTask() const { return data(C_notifyCustomTask).value<int>(); } inline void set_notifyCustomTask(const int &data) { setData(C_notifyCustomTask, data); } inline int notifyCustomTaskColor() const { return data(C_notifyCustomTaskColor).value<int>(); } inline void set_notifyCustomTaskColor(const int &data) { setData(C_notifyCustomTaskColor, data); } inline int notifyItemRequest() const { return data(C_notifyItemRequest).value<int>(); } inline void set_notifyItemRequest(const int &data) { setData(C_notifyItemRequest, data); } inline int notifyItemRequestColor() const { return data(C_notifyItemRequestColor).value<int>(); } inline void set_notifyItemRequestColor(const int &data) { setData(C_notifyItemRequestColor, data); } inline int notifyOrderFromOnlineStore() const { return data(C_notifyOrderFromOnlineStore).value<int>(); } inline void set_notifyOrderFromOnlineStore(const int &data) { setData(C_notifyOrderFromOnlineStore, data); } inline int notifyOrderFromOnlineStoreColor() const { return data(C_notifyOrderFromOnlineStoreColor).value<int>(); } inline void set_notifyOrderFromOnlineStoreColor(const int &data) { setData(C_notifyOrderFromOnlineStoreColor, data); } inline int notifyIncomingCall() const { return data(C_notifyIncomingCall).value<int>(); } inline void set_notifyIncomingCall(const int &data) { setData(C_notifyIncomingCall, data); } inline int notifyIncomingCallColor() const { return data(C_notifyIncomingCallColor).value<int>(); } inline void set_notifyIncomingCallColor(const int &data) { setData(C_notifyIncomingCallColor, data); } inline int notifyItemPurchaseRequest() const { return data(C_notifyItemPurchaseRequest).value<int>(); } inline void set_notifyItemPurchaseRequest(const int &data) { setData(C_notifyItemPurchaseRequest, data); } inline int notifyItemPurchaseRequestColor() const { return data(C_notifyItemPurchaseRequestColor).value<int>(); } inline void set_notifyItemPurchaseRequestColor(const int &data) { setData(C_notifyItemPurchaseRequestColor, data); } inline int printPKO() const { return data(C_printPKO).value<int>(); } inline void set_printPKO(const int &data) { setData(C_printPKO, data); } inline int printOutInvoice() const { return data(C_printOutInvoice).value<int>(); } inline void set_printOutInvoice(const int &data) { setData(C_printOutInvoice, data); } inline int printInInvoice() const { return data(C_printInInvoice).value<int>(); } inline void set_printInInvoice(const int &data) { setData(C_printInInvoice, data); } inline int printRKO() const { return data(C_printRKO).value<int>(); } inline void set_printRKO(const int &data) { setData(C_printRKO, data); } inline int printWarrantyDoc() const { return data(C_printWarrantyDoc).value<int>(); } inline void set_printWarrantyDoc(const int &data) { setData(C_printWarrantyDoc, data); } inline int printWorksList() const { return data(C_printWorksList).value<int>(); } inline void set_printWorksList(const int &data) { setData(C_printWorksList, data); } inline int printDiagResult() const { return data(C_printDiagResult).value<int>(); } inline void set_printDiagResult(const int &data) { setData(C_printDiagResult, data); } inline int printRepairRejectDoc() const { return data(C_printRepairRejectDoc).value<int>(); } inline void set_printRepairRejectDoc(const int &data) { setData(C_printRepairRejectDoc, data); } inline int printRKOOnItemIncome() const { return data(C_printRKOOnItemIncome).value<int>(); } inline void set_printRKOOnItemIncome(const int &data) { setData(C_printRKOOnItemIncome, data); } inline int printCheck() const { return data(C_printCheck).value<int>(); } inline void set_printCheck(const int &data) { setData(C_printCheck, data); } inline int printRepairReceptDoc() const { return data(C_printRepairReceptDoc).value<int>(); } inline void set_printRepairReceptDoc(const int &data) { setData(C_printRepairReceptDoc, data); } inline int printRepairStickers() const { return data(C_printRepairStickers).value<int>(); } inline void set_printRepairStickers(const int &data) { setData(C_printRepairStickers, data); } inline int printCartridgeReceptDoc() const { return data(C_printCartridgeReceptDoc).value<int>(); } inline void set_printCartridgeReceptDoc(const int &data) { setData(C_printCartridgeReceptDoc, data); } inline int printCartridgeStickers() const { return data(C_printCartridgeStickers).value<int>(); } inline void set_printCartridgeStickers(const int &data) { setData(C_printCartridgeStickers, data); } inline int defaultRepairStickersQty() const { return data(C_defaultRepairStickersQty).value<int>(); } inline void set_defaultRepairStickersQty(const int &data) { setData(C_defaultRepairStickersQty, data); } inline int autoSwitchKeyboardLayout() const { return data(C_autoSwitchKeyboardLayout).value<int>(); } inline void set_autoSwitchKeyboardLayout(const int &data) { setData(C_autoSwitchKeyboardLayout, data); } inline QString voipAsteriskHost() const { return data(C_voipAsteriskHost).value<QString>(); } inline void set_voipAsteriskHost(const QString &data) { setData(C_voipAsteriskHost, data); } inline int voipAsteriskPort() const { return data(C_voipAsteriskPort).value<int>(); } inline void set_voipAsteriskPort(const int &data) { setData(C_voipAsteriskPort, data); } inline QString voipAsteriskLogin() const { return data(C_voipAsteriskLogin).value<QString>(); } inline void set_voipAsteriskLogin(const QString &data) { setData(C_voipAsteriskLogin, data); } inline QString voipAsteriskPassword() const { return data(C_voipAsteriskPassword).value<QString>(); } inline void set_voipAsteriskPassword(const QString &data) { setData(C_voipAsteriskPassword, data); } inline int voipAsteriskWebPort() const { return data(C_voipAsteriskWebPort).value<int>(); } inline void set_voipAsteriskWebPort(const int &data) { setData(C_voipAsteriskWebPort, data); } inline QString voipPrefix() const { return data(C_voipPrefix).value<QString>(); } inline void set_voipPrefix(const QString &data) { setData(C_voipPrefix, data); } inline QString voipEndpoint() const { return data(C_voipEndpoint).value<QString>(); } inline void set_voipEndpoint(const QString &data) { setData(C_voipEndpoint, data); } inline QString voipKey() const { return data(C_voipKey).value<QString>(); } inline void set_voipKey(const QString &data) { setData(C_voipKey, data); } inline QString voipSecret() const { return data(C_voipSecret).value<QString>(); } inline void set_voipSecret(const QString &data) { setData(C_voipSecret, data); } inline QString onlineStoreUrl() const { return data(C_onlineStoreUrl).value<QString>(); } inline void set_onlineStoreUrl(const QString &data) { setData(C_onlineStoreUrl, data); } inline QString onlineStoreKey() const { return data(C_onlineStoreKey).value<QString>(); } inline void set_onlineStoreKey(const QString &data) { setData(C_onlineStoreKey, data); } inline int salaryClassic() const { return data(C_salaryClassic).value<int>(); } inline void set_salaryClassic(const int &data) { setData(C_salaryClassic, data); } inline int salaryIncludeNotIssuedByDefault() const { return data(C_salaryIncludeNotIssuedByDefault).value<int>(); } inline void set_salaryIncludeNotIssuedByDefault(const int &data) { setData(C_salaryIncludeNotIssuedByDefault, data); } inline int newClientSmsEnabled() const { return data(C_newClientSmsEnabled).value<int>(); } inline void set_newClientSmsEnabled(const int &data) { setData(C_newClientSmsEnabled, data); } inline t_repairStatuses repairStatusesVariantCopy() const { return data(C_repairStatuses).value<t_repairStatuses>(); } inline void set_repairStatuses(const t_repairStatuses &data) { setData(C_repairStatuses, data); } inline bool exchangeTypeAuto() const { return data(C_exchangeTypeAuto).value<bool>(); } inline void set_exchangeTypeAuto(const bool &data) { setData(C_exchangeTypeAuto, data); } inline int exchangeSourceId() const { return data(C_exchangeSourceId).value<int>(); } inline void set_exchangeSourceId(const int &data) { setData(C_exchangeSourceId, data); } inline bool exchangeTypeManual() const { return data(C_exchangeTypeManual).value<bool>(); } inline void set_exchangeTypeManual(const bool &data) { setData(C_exchangeTypeManual, data); } inline double exchangeRate() const { return data(C_exchangeRate).value<double>(); } inline void set_exchangeRate(const double &data) { setData(C_exchangeRate, data); } inline int backupEnable() const { return data(C_backupEnable).value<int>(); } inline void set_backupEnable(const int &data) { setData(C_backupEnable, data); } inline int backupImages() const { return data(C_backupImages).value<int>(); } inline void set_backupImages(const int &data) { setData(C_backupImages, data); } inline QDateTime backupTime() const { return data(C_backupTime).value<QDateTime>(); } inline void set_backupTime(const QDateTime &data) { setData(C_backupTime, data); } inline t_smsConfig smsConfigJsonVariantCopy() const { return data(C_smsConfigJson).value<t_smsConfig>(); } inline void set_smsConfigJson(const t_smsConfig &data) { setData(C_smsConfigJson, data); } inline int smsProvider() const { return data(C_smsProvider).value<int>(); } inline void set_smsProvider(const int &data) { setData(C_smsProvider, data); } inline int smsAuthType() const { return data(C_smsAuthType).value<int>(); } inline void set_smsAuthType(const int &data) { setData(C_smsAuthType, data); } inline QString smsApiId() const { return data(C_smsApiId).value<QString>(); } inline void set_smsApiId(const QString &data) { setData(C_smsApiId, data); } inline QString smsLogin() const { return data(C_smsLogin).value<QString>(); } inline void set_smsLogin(const QString &data) { setData(C_smsLogin, data); } inline QString smsPassword() const { return data(C_smsPassword).value<QString>(); } inline void set_smsPassword(const QString &data) { setData(C_smsPassword, data); } inline QString smsSender() const { return data(C_smsSender).value<QString>(); } inline void set_smsSender(const QString &data) { setData(C_smsSender, data); } inline t_emailConfig emailConfigJsonVariantCopy() const { return data(C_emailConfigJson).value<t_emailConfig>(); } inline void set_emailConfigJson(const t_emailConfig &data) { setData(C_emailConfigJson, data); } inline QString emailServer() const { return data(C_emailServer).value<QString>(); } inline void set_emailServer(const QString &data) { setData(C_emailServer, data); } inline int emailPort() const { return data(C_emailPort).value<int>(); } inline void set_emailPort(const int &data) { setData(C_emailPort, data); } inline QString emailLogin() const { return data(C_emailLogin).value<QString>(); } inline void set_emailLogin(const QString &data) { setData(C_emailLogin, data); } inline QString emailPassword() const { return data(C_emailPassword).value<QString>(); } inline void set_emailPassword(const QString &data) { setData(C_emailPassword, data); } inline int emailTimeout() const { return data(C_emailTimeout).value<int>(); } inline void set_emailTimeout(const int &data) { setData(C_emailTimeout, data); } inline bool emailEnableSsl() const { return data(C_emailEnableSsl).value<bool>(); } inline void set_emailEnableSsl(const bool &data) { setData(C_emailEnableSsl, data); } inline bool emailEnableImplicitSsl() const { return data(C_emailEnableImplicitSsl).value<bool>(); } inline void set_emailEnableImplicitSsl(const bool &data) { setData(C_emailEnableImplicitSsl, data); } inline QString emailTemplate() const { return data(C_emailTemplate).value<QString>(); } inline void set_emailTemplate(const QString &data) { setData(C_emailTemplate, data); }
private:
    void mapFields() final {
        sz = sizeof((const QString[]){"id", "currency", "classic_kassa", "time_zone", "phone_mask1", "phone_mask2", "settings.update_channel", "it_vis_opt", "it_vis_opt2", "it_vis_opt3", "it_vis_rozn", "it_vis_price_for_sc", "settings.voip", "realizator_enable", "online_store", "cartridge_enable", "is_master_set_on_new", "visit_source_force", "is_sn_req", "is_cartridge_sn_req", "history_by_sn", "is_photo_required", "is_photo_out_req", "diag_required", "manual_maker", "rep_price_by_manager", "debt_rep_2_salary", "rep_auto_company", "vw_enable", "settings.use_simplified_cartridge_repair", "card_close_time", "rep_img_limit", "settings.auto_assign_users", "settings.auto_assign_criteria", "IGNORE", "IGNORE", "IGNORE", "is_patronymic_required", "email_required", "address_required", "phone_required", "clients_are_dealers", "give2user_time", "diag_accept_time", "default_reserve_days", "item_img_limit", "is_reason_req", "default_works_warranty", "default_items_warranty", "default_items_used_warranty", "default_items_rep_warranty", "default_items_razb_warranty", "default_items_other_warranty", "settings.inform_comment", "settings.inform_comment_color", "settings.inform_status_color", "settings.inform_sms", "settings.inform_sms_color", "settings.inform_terms_color", "settings.inform_task_match", "settings.inform_task_match_color", "settings.inform_task_custom", "settings.inform_task_custom_color", "settings.inform_int_request", "settings.inform_int_request_color", "settings.inform_task_request", "settings.inform_task_request_color", "settings.inform_call", "settings.inform_call_color", "settings.inform_part_request", "settings.inform_part_request_color", "qs_print_pko", "qs_print_rn", "qs_print_pn", "qs_print_rko", "print_warranty", "print_works", "print_diagnostic", "print_reject", "rko_on_pn", "print_check", "settings.print_new_repair_report", "settings.print_rep_stickers", "settings.print_new_cartridge_report", "settings.print_cartridge_stickers", "rep_stickers_copy", "auto_switch_layout", "aster_host", "aster_port", "aster_login", "aster_password", "aster_web_port", "settings.voip_prefix", "settings.voip_endpoint", "settings.voip_key", "settings.voip_secret", "online_store_api", "online_store_key", "settings.classic_salary", "settings.salary_include_not_issued_by_default", "settings.new_client_sms_enabled", "statuses", "IGNORE", "exchange_source", "IGNORE", "exchange_rate", "backup_enable", "backup_images", "backup_time", "sms_config", "IGNORE", "IGNORE", "IGNORE", "IGNORE", "IGNORE", "IGNORE", "email_config", "IGNORE", "IGNORE", "IGNORE", "IGNORE", "IGNORE", "IGNORE", "IGNORE", "IGNORE",})/sizeof(const QString);


        insertNewField(C_id, "id"); insertNewField(C_currencyId, "currency"); insertNewField(C_classicKassa, "classic_kassa"); insertNewField(C_timeZoneId, "time_zone"); insertNewField(C_ascPhoneMask1, "phone_mask1"); insertNewField(C_ascPhoneMask2, "phone_mask2"); insertNewField(C_updateChannel, "settings.update_channel"); insertNewField(C_isPriceColOptVisible, "it_vis_opt"); insertNewField(C_isPriceColOpt2Visible, "it_vis_opt2"); insertNewField(C_isPriceColOpt3Visible, "it_vis_opt3"); insertNewField(C_isPriceColRoznVisible, "it_vis_rozn"); insertNewField(C_isPriceColServiceVisible, "it_vis_price_for_sc"); insertNewField(C_voipId, "settings.voip"); insertNewField(C_isRealizatorEnable, "realizator_enable"); insertNewField(C_onlineStoreId, "online_store"); insertNewField(C_isCartridgeRepairEnabled, "cartridge_enable"); insertNewField(C_isEngineerRequiredOnDeviceRecept, "is_master_set_on_new"); insertNewField(C_isVisitSourceRequired, "visit_source_force"); insertNewField(C_isSerialNumberRequired, "is_sn_req"); insertNewField(C_isCartridgeSerialNumberRequired, "is_cartridge_sn_req"); insertNewField(C_isSearchOnlyBySerial, "history_by_sn"); insertNewField(C_isPhotoOnReceptRequired, "is_photo_required"); insertNewField(C_isPhotoOnIssueRequired, "is_photo_out_req"); insertNewField(C_isDiagRequired, "diag_required"); insertNewField(C_isVendorAddingAllowedOnRecept, "manual_maker"); insertNewField(C_isRepairSummSetByManager, "rep_price_by_manager"); insertNewField(C_isPaySalaryForRepairsIssuedInDebt, "debt_rep_2_salary"); insertNewField(C_isAutoSetCompanyOnRepairRecept, "rep_auto_company"); insertNewField(C_isVendorWarrantyEnabled, "vw_enable"); insertNewField(C_useSimplifiedCartridgeRepair, "settings.use_simplified_cartridge_repair"); insertNewField(C_autoCloseRepairTabTimeout, "card_close_time"); insertNewField(C_repairImagesLimit, "rep_img_limit"); insertNewField(C_repairDispatcherUsersJson, "settings.auto_assign_users"); insertNewField(C_repairDispatcherAssignCriteriaJson, "settings.auto_assign_criteria"); insertNewField(C_isClientPatronymicRequired, "is_patronymic_required"); insertNewField(C_isClientEmailRequired, "email_required"); insertNewField(C_isClientAddressRequired, "address_required"); insertNewField(C_isClientPhoneRequired, "phone_required"); insertNewField(C_isAnyClientDealer, "clients_are_dealers"); insertNewField(C_timeoutForItemsRequestsHandling, "give2user_time"); insertNewField(C_timeoutForDiagnosisConfirmation, "diag_accept_time"); insertNewField(C_defaultItemReserveTime, "default_reserve_days"); insertNewField(C_itemImagesLimit, "item_img_limit"); insertNewField(C_isReasonForItemIncomeRequired, "is_reason_req"); insertNewField(C_defaultWorksWarranty, "default_works_warranty"); insertNewField(C_defaultNewItemsWarranty, "default_items_warranty"); insertNewField(C_defaultUsedItemsWarranty, "default_items_used_warranty"); insertNewField(C_defaultRefItemsWarranty, "default_items_rep_warranty"); insertNewField(C_defaultDisasmItemsWarranty, "default_items_razb_warranty"); insertNewField(C_defaultOtherItemsWarranty, "default_items_other_warranty"); insertNewField(C_notifyNewComment, "settings.inform_comment"); insertNewField(C_notifyNewCommentColor, "settings.inform_comment_color"); insertNewField(C_notifyRepairStatusUpdateColor, "settings.inform_status_color"); insertNewField(C_notifyIncomingSMS, "settings.inform_sms"); insertNewField(C_notifyIncomingSMSColor, "settings.inform_sms_color"); insertNewField(C_notifyOutOfTermAlarmColor, "settings.inform_terms_color"); insertNewField(C_notifyDeviceMatch, "settings.inform_task_match"); insertNewField(C_notifyDeviceMatchColor, "settings.inform_task_match_color"); insertNewField(C_notifyCustomTask, "settings.inform_task_custom"); insertNewField(C_notifyCustomTaskColor, "settings.inform_task_custom_color"); insertNewField(C_notifyItemRequest, "settings.inform_int_request"); insertNewField(C_notifyItemRequestColor, "settings.inform_int_request_color"); insertNewField(C_notifyOrderFromOnlineStore, "settings.inform_task_request"); insertNewField(C_notifyOrderFromOnlineStoreColor, "settings.inform_task_request_color"); insertNewField(C_notifyIncomingCall, "settings.inform_call"); insertNewField(C_notifyIncomingCallColor, "settings.inform_call_color"); insertNewField(C_notifyItemPurchaseRequest, "settings.inform_part_request"); insertNewField(C_notifyItemPurchaseRequestColor, "settings.inform_part_request_color"); insertNewField(C_printPKO, "qs_print_pko"); insertNewField(C_printOutInvoice, "qs_print_rn"); insertNewField(C_printInInvoice, "qs_print_pn"); insertNewField(C_printRKO, "qs_print_rko"); insertNewField(C_printWarrantyDoc, "print_warranty"); insertNewField(C_printWorksList, "print_works"); insertNewField(C_printDiagResult, "print_diagnostic"); insertNewField(C_printRepairRejectDoc, "print_reject"); insertNewField(C_printRKOOnItemIncome, "rko_on_pn"); insertNewField(C_printCheck, "print_check"); insertNewField(C_printRepairReceptDoc, "settings.print_new_repair_report"); insertNewField(C_printRepairStickers, "settings.print_rep_stickers"); insertNewField(C_printCartridgeReceptDoc, "settings.print_new_cartridge_report"); insertNewField(C_printCartridgeStickers, "settings.print_cartridge_stickers"); insertNewField(C_defaultRepairStickersQty, "rep_stickers_copy"); insertNewField(C_autoSwitchKeyboardLayout, "auto_switch_layout"); insertNewField(C_voipAsteriskHost, "aster_host"); insertNewField(C_voipAsteriskPort, "aster_port"); insertNewField(C_voipAsteriskLogin, "aster_login"); insertNewField(C_voipAsteriskPassword, "aster_password"); insertNewField(C_voipAsteriskWebPort, "aster_web_port"); insertNewField(C_voipPrefix, "settings.voip_prefix"); insertNewField(C_voipEndpoint, "settings.voip_endpoint"); insertNewField(C_voipKey, "settings.voip_key"); insertNewField(C_voipSecret, "settings.voip_secret"); insertNewField(C_onlineStoreUrl, "online_store_api"); insertNewField(C_onlineStoreKey, "online_store_key"); insertNewField(C_salaryClassic, "settings.classic_salary"); insertNewField(C_salaryIncludeNotIssuedByDefault, "settings.salary_include_not_issued_by_default"); insertNewField(C_newClientSmsEnabled, "settings.new_client_sms_enabled"); insertNewField(C_repairStatuses, "statuses"); insertNewField(C_exchangeSourceId, "exchange_source"); insertNewField(C_exchangeRate, "exchange_rate"); insertNewField(C_backupEnable, "backup_enable"); insertNewField(C_backupImages, "backup_images"); insertNewField(C_backupTime, "backup_time"); insertNewField(C_smsConfigJson, "sms_config"); insertNewField(C_emailConfigJson, "email_config");



        m_namesMap->insert(C_id, "id"); m_namesMap->insert(C_currencyId, "currencyId"); m_namesMap->insert(C_classicKassa, "classicKassa"); m_namesMap->insert(C_timeZoneId, "timeZoneId"); m_namesMap->insert(C_ascPhoneMask1, "ascPhoneMask1"); m_namesMap->insert(C_ascPhoneMask2, "ascPhoneMask2"); m_namesMap->insert(C_updateChannel, "updateChannel"); m_namesMap->insert(C_isPriceColOptVisible, "isPriceColOptVisible"); m_namesMap->insert(C_isPriceColOpt2Visible, "isPriceColOpt2Visible"); m_namesMap->insert(C_isPriceColOpt3Visible, "isPriceColOpt3Visible"); m_namesMap->insert(C_isPriceColRoznVisible, "isPriceColRoznVisible"); m_namesMap->insert(C_isPriceColServiceVisible, "isPriceColServiceVisible"); m_namesMap->insert(C_voipId, "voipId"); m_namesMap->insert(C_isRealizatorEnable, "isRealizatorEnable"); m_namesMap->insert(C_onlineStoreId, "onlineStoreId"); m_namesMap->insert(C_isCartridgeRepairEnabled, "isCartridgeRepairEnabled"); m_namesMap->insert(C_isEngineerRequiredOnDeviceRecept, "isEngineerRequiredOnDeviceRecept"); m_namesMap->insert(C_isVisitSourceRequired, "isVisitSourceRequired"); m_namesMap->insert(C_isSerialNumberRequired, "isSerialNumberRequired"); m_namesMap->insert(C_isCartridgeSerialNumberRequired, "isCartridgeSerialNumberRequired"); m_namesMap->insert(C_isSearchOnlyBySerial, "isSearchOnlyBySerial"); m_namesMap->insert(C_isPhotoOnReceptRequired, "isPhotoOnReceptRequired"); m_namesMap->insert(C_isPhotoOnIssueRequired, "isPhotoOnIssueRequired"); m_namesMap->insert(C_isDiagRequired, "isDiagRequired"); m_namesMap->insert(C_isVendorAddingAllowedOnRecept, "isVendorAddingAllowedOnRecept"); m_namesMap->insert(C_isRepairSummSetByManager, "isRepairSummSetByManager"); m_namesMap->insert(C_isPaySalaryForRepairsIssuedInDebt, "isPaySalaryForRepairsIssuedInDebt"); m_namesMap->insert(C_isAutoSetCompanyOnRepairRecept, "isAutoSetCompanyOnRepairRecept"); m_namesMap->insert(C_isVendorWarrantyEnabled, "isVendorWarrantyEnabled"); m_namesMap->insert(C_useSimplifiedCartridgeRepair, "useSimplifiedCartridgeRepair"); m_namesMap->insert(C_autoCloseRepairTabTimeout, "autoCloseRepairTabTimeout"); m_namesMap->insert(C_repairImagesLimit, "repairImagesLimit"); m_namesMap->insert(C_repairDispatcherUsersJson, "repairDispatcherUsersJson"); m_namesMap->insert(C_repairDispatcherAssignCriteriaJson, "repairDispatcherAssignCriteriaJson"); m_namesMap->insert(C_repairDispatcherUsers, "repairDispatcherUsers"); m_namesMap->insert(C_repairDispatcherDayLimit, "repairDispatcherDayLimit"); m_namesMap->insert(C_repairDispatcherStatuses, "repairDispatcherStatuses"); m_namesMap->insert(C_isClientPatronymicRequired, "isClientPatronymicRequired"); m_namesMap->insert(C_isClientEmailRequired, "isClientEmailRequired"); m_namesMap->insert(C_isClientAddressRequired, "isClientAddressRequired"); m_namesMap->insert(C_isClientPhoneRequired, "isClientPhoneRequired"); m_namesMap->insert(C_isAnyClientDealer, "isAnyClientDealer"); m_namesMap->insert(C_timeoutForItemsRequestsHandling, "timeoutForItemsRequestsHandling"); m_namesMap->insert(C_timeoutForDiagnosisConfirmation, "timeoutForDiagnosisConfirmation"); m_namesMap->insert(C_defaultItemReserveTime, "defaultItemReserveTime"); m_namesMap->insert(C_itemImagesLimit, "itemImagesLimit"); m_namesMap->insert(C_isReasonForItemIncomeRequired, "isReasonForItemIncomeRequired"); m_namesMap->insert(C_defaultWorksWarranty, "defaultWorksWarranty"); m_namesMap->insert(C_defaultNewItemsWarranty, "defaultNewItemsWarranty"); m_namesMap->insert(C_defaultUsedItemsWarranty, "defaultUsedItemsWarranty"); m_namesMap->insert(C_defaultRefItemsWarranty, "defaultRefItemsWarranty"); m_namesMap->insert(C_defaultDisasmItemsWarranty, "defaultDisasmItemsWarranty"); m_namesMap->insert(C_defaultOtherItemsWarranty, "defaultOtherItemsWarranty"); m_namesMap->insert(C_notifyNewComment, "notifyNewComment"); m_namesMap->insert(C_notifyNewCommentColor, "notifyNewCommentColor"); m_namesMap->insert(C_notifyRepairStatusUpdateColor, "notifyRepairStatusUpdateColor"); m_namesMap->insert(C_notifyIncomingSMS, "notifyIncomingSMS"); m_namesMap->insert(C_notifyIncomingSMSColor, "notifyIncomingSMSColor"); m_namesMap->insert(C_notifyOutOfTermAlarmColor, "notifyOutOfTermAlarmColor"); m_namesMap->insert(C_notifyDeviceMatch, "notifyDeviceMatch"); m_namesMap->insert(C_notifyDeviceMatchColor, "notifyDeviceMatchColor"); m_namesMap->insert(C_notifyCustomTask, "notifyCustomTask"); m_namesMap->insert(C_notifyCustomTaskColor, "notifyCustomTaskColor"); m_namesMap->insert(C_notifyItemRequest, "notifyItemRequest"); m_namesMap->insert(C_notifyItemRequestColor, "notifyItemRequestColor"); m_namesMap->insert(C_notifyOrderFromOnlineStore, "notifyOrderFromOnlineStore"); m_namesMap->insert(C_notifyOrderFromOnlineStoreColor, "notifyOrderFromOnlineStoreColor"); m_namesMap->insert(C_notifyIncomingCall, "notifyIncomingCall"); m_namesMap->insert(C_notifyIncomingCallColor, "notifyIncomingCallColor"); m_namesMap->insert(C_notifyItemPurchaseRequest, "notifyItemPurchaseRequest"); m_namesMap->insert(C_notifyItemPurchaseRequestColor, "notifyItemPurchaseRequestColor"); m_namesMap->insert(C_printPKO, "printPKO"); m_namesMap->insert(C_printOutInvoice, "printOutInvoice"); m_namesMap->insert(C_printInInvoice, "printInInvoice"); m_namesMap->insert(C_printRKO, "printRKO"); m_namesMap->insert(C_printWarrantyDoc, "printWarrantyDoc"); m_namesMap->insert(C_printWorksList, "printWorksList"); m_namesMap->insert(C_printDiagResult, "printDiagResult"); m_namesMap->insert(C_printRepairRejectDoc, "printRepairRejectDoc"); m_namesMap->insert(C_printRKOOnItemIncome, "printRKOOnItemIncome"); m_namesMap->insert(C_printCheck, "printCheck"); m_namesMap->insert(C_printRepairReceptDoc, "printRepairReceptDoc"); m_namesMap->insert(C_printRepairStickers, "printRepairStickers"); m_namesMap->insert(C_printCartridgeReceptDoc, "printCartridgeReceptDoc"); m_namesMap->insert(C_printCartridgeStickers, "printCartridgeStickers"); m_namesMap->insert(C_defaultRepairStickersQty, "defaultRepairStickersQty"); m_namesMap->insert(C_autoSwitchKeyboardLayout, "autoSwitchKeyboardLayout"); m_namesMap->insert(C_voipAsteriskHost, "voipAsteriskHost"); m_namesMap->insert(C_voipAsteriskPort, "voipAsteriskPort"); m_namesMap->insert(C_voipAsteriskLogin, "voipAsteriskLogin"); m_namesMap->insert(C_voipAsteriskPassword, "voipAsteriskPassword"); m_namesMap->insert(C_voipAsteriskWebPort, "voipAsteriskWebPort"); m_namesMap->insert(C_voipPrefix, "voipPrefix"); m_namesMap->insert(C_voipEndpoint, "voipEndpoint"); m_namesMap->insert(C_voipKey, "voipKey"); m_namesMap->insert(C_voipSecret, "voipSecret"); m_namesMap->insert(C_onlineStoreUrl, "onlineStoreUrl"); m_namesMap->insert(C_onlineStoreKey, "onlineStoreKey"); m_namesMap->insert(C_salaryClassic, "salaryClassic"); m_namesMap->insert(C_salaryIncludeNotIssuedByDefault, "salaryIncludeNotIssuedByDefault"); m_namesMap->insert(C_newClientSmsEnabled, "newClientSmsEnabled"); m_namesMap->insert(C_repairStatuses, "repairStatuses"); m_namesMap->insert(C_exchangeTypeAuto, "exchangeTypeAuto"); m_namesMap->insert(C_exchangeSourceId, "exchangeSourceId"); m_namesMap->insert(C_exchangeTypeManual, "exchangeTypeManual"); m_namesMap->insert(C_exchangeRate, "exchangeRate"); m_namesMap->insert(C_backupEnable, "backupEnable"); m_namesMap->insert(C_backupImages, "backupImages"); m_namesMap->insert(C_backupTime, "backupTime"); m_namesMap->insert(C_smsConfigJson, "smsConfigJson"); m_namesMap->insert(C_smsProvider, "smsProvider"); m_namesMap->insert(C_smsAuthType, "smsAuthType"); m_namesMap->insert(C_smsApiId, "smsApiId"); m_namesMap->insert(C_smsLogin, "smsLogin"); m_namesMap->insert(C_smsPassword, "smsPassword"); m_namesMap->insert(C_smsSender, "smsSender"); m_namesMap->insert(C_emailConfigJson, "emailConfigJson"); m_namesMap->insert(C_emailServer, "emailServer"); m_namesMap->insert(C_emailPort, "emailPort"); m_namesMap->insert(C_emailLogin, "emailLogin"); m_namesMap->insert(C_emailPassword, "emailPassword"); m_namesMap->insert(C_emailTimeout, "emailTimeout"); m_namesMap->insert(C_emailEnableSsl, "emailEnableSsl"); m_namesMap->insert(C_emailEnableImplicitSsl, "emailEnableImplicitSsl"); m_namesMap->insert(C_emailTemplate, "emailTemplate");

    };
    void registerReportFields() final {


        registerReportField("id", WidgetTypes::NoWidget); registerReportField("currencyId", WidgetTypes::ComboBox); registerReportField("classicKassa", WidgetTypes::CheckBox); registerReportField("timeZoneId", WidgetTypes::ComboBox); registerReportField("ascPhoneMask1", WidgetTypes::LineEdit); registerReportField("ascPhoneMask2", WidgetTypes::LineEdit); registerReportField("updateChannel", WidgetTypes::LineEdit); registerReportField("isPriceColOptVisible", WidgetTypes::CheckBox); registerReportField("isPriceColOpt2Visible", WidgetTypes::CheckBox); registerReportField("isPriceColOpt3Visible", WidgetTypes::CheckBox); registerReportField("isPriceColRoznVisible", WidgetTypes::CheckBox); registerReportField("isPriceColServiceVisible", WidgetTypes::CheckBox); registerReportField("voipId", WidgetTypes::ComboBox); registerReportField("isRealizatorEnable", WidgetTypes::CheckBox); registerReportField("onlineStoreId", WidgetTypes::ComboBox); registerReportField("isCartridgeRepairEnabled", WidgetTypes::CheckBox); registerReportField("isEngineerRequiredOnDeviceRecept", WidgetTypes::CheckBox); registerReportField("isVisitSourceRequired", WidgetTypes::CheckBox); registerReportField("isSerialNumberRequired", WidgetTypes::CheckBox); registerReportField("isCartridgeSerialNumberRequired", WidgetTypes::CheckBox); registerReportField("isSearchOnlyBySerial", WidgetTypes::CheckBox); registerReportField("isPhotoOnReceptRequired", WidgetTypes::CheckBox); registerReportField("isPhotoOnIssueRequired", WidgetTypes::CheckBox); registerReportField("isDiagRequired", WidgetTypes::CheckBox); registerReportField("isVendorAddingAllowedOnRecept", WidgetTypes::CheckBox); registerReportField("isRepairSummSetByManager", WidgetTypes::CheckBox); registerReportField("isPaySalaryForRepairsIssuedInDebt", WidgetTypes::CheckBox); registerReportField("isAutoSetCompanyOnRepairRecept", WidgetTypes::CheckBox); registerReportField("isVendorWarrantyEnabled", WidgetTypes::CheckBox); registerReportField("useSimplifiedCartridgeRepair", WidgetTypes::CheckBox); registerReportField("autoCloseRepairTabTimeout", WidgetTypes::SpinBox); registerReportField("repairImagesLimit", WidgetTypes::SpinBox); registerReportField("repairDispatcherUsersJson", WidgetTypes::NoWidget); registerReportField("repairDispatcherAssignCriteriaJson", WidgetTypes::NoWidget); registerReportField("repairDispatcherUsers", WidgetTypes::SCheckableComboBox); registerReportField("repairDispatcherDayLimit", WidgetTypes::SpinBox); registerReportField("repairDispatcherStatuses", WidgetTypes::SCheckableComboBox); registerReportField("isClientPatronymicRequired", WidgetTypes::CheckBox); registerReportField("isClientEmailRequired", WidgetTypes::CheckBox); registerReportField("isClientAddressRequired", WidgetTypes::CheckBox); registerReportField("isClientPhoneRequired", WidgetTypes::CheckBox); registerReportField("isAnyClientDealer", WidgetTypes::CheckBox); registerReportField("timeoutForItemsRequestsHandling", WidgetTypes::SpinBox); registerReportField("timeoutForDiagnosisConfirmation", WidgetTypes::SpinBox); registerReportField("defaultItemReserveTime", WidgetTypes::SpinBox); registerReportField("itemImagesLimit", WidgetTypes::SpinBox); registerReportField("isReasonForItemIncomeRequired", WidgetTypes::CheckBox); registerReportField("defaultWorksWarranty", WidgetTypes::ComboBox); registerReportField("defaultNewItemsWarranty", WidgetTypes::ComboBox); registerReportField("defaultUsedItemsWarranty", WidgetTypes::ComboBox); registerReportField("defaultRefItemsWarranty", WidgetTypes::ComboBox); registerReportField("defaultDisasmItemsWarranty", WidgetTypes::ComboBox); registerReportField("defaultOtherItemsWarranty", WidgetTypes::ComboBox); registerReportField("notifyNewComment", WidgetTypes::CheckBox); registerReportField("notifyNewCommentColor", WidgetTypes::ComboBox); registerReportField("notifyRepairStatusUpdateColor", WidgetTypes::ComboBox); registerReportField("notifyIncomingSMS", WidgetTypes::CheckBox); registerReportField("notifyIncomingSMSColor", WidgetTypes::ComboBox); registerReportField("notifyOutOfTermAlarmColor", WidgetTypes::ComboBox); registerReportField("notifyDeviceMatch", WidgetTypes::CheckBox); registerReportField("notifyDeviceMatchColor", WidgetTypes::ComboBox); registerReportField("notifyCustomTask", WidgetTypes::CheckBox); registerReportField("notifyCustomTaskColor", WidgetTypes::ComboBox); registerReportField("notifyItemRequest", WidgetTypes::CheckBox); registerReportField("notifyItemRequestColor", WidgetTypes::ComboBox); registerReportField("notifyOrderFromOnlineStore", WidgetTypes::CheckBox); registerReportField("notifyOrderFromOnlineStoreColor", WidgetTypes::ComboBox); registerReportField("notifyIncomingCall", WidgetTypes::CheckBox); registerReportField("notifyIncomingCallColor", WidgetTypes::ComboBox); registerReportField("notifyItemPurchaseRequest", WidgetTypes::CheckBox); registerReportField("notifyItemPurchaseRequestColor", WidgetTypes::ComboBox); registerReportField("printPKO", WidgetTypes::CheckBox); registerReportField("printOutInvoice", WidgetTypes::CheckBox); registerReportField("printInInvoice", WidgetTypes::CheckBox); registerReportField("printRKO", WidgetTypes::CheckBox); registerReportField("printWarrantyDoc", WidgetTypes::CheckBox); registerReportField("printWorksList", WidgetTypes::CheckBox); registerReportField("printDiagResult", WidgetTypes::CheckBox); registerReportField("printRepairRejectDoc", WidgetTypes::CheckBox); registerReportField("printRKOOnItemIncome", WidgetTypes::CheckBox); registerReportField("printCheck", WidgetTypes::CheckBox); registerReportField("printRepairReceptDoc", WidgetTypes::CheckBox); registerReportField("printRepairStickers", WidgetTypes::CheckBox); registerReportField("printCartridgeReceptDoc", WidgetTypes::CheckBox); registerReportField("printCartridgeStickers", WidgetTypes::CheckBox); registerReportField("defaultRepairStickersQty", WidgetTypes::SpinBox); registerReportField("autoSwitchKeyboardLayout", WidgetTypes::CheckBox); registerReportField("voipAsteriskHost", WidgetTypes::LineEdit); registerReportField("voipAsteriskPort", WidgetTypes::SpinBox); registerReportField("voipAsteriskLogin", WidgetTypes::LineEdit); registerReportField("voipAsteriskPassword", WidgetTypes::LineEdit); registerReportField("voipAsteriskWebPort", WidgetTypes::SpinBox); registerReportField("voipPrefix", WidgetTypes::LineEdit); registerReportField("voipEndpoint", WidgetTypes::LineEdit); registerReportField("voipKey", WidgetTypes::LineEdit); registerReportField("voipSecret", WidgetTypes::LineEdit); registerReportField("onlineStoreUrl", WidgetTypes::LineEdit); registerReportField("onlineStoreKey", WidgetTypes::LineEdit); registerReportField("salaryClassic", WidgetTypes::CheckBox); registerReportField("salaryIncludeNotIssuedByDefault", WidgetTypes::CheckBox); registerReportField("newClientSmsEnabled", WidgetTypes::CheckBox); registerReportField("repairStatuses", WidgetTypes::NoWidget); registerReportField("exchangeTypeAuto", WidgetTypes::RadioButton); registerReportField("exchangeSourceId", WidgetTypes::ComboBox); registerReportField("exchangeTypeManual", WidgetTypes::RadioButton); registerReportField("exchangeRate", WidgetTypes::DoubleSpinBox); registerReportField("backupEnable", WidgetTypes::CheckBox); registerReportField("backupImages", WidgetTypes::CheckBox); registerReportField("backupTime", WidgetTypes::DateTimeEdit); registerReportField("smsConfigJson", WidgetTypes::NoWidget); registerReportField("smsProvider", WidgetTypes::ComboBox); registerReportField("smsAuthType", WidgetTypes::ComboBox); registerReportField("smsApiId", WidgetTypes::LineEdit); registerReportField("smsLogin", WidgetTypes::LineEdit); registerReportField("smsPassword", WidgetTypes::LineEdit); registerReportField("smsSender", WidgetTypes::LineEdit); registerReportField("emailConfigJson", WidgetTypes::NoWidget); registerReportField("emailServer", WidgetTypes::LineEdit); registerReportField("emailPort", WidgetTypes::SpinBox); registerReportField("emailLogin", WidgetTypes::LineEdit); registerReportField("emailPassword", WidgetTypes::LineEdit); registerReportField("emailTimeout", WidgetTypes::SpinBox); registerReportField("emailEnableSsl", WidgetTypes::CheckBox); registerReportField("emailEnableImplicitSsl", WidgetTypes::CheckBox); registerReportField("emailTemplate", WidgetTypes::LineEdit);
       
    };

public:
    void initWidgets() final {


        initWidget(C_id, tr("id"), WidgetTypes::NoWidget); initWidget(C_currencyId, tr("currencyId"), WidgetTypes::ComboBox, 0, QString()); initWidget(C_classicKassa, tr("classicKassa"), WidgetTypes::CheckBox, 0); initWidget(C_timeZoneId, tr("timeZoneId"), WidgetTypes::ComboBox, 0, QString()); initWidget(C_ascPhoneMask1, tr("ascPhoneMask1"), WidgetTypes::LineEdit, 0); initWidget(C_ascPhoneMask2, tr("ascPhoneMask2"), WidgetTypes::LineEdit, 0); initWidget(C_updateChannel, tr("updateChannel"), WidgetTypes::LineEdit, 0); initWidget(C_isPriceColOptVisible, tr("isPriceColOptVisible"), WidgetTypes::CheckBox, 1); initWidget(C_isPriceColOpt2Visible, tr("isPriceColOpt2Visible"), WidgetTypes::CheckBox, 1); initWidget(C_isPriceColOpt3Visible, tr("isPriceColOpt3Visible"), WidgetTypes::CheckBox, 1); initWidget(C_isPriceColRoznVisible, tr("isPriceColRoznVisible"), WidgetTypes::CheckBox, 1); initWidget(C_isPriceColServiceVisible, tr("isPriceColServiceVisible"), WidgetTypes::CheckBox, 1); initWidget(C_voipId, tr("voipId"), WidgetTypes::ComboBox, 2, QString()); initWidget(C_isRealizatorEnable, tr("isRealizatorEnable"), WidgetTypes::CheckBox, 2); initWidget(C_onlineStoreId, tr("onlineStoreId"), WidgetTypes::ComboBox, 2, QString()); initWidget(C_isCartridgeRepairEnabled, tr("isCartridgeRepairEnabled"), WidgetTypes::CheckBox, 2); initWidget(C_isEngineerRequiredOnDeviceRecept, tr("isEngineerRequiredOnDeviceRecept"), WidgetTypes::CheckBox, 3); initWidget(C_isVisitSourceRequired, tr("isVisitSourceRequired"), WidgetTypes::CheckBox, 3); initWidget(C_isSerialNumberRequired, tr("isSerialNumberRequired"), WidgetTypes::CheckBox, 3); initWidget(C_isCartridgeSerialNumberRequired, tr("isCartridgeSerialNumberRequired"), WidgetTypes::CheckBox, 3); initWidget(C_isSearchOnlyBySerial, tr("isSearchOnlyBySerial"), WidgetTypes::CheckBox, 3); initWidget(C_isPhotoOnReceptRequired, tr("isPhotoOnReceptRequired"), WidgetTypes::CheckBox, 3); initWidget(C_isPhotoOnIssueRequired, tr("isPhotoOnIssueRequired"), WidgetTypes::CheckBox, 3); initWidget(C_isDiagRequired, tr("isDiagRequired"), WidgetTypes::CheckBox, 3); initWidget(C_isVendorAddingAllowedOnRecept, tr("isVendorAddingAllowedOnRecept"), WidgetTypes::CheckBox, 3); initWidget(C_isRepairSummSetByManager, tr("isRepairSummSetByManager"), WidgetTypes::CheckBox, 3); initWidget(C_isPaySalaryForRepairsIssuedInDebt, tr("isPaySalaryForRepairsIssuedInDebt"), WidgetTypes::CheckBox, 3); initWidget(C_isAutoSetCompanyOnRepairRecept, tr("isAutoSetCompanyOnRepairRecept"), WidgetTypes::CheckBox, 3); initWidget(C_isVendorWarrantyEnabled, tr("isVendorWarrantyEnabled"), WidgetTypes::CheckBox, 3); initWidget(C_useSimplifiedCartridgeRepair, tr("useSimplifiedCartridgeRepair"), WidgetTypes::CheckBox, 3); initWidget(C_autoCloseRepairTabTimeout, tr("autoCloseRepairTabTimeout"), WidgetTypes::SpinBox, 3, 0, 32400); initWidget(C_repairImagesLimit, tr("repairImagesLimit"), WidgetTypes::SpinBox, 3, 0, 20); initWidget(C_repairDispatcherUsersJson, tr("repairDispatcherUsersJson"), WidgetTypes::NoWidget); initWidget(C_repairDispatcherAssignCriteriaJson, tr("repairDispatcherAssignCriteriaJson"), WidgetTypes::NoWidget); initWidget(C_repairDispatcherUsers, tr("repairDispatcherUsers"), WidgetTypes::SCheckableComboBox, 4); initWidget(C_repairDispatcherDayLimit, tr("repairDispatcherDayLimit"), WidgetTypes::SpinBox, 4, 0, 7); initWidget(C_repairDispatcherStatuses, tr("repairDispatcherStatuses"), WidgetTypes::SCheckableComboBox, 4); initWidget(C_isClientPatronymicRequired, tr("isClientPatronymicRequired"), WidgetTypes::CheckBox, 5); initWidget(C_isClientEmailRequired, tr("isClientEmailRequired"), WidgetTypes::CheckBox, 5); initWidget(C_isClientAddressRequired, tr("isClientAddressRequired"), WidgetTypes::CheckBox, 5); initWidget(C_isClientPhoneRequired, tr("isClientPhoneRequired"), WidgetTypes::CheckBox, 5); initWidget(C_isAnyClientDealer, tr("isAnyClientDealer"), WidgetTypes::CheckBox, 5); initWidget(C_timeoutForItemsRequestsHandling, tr("timeoutForItemsRequestsHandling"), WidgetTypes::SpinBox, 6, 0, 72); initWidget(C_timeoutForDiagnosisConfirmation, tr("timeoutForDiagnosisConfirmation"), WidgetTypes::SpinBox, 6, 0, 72); initWidget(C_defaultItemReserveTime, tr("defaultItemReserveTime"), WidgetTypes::SpinBox, 7, 0, 30); initWidget(C_itemImagesLimit, tr("itemImagesLimit"), WidgetTypes::SpinBox, 7, 0, 20); initWidget(C_isReasonForItemIncomeRequired, tr("isReasonForItemIncomeRequired"), WidgetTypes::CheckBox, 7); initWidget(C_defaultWorksWarranty, tr("defaultWorksWarranty"), WidgetTypes::ComboBox, 8, 31); initWidget(C_defaultNewItemsWarranty, tr("defaultNewItemsWarranty"), WidgetTypes::ComboBox, 8, 31); initWidget(C_defaultUsedItemsWarranty, tr("defaultUsedItemsWarranty"), WidgetTypes::ComboBox, 8, 7); initWidget(C_defaultRefItemsWarranty, tr("defaultRefItemsWarranty"), WidgetTypes::ComboBox, 8, 31); initWidget(C_defaultDisasmItemsWarranty, tr("defaultDisasmItemsWarranty"), WidgetTypes::ComboBox, 8, 7); initWidget(C_defaultOtherItemsWarranty, tr("defaultOtherItemsWarranty"), WidgetTypes::ComboBox, 8, 0); initWidget(C_notifyNewComment, tr("notifyNewComment"), WidgetTypes::CheckBox, 9); initWidget(C_notifyNewCommentColor, tr("notifyNewCommentColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyRepairStatusUpdateColor, tr("notifyRepairStatusUpdateColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyIncomingSMS, tr("notifyIncomingSMS"), WidgetTypes::CheckBox, 9); initWidget(C_notifyIncomingSMSColor, tr("notifyIncomingSMSColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyOutOfTermAlarmColor, tr("notifyOutOfTermAlarmColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyDeviceMatch, tr("notifyDeviceMatch"), WidgetTypes::CheckBox, 9); initWidget(C_notifyDeviceMatchColor, tr("notifyDeviceMatchColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyCustomTask, tr("notifyCustomTask"), WidgetTypes::CheckBox, 9); initWidget(C_notifyCustomTaskColor, tr("notifyCustomTaskColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyItemRequest, tr("notifyItemRequest"), WidgetTypes::CheckBox, 9); initWidget(C_notifyItemRequestColor, tr("notifyItemRequestColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyOrderFromOnlineStore, tr("notifyOrderFromOnlineStore"), WidgetTypes::CheckBox, 9); initWidget(C_notifyOrderFromOnlineStoreColor, tr("notifyOrderFromOnlineStoreColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyIncomingCall, tr("notifyIncomingCall"), WidgetTypes::CheckBox, 9); initWidget(C_notifyIncomingCallColor, tr("notifyIncomingCallColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_notifyItemPurchaseRequest, tr("notifyItemPurchaseRequest"), WidgetTypes::CheckBox, 9); initWidget(C_notifyItemPurchaseRequestColor, tr("notifyItemPurchaseRequestColor"), WidgetTypes::ComboBox, 9, QString("#FFFFFFFF")); initWidget(C_printPKO, tr("printPKO"), WidgetTypes::CheckBox, 10); initWidget(C_printOutInvoice, tr("printOutInvoice"), WidgetTypes::CheckBox, 10); initWidget(C_printInInvoice, tr("printInInvoice"), WidgetTypes::CheckBox, 10); initWidget(C_printRKO, tr("printRKO"), WidgetTypes::CheckBox, 10); initWidget(C_printWarrantyDoc, tr("printWarrantyDoc"), WidgetTypes::CheckBox, 10); initWidget(C_printWorksList, tr("printWorksList"), WidgetTypes::CheckBox, 10); initWidget(C_printDiagResult, tr("printDiagResult"), WidgetTypes::CheckBox, 10); initWidget(C_printRepairRejectDoc, tr("printRepairRejectDoc"), WidgetTypes::CheckBox, 10); initWidget(C_printRKOOnItemIncome, tr("printRKOOnItemIncome"), WidgetTypes::CheckBox, 10); initWidget(C_printCheck, tr("printCheck"), WidgetTypes::CheckBox, 10); initWidget(C_printRepairReceptDoc, tr("printRepairReceptDoc"), WidgetTypes::CheckBox, 10); initWidget(C_printRepairStickers, tr("printRepairStickers"), WidgetTypes::CheckBox, 10); initWidget(C_printCartridgeReceptDoc, tr("printCartridgeReceptDoc"), WidgetTypes::CheckBox, 10); initWidget(C_printCartridgeStickers, tr("printCartridgeStickers"), WidgetTypes::CheckBox, 10); initWidget(C_defaultRepairStickersQty, tr("defaultRepairStickersQty"), WidgetTypes::SpinBox, 10, 0, 10); initWidget(C_autoSwitchKeyboardLayout, tr("autoSwitchKeyboardLayout"), WidgetTypes::CheckBox, 11); initWidget(C_voipAsteriskHost, tr("voipAsteriskHost"), WidgetTypes::LineEdit, 12); initWidget(C_voipAsteriskPort, tr("voipAsteriskPort"), WidgetTypes::SpinBox, 12, 0, 65535); initWidget(C_voipAsteriskLogin, tr("voipAsteriskLogin"), WidgetTypes::LineEdit, 12); initWidget(C_voipAsteriskPassword, tr("voipAsteriskPassword"), WidgetTypes::LineEdit, 12); initWidget(C_voipAsteriskWebPort, tr("voipAsteriskWebPort"), WidgetTypes::SpinBox, 12, 0, 65535); initWidget(C_voipPrefix, tr("voipPrefix"), WidgetTypes::LineEdit, 12); initWidget(C_voipEndpoint, tr("voipEndpoint"), WidgetTypes::LineEdit, 12); initWidget(C_voipKey, tr("voipKey"), WidgetTypes::LineEdit, 12); initWidget(C_voipSecret, tr("voipSecret"), WidgetTypes::LineEdit, 12); initWidget(C_onlineStoreUrl, tr("onlineStoreUrl"), WidgetTypes::LineEdit, 13); initWidget(C_onlineStoreKey, tr("onlineStoreKey"), WidgetTypes::LineEdit, 13); initWidget(C_salaryClassic, tr("salaryClassic"), WidgetTypes::CheckBox, 14); initWidget(C_salaryIncludeNotIssuedByDefault, tr("salaryIncludeNotIssuedByDefault"), WidgetTypes::CheckBox, 14); initWidget(C_newClientSmsEnabled, tr("newClientSmsEnabled"), WidgetTypes::CheckBox, 14); initWidget(C_repairStatuses, tr("repairStatuses"), WidgetTypes::NoWidget); initWidget(C_exchangeTypeAuto, tr("exchangeTypeAuto"), WidgetTypes::RadioButton, 21); initWidget(C_exchangeSourceId, tr("exchangeSourceId"), WidgetTypes::ComboBox, 21, 1); initWidget(C_exchangeTypeManual, tr("exchangeTypeManual"), WidgetTypes::RadioButton, 21); initWidget(C_exchangeRate, tr("exchangeRate"), WidgetTypes::DoubleSpinBox, 21, 0.00, 9999.99); initWidget(C_backupEnable, tr("backupEnable"), WidgetTypes::CheckBox, 31); initWidget(C_backupImages, tr("backupImages"), WidgetTypes::CheckBox, 31); initWidget(C_backupTime, tr("backupTime"), WidgetTypes::DateTimeEdit, 31); initWidget(C_smsConfigJson, tr("smsConfigJson"), WidgetTypes::NoWidget); initWidget(C_smsProvider, tr("smsProvider"), WidgetTypes::ComboBox, 41, QString()); initWidget(C_smsAuthType, tr("smsAuthType"), WidgetTypes::ComboBox, 41, QString()); initWidget(C_smsApiId, tr("smsApiId"), WidgetTypes::LineEdit, 41); initWidget(C_smsLogin, tr("smsLogin"), WidgetTypes::LineEdit, 41); initWidget(C_smsPassword, tr("smsPassword"), WidgetTypes::LineEdit, 41); initWidget(C_smsSender, tr("smsSender"), WidgetTypes::LineEdit, 41); initWidget(C_emailConfigJson, tr("emailConfigJson"), WidgetTypes::NoWidget); initWidget(C_emailServer, tr("emailServer"), WidgetTypes::LineEdit, 41); initWidget(C_emailPort, tr("emailPort"), WidgetTypes::SpinBox, 41, 0, 65535); initWidget(C_emailLogin, tr("emailLogin"), WidgetTypes::LineEdit, 41); initWidget(C_emailPassword, tr("emailPassword"), WidgetTypes::LineEdit, 41); initWidget(C_emailTimeout, tr("emailTimeout"), WidgetTypes::SpinBox, 41, 0, 300); initWidget(C_emailEnableSsl, tr("emailEnableSsl"), WidgetTypes::CheckBox, 41); initWidget(C_emailEnableImplicitSsl, tr("emailEnableImplicitSsl"), WidgetTypes::CheckBox, 41); initWidget(C_emailTemplate, tr("emailTemplate"), WidgetTypes::LineEdit, 41);

        configureWidgets();
        setWidgetsData();
    };
public:
    void load() override;
    void save() override;
private:
    std::unique_ptr<QMap<int, int>> m_classDbMap;
    std::shared_ptr<QSqlRecord> m_record_settings;
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
