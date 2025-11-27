#include "sworkmodel.h"
#include <SSaleTableModel>
#include <SSqlQueryModel>
#include <ProjectGlobals>
#include <SStandardItemModel>
#include <SStoreItemModel>
#include <SLogRecordModel>

SWorkModel::SWorkModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_tableName = "works";
    i_obligatoryFields << "user" << "name" << "warranty";
    setPrimaryKeyIndex(0);
    i_logRecord->set_type(SLogRecordModel::Repair);
    connect(this, &SSingleRowModel::beginDataChange, this, &SWorkModel::setDataRework);
}

/* Контруктор записей в журнал при добавлении новой работы.
 * Т. к. записи в журнал включают значения нескольких полей, то на разных этапах
 * установки значений текст может быть неполноценным. Чтобы сообщения бы законченными
 * нужно вызвать этот метод после изменения каждого поля.
 * Генерирование сообщений непосредственно перед коммитом, когда значения всех полей уже
 * наверняка заданы, затрудняет автоматическое тестирование и требует отказаться от очистки
 * буфера сообщений после коммита, что в свою очередь вызовет другие проблемы.
 * .
*/
void SWorkModel::constructInitialLogMsg(const int column)
{
    QString logText;
    QString key; // порядок сообщений и препятствование дублированию
    switch(column)
    {
        case C_name: key = "!addRename"; logText = tr("Добавлена работа \"%1\" стоимостью %2").arg(name(), sysLocale.toCurrencyString(price())); break;
        case C_warranty: key = "warranty"; logText = tr("Срок гарантии на работу \"%1\" установлен по умолчанию (\"%2\")").arg(name(), warrantyTermsModel->getDisplayRole(warranty(), 1)); break;
        case C_user: key = "engineer"; logText = tr("Исполнителем работы \"%1\" назначен %2").arg(name(), usersModel->getDisplayRole(user(), 1)); break;
        default: return;
    }

    addAdminModeMarkToLogText(logText);
    appendLogText(logText, key);
}

void SWorkModel::repairChanged(const QVariant &data)
{
    i_logRecord->setData(SLogRecordModel::C_repair, data);
    constructInitialLogMsg(C_name);
}

void SWorkModel::userChanged()
{
    QString logText;

    if(int old = commitedData(C_user).value_or(QVariant()).toInt())
        logText = tr("Исполнитель работы \"%1\" изменён с %2 на %3").arg(name(), usersModel->getDisplayRole(old), usersModel->getDisplayRole(user()));
    else
    {
        constructInitialLogMsg(C_user);
        return;
    }

    addAdminModeMarkToLogText(logText);
    appendLogText(logText, "engineer");
}

void SWorkModel::nameChanged()
{
    QString logText;

    auto old = commitedData(C_name).value_or(QVariant()).toString();
    if(!old.isEmpty())
        logText = tr("Название работы изменено с \"%1\" на \"%2\"").arg(old, name());
    else if(isPrimaryKeyValid())   // для новой произвольной работы или работы из прайс-листа запись в журнал не нужна
        logText = tr("Название работы изменено на \"%1\"").arg(name());
    else
    {
        constructInitialLogMsg(C_name);
        constructInitialLogMsg(C_user);
        constructInitialLogMsg(C_warranty);
        return;
    }

    addAdminModeMarkToLogText(logText);
    appendLogText(logText, "!addRename");
}

void SWorkModel::priceChanged()
{
    QString logText;

    auto old = commitedData(C_price).value_or(QVariant(QVariant::Double));
    if(!old.isNull())
        logText = tr("Стоимость работы \"%1\" изменёна с %2 на %3").arg(name(), sysLocale.toCurrencyString(old.toDouble()), sysLocale.toCurrencyString(price()));
    else if(isPrimaryKeyValid())
        logText = tr("Стоимость работы \"%1\" установлена %2").arg(name(), sysLocale.toCurrencyString(price()));
    else
    {
        constructInitialLogMsg(C_name);
        return;
    }

    addAdminModeMarkToLogText(logText);
    appendLogText(logText, "priceChange");
}

