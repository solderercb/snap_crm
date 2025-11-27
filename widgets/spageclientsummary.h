#ifndef SPAGECLIENTSUMMARY_H
#define SPAGECLIENTSUMMARY_H

#include <STabPage>

class QWidget;
class SClientModel;
class SPhonesModel;

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
protected:
    void randomFill();
private:
    Ui::SPageClientSummary *ui;
    SClientModel *m_client;
    SPhonesModel *m_phones;
    void fillBanks();
protected slots:
#ifdef QT_DEBUG
    void test_scheduler_handler() ;
    void test_scheduler2_handler() ;
#endif
};

#endif // SPAGECLIENTSUMMARY_H
