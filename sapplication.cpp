#include "sapplication.h"
#include <exception>
#include <QDebug>
#include "global.h"
#include "widgets/shortlivednotification.h"

SApplication::SApplication(int& argc, char** argv) :
    QApplication(argc, argv)
{
}

// подсмотрено https://stackoverflow.com/a/14804711 (http://www.02.246.ne.jp/~torutk/cxx/qt/QtMemo.html)
bool SApplication::notify(QObject *receiver, QEvent *event)
{
    bool done = true;
    try {
        done = QApplication::notify(receiver, event);
    }
    // запись в журнал и прочие действия для разных типов исключений:
    catch (const std::exception& ex)
    {
        qDebug().nospace() << "SApplication::notify() | std::exception: " << ex.what();
    }
    catch (const int &ex)
    {
        qDebug().nospace() << "SApplication::notify() | int exception: " << ex;
    }
    catch (const Global::ThrowType &ex)
    {
        qDebug().nospace() << "SApplication::notify() | Global exception: " << (Global::ThrowType)ex;
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Ошибка"), tr("Локальное время и время сервера отличаются на более чем 30 секунд"), QColor("#FFC7AD"), QColor("#FFA477"));
    }
    catch (...)
    {
        qDebug().nospace() << "SApplication::notify() | unknown exception";
    }
    return done;

}
