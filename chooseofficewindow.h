#ifndef CHOOSEOFFICEWINDOW_H
#define CHOOSEOFFICEWINDOW_H

#include <QDialog>
#include "windowsdispatcher.h"

namespace Ui {
class chooseOfficeWindow;
}

class chooseOfficeWindow : public QDialog
{
    Q_OBJECT

signals:
    void officeChoosed();
public:
    explicit chooseOfficeWindow(windowsDispatcher *parent = nullptr);
    ~chooseOfficeWindow();

private:
    Ui::chooseOfficeWindow *ui;
    windowsDispatcher *windowsDispatcherObj;    // для доступа к моделям данных пользователя, компаний, офисов
    QSqlQueryModel* companiesModel;
    QSqlQueryModel* officesModel;
    QMap<QString, QVariant> *userData;
    QMap<QString, bool> *permissions;

private slots:
    void accept();
};

#endif // CHOOSEOFFICEWINDOW_H
