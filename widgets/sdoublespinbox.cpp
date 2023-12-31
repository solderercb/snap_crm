#include "sdoublespinbox.h"
#include "global.h"

SDoubleSpinBox::SDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
    setButtonSymbols(QAbstractSpinBox::NoButtons);
    if(comSettings->classicKassa)
    {
        setDecimals(2);
    }
    else
    {
        setDecimals(0);
    }
    setMaximum(9999999);
    setGroupSeparatorShown(true);
}
