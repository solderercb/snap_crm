#include "global.h"
#include "appver.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bottoolbarwidget.h"
#include "tabcommon.h"
#include "tabrepairnew.h"
#include "tabrepairs.h"
#include "tabrepair.h"
#include "tabsale.h"
#include "tabclients.h"
#include "tabprintdialog.h"
#include "mylineedit.h"
#include "com_sql_queries.h"

tabBarEventFilter::tabBarEventFilter(QObject *parent) :
    QObject(parent)
{
}

bool tabBarEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)  // закрытие вкладки по клику средней кнопкой мыши (колёсиком)
    {
        QTabBar *tabBar = static_cast<QTabBar*>(watched);
        QMouseEvent *mouseButtonPress = static_cast<QMouseEvent*>(event);

        if (mouseButtonPress->button() == Qt::MiddleButton)
        {
//            qDebug() << watched->objectName() << ": viewEventFilter: " << event << "; tab = " << tabBar->tabAt(mouseButtonPress->position().toPoint());
            emit tabBar->tabCloseRequested(tabBar->tabAt(mouseButtonPress->position().toPoint()));
        }
    }
    return false;
}

MainWindow* MainWindow::p_instance = nullptr;

MainWindow::MainWindow(windowsDispatcher *parent) :
    QMainWindow(nullptr),
	ui(new Ui::MainWindow)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
	QTextCodec::setCodecForLocale(codec);

    ui->setupUi(this);

    setWindowTitle("SNAP CRM ["+userData->value("current_office_name").toString()+"] ["+QSqlDatabase::database("connMain").userName()+"]");
    initGlobalModels();

    tabBarEventFilter *tabBarEventFilterObj = new tabBarEventFilter(this);  // Фильтр событий tabBar. В частности, закрытие вкладки по клику средней кнопкой мыши (колёсиком)
    ui->tabWidget->tabBar()->installEventFilter(tabBarEventFilterObj);

    this->move(0, 0);   // размер и положение окна по умолчанию
    this->resize(1440, 960);
    if(userLocalData->value("WorkspaceState").toString() == "Maximized")
    {
        setWindowState(Qt::WindowMaximized);
    }
    else
    {
        this->move(userLocalData->value("WorkspaceLeft").toInt(), userLocalData->value("WorkspaceTop").toInt());
        this->resize(userLocalData->value("WorkspaceWidth").toInt(), userLocalData->value("WorkspaceHeight").toInt());
    }

    QString toolButtonStyle = "QToolButton{\
                                   border: 0px;\
                                   background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                     stop: 0 #f6f7fa, stop: 1 #dadbde);\
                               }\
                               \
                               QToolButton::menu-button {\
                                   width: 32px;\
                               }";
    ui->toolBar->setStyleSheet("QToolBar::separator {width: 10px; border: none;}");
    /* Кнопка Ремонты и меню */
    QMenu *workshop_menu = new QMenu();
    QAction *workshop_new = new QAction("Принять", this);
    workshop_menu->addAction(workshop_new);
    QObject::connect(workshop_new,SIGNAL(triggered()),this,SLOT(createTabRepairNew()));
    QAction *workshop_refill = new QAction("Заправка", this);
    workshop_menu->addAction(workshop_refill);
    workshop_refill->setEnabled(false);
    QAction *workshop_price = new QAction("Прайс-лист", this);
    workshop_menu->addAction(workshop_price);
    workshop_price->setEnabled(false);
    QAction *workshop_editor = new QAction("Групповой редактор ремонтов", this);
    workshop_menu->addAction(workshop_editor);
    workshop_editor->setEnabled(false);
    QToolButton* workshop_button = new QToolButton();
//    QAction *workshop = new QAction("Ремонты", this);
//    workshop_button->addAction(workshop);
    workshop_button->setMenu(workshop_menu);
    workshop_button->setPopupMode(QToolButton::MenuButtonPopup);
    workshop_button->setText("Ремонты");
    workshop_button->setFixedSize(128,48);
    workshop_button->setStyleSheet(toolButtonStyle);
    ui->toolBar->addWidget(workshop_button);
    ui->toolBar->addSeparator();
    QObject::connect(workshop_button, SIGNAL(clicked()), this, SLOT(createTabRepairs()));

    /* Кнопка Товары и меню */
    QMenu *goods_menu = new QMenu();
    QAction *goods_arrival = new QAction("Приход", this);
    goods_menu->addAction(goods_arrival);
    goods_arrival->setEnabled(false);
    QAction *goods_sale = new QAction("Продажа", this);
    goods_menu->addAction(goods_sale);
    QObject::connect(goods_sale,SIGNAL(triggered()),this,SLOT(createTabSale()));
    QAction *store_docs = new QAction("Документы", this);
    goods_menu->addAction(store_docs);
    store_docs->setEnabled(false);
    QAction *purchase_manager = new QAction("Менеджер закупок", this);
    goods_menu->addAction(purchase_manager);
    purchase_manager->setEnabled(false);
    QAction *store_manager = new QAction("Управление складом", this);
    goods_menu->addAction(store_manager);
    store_manager->setEnabled(false);
    QAction *goods_editor = new QAction("Групповой редактор товаров", this);
    goods_menu->addAction(goods_editor);
    goods_editor->setEnabled(false);
    QAction *goods_uploader = new QAction("Выгрузка товаров", this);
    goods_menu->addAction(goods_uploader);
    goods_uploader->setEnabled(false);
    QAction *device_buyout = new QAction("Выкуп техники", this);
    goods_menu->addAction(device_buyout);
    device_buyout->setEnabled(false);
    QAction *stocktaking = new QAction("Переучет", this);
    goods_menu->addAction(stocktaking);
    stocktaking->setEnabled(false);
    QToolButton* goods_button = new QToolButton();
    goods_button->setMenu(goods_menu);
    goods_button->setPopupMode(QToolButton::MenuButtonPopup);
    goods_button->setText("Товары");
    goods_button->setFixedSize(128,48);
    goods_button->setStyleSheet(toolButtonStyle);
    ui->toolBar->addWidget(goods_button);
    ui->toolBar->addSeparator();

    /* Кнопка Клиенты и меню */
    QMenu *clients_menu = new QMenu();
    QAction *client_new = new QAction("Новый клиент", this);
    clients_menu->addAction(client_new);
    client_new->setEnabled(false);
    QAction *clients_calls = new QAction("Вызовы", this);
    clients_menu->addAction(clients_calls);
    clients_calls->setEnabled(false);
    QAction *clients_sms = new QAction("SMS", this);
    clients_menu->addAction(clients_sms);
    clients_sms->setEnabled(false);
