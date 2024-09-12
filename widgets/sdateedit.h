#ifndef SDATEEDIT_H
#define SDATEEDIT_H

#include <QDateEdit>
#include <QWidget>
#include <QDebug>

#define DATE_EDIT_REFRESH_TIMER 30000

class SDateEdit : public QDateEdit
{
    Q_OBJECT
Q_SIGNALS:
    void dateChanged(const QDate &date);
public:
    explicit SDateEdit(QWidget *parent = nullptr);
    explicit SDateEdit(const QDate &date, QWidget *parent = nullptr);
    ~SDateEdit();
    QString textFromDateTime(const QDateTime &dateTime) const override;
    QDate date() const;
    void setDate(const QDate &date);
    bool event(QEvent *event) override;
    void setDisplayFormat(const QString &format);
    void setDateTrackingEnabled(bool state = true);
    void setAllowBackDate(bool state = true);
    void setAllowFutureDate(bool state = true);
    void clear() override;
protected:
    void fixup(QString &input) const override;
private:
    QDate m_date;
    QTimer *m_refreshTimer = nullptr;
    bool m_dateTrackingEnabled = 0;
    bool m_allowBackDate = 0;
    bool m_allowFutureDate = 0;
public slots:
    void overrideDateChanged(const QDate &date);
private slots:
    void dateRefresh();
};

#endif // SDATEEDIT_H
