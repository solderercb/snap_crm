#ifndef SMODALWIDGET_H
#define SMODALWIDGET_H

#include "widgets/swidget.h"
#include "global.h"
#include "shortlivednotification.h"

class SModalWidget : public SWidget
{
    Q_OBJECT
signals:
    void onDelete();
public:
    explicit SModalWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~SModalWidget();
private:
    QWidget *background;
};

#endif // SMODALWIDGET_H
