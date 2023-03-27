#include "sfieldvaluemodel.h"

SFieldValueModel::SFieldValueModel(QObject *parent) : SComRecord(parent)
{
    i_obligatoryFields << "field_id" << "value";
    i_tableName = "field_values";
}

SFieldValueModel::SFieldValueModel(const int id, QObject *) : SFieldValueModel()
{
    load(id);
}

SFieldValueModel::~SFieldValueModel()
{
    deleteWidget();
}

void SFieldValueModel::load(const int id)
{
    SSqlQueryModel *fieldModel = new SSqlQueryModel(this);
    fieldModel->setQuery(QUERY_SEL_ADD_FIELD(id));
    if(!fieldModel->lastError().isValid())
    {
        m_isValid = 1;
        load(fieldModel->record(0));
    }
    delete fieldModel;
}

void SFieldValueModel::load(QSqlRecord record)
{
    i_valuesMap.clear();

    i_id = record.value("id").toInt();
    m_name = record.value("name").toString();
    m_fieldId = record.value("field_id").toInt();
    m_repairId = record.value("repair_id").toInt();
    m_itemId = record.value("item_id").toInt();
    m_value = record.value("value").toString();
    m_printable = record.value("printable").toBool();
}

QWidget *SFieldValueModel::widget()
{
    return m_widget;
}

QWidget *SFieldValueModel::createWidget(const QSqlRecord &record)
{
    if(m_widget != nullptr)
        return m_widget;

    QSizePolicy *sizePolicy = new QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    int type = record.value(2).toInt();
    setProperty("fieldType", type);
    setProperty("fieldId", record.value(3).toInt());
    setProperty("fieldRequired", record.value(4).toBool());
    setProperty("fieldPrintable", record.value(5).toBool());
    setProperty("fieldDevMatch", record.value(6).toBool());
    setProperty("fieldInputMask", record.value(7).toString());

    switch (type)
    {
        case WidgetType::LineEdit: m_widget = createLineEdit(record); break;
        case WidgetType::ComboBox: m_widget = createComboBox(record); break;
        case WidgetType::DateEdit: m_widget = createDateTime(record); break;
        default: m_widget = createDummyWidget(record);
    }
    m_widget->setSizePolicy(*sizePolicy);
    delete sizePolicy;

    return m_widget;
}

void SFieldValueModel::setDefaultStyleSheet()
{
    switch (this->property("fieldType").toInt())
    {
        case WidgetType::LineEdit: m_widget->setStyleSheet(commonLineEditStyleSheet); break;
        case WidgetType::ComboBox: m_widget->setStyleSheet(commonComboBoxStyleSheet); break;
        case WidgetType::DateEdit: m_widget->setStyleSheet(commonDateEditStyleSheet); break;
        default: m_widget->setStyleSheet(commonLineEditStyleSheet);
    }
}

bool SFieldValueModel::validate()
{
    bool ret = 1;

    if(this->property("fieldRequired").toBool())
    {
        switch (this->property("fieldType").toInt())
        {
            case WidgetType::ComboBox: if(comboBox()->currentIndex() == -1) {m_widget->setStyleSheet(commonComboBoxStyleSheetRed); ret = 0;}; break;
//            case WidgetType::DateEdit: if(dateEdit()->method()) {m_widget->setStyleSheet(commonDateEditStyleSheetRed); ret = 0;}; break; // TODO: придумать критерии проверки DateEdit
            case WidgetType::LineEdit:
            default: if(lineEdit()->text().isEmpty()) {m_widget->setStyleSheet(commonLineEditStyleSheetRed); ret = 0;};
        }
    }
    if(!this->property("fieldInputMask").toString().isEmpty())
    {
        if(this->property("fieldType").toInt() == WidgetType::LineEdit)
        {
            if(!lineEdit()->hasAcceptableInput())
            {
                m_widget->setStyleSheet(commonLineEditStyleSheetRed);
                ret = 0;
            }
        }
    }

    if(ret)
        setDefaultStyleSheet();

    return ret;
}

void SFieldValueModel::deleteWidget()
{
    if(m_widget == nullptr)
        return;

    if(property("fieldType") == WidgetType::ComboBox)
    {
        QAbstractItemModel* widgetModel;
        widgetModel = comboBox()->model();
        if(widgetModel)
            delete widgetModel;
    }
    delete m_widget;
    m_widget = nullptr;
}

