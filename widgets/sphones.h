#ifndef SPHONES_H
#define SPHONES_H

#include <QWidget>

class SPhone;
class SPhoneModel;
class SPhonesModel;

namespace Ui {
class SPhones;
}

class SPhones : public QWidget
{
    Q_OBJECT

signals:
    void primaryPhoneEdited(QString);

public:
    explicit SPhones(QWidget *parent = nullptr);
    explicit SPhones(SPhonesModel* model, QWidget *parent = nullptr);
    ~SPhones();
    SPhoneModel* phone(int);
    void setModel(SPhonesModel*);
    void clear();
    void deleteAllForms();
    bool isValid();
    SPhone* primary();
    QList<SPhone*> forms();
    void setEditable(const int);

private:
    Ui::SPhones *ui;
    bool m_isPrimary = 0;
    bool m_notEditable = 0;
    SPhonesModel *m_phonesModel;
    QList<SPhone*> m_phoneFormsList;
    QList<SPhoneModel*> m_phoneModelsList;
    SPhone *m_primaryForm;
    void addForm(SPhoneModel*);

private slots:
    void delPhone(SPhone*);
    void updateFormsButtons();
    void slotModelUpdated();
public slots:
    void addPhone();
};

#endif // SPHONES_H
