#include "tabtechreports.h"
#include "ui_tabtechreports.h"
#include "tabprintdialog.h"

tabTechReports* tabTechReports::p_instance = nullptr;

tabTechReports *tabTechReports::getInstance(MainWindow *parent)
{
    if( !p_instance )
      p_instance = new tabTechReports(parent);
    return p_instance;
}

tabTechReports::tabTechReports(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabTechReports)
{
    logUserActivity();
    ui->setupUi(this);
    ui->comboBoxCompany->setModel(companiesModel);

    this->setAttribute(Qt::WA_DeleteOnClose);

    tableUpdateDelay = new QTimer(this);
    connect(tableUpdateDelay, &QTimer::timeout, this, &tabTechReports::buttonRefreshClicked);
    tableUpdateDelay->setSingleShot(true);

    m_tableModel = new STableTechReportsModel();
    ui->tableView->setModel(m_tableModel);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &tabTechReports::buttonRefreshClicked);
    connect(ui->pushButtonPrint, &QPushButton::clicked, this, &tabTechReports::buttonPrintClicked);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &tabTechReports::lineEditSearchTextChanged);
    refreshTable();

    ui->widgetPeriodSelector->setMovingInterval(SPeriodSelector::Year);
    connect(ui->widgetPeriodSelector, &SPeriodSelector::refreshButtonClicked, this, &tabTechReports::buttonRefreshClicked);
}

tabTechReports::~tabTechReports()
{
    delete m_tableModel;
    delete ui;
    p_instance = nullptr;
}

QString tabTechReports::tabTitle()
{
    return tr("Акты тех. состояния");
}

QList<STechReportModel *> tabTechReports::reportsListFromSelection()
{
    QList<STechReportModel*> list;
    QList<int> *idsList = ui->tableView->selectedReportsList();
    STechReportModel *report;

    for(int i = 0; i < idsList->count(); i++)
    {
        report = new STechReportModel();
        report->load(idsList->at(i));
        list.append(report);
    }

    delete idsList;

    return list;
}

void tabTechReports::buttonRefreshClicked()
{
    refreshTable(STableViewBase::ScrollPosReset, STableViewBase::SelectionReset);
}

void tabTechReports::lineEditSearchTextChanged(QString)
{
    ui->tableView->clearSelection();
    ui->tableView->resetVScrollPos();
    tableUpdateDelay->stop();
    tableUpdateDelay->start(350);
}

void tabTechReports::refreshTable(bool preserveScrollPos, bool preserveSelection)
{
    ui->tableView->setQuery("SELECT t1.`id`, t1.`num`, t1.`created`, t1.`user`, t1.`company`, IF(t2.`type`, IF(LENGTH(TRIM(t2.`name`)), t2.`name`, t2.`ur_name`), CONCAT_WS(' ', t2.`surname`, t2.`name`, t2.`patronymic`)) AS 'client', t1.`device`, t1.`inventory_number`, t1.`serial_number`, CONVERT(t1.`production_date`, CHAR) AS 'production_date', CONVERT(t1.`purchase_date`, CHAR) AS 'purchase_date', t1.`initial_cost`, t1.`residual_cost`, t1.`fault`, t1.`diagnostic_result`, t1.`conclusion`, t1.`notes` FROM `tech_reports` AS t1 LEFT JOIN `clients` AS t2 ON t1.`client` = t2.`id`", QSqlDatabase::database("connMain"));
    ui->tableView->setUniqueIdColumn(STableTechReportsModel::Columns::Id);

    FilterList l1;
    l1.op = FilterList::Or;

    FilterList l2;
    l2.op = FilterList::Or;

    l2.fields.append(STableViewBase::initFilterField("t1.`num`", FilterField::Contains, ui->lineEditSearch->text()));
    l2.fields.append(STableViewBase::initFilterField("t1.`device`", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    l2.fields.append(STableViewBase::initFilterField("t1.`inventory_number`", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    l2.fields.append(STableViewBase::initFilterField("t1.`serial_number`", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    l2.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t2.`short_name`, t2.`ur_name`)", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    l2.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t2.`surname`, t2.`name`, t2.`patronymic`)", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));

    FilterList l3;
    l3.op = FilterList::And;

    l3.fields.append(STableViewBase::initFilterField("`company`", FilterField::Equals, companiesModel->databaseIDByRow(ui->comboBoxCompany->currentIndex())));
    l3.fields.append(STableViewBase::initFilterField("t1.`created`", FilterField::MoreEq, ui->widgetPeriodSelector->periodBegin()));
    l3.fields.append(STableViewBase::initFilterField("t1.`created`", FilterField::Less, ui->widgetPeriodSelector->periodEnd()));

    l1.childs.append(l2);
    l1.childs.append(l3);

//    query_group.clear();
//    ui->tableView->setGrouping(query_group);

    ui->tableView->setFilter(l1);
    ui->tableView->refresh(preserveScrollPos, preserveSelection);

//    if(!preserveSelection)  // при обновлении модели сигнал QItemSelectionModel::selectionChanged не генерируется
//        updateWidgets();
}

void tabTechReports::buttonPrintClicked()
{
    QList<STechReportModel*> list = reportsListFromSelection();
    tabPrintDialog::printTechReports(list, true);
//    tabPrintDialog *tab = tabPrintDialog::create(Global::Reports::tech_report);
//    tab->addDataModel(m_tableModel);
//    tab->startRender();
}
