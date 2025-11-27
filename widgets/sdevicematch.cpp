#include "sdevicematch.h"
#include "ui_sdevicematch.h"
#include <ProjectGlobals>
#include <ProjectQueries>
#include <SGroupBoxEventFilter>

SDeviceMatch::SDeviceMatch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SDeviceMatch)
{
    ui->setupUi(this);
    this->hide();

    groupBoxEventFilter = new SGroupBoxEventFilter(this);
    ui->groupBoxDeviceMatch->installEventFilter(groupBoxEventFilter);
    ui->tableViewDeviceMatch->horizontalHeader()->setHidden(false);
    connect(groupBoxEventFilter,SIGNAL(toggleElementsVisibility()),this,SLOT(toggleElementsVisibility()));
    tableModel = new QSqlQueryModel();       // таблица с результатами поиска
    ui->tableViewDeviceMatch->setModel(tableModel);  // указываем модель таблицы
}

SDeviceMatch::~SDeviceMatch()
{
    delete ui;
}

void SDeviceMatch::findDevice()
{
    QStringList query_where;    // список условий для запроса к БД
    QString query;  // весь запрос к БД

    if(serialNumber.length() >= 3 || fieldClauses.count() )  // если пользователь ввёл более двух символов в одно из полей
    {
        query_where.clear();
        if (serialNumber.length() >= 3 )
            query_where << QUERY_SEL_DEVICE_MATCH_BY_SN.arg(serialNumber);
        if (fieldClauses.count())
            query_where << QUERY_SEL_DEVICE_MATCH_BY_FIELDS.arg(fieldClauses.values().join("\n      OR "));   // условие поиска по доп. полям

        query = QUERY_SEL_DEVICE_MATCH.arg((query_where.count()>0?query_where.join("\n  OR "):""));
        tableModel->setQuery(query, QSqlDatabase::database("connMain"));

        // изменяем размер столбцов под содержимое.
        // TODO: нужно бы создать свой класс с наследованием QTableView, реализовать в нём пропорциональное изменение ширин столбцов
        // при изменении размера окна и тултип для длинного текста. Этот класс использовать везде
        ui->tableViewDeviceMatch->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        QStringList horizontalHeader = {"Id",tr("Устройство"),tr("Неисправность"),tr("SN"),tr("Клиент")};
        for(int i = 0; i < horizontalHeader.size(); i++)
        {
            tableModel->setHeaderData(i, Qt::Horizontal, horizontalHeader[i]);
        }
        if (tableModel->rowCount() > 0)
        {
            this->show();  // только если возвращены результаты, показываем виджет
            ui->tableViewDeviceMatch->show();       // может случиться ситуация, когда таблица будет скрыта, поэтому принудительно отображаем её
        }
        else
            this->hide();  // иначе прячем виджет
    }
    else
    {
        tableModel->clear(); // если кол-во введённых пользователем символов меньше трёх, то удаляем результаты предыдущего запроса и прячем виджет.
        this->hide();
    }
}

QString SDeviceMatch::prepareFieldClause(const int fieldId, const QString &text)
{
    return QUERY_SEL_DEVICE_MATCH_BY_FIELD.arg(fieldId).arg(text);
}

void SDeviceMatch::clearFields()
{
    fieldClauses.clear();
    fieldIds.clear();
}

void SDeviceMatch::clearTable()
{
    tableModel->clear();
}

void SDeviceMatch::findByField(const int fieldId, const QString &text)
{
    if(text.length() >= 3)
        fieldClauses.insert(fieldId, prepareFieldClause(fieldId, text));
    else
        fieldClauses.remove(fieldId);
    findDevice();
}

void SDeviceMatch::findBySN(const QString &text)
{
    serialNumber = text;
    findDevice();
}

void SDeviceMatch::deviceMatchTableDoubleClicked(QModelIndex index)
{
    emit deviceSelected(tableModel->index(index.row(), 0).data().toInt());
    ui->tableViewDeviceMatch->hide();   // прячем таблицу, а не весь groupBox (вдруг пользователь промахнётся)
}

void SDeviceMatch::toggleElementsVisibility()
{
    ui->tableViewDeviceMatch->setVisible(!ui->tableViewDeviceMatch->isVisible());
}
