#ifndef SPHONES_H
#define SPHONES_H

#include <QWidget>
#include <QList>
#include "../models/sphonesmodel.h"
#include "../models/ssqlquerymodel.h"
#include "sphone.h"

namespace Ui {
class SPhones;
}

class SPhones : public QWidget
{
    Q_OBJECT

signals:
    void inputUpdated(QString);
    void typeChanged(int);

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

private:
    Ui::SPhones *ui;
    bool m_isPrimary = 0;
    SPhonesModel *m_phonesModel;
    QList<SPhone*> m_phoneFormsList;
    QList<SPhoneModel*> m_phoneModelsList;
    SPhone *m_primaryForm;
    void addForm(SPhoneModel*);
    void removeForm();
    void updateFormsButtons();

private slots:
    void addPhone();
    void delPhone(SPhone*);
};

#endif // SPHONES_H