//    QAction *clients_editor = new QAction("Групповой редактор клиентов", this);
//    clients_menu->addAction(clients_editor);
    QToolButton* clients_button = new QToolButton();
    clients_button->setMenu(clients_menu);
    clients_button->setPopupMode(QToolButton::MenuButtonPopup);
    clients_button->setText("Клиенты");
    clients_button->setFixedSize(128,48);
    clients_button->setStyleSheet(toolButtonStyle);
    ui->toolBar->addWidget(clients_button);
    QObject::connect(clients_button, SIGNAL(clicked()), this, SLOT(createTabClients()));

    QPushButton* pushButton05 = new QPushButton();
    pushButton05->setText("pushButton05");
//    pushButton05->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    ui->gridLayout_4->addWidget(pushButton05, 0, 0, 1, 1);
    QPushButton* pushButton04 = new QPushButton();
    pushButton04->setText("pushButton04");
    ui->gridLayout_4->addWidget(pushButton04, 1, 1, 1, 1);
    QPushButton* pushButton03 = new QPushButton();
    pushButton03->setText("pushButton03");
    ui->gridLayout_4->addWidget(pushButton03, 2, 2, 1, 1);
    QPushButton* pushButton02 = new QPushButton();
    pushButton02->setText("pushButton02");
    ui->gridLayout_4->addWidget(pushButton02, 3, 3, 1, 1);
    QPushButton* pushButton01 = new QPushButton();
    pushButton01->setText("pushButton01");
    ui->gridLayout_4->addWidget(pushButton01, 4, 4, 1, 1);

    // ТУТА нужно быть аккуратным! Если в конструкторе tabRepairs создаётся связь со слотом объекта MainWindow, то получим цикл.
    // лучше создавать вкладки по-умолчанию в main.cpp
//    createTabRepairs(0);

//    createTabRepairNew();
//    createTabRepairs(1);
//    createTabRepair();

    comboboxSourceModel = new QStandardItemModel();
	ui->comboBoxSourceWarehouse->setModel(comboboxSourceModel);
	comboboxDestModel = new QStandardItemModel();
	ui->comboBoxDestWarehouse->setModel(comboboxDestModel);

	tree_model = new QStandardItemModel();
	ui->treeView->setUniformRowHeights(true); // all items in the treeview have the same height
	ui->treeView->setModel(tree_model);

	tableGoodsModel = new QStandardItemModel(0,3);	// Три столбца
//	proxy_tableGoodsHeaders = new QSortFilterProxyModel(this);
//	proxy_tableGoodsHeaders->setSourceModel(tableGoodsModel);
//	proxy_tableGoodsHeaders->setSortRole(Qt::EditRole);
	ui->tableGoods->setModel(tableGoodsModel);
//	ui->tableGoods->setModel(proxy_tableGoodsHeaders);
//	ui->tableGoods->horizontalHeader()->setEnabled(true);
	tableGoodsHeaders << "Артикул" << "Наименование" << "Примечание";
	tableGoodsModel->setHorizontalHeaderLabels(tableGoodsHeaders);
	ui->tableGoods->horizontalHeader()->setStretchLastSection(true);
//	or
//	ui->tableGoods->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ширина столбцов одинаковая, неизменяемая.
//	ui->label->setText(QString::number(ui->tableGoods->geometry().width()));
	ui->tableGoods->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui->tableGoods->verticalHeader()->setDefaultSectionSize(18);
	ui->tableGoods->setColumnWidth(0, 50);
	ui->tableGoods->setColumnWidth(1, 200);
/*
	ui->tableGoods->setColumnWidth(0, (ui->verticalLayout_3->geometry().width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent))*0.12-1);
	ui->tableGoods->setColumnWidth(1, (ui->verticalLayout_3->geometry().width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent))*0.44-1);
	ui->tableGoods->setColumnWidth(2, (ui->verticalLayout_3->geometry().width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent))*0.44-1);
*/

	tableConsignmentsModel = new QStandardItemModel(0,8); // Семь столбцов
	tableConsignmentsHeaders << "Артикул" << "Наименование" << "Примечание" << "Кол-во" << "Реализация" << "Резерв" << "Цена" << "Место";
	tableConsignmentsModel->setHorizontalHeaderLabels(tableConsignmentsHeaders);
	ui->tableConsignments->setModel(tableConsignmentsModel);
	ui->tableConsignments->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui->tableConsignments->verticalHeader()->setDefaultSectionSize(18);

	QObject::connect(ui->btnLogin,SIGNAL(clicked(bool)),this,SLOT(btnClick()));


    for(int i = ui->tabWidget->count() - 1; i >= 0; i--)
        closeTab(i);

