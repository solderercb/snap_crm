#ifndef SGROUPBOXEVENTFILTER_H
#define SGROUPBOXEVENTFILTER_H

#include <QObject>
#include <QGroupBox>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableView>
#include <QPushButton>
#include <QMouseEvent>

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
