#ifndef SDOUBLESPINBOX_H
#define SDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QObject>
#include <QWidget>

class SDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit SDoubleSpinBox(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;
private:
};

#endif // SDOUBLESPINBOX_H
