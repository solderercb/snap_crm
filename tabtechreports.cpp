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

    m_tableModel = new STableTechReportsModel(this);
    ui->tableView->setModel(m_tableModel);
    ui->tableView->setQuery(QUERY_SEL_TECH_REPORTS_STATIC, QSqlDatabase::database("connMain"));
    ui->tableView->setUniqueIdColumn(STableTechReportsModel::Columns::Id);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &tabTechReports::buttonRefreshClicked);
    connect(ui->pushButtonPrint, &QPushButton::clicked, this, &tabTechReports::buttonPrintClicked);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &tabTechReports::lineEditSearchTextChanged);
    refreshTable();

    ui->widgetPeriodSelector->setMovingInterval(SPeriodSelector::Year);
    ui->widgetPeriodSelector->setDefaultPeriod();
    connect(ui->widgetPeriodSelector, &SPeriodSelector::refreshButtonClicked, this, &tabTechReports::buttonRefreshClicked);
}

tabTechReports::~tabTechReports()
{
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
    constructQueryClause();
    ui->tableView->delayedRefresh(350);
}

void tabTechReports::constructQueryClause()
{
    FilterList list;
    list.op = FilterList::And;

    list.fields.append(STableViewBase::initFilterField("`company`", FilterField::Equals, companiesModel->databaseIDByRow(ui->comboBoxCompany->currentIndex())));

    FilterList searchText;
    searchText.op = FilterList::Or;

    searchText.fields.append(STableViewBase::initFilterField("t1.`num`", FilterField::Contains, ui->lineEditSearch->text()));
    searchText.fields.append(STableViewBase::initFilterField("t1.`device`", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    searchText.fields.append(STableViewBase::initFilterField("t1.`inventory_number`", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    searchText.fields.append(STableViewBase::initFilterField("t1.`serial_number`", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    searchText.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t2.`short_name`, t2.`ur_name`)", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));
    searchText.fields.append(STableViewBase::initFilterField("CONCAT_WS(' ', t2.`surname`, t2.`name`, t2.`patronymic`)", FilterField::Contains, ui->lineEditSearch->text(), Qt::CaseInsensitive));

    FilterList period;
    period.op = FilterList::And;

    period.fields.append(STableViewBase::initFilterField("t1.`created`", FilterField::MoreEq, ui->widgetPeriodSelector->periodBegin()));
    period.fields.append(STableViewBase::initFilterField("t1.`created`", FilterField::Less, ui->widgetPeriodSelector->periodEnd()));

    list.childs.append(searchText);
    list.childs.append(period);

    query_group.clear();

    ui->tableView->setFilter(list);
    ui->tableView->setGrouping(query_group);

}

void tabTechReports::refreshTable(bool preserveScrollPos, bool preserveSelection)
{
    constructQueryClause();

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