QWidget *SFieldValueModel::createLineEdit(const QSqlRecord &record)
{
    QLineEdit *widget = new QLineEdit();
    widget->setPlaceholderText(record.value(0).toString());
    if(!record.value(1).toString().isEmpty())
    {
        widget->setText(record.value(1).toString());
        this->setValue(widget->text());
    }
    if(property("fieldDevMatch").toBool())
        connect(widget,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    if(!property("fieldInputMask").toString().isEmpty())
        widget->setInputMask(property("fieldInputMask").toString());

    return widget;
}

QWidget *SFieldValueModel::createComboBox(const QSqlRecord &record)
{
    SComboBox *widget = new SComboBox();
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

QWidget *SFieldValueModel::createDateTime(const QSqlRecord &record)
{
    QDateEdit *widget = new QDateEdit();
    if(record.value(1).toString().isEmpty())
    {
        widget->setDate(QDate::currentDate());
    }
    else
    {
        widget->setDate(record.value(1).toDate());
    }
    this->setValue(widget->text());
    widget->setCalendarPopup(true);

    return widget;
}

QWidget *SFieldValueModel::createDummyWidget(const QSqlRecord &record)
{
    // TODO: В АСЦ не реализовано, поэтому используем LineEdit
    QLineEdit *widget = new QLineEdit();
    widget->setPlaceholderText(record.value(0).toString());
    widget->setEnabled(false);

    return widget;
}

QComboBox *SFieldValueModel::comboBox()
{
    return static_cast<QComboBox*>(m_widget);
}

QLineEdit *SFieldValueModel::lineEdit()
{
    return static_cast<QLineEdit*>(m_widget);
}

QDateEdit *SFieldValueModel::dateEdit()
{
    return static_cast<QDateEdit*>(m_widget);
}

int SFieldValueModel::id()
{
    return m_id;
}

void SFieldValueModel::setId(const int id)
{
    i_id = id;
}

int SFieldValueModel::fieldId()
{
    return m_fieldId;
}

void SFieldValueModel::setFieldId(const int field_id)
{
    i_valuesMap.insert("field_id", field_id);
}

int SFieldValueModel::repairId()
{
    return m_repairId;
}

void SFieldValueModel::setRepairId(const int repair_id)
{
    i_valuesMap.insert("repair_id", repair_id);
    i_logRecord->setRepairId(repair_id);
}

int SFieldValueModel::itemId()
{
    return m_itemId;
}

void SFieldValueModel::setItemId(const int item_id)
{
    i_valuesMap.insert("item_id", item_id);
    i_logRecord->setItemId(item_id);
}

QString SFieldValueModel::name()
{
    return m_name;
}

QString SFieldValueModel::value()
{
    return m_value;
}

void SFieldValueModel::setValue(const QString &value)
{
    if(i_id)
    {
        if(value.isEmpty())
            emit emptied(this);
        else
            appendLogText(tr("Значение параметра %1 изменено с \"%2\" на \"%3\"").arg(m_name,m_value,value));
    }

    m_value = value;
    i_valuesMap.insert("value", value);
}

void SFieldValueModel::textChanged(QString text)
{
    emit textChanged(this->property("fieldId").toInt(), text);
}

bool SFieldValueModel::commit()
{
    if(m_widget != nullptr)
    {
        setFieldId(this->property("fieldId").toInt());
        switch (this->property("fieldType").toInt())
        {
            case WidgetType::ComboBox: setValue(comboBox()->currentText()); break;
            case WidgetType::DateEdit: setValue(dateEdit()->date().toString("yyyy-MM-dd")); break;
            case WidgetType::LineEdit:
            default: setValue(lineEdit()->text());
        }
    }
    if(value().isEmpty())
        return 1;

    if(i_id)
    {
        if(!update())
            return 0;
    }
    else
    {
        if(!insert())
            return 0;
    }
    commitLogs();
    return i_nErr;
}

bool SFieldValueModel::delDBRecord()
{
    i_logRecord->setText(tr("Параметр %1 (\"%2\") удалён").arg(m_name, m_value));
    i_logRecord->commit();
    return SComRecord::del();
}

bool SFieldValueModel::isValid()
{
    return m_isValid;
}

bool SFieldValueModel::deviceMatch()
{
    return property("fieldDevMatch").toBool();
}

bool SFieldValueModel::isPrintable()
{
    return m_printable;
}

#ifdef QT_DEBUG
void SFieldValueModel::randomFill()
{
    switch (this->property("fieldType").toInt())
    {
        case WidgetType::ComboBox: randomComboBoxIndex(); break;
        case WidgetType::DateEdit: randomDateEditValue(); break;
        case WidgetType::LineEdit:
        default: randomLineEditText();
    }
}

/*  Случайное число
*/
void SFieldValueModel::randomLineEditText()
{
    lineEdit()->setText(QString::number(QRandomGenerator::global()->bounded(2147483647)));
}

/*  Выбирает случайный индекс модели данных
*/
void SFieldValueModel::randomComboBoxIndex()
{
    int i = comboBox()->model()->rowCount();
    comboBox()->setCurrentIndex(QRandomGenerator::global()->bounded(i));
}

/*  Прибавляет к текущей дате случайное число от 0 до 90
*/
void SFieldValueModel::randomDateEditValue()
{
    dateEdit()->setDate(QDate::fromJulianDay(QDate::currentDate().toJulianDay() + QRandomGenerator::global()->bounded(90)));
}
#endif

