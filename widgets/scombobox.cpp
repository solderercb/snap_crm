#include "scombobox.h"

SComboBox::SComboBox(QWidget *parent) :
    QComboBox(parent)
{

}

SComboBox::~SComboBox()
{

}

void SComboBox::disableWheelEvent(bool state)
{
    wheelEventOn = !state;
}

void SComboBox::wheelEvent(QWheelEvent *event)
{
    if (wheelEventOn)
    {
        QComboBox::wheelEvent(event);
    }
}

void SComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
}

/* Возвращает код, используемый в БД, выбранного индекса */
int SComboBox::currentDbId()
{
//    model()->index(currentIndex(),)
}
