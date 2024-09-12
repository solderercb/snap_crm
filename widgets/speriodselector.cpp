#include "speriodselector.h"
#include "ui_speriodselector.h"

SPeriodSelector::SPeriodSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPeriodSelector)
{
    ui->setupUi(this);
    initIntervalSelectionMenu();
    ui->dateEditPeriodBegin->setAllowBackDate(true);
    ui->dateEditPeriodEnd->setAllowBackDate(true);
    ui->dateEditPeriodEnd->setAllowFutureDate(true);
    connect(ui->dateEditPeriodBegin, &QDateEdit::dateChanged, this, &SPeriodSelector::periodBeginChanged);
    connect(ui->dateEditPeriodEnd, &QDateEdit::dateChanged, this, &SPeriodSelector::periodEndChanged);
    connect(ui->toolButtonMovePrevPeriod, &QToolButton::clicked, this, &SPeriodSelector::movePrevPeriod);
    connect(ui->toolButtonMoveNextPeriod, &QToolButton::clicked, this, &SPeriodSelector::moveNextPeriod);
    connect(ui->toolButtonDefaultPeriod, &QToolButton::clicked, this, &SPeriodSelector::setDefaultPeriod);
    connect(ui->toolButtonDefaultPeriod, &QToolButton::customContextMenuRequested, this, &SPeriodSelector::intervalMenuRequest);
    connect(ui->toolButtonRefresh, &QToolButton::clicked, this, &SPeriodSelector::refreshButtonClicked);

    setMovingInterval(MovingInterval::Day);
    ui->dateEditPeriodBegin->setDate(QDate());
    ui->dateEditPeriodEnd->setDate(QDate());
}

SPeriodSelector::~SPeriodSelector()
{
    delete ui;
}

/* Преобразование даты с локальным часовым поясом в дату и время с UTC
 * Возвращаемые данные предназначены непосредственно для передачи в запросах к БД
*/
QDateTime SPeriodSelector::dateToUTCTime(const QDate &date)
{
    QDateTime time;
    time.setDate(date);

    return time.toUTC();
}

/* Дата начала периода
 * Возвращает QDateTime в формате UTC
*/
QString SPeriodSelector::periodBegin()
{
    return dateToUTCTime(m_periodBegin).toString("yyyy-MM-dd hh:mm:ss");
}

/* Установка даты начала периода
 * Передаваемое значение преобразуется в локальное для корректного отображения даты
*/
void SPeriodSelector::setPeriodBegin(const QDateTime &dateTime)
{
    setPeriodBegin(dateTime.toLocalTime().date());
}

/* Установка даты начала периода
 * Значение передаётся в виджет и должно соответствовать локальному
 * Началом периода будет 00 часов 00 минут 00 секунд 000мсек переданной даты
*/
void SPeriodSelector::setPeriodBegin(const QDate date)
{
    ui->dateEditPeriodBegin->setDate(date);
}

/* Дата конца периода
 * Возвращает QDateTime в формате UTC
*/
QString SPeriodSelector::periodEnd()
{
    return dateToUTCTime(m_periodEnd.addDays(1)).toString("yyyy-MM-dd hh:mm:ss");
}

/* Установка даты конца периода
 * Передаваемое значение преобразуется в локальное для корректного отображения даты
*/
void SPeriodSelector::setPeriodEnd(const QDateTime &dateTime)
{
    setPeriodEnd(dateTime.toLocalTime().date().addDays(-1));
}

/* Установка даты конца периода
 * Значение передаётся в виджет и должно соответствовать локальному
 * Концом периода будет 23 часа 59 минут 59 секунд 999мсек переданной даты
*/
void SPeriodSelector::setPeriodEnd(const QDate &date)
{
    ui->dateEditPeriodEnd->setDate(date);
}

/* Установка дат начала и конца периода
 * Передаваемые значения преобразуется в UTC
*/
void SPeriodSelector::setPeriod(const QDateTime &begin, const QDateTime &end)
{
    setPeriodBegin(begin);
    setPeriodEnd(end);
}

