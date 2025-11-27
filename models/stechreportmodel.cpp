#include "stechreportmodel.h"
#include <SClientModel>

const QRegularExpression STechReportModel::validDateFormat = QRegularExpression("\\d{4}-\\d{2}-\\d{2}");

STechReportModel::STechReportModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "created" << "user" << "company" << "client" << "device" << "diagnostic_result" << "conclusion" << "notes";
    i_tableName = "tech_reports";
    setPrimaryKeyIndex(0);
    connect(this, &SSingleRowModel::beginDataChange, this, &STechReportModel::setDataRework);
}

STechReportModel::~STechReportModel()
{
}

QString STechReportModel::constructSelectQuery()
{
    return QString("SELECT `id`, `num`, `created`, `user`, `company`, `client`, `device`, `inventory_number`, `serial_number`, CONVERT(`production_date`, CHAR) AS 'production_date', CONVERT(`purchase_date`, CHAR) AS 'purchase_date', `initial_cost`, `residual_cost`, `fault`, `diagnostic_result`, `conclusion`, `notes` FROM `tech_reports` WHERE `id` = %1;").arg(id());
}

void STechReportModel::load(const int &id)
{
    setPrimaryKey(id);
    SSingleRowJModel::load();

    m_clientModel = std::make_unique<SClientModel>(clientId(), this);
    fillEmptyField(C_serialNumber);
    fillEmptyField(C_inventoryNumber);
}

void STechReportModel::initDemo()
{
    m_record->setValue(C_id, 123);
    m_record->setValue(C_num, "74");
    m_record->setValue(C_created, QDateTime(QDate(2025,8,10), QTime(21,50,00)));
    m_record->setValue(C_user, 1);
    m_record->setValue(C_company, 1);
    m_record->setValue(C_clientId, 456);
    m_record->setValue(C_device, "Кондиционер Аkira");
    m_record->setValue(C_inventoryNumber, "212.72.20941032");
    m_record->setValue(C_serialNumber, "SN7890AA24");
    m_record->setValue(C_produced, "2008-00-00");
    m_record->setValue(C_purchased, "2008-04-01");
    m_record->setValue(C_initialCost, 0);
    m_record->setValue(C_residualCost, 1900);
    m_record->setValue(C_fault, "Не охлаждает");
    m_record->setValue(C_diagnosticResult, "Узлы устройства находятся в исправном состоянии, внешних повреждений при обследовании не обнаружено; в охлаждающем контуре отсутствует хладагент. В процессе обследования производился внешний визуальный осмотр, съем защитных кожухов, исследование внутренних элементов, а также проверка его работы.");
    m_record->setValue(C_conclusion, "Для восстановления работоспособности устройства требуется заправка хладагентом Жы-123; рекомендуется заменить теплоизоляцию на наружных трубках охлаждающего контура. Требуется очистка радиатора внешнего блока от пыли/грязи.");
    m_record->setValue(C_notes, "Проверка герметичности охлаждающего контура не производилась. В случае нарушений герметичности рекомендуется заменить все соединительные трубки между внешним и внутренним блоками.");

    m_clientModel = std::make_unique<SClientModel>();
    m_clientModel->initDemo();

    auto tmp = data(C_produced);
    setDateHandler(tmp);
}

void STechReportModel::fillEmptyField(const int index)
{
    auto value = m_record->value(index).toString();
    if(value.isEmpty())
        m_record->setValue(index, "—");
}

/* Форматирование строки даты производства/приобретения
*/
QString STechReportModel::dateToString(const int &year, const int &month, const int &day)
{
    if(!year)
        return "—";

    QString ret = QString::number(year);
    if(month)
        ret = QString::number(month).rightJustified(2, '0') + "." + ret;
    if(day)
        ret = QString::number(day).rightJustified(2, '0') + "." + ret;

    return ret;
}

/* Извлечение года, месяца и дня из даты в виде строки, отформатированной по правилу yyyy-MM-dd
 * Класс QDate не допускает ноль в поле месяца и дня, MySQL же допускает такие значения
*/
int STechReportModel::yearFromString(const QString &date)
{
    return date.leftRef(4).toInt();
}

int STechReportModel::monthFromString(const QString &date)
{
    return date.midRef(5, 2).toInt();
}

int STechReportModel::dayFromString(const QString &date)
{
    return date.midRef(8, 2).toInt();
}

QVariant STechReportModel::client()
{
    return m_clientModel->fullLongName();
}

QVariant STechReportModel::productionDate()
{
    auto d = produced();
    return dateToString(yearFromString(d), monthFromString(d), dayFromString(d));
}

QVariant STechReportModel::purchaseDate()
{
    auto d = purchased();
    return dateToString(yearFromString(d), monthFromString(d), dayFromString(d));
}

void STechReportModel::setDateHandler(QVariant &data)
{
    // защита от дурака
    Q_ASSERT_X(data.toString().contains(validDateFormat), "Wrong format", "date must be passed in yyyy-mm-dd format");
}

void STechReportModel::setDataRework(const int index, QVariant &data)
{
    switch (index)
    {
        case C_produced:
        case C_purchased: setDateHandler(data); break;
        default: break;
    }
}

