#ifndef SWIDGET_H
#define SWIDGET_H

#include <QObject>
#include <QWidget>
#include <SWidgetCommonMethods>

class SWidget : public QWidget, public SWidgetCommonMethods
{
    Q_OBJECT
public:
    explicit SWidget(QWidget *parent = nullptr, Qt::WindowFlags = Qt::WindowFlags());
    ~SWidget();
    QFont guiFont();
protected:
    SWidget* findParentTab();
    virtual QString queryLogFile() override {return metaObject()->className();};
public slots:
    virtual bool manualSubmit() override {return SWidgetCommonMethods::manualSubmit();};
protected slots:
    virtual void guiFontChanged(){};
};

#endif // SWIDGET_H
