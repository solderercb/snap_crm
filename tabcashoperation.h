#ifndef TABCASHOPERATION_H
#define TABCASHOPERATION_H

#include <QWidget>
#include "tabcommon.h"
#include "models/scashregistermodel.h"
#include "models/sclientmodel.h"
#include "models/sphonemodel.h"
#include "widgets/shortlivednotification.h"
#include "models/slogrecordmodel.h"

namespace Ui {
class tabCashOperation;
}

class tabCashOperation : public tabCommon
{
    Q_OBJECT

public:
    explicit tabCashOperation(MainWindow *parent = nullptr);
    explicit tabCashOperation(int, MainWindow *parent = nullptr);
    ~tabCashOperation();

private:
    Ui::tabCashOperation *ui;
    static QMap<int, tabCashOperation*> p_instance;
    int order_id = 0;
    SClientModel *clientModel;
    void updateWidgets();
    bool checkInput();

private slots:
    void clearClientCreds(bool);
    void fillClientCreds(int);
    void orderTypeChanged(int);
    void dateChanged(QDate);
    void findClientByLastname(QString);
    void openDocument();
    void amountChanged(QString);
    void constructReason(int);
    void paymentSystemChanged(int);
};

#endif // TABCASHOPERATION_H
