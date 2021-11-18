// found at https://stackoverflow.com/a/39815238

#ifndef MY_WIDGET_H
#define MY_WIDGET_H

#include <QWidget>
#include <QMainWindow>

class MyMainWindow;
class QToolBar;

class MyWidget : public QWidget{

    Q_OBJECT

public:
    MyWidget(MyWidget* parent = 0){
        if(parent){
            connect(parent, SIGNAL(window_closed()),
                    this, SLOT(close()));
            connect(parent, SIGNAL(destroyed(QObject*)),
                    this, SLOT(deleteLater()));
        }
    }

    MyWidget(MyMainWindow* parent);

    void closeEvent(QCloseEvent* event){
        emit window_closed();
        QWidget::closeEvent(event);
    }

    void addToolBarBreak(Qt::ToolBarArea area = Qt::TopToolBarArea);
    void insertToolBarBreak(QToolBar *before);

    void addToolBar(Qt::ToolBarArea area, QToolBar *toolbar);
    void addToolBar(QToolBar *toolbar);
    QToolBar *addToolBar(const QString &title);
    void insertToolBar(QToolBar *before, QToolBar *toolbar);
    void removeToolBar(QToolBar *toolbar);
    void removeToolBarBreak(QToolBar *before);

    bool unifiedTitleAndToolBarOnMac() const;

    Qt::ToolBarArea toolBarArea(const QToolBar *toolbar) const;
    bool toolBarBreak(QToolBar *toolbar) const;

signals:
    void window_closed();
};

class MyMainWindow : public QMainWindow{

    Q_OBJECT

public:
    MyMainWindow(MyMainWindow* parent = 0){
        if(parent){
            connect(parent, SIGNAL(window_closed()),
                    this, SLOT(close()));
            connect(parent, SIGNAL(destroyed(QObject*)),
                    this, SLOT(deleteLater()));
        }
    }

    MyMainWindow(MyWidget* parent){
        connect(parent, SIGNAL(window_closed()),
                this, SLOT(close()));
        connect(parent, SIGNAL(destroyed(QObject*)),
                this, SLOT(deleteLater()));
    }

    void closeEvent(QCloseEvent* event){
        emit window_closed();
        QMainWindow::closeEvent(event);
    }

signals:
    void window_closed();
};

#endif // MY_WIDGET_H