/* Установка периода по умолчанию по дате и интервалу
 * Передаваемая дата будет преобразована в локальную
*/
void SPeriodSelector::setPeriod(const QDateTime &time, const MovingInterval interval)
{
    bool backup = m_isPeriodAjustable;
    m_isPeriodAjustable = 0;
    setMovingInterval(interval);
    setPeriodBegin(time);
    m_isPeriodAjustable = backup;
}

int SPeriodSelector::movingInterval()
{
    return (int)m_movingInterval;
}

/* Установка периода по умолчанию
 * В соответствии со значением в переменной m_movingInterval
 * производится установка значений в виджетах QDateEdit
 * Если m_isPeriodAjustable == 1 период устанавливается относительно текущей даты,
 * в противном случае относительно даты, которую выбрал пользователь
*/
void SPeriodSelector::setDefaultPeriod()
{
    QDate date;

    if(m_isPeriodAjustable || !m_periodBegin.isValid())
        date = QDate::currentDate();
    else
        date = m_periodBegin;

    ui->dateEditPeriodBegin->blockSignals(true);
    switch (m_movingInterval)
    {
        case MovingInterval::Week: m_periodBegin = date.addDays(-date.dayOfWeek() + 1); m_periodEnd = m_periodBegin.addDays(6); break;
        case MovingInterval::Day:
        case MovingInterval::Month: m_periodBegin = QDate(date.year(), date.month(), 1); m_periodEnd = m_periodBegin.addDays(date.daysInMonth() - 1); break;
        case MovingInterval::Quarter: m_periodEnd = lastDateOfQuarter(date); m_periodBegin = firstDateOfQuarter(date); break;
        case MovingInterval::Year: m_periodBegin = QDate(date.year(), 1, 1); m_periodEnd = QDate(date.year(), 12, 31); break;
        default: break;
    }
    ui->dateEditPeriodBegin->setDate(m_periodBegin);
    ui->dateEditPeriodBegin->blockSignals(false);
    ui->dateEditPeriodEnd->setDate(m_periodEnd);
}

void SPeriodSelector::intervalMenuActionClick(int id)
{
    setMovingInterval((MovingInterval)id);
}

void SPeriodSelector::intervalMenuRequest(const QPoint &pos)
{
    if(m_isPeriodAjustable)
        m_intervalMenu->popup(ui->toolButtonDefaultPeriod->mapToGlobal(pos));
}

/* Установка интервала по умолчанию
 * Значение используется при перемещении между периодами и при установке периода по уполчанию
*/
void SPeriodSelector::setMovingInterval(const MovingInterval interval)
{
    QString toolTip;
    m_movingInterval = interval;
    switch (m_movingInterval)
    {
        case MovingInterval::Week: toolTip = tr("С начала текущей недели"); break;
        case MovingInterval::Day:
        case MovingInterval::Month: toolTip = tr("С начала текущего месяца"); break;
        case MovingInterval::Quarter: toolTip = tr("С начала текущего квартала"); break;
        case MovingInterval::Year: toolTip = tr("С начала текущего года"); break;
        default: break;
    }

    ui->toolButtonDefaultPeriod->setToolTip(toolTip);
    if(!m_isPeriodAjustable)
        setDefaultPeriod();
}

void SPeriodSelector::setPeriodEndWidgetEditable(const bool state)
{
    ui->dateEditPeriodEnd->setReadOnly(!state);
}

/* Скрытие/отображение кнопки установки периода по умолчанию
 * Если кнопка скрыта, подразумевается что период равен интервалу
*/
void SPeriodSelector::setDefaultPeriodButtonVisible(const bool state)
{
    ui->toolButtonDefaultPeriod->setVisible(state);
    setPeriodAjustable(state);
}

void SPeriodSelector::setRefreshButtonVisible(const bool state)
{
    ui->toolButtonRefresh->setVisible(state);
}

void SPeriodSelector::hideRefreshButton()
{
    setRefreshButtonVisible(false);
}

/* Включение/выключение режима жесткой привязки периода к началу недели/месяца/квартала/года
 *
*/
void SPeriodSelector::setPeriodAjustable(bool state)
{
    m_isPeriodAjustable = state;
    if(!m_isPeriodAjustable)
    {
        setDefaultPeriod();
    }
    setPeriodEndWidgetEditable(state);
}

