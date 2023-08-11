#include "global.h"
#include "smodalwidget.h"
#include "ui_smodalwidget.h"

SModalWidget::SModalWidget(QWidget *parent, Qt::WindowFlags flags) :
    SWidget(parent, flags)
{
    background = new QWidget(parent);
    background->setStyleSheet("QWidget { background: rgba(154, 154, 154, 128);}");
    background->resize(QApplication::activeWindow()->size());
    background->setVisible(true);

    setWindowModality(Qt::WindowModal);
}

SModalWidget::~SModalWidget()
{
    delete background;
    emit onDelete();
}
