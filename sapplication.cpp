#include "sapplication.h"
#include <exception>
#include <QDebug>
#include "global.h"

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
        qDebug().nospace() << "SApplication::notify() | Unhandled std::exception: " << ex.what();
    }
    catch (const int &ex)
    {
        qDebug().nospace() << "SApplication::notify() | Unhandled int exception: " << ex;
    }
    catch (const Global::ThrowType &ex)
    {
        Global::errorPopupMsg(ex);
        qDebug().nospace() << "SApplication::notify() | Unhandled ThrowType exception: " << ex;
    }
    catch (...)
    {
        qDebug().nospace() << "SApplication::notify() | Unhandled unknown exception";
    }
    return done;

}
