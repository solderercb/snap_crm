#include "scommentsmodel.h"

SCommentsModel::SCommentsModel(QObject *parent) :
    SStandardItemModel(parent),
    m_queryData(new QSqlQueryModel)
{
    connect(m_queryData, SIGNAL(modelReset()), this, SLOT(sqlDataChanged()));
}

SCommentsModel::~SCommentsModel()
{
}

void SCommentsModel::load(const int id)
{
    setObjId(id);
    switch(m_mode)
    {
        case SCommentModel::Mode::Repair: setQuery(QUERY_SEL_COMMENTS("remont",id)); break;
        case SCommentModel::Mode::Client: setQuery(QUERY_SEL_COMMENTS("client",id)); break;
        case SCommentModel::Mode::Task: setQuery(QUERY_SEL_COMMENTS("task_id",id)); break;
        case SCommentModel::Mode::PartRequest: setQuery(QUERY_SEL_COMMENTS("part_request",id)); break;
    }
}

bool SCommentsModel::add(const QString &text)
{
    bool nErr = 1;
    int newId = 0;
    QList<QStandardItem*> row;
    QStandardItem *item;
    item = new QStandardItem(); // id
    item->setData(1, SCommentModel::DataRoles::Changed);
    row << item;
    item = new QStandardItem();
    item->setData(QDateTime::currentDateTimeUtc(), Qt::EditRole);
    item->setData(1, SCommentModel::DataRoles::Changed);
    row << item;
    item = new QStandardItem(QString::number(userDbData->id));
    item->setData(1, SCommentModel::DataRoles::Changed);
    row << item;
    item = new QStandardItem(text);
    item->setData(1, SCommentModel::DataRoles::Changed);
    row << item;

    SCommentModel *comment = new SCommentModel(row);
    comment->setObjId(m_mode, m_objId);
    nErr = comment->commit();
    newId = comment->id();
    delete comment;

    if(!nErr)
        throw 1;

    this->insertRow(0, row);
    QStandardItemModel::setData(this->index(0, SCommentModel::ColId), newId);
    clearChangedFlagForAllField();

    return 1;
}

bool SCommentsModel::remove(const int row)
{
    bool nErr = 1;
    QList<QStandardItem*> rowItems = this->row(row);
    QStandardItem* item;

    SCommentModel *comment = new SCommentModel(rowItems);
    nErr = comment->remove();
    delete comment;

    if(!nErr)
        throw 1;

    for(int i = rowItems.count() - 1; i >= 0; i--)
    {
        item = rowItems.at(i);
        rowItems.removeAt(i);
        delete item;
    }
    removeRow(row);

    return nErr;
}

QVariant SCommentsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
            case SCommentModel::Columns::ColUser: return allUsersMap->value(QStandardItemModel::data(index, role).toInt());
            case SCommentModel::Columns::ColCreated: return createdStr(index);
        }
    }

    return QStandardItemModel::data(index, role);
}

bool SCommentsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool nErr = 1;
    nErr &= QStandardItemModel::setData(index, value, role);
    nErr &= QStandardItemModel::setData(index, 1, SComRecord::Changed); // пометка поля изменённым
    nErr &= QStandardItemModel::setData(this->index(index.row(), SCommentModel::ColId), 1, SComRecord::Changed); // пометка строки изменённой

    return nErr;
}

int SCommentsModel::mode()
{
    return m_mode;
}

void SCommentsModel::setMode(const int mode)
{
    m_mode = mode;
}

void SCommentsModel::setObjId(const int id)
{
    m_objId = id;
}

bool SCommentsModel::isColumnHidden(const int column)
{
    return (m_hiddenColumns >> column)&0x01;
}

int SCommentsModel::recordId(const int row)
{
    return QStandardItemModel::data(this->index(row, SCommentModel::ColId)).toInt();
}

int SCommentsModel::userId(const int row)
{
    return QStandardItemModel::data(this->index(row, SCommentModel::ColUser)).toInt();
}

QDateTime SCommentsModel::created(const int row) const
{
    QDateTime date = QStandardItemModel::data(this->index(row, SCommentModel::ColCreated)).toDateTime();
    date.setTimeZone(QTimeZone::utc());
    return date;
}

QString SCommentsModel::text(const int row)
{
    return QStandardItemModel::data(this->index(row, SCommentModel::ColText)).toString();
}

bool SCommentsModel::setText(const int rowNum, const QString &text)
{
    bool nErr = 1;
    setData(this->index(rowNum, SCommentModel::ColText), text);
    QList<QStandardItem*> rowItems = row(rowNum);
    SCommentModel *comment = new SCommentModel(rowItems);
    comment->setObjId(m_mode, m_objId);
    nErr = comment->commit();
    delete comment;

    if(!nErr)
        throw 1;

    clearChangedFlagForAllField();
    return 1;
}

QString SCommentsModel::createdStr(const QModelIndex &index) const
{
    return created(index.row()).toLocalTime().toString("dd.MM.yyyy hh:mm:ss");
}

void SCommentsModel::clearChangedFlagForAllField()
{
    for(int i = 0; i < rowCount(); i++)
    {
        if(index(i, 0).data(SComRecord::Changed).toBool())
        {
            for(int j = 1; j < columnCount(); j++) // в нулевом столбце — id записи в таблице, он не изменяется средствами программы
            {
                if(index(i, j).data(SComRecord::Changed).toBool())
                {
                    QStandardItemModel::setData(index(i, j), 0, SComRecord::Changed);   // снятие флага о наличии изменений в поле
                    QStandardItemModel::setData(index(i, j), QVariant(), SComRecord::OldValue);   // очистка старого значения
                }
            }
            QStandardItemModel::setData(index(i, SCommentModel::ColId), 0, SComRecord::Changed);   // снятие флага о наличии изменений в строке
        }
    }
}

void SCommentsModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    m_queryData->setQuery(query, db);
}

QList<QStandardItem *> SCommentsModel::row(int row) const
{
    QList<QStandardItem*> rowItems;
    for(int column = 0; column < columnCount(); column++)
    {
        rowItems << QStandardItemModel::item(row, column);
    }

    return rowItems;
}

/* В этом слоте происходит копирование данных из QSqlQueryModel в QStandardItemModel
 * Для загрузки комментариев будет достаточно вызвать метод this->setQuery()
 */
void SCommentsModel::sqlDataChanged()
{
    clear();
    setRowCount(m_queryData->rowCount());
    setColumnCount(m_queryData->columnCount());
    this->blockSignals(true);
    for(int i = 0; i < m_queryData->rowCount(); i++)
    {
        for(int j = 0; j < m_queryData->columnCount(); j++)
        {
            QStandardItemModel::setData(index(i, j), m_queryData->index(i, j).data());
        }
    }
    this->blockSignals(false);
    m_queryData->blockSignals(true);
    m_queryData->clear();
    m_queryData->blockSignals(false);
    endResetModel();
}
