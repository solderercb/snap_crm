#include "sgroupboxeventfilter.h"

SGroupBoxEventFilter::SGroupBoxEventFilter(QObject *parent) : QObject(parent)
{

}

bool SGroupBoxEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    //    qDebug() << watched->objectName() << ": viewEventFilter: " << event;

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

    // Нажатие левой кнопкой мыши на заголовке groupBox'а скрывает/показывает таблицу (при скрытии groupBox сжимается по вертикали)
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
                QTableView *table = groupBox->findChild<QTableView*>();
                table->setVisible(!table->isVisible());

            /* Это заготовка на будущее
               Приходится использовать отдельные списки вместо QWidget, т. к., при
               использовании последнего, дочерние элемены скрываются нормально, но
               обратно не отображаются.
               Однако, этот код требует доработки: при первом щелчке нужно сохранение состояния
               отображения и скрытие всех, а при повторном щелчке на заголовке группы — восстановление
               сохранённых состояний (это нужно, т. к. могут быть эл-ты не доступные пользователю
               ввиду ограниченности прав */
//                QList<QTableView *> tables = groupBox->findChildren<QTableView *>();
//                for(int i = 0; i < tables.count(); i++)
//                {
//                    tables.at(i)->setVisible(!tables.at(i)->isVisible());
//                }
//                QList<QPushButton *> buttons = groupBox->findChildren<QPushButton *>();
//                for(int i = 0; i < buttons.count(); i++)
//                {
//                    buttons.at(i)->setVisible(!buttons.at(i)->isVisible());
//                }
//                QList<QLabel *> labels = groupBox->findChildren<QLabel *>();
//                for(int i = 0; i < labels.count(); i++)
//                {
//                    labels.at(i)->setVisible(!labels.at(i)->isVisible());
//                }
//                QList<QLineEdit *> lineEdits = groupBox->findChildren<QLineEdit *>();
//                for(int i = 0; i < lineEdits.count(); i++)
//                {
//                    lineEdits.at(i)->setVisible(!lineEdits.at(i)->isVisible());
//                }
//                QList<QTextEdit *> textEdits = groupBox->findChildren<QTextEdit *>();
//                for(int i = 0; i < textEdits.count(); i++)
//                {
//                    textEdits.at(i)->setVisible(!textEdits.at(i)->isVisible());
//                }
//                QList< *>  = groupBox->findChildren< *>();
//                for(int i = 0; i < .count(); i++)
//                {
//                    .at(i)->setVisible(!.at(i)->isVisible());
//                }
            }
        }
    }
    return false;
}
