#include "sphonemodel.h"

SPhoneModel::SPhoneModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "mask" << "customer";

    i_logRecord->setType(SLogRecordModel::Client);
}

SPhoneModel::SPhoneModel(const QSqlRecord &record, QObject *parent) : SPhoneModel(parent)
{
    m_id = record.value("id").toInt();
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
}

SPhoneModel::~SPhoneModel()
{
}

bool SPhoneModel::commit()
{
    if(m_record)
    {
        // update();
    }
    else
    {
        insert("tel");
    }
    i_logRecord->commit();

    return i_nDBErr;
}

void SPhoneModel::setClient(const int id)
{
    if(m_record)
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
    if(m_record)
    {
        // TODO: запись в журнал "телефон изменён с ххх на ууу"
    }
    else
    {
        i_logRecord->setText("Добавлен номер " + phone);
    }
}

int SPhoneModel::mask()
{
    return m_mask;
}

void SPhoneModel::setMask(const int index)
{
    i_valuesMap.insert("mask", clientPhoneTypesModel->databaseIDByRow(index));
}

/*  Возвращает индекс модели данных, содержащих типы телефонных номеров
 *  Метод предназначен для установки индекса comboBox
 */
int SPhoneModel::maskIndex()
{
    return clientPhoneTypesModel->rowByDatabaseID(mask(), "id");
}

void SPhoneModel::setType(const int type)
{
    i_valuesMap.insert("type", type);
    // Запись в журнал при изменении типа будет осуществляться в классе SPhonesModel
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

void SPhoneModel::setMessengers(const int opt)
{
    if(opt&Messengers::Viber)
        i_valuesMap.insert("viber", 1);
    if(opt&Messengers::Telegram)
        i_valuesMap.insert("telegram", 1);
    if(opt&Messengers::Whatsapp)
        i_valuesMap.insert("whatsapp", 1);
    // TODO: подумать над необходимостью записи в журнал
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
