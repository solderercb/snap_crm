#ifndef SPhone_H
#define SPhone_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include "../models/ssqlquerymodel.h"
#include "../models/sphonemodel.h"
#include "global.h"

namespace Ui {
class SPhone;
}

class SCheckBox : public QCheckBox
{
public:
    explicit SCheckBox(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *);
};

class SPhone : public QWidget
{
    Q_OBJECT

signals:
    void markedPrimary();
    void addPhone();
    void maskChanged(int);
    void inputUpdated(QString);

public:
    explicit SPhone(QWidget *parent = nullptr);
    explicit SPhone(SPhoneModel*, QWidget *parent = nullptr);
    ~SPhone();
    void setModel(SPhoneModel*);
    void setReadOnly(bool stat = true);
    void clear();
    void fillPhone(const QSqlRecord&);
    QString phone();
    QString cleanedPhone();
    void setPrimary();
    bool isValid();

private:
    Ui::SPhone *ui;
    QString m_mask;
    QString m_visibleMask;
    bool m_isPrimary = 0;
    QString m_clearPhone;    // строка символов, которые ввёл пользователь (т. е. текст отображаемый в lineEdit над которым выполнена операция т. н. XOR с заданной маской
    SPhoneModel *m_phoneModel;

private slots:
    void phoneMaskChanged(int);
    void checkBoxTypeToggled(bool);
    void setCheckBoxPrimaryState(int secondary = SPhoneModel::Additional);
    void delPhone();
    void editPhone();
    void textChanged(QString);
    void phoneEditFinished();
    void notesEditFinished();
};

#endif // SPhone_H
