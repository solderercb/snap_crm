#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
	Q_OBJECT

signals:
	void ConnectToDB(const QString &username, const QString &password, const uchar DB_ID);
	void btnCancelClick();

public:
	explicit LoginWindow(QWidget *parent = 0);
	~LoginWindow();

private:
	Ui::LoginWindow *ui;

public slots:
	void DBConnectErr(const QString &message);

private slots:
	void ConnectToDB();
	void slotCancelClick();
};

#endif // LOGINWINDOW_H
