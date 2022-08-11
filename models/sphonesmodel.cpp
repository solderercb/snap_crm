#include "sphonesmodel.h"

SPhonesModel::SPhonesModel(int client, QObject *parent) :
    QObject(parent)
{
    if(client)
        load(client);

}

SPhonesModel::~SPhonesModel()
{
    clear();
}

SPhoneModel *SPhonesModel::primary()
{
    if(m_phonesList.isEmpty())  // если в БД нет телефонных номеров клиента
    {
        SPhoneModel *dummyPhone = new SPhoneModel(this);
        dummyPhone->setPrimary();  // Первый всегда основной
        add(dummyPhone);
    }

    return m_phonesList.first();
}

QList<SPhoneModel *> SPhonesModel::phonesList()
{
    return m_phonesList;
}

bool SPhonesModel::load(int client)
{
    clear();
    query = new QSqlQuery(QSqlDatabase::database("connMain"));

    query->exec(QUERY_SEL_CLIENT_PHONES(client));
    while(query->next())
    {
        phoneItemHandler(query->record());
    }

    delete query;
    return 0;
}

void SPhonesModel::add(SPhoneModel *phone)
{
    connect(phone,SIGNAL(markedPrimary(SPhoneModel*)),this,SLOT(switchPrimaryPhone(SPhoneModel*)));

    m_phonesList.append(phone);

}

bool SPhonesModel::isEmpty()
{
    return m_phonesList.isEmpty();
}

void SPhonesModel::setClient(const int id)
{
    SPhoneModel *item;
    foreach(item, m_phonesList)
    {
        item->setClient(id);
    }
}

bool SPhonesModel::commit()
{
    SPhoneModel *item;
    foreach(item, m_phonesList)
    {
        if(!item->commit())
            return 0;
    }
    return 1;
}

void SPhonesModel::clear()
{
    SPhoneModel *phone;
    while( !m_phonesList.isEmpty() )
    {
        phone = m_phonesList.last();
        m_phonesList.removeLast();
        delete phone;
    }
}

SPhoneModel *SPhonesModel::phoneItemHandler(const QSqlRecord &record)
{
    SPhoneModel *item = new SPhoneModel(record, this);
    add(item);
    return item;
}

void SPhonesModel::switchPrimaryPhone(SPhoneModel *newPrimaryPhone)
{
    SPhoneModel *item;
    foreach(item, m_phonesList)
    {
        if(item == newPrimaryPhone)
            continue;

        item->setPrimary(SPhoneModel::Additional);
    }

}

