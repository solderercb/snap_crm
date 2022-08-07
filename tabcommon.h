#ifndef TABCOMMON_H
#define TABCOMMON_H

#include <qglobal.h>
#include <QWidget>
#include "mainwindow.h"

class tabCommon : public QWidget
{
    Q_OBJECT

signals:
    void updateLabel(QWidget*, QString);
    void updateIcon(QWidget*, QIcon);

public:
    explicit tabCommon(MainWindow *p = nullptr);
    ~tabCommon();
    virtual bool tabCloseRequest();

};


#endif // TABCOMMON_H
