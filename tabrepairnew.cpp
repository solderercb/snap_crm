#include "tabrepairnew.h"
#include "ui_tabrepairnew.h"
#include "bottoolbarwidget.h"

tabRepairNew::tabRepairNew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabRepairNew)
{
    ui->setupUi(this);
//    BotToolbarWidget* botToolbarWidget_ = new BotToolbarWidget(ui->botToolbar);
//    ui->gridLayout->addWidget(botToolbarWidget_, 30, 0, 1, 5, Qt::AlignBottom);
    ui->comboBox->addItem("aaaa");
    ui->comboBox->addItem("bbbb");
    if(QLineEdit *le = ui->comboBox->lineEdit())    // Рисуем кнопку очистки в комбобоксе (это работает)
    {
        le->setClearButtonEnabled(true);
    }


}

tabRepairNew::~tabRepairNew()
{
    delete ui;
}
