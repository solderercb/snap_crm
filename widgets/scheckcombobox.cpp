#include "scheckcombobox.h"
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QEvent>
#include <QIdentityProxyModel>
#include <QDebug>

SCheckComboBox::SCheckComboBox(QWidget *parent) :
    SComboBox(parent)
{
    disableWheelEvent(true);
    setKeepListShown(true);
}

SCheckComboBox::~SCheckComboBox()
{

}

/* Настройка модели данных
 * Для правильной работы данные в модели должны быть предствлены как минимум двумя столбцами: name, id
*/
void SCheckComboBox::setModel(QAbstractItemModel *model)
{
    Q_ASSERT_X(model->columnCount() > 1, this->metaObject()->className(), "Model must have as minimum two columns: name, id");
    m_proxyModel = new SCheckComboBoxDataModel(this);
    m_proxyModel->setSourceModel(model);
    m_proxyModel->showSelectAllRow(m_showSelectAllRow);
    SComboBox::setModel(m_proxyModel);
    SComboBox::setCurrentIndex(-1);

    connect(m_proxyModel, &SCheckComboBoxDataModel::dataChanged, this, &SCheckComboBox::dataChanged);

}

/* Возвращает список id отмеченных галочной строк
*/
QList<int> SCheckComboBox::checked()
{
    QList<int> ret;
    QList<QModelIndex>::ConstIterator it = m_proxyModel->m_checkedItems.constBegin();
    while(it != m_proxyModel->m_checkedItems.constEnd())
    {
        if(!(m_proxyModel->isSelectAllRowShown() && it->row() == 0))
            ret.append(it->data().toInt());
        ++it;
    }

    return ret;
}

/* Аналогично checked(), но результат в виде строки, где id через запятую
*/
QString SCheckComboBox::checkedString()
{
    QStringList stringList;
    foreach (auto v, checked())
    {
        stringList.append(QString::number(v));
    }
    return stringList.join(',');
}

/* Возвращает список id НЕ отмеченных галочной строк
*/
QList<int> SCheckComboBox::unchecked()
{
    QList<int> ret;
    int start = 0;
    if(m_showSelectAllRow)
        start = 1;

    for(int i = start; i < m_proxyModel->rowCount(); i++)
    {
        QModelIndex item = m_proxyModel->index(i, SCheckComboBoxDataModel::Columns::Id);
        if(m_proxyModel->m_checkedItems.contains(item))
            continue;

        ret.append(item.data().toInt());
    }

    return ret;
}

/* Переключение состояний всех checkBox'ов в выключенное.
*/
void SCheckComboBox::resetChecked()
{
    Q_ASSERT_X(m_proxyModel, this->objectName().toLocal8Bit(), "data model not set");

    m_proxyModel->setAll(Qt::Unchecked);
}

/* Переключение состояний checkBox'ов, соответствующих id пользователя из списка list, во включенное.
*/
void SCheckComboBox::setChecked(const QList<int> &list)
{
    resetChecked();
    toggleChecked(list);
}

void SCheckComboBox::setChecked(const QString &csv_list)
{
    QStringList stringList = csv_list.split(',');
    QList<int> intList;
    foreach (auto v, stringList)
    {
        intList.append(v.toInt());
    }
    setChecked(intList);
}

/* Переключение состояния checkBox, соответствующего id, в противоположное состояние
*/
void SCheckComboBox::toggleChecked(const int id)
{
    Q_ASSERT_X(m_proxyModel, this->objectName().toLocal8Bit(), "data model not set");

    QModelIndexList l = m_proxyModel->match(m_proxyModel->index(0, SCheckComboBoxDataModel::Columns::Id), Qt::DisplayRole, id);
    QModelIndexList::ConstIterator it = l.constBegin();
    while(it != l.constEnd())
    {
        m_proxyModel->setData(*it, Qt::Checked, Qt::CheckStateRole); // в методе SCheckComboBoxDataModel::setData состояние изменяется на противоположное
        ++it;
    }
}

