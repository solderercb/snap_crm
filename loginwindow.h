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
#include <QtAutoUpdaterWidgets/UpdateController>
#include <QtAutoUpdaterWidgets/UpdateButton>

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
private:
	Ui::LoginWindow *ui;
    QGraphicsPixmapItem *logo;
    QGraphicsScene *logoScene;
    QSqlDatabase connMain;
    QSqlDatabase connNtfy;
    QSqlDatabase connThird;
//    QSqlDatabase connFourth;
    QTimer *debugLoginDelay;
    void statusBarMsg(const QString&, int delay = 2500);
    QTimer *statusBarDelay;
    SSLOptionsDialog *modalWidget;
    QWidget *overlay;
    QtAutoUpdater::UpdateController *updateController = nullptr;
    bool checkAppVer();
    int checkSchema();
    bool checkProcessPriv();
    QStringList usersOnline();
    bool updateDB(int);
    void closeConnections();
    void debugInitLoginOptions();
    void fillConnectionParams();
    void startMaintanaceTool();
public slots:
    void show();
private slots:
    void editPassword_onReturnPressed();
    void btnLoginHandler();
    void btnCancelHandler();
    void clearStatusLabel();
    void createSSLOptionsDialog();
    void closeSSLOptionsDialog();
    void selectAscExe();
    void debugLogin();
};

#endif // LOGINWINDOW_H
