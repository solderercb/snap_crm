#include "bottoolbarwidget.h"
#include "ui_bottoolbarwidget.h"

BotToolbarWidget::BotToolbarWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::BotToolbarWidget)
{
    ui->setupUi(this);

    this->setTitleBarWidget(new QWidget());
}

BotToolbarWidget::~BotToolbarWidget()
{
    delete ui;
}