/* Переключение состояний checkBox'ов, соответствующих id пользователя из списка list, в противоположные состояния
*/
void SCheckComboBox::toggleChecked(const QList<int> &list)
{
    QList<int>::ConstIterator it = list.constBegin();
    while(it != list.constEnd())
    {
        toggleChecked(*it);
        ++it;
    }
}

bool SCheckComboBox::eventFilter(QObject *watched, QEvent *event)
{
    return SComboBox::eventFilter(watched, event);
}

void SCheckComboBox::dataChanged(const QModelIndex &topLeft, const QModelIndex &botRight, const QVector<int> &roles)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(botRight)
    Q_UNUSED(roles)

    QStringList l;

    if(m_proxyModel->isSelectAllRowShown() && m_proxyModel->m_checkedItems.contains(m_proxyModel->index(0, SCheckComboBoxDataModel::Columns::Id))) // Если выбраны все, то вместо списка пользователей отображается "Все"
    {
        l.append(tr("All"));
    }
    else
    {
    // вариант 1: отмеченные элементы будут отображаться в строке в порядке отметки
//        QList<QModelIndex>::ConstIterator it = m_proxyModel->m_checkedItems.constBegin();
//        while(it != m_proxyModel->m_checkedItems.constEnd())
//        {
//            l.append(it->siblingAtColumn(SCheckComboBoxDataModel::Name).data().toString());
//            ++it;
//        }

    // вариант 2: отмеченные элементы будут отображаться в строке в соответствии с порядком элементов в модели данных
        for(int i = 0; i < m_proxyModel->rowCount(); i++)
        {
            QModelIndex idx = m_proxyModel->index(i, SCheckComboBoxDataModel::Id);
            if(m_proxyModel->m_checkedItems.contains(idx))
                l.append(idx.siblingAtColumn(SCheckComboBoxDataModel::Name).data().toString());

        }
    }

    // порядок важен: сначала нужно задать защещающий текст и только потом сбросить индекс
    setPlaceholderText(l.join(" / "));
    setCurrentIndex(-1);

}

void SCheckComboBox::rowActivated(int row)
{
    setIgnorePopupHide(1);
    SComboBox::rowActivated(row);
    m_proxyModel->toggleCheckBox(m_proxyModel->index(row, SCheckComboBoxDataModel::Columns::Id));
}

void SCheckComboBox::showPopup()
{
    SComboBox::showPopup();
}

void SCheckComboBox::hidePopup()
{
    SComboBox::hidePopup();
    setIgnorePopupHide(false);
}

void SCheckComboBox::showSelectAllRow(bool state)
{
    m_showSelectAllRow = state;

    if(!m_proxyModel)
        return;

    m_proxyModel->showSelectAllRow(state);
}

/********************************************************************************************************/
SCheckComboBoxDataModel::SCheckComboBoxDataModel(QObject *parent) :
    QConcatenateTablesProxyModel(parent)
{
    Q_UNUSED(parent)
}

SCheckComboBoxDataModel::~SCheckComboBoxDataModel()
{

}

Qt::ItemFlags SCheckComboBoxDataModel::flags(const QModelIndex &item) const
{
    Q_UNUSED(item)

    return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//    return QIdentityProxyModel::flags(item) | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
}

QVariant SCheckComboBoxDataModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::CheckStateRole)
        return m_checkedItems.contains(item.siblingAtColumn(Columns::Id))?Qt::Checked:Qt::Unchecked;

    return QConcatenateTablesProxyModel::data(item, role);
}

bool SCheckComboBoxDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole)
    {
        toggleCheckBox(index);

        return true;
    }

    return QConcatenateTablesProxyModel::setData(index, value, role);
}

void SCheckComboBoxDataModel::setSourceModel(QAbstractItemModel *model)
{
    m_sourceModel = model;
    QConcatenateTablesProxyModel::addSourceModel(model);
}

