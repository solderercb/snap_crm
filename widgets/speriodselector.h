#ifndef SPERIODSELECTOR_H
#define SPERIODSELECTOR_H

#include <QWidget>

class QMenu;
class QSignalMapper;

namespace Ui {
class SPeriodSelector;
}

class SPeriodSelector : public QWidget
{
    Q_OBJECT
signals:
    void refreshButtonClicked();
public:
    enum MovingInterval {Day, Week, Month, Quarter, Year};
    Q_ENUM(MovingInterval);
    explicit SPeriodSelector(QWidget *parent = nullptr);
    ~SPeriodSelector();
    QString periodBegin();
    void setPeriodBegin(const QDateTime &dateTime);
    void setPeriodBegin(const QDate date);
    QString periodEnd();
    void setPeriodEnd(const QDateTime &dateTime);
    void setPeriodEnd(const QDate &date);
    void setPeriod(const QDateTime &begin, const QDateTime &end);
    void setPeriod(const QDateTime &time, const MovingInterval interval);
    int movingInterval();
    void setMovingInterval(const MovingInterval interval);
    void setPeriodEndWidgetEditable(const bool state = true);
    void setDefaultPeriodButtonVisible(const bool state = true);
    void hideDefaultPeriodButton();
    void setRefreshButtonVisible(const bool state = true);
    void hideRefreshButton();
    void setPeriodAjustable(bool isPeriodAjustable);
private:
    Ui::SPeriodSelector *ui;
    MovingInterval m_movingInterval = Day;      // Интервал по умолчанию
    MovingInterval m_movingIntervalOverride;    // Изменённый интервал по умолчанию
    QDate m_periodBegin;    // Начало периода в локальном часовом поясе
    QDate m_periodEnd;      // Конец периода в локальном часовом поясе; отображаемая дата на 1 день раньше
    bool m_isPeriodAjustable = 1;
    QMenu *m_intervalMenu;
    QSignalMapper *m_signalMapper;
    QDateTime dateToUTCTime(const QDate &date);
    QDate firstDateOfQuarter(const QDate &date);
    QDate lastDateOfQuarter(const QDate &date);
    void movePeriod(const int direction);
    void initIntervalSelectionMenu();
    void translate();
private slots:
    void periodBeginChanged(const QDate date);
    void periodEndChanged(const QDate date);
    void movePrevPeriod();
    void moveNextPeriod();
    void intervalMenuActionClick(int id);
    void intervalMenuRequest(const QPoint &pos);
public slots:
    void setDefaultPeriod();
};

#endif // SPERIODSELECTOR_H