#ifdef QT_DEBUG
    get_warehouses_list();
    btnClick();

    test_scheduler = new QTimer();
    test_scheduler->setSingleShot(true);
    test_scheduler2 = new QTimer();
    test_scheduler2->setSingleShot(true);
    QObject::connect(test_scheduler, SIGNAL(timeout()), this, SLOT(test_scheduler_handler()));
    QObject::connect(test_scheduler2, SIGNAL(timeout()), this, SLOT(test_scheduler2_handler()));
    test_scheduler->start(200);
#endif
}

// https://stackoverflow.com/a/17482796
void MainWindow::closeEvent(QCloseEvent *event)
{
//    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "APP_NAME",
//                                                                tr("Are you sure?\n"),
//                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
//                                                                QMessageBox::Yes);
//    if (resBtn != QMessageBox::Yes) {
//        event->ignore();
//    } else {
//        event->accept();
//    }
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    bool nDBErr = 1;
    query->exec(QUERY_BEGIN);
    QUERY_EXEC(query, nDBErr)(QUERY_UPD_LAST_USER_ACTIVITY(userData->value("id").toString()));
    QUERY_EXEC(query, nDBErr)(QUERY_INS_USER_ACTIVITY(QString("Logout")));
    QUERY_COMMIT_ROLLBACK(query, nDBErr);
    delete query;
}

MainWindow::~MainWindow()
{
    delete tableConsignmentsModel;
	delete tableGoodsModel;
	delete tree_model;
	delete comboboxSourceModel;
	delete ui;
    p_instance = nullptr;
    delete warehousesModel;
    delete usersModel;
    delete managersModel;
    delete engineersModel;
    delete repairBoxesModel;
    for (int i = 0; i<connections.size(); i++)  // открываем вспомогательные соединения
        connections[i]->close();
}

MainWindow* MainWindow::getInstance(windowsDispatcher *parent)   // singleton: MainWindow только один объект
{
    if( !p_instance )
      p_instance = new MainWindow(parent);

    return p_instance;
}

void MainWindow::btnClick()
{
    enum {store_cats_id, store_cats_parent, store_cats_name, store_cats_position};

    if (!(QSqlDatabase::database("connMain").isOpen()))
    {
        return;
    }

    QSqlQuery* cat_tree = new QSqlQuery(QSqlDatabase::database("connMain"));
//    QStandardItem* parentItem;
//    QStandardItem* childItem;
    QVector<uint32_t> store_cats_ids;
    QVector<QStandardItem*> store_cats;
    uint32_t parent = 0, i = 0, j = 0;

//    QStandardItem* testItem = new QStandardItem("test");

//	parentItem = tree_model->invisibleRootItem();
    cat_tree->exec("SELECT `id`,`parent`,`name` FROM store_cats WHERE `store_id` = 1 AND `enable` = 1 ORDER BY `parent`, `position`;");

    store_cats.resize(cat_tree->size());
    store_cats_ids.resize(cat_tree->size());

    while(cat_tree->next())
	{
        QStandardItem* item = new QStandardItem(cat_tree->value(store_cats_name).toString());
        item->setData(cat_tree->value(store_cats_id).toString(), Qt::UserRole+1); // Кроме текстовой записи сохраняем данные столбцов code и parent
        item->setData(cat_tree->value(store_cats_parent).toString(), Qt::UserRole+2); // Работает. НЕ ТРОГАТЬ!
        if(!cat_tree->value(store_cats_parent).toInt()) // Если текущий элемент выборки не содержит значения parent, то сразу добавляем его в TreeView (это корневой эл-т)
            tree_model->appendRow(item);
        store_cats_ids[i] = cat_tree->value(store_cats_id).toInt();
        store_cats[i] = item;  // указатели на объекты храним в массиве; ключ массива — id категории товара
        i++;

//        qDebug() << cat_tree->value(store_cats_id).toInt();
    }
    for(uint i=0; i < store_cats.size(); i++)
    {
//        if (store_cats[i])  // обработку производим только тех эл-тов массива, в которых содержится указатель на объект
//        {
//            childItem = store_cats[i];
            parent = store_cats[i]->data(Qt::UserRole+2).toInt();
            if(parent)  // если записан parent
            {
                j = 0;
                while (store_cats_ids[j] != parent)
                    j++;
//                parentItem = store_cats[j];
//                qDebug() << store_cats[i]->text().toStdWString() << " and it's parent " << store_cats[j]->text().toStdWString();
                store_cats[j]->appendRow(store_cats[i]);
            }
//        }
    }
    delete cat_tree;
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
	readGoods(index, ui->comboBoxSourceWarehouse->currentData(Qt::UserRole+1).toString());
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
	readGoods(index, ui->comboBoxSourceWarehouse->currentData(Qt::UserRole+1).toString());
}

