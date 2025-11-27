#ifndef WINDOWSDISPATCHER_H
#define WINDOWSDISPATCHER_H
#include <QObject>

class windowsDispatcher : public QObject
{
    Q_OBJECT
signals:
    void quit();
public:
    windowsDispatcher(QObject* parent = nullptr);
    virtual ~windowsDispatcher();
private:
    void debugInitSettings();
public slots:
    void connectOK();
    void createChooseOfficeWindow();
    void createMainWindow();
};

#endif
