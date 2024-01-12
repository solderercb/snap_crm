#include "srepairstablefiltermenu.h"
#include "ui_srepairstablefiltermenu.h"
#include "models/ssqlquerymodel.h"
#include "models/sstandarditemmodel.h"

Widget::Widget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButtonOfficeNot->setVisible(false);
    ui->comboBoxOffice->setVisible(false);
    ui->toolButtonClearOffice->setVisible(false);
    ui->pushButtonRfuNot->setVisible(false);
    ui->comboBoxRfu->setVisible(false);
    ui->toolButtonClearRfu->setVisible(false);
    ui->widgetPeriod->setRefreshButtonVisible(false);
    ui->widgetPeriod->setMovingInterval(SPeriodSelector::Year);

    QIcon checkBoxIcon;
    checkBoxIcon.addFile(QString::fromUtf8(":/icons/light/checkbox-unchecked.png"), QSize(), QIcon::Normal, QIcon::Off);
    checkBoxIcon.addFile(QString::fromUtf8(":/icons/light/checkbox-excl.png"), QSize(), QIcon::Normal, QIcon::On);
    initNotConditionWidget(checkBoxIcon, ui->pushButtonOnlyCurrentUsersRepairsNot);
    initNotConditionWidget(checkBoxIcon, ui->pushButtonOfficeNot);
    initNotConditionWidget(checkBoxIcon, ui->pushButtonRepairStateNot);
    initNotConditionWidget(checkBoxIcon, ui->pushButtonEmployeeNot);
    initNotConditionWidget(checkBoxIcon, ui->pushButtonClientsCatNot);
    initNotConditionWidget(checkBoxIcon, ui->pushButtonRfuNot);
    initNotConditionWidget(checkBoxIcon, ui->pushButtonPeriodNot);
}

Widget::~Widget()
{
    delete ui;
}

Ui::Widget *Widget::Ui()
{
    return ui;
}

bool Widget::event(QEvent *e)
{
    if (e->type() == QEvent::Hide)
    {
        emit hidden();
    }
    return QFrame::event(e);
}

void Widget::initNotConditionWidget(const QIcon &icon, QPushButton *button)
{
    button->setCheckable(true);
    button->setIcon(icon);
    button->setStyleSheet(QString::fromUtf8("QPushButton {"
                                            "  background-color: rgba(240, 240, 240, 0);"
                                            "}"));
}

SRepairsTableFilterMenu::SRepairsTableFilterMenu(QWidget * parent) : QWidgetAction(parent)
{
}

void SRepairsTableFilterMenu::updateFilterStateFlag()
{
    m_filter->filterActive = 0;

    m_filter->filterActive |= (m_filter->office >= 0);
    m_filter->filterActive |= (m_filter->clientsCat >= 0);
    m_filter->filterActive |= (m_filter->employee >= 0);
    m_filter->filterActive |= (m_filter->repairState >= 0);
    m_filter->filterActive |= !m_filter->periodBegin.isEmpty();
    m_filter->filterActive |= !m_filter->periodEnd.isEmpty();
    m_filter->filterActive |= m_filter->onlyCurrentUsersRepairs;
    //    m_filter->filterActive |= (m_filter->rfu >= 0);
}

QWidget* SRepairsTableFilterMenu::createWidget(QWidget *parent)
{
    widget = new Widget(parent);
    ui = static_cast<Widget*>(widget)->Ui();
    hideEventFilter = 1;   // при закрытии всплывающего меню почему-то генерируется два события Hide
    // после создания виджета устанавливаем модели данных для комбобоксов
    if (officeModelBuf)
    {
        ui->pushButtonOfficeNot->setVisible(true);
        ui->comboBoxOffice->setVisible(true);
        ui->toolButtonClearOffice->setVisible(true);
        ui->pushButtonOfficeNot->setChecked(m_filter->officeNot);
        ui->comboBoxOffice->setModel(static_cast<SStandardItemModel*>(officeModelBuf));
        ui->comboBoxOffice->setCurrentIndex(officeModelBuf->rowByDatabaseID(m_filter->office));
    }
    if (repairStateModelBuf)
    {
        ui->pushButtonRepairStateNot->setChecked(m_filter->repairStateNot);
        ui->comboBoxRepairState->setModel(static_cast<SStandardItemModel*>(repairStateModelBuf));
        ui->comboBoxRepairState->setCurrentIndex(repairStateModelBuf->rowByDatabaseID(m_filter->repairState));
    }
    if (employeeModelBuf)
    {
        ui->pushButtonEmployeeNot->setChecked(m_filter->employeeNot);
        ui->comboBoxEmployee->setModel(static_cast<SStandardItemModel*>(employeeModelBuf));
        ui->comboBoxEmployee->setCurrentIndex(employeeModelBuf->rowByDatabaseID(m_filter->employee));
    }
    if (clientsCatModelBuf)
    {
        ui->pushButtonClientsCatNot->setChecked(m_filter->clientsCatNot);
        ui->comboBoxClientsCat->setModel(clientsCatModelBuf);
        ui->comboBoxClientsCat->setCurrentIndex(m_filter->clientsCat);
    }
//    if (rfuModelBuf)
//    {
//        ui->pushButtonRfuNot->setVisible(true);
//        ui->comboBoxRfu->setVisible(true);
//        ui->toolButtonClearRfu->setVisible(true);
//        ui->pushButtonRfuNot->setChecked(m_filter->rfuNot);
//        ui->comboBoxRfu->setModel(rfuModelBuf);
//        ui->comboBoxRfu->setCurrentIndex(-1);
//        ui->comboBoxRfu->setCurrentIndex(m_filter->rfu.toInt());
//    }
    if (m_filter)
    {
        QDateTime tmp;

        ui->pushButtonOnlyCurrentUsersRepairsNot->setChecked(m_filter->onlyCurrentUsersRepairsNot);
        ui->checkBoxOnlyCurrentUsersRepairs->setChecked(m_filter->onlyCurrentUsersRepairs);

        ui->pushButtonPeriodNot->setChecked(m_filter->periodNot);
        ui->widgetPeriod->setMovingInterval((SPeriodSelector::MovingInterval)m_filter->periodMovingInterval);
        tmp = QDateTime::fromString(m_filter->periodBegin, "yyyy-MM-dd hh:mm:ss");
        tmp.setTimeZone(QTimeZone::utc());
        ui->widgetPeriod->setPeriodBegin(tmp);
        tmp = QDateTime::fromString(m_filter->periodEnd, "yyyy-MM-dd hh:mm:ss");
        tmp.setTimeZone(QTimeZone::utc());
        ui->widgetPeriod->setPeriodEnd(tmp);
    }

    connect(widget, SIGNAL(hidden()), this, SLOT(slotHidden()));
    connect(ui->toolButtonClearOffice, &QToolButton::clicked, this, &SRepairsTableFilterMenu::clearComboBoxOffice);
    connect(ui->toolButtonClearState, &QToolButton::clicked, this, &SRepairsTableFilterMenu::clearComboBoxStatus);
    connect(ui->toolButtonClearEmployee, &QToolButton::clicked, this, &SRepairsTableFilterMenu::clearComboBoxEmployee);
    connect(ui->toolButtonClearClientsCat, &QToolButton::clicked, this, &SRepairsTableFilterMenu::clearComboBoxClients);
//    connect(ui->toolButtonClearRfu, &QToolButton::clicked, this, &SRepairsTableFilterMenu::clearcomboBoxRfu);
    connect(ui->toolButtonClearPeriod, &QToolButton::clicked, this, &SRepairsTableFilterMenu::clearPeriod);
    connect(ui->pushButtonApply, &QPushButton::clicked, [=]{/*this->releaseWidget(widget); */parent->close();});

    return widget;
}

