#include "stechreportmodel.h"

STechReportModel::STechReportModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "created" << "user" << "company" << "client" << "device" << "diagnostic_result" << "conclusion" << "notes";
    i_tableName = "tech_reports";
    i_idColumnName = "id";
}

STechReportModel::~STechReportModel()
{

}

void STechReportModel::load(const int &id)
{
    QSqlQuery *record = new QSqlQuery(QSqlDatabase::database("connMain"));
    record->exec(QString("SELECT `id`, `num`, `created`, `user`, `company`, `client`, `device`, `inventory_number`, `serial_number`, CONVERT(`production_date`, CHAR) AS 'production_date', CONVERT(`purchase_date`, CHAR) AS 'purchase_date', `initial_cost`, `residual_cost`, `fault`, `diagnostic_result`, `conclusion`, `notes` FROM `tech_reports` WHERE `id` = %1;").arg(id));

    if(record->first())
    {
        QString productionDate = record->value("production_date").toString();
        QString purchaseDate = record->value("purchase_date").toString();

        i_id = id;
        i_valuesMap.clear();
        m_num = record->value("num").toString();
        i_createdUtc = record->value("created").toDateTime();
        m_user = record->value("user").toInt();
        m_company = record->value("company").toInt();
        m_client = record->value("client").toInt();
        m_device = record->value("device").toString();
        m_inventoryNumber = record->value("inventory_number").toString();
        m_serialNumber = record->value("serial_number").toString();
        m_productionYear = yearFromString(productionDate);
        m_productionMonth = monthFromString(productionDate);
        m_productionDay = dayFromString(productionDate);
        m_purchaseYear = yearFromString(purchaseDate);
        m_purchaseMonth = monthFromString(purchaseDate);
        m_purchaseDay = dayFromString(purchaseDate);
        m_initialCost = record->value("initial_cost").toDouble();
        m_residualCost = record->value("residual_cost").toDouble();
        m_fault = record->value("fault").toString();
        m_diagnosticResult = record->value("diagnostic_result").toString();
        m_conclusion = record->value("conclusion").toString();
        m_notes = record->value("notes").toString();

        m_clientModel = new SClientModel(m_client, this);

        fillEmptyField(m_serialNumber);
        fillEmptyField(m_inventoryNumber);
    }

    delete record;
}

const int& STechReportModel::id()
{
    return i_id;
}

const QString& STechReportModel::num()
{
    return m_num;
}

void STechReportModel::setNum(const QString &num)
{
    i_valuesMap.insert("num", num);
}

QString STechReportModel::created()
{
    return localDateTime(createdUtc()).toString("yyyy-MM-dd hh:mm:ss");
}

const int& STechReportModel::user()
{
    return m_user;
}

void STechReportModel::setUser(const int &user)
{
    i_valuesMap.insert("user", user);
}

const int& STechReportModel::company()
{
    return m_company;
}

void STechReportModel::setCompany(const int &company)
{
    i_valuesMap.insert("company", company);
}

const QString STechReportModel::client()
{
    return m_clientModel->fullLongName();
}

const int& STechReportModel::clientId()
{
    return m_client;
}

void STechReportModel::setClientId(const int &client)
{
    i_valuesMap.insert("client", client);
}

const QString& STechReportModel::device()
{
    return m_device;
}

void STechReportModel::setDevice(const QString &device)
{
    i_valuesMap.insert("device", device);
}

const QString& STechReportModel::inventoryNumber()
{
    return m_inventoryNumber;
}

void STechReportModel::setInventoryNumber(const QString &inventory_number)
{
    i_valuesMap.insert("inventory_number", inventory_number);
}

const QString& STechReportModel::serialNumber()
{
    return m_serialNumber;
}

void STechReportModel::setSerialNumber(const QString &serial_number)
{
    i_valuesMap.insert("serial_number", serial_number);
}

void STechReportModel::setProductionDate(const QDate &production_date)
{
    i_valuesMap.insert("production_date", production_date);
}

void STechReportModel::setPurchaseDate(const QDate &purchase_date)
{
    i_valuesMap.insert("purchase_date", purchase_date);
}

const double& STechReportModel::initialCost()
{
    return m_initialCost;
}

void STechReportModel::setInitialCost(const double &initial_cost)
{
    i_valuesMap.insert("initial_cost", initial_cost);
}

const double& STechReportModel::residualCost()
{
    return m_residualCost;
}

void STechReportModel::setResidualCost(const double &residual_cost)
{
    i_valuesMap.insert("residual_cost", residual_cost);
}

const QString& STechReportModel::fault()
{
    return m_fault;
}

void STechReportModel::setFault(const QString &text)
{
    i_valuesMap.insert("fault", text);
}

const QString& STechReportModel::diagnosticResult()
{
    return m_diagnosticResult;
}

void STechReportModel::setDiagnosticResult(const QString &text)
{
    i_valuesMap.insert("diagnostic_result", text);
}

const QString& STechReportModel::conclusion()
{
    return m_conclusion;
}

void STechReportModel::setConclusion(const QString &text)
{
    i_valuesMap.insert("conclusion", text);
}

const QString &STechReportModel::notes()
{
    return m_notes;
}

void STechReportModel::setNotes(const QString &text)
{
    i_valuesMap.insert("notes", text);
}

void STechReportModel::initDemo()
{
    QString productionDate = "2008-00-00";
    QString purchaseDate = "2008-04-01";

    i_id = 123;
    m_num = "74";
    i_createdUtc = QDateTime::fromString("2024.01.05 12:00:00", "yyyy.MM.dd hh:mm:ss");
    m_user = 1;
    m_company = 1;
    m_client = 456;
    m_device = "Кондиционер Аkira";
    m_inventoryNumber = "212.72.20941032";
    m_serialNumber = "SN7890AA24";
    m_productionYear = yearFromString(productionDate);
    m_productionMonth = monthFromString(productionDate);
    m_productionDay = dayFromString(productionDate);
    m_purchaseYear = yearFromString(purchaseDate);
    m_purchaseMonth = monthFromString(purchaseDate);
    m_purchaseDay = dayFromString(purchaseDate);
    m_initialCost = 0;
    m_residualCost = 1900;
    m_fault = "Не охлаждает";
    m_diagnosticResult = "Узлы устройства находятся в исправном состоянии, внешних повреждений при обследовании не обнаружено; в охлаждающем контуре отсутствует хладагент. В процессе обследования производился внешний визуальный осмотр, съем защитных кожухов, исследование внутренних элементов, а также проверка его работы.";
    m_conclusion = "Для восстановления работоспособности устройства требуется заправка хладагентом Жы-123; рекомендуется заменить теплоизоляцию на наружных трубках охлаждающего контура. Требуется очистка радиатора внешнего блока от пыли/грязи.";
    m_notes = "Проверка герметичности охлаждающего контура не производилась. В случае нарушений герметичности рекомендуется заменить все соединительные трубки между внешним и внутренним блоками.";

    m_clientModel = new SClientModel();
    m_clientModel->setParent(this);
    m_clientModel->initDemo();
}

void STechReportModel::fillEmptyField(QString &field)
{
    if(field.isEmpty())
        field = "—";
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

QString STechReportModel::productionDate()
{
    return dateToString(m_productionYear, m_productionMonth, m_productionDay);
}

QString STechReportModel::purchaseDate()
{
    return dateToString(m_purchaseYear, m_purchaseMonth, m_purchaseDay);
}

