#include "tabcommon.h"

tabCommon::tabCommon(MainWindow *p) :
    QWidget(p)
{
}

tabCommon::~tabCommon()
{
}

bool tabCommon::tabCloseRequest()
{
    return 1;
}
