#ifndef TABCLIENTSUMMARY_H
#define TABCLIENTSUMMARY_H

#include <QWidget>

namespace Ui {
class tabClientSummary;
}

class tabClientSummary : public QWidget
{
    Q_OBJECT

public:
    explicit tabClientSummary(QWidget *parent = nullptr);
    ~tabClientSummary();

private:
    Ui::tabClientSummary *ui;
};

#endif // TABCLIENTSUMMARY_H
