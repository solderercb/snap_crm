#ifndef TABCLIENTSUMMARY_H
#define TABCLIENTSUMMARY_H

#include <QWidget>
#include "models/sclientmodel.h"

namespace Ui {
class tabClientSummary;
}

class tabClientSummary : public QWidget
{
    Q_OBJECT
public:
    explicit tabClientSummary(QWidget *parent = nullptr);
    explicit tabClientSummary(int id, QWidget *parent = nullptr);
    ~tabClientSummary();
    SClientModel* model();

private:
    Ui::tabClientSummary *ui;
    SClientModel *m_client;
    SPhonesModel *m_phones;
    void fillBanks();
    void fillBinaryProperties();
    void setCursorPositionsToZero();
};

#endif // TABCLIENTSUMMARY_H
