#include "spagesalarysales.h"
#include "ui_spagesalarysales.h"
#include "stableviewsalarysaleparts.h"
#include "stableviewsalarysales.h"

SPageSalarySales::SPageSalarySales(QWidget *parent) :
    SPageSalaryBase(parent),
    ui(new Ui::SPageSalarySales)
{
    ui->setupUi(this);
    SPageSalarySales::guiFontChanged();

    ui->tableViewSales->setModel(parentTab->m_sales);
    ui->tableViewSaleParts->setModel(parentTab->m_saleParts);

    connect(ui->tableViewSales, &STableViewSalarySales::pressed, this, &SPageSalarySales::tableSalesRowActivated);
    connect(ui->tableViewSales, &STableViewSalarySales::clicked, this, &SPageSalarySales::tableSalesRowActivated);
    connect(ui->tableViewSales, &STableViewSalarySales::activated, this, &SPageSalarySales::tableSalesRowActivated);
    connect(ui->tableViewSales, &STableViewSalarySales::doubleClicked, this, &SPageSalarySales::tableSalesRowDoubleClicked);
    connect(ui->tableViewSaleParts, &STableViewSalarySales::doubleClicked, this, &SPageSalarySales::tableSalePartsRowDoubleClicked);
    connect(ui->tableViewSales->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SPageSalarySales::tableSalesRowSelected);
}

SPageSalarySales::~SPageSalarySales()
{
    delete ui;
}

void SPageSalarySales::loadDocumentData(const int id)
{
    ui->tableViewSaleParts->setQuery(QUERY_SEL_SALARY_SALE_PARTS(id));
    ui->tableViewSaleParts->refresh();
}

void SPageSalarySales::updateModels()
{
    ui->tableViewSales->setQuery(QUERY_SEL_SALARY_SALE_DOCS(
                                   parentTab->periodBegin(),
                                   parentTab->periodEnd(),
                                   parentTab->employeeId()));

    ui->tableViewSales->refresh();

    parentTab->setModelUpdatedFlag(SalesModel);
}

void SPageSalarySales::updateWidgets()
{
    m_userModel = parentTab->m_userModel;

    ui->labelPercentPartsValue->setText(QString::number(m_userModel->paySale()));
    ui->labelSalesSummValue->setText(sysLocale.toString(parentTab->m_sales->total(5), 'f', comSettings->classicKassa?2:0));
}

void SPageSalarySales::tableSalesRowSelected(const QModelIndex &current, const QModelIndex &prev)
{
    Q_UNUSED(prev);
    tableSalesRowActivated(current);
}

void SPageSalarySales::tableSalesRowActivated(const QModelIndex &index)
{
    loadDocumentData(parentTab->m_sales->index(index.row(), 0).data().toInt());
}

void SPageSalarySales::tableSalesRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSale(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalarySales::tableSalePartsRowDoubleClicked(const QModelIndex &index)
{
    mainWindow->createTabSparePart(parentTab->m_repairs->index(index.row(), 0).data().toInt());
}

void SPageSalarySales::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize);
    font.setBold(true);

    ui->labelPercentPartsValue->setFont(font);
    ui->labelSalesSummValue->setFont(font);
}

