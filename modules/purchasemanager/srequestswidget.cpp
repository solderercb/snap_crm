#include "srequestswidget.h"
#include "ui_srequestswidget.h"
#include "mainwindow.h"
#include "modules/purchasemanager/tabrequest.h"
#include "modules/purchasemanager/tabmanager.h"

SPartsRequests::SPartsRequests(QWidget *parent) :
    SWidget(parent),
    ui(new Ui::SPartsRequests)
{
    ui->setupUi(this);

    m_model = new SPartsRequestsModel(this, QSqlDatabase::database("connMain"));
    m_model->setEditStrategy(QSqlTableModel::EditStrategy::OnManualSubmit);
    ui->tableView->setModel(m_model);
    ui->tableView->setQuery(QUERY_SEL_PARTS_REQUESTS_STATIC, QSqlDatabase::database("connMain"));
    ui->tableView->setGrouping(QStringList{"t1.`id`"});    // default GROUP part of query
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &SPartsRequests::tableRowSelected);
    connect(ui->pushButtonSaveChanges, &QPushButton::clicked, this, &SPartsRequests::commit);
    connect(ui->tableView, &STableViewPartsRequests::doubleClicked, this, &SPartsRequests::tableRowDoubleClicked);
    connect(ui->tableView, &STableViewPartsRequests::checkChanges, this, &SPartsRequests::checkChanges);
    connect(ui->pushButtonCreateCopy, &QPushButton::clicked, this, &SPartsRequests::createRequestCopy);
    connect(m_model, &SPartsRequestsModel::modelReset, this, &SPartsRequests::modelReset);
    connect(m_model, &SPartsRequestsModel::stateChanged, this, &SPartsRequests::requestStateChanged);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &SPartsRequests::cancelRequests);

    updateWidgets();
}

SPartsRequests::~SPartsRequests()
{
    delete ui;
}

void SPartsRequests::updateWidgets()
{
    ui->pushButtonCreateCopy->setVisible(m_buttonsVisible);
    ui->pushButtonSaveChanges->setVisible(m_buttonsVisible);
    ui->pushButtonCreateInvoice->setVisible(m_buttonsVisible);
}

bool SPartsRequests::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    return false;
}

STableViewBase *SPartsRequests::tableView()
{
    return ui->tableView;
}

/* Вызов метода проверки несохранённых данных в таблицах заявок и ссылок на вкладке Мереджер закупок
 * Проверку нужно выполнять только на вкладке Менеджера закупок, а в карточке ремонта не нужно.
 * Параметры аналогичны методу tabPurchaseManager::confirmChanges
*/
void SPartsRequests::checkChanges(int &result, const int mask, QMessageBox::StandardButtons buttons)
{
    tabPurchaseManager *tab = dynamic_cast<tabPurchaseManager*>(findParentTab());
    if(!tab)
        return;

    tab->confirmChanges(result, mask, buttons);
}

void SPartsRequests::setFilter(const FilterList &filter)
{
    ui->tableView->setFilter(filter);
}

void SPartsRequests::refresh(bool preserveScrollPos, bool preserveSelection)
{
    ui->tableView->refresh(preserveScrollPos, preserveSelection);
}

void SPartsRequests::delayedRefresh(const int msec)
{
    ui->tableView->delayedRefresh(msec);
}

/* Скрытие кнопок Новый запрос, Сохранить и Создать ПН
 * При использовании виджета в карточке ремонта кнопки отображать не нужно
*/
void SPartsRequests::setButtonsVisible(bool visible)
{
    m_buttonsVisible = visible;
    updateWidgets();
}

bool SPartsRequests::isDirty()
{
    return m_model->isDirty();
}

void SPartsRequests::commit()
{
    m_model->commit();

    shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Менеджер закупок"), tr("Изменения успешно сохранёны"), QColor(214,239,220), QColor(229,245,234));
}

void SPartsRequests::tableRowSelected(const QModelIndex &current, const QModelIndex &prev)
{
    Q_UNUSED(prev)
    emit rowSelected(current.siblingAtColumn(SPartsRequestsModel::Columns::Id).data().toInt());
    emit requestStateChanged((SPartRequest::State)m_model->unformattedData(current.siblingAtColumn(SPartsRequestsModel::Columns::State)).toInt());
}

void SPartsRequests::tableRowDoubleClicked(const QModelIndex &index)
{
    MainWindow::getInstance()->createTabPartRequest(index.siblingAtColumn(SPartsRequestsModel::Columns::Id).data().toInt());
}

void SPartsRequests::cancelRequests()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();

    if(!selection->hasSelection())
        return;

    QModelIndexList rows = selection->selectedRows();
    QModelIndexList::const_iterator it = rows.constBegin();
    while(it != rows.constEnd())
    {
        if(m_model->flags(it->siblingAtColumn(SPartsRequestsModel::Columns::State)) & Qt::ItemIsEditable)
        {
            m_model->setData(it->siblingAtColumn(SPartsRequestsModel::Columns::State), SPartRequest::State::Cancelled);
        }
        ++it;
    }
}

void SPartsRequests::createRequestCopy()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();
    MainWindow *mw = MainWindow::getInstance();
    tabPartRequest *newTab = nullptr;

    if(!selection->hasSelection())
        return;

    if(selection->selectedRows().count() > 1)
    {
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Информация"), tr("Выделено более одной строки"), QColor(212,237,242), QColor(229,244,247));
        return;
    }

    newTab = static_cast<tabPartRequest*>(mw->createTabPartRequest(0));
    newTab->copyRequest(m_model->index(selection->selectedRows().at(0).row(), SPartsRequestsModel::Columns::Id).data().toInt());
}
