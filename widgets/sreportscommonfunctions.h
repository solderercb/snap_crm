#ifndef SREPORTSCOMMONFUNCTIONS_H
#define SREPORTSCOMMONFUNCTIONS_H

#include <LimeReport>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QByteArray>
#include <QVariant>
#include <QCryptographicHash>
#include "models/ssqlquerymodel.h"
#include "models/sclientmodel.h"
#include "models/srepairmodel.h"
#include "models/sphonesmodel.h"
#include "models/sfieldsmodel.h"
#include "models/ssaletablemodel.h"
#include "models/stablerepairsmodel.h"

class SReportsCommonFunctions
{
public:
    SReportsCommonFunctions();
    ~SReportsCommonFunctions();
    QString hash(QByteArray *data);
    void addDataModel(void *p);
protected:
    int m_reportType;
    QString m_reportName;
    LimeReport::ReportEngine *m_report;
    QMap<QString, QVariant> m_reportVars;
    QString m_templatesDir;
    QFile CurrentFile;
    QStringList m_reportDatasouces;
    STableRepairsModel* i_repairsTable = nullptr;
    void setTemplateName(const QString &reportName);
    QByteArray* readFile(QFile &file);
    bool writeFile(QFile &file, QByteArray *data = nullptr);
    bool loadTemplateFromFile();
    bool loadTemplateFromDB();
    void initDataSources();
    virtual void initRepairDataSources();
    virtual void initRepairStickerDataSources();
    virtual void initWorksDataSources();
    virtual void initItemStickerDataSources();
    virtual void initPKODataSources();
    virtual void initRepairsDataSources();
    virtual void notImplementedReport(){};
private:
    STableRepairsModel *m_repairsDemoModel = nullptr;
    QList<void*> m_extDataModels;
    QStandardItemModel *initDemoModel(const QStringList &demoHeaders, const QList<QVariant> &demoValues);
};

#endif // SREPORTSCOMMONFUNCTIONS_H
