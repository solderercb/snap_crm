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
    ui->tableView->horizontalHeader()->setHidden(false);
    connect(groupBoxEventFilter,SIGNAL(toggleElementsVisibility()),this,SLOT(toggleElementsVisibility()));
    clientsMatchTable = new SSqlFetchingModel(this);
    ui->tableView->setModel(clientsMatchTable);
    ui->tableView->setQuery(QUERY_SEL_CLIENT_MATCH_STATIC, QSqlDatabase::database("connMain"));
}

SClientMatch::~SClientMatch()
{
    delete ui;
    delete groupBoxEventFilter;
}

void SClientMatch::clear()
{
    clientsMatchTable->clear();
    this->hide();
    lastName = QString();
    companyName = QString();
    phone = QString();
}

/* TODO: нужно изменить название метода на setNameScope */
void SClientMatch::setClientType(int type)
{
    clientType = type;
}

void SClientMatch::setCategory(int category)
{
    m_category = category;
}

void SClientMatch::setEnabled(bool state)
{
    m_enabled = state;
}

void SClientMatch::setPhoneMask(const int index)
{
    phoneMask = clientPhoneTypesModel->index(index, 2).data().toString().replace(QRegularExpression("[09]"), "_");   // в маске телефона меняем 0 и 9 на подчеркивание; 0 и 9 — это специальные маскировочные символы (см. справку QLineEdit, inputMask)
}

void SClientMatch::findClient()
{
    QString enteredByUserDigits;    // строка символов, которые ввёл пользователь (т. е. текст отображаемый в lineEdit над которым выполнена операция т. н. XOR с заданной маской
    QStringList groupClause;
    FilterList whereClause;
    whereClause.op = FilterList::And;

    if(phone.length())
        for (int i = 0; i < phoneMask.length(); i++ )  // определяем какие символы ввёл пользователь
        {
            if(phoneMask.at(i) != phone.at(i))
                enteredByUserDigits.append(phone.at(i));
        }

    whereClause.fields.append(STableViewBase::initFilterField("`state`", FilterField::Equals, 1));
    if(m_category != SClientModel::Categories::All)
         whereClause.fields.append(STableViewBase::initFilterField(clientsTypesList->item(m_category, 2)->text(), FilterField::NoOp, 1));  // категория клиентов

    FilterField::Op matchFlag;
    if(userDbData->useRegExpSearch)
        matchFlag = FilterField::RegExp;
    else
        matchFlag = FilterField::Contains;
    FilterList nameClause;
    nameClause.op = FilterList::Or;
    if (lastName.length() >= 3 )
        nameClause.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)", matchFlag, lastName, Qt::CaseInsensitive));

    if (companyName.length() >= 3 )
        nameClause.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t1.`name`, t1.`ur_name`, t1.`short_name`)", matchFlag, companyName, Qt::CaseInsensitive));

    if (enteredByUserDigits.length() >= 3 )
    {
        nameClause.fields.append(STableViewBase::initFilterField("IFNULL(t2.`phone`, '')", FilterField::Wildcard, phone));
        nameClause.fields.append(STableViewBase::initFilterField("IFNULL(t2.`phone_clean`, '')", FilterField::RegExp, enteredByUserDigits));
    }

    whereClause.childs.append(nameClause);

    groupClause.clear();
    groupClause << "t1.`id`";

    if(nameClause.fields.size())
    {
        ui->tableView->setFilter(whereClause);
        ui->tableView->setGrouping(groupClause);

        ui->tableView->refresh(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);

        // TODO: сортировка таблицы совпадений
        if (clientsMatchTable->rowCount() > 0)
        {
            this->show();  // только если возвращены результаты, показываем виджет
            ui->tableView->show();       // может случиться ситуация, когда таблица будет скрыта, поэтому принудительно отображаем её
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
    if(!m_enabled)
        return;

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
    ui->tableView->hide();   // прячем таблицу, а не весь groupBox (вдруг пользователь промахнётся)
}

void SClientMatch::toggleElementsVisibility()
{
    ui->tableView->setVisible(!ui->tableView->isVisible());
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

void matchingClientsTable::clearModel()
{
    m_model->clear();
}

void matchingClientsTable::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    m_model->setQuery(query, database);
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
