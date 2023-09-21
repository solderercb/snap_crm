#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>
#include <QCryptographicHash>
#include <QFile>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QResource>
#include <QIODevice>
#include "models/ssloptionsdialog.h"
#include "widgets/shortlivednotification.h"
#include "windowsdispatcher.h"

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
    explicit LoginWindow(QObject *parent = nullptr);
	~LoginWindow();
    void debugLogin();

private:
	Ui::LoginWindow *ui;
    QGraphicsPixmapItem *logo;
    QGraphicsScene *logoScene;
    QSqlDatabase connMain;
    QSqlDatabase connNtfy;
    QSqlDatabase connThird;
//    QSqlDatabase connFourth;
    void statusBarMsg(const QString&, int delay = 2500);
    QTimer *statusBarDelay;
    SSLOptionsDialog *modalWidget;
    QWidget *overlay;
    bool checkAppVer();
    int checkSchema();
    QStringList usersOnline();
    bool updateDB(int);
    void closeConnections();
    void debugInitLoginOptions();
    void fillConnectionParams();
private slots:
    void editPassword_onReturnPressed();
    void btnLoginHandler();
    void btnCancelHandler();
    void clearStatusLabel();
    void createSSLOptionsDialog();
    void closeSSLOptionsDialog();
    void selectAscExe();
};

#endif // LOGINWINDOW_H
