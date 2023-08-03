#include "sphones.h"
#include "ui_sphones.h"

SPhones::SPhones(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPhones)
{
    ui->setupUi(this);
}

SPhones::SPhones(SPhonesModel *model, QWidget *parent) :
    SPhones(parent)
{
    setModel(model);
}

SPhones::~SPhones()
{
    clear();
    delete ui;
}

SPhoneModel *SPhones::phone(int item)
{
    return m_phonesModel->phonesList().at(item);
}

void SPhones::setModel(SPhonesModel *model)
{
    m_phonesModel = model;
    slotModelUpdated();
    connect(m_phonesModel,SIGNAL(modelUpdated()),this,SLOT(slotModelUpdated()));
}

void SPhones::addForm(SPhoneModel *model)
{
    SPhone *phoneForm = new SPhone(this);
    if(m_notEditable)
        phoneForm->setEditable(SPhone::NotEditable);
    phoneForm->setModel(model);
    connect(phoneForm,SIGNAL(buttonAddClicked()),this,SLOT(addPhone()));
    connect(phoneForm,SIGNAL(delPhone(SPhone*)),this,SLOT(delPhone(SPhone*)));
    connect(phoneForm,SIGNAL(sigEditPhone()),this,SLOT(updateFormsButtons()));
    connect(phoneForm,SIGNAL(sigEditPhone()),this,SLOT(markPhonesModelUpdated()));
    connect(phoneForm,SIGNAL(markedPrimary()),model,SLOT(setPrimaryUi()));
    connect(phoneForm,SIGNAL(phoneEdited(QString)),this,SIGNAL(primaryPhoneEdited(QString)));

    m_isPrimary = model->type();
    if(m_isPrimary)
        m_primaryForm = phoneForm;  // форма с основным телефоном только одна; задача определения типа лежит на методах класса SPhonesModel

    m_phoneFormsList.append(phoneForm);
    ui->gridLayout_2->addWidget(phoneForm);
}

/*  Скрытие и отображение кнопок на всех формах:
 *  кнопка Add должна быть только на последней (нижней) форме;
 *  кнопка Del не должна отображаться на форме основного телефона;
 */
void SPhones::updateFormsButtons()
{
    QList<SPhone*>::const_iterator i = m_phoneFormsList.constBegin();
    while(i != m_phoneFormsList.constEnd()-1)
    {
        (*i)->setButtonVisible(SPhone::Add, false);
        i++;
    }
    (*i)->setButtonVisible(SPhone::Add, true);
}

void SPhones::markPhonesModelUpdated()
{
    m_phonesModel->markUpdated();
}

void SPhones::slotModelUpdated()
{
    deleteAllForms();

    SPhoneModel *phoneModel;
    m_phoneModelsList = m_phonesModel->phonesList();
    foreach(phoneModel, m_phoneModelsList)
    {
        addForm(phoneModel);
        if(!permissions->viewClients)   // при ограниченных правах номер не отображается и достаточно только одной формы с надписью "no permissions"
            return;
    }
    if(m_phoneModelsList.isEmpty() & !m_notEditable)
        addForm(m_phonesModel->primary());
}

void SPhones::clear()
{
    deleteAllForms();
}

void SPhones::deleteAllForms()
{
    SPhone *phoneForm;
    while( !m_phoneFormsList.isEmpty() )
    {
        phoneForm = m_phoneFormsList.last();
        m_phoneFormsList.removeLast();
        delete phoneForm;
    }
}

bool SPhones::isValid()
{
    bool ret = 1;
    QList<SPhone*>::const_iterator i = m_phoneFormsList.constBegin();
    while(i != m_phoneFormsList.constEnd())
    {
        ret &= (*i)->isValid();
        i++;
        if(!permissions->viewClients)   // при ограниченных правах отображается только одна форма и проверять остальные не нужно
            return ret;
    }
    return ret;
}

SPhone *SPhones::primary()
{
    return m_primaryForm;
}

QList<SPhone *> SPhones::forms()
{
    return m_phoneFormsList;
}

void SPhones::setEditable(const int state)
{
    if(state == SPhone::NotEditable)
    {
        m_notEditable = 1;
    }
    else
        m_notEditable = 0;
}

/*  SLOT
 */
void SPhones::addPhone()
{
    QList<SPhoneModel*> list;
    m_phonesModel->add(new SPhoneModel(m_phonesModel));
    list = m_phonesModel->phonesList();
    addForm(list.last());
    updateFormsButtons();
}

/*  SLOT
 */
void SPhones::delPhone(SPhone *form)
{
    m_phonesModel->remove(form->model());
    int formIndex = m_phoneFormsList.indexOf(form);
    m_phoneFormsList.removeAt(formIndex);
    delete form;
    updateFormsButtons();
}