void SWorkModel::countChanged()
{
    auto old = commitedData(C_count);
    if(!old)
        return;

    QString logText = tr("Кол-во \"%1\" изменёно с %2 на %3").arg(name()).arg(old->toInt()).arg(count());

    addAdminModeMarkToLogText(logText);
    appendLogText(logText, "qty");
}

void SWorkModel::warrantyChanged()
{
    QString logText;

    auto old = commitedData(C_warranty).value_or(QVariant());
    if(!old.toString().isEmpty())
        logText = tr("Срок гарантии на работу \"%1\" изменён с \"%2\" на \"%3\"").arg(name(), warrantyTermsModel->getDisplayRole(old.toInt(), 1), warrantyTermsModel->getDisplayRole(warranty(), 1));
    else if(isPrimaryKeyValid() || (!isPrimaryKeyValid() && warranty() != 0) )   // первое изменение срока гарантии при автосохранении списка или установка срока гарантии до первого сохранения списка
        logText = tr("Срок гарантии на работу \"%1\" установлен \"%2\"").arg(name(), warrantyTermsModel->getDisplayRole(warranty(), 1));
    else
    {
        constructInitialLogMsg(C_warranty);
        return;
    }

    addAdminModeMarkToLogText(logText);
    appendLogText(logText, "warranty");
}

/* При добавлении работы из прайс-листа и инициализации модели сначала задаётся
 * процент заработка инженера payRepair, а затем id работы; поле payRepairQuick
 * нужно инициализировать дополнительно
*/
void SWorkModel::priceIdChanged()
{
    set_payRepairQuick(payRepair());
}

bool SWorkModel::remove()
{
    bool ret = 1;
    i_logTexts.clear();
    QString logText = tr("Удалена работа \"%1\" стоимостью %2").arg(name(), sysLocale.toCurrencyString(price()));

    addAdminModeMarkToLogText(logText);
    i_logRecord->set_text(logText);
    ret &= del();
    ret &= i_logRecord->commit();

    return ret;
}

bool SWorkModel::commit()
{
    if(!isPrimaryKeyValid())
    {
        initMandatoryField(C_created, QDateTime::currentDateTime());
    }
    return SSingleRowJModel::commit();
}

void SWorkModel::setDataRework(const int index, QVariant &data)
{
    switch(index)
    {
        case C_priceId: if(!data.toInt()) data = QVariant(); break;
        case C_user: userBeforeChange(data.toInt()); break;
        default: break;
    }
}

void SWorkModel::logDataChange(const int index, const QVariant &data)
{
    Q_UNUSED(data)

    switch(index)
    {
        case C_user: userChanged(); break;
        case C_repair: repairChanged(data); break;
        case C_name: nameChanged(); break;
        case C_price: priceChanged(); break;
        case C_count: countChanged(); break;
        case C_warranty: warrantyChanged(); break;
        case C_priceId: priceIdChanged(); break;
        default: ;
    }
}

void SWorkModel::addAdminModeMarkToLogText(QString &text)
{
    if(text.isEmpty())
        return;

    if(!parent())
        return;

    SSaleTableModel *model = static_cast<SSaleTableModel*>(parent());
    if(model->state() == WorkshopSaleModel::State::Adm)
        text.prepend("[A] ");
}

void SWorkModel::userBeforeChange(const int userId)
{
    if(priceId())
        return;

    set_payRepair(usersSalaryTaxesModel->value(userId, "id", "pay_repair").toInt());
    set_payRepairQuick(usersSalaryTaxesModel->value(userId, "id", "pay_repair_quick").toInt());
}

void SWorkModel::updateLogAssociatedRecId()
{
    // установка id ремонта происходит в конструкторе; метод переопределён для подавления предупреждения
}

void SWorkModel::initFieldWithPrevLoaded(const int index, const QVariant &value)
{
    SSingleRowJModel::initFieldWithPrevLoaded(index, value);

    switch (index)
    {
        case C_repair: i_logRecord->setData(SLogRecordModel::C_repair, value); break;
        default: break;
    }
}

bool SWorkModel::setData(const int index, const QVariant &data)
{
    if(index < 0)
        return 1;

    return SSingleRowJModel::setData(index, data);
}
