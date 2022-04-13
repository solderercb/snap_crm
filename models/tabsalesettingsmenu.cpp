#include "tabsalesettingsmenu.h"
#include "ui_tabsalesettingsmenu.h"

popupm::popupm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::popupm)
{
    ui->setupUi(this);
}

popupm::~popupm()
{
    delete ui;
}

bool popupm::event(QEvent *e)
{
    if (e->type() == QEvent::Hide)
    {
//        qDebug() << "popupm::event(QEvent *e); " << e->type();
        emit hidden();
    }
    QFrame::event(e);
}

tabSaleSettingsMenu::tabSaleSettingsMenu(QWidget * parent) : QWidgetAction(parent),
    w(parent)
{
}

QWidget * tabSaleSettingsMenu::createWidget(QWidget *parent)
{
    widget = new popupm(parent);
    hideEventFilter = 1;   // при закрытии всплывающего меню почему-то генерируется два события Hide
    // после создания виджета устанавливаем модели данных для комбобоксов
    if (filterSettingsBuf)
    {
        if (filterSettingsBuf->contains("printCheck"))
            widget->ui->checkBoxPrintCheck->setChecked(filterSettingsBuf->value("printCheck"));

        if (filterSettingsBuf->contains("printDoc"))
            widget->ui->checkBoxPrintDoc->setChecked(filterSettingsBuf->value("printDoc"));

        if (filterSettingsBuf->contains("showItemDescr"))
            widget->ui->checkBoxShowItemDescr->setChecked(filterSettingsBuf->value("showItemDescr"));
    }

    connect(widget, SIGNAL(hidden()), this, SLOT(slotHidden()));
    return widget;

}

void tabSaleSettingsMenu::slotHidden()
{
    if (hideEventFilter)
    {
        hideEventFilter = 0;
        if (filterSettingsBuf)
        {
            filterSettingsBuf->insert("printCheck", widget->ui->checkBoxPrintCheck->isChecked());
            filterSettingsBuf->insert("printDoc", widget->ui->checkBoxPrintDoc->isChecked());
            filterSettingsBuf->insert("showItemDescr", widget->ui->checkBoxShowItemDescr->isChecked());
        }
        emit hidden();
    }
}
