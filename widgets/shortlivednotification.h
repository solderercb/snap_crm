#ifndef SHORTLIVEDNOTIFICATION_H
#define SHORTLIVEDNOTIFICATION_H

#include <QtWidgets/QApplication>
#include <QWidget>
#include <QLabel>
#include <QScreen>
#include <QTimer>
#define MAX_POPUPS 3
#define DEFAULT_WIDTH  385
#define DEFAULT_HEIGHT  90

class label : public QLabel
{
    Q_OBJECT

signals:
    void clicked();
public:
    explicit label(QWidget *parent = nullptr);
    ~label();
protected:
    void mousePressEvent(QMouseEvent *);
};

class shortlivedNotification : public QWidget
{
    Q_OBJECT
public:
    enum {topLeft, topRight, bottomLeft, bottomRight};
    explicit shortlivedNotification(QObject *parent, QString, QString, QColor captionColor_ = captionColor, QColor messageColor_ = messageColor, int millisecondsTimeout = 3000);
    // TODO: сделать перегруженный контруктор для более гибкого использования класса.
    ~shortlivedNotification();
    void repaintPopups();
    QSize sizeHint();
    static void setSize(QSize);
    static void setSize(int widht, int height);
    static void setAppearance(QWidget *, int);
    static void setCaptionFont(QFont);
    static void setMessageFont(QFont);
    static void setCaptionColor(QColor);
    static void setMessageColor(QColor);
    static void setMaxDisplayedNotifications(int);

private:
    label *labelMessage;
    label *labelCaption;
    QTimer *autoDisappearTimer, *animatedPaintTimer;
    static QVector<shortlivedNotification*> p_instance;
    static int popupsCounter;
    static QPoint appearance;
    static int appearanceStep;
    static QSize currentScreen;
    static QSize size;
    static QFont captionFont;
    static QFont messageFont;
    static QColor captionColor;
    static QColor messageColor;
    static int maxDisplayedNotifications;

private slots:
    void close();
    void autoDisappearTimer_handler();
    void animatedPaintTimer_handler();

};

#endif // SHORTLIVEDNOTIFICATION_H
