#include "sdateedit.h"

SDateEdit::SDateEdit(QWidget *parent) :
    QDateEdit(parent)
{
    QDateEdit::setDate(QDate::currentDate());   // до подключения сигнала, чтобы выпадающий календарь по умолчанию отображал текущий месяц
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
    if(m_date.isValid())
        return QDateEdit::textFromDateTime(QDateTime(m_date, QTime()));

    return "--.--.----";
}

QValidator::State SDateEdit::validate(QString &text, int &pos) const
{
    if(m_date.isValid())
        return QDateEdit::validate(text, pos);

    return QValidator::Invalid;
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

void SDateEdit::fixup(QString &input) const
{
    QDateEdit::fixup(input);
}

void SDateEdit::overrideDateChanged(const QDate &date)
{
    m_date = date;
}

