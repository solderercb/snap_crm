#include "scomrecord.h"

SComRecord::SComRecord(QObject *parent) : SDatabaseRecord(parent)
{
    i_logRecord = new SLogRecordModel(this);
    i_logTexts = new QMap<QString, QString>();
}

SComRecord::~SComRecord()
{
    delete i_logRecord;
    delete i_logTexts;
}

/* Добавление текста журнала
 * При создании/обновлении записи может измениться несколько полей за раз,
 * а изменения некотрых полей должны обязательно записываться в журнал
 */
void SComRecord::appendLogText(const QString &text)
{
    if(text.isEmpty())
        return;

    i_logTexts->insert(QString::number(i_logTexts->size()), text);
}

/*  Добавление текста журнала с возможностью переопределить порядок записи.
 *  Метод appendLogText(const QString&) в качестве ключа использует порядковый номер
 *  записи ("0", "1", "2", ...). Если какая-то из записей журнала должа быть сделана
 *  раньше, нужно указать ключ, который при алфивитной сортировке будет выше чисел,
 *  например, "!".
*/
void SComRecord::appendLogText(const QString &text, const QString &order_key)
{
    i_logTexts->insert(order_key, text);
}

void SComRecord::removeLogText(const QString &key)
{
    i_logTexts->remove(key);
}

bool SComRecord::commit()
{
    SDatabaseRecord::commit();
    i_nErr &= commitLogs();
    if(i_nErr)
        SDatabaseRecord::setDirty(false);
    return i_nErr;
}

/* Переопределённый метод
 * TODO: Это временное решение; метод будет удалён после переработки всех наследующих
 * классов; ранее при вызове сеттера новое значение записывалось в кэш i_valuesMap, а после
 * успешного выполнения запроса к БД кэш очищался. В тех случаях, когда требовалось получить
 * новое значение до выполнения запроса, обновлялась также приватная переменная. Это
 * требовало более тщательного контроля при разработке и могло приводить к разным ошибкам и
 * сбоям. Новая схема работает по аналогии с кэшем в QSqlTableModel, а переопределённый
 * метод позволит в каждом отдельном классе управлять очисткой i_valuesMap.
*/
void SComRecord::dbErrFlagHandler(bool flushCache)
{
    SDatabaseRecord::dbErrFlagHandler(flushCache);
}

bool SComRecord::commitLogs()
{
    if(i_logTexts->isEmpty())
        return 1;

    QString logText;
    updateLogAssociatedRecId();
    foreach(logText, *i_logTexts)
    {
        i_logRecord->setText(logText);
        i_nErr &= i_logRecord->commit();
    }

    i_logTexts->clear();
    return 1;
}

/*  Метод предназначен для инициализации полей, отмеченных изменёнными, в запросе к БД.
 *  Используется в паре модель_таблицы/модель_записи (например, SSaleTableModel/SStoreSaleItemModel).
*/
void SComRecord::initQueryFields(const QList<QStandardItem *> &record)
{
    for(int i = 1; i < record.count(); i++) // в нулевом столбце — id записи в таблице, он не изменяется средствами программы
    {
        if(!record.at(i)->data(DataRoles::Changed).toBool())
            continue;

        setQueryField(i, record.at(i)->data(Qt::DisplayRole), record.at(i)->data(DataRoles::OldValue));
    }
}

/* Метод должен быть переопределён в наследующем классе для установки ID записи
 * в соответствующее поле i_logText (ремонт, клиент. и т. п.)
 */
void SComRecord::updateLogAssociatedRecId()
{
    QMessageLogger(__FILE__, __LINE__, nullptr).warning("ASSERT failure in %s: \"%s\", file %s, line %d",
                                                        metaObject()->className(),
                                                        "Probably associated ID for i_logText not set",
                                                        __FILE__,
                                                        __LINE__);
}

/* Сброс флага наличия несохранённых изменений должен быть сброшен только после выполнения запросов
 * записи в таблицу logs, поэтому в этом переопределённом методе ничего не нужно делать.
 * TODO: позже нужно переделать; см. комментарий к методу SComRecord::dbErrFlagHandler
*/
void SComRecord::setDirty(const bool state)
{
}

/* Метод получения данных для отчетов LimeReport
 * Для получения данных используется мета-система объектов; при вызове слота происходит вызов метода класса, указанный в параметре Q_PROPERTY READ.
 * Любой Q_OBJECT класс в свойстве с индексом 0 содержит название объекта, поэтому он пропускается.
 * При инициализации такого источника данных сначала происходит сбор сведений о всех полях (info.dataType = 2; info.index = <индекс Q_PROREPTY>); а при рендеринге
 * данные забираются по имени свойства, а info.index соответсвует строке (применимо для табличных данных, например список работ и деталей).
 * IsEmpty и HasNext, похоже, уже не используются и оставлены для совместимости
 */
void SComRecord::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
//    qDebug().nospace() << "[" << this << "] reportCallbackData() | info.dataType = " << info.dataType << "; info.index = " << info.index << "; info.columnName = " << info.columnName;
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = 0; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = metaObject()->property(info.index + 1).name(); break;
        case LimeReport::CallbackInfo::ColumnData: data = metaObject()->property( metaObject()->indexOfProperty(info.columnName.toLocal8Bit()) ).read(this); break;
        case LimeReport::CallbackInfo::ColumnCount: data = metaObject()->propertyCount() - 1; break;
        case LimeReport::CallbackInfo::RowCount: data = 1; break;   // данные клиента — это одна запись из таблицы clients, поэтому всегда одна строка
    }
}

/* "Навигация" по модели данных
 * В данном случае не имеет никакого смысла, т. к. строка единственная
*/
void SComRecord::reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
    Q_UNUSED(type);
    result = 1;
}