void MainWindow::readGoods(const QModelIndex &index, const QString &warehouse_code)
{
    if (!(QSqlDatabase::database("connMain").isOpen()))
    {
        return;
    }

	QVariant a = ui->treeView->model()->data(index, Qt::UserRole+1);
	QVariant b = ui->treeView->model()->data(index, Qt::UserRole+2);
	ui->label->setText("code: "
										 + a.toString()
										 + "; parent: "
										 + b.toString()); // Работает. НЕ ТРОГАТЬ!

	tableGoodsModel->setRowCount(0); // Удаляем имеющиеся строки
    QSqlQuery* goods = new QSqlQuery(QSqlDatabase::database("connMain"));
	QString query;
	QStandardItem* newCol;

    query = QString("SELECT t1.`articul`, t1.`name`, t1.`description` FROM store_items AS t1 WHERE t1.`category` LIKE '%%1%' AND t1.`count`>0 AND t1.`store` LIKE '%2' GROUP BY t1.`articul` ORDER BY t1.`articul`;").arg(ui->treeView->model()->data(index, Qt::UserRole+1).toString(),warehouse_code);
	goods->exec(query);

	while(goods->next())
	{
		QList<QStandardItem*> newRow;

		for(uint8_t i=0;i<3;i++)
		{
			newCol = new QStandardItem(goods->value(i).toString()); // Артикул, Наименование и Примечание
			newRow.append(newCol);
		}

		tableGoodsModel->appendRow(newRow);
	}

	delete goods;
}

void MainWindow::get_warehouses_list()
{
    if (!(QSqlDatabase::database("connMain").isOpen()))
    {
        return;
    }

    QSqlQuery* warehouse_list = new QSqlQuery(QSqlDatabase::database("connMain"));
	QString query;
	QStandardItem *newRow;
	QStandardItem *newRow2;

    query = QString("SELECT `name`, `id`, `office` FROM stores WHERE `active` = 1;");
	warehouse_list->exec(query);

    newRow = new QStandardItem();
    newRow->setText("All");
    newRow->setData("0",Qt::UserRole+1);
    comboboxSourceModel->appendRow(newRow);	// Добавляем строку в модель comboBox (склад-источник)

	while(warehouse_list->next())
	{
		newRow = new QStandardItem();
		newRow2 = new QStandardItem();
        newRow->setText(warehouse_list->value(0).toString());
        newRow->setData(warehouse_list->value(1).toString(),Qt::UserRole+1);
        newRow2->setText(warehouse_list->value(0).toString());
        newRow2->setData(warehouse_list->value(1).toString(),Qt::UserRole+1);
		comboboxSourceModel->appendRow(newRow);	// Добавляем строку в модель comboBox (склад-источник)
		comboboxDestModel->appendRow(newRow2);	// Добавляем строку в модель comboBox (склад-приёмник)
	}

//	ui->comboBoxDestWarehouse->setCurrentIndex(1); // По умолчанию складом-приёмником является второй элемент в списке

    delete warehouse_list;
}

