#include "tabclientsummary.h"
#include "ui_tabclientsummary.h"

tabClientSummary::tabClientSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabClientSummary)
{
    ui->setupUi(this);
}

tabClientSummary::~tabClientSummary()
{
    delete ui;
}
