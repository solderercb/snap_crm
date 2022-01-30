#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>

#define NO_LOGIN

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
	Q_OBJECT

signals:
	void btnCancelClick();
    void DBConnectOK();

public:
	explicit LoginWindow(QWidget *parent = 0);
	~LoginWindow();
#ifdef NO_LOGIN
    void debugLogin();
#endif

private:
	Ui::LoginWindow *ui;
    QSqlDatabase connMain;
    QSqlDatabase connNtfy;
//    QSqlDatabase connThird;
//    QSqlDatabase connFourth;

private slots:
    void btnLoginHandler();
    void btnCancelHandler();
};

#endif // LOGINWINDOW_H