QDate SPeriodSelector::firstDateOfQuarter(const QDate &date)
{
    switch (date.month())
    {
        case 1:
        case 2:
        case 3: return QDate(date.year(), 1, 1);
        case 4:
        case 5:
        case 6: return QDate(date.year(), 4, 1);
        case 7:
        case 8:
        case 9: return QDate(date.year(), 7, 1);
        case 10:
        case 11:
        case 12: return QDate(date.year(), 10, 1);
    }
    return QDate::currentDate();
}

QDate SPeriodSelector::lastDateOfQuarter(const QDate &date)
{
    switch (date.month())
    {
        case 1:
        case 2:
        case 3: return QDate(date.year(), 3, 31);
        case 4:
        case 5:
        case 6: return QDate(date.year(), 6, 30);
        case 7:
        case 8:
        case 9: return QDate(date.year(), 9, 30);
        case 10:
        case 11:
        case 12: return QDate(date.year(), 12, 31);
    }
    return QDate::currentDate();
}

void SPeriodSelector::movePeriod(const int direction)
{
    QDate newPeriodBegin, newPeriodEnd;
    switch (m_movingInterval)
    {
        case MovingInterval::Day:  newPeriodBegin = m_periodBegin.addDays(direction); newPeriodEnd = m_periodEnd.addDays(direction); break;
        case MovingInterval::Week: newPeriodBegin = m_periodBegin.addDays(direction*7); newPeriodEnd = m_periodEnd.addDays(direction*7); break;
        case MovingInterval::Month: newPeriodBegin = m_periodBegin.addMonths(direction); newPeriodEnd = m_periodEnd.addMonths(direction); break;
        case MovingInterval::Quarter: newPeriodBegin = m_periodBegin.addMonths(direction*3); newPeriodEnd = m_periodEnd.addMonths(direction*3); break;
        case MovingInterval::Year: newPeriodBegin = QDate(m_periodBegin.year() + direction, 1, 1); newPeriodEnd = QDate(m_periodEnd.year() + direction, 12, 31); break;
        default: break;
    }
    ui->dateEditPeriodBegin->setDate(newPeriodBegin);
    ui->dateEditPeriodEnd->setDate(newPeriodEnd);
}

void SPeriodSelector::initIntervalSelectionMenu()
{
    m_intervalMenu = new QMenu(this);
    QAction *action;
    m_signalMapper = new QSignalMapper(this);
    QMetaEnum intervals = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("MovingInterval"));

    QObject::connect(m_signalMapper, &QSignalMapper::mappedInt, this, &SPeriodSelector::intervalMenuActionClick);

    for(int i = 0; i < intervals.keyCount(); i++)
    {
        action = new QAction(tr(intervals.key(i))/*, this*/);
        m_intervalMenu->addAction(action);
        m_signalMapper->setMapping(action, intervals.value(i));
        connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    }

    ui->toolButtonDefaultPeriod->setMenu(m_intervalMenu);
    ui->toolButtonDefaultPeriod->setContextMenuPolicy(Qt::CustomContextMenu);

//    QAction *workshop_new = new QAction(tr("Принять"), this);
//    workshop_menu->addAction(workshop_new);
//    workshop_new->setVisible();
    //    QObject::connect(workshop_new,SIGNAL(triggered()),this,SLOT(createTabRepairNew()));
}

void SPeriodSelector::translate()
{
    tr("Day");
    tr("Week");
    tr("Month");
    tr("Quarter");
    tr("Year");
}

void SPeriodSelector::periodBeginChanged(const QDate date)
{
    Q_UNUSED(date)
    m_periodBegin = ui->dateEditPeriodBegin->date();
    if(!m_isPeriodAjustable)
        setDefaultPeriod();
}

void SPeriodSelector::periodEndChanged(const QDate date)
{
    Q_UNUSED(date)
    m_periodEnd = ui->dateEditPeriodEnd->date();
}

void SPeriodSelector::movePrevPeriod()
{
    movePeriod(-1);
}

void SPeriodSelector::moveNextPeriod()
{
    movePeriod(1);
}
