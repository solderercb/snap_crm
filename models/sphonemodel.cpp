#include "sphonemodel.h"

SPhoneModel::SPhoneModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "mask" << "customer";
    i_tableName = "tel";
    i_idColumnName = "id";

    i_logRecord->setType(SLogRecordModel::Client);

    m_mask = clientPhoneTypesModel->record(0).value("id").toInt();  // тип телефона по умолчанию
}

SPhoneModel::SPhoneModel(const QSqlRecord &record, QObject *parent) : SPhoneModel(parent)
{
    i_id = record.value("id").toInt();
    m_client_id = record.value("customer").toInt();
    i_logRecord->setClient(m_client_id);
    m_phone = record.value("phone").toString();
    m_phoneClean = record.value("phone_clean").toString();
    m_mask = record.value("mask").toInt();
    m_type = record.value("type").toInt();
    m_note = record.value("note").toString();
    m_messengers = (record.value("viber").toBool()?Messengers::Viber:0) | \
                   (record.value("telegram").toBool()?Messengers::Telegram:0) | \
                   (record.value("whatsapp").toBool()?Messengers::Whatsapp:0);
    m_initialMessengers = m_messengers;
    m_receiveSMS = record.value("notify").toBool();
}

SPhoneModel::~SPhoneModel()
{
}

bool SPhoneModel::commit()
{
    if(i_id)
    {
        update();
    }
    else
    {
        if(!i_valuesMap.contains("notify"))
            i_valuesMap.insert("notify", comSettings->value("newClientSmsEnabled", 0));    // TODO: добавить значение по умолчанию в общие настройки
        insert();
    }
    commitLogs();

    return i_nErr;
}

void SPhoneModel::setClient(const int id)
{
    if(i_id)
        return;

    i_valuesMap.insert("customer", id);
    i_logRecord->setClient(id);
}

QString SPhoneModel::phone()
{
    // TODO: придумать механизм сокрытия части номера, если у пользователя недостаточно прав
    return m_phone;
}

void SPhoneModel::setPhone(const QString &phone)
{
    i_valuesMap.insert("phone", phone);
    i_valuesMap.insert("phone_clean", cleanPhone(phone));
    if(i_id)
    {
        appendLogText(tr("Номер изменён с %1 на %2").arg(m_phone, phone), "Phone_changed");
    }
    else
    {
        appendLogText(tr("Добавлен номер %1").arg(phone), "Phone_added");
    }
    m_phone = phone;
}

int SPhoneModel::mask()
{
    return m_mask;
}

/*  Установка id маски телефонного номера по индексу
*/
void SPhoneModel::setMask(const int index)
{
    m_mask = clientPhoneTypesModel->databaseIDByRow(index);
    i_valuesMap.insert("mask", m_mask);
}

/*  Возвращает индекс модели данных, содержащих типы телефонных номеров
 *  Метод предназначен для установки индекса comboBox
 */
int SPhoneModel::maskIndex()
{
    return clientPhoneTypesModel->rowByDatabaseID(mask(), "id");
}

int SPhoneModel::type()
{
    return m_type;
}

QString SPhoneModel::note()
{
    return m_note;
}


void SPhoneModel::setNote(const QString &note)
{
    i_valuesMap.insert("note", note);
    // TODO: подумать над необходимостью записи в журнал; например, "добавлен номер ххх с комментарием ууу"
}

int SPhoneModel::messengers()
{
    return m_messengers;
}

/*  Изменение связи номера с учетной записью мессенджера
 *  Допускается только одна опция за раз
 *  Первый вызов метода для определённой опции всегда подразумевает изменение состояния этой опции;
 *  при втором вызове нужно удалить элементы из массива значений и массива записей в журнал
 */
void SPhoneModel::setMessengers(const int option, bool state)
{
    static int enumIndex = staticMetaObject.indexOfEnumerator("Messengers");
    QMetaEnum enumerator = staticMetaObject.enumerator(enumIndex);
    for (int i = 0; i<enumerator.keyCount(); ++i)
    {
        if(option == enumerator.value(i))
        {
            bool initialOptionState = ((m_initialMessengers&enumerator.value(i)) != 0);
            if(initialOptionState != state)
            {
                i_valuesMap.insert(enumerator.key(i), state );
                if(state)
                    appendLogText(tr("Установлена связь номера %1 с уч. записью %2").arg(m_phone, enumerator.key(i)), enumerator.key(i));
                else
                    appendLogText(tr("Разорвана связь номера %1 с уч. записью %2").arg(m_phone, enumerator.key(i)), enumerator.key(i));
            }
            else
            {
                i_valuesMap.remove(enumerator.key(i));
                removeLogText(enumerator.key(i));
            }
            break;
        }
    }
}

bool SPhoneModel::isEmpty()
{
    if(i_id)
        return false;
    return true;
}

QString SPhoneModel::cleanPhone(const QString &phone)
{
    QString cleanPhone;

    for (int i = 0; i < phone.length(); i++ )  // переписываем "чистый" номер
    {
        if(phone.at(i).isDigit())
            cleanPhone.append(phone.at(i));
    }

    return cleanPhone;
}

void SPhoneModel::setType(const int type)
{
    m_type = type;
    i_valuesMap.insert("type", type);

    if(type == Primary)
    {
        emit markedPrimary(this);
    }
    else
        emit modelUpdated();
}

/*  Установка основным номером через вызов метода (т. е. программно)
 *  Можно установить номер не основным, передав false
 */
void SPhoneModel::setPrimary(int primary)
{
    setType(primary);
    if(primary == Primary)
        emit modelUpdated();   // при программной установке основным тоже нужно изменить состояние checkBox'а

}

bool SPhoneModel::delDBRecord()
{
    i_logRecord->setText(tr("Номер %1 удалён").arg(m_phone));
    i_logRecord->commit();
    return SComRecord::del();
}

bool SPhoneModel::receiveSMS()
{
    return m_receiveSMS;
}

void SPhoneModel::setReceiveSMS(bool state)
{
    i_valuesMap.insert("notify", state);
}

/*  SLOT: Установка основным номером через UI (т. е. включение checkBox'а мышкой)
 */
void SPhoneModel::setPrimaryUi()
{
    setType(SPhoneModel::Primary);
}