void SRepairsTableFilterMenu::slotHidden()
{
    if (hideEventFilter)
    {
        hideEventFilter = 0;
        if (m_filter)
        {
            m_filter->onlyCurrentUsersRepairsNot = ui->pushButtonOnlyCurrentUsersRepairsNot->isChecked();
            m_filter->onlyCurrentUsersRepairs = ui->checkBoxOnlyCurrentUsersRepairs->isChecked();

            m_filter->officeNot = ui->pushButtonOfficeNot->isChecked();
            if (officeModelBuf && ui->comboBoxOffice->currentIndex() >= 0)
                m_filter->office = officeModelBuf->databaseIDByRow(ui->comboBoxOffice->currentIndex());
            else
                m_filter->office = -1;

            m_filter->repairStateNot = ui->pushButtonRepairStateNot->isChecked();
            if (repairStateModelBuf && ui->comboBoxRepairState->currentIndex() >= 0)
                m_filter->repairState = repairStateModelBuf->databaseIDByRow(ui->comboBoxRepairState->currentIndex());
            else
                m_filter->repairState = -1;

            m_filter->employeeNot = ui->pushButtonEmployeeNot->isChecked();
            if (employeeModelBuf && ui->comboBoxEmployee->currentIndex() >= 0)
                m_filter->employee = employeeModelBuf->databaseIDByRow(ui->comboBoxEmployee->currentIndex());
            else
                m_filter->employee = -1;

            m_filter->clientsCatNot = ui->pushButtonClientsCatNot->isChecked();
            if (clientsCatModelBuf)
                m_filter->clientsCat = ui->comboBoxClientsCat->currentIndex();

            m_filter->periodNot = ui->pushButtonPeriodNot->isChecked();
            m_filter->periodBegin = ui->widgetPeriod->periodBegin();
            m_filter->periodEnd = ui->widgetPeriod->periodEnd();
            m_filter->periodMovingInterval = ui->widgetPeriod->movingInterval();

//            m_filter->rfuNot = ui->pushButtonRfuNot->isChecked();
//            m_filter->rfu = ui->comboBoxRfu->currentIndex();
        }
        updateFilterStateFlag();
        emit hidden();
    }
}

void SRepairsTableFilterMenu::clearComboBoxOffice()
{
    ui->pushButtonOfficeNot->setChecked(false);
    ui->comboBoxOffice->setCurrentIndex(-1);
}

void SRepairsTableFilterMenu::clearComboBoxStatus()
{
    ui->pushButtonRepairStateNot->setChecked(false);
    ui->comboBoxRepairState->setCurrentIndex(-1);
}

void SRepairsTableFilterMenu::clearComboBoxEmployee()
{
    ui->pushButtonEmployeeNot->setChecked(false);
    ui->comboBoxEmployee->setCurrentIndex(-1);
}

void SRepairsTableFilterMenu::clearComboBoxClients()
{
    ui->pushButtonClientsCatNot->setChecked(false);
    ui->comboBoxClientsCat->setCurrentIndex(-1);
}

void SRepairsTableFilterMenu::clearPeriod()
{
    ui->pushButtonPeriodNot->setChecked(false);
    ui->widgetPeriod->setPeriodBegin(QDate());
    ui->widgetPeriod->setPeriodEnd(QDate());
}

//void SRepairsTableFilterMenu::clearcomboBoxRfu()
//{
//    ui->comboBoxRfu->setCurrentIndex(-1);
//}