void MainWindow::initGlobalModels()
{
    QString query;

    queryCommonSettings = new QSqlQuery(QSqlDatabase::database("connMain"));

    queryCommonSettings->exec(QUERY_SEL_COMMON_SETTINGS);
    comSettings = new QMap<QString, QVariant>;
    queryCommonSettings->first();
    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные общих настроек могут быть дополнены.
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД.
    // TODO: чтение файла настроек в AppData
    // TODO: чтение файлов настроек пользователя из каталога приложения (например, program files): RepairsGrid-user.xml, SaleGrid-user.xml и многие другие
    for (int i = 0; i < queryCommonSettings->record().count(); i++)
    {
        comSettings->insert(queryCommonSettings->record().fieldName(i), queryCommonSettings->value(i));
    }
    queryCommonSettings->exec(QUERY_SEL_COMMON_SETTINGS2);  // вторая "порция" общих настроек ( в ASC CRM настроки из табил. config начиная с v3.7.18.969 schemaversion 258 постепенно "переезжают" в табл. settings; нова ятаблица имеет приоритет
    while(queryCommonSettings->next())
    {
        if (comSettings->contains(queryCommonSettings->value(0).toString()))
            comSettings->value(queryCommonSettings->value(0).toString()) = queryCommonSettings->value(1);
        else
            comSettings->insert(queryCommonSettings->value(0).toString(), queryCommonSettings->value(1));
    }

    for (int i=0; i < comSettings->value("phone_mask1").toString().size(); i++)     // В ASC CRM (а точнее в DevExpress) маски ввода в lineEdit работают не так, как в Qt
        if (comSettings->value("phone_mask1").toString().at(i) == '0')              // В Qt: 0 - это необязательная цифра, а 9 — обязательная цифра. Чтобы работала проверка
        {                                                                           // введённого номера телефона (если в настройках установлен флаг обязательности), нужно изменить маску.
            qDebug() << "МАСКА ТЕЛЕФОНА 1!!!";                                      // (это заглушка для отладки)
            break;                                                                  // TODO: добавить хитрую подсказку для полей ввода номера, если обнаруживается такое несовпадение
        }
    for (int i=0; i < comSettings->value("phone_mask2").toString().size(); i++)
        if (comSettings->value("phone_mask2").toString().at(i) == '0')
        {
            qDebug() << "МАСКА ТЕЛЕФОНА 2!!!";
            break;
        }
    comSettings->insert("phone_mask1", comSettings->value("phone_mask1").toString().replace('0', '9')); // предупреждалка пусть орёт, но чтобы работала проверка, изменим маску внаглую
    comSettings->insert("phone_mask2", comSettings->value("phone_mask2").toString().replace('0', '9'));

    QList<QStandardItem*> *clientPhoneTypesSelector;
    clientPhoneTypesSelector = new QList<QStandardItem*>();
    *clientPhoneTypesSelector << new QStandardItem("мобильный") << new QStandardItem("1") << new QStandardItem(comSettings->value("phone_mask1").toString()); // в ASC формат задаётся нулями, но в поиске совпадающих клиентов  это предусмотрено
    clientPhoneTypesModel->appendRow( *clientPhoneTypesSelector );
    clientPhoneTypesSelector = new QList<QStandardItem*>();;
    *clientPhoneTypesSelector << new QStandardItem("городской") << new QStandardItem("2") << new QStandardItem(comSettings->value("phone_mask2").toString());
    clientPhoneTypesModel->appendRow( *clientPhoneTypesSelector );

    warehousesModel->setQuery(QUERY_SEL_WAREHOUSES(userData->value("current_office").toInt()), QSqlDatabase::database("connMain"));
    allUsersModel->setQuery(QUERY_SEL_ALL_USERS, QSqlDatabase::database("connMain"));
    for(int i = 0; i < allUsersModel->rowCount(); i++)
    {
        allUsersMap->insert(allUsersModel->record(i).value("id").toInt(), allUsersModel->record(i).value("username").toString());
    }
    usersModel->setQuery(QUERY_SEL_USERS, QSqlDatabase::database("connMain"));
    managersModel->setQuery(QUERY_SEL_MANAGERS, QSqlDatabase::database("connMain"));
    engineersModel->setQuery(QUERY_SEL_ENGINEERS, QSqlDatabase::database("connMain"));
//    itemBoxesModel->setQuery(QUERY_SEL_ITEM_BOXES(userData->value("current_office").toInt()), QSqlDatabase::database("connMain"));
    repairBoxesModel->setQuery(QUERY_SEL_REPAIR_BOXES, QSqlDatabase::database("connMain"));
    paymentSystemsModel->setQuery(QUERY_SEL_PAYMENT_SYSTEMS, QSqlDatabase::database("connMain")); // TODO: нужна прокси-модель для отображения платёжных систем в соответствии с правами пользователя
    clientAdTypesList->setQuery(QUERY_SEL_CLIENT_AD_TYPES, QSqlDatabase::database("connMain"));

    QVector<QString> clientTypesList = {"Все клиенты", "Организации", "Посредники", "Поставщики", "Постоянные клиенты", "Проблемные клиенты", "Реализаторы", "Архивные"};
    QVector<QString> clientTypesQueryFilterList = {"t1.`state` = 1", "t1.`type` = 1 AND t1.`state` = 1", "t1.`is_agent` = 1 AND t1.`state` = 1", "t1.`is_dealer` = 1 AND t1.`state` = 1", "t1.`is_regular` = 1 AND t1.`state` = 1", "t1.`is_bad` = 1 AND t1.`state` = 1", "t1.`is_realizator` = 1 AND t1.`state` = 1", "t1.`state` = 0"};
    for (int i=0; i<clientTypesList.size(); i++)
    {
        clientTypeSelector = new QList<QStandardItem*>();
        *clientTypeSelector << new QStandardItem(clientTypesList.at(i)) << new QStandardItem(QString::number(i)) << new QStandardItem(clientTypesQueryFilterList.at(i));
        clientsTypesList->appendRow(*clientTypeSelector);
    }

    QJsonDocument jsonDoc(QJsonDocument::fromJson(comSettings->value("statuses").toByteArray()));
    if (jsonDoc.isArray())
    {
        QJsonArray jsonArray(jsonDoc.array());
        readStatuses(*statusesModel, jsonArray);
    }
    else
    {
        QList<QStandardItem*> *el = new QList<QStandardItem*>({new QStandardItem("Model init failed")});
        statusesModel->appendRow(*el);
    }

    QVector<QString> notifyStatusesList = {"---", "Клиент оповещён", "Клиент не отвечает", "Клиент не доступен", "Не оповещён прочее"};
    for (int i=0; i<notifyStatusesList.size(); i++)
    {
        notifyStatusSelector = new QList<QStandardItem*>();
        *notifyStatusSelector << new QStandardItem(notifyStatusesList.at(i)) << new QStandardItem(QString::number(i));
        notifyStatusesModel->appendRow(*notifyStatusSelector);
    }

    // TODO: В АСЦ в списке сроков гарантии также присутствуют "Согласно гарантии производителя", "Согласно ЗоЗПП" и "Согласно ФГТ", но, похоже, их выбор не реализован
    QVector<QString> warrantyTermsList = {"нет", "7 дней", "14 дней", "1 мес", "2 мес", "3 мес", "4 мес", "6 мес", "1 год", "2 года", "3 года"};
    int warrantyTerms[] = {0, 7, 14, 31, 62, 93, 124, 186, 365, 730, 1095};
    // Модель для комбобоксов при редактировании, а QMap для отображения
    for (int i=0; i<warrantyTermsList.size(); i++)
    {
        warrantyTermSelector = new QList<QStandardItem*>();
        *warrantyTermSelector << new QStandardItem(warrantyTermsList.at(i)) << new QStandardItem(QString::number(warrantyTerms[i]));
        warrantyTermsModel->appendRow(*warrantyTermSelector);
        warrantyTermsMap->insert(warrantyTerms[i], warrantyTermsList.at(i));
    }

    QVector<QString> rejectReasonList = {"отказ от ремонта", "ремонт не возможен", "ремонт не возможен из-за отсутствия запчастей", "ремонт не рентабелен", "неисправносте не проявилась", "другие причины"};
    QList<QStandardItem*> *rejectReasonSelector;
    for (int i=0; i<rejectReasonList.size(); i++)
    {
        rejectReasonSelector = new QList<QStandardItem*>();
        *rejectReasonSelector << new QStandardItem(rejectReasonList.at(i)) << new QStandardItem(QString::number(i));
        rejectReasonModel->appendRow(*rejectReasonSelector);
    }

    QVector<QString> priceColNamesList = {"Цена для сервиса", "Цена розница", "Цена опт", "Цена опт2", "Цена опт3"};
    QVector<QString> priceColIdsList = {"1", "2", "3", "4", "5"};
    QVector<QString> priceColDBFieldsList = {"price", "price2", "price3", "price4", "price5"};
    QList<QStandardItem*> *priceColSelector;
    for (int i=0; i<priceColNamesList.size(); i++)
    {
        priceColSelector = new QList<QStandardItem*>();
        *priceColSelector << new QStandardItem(priceColNamesList.at(i)) << new QStandardItem(priceColIdsList.at(i)) << new QStandardItem(priceColDBFieldsList.at(i));
        priceColModel->appendRow(*priceColSelector);
    }

    QVector<QString> itemUnitsList = {"шт", "г", "м", "см", "л"};
    QVector<QString> itemUnitsIdsList = {"1", "2", "3", "4", "5"};
    QList<QStandardItem*> *itemUnitsSelector;
    for (int i=0; i<itemUnitsList.size(); i++)
    {
        itemUnitsSelector = new QList<QStandardItem*>();
        *itemUnitsSelector << new QStandardItem(itemUnitsList.at(i)) << new QStandardItem(itemUnitsIdsList.at(i));
        itemUnitsModel->appendRow(*itemUnitsSelector);
    }

#ifdef QT_DEBUG
    initClients4Test();
#endif
}

