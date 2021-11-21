#ifndef TABREPAIRNEW_H
#define TABREPAIRNEW_H

#include <QWidget>

namespace Ui {
class tabRepairNew;
}

class tabRepairNew : public QWidget
{
    Q_OBJECT

public:
    explicit tabRepairNew(QWidget *parent = nullptr);
    ~tabRepairNew();

private:
    Ui::tabRepairNew *ui;
};

#endif // TABREPAIRNEW_H
