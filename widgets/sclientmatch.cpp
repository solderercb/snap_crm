#include "sclientmatch.h"
#include "ui_sclientmatch.h"

SClientMatch::SClientMatch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SClientMatch)
{
    ui->setupUi(this);
    this->hide();

    groupBoxEventFilter = new SGroupBoxEventFilter(this);
    ui->groupBoxClientMatch->installEventFilter(groupBoxEventFilter);
    ui->tableViewClientMatch->horizontalHeader()->setHidden(false);
    connect(groupBoxEventFilter,SIGNAL(toggleElementsVisibility()),this,SLOT(toggleElementsVisibility()));
    clientsMatchTable = new SSqlFetchingModel();
    ui->tableViewClientMatch->setModel(clientsMatchTable);
}

SClientMatch::~SClientMatch()
{
    delete ui;
    delete clientsMatchTable;
    delete groupBoxEventFilter;
}

void SClientMatch::clear()
{
    clientsMatchTable->clear();
    this->hide();
}

void SClientMatch::setClientType(int type)
{
    clientType = type;
}

void SClientMatch::setPhoneMask(const int index)
{
    phoneMask = clientPhoneTypesModel->index(index, 2).data().toString().replace(QRegularExpression("[09]"), "_");   // в маске телефона меняем 0 и 9 на подчеркивание; 0 и 9 — это специальные маскировочные символы (см. справку QLineEdit, inputMask)
}

void SClientMatch::findClient()
{
    QString enteredByUserDigits;    // строка символов, которые ввёл пользователь (т. е. текст отображаемый в lineEdit над которым выполнена операция т. н. XOR с заданной маской
    QStringList query_where;    // список условий для запроса к БД
    QString query;  // весь запрос к БД
    int i;

    if(phone.length())
        for (i = 0; i < phoneMask.length(); i++ )  // определяем какие символы ввёл пользователь
        {
            if(phoneMask.at(i) != phone.at(i))
                enteredByUserDigits.append(phone.at(i));
        }
    if(lastName.length() >= 3 || enteredByUserDigits.length() >= 3 || companyName.length() >= 3 )  // если пользователь ввёл более двух символов в одно из полей
    {
        query_where.clear();
        if (lastName.length() >= 3 )
            query_where << QString("LCASE(CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)) REGEXP LCASE('%1')").arg(lastName);   // условие поиска по фамилии, имени и отчеству
        if (companyName.length() >= 3 )
            query_where << QString("LCASE(CONCAT_WS(' ', t1.`name`, t1.`ur_name`, t1.`short_name`)) REGEXP LCASE('%1')").arg(companyName);   // условие поиска организации (тип: юр. лицо)
        if (enteredByUserDigits.length() >= 3 )
            query_where << QString("IFNULL(t2.`phone`, '') LIKE '%1' OR IFNULL(t2.`phone_clean`, '') REGEXP '%2'").arg(phone, enteredByUserDigits);   // условие поиска по телефонному номеру

        query = QUERY_SEL_CLIENT_MATCH.arg((query_where.count()>0?"AND (" + query_where.join(" OR ") + ")":""));
        clientsMatchTable->setQuery(query, QSqlDatabase::database("connMain"));
        // TODO: сортировка таблицы совпадений
        if (clientsMatchTable->rowCount() > 0)
        {
            this->show();  // только если возвращены результаты, показываем виджет
            ui->tableViewClientMatch->show();       // может случиться ситуация, когда таблица будет скрыта, поэтому принудительно отображаем её
        }
        else
            this->hide();  // иначе прячем виджет
    }
    else    // если кол-во введённых пользователем символов меньше трёх, то удаляем результаты предыдущего запроса и прячем виджет.
    {
        clear();
    }
}

void SClientMatch::findByName(const QString &text)
{
    switch(clientType)
    {
        case NameSearchScope::LastName: lastName = text; break;
        case NameSearchScope::Name: companyName = text; break;
        default: companyName = text; lastName = text;
    }
    findClient();
}

void SClientMatch::findByPhone(const QString &text)
{
    phone = text;
    findClient();
}

void SClientMatch::findByPhone(const QString &text, const int mask)
{
    setPhoneMask(mask);
    findByPhone(text);
}

void SClientMatch::clientMatchTableDoubleClicked(QModelIndex index)
{
    emit clientSelected(clientsMatchTable->index(index.row(), 0).data().toInt());
    ui->tableViewClientMatch->hide();   // прячем таблицу, а не весь groupBox (вдруг пользователь промахнётся)
}

void SClientMatch::toggleElementsVisibility()
{
    ui->tableViewClientMatch->setVisible(!ui->tableViewClientMatch->isVisible());
}

// ===============================================================================================================
matchingClientsTable::matchingClientsTable(QWidget *parent) :
    STableViewBase(SLocalSettings::ClientsMatchGrid, parent)
{
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::FullName].Width_marked = true;  // по умолчанию автоширина столбца с ФИО
}

matchingClientsTable::~matchingClientsTable()
{
}

void matchingClientsTable::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SSqlFetchingModel*>(model);
    STableViewBase::setModel(model);
//    matchingClientsTableItemDelegates *itemDelagates = new matchingClientsTableItemDelegates(m_model, this);
//    setItemDelegate(itemDelagates);
}

void matchingClientsTable::translateNames()
{
    tr("FullName");
    tr("Balance");
    tr("Repairs");
    tr("Purchases");
    tr("Type");
    tr("PrimaryPhone");
}
