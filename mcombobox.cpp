#include "mcombobox.h"

MComboBox::MComboBox(QWidget *parent) :
    QComboBox(parent)
{

}

MComboBox::~MComboBox()
{

}

void MComboBox::disableWheelEvent(bool state)
{
    wheelEventOn = !state;
}

void MComboBox::wheelEvent(QWheelEvent *event)
{
    if (wheelEventOn)
    {
        QComboBox::wheelEvent(event);
    }
}

void MComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
}
