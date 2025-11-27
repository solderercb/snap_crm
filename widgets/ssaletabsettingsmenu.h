#ifndef TABSALEPOPUPFORM_H
#define TABSALEPOPUPFORM_H

#include <QFrame>
#include <QWidgetAction>

namespace Ui {
    class tabSalePopupForm;
}

class tabSalePopupForm : public QFrame
{
    Q_OBJECT
signals:
    void hidden();
public:
    explicit tabSalePopupForm(QWidget *parent = nullptr);
    ~tabSalePopupForm();
    Ui::tabSalePopupForm *ui; // в данном случае ui должен быть public

private:
    bool event(QEvent *);
};

class  tabSaleSettingsMenu: public QWidgetAction
{
    Q_OBJECT
signals:
    void hidden();
public:
    explicit tabSaleSettingsMenu(QWidget * parent);
    tabSalePopupForm *widget;
    enum Params {PrintCheck = 1 << 0, PrintDoc = 1 << 1, ShowDescr = 1 << 2};
    void setParamsModel(int *params){settings = params;};
    Q_ENUM(Params);
private:
    QWidget *w;
    bool hideEventFilter;
    int *settings = nullptr;
protected:
    QWidget * createWidget(QWidget *parent);
private slots:
    void slotHidden();
};

#endif // TABSALEPOPUPFORM_H
