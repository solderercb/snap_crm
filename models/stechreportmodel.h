#ifndef STECHREPORTMODEL_H
#define STECHREPORTMODEL_H

#include <SSingleRowJModel>

class SClientModel;

#include "ssinglerowmodel_predef.h"     // этот файл нужно подключать после ssinglerowmodel.h и до списка элементов
#define TABLE_FIELDS                                                        \
    TABLE_FIELD(id, id, int, 1)                                             \
    TABLE_FIELD(num, num, QString, 1)                                       \
    TABLE_FIELD(created, created, QDateTime, 1)                             \
    TABLE_FIELD(user, user, int, 1)                                         \
    TABLE_FIELD(company, company, int, 1)                                   \
    TABLE_FIELD(client, clientId, int, 1)                                   \
    TABLE_FIELD(device, device, QString, 1)                                 \
    TABLE_FIELD(inventory_number, inventoryNumber, QString, 1)              \
    TABLE_FIELD(serial_number, serialNumber, QString, 1)                    \
    TABLE_FIELD(production_date, produced, QString, 0)                      \
    TABLE_FIELD(purchase_date, purchased, QString, 0)                       \
    TABLE_FIELD(initial_cost, initialCost, double, 1)                       \
    TABLE_FIELD(residual_cost, residualCost, double, 1)                     \
    TABLE_FIELD(fault, fault, QString, 1)                                   \
    TABLE_FIELD(diagnostic_result, diagnosticResult, QString, 1)            \
    TABLE_FIELD(conclusion, conclusion, QString, 1)                         \
    TABLE_FIELD(notes, notes, QString, 1)

// Список дополнительных полей для отчетов
#undef ADDITIONAL_REPORT_FIELDS
#define ADDITIONAL_REPORT_FIELDS                                            \
    ADDITIONAL_REPORT_FIELD(client, client)                                 \
    ADDITIONAL_REPORT_FIELD(productionDate, productionDate)                 \
    ADDITIONAL_REPORT_FIELD(purchaseDate, purchaseDate)

class STechReportModel : public SSingleRowJModel
{
    Q_OBJECT
    friend class STableTechReportsModel;
    friend class SListSourceDataCallbackHandler;
public:
    explicit STechReportModel(QObject *parent = nullptr);
    ~STechReportModel();
#include "ssinglerowmodel_init.h"     // этот файл нужно подключать именно здесь
public:
    void load(const int &id);
    void initDemo() override;
private:
    std::unique_ptr<SClientModel> m_clientModel;
    static const QRegularExpression validDateFormat;
    QString constructSelectQuery() override;
    void fillEmptyField(const int index);
    void setDateHandler(QVariant &data);
    static QString dateToString(const int &year, const int &month, const int &day);
    static int yearFromString(const QString &date);
    static int monthFromString(const QString &date);
    static int dayFromString(const QString &date);
    QString reportInitialCost();
    QString reportResidualCost();
public Q_SLOTS:
    QVariant client();
    QVariant productionDate();
    QVariant purchaseDate();
private slots:
    void setDataRework(const int index, QVariant &data);
};

#endif // STECHREPORTMODEL_H