bool MainWindow::readStatuses(QStandardItemModel &model, QJsonArray &jsonArray)
{
    QList<QStandardItem*> *list;

    if (!jsonArray.empty())
    {
        QJsonArray::iterator i = jsonArray.begin();
        do
        {
            QJsonObject jsonObj = i->toObject();
            list = new QList<QStandardItem*>;
            *list << new QStandardItem(jsonObj["Name"].toString())\
                  << new QStandardItem(QString::number(jsonObj["Id"].toInt()))\
                  << new QStandardItem(jsonObj["Color"].toString())\
                  << new QStandardItem(jsonObj["Terms"].toString())\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Contains"],"|"))\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Actions"],"|"))\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Roles"],"|"));
            model.appendRow(*list);
            i++;
        }
        while ( i != jsonArray.end());
    }
    return 1;
}

QString MainWindow::jsonArrayJoin(QJsonValue value, const QString)
{
    if(value.isArray())
    {
        QString out = "";
        QVariantList array = value.toArray().toVariantList();
        if (!array.empty())
        {
            QVariantList::iterator i = array.begin();
            do
            {
                if (out != "")
                    out += "|";
                out += i->toString();
                i++;
            }
            while ( i != array.end());
        }
        return out;
    }
    return "not an array";
}

// При смене склада обновляем таблицу
void MainWindow::on_comboBoxSourceWarehouse_currentIndexChanged(int cur_src_index)
{
	uint8_t max_dest_index = ui->comboBoxDestWarehouse->model()->rowCount(); // Кол-во строк combobox склада-приёмника [0..x]
	uint8_t cur_dest_index = ui->comboBoxDestWarehouse->currentIndex();	// текущий индекс эл-та combobox склада-приёмника
	uint8_t new_dest_index = cur_dest_index;	// новый индекс эл-та combobox склада-приёмника
	if (cur_dest_index == cur_src_index) // Если индекс элемента combobox склада-приёмника равен индексу эл-та combobox склада-источника
	{
		if (new_dest_index==max_dest_index-1)	// Если после увеличения индекса окажется, что он превышает кол-во элементов combobox'а склада-приёмника
			new_dest_index--;	// то уменьшаем индекс элемента combobox склада-приёмника
		else
			new_dest_index++;	// иначе увеличиваем индекс элемента combobox склада-приёмника
	}
	ui->comboBoxDestWarehouse->setCurrentIndex(new_dest_index);	// переключаем combobox
	for (uint8_t i=0;i<max_dest_index;i++)	// в цикле включаем все элементы combobox'а
		if(comboboxDestModel->index(i,0).isValid()) // действие будет выполняться только для действительных элементов
			comboboxDestModel->item(i)->setEnabled(1);
	if(comboboxDestModel->index(cur_src_index,0).isValid())	// действие будет выполняться только для действительных элементов
		comboboxDestModel->item(cur_src_index)->setEnabled(0); // отключаем пункт списка (будет отображаться, но его нельзя будет выбрать)

	readGoods(ui->treeView->currentIndex(), ui->comboBoxSourceWarehouse->itemData(cur_src_index, Qt::UserRole+1).toString());	// перечитываем таблицу товаров
 }

void MainWindow::on_tableGoods_activated(const QModelIndex &index)
{
	readConsignments(index, ui->comboBoxSourceWarehouse->currentData(Qt::UserRole+1).toString());
}

void MainWindow::on_tableGoods_clicked(const QModelIndex &index)
{
    readConsignments(index, ui->comboBoxSourceWarehouse->currentData(Qt::UserRole+1).toString());
}

void MainWindow::reactivateCallerTab(QWidget *caller)
{
    ui->tabWidget->setCurrentWidget(caller); // Переключаемся на вкладку caller
}

