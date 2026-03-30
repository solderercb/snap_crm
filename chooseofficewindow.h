#ifndef CHOOSEOFFICEWINDOW_H
#define CHOOSEOFFICEWINDOW_H

#include <QDialog>

class windowsDispatcher;

namespace Ui {
class chooseOfficeWindow;
}

class chooseOfficeWindow : public QDialog
{
    Q_OBJECT

signals:
    void officeChoosed();
    void closeWindow();
public:
    explicit chooseOfficeWindow(windowsDispatcher *parent = nullptr);
    ~chooseOfficeWindow();

private:
    Ui::chooseOfficeWindow *ui;
    windowsDispatcher *windowsDispatcherObj;    // для доступа к моделям данных пользователя, компаний, офисов

private slots:
    void accept() override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // CHOOSEOFFICEWINDOW_H
