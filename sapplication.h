#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include <QApplication>

class SApplication : public QApplication
{
public:
    SApplication(int& argc, char** argv);
    bool notify(QObject* receiver, QEvent* event);
public Q_SLOTS:
    void quit();
};

#endif // SAPPLICATION_H
