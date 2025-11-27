#ifndef SPAGEDINTERFACE_H
#define SPAGEDINTERFACE_H

#include <QObject>
#include <QWidget>

class QGridLayout;
class QStackedWidget;
class QLine;
class SToolboxSwitchPanel;
class ProjectGlobals;

class SPagedInterface : public QWidget
{
    Q_OBJECT
public:
    explicit SPagedInterface(QWidget *parent = nullptr);
    ~SPagedInterface();
    void addButton(const QString &caption, QIcon icon, const int id);
    void redraw();
    void setButtonText(const int button, const QString &text);
    void setButtonIcon(const int button, const QIcon icon);
    bool closeRequest();
private:
    QGridLayout *rootGridLayout;
    QGridLayout *tabLayout;
    SToolboxSwitchPanel *toolboxSwitchPanel;
    QStackedWidget *stackedWidget;
    QFrame *line;
    QMap<int, QWidget*> i_pages;
    void delPages();
signals:
    void initPage(const int page);
    void updateBotToolbar(const int page);

public slots:
    void addPage(QWidget *widget, const int page);
    void switchPage(int);
};

#endif // SPAGEDINTERFACE_H
