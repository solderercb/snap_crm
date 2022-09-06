#include "sphonesmodel.h"

SPhonesModel::SPhonesModel(int client, QObject *parent) :
    QObject(parent)
{
    m_client = client;
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
        dummyPhone->setClient(m_client);
        add(dummyPhone);
        markUpdated();
    }

    return m_phonesList.first();
}

QList<SPhoneModel *> SPhonesModel::phonesList()
{
    return m_phonesList;
}

bool SPhonesModel::load(int client)
{
    m_client = client;

    clear();
    query = new QSqlQuery(QSqlDatabase::database("connMain"));

    query->exec(QUERY_SEL_CLIENT_PHONES(client));
    while(query->next())
    {
        phoneItemHandler(query->record());
    }

    delete query;
    markUpdated(false);

    return 0;
}

void SPhonesModel::add(SPhoneModel *phone)
{
    connect(phone,SIGNAL(markedPrimary(SPhoneModel*)),this,SLOT(switchPrimaryPhone(SPhoneModel*)));

    m_phonesList.append(phone);
    phone->setClient(m_client);
}

void SPhonesModel::remove(SPhoneModel *phone)
{
    m_removeList.append(phone);
    int modelIndex = m_phonesList.indexOf(phone);
    m_phonesList.removeAt(modelIndex);
}

bool SPhonesModel::isEmpty()
{
    return m_phonesList.isEmpty();
}

void SPhonesModel::setClient(const int id)
{
    m_client = id;
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
            throw 1;
    }

    if(logRecord != nullptr)
    {
        if(!logRecord->commit())
            throw 1;        // запись в журнал об изменении основного номера
        logRecord->deleteLater();
    }

    while( !m_removeList.isEmpty() )
    {
        item = m_removeList.last();
        if(!item->delDBRecord())
            throw 1;

        m_removeList.removeLast();
        item->deleteLater();
    }

    markUpdated(false);

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

bool SPhonesModel::isUpdated()
{
    return m_updated;
}

void SPhonesModel::markUpdated(bool state)
{
    m_updated = state;
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
    if(logRecord == nullptr)
        logRecord = new SLogRecordModel();  // запись в журнал о смене основного номера телефона
    logRecord->setClient(m_client);
    logRecord->setType(SLogRecordModel::Client);
    logRecord->setText(tr("Номер %1 задан основным").arg(newPrimaryPhone->phone()));
    foreach(item, m_phonesList)
    {
        if(item == newPrimaryPhone)
            continue;

        item->setPrimary(false);
    }

}

