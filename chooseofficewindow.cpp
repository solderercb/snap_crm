#include "chooseofficewindow.h"
#include "ui_chooseofficewindow.h"
#include <QComboBox>
#include <ProjectGlobals>
#include <SUserSettings>
#include <appVer>
#include <SSqlQueryModel>
#include <windowsDispatcher>

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
    ui->officesComboBox->setPlaceholderText(tr("<выберите офис>"));
    ui->officesComboBox->setStyleSheet(commonComboBoxStyleSheet);
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
        userDbData->set_currentOffice(officesModel->databaseIDByRow(comboBoxOfficeIndex));
        emit officeChoosed();
        this->deleteLater();
    }
}
