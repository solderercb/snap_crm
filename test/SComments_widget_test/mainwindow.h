#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <tabCommon>

class MainWindow;
static MainWindow* m_instance = nullptr;

class MainWindow
{
public:
    MainWindow(){};
    ~MainWindow(){m_instance = nullptr;};
    static MainWindow* getInstance(){if(!m_instance) m_instance = new MainWindow(); return m_instance;};

    template<typename t>
    QList<t> findChildren(){return QList<t>();};
private:
};

#endif // MAINWINDOW_H
