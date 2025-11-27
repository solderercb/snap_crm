#include "scommentsmodel.h"
#include <SUserSettings>
#include <QTimeZone>
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SCommentModel>

SCommentsModel::SCommentsModel(QObject *parent) :
    SStandardItemModel(parent),
    m_queryData(new QSqlQueryModel),
    m_mode(SCommentModel::NotSet)
{
    m_standardItemModel = this;
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
        case SCommentModel::Mode::Repair: setQuery(QUERY_SEL_COMMENTS("remont",id), loadConnection()); break;
        case SCommentModel::Mode::Client: setQuery(QUERY_SEL_COMMENTS("client",id), loadConnection()); break;
        case SCommentModel::Mode::Task: setQuery(QUERY_SEL_COMMENTS("task_id",id), loadConnection()); break;
        case SCommentModel::Mode::PartRequest: setQuery(QUERY_SEL_COMMENTS("part_request",id), loadConnection()); break;
    }
}

bool SCommentsModel::add(const QString &text)
{
    auto comment = std::make_shared<SCommentModel>();
    comment->setDatabase(commitConnection());
    comment->set_text(text);
    comment->setObjId(m_mode, m_objId);
    if(!comment->commit())
        return 0;

    insertRows(0, 1);
    setCacheItem(0, comment);
//    dataChanged(index(0,0), index(0, SCommentModel::C_text), QVector<int>({Qt::DisplayRole, Qt::EditRole}));

    return 1;
}

/* Инициализация SSingleRowModel ранее загруженными данными из указанной строки
 * Возвращает 0 если в процессе возникла ошибка
*/
void SCommentsModel::initSingleRowModel(const int row, std::shared_ptr<SSingleRowJModel> model)
{
    model->setPrimaryKey(this->data(index(row, SCommentModel::C_id)));
    model->initFieldWithPrevLoaded(SCommentModel::C_created, SStandardItemModel::data(index(row, SCommentModel::C_created)));
    model->initFieldWithPrevLoaded(SCommentModel::C_user, SStandardItemModel::data(index(row, SCommentModel::C_user)));
    model->initFieldWithPrevLoaded(SCommentModel::C_text, SStandardItemModel::data(index(row, SCommentModel::C_text)));
}

bool SCommentsModel::remove(const int row)
{
    bool nErr = 1;

    auto comment = std::make_shared<SCommentModel>();
    comment->setDatabase(commitConnection());
    comment->setPrimaryKey(this->data(index(row, SCommentModel::C_id)));  // для удаления достаточно инициализировать только primarykey
    nErr = comment->remove();

    if(nErr)
        SStandardItemModel::removeRows(row, 1);

    return nErr;
}

QVariant SCommentsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column()) {
            case SCommentModel::Columns::C_user: return userFromId(index);
            case SCommentModel::Columns::C_created: return createdStr(index);
        }
    }

    return SStandardItemModel::data(index, role & 0xFF);
}

bool SCommentsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool nErr = 1;
    nErr &= SStandardItemModel::setData(index, value, role);

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
    return SStandardItemModel::data(this->index(row, SCommentModel::C_id)).toInt();
}

int SCommentsModel::userId(const int row)
{
    return SStandardItemModel::data(this->index(row, SCommentModel::C_user)).toInt();
}

QDateTime SCommentsModel::created(const int row) const
{
    return timestampUtc(index(row, SCommentModel::C_created));
}

QString SCommentsModel::text(const int row)
{
    return SStandardItemModel::data(this->index(row, SCommentModel::C_text)).toString();
}

bool SCommentsModel::setText(const int rowNum, const QString &text)
{
    bool nErr = 1;
    setData(this->index(rowNum, SCommentModel::C_text), text);
    auto comment = cacheItem(rowNum);
    comment->setDatabase(commitConnection());
    nErr = comment->commit();
    return nErr;
}

void SCommentsModel::clear()
{
    SStandardItemModel::clear();
}

std::shared_ptr<SSingleRowJModel> SCommentsModel::singleRowModel(const int row)
{
    auto model = cacheItem(row);
    if(model)
    {
        return model;
    }

    model = std::make_shared<SCommentModel>();
    initSingleRowModel(row, model);
    setCacheItem(row, model);
    return model;
}

QString SCommentsModel::createdStr(const QModelIndex &index) const
{
    return timestampLocal(index).toString("dd.MM.yyyy hh:mm:ss");
}

void SCommentsModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    m_queryData->setQuery(query, db);
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
