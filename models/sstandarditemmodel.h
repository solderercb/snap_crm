#ifndef SSTANDARDITEMMODEL_H
#define SSTANDARDITEMMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <SAbstractItemModel>
#include <QMetaEnum>

class SSingleRowJModel;

class SStandardItemModel : public QStandardItemModel, public SAbstractItemModel
{
    Q_OBJECT
    friend class TClassTest;
    friend class StoreSaleModel;
public:
    explicit SStandardItemModel(QObject *parent = nullptr);
    ~SStandardItemModel();
    static SStandardItemModel* modelFromEnum(QMetaEnum &_enum, QString (*contextTr)(const char *, const char *, int) = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    std::shared_ptr<SSingleRowJModel> cacheItem(const int row) const;
    void setCacheItem(const int row, std::shared_ptr<SSingleRowJModel> model);
    void clear();
    void clearCache();
    virtual bool isDirty();
    QSqlDatabase loadConnection() const;
    QSqlDatabase commitConnection() const;
    void setConnection(const QSqlDatabase &conn);
protected:
    std::shared_ptr<QSqlDatabase> m_connection;
    bool isSingleRowModelExists(const int row);
    virtual std::shared_ptr<SSingleRowJModel> singleRowModel(const int) {return nullptr;};
private:
    QMap<int, std::shared_ptr<SSingleRowJModel>> m_modified;
};

class MapperForSingleRowModel
{
public:
    virtual int singleRowModelIndex(const int tableModelIndex) = 0;
    virtual ~MapperForSingleRowModel() = default;
    QVariant extraData(const int index) const {
        QVariant value;
        if(!m_extraFields.contains(index))
            return value;
        return m_extraFields.value(index);
    };
    virtual void initSingleRowModelField(const int, const QVariant&){}; // запись данных в контейнер m_record
    virtual bool setSingleRowModelData(const int, const QVariant&){return 1;};  // запись данных в кэш
    std::optional<int> setExtraData(const int index, const QVariant &value) { // возвращает std::nullopt если данные попадают в контейнер m_extraFields (т. е. SingleRowModel для этой строки не содержит поля, соответствующего index)
        int srm_index = singleRowModelIndex(index);
        if(srm_index < 0)
        {
            m_extraFields.insert(index, value);
            return std::nullopt;
        }
        return srm_index;
    };
private:
    QMap<int, QVariant> m_extraFields;  // доп. данные для таблицы (всё, что не попадёт в SSingleRowModel, но необходимо при отображении)
};


#endif // SSTANDARDITEMMODEL_H
