#ifndef SGROUPBOXEVENTFILTER_H
#define SGROUPBOXEVENTFILTER_H

#include <QObject>

class QEvent;

class SGroupBoxEventFilter : public QObject
{
    Q_OBJECT
signals:
    void toggleElementsVisibility();

public:
    explicit SGroupBoxEventFilter(QObject *);

protected:
    bool eventFilter(QObject*, QEvent*) override;

private:

};

#endif // SGROUPBOXEVENTFILTER_H
