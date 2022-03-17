#include "repairtablefiltermenu.h"
#include "ui_repairtablefiltermenu.h"

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

repairTableFilterMenu::repairTableFilterMenu(QWidget * parent) : QWidgetAction(parent),
    w(parent)
{
}

QWidget * repairTableFilterMenu::createWidget(QWidget *parent)
{
    widget = new popupm(parent);
    hideEventFilter = 1;   // при закрытии всплывающего меню почему-то генерируется два события Hide
    // после создания виджета устанавливаем модели данных для комбобоксов
    if (officeModelBuf)
    {
        widget->ui->comboBoxOffice->setModel(officeModelBuf);
        widget->ui->comboBoxOffice->setCurrentIndex(-1);
        if (filterSettingsBuf->contains("office"))
            widget->ui->comboBoxOffice->setCurrentIndex(filterSettingsBuf->value("office"));
    }
    if (statusModelBuf)
    {
        widget->ui->comboBoxStatus->setModel(statusModelBuf);
        widget->ui->comboBoxStatus->setCurrentIndex(-1);
        if (filterSettingsBuf->contains("status"))
            widget->ui->comboBoxStatus->setCurrentIndex(filterSettingsBuf->value("status"));
    }
    if (employeeModelBuf)
    {
        widget->ui->comboBoxEmployee->setModel(employeeModelBuf);
        widget->ui->comboBoxEmployee->setCurrentIndex(-1);
        if (filterSettingsBuf->contains("employee"))
            widget->ui->comboBoxEmployee->setCurrentIndex(filterSettingsBuf->value("employee"));
    }
    if (clientModelBuf)
    {
        widget->ui->comboBoxClients->setModel(clientModelBuf);
        widget->ui->comboBoxClients->setCurrentIndex(-1);
        if (filterSettingsBuf->contains("client"))
            widget->ui->comboBoxClients->setCurrentIndex(filterSettingsBuf->value("client"));
    }
    if (xModelBuf)
    {
        widget->ui->comboBoxX->setModel(xModelBuf);
        widget->ui->comboBoxX->setCurrentIndex(-1);
        if (filterSettingsBuf->contains("x"))
            widget->ui->comboBoxX->setCurrentIndex(filterSettingsBuf->value("x"));
    }
    if (filterSettingsBuf)
    {
        if (filterSettingsBuf->contains("onlyMyOrders"))
            widget->ui->checkBoxOnlyMyOrders->setChecked(filterSettingsBuf->value("onlyMyOrders"));
    }

    connect(widget, SIGNAL(hidden()), this, SLOT(slotHidden()));
    QObject::connect(widget->ui->toolButton, SIGNAL(clicked()), this, SLOT(clearComboBoxOffice()));
    QObject::connect(widget->ui->toolButton_2, SIGNAL(clicked()), this, SLOT(clearComboBoxStatus()));
    QObject::connect(widget->ui->toolButton_3, SIGNAL(clicked()), this, SLOT(clearComboBoxEmployee()));
    QObject::connect(widget->ui->toolButton_4, SIGNAL(clicked()), this, SLOT(clearComboBoxClients()));
    QObject::connect(widget->ui->toolButton_5, SIGNAL(clicked()), this, SLOT(clearComboBoxX()));
    return widget;

}

void repairTableFilterMenu::slotHidden()
{
    if (hideEventFilter)
    {
        hideEventFilter = 0;
        if (filterSettingsBuf)
        {
            filterSettingsBuf->insert("onlyMyOrders", widget->ui->checkBoxOnlyMyOrders->isChecked());
            filterSettingsBuf->insert("office", widget->ui->comboBoxOffice->currentIndex());
            filterSettingsBuf->insert("status", widget->ui->comboBoxStatus->currentIndex());
            filterSettingsBuf->insert("employee", widget->ui->comboBoxEmployee->currentIndex());
            filterSettingsBuf->insert("client", widget->ui->comboBoxClients->currentIndex());
            filterSettingsBuf->insert("x", widget->ui->comboBoxX->currentIndex());
        }
        emit hidden();
    }
}

void repairTableFilterMenu::clearComboBoxOffice()
{
    widget->ui->comboBoxOffice->setCurrentIndex(-1);
}

void repairTableFilterMenu::clearComboBoxStatus()
{
    widget->ui->comboBoxStatus->setCurrentIndex(-1);
}

void repairTableFilterMenu::clearComboBoxEmployee()
{
    widget->ui->comboBoxEmployee->setCurrentIndex(-1);
}

void repairTableFilterMenu::clearComboBoxClients()
{
    widget->ui->comboBoxClients->setCurrentIndex(-1);
}

void repairTableFilterMenu::clearComboBoxX()
{
    widget->ui->comboBoxX->setCurrentIndex(-1);
}
