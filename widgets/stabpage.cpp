#include "stabpage.h"

STabPage::STabPage(QWidget *parent) : QWidget(parent)
{

}

void STabPage::setCursorPositionsToZero()
{
    QLineEdit *le;
    QList<QLineEdit*> list = this->findChildren<QLineEdit*>(QRegularExpression(".*"));
    foreach(le, list)
        le->setCursorPosition(0);
}
