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

class  repairTableFilterMenu: public QWidgetAction
{
    Q_OBJECT
signals:
    void hidden();
public:
    explicit repairTableFilterMenu(QWidget * parent);
    popupm *widget;
    void setComboBoxOfficeModel(QAbstractItemModel *model){officeModelBuf = model;};
    void setComboBoxStatusModel(QAbstractItemModel *model){statusModelBuf = model;};
    void setComboBoxEmployeeModel(QAbstractItemModel *model){employeeModelBuf = model;};
    void setComboBoxClientModel(QAbstractItemModel *model){clientModelBuf = model;};
    void setComboBoxXModel(QAbstractItemModel *model){xModelBuf = model;};
    void setFilterSettings(QMap<QString, int> *filter){filterSettingsBuf = filter;};
private:
    QWidget *w;
    bool hideEventFilter;
    QAbstractItemModel *officeModelBuf = nullptr;
    QAbstractItemModel *statusModelBuf = nullptr;
    QAbstractItemModel *employeeModelBuf = nullptr;
    QAbstractItemModel *clientModelBuf = nullptr;
    QAbstractItemModel *xModelBuf = nullptr;
    QMap<QString, int> *filterSettingsBuf = nullptr;
protected:
    QWidget * createWidget(QWidget *parent);
private slots:
    void slotHidden();
    void clearComboBoxOffice();
    void clearComboBoxStatus();
    void clearComboBoxEmployee();
    void clearComboBoxClients();
    void clearComboBoxX();
};

#endif // POPUPM_H
