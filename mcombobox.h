#ifndef MCOMBOBOX_H
#define MCOMBOBOX_H

#include <QComboBox>

class MComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MComboBox(QWidget *parent = nullptr);
    ~MComboBox();
    void disableWheelEvent(bool);
    void wheelEvent(QWheelEvent*);
    void resizeEvent(QResizeEvent*);

private:
    bool wheelEventOn = 1;
    const QString commonComboBoxStyleSheet = "QComboBox {  border: 1px solid gray;  padding: 1px 18px 1px 3px;}\
            QComboBox::drop-down {  border: 0px;}\
            QComboBox::down-arrow{  image: url(down-arrow.png);  width: 16px;  height: 20px;}\
            QComboBox::hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
            QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
signals:

};

#endif // MCOMBOBOX_H
