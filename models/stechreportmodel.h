#ifndef STECHREPORTMODEL_H
#define STECHREPORTMODEL_H

#include "scomrecord.h"
#include <QObject>
#include "models/sclientmodel.h"

class STechReportModel : public SComRecord
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString num READ num)
    Q_PROPERTY(QString created READ created)
    Q_PROPERTY(int user READ user)
    Q_PROPERTY(int company READ company)
    Q_PROPERTY(QString client READ client)
    Q_PROPERTY(QString device READ device)
    Q_PROPERTY(QString inventoryNumber READ inventoryNumber)
    Q_PROPERTY(QString serialNumber READ serialNumber)
    Q_PROPERTY(QString productionDate READ productionDate)
    Q_PROPERTY(QString purchaseDate READ purchaseDate)
    Q_PROPERTY(double initialCost READ initialCost)
    Q_PROPERTY(double residualCost READ residualCost)
    Q_PROPERTY(QString fault READ fault)
    Q_PROPERTY(QString diagnosticResult READ diagnosticResult)
    Q_PROPERTY(QString conclusion READ conclusion)
    Q_PROPERTY(QString notes READ notes)
public:
    explicit STechReportModel(QObject *parent = nullptr);
    ~STechReportModel();
    void load(const int &id);
    const int& id();
    const QString& num();
    void setNum(const QString &num);
    QString created();
    const int& user();
    void setUser(const int &user);
    const int& company();
    void setCompany(const int &company);
    const int& clientId();
    void setClientId(const int &client);
    const QString client();
    const QString& device();
    void setDevice(const QString &device);
    const QString& inventoryNumber();
    void setInventoryNumber(const QString &inventory_number);
    const QString& serialNumber();
    void setSerialNumber(const QString &serial_number);
    void setProductionDate(const QDate &production_date);
    void setPurchaseDate(const QDate &purchase_date);
    const double& initialCost();
    void setInitialCost(const double &initial_cost);
    const double& residualCost();
    void setResidualCost(const double &residual_cost);
    const QString& fault();
    void setFault(const QString &text);
    const QString& diagnosticResult();
    void setDiagnosticResult(const QString &text);
    const QString& conclusion();
    void setConclusion(const QString &text);
    const QString& notes();
    void setNotes(const QString &text);
    void initDemo() override;
private:
    QString m_num;
    int m_user;
    int m_company;
    int m_client;
    QString m_device;
    QString m_inventoryNumber;
    QString m_serialNumber;
    int m_productionYear, m_productionMonth, m_productionDay;
    int m_purchaseYear, m_purchaseMonth, m_purchaseDay;
    double m_initialCost;
    double m_residualCost;
    QString m_fault;
    QString m_diagnosticResult;
    QString m_conclusion;
    QString m_notes;
    SClientModel *m_clientModel;
    void fillEmptyField(QString &field);
    QString dateToString(const int &year, const int &month, const int &day);
    int yearFromString(const QString &date);
    int monthFromString(const QString &date);
    int dayFromString(const QString &date);
    QString productionDate();
    QString purchaseDate();
    QString reportInitialCost();
    QString reportResidualCost();
};

#endif // STECHREPORTMODEL_H