void MainWindow::readConsignments(const QModelIndex &index, const QString &warehouse_code)
{
    if (!(QSqlDatabase::database("connMain").isOpen()))
    {
        return;
    }

	ui->label->setText("Art: " + index.model()->index(index.row(),0).data().toString());

	tableConsignmentsModel->setRowCount(0); // Удаляем имеющиеся строки
    QSqlQuery* consignments = new QSqlQuery(QSqlDatabase::database("connMain"));
	QString query;
	QStandardItem* newCol;
//	"Артикул" << "Наименование" << "Примечание" << "Кол-во" << "Реализация" << "Резерв" << "Цена" << "Место"
	query = QString("SELECT `art`,`name`,`primech`,`kolv`-`sale`-`reserv`,`rent`,`reserv`,`price3`,`place`,`index_i` FROM goods_base WHERE `art` = %1 AND `kolv`-`sale`-`reserv`>0 AND `sklad` LIKE '%2' AND `status` = 1;").arg(index.model()->index(index.row(),0).data().toString(),warehouse_code);
	consignments->exec(query);

	while(consignments->next())
	{
		QList<QStandardItem*> newRow;

		for(uint8_t i=0;i<8;i++)
		{
			newCol = new QStandardItem(consignments->value(i).toString()); // Артикул, Наименование, Примечание, Кол-во доступно, Кол-во на реализации, Кол-во в резерве, Цена и Место хранения.
			newRow.append(newCol);
		}

		tableConsignmentsModel->appendRow(newRow);
	}

	delete consignments;
//	ui->comboBoxSourceWarehouse->currentData(Qt::UserRole+1).toString();	// source warehouse selected in comboboxSourceWarehouse
//	ui->treeView->model()->data(index, Qt::UserRole+1).toString();	// index data
}


void MainWindow::createTabRepairs(int type)
{
    const QString tabLabels[] = {"Ремонты", "Выбрать ремонт"};
    tabRepairs *subwindow = tabRepairs::getInstance(type, this);
    if (ui->tabWidget->indexOf(subwindow) == -1) // Если такой вкладки еще нет, то добавляем
        ui->tabWidget->addTab(subwindow, tabLabels[type]);

    if (type == 0)
    {
        //
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), this, SLOT(createTabRepair(int)));
    }
    else
    {
        // Сигнал экземпляра вкладки выбора предыдущего ремонта подключаем к слоту вкладки нового ремонта для вставки номера в соотв. поле
        // и к слоту MainWindow, в котором происходит переключение на вкладку приёма в ремонт при закрытии вкладки выбора ремонта
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), tabRepairNew::getInstance(), SLOT(fillDeviceCreds(int)));
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), this, SLOT(reactivateTabRepairNew(int)));
        QObject::connect(subwindow,SIGNAL(buttonRepairNewClicked()), this, SLOT(createTabRepairNew()));
    }

    ui->tabWidget->setCurrentWidget(subwindow); // Переключаемся на вкладку Ремонты/Выбрать ремонт
}

void MainWindow::createTabRepair(int repair_id)
{
    tabRepair *subwindow = tabRepair::getInstance(repair_id, this);
    if(ui->tabWidget->indexOf(subwindow) == -1)
        ui->tabWidget->addTab(subwindow, "Ремонт " + QString::number(repair_id));
    ui->tabWidget->setCurrentWidget(subwindow);
}

void MainWindow::createTabRepairNew()
{
    tabRepairNew *subwindow = tabRepairNew::getInstance(this);
    if (ui->tabWidget->indexOf(subwindow) == -1) // Если такой вкладки еще нет, то добавляем
        ui->tabWidget->addTab(subwindow, "Приём в ремонт");
    ui->tabWidget->setCurrentWidget(subwindow); // Переключаемся на вкладку Приём в ремонт
    QObject::connect(subwindow,SIGNAL(createTabSelectPrevRepair(int)), this, SLOT(createTabRepairs(int)));
    QObject::connect(subwindow,SIGNAL(createTabSelectExistingClient(int, QWidget*)), this, SLOT(createTabClients(int, QWidget*)));
    QObject::connect(subwindow,SIGNAL(createTabClient(int)), this, SLOT(createTabClient(int)));
    QObject::connect(subwindow,SIGNAL(generatePrintout(QMap<QString, QVariant>)), this, SLOT(createTabPrint(QMap<QString, QVariant>)));
}

void MainWindow::createTabPrint(QMap<QString, QVariant> report_vars)
{
    report_vars.detach();
    qDebug() << "MainWindow::createTabPrint()";
    qDebug() << report_vars;
    tabPrintDialog *printDialog;

    printDialog = new tabPrintDialog(this, report_vars);
    ui->tabWidget->addTab(printDialog, "Print");
    ui->tabWidget->setCurrentWidget(printDialog);
}

void MainWindow::reactivateTabRepairNew(int)
{
    ui->tabWidget->setCurrentWidget(tabRepairNew::getInstance());
}

void MainWindow::createTabSale(int doc_id)
{
    tabSale *subwindow = tabSale::getInstance(doc_id, this);
    if(ui->tabWidget->indexOf(subwindow) == -1)
        if(doc_id)
            ui->tabWidget->addTab(subwindow, "Расходная накладная " + QString::number(doc_id));
        else
            ui->tabWidget->addTab(subwindow, "Продажа");
    ui->tabWidget->setCurrentWidget(subwindow);
    QObject::connect(subwindow,SIGNAL(createTabSelectExistingClient(int, QWidget *)), this, SLOT(createTabClients(int, QWidget *)));
    QObject::connect(subwindow,SIGNAL(createTabClient(int)), this, SLOT(createTabClient(int)));
    QObject::connect(subwindow,SIGNAL(generatePrintout(QMap<QString, QVariant>)), this, SLOT(createTabPrint(QMap<QString, QVariant>)));
    // TODO: QObject::connect(subwindow,SIGNAL(createTabClient(int)), this, SLOT(createTabClient(int)));
}

