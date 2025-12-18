#include "sphonesmodel.h"
#include <QMetaProperty>
#include <SSqlQueryModel>
#include <ProjectGlobals>
#include <SPhoneModel>
#include <SLogRecordModel>

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
    if(m_phonesList.isEmpty())
    {
        SPhoneModel *dummyPhone = new SPhoneModel(this);

        if(m_client)   // если в БД нет телефонных номеров клиента
        {
            dummyPhone->setPrimaryKey(1);
            dummyPhone->setCacheData(SPhoneModel::C_client, m_client, SSingleRowModelBase::ModifiedField::Executed); // чтобы модель осталась чистой после вызова SPhonesModel::add()
        }
        else    // если новый клиент
        {
            dummyPhone->set_primary();  // Первый всегда основной
        }
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

    m_client = client;
    auto q = std::make_unique<QSqlQuery>(QSqlDatabase::database(TdConn::main()));
    auto dummy = std::make_unique<SPhoneModel>();

    QString query = dummy->fieldsForSelectQuery();
    query.prepend("SELECT\n");
    query.append(QString("\n"\
                         "FROM `tel`\n"\
                         "WHERE `customer` = %1\n"\
                         "ORDER BY `type` DESC, `id` DESC;")
                 .arg(client));

    q->exec(query);
    while(q->next())
    {
        sqlRecordHandler(q->record());
    }

    return 0;
}

void SPhonesModel::add(SPhoneModel *phone)
{
    connect(phone, &SPhoneModel::markedPrimary, this, &SPhonesModel::switchPrimaryPhone);

    m_phonesList.append(phone);
    phone->set_client(m_client);
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
        item->set_client(id);
    }
}

bool SPhonesModel::commit()
{
    SPhoneModel *item;
    foreach(item, m_phonesList)
    {
        item->commit();
    }

    if(m_newPrimaryPhone != nullptr)
    {
        // запись в журнал о смене основного номера телефона
        SLogRecordModel logRecord;
        logRecord.set_client(m_client);
        logRecord.set_type(SLogRecordModel::Client);
        logRecord.set_text(tr("Номер %1 задан основным").arg(m_newPrimaryPhone->phone()));
        m_newPrimaryPhone = nullptr;
        logRecord.commit();
    }

    while( !m_removeList.isEmpty() )
    {
        item = m_removeList.last();
        item->delDBRecord();

        m_removeList.removeLast();
        item->deleteLater();
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
    m_client = 0;
    m_updated = 0;
}

bool SPhonesModel::isUpdated()
{
    SPhoneModel *item;
    foreach(item, m_phonesList)
    {
        if(item->isDirty())
            return 1;
    }

    return 0;
}

void SPhonesModel::reset()
{
    clear();
    primary();
    emit modelUpdated();
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
    if(m_reportPhoneIndex == -1)
        return QString();

    return m_phonesList.at(m_reportPhoneIndex)->phone();
}

void SPhonesModel::initDemo()
{
    SPhoneModel *primary = new SPhoneModel();
    primary->set_maskIndex(clientPhoneTypesModel->index(0, 1).data().toInt());
    primary->set_phone("123-45-67");
    SPhoneModel *secondary = new SPhoneModel();
    secondary->set_maskIndex(clientPhoneTypesModel->index(0, 1).data().toInt());
    secondary->set_phone("765-43-21");
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
    m_newPrimaryPhone = newPrimaryPhone;
    foreach(item, m_phonesList)
    {
        if(item == newPrimaryPhone)
            continue;

        item->set_primary(false);
    }

}

/* Метод получения данных для отчетов LimeReport
 * Смотри описание метода с таким же названием в классе SSingleRowJModel
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
    result = 0;

    if(!m_phonesList.count())
    {
        m_reportPhoneIndex = -1;
        return;
    }

    if(type == LimeReport::CallbackInfo::First)
        m_reportPhoneIndex = 0;
    else
    {
        if(m_reportPhoneIndex+1 >= m_phonesList.count())
        {
            return;
        }
        m_reportPhoneIndex++;
    }
    result = 1;
}
