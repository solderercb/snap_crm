#ifndef BOTTOOLBARWIDGET_H
#define BOTTOOLBARWIDGET_H

#include <QDockWidget>

namespace Ui {
class BotToolbarWidget;
}

class BotToolbarWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit BotToolbarWidget(QWidget *parent = nullptr);
    ~BotToolbarWidget();

private:
    Ui::BotToolbarWidget *ui;
};

#endif // BOTTOOLBARWIDGET_H
