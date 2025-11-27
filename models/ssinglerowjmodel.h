/*  Вспомогательный класс для работы с записью таблицы БД
 *  Содержит некоторие ширпотребные методы и объекты, которые,
 *  в свою очередь, тоже наследуют базовый класс (например,
 *  объект класса SLogRecordModel)
 */
#ifndef SCOMRECORD_NEW_H
#define SCOMRECORD_NEW_H

#include <SSingleRowModel>
#include <LimeReport>

class QStandardItem;
class SLogRecordModel;

class SSingleRowJModel : public SSingleRowModel
{
    Q_OBJECT
public:
    explicit SSingleRowJModel(QObject *parent = nullptr);
    ~SSingleRowJModel();
    void appendLogText(const QString&);
    void appendLogText(const QString&, const QString &disambiguation);
    void removeLogText(const QString &disambiguation);
    virtual void initDemo(){};
    void setDatabase(const QSqlDatabase &database) override;
    virtual bool commit() override;
protected:
    bool commitLogs();
    virtual void updateLogAssociatedRecId();
    QMap<QString, QString> i_logTexts;
    std::unique_ptr<SLogRecordModel> i_logRecord;
public slots:
    void reportCallbackData(const LimeReport::CallbackInfo &info, QVariant &data);
    void reportCallbackDataChangePos(const LimeReport::CallbackInfo::ChangePosType &type, bool &result);
};

#endif // SCOMRECORD_NEW_H
