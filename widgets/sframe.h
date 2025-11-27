#ifndef SFRAME_H
#define SFRAME_H

#include <QFrame>
#include <SWidgetCommonMethods>
#include <QObject>

class SFrame : public QFrame, public SWidgetCommonMethods
{
    Q_OBJECT
public:
    SFrame(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
protected:
    virtual QString queryLogFile() override {return metaObject()->className();};
};

#endif // SFRAME_H
