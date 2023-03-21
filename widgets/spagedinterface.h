#ifndef SPAGEDINTERFACE_H
#define SPAGEDINTERFACE_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QLine>
#include <widgets/stoolboxswitchpanel.h>

class SPagedInterface : public QWidget
{
    Q_OBJECT
public:
    explicit SPagedInterface(QWidget *parent = nullptr);
    ~SPagedInterface();
    void addButton(const QString &caption, QIcon icon, const int id);
    void redraw();
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
    void switchPage(const int);
};

#endif // SPAGEDINTERFACE_H
