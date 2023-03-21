#ifndef STABPAGE_H
#define STABPAGE_H

#include <QObject>
#include <QWidget>

class STabPage : public QWidget
{
    Q_OBJECT
public:
    explicit STabPage(QWidget *parent = nullptr);
protected:
    void setCursorPositionsToZero();
private:
signals:

};

#endif // STABPAGE_H
