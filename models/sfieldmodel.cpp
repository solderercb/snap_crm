#include "sfieldmodel.h"
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <ProjectGlobals>
#include <QLineEdit>
#include <SComboBox>
#include <SDateEdit>

SFieldModel::SFieldModel(QObject *parent) : SSingleRowJModel(parent)
{
    mapFields();

    i_obligatoryFields << "name" << "type" << "def_values" << "devices" << "categories" << "input_mask";
    i_tableName = "fields";
    setPrimaryKeyIndex(0);
}

SFieldModel::SFieldModel(const int id, QSqlDatabase database, QObject *parent) : SFieldModel(parent)
{
    Q_UNUSED(parent)

    initSqlQuery(database);
    setPrimaryKey(id);
    load();
}

SFieldModel::~SFieldModel()
{
    deleteWidget();
}

QWidget *SFieldModel::widget()
{
    return m_widget;
}

void SFieldModel::setDefaultStyleSheet()
{
    switch (type())
    {
        case WidgetType::LineEdit: m_widget->setStyleSheet(commonLineEditStyleSheet); break;
        case WidgetType::ComboBox: m_widget->setStyleSheet(commonComboBoxStyleSheet); break;
        case WidgetType::DateEdit: m_widget->setStyleSheet(commonDateEditStyleSheet); break;
        default: m_widget->setStyleSheet(commonLineEditStyleSheet);
    }
}

QWidget *SFieldModel::createWidget(const QString &value)
{
    if(m_widget != nullptr)
        return m_widget;

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    switch (type())
    {
        case WidgetType::LineEdit: m_widget = createLineEdit(); break;
        case WidgetType::ComboBox: m_widget = createComboBox(); break;
        case WidgetType::DateEdit: m_widget = createDateTime(); break;
        default: m_widget = createDummyWidget();
    }
    m_widget->setSizePolicy(sizePolicy);

    if(!value.isEmpty())
        setWidgetValue(value);

    return m_widget;
}

void SFieldModel::deleteWidget()
{
    if(m_widget == nullptr)
        return;

    if(type() == WidgetType::ComboBox)
    {
        QAbstractItemModel* widgetModel;
        widgetModel = comboBox()->model();
        if(widgetModel)
            delete widgetModel;
    }
    delete m_widget;
    m_widget = nullptr;
}

void SFieldModel::setWidgetValue(const QString &value)
{
    switch (type())
    {
        case WidgetType::ComboBox: comboBox()->setCurrentText(value); break;
        case WidgetType::DateEdit: dateEdit()->setDate(stringToDate(value)); break;
        case WidgetType::LineEdit:
        default:                   lineEdit()->setText(value); break;
    }
}

void SFieldModel::setWidgetDefaultValue()
{
    switch (type())
    {
        case WidgetType::ComboBox: return;
        case WidgetType::DateEdit:
            if(defaultValue().isEmpty())
                return;
            else if(defaultValue().compare("00.00.0000"))
                dateEdit()->setDate(QDate::currentDate());
            else
                dateEdit()->setDate(stringToDate(defaultValue()));
            return;
        case WidgetType::LineEdit:
        default:
            if(defaultValue().isEmpty())
                lineEdit()->setCursorPosition(0);
            else
                lineEdit()->setText(defaultValue());
            return;
    }
}

bool SFieldModel::validate()
{
    bool ret = 1;

    if(required())
    {
        switch (type())
        {
            case WidgetType::ComboBox: if(comboBox()->currentIndex() == -1) {m_widget->setStyleSheet(commonComboBoxStyleSheetRed); ret = 0;}; break;
//            case WidgetType::DateEdit: if(dateEdit()->method()) {m_widget->setStyleSheet(commonDateEditStyleSheetRed); ret = 0;}; break; // TODO: придумать критерии проверки DateEdit
            case WidgetType::LineEdit:
            default: if(lineEdit()->text().isEmpty()) {m_widget->setStyleSheet(commonLineEditStyleSheetRed); ret = 0;};
        }
    }
    if(!inputMask().isEmpty())
    {
        if(type() == WidgetType::LineEdit)
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

QWidget *SFieldModel::createLineEdit()
{
    QLineEdit *widget = new QLineEdit();
    widget->setPlaceholderText(name());
    if(deviceMatch())
        connect(widget, &QLineEdit::textChanged, this, [=](const QString &text){emit textChanged(id(), text);});
    if(!inputMask().isEmpty())
    {
        widget->setInputMask(inputMask());
        widget->setCursorPosition(0);
    }

    return widget;
}

QWidget *SFieldModel::createComboBox()
{
    SComboBox *widget = new SComboBox();
    QStandardItemModel* widgetModel = new QStandardItemModel();
    QStringList widgetItems = defaultValue().split('\n');
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
    widget->lineEdit()->setPlaceholderText(name());

    return widget;
}

QWidget *SFieldModel::createDateTime()
{
    SDateEdit *widget = new SDateEdit();
    widget->setAllowBackDate();
    widget->setAllowFutureDate();
    widget->setCalendarPopup(true);
    widget->setDate(QDate());

    return widget;
}

QWidget *SFieldModel::createDummyWidget()
{
    // TODO: В АСЦ не реализовано, поэтому используем LineEdit
    QLineEdit *widget = new QLineEdit();
    widget->setPlaceholderText(name());
    widget->setEnabled(false);

    return widget;
}

QComboBox *SFieldModel::comboBox()
{
    return static_cast<QComboBox*>(m_widget);
}

QLineEdit *SFieldModel::lineEdit()
{
    return static_cast<QLineEdit*>(m_widget);
}

SDateEdit *SFieldModel::dateEdit()
{
    return static_cast<SDateEdit*>(m_widget);
}

QDate SFieldModel::stringToDate(const QString &str)
{
    return QDate::fromString(str, "dd.MM.yyyy");
}

QString SFieldModel::fieldsForSelectQuery()
{
    QString query;
    for(int i = 0; i < rec()->count(); i++)
    {
        if(!query.isEmpty())
            query.append(",\n");
        if(i == C_defaultValue)
            query.append(QString("  REPLACE(`%1`,'\\r','') AS '%1'").arg(rec()->fieldName(i)));
        else
            query.append(QString("  `%1`").arg(rec()->fieldName(i)));
    }

    return query;
}