void SCheckComboBoxDataModel::setCheckBox(const QModelIndex &index, int state)
{
    if(state == Qt::Checked && !m_checkedItems.contains(index.siblingAtColumn(Columns::Id)))
        m_checkedItems.append(index.siblingAtColumn(Columns::Id));
    else if(state == Qt::Unchecked)
        m_checkedItems.removeAll(index.siblingAtColumn(Columns::Id));
    else
        return;

    emit dataChanged(index, index, {Qt::CheckStateRole});
}

void SCheckComboBoxDataModel::setCheckBox(const int row, int state)
{
    QModelIndex item = index(row, Columns::Id);
    setCheckBox(item, state);
}

void SCheckComboBoxDataModel::toggleCheckBox(const QModelIndex &item)
{
    if(!item.isValid())
        return;

    if(!m_checkedItems.contains(item.siblingAtColumn(Columns::Id)))
    {
        m_checkedItems.append(item.siblingAtColumn(Columns::Id));
    }
    else
    {
        m_checkedItems.removeAll(item.siblingAtColumn(Columns::Id));
    }

    if(m_showSelectAllRow)
    {
        QModelIndex allIndex = index(0, Columns::Id);
        if(item.row() == 0)
            toggleAll();
        else if(m_checkedItems.count() == m_sourceModel->rowCount() && !m_checkedItems.contains(allIndex))
            m_checkedItems.append(allIndex);
        else if(m_checkedItems.contains(allIndex) && m_checkedItems.count() < m_sourceModel->rowCount() + 1 )
            m_checkedItems.removeAll(allIndex);
    }

    emit dataChanged(item, item, {Qt::CheckStateRole});
}

void SCheckComboBoxDataModel::toggleCheckBox(const int row)
{
    QModelIndex item = index(row, Columns::Id);
    toggleCheckBox(item);
}

/* Переключение состояний всех checkBox'ов в state
 * В методе генерируется сигнал dataChanged
*/
void SCheckComboBoxDataModel::setAll(const int state)
{
    blockSignals(true);
    for(int i = 1; i < rowCount(); i++)
    {
        setCheckBox(i, state);
    }
    blockSignals(false);

    QModelIndex idxFirst = index(0, Columns::Id);

    if(m_showSelectAllRow)
        emit dataChanged(idxFirst, idxFirst, {Qt::CheckStateRole});
    else
    {
        QModelIndex idxLast = index(rowCount() - 1, Columns::Id);
        emit dataChanged(idxFirst, idxLast, {Qt::CheckStateRole});
    }
}

/* Переключение состояний всех checkBox'ов во включенное или выключенное.
 * Новое состояние будет противоположным состоянию элемента "Все".
 * Если элемент "Все" не отображается, то ничего не происходит.
*/
void SCheckComboBoxDataModel::toggleAll()
{
    if(!m_showSelectAllRow)
        return;

    QModelIndex allIndex = index(0, Columns::Id);
    int newState;
    if(m_checkedItems.contains(allIndex))
        newState = Qt::Checked;
    else
        newState = Qt::Unchecked;

    setAll(newState);
}

void SCheckComboBoxDataModel::insertSelectAllRow()
{
    if(m_showSelectAllRow)
        return;

    // строки объединённой модели отображаются в порядке добавления моделей, поэтому чтобы строка "Всё" была вверху
    // нужно удалить исходную модель
    removeSourceModel(m_sourceModel);

    if(!rowSelectAll)
        rowSelectAll = new QStandardItemModel();
    rowSelectAll->insertRow(0, {new QStandardItem(tr("All")), new QStandardItem(tr(""))});

    addSourceModel(rowSelectAll);
    addSourceModel(m_sourceModel);

    m_showSelectAllRow = true;
}

void SCheckComboBoxDataModel::removeSelectAllRow()
{
    if(!m_showSelectAllRow)
        return;

    removeSourceModel(rowSelectAll);
    m_showSelectAllRow = false;
}

void SCheckComboBoxDataModel::showSelectAllRow(bool state)
{
    if(state)
    {
        insertSelectAllRow();
    }
    else
    {
        removeSelectAllRow();
    }
}

bool SCheckComboBoxDataModel::isSelectAllRowShown()
{
    return m_showSelectAllRow;
}
