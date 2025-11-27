#ifndef SFIELDVALUEMODEL_H
#define SFIELDVALUEMODEL_H

#include <SSingleRowJModel>
#include <QSqlRecord>
#include <SLogRecordModel>

class SFieldModel;
class FieldFactory;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 0)                                             \
    TABLE_FIELD(field_id, fieldId, int, 0)                                  \
    TABLE_FIELD(repair_id, repairId, int, 0)                                \
    TABLE_FIELD(item_id, itemId, int, 0)                                    \
    TABLE_FIELD(value, value, QString, 0)

class SFieldValueModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class TClassTest;
    friend class SFieldsModel;
    friend class FieldFactory;
public:
    explicit SFieldValueModel(QObject *parent = nullptr);
    explicit SFieldValueModel(const int, QObject *parent = nullptr);
    explicit SFieldValueModel(std::shared_ptr<SFieldModel> field, QObject *parent = nullptr);
    ~SFieldValueModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void setHandler(std::unique_ptr<FieldFactory> owner);
    void load(const int);
    void load(const QSqlRecord &record);
    QString name();
    bool isPrintable();
    bool delDBRecord();
#ifdef QT_DEBUG
    void randomFill();
#endif
private:
    std::shared_ptr<SFieldModel> m_entityModel;
    std::unique_ptr<FieldFactory> m_field;
    QString constructSelectQuery() override;
    bool acquireWidgetValue();
    void enableEdit(std::shared_ptr<SFieldModel> entityModel);
    void setOwnerId(const int id);
    void valueChanged(const QVariant &data);
    void updateLogAssociatedRecId() override;
#ifdef QT_DEBUG
    void randomLineEditText();
    void randomComboBoxIndex();
    void randomDateEditValue();
#endif
protected slots:
    void logDataChange(const int index, const QVariant &data) override;
};


// Классы для реализации паттернов Фабрика/Стратегия
// Реализация два-в-одном, чтобы всё было в одном месте
class FieldFactory {
public:
    enum Type {Item = 0, Repair = 1};
    virtual ~FieldFactory() = default;
    virtual void initFieldHandler(std::shared_ptr<SFieldValueModel> model) = 0;
    virtual void setId(const int id)  = 0;
    virtual void updateLogRecord() = 0;
    virtual int id() = 0;
    virtual int type() = 0;
    virtual QString query(const int id) = 0;
    inline SLogRecordModel* logRecord(){if(auto f = fieldValue.lock()) return f->i_logRecord.get(); return nullptr;}; // Такая реализация потому, что i_logRecord protected. Вроде, не должно вызвать проблем
    std::weak_ptr<SFieldValueModel> fieldValue;
};

// ВНИМАНИЕ! В режиме пошаговой отладки в инспекторе локальных переменных по неизвестной причине указатель f отображается
// с некорректным адресом (смещен на несколько байт) если тело метода записано в одну строку.
// С причиной не разбирался, но на это точно не влияют inline и auto. Не смотря на это отрабатывает вроде всё правильно.
class SRepairField : public FieldFactory {
public:
    void initFieldHandler(std::shared_ptr<SFieldValueModel> model) override {model->setHandler(std::make_unique<SRepairField>(model));};
    SRepairField(){};
    explicit SRepairField(std::shared_ptr<SFieldValueModel> model){fieldValue = model;};
    inline void setId(const int id) override {if(auto f = fieldValue.lock()) f->set_repairId(id);};
    inline void updateLogRecord() override {if(auto l = logRecord()) l->set_repair(fieldValue.lock()->repairId());};
    inline int id() override {if(auto f = fieldValue.lock()) return f->repairId(); return 0;};
    int type() override {return Type::Repair;};
    QString query(const int id) override;
};
class SItemField : public FieldFactory {
public:
    void initFieldHandler(std::shared_ptr<SFieldValueModel> model) override {model->setHandler(std::make_unique<SItemField>(model));};
    SItemField(){};
    explicit SItemField(std::shared_ptr<SFieldValueModel> model){fieldValue = model;};
    inline void setId(const int id) override {if(auto f = fieldValue.lock()) f->set_itemId(id);};
    inline void updateLogRecord() override {if(auto l = logRecord()) l->set_item(fieldValue.lock()->itemId());};
    inline int id() override {if(auto f = fieldValue.lock()) return f->itemId(); return 0;};
    int type() override {return Type::Item;};
    QString query(const int id) override;
};

#endif // SFIELDVALUEMODEL_H