/*  Создание вкладки Клиенты (type = 0) или Выбрать клиента (type = 1)
 *  caller - указатель на вызывающую вкладку-виджет, необходимый для правильной передачи id выбранного клиента
 *  (данный слот может быть вызван с вкладок Приём в ремонт и Продать, а в будущем и других)
*/
void MainWindow::createTabClients(int type, QWidget *caller)
{
    const QString tabLabels[] = {"Клиенты", "Выбрать клиента"};
    tabClients *subwindow = tabClients::getInstance(type, this);
    if (ui->tabWidget->indexOf(subwindow) == -1) // Если такой вкладки еще нет
    {
        if(caller)  // если передан параметр, то вставляем её сразу после вызывающей
            ui->tabWidget->insertTab(ui->tabWidget->indexOf(caller) + 1, subwindow, tabLabels[type]);
        else    // иначе добавляем в конец
            ui->tabWidget->addTab(subwindow, tabLabels[type]);
    }

    if (type == 0)
    {
        //
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), this, SLOT(createTabClient(int)));
    }
    else
    {
        // Сигнал вкладки выбора клиента подключаем к слоту вызывающей вкладки для заполнения соотв. полей
        // и к слоту MainWindow, в котором происходит переключение на вызывающую вкладку после выбора клиента
        // TODO: нужно придумать более элегантный способ подключения сигнала doubleClicked(int) к слоту вызывающей вкладки
        if ( QString(caller->metaObject()->className()).compare("tabRepairNew", Qt::CaseSensitive) == 0 )
            QObject::connect(subwindow,SIGNAL(doubleClicked(int)), tabRepairNew::getInstance(), SLOT(fillClientCreds(int)));
        else if ( QString(caller->metaObject()->className()).compare("tabSale", Qt::CaseSensitive) == 0 )
            QObject::connect(subwindow,SIGNAL(doubleClicked(int)), tabSale::getInstance(0), SLOT(fillClientCreds(int)));

        // Чтобы после выбора клиента произошло автоматическое переключение на вызывающую вкладку, требуются доп. "телодвижения"
        subwindow->setCallerPtr(caller);
        QObject::connect(subwindow,SIGNAL(activateCaller(QWidget *)), this, SLOT(reactivateCallerTab(QWidget *)));
    }

    ui->tabWidget->setCurrentWidget(subwindow); // Переключаемся на вкладку Выбрать клиента
    subwindow->lineEditSearchSetFocus();

}

void MainWindow::createTabClient(int id)
{
    qDebug() << "Сейчас должна была бы открыться карточка клиента" << id;
}

void MainWindow::closeTab(int index)
{
    QWidget *w = ui->tabWidget->widget(index);
    if (QString(w->metaObject()->className()).compare("QWidget", Qt::CaseSensitive) != 0)  // Временное: на момент первоначальной разработки могут использоваться обычные QWidget-вкладки и при их закрытии приложение будет падать
    {
        tabCommon &tab = *(static_cast<tabCommon*>(w)); // мои классы ОБЯЗАТЕЛЬНО должны наследовать tabCommon
        if (tab.tabCloseRequest())   // Перед закрытием  вкладки нужно проверить нет ли несохранённых данных
        {
            delete w;
//            ui->tabWidget->removeTab(index);
            return;
        }
    }
    delete w;
}

#ifdef QT_DEBUG
void MainWindow::test_scheduler_handler()  // обработик таймера открытия вкладки
{
    qDebug() << "test_scheduler_handler(), test_scheduler_counter = " << test_scheduler_counter++;
//    createTabClients(0);
//    QSqlQuery rand_rep_id = QSqlQuery(QSqlDatabase::database("connMain"));
//    rand_rep_id.exec("SELECT ROUND(RAND()*25000, 0) INTO @id;");
//    rand_rep_id.exec("SELECT `id` FROM `workshop` WHERE `id` >= @id AND `hidden` = 0 LIMIT 1;");
//    qDebug() << rand_rep_id.lastError().databaseText();
//    if (rand_rep_id.next())
//    {
//        qDebug() << rand_rep_id.value(0);
//        createTabRepair(rand_rep_id.value(0).toInt());
//    }
    createTabRepair(24972);
//    createTabSale(16316);
//    createTabSale(0);
//    if (test_scheduler_counter < 375)
//    {
//        createTabRepairNew();
//        QMap<QString, QVariant> report_vars;
//        report_vars.insert("repair_id", 25037);
//        report_vars.insert("type", "new_rep");
//        createTabPrint(report_vars);
//    }
//    test_scheduler2->start(1000);    //  (пере-)запускаем таймер закрытия вкладки

}

void MainWindow::test_scheduler2_handler()  // обработик таймера закрытия вкладки
{
//    qDebug() << "MainWindow::test_scheduler2_handler(), clientTabId = " << ui->tabWidget->indexOf(tabRepairNew::getInstance(this));
    closeTab(ui->tabWidget->indexOf(tabRepairNew::getInstance(0)));
    test_scheduler->start(500);    //  перезапускаем таймер открытия вкладки
}
#endif

