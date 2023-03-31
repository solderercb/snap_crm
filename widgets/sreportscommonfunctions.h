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


class SReportsCommonFunctions
{
public:
    SReportsCommonFunctions();
    QString hash(QByteArray *data);
protected:
    int m_reportType;
    QString m_reportName;
    LimeReport::ReportEngine *m_report;
    QMap<QString, QVariant> m_reportVars;
    QString m_templatesDir;
    QFile CurrentFile;
    QStringList m_reportDatasouces;
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
private:
    QStandardItemModel* initDemoModel(const QStringList &demoHeaders, const QList<QVariant> &demoValues);
};

#endif // SREPORTSCOMMONFUNCTIONS_H
