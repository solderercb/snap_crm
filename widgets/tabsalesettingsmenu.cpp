#include "tabsalesettingsmenu.h"
#include "ui_tabsalesettingsmenu.h"

tabSalePopupForm::tabSalePopupForm(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::tabSalePopupForm)
{
    ui->setupUi(this);
}

tabSalePopupForm::~tabSalePopupForm()
{
    delete ui;
}

bool tabSalePopupForm::event(QEvent *e)
{
    if (e->type() == QEvent::Hide)
    {
//        qDebug() << "tabSalePopupForm::event(QEvent *e); " << e->type();
        emit hidden();
    }
    return QFrame::event(e);
}

tabSaleSettingsMenu::tabSaleSettingsMenu(QWidget * parent) : QWidgetAction(parent),
    w(parent)
{
}

QWidget * tabSaleSettingsMenu::createWidget(QWidget *parent)
{
    widget = new tabSalePopupForm(parent);
    hideEventFilter = 1;   // при закрытии всплывающего меню почему-то генерируется два события Hide
    // после создания виджета устанавливаем модели данных для комбобоксов
    if (settings)
    {
        widget->ui->checkBoxPrintCheck->setChecked(*settings & Params::PrintCheck);
        widget->ui->checkBoxPrintDoc->setChecked(*settings & Params::PrintDoc);
        widget->ui->checkBoxShowItemDescr->setChecked(*settings & Params::ShowDescr);
    }

    connect(widget, SIGNAL(hidden()), this, SLOT(slotHidden()));
    return widget;

}

void tabSaleSettingsMenu::slotHidden()
{
    if (hideEventFilter)
    {
        hideEventFilter = 0;
        if (settings)
        {
            *settings &= ~(Params::PrintCheck | Params::PrintDoc | Params::ShowDescr);
            *settings |= widget->ui->checkBoxPrintCheck->isChecked()?Params::PrintCheck:0;
            *settings |= widget->ui->checkBoxPrintDoc->isChecked()?Params::PrintDoc:0;
            *settings |= widget->ui->checkBoxShowItemDescr->isChecked()?Params::ShowDescr:0;
        }
        emit hidden();
    }
}
