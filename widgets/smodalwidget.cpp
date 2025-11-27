#include "smodalwidget.h"
#include <ProjectGlobals>
#include <MainWindow>

SModalWidget::SModalWidget(QWidget *parent, Qt::WindowFlags flags) :
    SWidget(parent, flags)
{
    background = new QWidget(parent);
    background->setStyleSheet("QWidget { background: rgba(154, 154, 154, 128);}");
    background->resize(MainWindow::getInstance()->size());
    background->setVisible(true);

    setWindowModality(Qt::WindowModal);
}

SModalWidget::SModalWidget(QWidget *parent, State state) :
    SWidget(parent)
{
    if(state == Hidden)
        hide();
}

SModalWidget::~SModalWidget()
{
    if(background)
        delete background;
    emit onDelete();
}
