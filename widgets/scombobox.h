#ifndef MCOMBOBOX_H
#define MCOMBOBOX_H

#include <QComboBox>

class SComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SComboBox(QWidget *parent = nullptr);
    ~SComboBox();
    void disableWheelEvent(bool);
    void wheelEvent(QWheelEvent*);
    void resizeEvent(QResizeEvent*);
    int currentDbId();

private:
    bool wheelEventOn = 1;

signals:

};

#endif // MCOMBOBOX_H
