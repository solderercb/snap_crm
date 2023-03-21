#ifndef SPAGECLIENTSUMMARY_H
#define SPAGECLIENTSUMMARY_H

#include <QWidget>
#include "models/sclientmodel.h"
#include "widgets/stabpage.h"

namespace Ui {
class SPageClientSummary;
}

class SPageClientSummary : public STabPage
{
    Q_OBJECT
public:
    explicit SPageClientSummary(QWidget *parent = nullptr);
    explicit SPageClientSummary(int id, QWidget *parent = nullptr);
    ~SPageClientSummary();
    SClientModel* model();

private:
    Ui::SPageClientSummary *ui;
    SClientModel *m_client;
    SPhonesModel *m_phones;
    void fillBanks();
    void fillBinaryProperties();
//    void setCursorPositionsToZero();
};

#endif // SPAGECLIENTSUMMARY_H
