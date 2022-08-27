#include "sfieldsmodel.h"

SFieldsModel::SFieldsModel(bool type, QObject *parent) :
    QObject(parent),
    m_isRepair(type)
{
    query = new QSqlQuery(QSqlDatabase::database("connMain"));
}

SFieldsModel::~SFieldsModel()
{
    qDebug().nospace() << "[SFieldsModel] ~SFieldsModel() | In";
    delete query;
    deleteWidgets();
    qDebug().nospace() << "[SFieldsModel] ~SFieldsModel() | all widgets deleted successfully";
    deleteFields();
    qDebug().nospace() << "[SFieldsModel] ~SFieldsModel() | Out";
}

void SFieldsModel::deleteWidgets()
{
    qDebug().nospace() << "[SFieldsModel] deleteWidgets() | In";
    QWidget *w;

    while( !m_widgetsList.isEmpty() )
    {
        w = m_widgetsList.last();
        qDebug().nospace() << "[SFieldsModel] deleteWidgets() | Step x.0";
        if(w->property("fieldType") == Type::ComboBox)
        {
            QComboBox *cb = static_cast<QComboBox*>(w);
            QAbstractItemModel* widgetModel;
            widgetModel = cb->model();
            qDebug().nospace() << "[SFieldsModel] deleteWidgets() | Step x.1a";
            if(widgetModel)
                delete widgetModel;
        }
        qDebug().nospace() << "[SFieldsModel] deleteWidgets() | Step x.1";
        m_widgetsList.removeLast();
        qDebug().nospace() << "[SFieldsModel] deleteWidgets() | Step x.2";
        delete w;
    }
    qDebug().nospace() << "[SFieldsModel] deleteWidgets() | Out";
}

void SFieldsModel::deleteFields()
{
    SFieldValueModel *f;

    while(!m_fieldsList.isEmpty())
    {
        f = m_fieldsList.last();
        m_fieldsList.removeLast();
        delete f;
    }
}

QList<SFieldValueModel*> SFieldsModel::list()
{
    return m_fieldsList;
}

QList<QWidget *> SFieldsModel::widgetsList()
{
    return m_widgetsList;
}

/*  Инициализация массива виджетов для нового товара или ремонта
 *  isRepair == 1 — инициализация виджетов для класса <id> устройств,
 *  иначе для <id> категории товаров
 */
bool SFieldsModel::initWidgets(const int id)
{
    qDebug().nospace() << "[SFieldsModel] initWidgets() | In";
    if(id == 0)
        return 0;

    qDebug().nospace() << "[SFieldsModel] initWidgets() | step 0";
    if( !(m_widgetsList.isEmpty() && m_fieldsList.isEmpty()) )
    {
        qDebug().nospace() << "[SFieldsModel] initWidgets() | step 0.0";
        deleteWidgets();
        qDebug().nospace() << "[SFieldsModel] initWidgets() | step 0.1";
        deleteFields();
        qDebug().nospace() << "[SFieldsModel] initWidgets() | step 0.2";
    }

    qDebug().nospace() << "[SFieldsModel] initWidgets() | step 1";
    QSizePolicy *sizePolicy = new QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    QWidget *widget;
    SFieldValueModel *field;
    int type = 0;

    qDebug().nospace() << "[SFieldsModel] initWidgets() | step 2";
    query->exec(QUERY_SEL_ADDITIONAL_FIELDS_TYPES((m_isRepair?1:0), id));
    while(query->next())
    {
        type = query->value(2).toInt();
        field = new SFieldValueModel();

        switch (type)
        {
            case Type::LineEdit: widget = createLineEdit(query->record()); break;
            case Type::ComboBox: widget = createComboBox(query->record()); break;
            case Type::DateEdit: widget = createDateTime(query->record()); break;
            default: widget = createDummyWidget(query->record());
        }
        qDebug().nospace() << "[SFieldsModel] initWidgets() | step 3.x";
        widget->setSizePolicy(*sizePolicy);
        m_widgetsList.append(widget);
        m_fieldsList.append(field);
        widget->setProperty("fieldId", query->value(3).toInt());
        widget->setProperty("fieldType", type);
        widget->setProperty("fieldRequired", query->value(4).toBool());
        widget->setProperty("fieldPrintable", query->value(5).toBool());
    }
    delete sizePolicy;
    qDebug().nospace() << "[SFieldsModel] initWidgets() | Out";
    return 1;
}

