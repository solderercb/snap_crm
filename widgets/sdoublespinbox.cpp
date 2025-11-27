#include "sdoublespinbox.h"
#include <ProjectGlobals>
#include <SComSettings>

SDoubleSpinBox::SDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
    setButtonSymbols(QAbstractSpinBox::NoButtons);
    if(comSettings->classicKassa())
    {
        setDecimals(2);
    }
    else
    {
        setDecimals(0);
    }
    setMaximum(9999999);
    setGroupSeparatorShown(true);
    installEventFilter(this);
}

bool SDoubleSpinBox::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        // В английской раскладке разделитель по умолчанию — точка, а в русской — запятая; при этом в Windows для стран СНГ (и других)
        // разделитель по умолчанию — запятая.
        // При вводе дробной части может быть включена не та раскладка клавиатуры и тогда символ не введётся; это дико бесит.
        QKeyEvent *e = dynamic_cast<QKeyEvent*>(event);
        if(e->key() == Qt::Key_Comma || e->key() == Qt::Key_Period)
        {
//            qDebug().nospace() << "[" << this << "] keyPressEvent() | watched: " << watched \
//                     << ", event->key(): " << e->text() \
//                     << ", sysLocale.decimalPoint(): " << sysLocale.decimalPoint() \
//                     << ", sysLocale.groupSeparator(): " << sysLocale.groupSeparator();
            if(e->text().compare(sysLocale.decimalPoint()))
            {
                e->ignore();
                QKeyEvent* newEvent = new QKeyEvent(e->type(), e->key(), e->modifiers(),
                                                    sysLocale.decimalPoint(), e->isAutoRepeat(), e->count());
                QCoreApplication::postEvent(this, newEvent);
                return true;
            }
        }
    }

    return QDoubleSpinBox::eventFilter(watched, event);
}
