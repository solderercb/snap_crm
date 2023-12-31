#ifndef TABCASHMOVEEXCH_H
#define TABCASHMOVEEXCH_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QMap>
#include "tabcommon.h"
#include "models/slogrecordmodel.h"
#include "models/scashregistermodel.h"
#include "models/ssortfilterproxymodel.h"
#include "widgets/shortlivednotification.h"

namespace Ui {
class tabCashMoveExch;
}

class tabCashMoveExch : public tabCommon
{
    Q_OBJECT
signals:
    void generatePrintout(QMap<QString, QVariant>);
public:
    enum Type {Move = 0, Exchange = 1};
    explicit tabCashMoveExch(MainWindow *parent = nullptr);
    static tabCashMoveExch* getInstance(MainWindow *parent);
    ~tabCashMoveExch();
    QString tabTitle() override;
private:
    static tabCashMoveExch* p_instance;
    SCashRegisterModel *cashRegisterSrc = nullptr;
    SCashRegisterModel *cashRegisterDst = nullptr;
    SSortFilterProxyModel *paymentSystemsProxyModel;
    bool m_opType = Type::Move;
    int m_srcOrderId = 0;
    int m_dstOrderId = 0;
    double m_srcAmount = 0;
    double m_dstAmount = 0;
    double m_rate = 1.0;
    void (SCashRegisterModel::*method)(int) = nullptr;
    QString m_reason;
    bool m_widgetsRO = 0;
    bool m_reasonRO = 1;
    bool m_reasonManEdited = 0;
    bool m_skipAutoLogRecord = 1;
    bool m_backdate = 0;
    bool m_showCheckBoxPrint = 1;
    bool m_showFlipCurrencyButton = 0;
    double prevCurrencyRate = 0;
    bool currencyFlipped = 0;
    QString chargeType; // используется для подписи поля Комиссия/Курс и для создания части примечания
    Ui::tabCashMoveExch *ui;
    void setWidgetsParams();
    void updateWidgets();
    bool checkInput();
    void initCashRegisterModel();
    void setDefaultStylesheets();
    bool commit(bool repeatAfter = 0);
    void print();
    void updateOrderIdLineEdit();
    void constructReasonPart();
    void calculateDstAmount();

#ifdef QT_DEBUG
    enum RandomFillerStep {OpType = 1, SrcAccount, DstAccount, Amount, End};
    void randomFill() override;
#endif

private slots:
    void operationTypeChanged(bool);
    void dateChanged(QDate);
    void autoReasonEnabled(int);
    void reasonEdited(QString);
    void buttonSaveClicked();
    void buttonSaveMoreClicked();
    void buttonPrintClicked();
    void srcAmountChanged(double);
    void dstAmountChanged(double);
    void chargeChanged(double);
    void chargeChanged();
    void flipCurrency();
    void guiFontChanged() override;
#ifdef QT_DEBUG
    void test_scheduler_handler() override;
    void test_scheduler2_handler() override;
#endif
};

#endif // TABCASHMOVEEXCH_H
