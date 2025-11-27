#include "sfieldvaluemodel.h"
#include <ProjectQueries>
#include <QRandomGenerator>
#include <QLineEdit>
#include <QComboBox>
#include <SDateEdit>
#include <SFieldModel>

SFieldValueModel::SFieldValueModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "field_id" << "value";
    i_tableName = "field_values";
    setPrimaryKeyIndex(0);
}

SFieldValueModel::SFieldValueModel(const int id, QObject *parent) : SFieldValueModel(parent)
{
    load(id);
}

SFieldValueModel::SFieldValueModel(std::shared_ptr<SFieldModel> field, QObject *parent) : SFieldValueModel(parent)
{
    m_entityModel = field;
}

SFieldValueModel::~SFieldValueModel()
{
}

QString SFieldValueModel::constructSelectQuery()
{
    return QUERY_SEL_ADD_FIELD(id());
}

void SFieldValueModel::load(const int id)
{
    setPrimaryKey(id);
    SSingleRowJModel::load();
}

void SFieldValueModel::load(const QSqlRecord &record)
{
    m_record = std::make_unique<QSqlRecord>(record);
}

QString SFieldValueModel::name()
{
    if(m_entityModel.get() != nullptr)
        return m_entityModel->name();
    if(isModelLoaded())
        return m_record->value("name").toString();

    Q_ASSERT_X(0, "Can't get field name", "model is neither loaded nor in edit state");
    return QString();
}

bool SFieldValueModel::isPrintable()
{
    if(m_entityModel.get() != nullptr)
        return m_entityModel->printable();
    if(isModelLoaded())
        return m_record->value("printable").toBool();

    Q_ASSERT_X(0, "Can't get field property \"isPrintable\"", "model is neither loaded nor in edit state");
    return false;
}

void SFieldValueModel::valueChanged(const QVariant &data)
{
    if(isPrimaryKeyValid())
    {
        auto old = commitedData(C_value).value_or(QVariant());
        appendLogText(tr("Значение параметра %1 изменено с \"%2\" на \"%3\"").arg(name(), old.toString(), data.toString()), "notes");
    }
    else
        set_fieldId(m_entityModel->id());
}

/* Сохранение текущего значения виджета в поле value
 * Возвращает 1, если значение виджета не пустое
*/
bool SFieldValueModel::acquireWidgetValue()
{
    Q_ASSERT_X(m_entityModel.get() != nullptr, "Can't acquire widget text", "model is not in edit state");

    QString value;

    switch (m_entityModel->type())
    {
        case SFieldModel::WidgetType::ComboBox: value = m_entityModel->comboBox()->currentText(); break;
        case SFieldModel::WidgetType::DateEdit: value = m_entityModel->dateEdit()->date().toString("yyyy-MM-dd"); break;
        case SFieldModel::WidgetType::LineEdit:
        default:                                value = m_entityModel->lineEdit()->text();
    }


    if(value.isEmpty())
    {
        return 0;
    }

    set_value(value);

    return 1;
}

void SFieldValueModel::enableEdit(std::shared_ptr<SFieldModel> entityModel)
{
    m_entityModel = entityModel;
    m_field->updateLogRecord();
}

void SFieldValueModel::setHandler(std::unique_ptr<FieldFactory> handler)
{
    m_field = std::move(handler);
}

void SFieldValueModel::setOwnerId(const int id)
{
    m_field->setId(id);
    m_field->updateLogRecord();
}

bool SFieldValueModel::delDBRecord()
{
    auto old = commitedData(C_value).value_or(QVariant());
    i_logRecord->set_text(tr("Параметр %1 (\"%2\") удалён").arg(name(), old.toString()));

    i_logRecord->commit();
    return SSingleRowJModel::del();
}

#ifdef QT_DEBUG
void SFieldValueModel::randomFill()
{
    switch (m_entityModel->type())
    {
        case SFieldModel::WidgetType::ComboBox: randomComboBoxIndex(); break;
        case SFieldModel::WidgetType::DateEdit: randomDateEditValue(); break;
        case SFieldModel::WidgetType::LineEdit:
        default: randomLineEditText();
    }
}

/*  Случайное число
*/
void SFieldValueModel::randomLineEditText()
{
    m_entityModel->lineEdit()->setText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
}

/*  Выбирает случайный индекс модели данных
*/
void SFieldValueModel::randomComboBoxIndex()
{
    int i = m_entityModel->comboBox()->model()->rowCount();
    m_entityModel->comboBox()->setCurrentIndex(QRandomGenerator::global()->bounded(i));
}

/*  Прибавляет к текущей дате случайное число от 0 до 90
*/
void SFieldValueModel::randomDateEditValue()
{
    m_entityModel->dateEdit()->setDate(QDate::fromJulianDay(QDate::currentDate().toJulianDay() + QRandomGenerator::global()->bounded(90)));
}
#endif

void SFieldValueModel::logDataChange(const int index, const QVariant &data)
{
    switch (index)
    {
        case C_repairId: i_logRecord->set_repair(data.toInt()); break;
        case C_itemId: i_logRecord->set_item(data.toInt()); break;
        case C_value: valueChanged(data); break;
        default: break;
    }
}

void SFieldValueModel::updateLogAssociatedRecId()
{
}

QString SRepairField::query(const int id)
{
    return QUERY_SEL_REPAIR_ADD_FIELDS(id);
}

QString SItemField::query(const int id)
{
    return QUERY_SEL_ITEM_ADD_FIELDS(id);
}
