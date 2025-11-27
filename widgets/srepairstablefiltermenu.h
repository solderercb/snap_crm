#ifndef SREPAIRSTABLEFILTERMENU_H
#define SREPAIRSTABLEFILTERMENU_H

#include <QWidget>
#include <QFrame>
#include <QWidgetAction>

class SAbstractItemModel;
class QEvent;

namespace Ui {
    class Widget;
}

/* Класс-прокладка, непосредственно предоставляющий форму меню */
class Widget : public QFrame
{
    Q_OBJECT
signals:
    void hidden();
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    Ui::Widget* Ui();
private:
    Ui::Widget *ui;
    bool event(QEvent *);
    void initNotConditionWidget(const QIcon &icon, QPushButton *button);
};

class  SRepairsTableFilterMenu: public QWidgetAction
{
    Q_OBJECT
signals:
    void hidden();
public:
    struct Filter
    {
        bool filterActive = 0;
        bool onlyCurrentUsersRepairs = 0;
        bool onlyCurrentUsersRepairsNot = 0;
        int office = -1;
        bool officeNot = 0;
        int repairState = -1;
        bool repairStateNot = 0;
        int employee = -1;
        bool employeeNot = 0;
        int clientsCat = -1;
        bool clientsCatNot = 0;
        QString periodBegin;
        QString periodEnd;
        int periodMovingInterval = 0;
        bool periodNot = 0;
        QVariant rfu;
        bool rfuNot = 0;
    };
    explicit SRepairsTableFilterMenu(QWidget * parent);
    QWidget *widget;
    void setComboBoxOfficeModel(SAbstractItemModel *model){officeModelBuf = model;};
    void setComboBoRepairStateModel(SAbstractItemModel *model){repairStateModelBuf = model;};
    void setComboBoxEmployeeModel(SAbstractItemModel *model){employeeModelBuf = model;};
    void setComboBoxClientsCatModel(QAbstractItemModel *model){clientsCatModelBuf = model;};
//    void setComboBoxRfuModel(SAbstractItemModel *model){rfuModelBuf = model;};
    void setFilterSettings(Filter *filter){m_filter = filter;};
private:
    Ui::Widget *ui;
    bool hideEventFilter;
    SAbstractItemModel *officeModelBuf = nullptr;
    SAbstractItemModel *repairStateModelBuf = nullptr;
    SAbstractItemModel *employeeModelBuf = nullptr;
    QAbstractItemModel *clientsCatModelBuf = nullptr;
//    SAbstractItemModel *rfuModelBuf = nullptr;
//    QMap<QString, QVariant> *filterSettingsBuf = nullptr;
    Filter *m_filter = nullptr;
    void updateFilterStateFlag();
protected:
    QWidget * createWidget(QWidget *parent);
private slots:
    void slotHidden();
    void clearComboBoxOffice();
    void clearComboBoxStatus();
    void clearComboBoxEmployee();
    void clearComboBoxClients();
//    void clearComboBoxRfu();
    void clearPeriod();
};

#endif // SREPAIRSTABLEFILTERMENU_H
