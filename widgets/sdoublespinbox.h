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
};

#endif // SDOUBLESPINBOX_H
