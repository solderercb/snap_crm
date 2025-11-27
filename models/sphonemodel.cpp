#include "sphonemodel.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SComSettings>
#include <SPermissions>
#include <SSqlQueryModel>
#include <QSqlRecord>
#include <QMetaEnum>
#include <SLogRecordModel>

SPhoneModel::SPhoneModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "mask" << "customer";
    i_tableName = "tel";
    setPrimaryKeyIndex(0);

    m_record->setValue(C_mask, clientPhoneTypesModel->record(0).value("id").toInt());  // тип телефона по умолчанию
    m_record->setValue(C_receiveSMS, comSettings->newClientSmsEnabled());
    i_logRecord->set_type(SLogRecordModel::Client);
}

SPhoneModel::SPhoneModel(const QSqlRecord &record, QObject *parent) : SPhoneModel(parent)
{
    m_record = std::make_shared<QSqlRecord>(record);
    i_logRecord->set_client(client());
}

SPhoneModel::~SPhoneModel()
{
}

QString SPhoneModel::fieldsForSelectQuery()
{
    QString fields;

    // TODO: заменить "no permissions" на частично скрытое значение (например, "+7 123 ***-78-90")
    auto viewClient = [=](int index){if(!permissions->viewClients) return restrictedFieldForSelectQuery(index); return fieldForSelectQuery(index);};
    for(int i = 0; i < rec()->count(); i++)
    {
        if(!fields.isEmpty())
            fields.append(",\n");

        switch (i)
        {
            case C_phone:
            case C_phoneClean:
            case C_note: fields.append(viewClient(i)); break;
            default: fields.append(fieldForSelectQuery(i)); break;
        }
    }

    return fields;
}

bool SPhoneModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_receiveSMS, comSettings->newClientSmsEnabled());
    }

    SSingleRowJModel::commit();

    return 1;
}

// TODO: придумать механизм сокрытия части номера, если у пользователя недостаточно прав

/*  Установка id маски телефонного номера по индексу
*/
void SPhoneModel::set_maskIndex(const int index)
{
    set_mask(clientPhoneTypesModel->databaseIDByRow(index));
}

/*  Возвращает индекс модели данных, содержащих типы телефонных номеров
 *  Метод предназначен для установки индекса comboBox
 */
int SPhoneModel::maskIndex()
{
    if(!permissions->viewClients)
        return -1;

    return clientPhoneTypesModel->rowByDatabaseID(mask(), "id");
}

int SPhoneModel::messengers()
{
    int messengers = 0;

    if(viber())
        messengers |= Viber;
    if(telegram())
        messengers |= Telegram;
    if(whatsapp())
        messengers |= Whatsapp;

    return messengers;
}

/*  Изменение связи номера с учетными записями мессенджеров
 */
void SPhoneModel::set_messengers(const int options, bool state)
{
    if(options&Messengers::Viber)
        set_viber(state);
    else if(options&Messengers::Telegram)
        set_telegram(state);
    else if(options&Messengers::Whatsapp)
        set_whatsapp(state);
}

bool SPhoneModel::isNew()
{
    if(isPrimaryKeyValid())
        return false;
    return true;
}

QString SPhoneModel::cleanupPhone(const QString &phone)
{
    QString cleanPhone;

    for (int i = 0; i < phone.length(); i++ )
    {
        if(phone.at(i).isDigit())
            cleanPhone.append(phone.at(i));
    }

    return cleanPhone;
}

void SPhoneModel::typeChanged(const int type)
{
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
void SPhoneModel::set_primary(int primary)
{
    set_type(primary);
    if(primary == Primary)
        emit modelUpdated();   // при программной установке основным тоже нужно изменить состояние checkBox'а

}

bool SPhoneModel::delDBRecord()
{
    if(isPrimaryKeyValid())
    {
        i_logRecord->set_text(tr("Номер %1 удалён").arg(phone()));
        i_logRecord->commit();
        return SSingleRowJModel::del();
    }

    return 1;
}

/*  SLOT: Установка основным номером через UI (т. е. включение checkBox'а мышкой)
 */
void SPhoneModel::setPrimaryUi()
{
    set_type(SPhoneModel::Primary);
}

void SPhoneModel::updateLogAssociatedRecId()
{
}

void SPhoneModel::phoneChanged(const QString &phone)
{
    set_phoneClean(cleanupPhone(phone));
    if(isPrimaryKeyValid())
    {
        auto old = commitedData(C_phone).value_or(QVariant());
        appendLogText(tr("Номер изменён с %1 на %2").arg(old.toString(), phone), "Phone_changed");
    }
    else
    {
        appendLogText(tr("Добавлен номер %1").arg(phone), "Phone_added");
    }
}

void SPhoneModel::messangersChanged(const int index, bool state)
{
    static int enumIndex = staticMetaObject.indexOfEnumerator("Messengers");
    QMetaEnum enumerator = staticMetaObject.enumerator(enumIndex);

    int messengerId = 0;
    switch (index)
    {
        case C_viber:    messengerId = Messengers::Viber; break;
        case C_telegram: messengerId = Messengers::Telegram; break;
        case C_whatsapp: messengerId = Messengers::Whatsapp; break;
        default: return; break;
    }

    QString messenger = enumerator.valueToKey(messengerId);
    auto old = commitedData(index).value_or(QVariant());
    if(state != old.toBool())
    {
        if(state)
            appendLogText(tr("Установлена связь номера %1 с уч. записью %2").arg(phone(), messenger), messenger);
        else
            appendLogText(tr("Разорвана связь номера %1 с уч. записью %2").arg(phone(), messenger), messenger);
    }
    else
    {
        removeLogText(messenger);
    }

}

void SPhoneModel::logDataChange(const int index, const QVariant &data)
{
    switch (index)
    {
        case C_phone: phoneChanged(data.toString()); break;
        case C_client: i_logRecord->set_client(data.toInt()); break;
        case C_type: typeChanged(data.toInt()); break;
        case C_viber:
        case C_telegram:
        case C_whatsapp: messangersChanged(index, data.toBool()); break;
        default: break;
    }
}
