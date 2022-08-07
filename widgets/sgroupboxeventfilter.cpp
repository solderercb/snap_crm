#include "sgroupboxeventfilter.h"

SGroupBoxEventFilter::SGroupBoxEventFilter(QObject *parent) : QObject(parent)
{

}

bool SGroupBoxEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    // когда указатель находится на заголовке (по всей ширине groupBox'а), устанавливаем курсор в виде руки с указательным пальцем
    // TODO: проверить сколько эта метода жрёт ресурсов
    QGroupBox *groupBox = static_cast<QGroupBox*>(watched);
    if (event->type() == QEvent::HoverMove)
    {
#if QT_VERSION >= 0x060000
        QPoint point = static_cast<QHoverEvent*>(event)->position().toPoint();
#else
        QPoint point = static_cast<QHoverEvent*>(event)->pos();
#endif
        if (point.x() > 0 && point.x() < groupBox->width() && point.y() >0 && point.y() < 20)
            groupBox->setCursor(Qt::PointingHandCursor);
        else
            groupBox->unsetCursor();
    }

    // Нажатие левой кнопкой мыши на заголовке groupBox'а скрывает/показывает элементы groupBox'а (при скрытии groupBox сжимается по вертикали)
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseButtonPress = static_cast<QMouseEvent*>(event);

        if (mouseButtonPress->button() == Qt::LeftButton)
        {
#if QT_VERSION >= 0x060000
            if (mouseButtonPress->position().toPoint().x() > 0 && mouseButtonPress->position().toPoint().x() < groupBox->width() && mouseButtonPress->position().toPoint().y() >0 && mouseButtonPress->position().toPoint().y() < 20)
#else
            if (mouseButtonPress->localPos().toPoint().x() > 0 && mouseButtonPress->localPos().toPoint().x() < groupBox->width() && mouseButtonPress->localPos().toPoint().y() >0 && mouseButtonPress->localPos().toPoint().y() < 20)
#endif
            {
                emit toggleElementsVisibility();
            }
        }
    }
    return false;
}
