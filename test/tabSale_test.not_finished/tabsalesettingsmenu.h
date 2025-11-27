#ifndef TABSALESETTINGSMENU_H
#define TABSALESETTINGSMENU_H

#include <QWidgetAction>

class  tabSaleSettingsMenu: public QWidgetAction
{
    Q_OBJECT
public:
    enum Params {PrintCheck = 1 << 0, PrintDoc = 1 << 1, ShowDescr = 1 << 2};
    explicit tabSaleSettingsMenu(QObject *parent = nullptr);
    void setParamsModel(int *params);

};

#endif // TABSALESETTINGSMENU_H
