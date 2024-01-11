#ifndef SDATEEDIT_H
#define SDATEEDIT_H

#include <QDateEdit>
#include <QWidget>
#include <QDebug>

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
    QValidator::State validate(QString &text, int &pos) const override;
    QDate date() const;
    void setDate(const QDate &date);
    bool event(QEvent *event) override;
    void setDisplayFormat(const QString &format);
protected:
    void fixup(QString &input) const override;
private:
    QDate m_date;
public slots:
    void overrideDateChanged(const QDate &date);
};

#endif // SDATEEDIT_H
