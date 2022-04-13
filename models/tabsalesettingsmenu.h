#ifndef POPUPM_H
#define POPUPM_H

#include <QWidget>
#include <QFrame>
#include <QWidgetAction>
#include <QStandardItemModel>

namespace Ui {
    class popupm;
}

class popupm : public QFrame
{
    Q_OBJECT
signals:
    void hidden();
public:
    explicit popupm(QWidget *parent = nullptr);
    ~popupm();
    Ui::popupm *ui; // в данном случае ui должен быть public

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
    popupm *widget;
    void setFilter(QMap<QString, int> *filter){filterSettingsBuf = filter;};
private:
    QWidget *w;
    bool hideEventFilter;
    QMap<QString, int> *filterSettingsBuf = nullptr;
protected:
    QWidget * createWidget(QWidget *parent);
private slots:
    void slotHidden();
};

#endif // POPUPM_H
