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
        sqlRecordHandler(query->record());
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

void SPhonesModel::reset()
{
    clear();
    primary();
    emit modelUpdated();
}

void SPhonesModel::markUpdated(bool state)
{
    m_updated = state;
}

QString SPhonesModel::primaryStr()
{
    if(m_phonesList.count())
        return primary()->phone();

    return "";
}

QString SPhonesModel::phone2()
{
    if(m_phonesList.count() < 2)
        return "";

    QList<SPhoneModel*>::const_iterator i = m_phonesList.constBegin();
    i++;    // первый всегда основной, поэтому сразу к следующему
    if(i != m_phonesList.constEnd())
        return (*i)->phone();

    return "";
}

QString SPhonesModel::phone3()
{
    if(m_phonesList.count() < 3)
        return "";

    QList<SPhoneModel*>::const_iterator i = m_phonesList.constBegin();
    i++;    // первый всегда основной, поэтому сразу к следующему
    i++;
    if(i != m_phonesList.constEnd())
        return (*i)->phone();

    return "";
}

QString SPhonesModel::allPhones()
{
    QStringList list;
    QList<SPhoneModel*>::const_iterator i = m_phonesList.constBegin();
    while(i != m_phonesList.constEnd())
    {
        list << (*i)->phone();
        i++;
    }
    return list.join(", ");
}

QString SPhonesModel::iterativePhone()
{
    return m_phonesList.at(m_reportPhoneIndex)->phone();
}

void SPhonesModel::initDemo()
{
    SPhoneModel *primary = new SPhoneModel();
    primary->setMask(clientPhoneTypesModel->index(0, 1).data().toInt());
    primary->setPhone("123-45-67");
    SPhoneModel *secondary = new SPhoneModel();
    secondary->setMask(clientPhoneTypesModel->index(0, 1).data().toInt());
    secondary->setPhone("765-43-21");
    add(primary);
    add(secondary);
}

SPhoneModel *SPhonesModel::sqlRecordHandler(const QSqlRecord &record)
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

/* Метод получения данных для отчетов LimeReport
 * Смотри описание метода с таким же названием в классе SComRecord
 */
void SPhonesModel::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
//    qDebug().nospace() << "[" << this << "] reportCallbackData() | info.dataType = " << info.dataType << "; info.index = " << info.index << "; info.columnName = " << info.columnName;
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = 0; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = metaObject()->property(info.index + 1).name(); break;
        case LimeReport::CallbackInfo::ColumnData: data = metaObject()->property( metaObject()->indexOfProperty(info.columnName.toLocal8Bit()) ).read(this); break;
        case LimeReport::CallbackInfo::ColumnCount: data = metaObject()->propertyCount() - 1; break;
        case LimeReport::CallbackInfo::RowCount: data = m_phonesList.count(); break;
    }
}

/* "Навигация" по модели данных
 * В данном случае не имеет никакого смысла, т. к. использование модели данных в качестве
*/
void SPhonesModel::reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
//    qDebug().nospace() << "[" << this << "] reportCallbackDataChangePos() | type = " << type;
    if(type == LimeReport::CallbackInfo::First)
        m_reportPhoneIndex = 0;
    else
    {
        if(m_reportPhoneIndex+1 >= m_phonesList.count())
        {
            result = 0;
            return;
        }
        m_reportPhoneIndex++;
    }
    result = 1;
}
