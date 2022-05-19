#include "global.h"
#include "appver.h"
#include "chooseofficewindow.h"
#include "ui_chooseofficewindow.h"

chooseOfficeWindow::chooseOfficeWindow(windowsDispatcher *parent) :
    QDialog(nullptr),
    ui(new Ui::chooseOfficeWindow)
{
    ui->setupUi(this);

    windowsDispatcherObj = parent;

    ui->companiesComboBox->hide();
//    ui->companiesComboBox->setModel(windowsDispatcherObj->companiesModel);
//    ui->companiesComboBox->setModelColumn(0);
//    ui->companiesComboBox->setCurrentIndex(-1);
    ui->officesComboBox->setModel(officesModel);
    // QComboBox::setPlaceholderText(const QString&) https://bugreports.qt.io/browse/QTBUG-90595
    ui->officesComboBox->setPlaceholderText("<выберите офис>");
    ui->officesComboBox->setStyleSheet("QComboBox {  border: 1px solid gray;  padding: 1px 18px 1px 3px;}\
                                       QComboBox::drop-down {  border: 0px;}\
                                       QComboBox::down-arrow{  image: url(down-arrow.png);  width: 16px;  height: 20px;}\
                                       QComboBox::hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
                                       QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}");
    ui->officesComboBox->setModelColumn(1);
    ui->officesComboBox->setCurrentIndex(-1);
}

chooseOfficeWindow::~chooseOfficeWindow()
{
    delete ui;
}

void chooseOfficeWindow::accept()
{
//    int comboBoxCompanyIndex = ui->companiesComboBox->currentIndex();
//    userDbData->insert("current_company", windowsDispatcherObj->companiesModel->index(comboBoxCompanyIndex, 1).data().toInt());
    int comboBoxOfficeIndex = ui->officesComboBox->currentIndex();
    if (comboBoxOfficeIndex >= 0)
    {
        userDbData->insert("current_office", officesModel->record(comboBoxOfficeIndex).value("id").toInt());
        userDbData->insert("current_office_name", officesModel->record(comboBoxOfficeIndex).value("name").toString());
        emit officeChoosed();
        this->deleteLater();
    }
}
