#include "ssinglerowjmodel.h"
#include <QStandardItem>
#include <SLogRecordModel>

SSingleRowJModel::SSingleRowJModel(QObject *parent) : SSingleRowModel(parent)
{
    i_logRecord = std::make_unique<SLogRecordModel>(this);
}

SSingleRowJModel::~SSingleRowJModel()
{
}

/* Добавление текста журнала
 * При создании/обновлении записи может измениться несколько полей за раз,
 * а изменения некотрых полей должны обязательно записываться в журнал
 */
void SSingleRowJModel::appendLogText(const QString &text)
{
    if(text.isEmpty())
        return;

    i_logTexts.insert(QString::number(i_logTexts.size()), text);
}

/*  Добавление текста журнала с возможностью переопределить порядок записи.
 *  Метод appendLogText(const QString&) в качестве ключа использует порядковый номер
 *  записи ("0", "1", "2", ...). Если какая-то из записей журнала должа быть сделана
 *  раньше, нужно указать ключ, который при алфивитной сортировке будет выше чисел,
 *  например, "!".
*/
void SSingleRowJModel::appendLogText(const QString &text, const QString &order_key)
{
    i_logTexts.insert(order_key, text);
}

void SSingleRowJModel::removeLogText(const QString &key)
{
    i_logTexts.remove(key);
}

bool SSingleRowJModel::commit()
{
    SSingleRowModel::commit();
    commitLogs();
    return 1;
}

bool SSingleRowJModel::commitLogs()
{
    if(i_logTexts.isEmpty())
        return 1;

    QString logText;
    updateLogAssociatedRecId();
    foreach(logText, i_logTexts)
    {
        i_logRecord->set_text(logText);
        i_logRecord->commit();
    }

    i_logTexts.clear();
    return 1;
}

/* Метод должен быть переопределён в наследующем классе для установки ID записи
 * в соответствующее поле i_logText (ремонт, клиент. и т. п.)
 */
void SSingleRowJModel::updateLogAssociatedRecId()
{
    QMessageLogger(__FILE__, __LINE__, nullptr).warning("ASSERT failure in %s: \"%s\", file %s, line %d",
                                                        metaObject()->className(),
                                                        "Probably associated ID for i_logText not set",
                                                        __FILE__,
                                                        __LINE__);
}

/* Метод получения данных для отчетов LimeReport
 * При инициализации источника данных сначала происходит сбор сведений о всех полях (info.dataType = 2; info.index = ключ контейнера reportColumnHeader).
 * При рендеринге получение данных происходит по названию (значение элемента контейнера reportColumnHeader), а info.index соответсвует строке (применимо
 * для табличных данных, например список работ и деталей).
 * IsEmpty и HasNext, похоже, уже не используются и оставлены для совместимости
 */
void SSingleRowJModel::reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    switch (info.dataType)
    {
        case LimeReport::CallbackInfo::IsEmpty: data = 0; break;
        case LimeReport::CallbackInfo::HasNext: data = 0; break;
        case LimeReport::CallbackInfo::ColumnHeaderData: data = reportColumnHeader(info.index); break;
        case LimeReport::CallbackInfo::ColumnData: reportData(info.columnName, data); break;
        case LimeReport::CallbackInfo::ColumnCount: data = reportColumnCount(); break;
        case LimeReport::CallbackInfo::RowCount: data = 1; break;   // всегда одна строка
    }
}

/* "Навигация" по модели данных
 * В данном случае не имеет никакого смысла, т. к. строка единственная
*/
void SSingleRowJModel::reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result)
{
    Q_UNUSED(type);
    result = 1;
}

void SSingleRowJModel::setDatabase(const QSqlDatabase &database)
{
    SSingleRowModel::setDatabase(database);
    i_logRecord->setDatabase(database);
}