QWidget *SFieldsModel::createLineEdit(const QSqlRecord &record)
{
    QLineEdit *widget = new QLineEdit();
    widget->setPlaceholderText(record.value(0).toString());
    return widget;
}

QWidget *SFieldsModel::createComboBox(const QSqlRecord &record)
{
    QComboBox *widget = new QComboBox();
    QStandardItemModel* widgetModel = new QStandardItemModel();
    QStringList widgetItems = record.value(1).toString().split('\n');
    QStandardItem *newRow;
    QFontMetrics *fm = new QFontMetrics(widget->font());
    int itemTextWidth, dropDownListWidth = 0;

    widget->setModel(widgetModel);
//            if (widgetItems.at(0) != "")   // Принудительно добавляем пустую строку
//            {
//                newRow = new QStandardItem();
//                newRow->setText("");
//                widgetModel->appendRow(newRow);
//            }
    for (int i=0; i<widgetItems.size(); i++)
    {
        newRow = new QStandardItem();
        newRow->setText(widgetItems.at(i));
        widgetModel->appendRow(newRow);

        // определяем наибольшую длину текста в списке элементов
        itemTextWidth = fm->size(Qt::TextSingleLine, widgetItems.at(i)).width() + 10;
        if (itemTextWidth > dropDownListWidth)
            dropDownListWidth = itemTextWidth;
    }
    delete fm;  // больше не нужен
    widget->setMinimumWidth(100);
    widget->view()->setMinimumWidth(dropDownListWidth);
    widget->setCurrentIndex(-1);
    widget->setEditable(true);
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    widget->lineEdit()->setPlaceholderText(record.value(0).toString());

    return widget;
}

QWidget *SFieldsModel::createDateTime(const QSqlRecord &)
{
    QDateEdit *widget = new QDateEdit(QDate::currentDate());
    widget->setCalendarPopup(true);

    return widget;
}

QWidget *SFieldsModel::createDummyWidget(const QSqlRecord &record)
{
    // TODO: В АСЦ не реализовано, поэтому используем LineEdit
    QLineEdit *widget = new QLineEdit();
    widget->setPlaceholderText(record.value(0).toString());
    widget->setEnabled(false);

    return widget;
}

bool SFieldsModel::load(int id)
{
    if(m_repair == 0 && m_item == 0)
        return 0;

    if(m_repair)
        query->exec(QUERY_SEL_REPAIR_ADD_FIELDS(id));
    else
        query->exec(QUERY_SEL_ITEM_ADD_FIELDS(id));

    while(query->next())
    {
        itemHandler(query->record());
    }

    delete query;
    return 1;
}

void SFieldsModel::add(SFieldValueModel *item)
{
    m_fieldsList.append(item);
}

void SFieldsModel::remove(SFieldValueModel *item)
{
    m_removeList.append(item);
    int modelIndex = m_fieldsList.indexOf(item);
    m_fieldsList.removeAt(modelIndex);
}

bool SFieldsModel::isEmpty()
{
    return m_fieldsList.isEmpty();
}

void SFieldsModel::setRepair(const int id)
{
    m_repair = id;
}

void SFieldsModel::setItem(const int id)
{
    m_item = id;
}

bool SFieldsModel::commit()
{
    SFieldValueModel *item;
    foreach(item, m_fieldsList)
    {
        if(!item->commit())
            return 0;
    }

    while( !m_removeList.isEmpty() )
    {
        item = m_removeList.last();
        if(!item->delDBRecord())
            return 0;

        m_removeList.removeLast();
        item->deleteLater();
    }

    return 1;
}

SFieldValueModel *SFieldsModel::itemHandler(const QSqlRecord &record)
{
    SFieldValueModel *item = new SFieldValueModel(this);
    item->load(record);
    add(item);
    return item;
}
