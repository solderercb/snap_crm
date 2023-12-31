#ifndef SPhone_H
#define SPhone_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include "../models/ssqlquerymodel.h"
#include "../models/sphonemodel.h"
#include "global.h"
#ifdef QT_DEBUG
#include <QRandomGenerator>
#endif

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
    void buttonAddClicked();
    void delPhone(SPhone*);
    void sigEditPhone();
    void phoneEdited(QString);

public:
    enum Buttons{Add = 0, Del = 1, Edit = 2};
    enum Mode{EditableRW = 0, EditableRO, NotEditable};
    explicit SPhone(QWidget *parent = nullptr);
    explicit SPhone(SPhoneModel*, QWidget *parent = nullptr);
    ~SPhone();
    SPhoneModel* model();
    void setModel(SPhoneModel*);
    void setReadOnly(bool stat = true);
    void clear();
#ifdef QT_DEBUG
    void testFill(const int, const QString&);
#endif
    QString phone();
    QString cleanedPhone();
    void setPrimary();
    bool isValid();
    void setButtonVisible(Buttons button, bool state = true);
    void updateButtons();
    int maskIndex();
    void setEditable(const int);

private:
    Ui::SPhone *ui;
    bool m_isPrimary = 0;
    bool m_isReadOnly = 0;
    bool m_notEditable = 0;
    QString m_clearPhone;    // строка символов, которые ввёл пользователь (т. е. текст отображаемый в lineEdit над которым выполнена операция т. н. XOR с заданной маской
    SPhoneModel *m_phoneModel;
    int m_messengers;
    bool m_isValid = 1;

private slots:
    void phoneMaskChanged(int);
    void checkBoxTypeToggled(bool);
    void modelUpdated();
    void buttonDelClicked();
    void buttonEditClicked();
    void numberEdited(QString);
    void numberEditFinished();
    void notesEditFinished();
    void checkBoxViberClicked(bool);
    void checkBoxTelegramClicked(bool);
    void checkBoxWhatsappClicked(bool);
    void checkBoxSMSClicked(bool);
    void guiFontChanged();
};

#endif // SPhone_H
