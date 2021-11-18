// found at https://stackoverflow.com/a/39815238

#include "my_widget.h"

MyWidget::MyWidget(MyMainWindow* parent){
    connect(parent, SIGNAL(window_closed()),
            this, SLOT(close()));
    connect(parent, SIGNAL(destroyed(QObject*)),
            this, SLOT(deleteLater()));
}