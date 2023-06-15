#include "tabsettings.h"
#include "ui_tabsettings.h"
#include "global.h"
#include "widgets/spagesettingsdoctempateeditor.h"
#include "widgets/settingstabwidgets/ssettingspagerolesandpermissions.h"
#include "widgets/settingstabwidgets/ssettingspageglobal.h"

tabSettings* tabSettings::p_instance = nullptr;

tabSettings::tabSettings(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabSettings)
{
    ui->setupUi(this);
    connect(ui->pages, &SPagedInterface::updateBotToolbar, this, &tabSettings::updateBotToolbar);
    connect(ui->pages, &SPagedInterface::initPage, this, &tabSettings::initPage);
    connect(this, &tabSettings::pageInitFinished, ui->pages, &SPagedInterface::addPage);

//    ui->buttonSave->setEnabled(0);
    ui->buttonRefresh->setEnabled(0);

    ui->pages->setButtonText(0, "Персональные настройки");
//    ui->pages->switchPage(0);  // по-умолчанию страница 0
    if(permissions->editGlobalSettings)
    {
        ui->pages->addButton(tr("Основные"), QIcon(), Page::Global);
        ui->pages->addButton(tr("Организация"), QIcon(), Page::Company);
        ui->pages->addButton(tr("Сотрудники"), QIcon(), Page::Employees);
        ui->pages->addButton(tr("Роли"), QIcon(), Page::UserRoles);
        ui->pages->addButton(tr("Склады и ячейки"), QIcon(), Page::Warehouses);
        ui->pages->addButton(tr("Финансы"), QIcon(), Page::Finances);
        ui->pages->addButton(tr("Каталог устройств"), QIcon(), Page::Devices);
        ui->pages->addButton(tr("Статусы ремонта"), QIcon(), Page::RepairStatuses);
        ui->pages->addButton(tr("Поля и источники обращения"), QIcon(), Page::AdditionalFieldsAndAdSources);
        ui->pages->addButton(tr("Шаблоны документов"), QIcon(), Page::DocTemplates);
        ui->pages->addButton(tr("Оповещения"), QIcon(), Page::Notifications);
        ui->pages->addButton(tr("Резервное копирование"), QIcon(), Page::Backup);
    }
    ui->pages->redraw();
    // эмуляция задержки
#ifdef QT_DEBUG
    test_scheduler->start(500);
#endif
}

tabSettings::~tabSettings()
{
    delete ui;
    p_instance = nullptr;
}

QString tabSettings::tabTitle()
{
    return tr("Настройки");
}

bool tabSettings::tabCloseRequest()
{
    return ui->pages->closeRequest();
}

void tabSettings::updateBotToolbar(const int page)
{
    m_buttonSaveVisible = 1;
    m_buttonRefreshVisible = 0;
    m_lineEditSearchVisible = 0;

    switch (page)
    {
//        case Page::User: m_buttonSaveVisible = 0; break;
//        case Page::Global: if(!m_editStrategy) m_buttonSaveVisible = 0; break;
        case Page::DocTemplates: m_buttonSaveVisible = 0; break;
        default: ;
    }

    ui->buttonSave->setVisible(m_buttonSaveVisible);
    ui->buttonRefresh->setVisible(m_buttonRefreshVisible);
    ui->lineEditFilter->setVisible(m_lineEditSearchVisible);
}

void tabSettings::initPage(const int page)
{
    QWidget *widget;
    switch (page)
    {
//        case Page::User: widget = new QWidget(); break;
        case Page::Global: widget = new SSettingsPageGlobal(this); break;
//        case Page::Company: widget = new QWidget(); break;
//        case Page::Employees: widget = new QWidget(); break;
        case Page::UserRoles: widget = new SSettingsPageRolesAndPermissions(this); break;
//        case Page::Warehouses: widget = new QWidget(); break;
//        case Page::Finances: widget = new QWidget(); break;
//        case Page::Devices: widget = new QWidget(); break;
//        case Page::RepairStatuses: widget = new QWidget(); break;
//        case Page::AdditionalFieldsAndAdSources: widget = new QWidget(); break;
        case Page::DocTemplates: widget = new SPageSettingsDocTempateEditor(this); break;
//        case Page::Notifications: widget = new QWidget(); break;
//        case Page::Backup: widget = new QWidget(); break;
        default: return;
    }
    emit pageInitFinished(widget, page);
}

void tabSettings::buttonSaveClicked()
{
    bool nErr = 1;
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    m_queryLog = new SQueryLog();

    try
    {
        m_queryLog->start(metaObject()->className());
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);

        emit saveSettings();

#ifdef QT_DEBUG
//        throw 0; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (int type)
    {
        nErr = 0;
        if(type == 0)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
//            nErr = 1; // это чтобы проверить работу дальше
        }
        else
        {
            QUERY_COMMIT_ROLLBACK(query, nErr);
            shortlivedNotification *newPopup = new shortlivedNotification(this,
                                                                          tr("Информация"),
                                                                          tr("Не удалось сохранить настройки (ошибка запроса к БД)."),
                                                                          QColor(255,164,119),
                                                                          QColor(255,199,173));
        }
    }

    m_queryLog->stop();
    if(nErr)
        shortlivedNotification *newPopup = new shortlivedNotification(this, tabTitle(), tr("Настройки успешно сохранены"), QColor(214,239,220), QColor(229,245,234));

    emit updateDaughterPagesModels();

    delete query;
    delete m_queryLog;
}

tabSettings* tabSettings::getInstance(MainWindow *parent)
{
  if( !p_instance )
    p_instance = new tabSettings(parent);
  return p_instance;
}

#ifdef QT_DEBUG
void tabSettings::randomFill()
{

}

void tabSettings::test_scheduler_handler()
{
    ui->pages->switchPage(Page::Global);
//    ui->pages->switchPage(Page::DocTemplates);
//    test_scheduler2->start(3000);

}

void tabSettings::test_scheduler2_handler()
{
    deleteLater();
}
#endif

