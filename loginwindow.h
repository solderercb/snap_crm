#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

class QGraphicsPixmapItem;
class QGraphicsScene;
class QTimer;
class SSLOptionsDialog;
namespace QtAutoUpdater {
class UpdateController;
};

#include <QWidget>
#include <QSqlDatabase>

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
    std::unique_ptr<QTimer> debugLoginDelay;
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
    bool setDebugLoginCreds();
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
