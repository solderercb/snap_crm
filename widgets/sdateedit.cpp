#include "sdateedit.h"

SDateEdit::SDateEdit(QWidget *parent) :
    QDateEdit(parent)
{
    setDate(QDate::currentDate());   // до подключения сигнала, чтобы выпадающий календарь по умолчанию отображал текущий месяц
    connect(this, &QDateEdit::dateChanged, this, &SDateEdit::overrideDateChanged);
}

SDateEdit::SDateEdit(const QDate &date, QWidget *parent) :
    SDateEdit(parent)
{
    setDate(date);
}

SDateEdit::~SDateEdit()
{
}

QString SDateEdit::textFromDateTime(const QDateTime &dateTime) const
{
    Q_UNUSED(dateTime)

    if(m_date.isValid())
        return QDateEdit::textFromDateTime(QDateTime(m_date, QTime()));

    return "--.--.----";
}

QDate SDateEdit::date() const
{
    return m_date;
}

void SDateEdit::setDate(const QDate &date)
{
    m_date = date;
    QDateEdit::blockSignals(true);
    if(m_date.isValid())
        QDateEdit::setDate(m_date);
    else
        QDateEdit::setDate(QDate::currentDate());
    QDateEdit::blockSignals(false);
    emit QDateEdit::dateChanged(date);
}

bool SDateEdit::event(QEvent *event)
{
    return QDateEdit::event(event);
}

void SDateEdit::setDisplayFormat(const QString &format)
{
    QDateEdit::setDisplayFormat(format);
}

/* Функция автообновления значения виджета
 * Предназначена для случаев когда вкладка, на которой допускается изменение даты создания, открыта несколько дней назад,
 * а операция проведена позже и в БД может быть записана неправильная дата.
 * Примеры таких вкладок: создания РКО/ПКО, создание заявки на закупку;
*/
void SDateEdit::setDateTrackingEnabled(bool state)
{
    m_dateTrackingEnabled = state;

    if(m_dateTrackingEnabled)
    {
        if(!m_refreshTimer)
        {
            m_refreshTimer = new QTimer(this);
            m_refreshTimer->setSingleShot(true);
            connect(m_refreshTimer, &QTimer::timeout, this, &SDateEdit::dateRefresh);
            setDate(QDate::currentDate());
        }
    }
    else
    {
        if(m_refreshTimer)
        {
            disconnect(m_refreshTimer, &QTimer::timeout, this, &SDateEdit::dateRefresh);
            delete m_refreshTimer;
        }
    }
}

void SDateEdit::setAllowBackDate(bool state)
{
    m_allowBackDate = state;
}

void SDateEdit::fixup(QString &input) const
{
    QDateEdit::fixup(input);
}

void SDateEdit::setAllowFutureDate(bool state)
{
    m_allowFutureDate = state;
}

void SDateEdit::clear()
{
    m_date = QDate();
}

void SDateEdit::overrideDateChanged(const QDate &date)
{
    m_date = date;

    if( (!m_allowBackDate && date < QDate::currentDate()) ||
        (!m_allowFutureDate && date >= QDate::currentDate()) ||
        (m_allowFutureDate && date == QDate::currentDate()) )
    {
        QDateEdit::setDate(QDate::currentDate());
        if(m_dateTrackingEnabled)
            m_refreshTimer->start(DATE_EDIT_REFRESH_TIMER);

        return;
    }

    if(m_dateTrackingEnabled)
        m_refreshTimer->stop();
}

void SDateEdit::dateRefresh()
{
    setDate(QDate::currentDate());
    m_refreshTimer->start(DATE_EDIT_REFRESH_TIMER);
}

