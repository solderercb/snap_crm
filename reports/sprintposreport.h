#ifndef SPRINTPOSREPORT_H
#define SPRINTPOSREPORT_H

#include <QObject>
#include <QLocale>
#include "global.h"
#include "models/sclientmodel.h"
#include "models/srepairmodel.h"
#include "escposprinter.h"

class SPrintPOSReport : public QObject
{
    Q_OBJECT
public:
    explicit SPrintPOSReport();
    ~SPrintPOSReport();
    void setClientModel(SClientModel *client);
    void addPrintJob(SRepairModel *repair);
    void openPrinter(const QString &name);
    void closePrinter();
private:
    EscPosQt::EscPosPrinter *printer = nullptr;
    SClientModel *m_client = nullptr;
    QLocale locale;
};

#endif // SPRINTPOSREPORT_H
