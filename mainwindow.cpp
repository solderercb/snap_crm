#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bottoolbarwidget.h"
#include "tabrepairnew.h"
#include "ui_tabrepairnew.h"
#include "tabrepairs.h"
#include "tabrepair.h"
#include "tabclients.h"
#include "mylineedit.h"

#define NO_LOGIN
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

// Тело конструктора MainWindow(QWidget *parent)
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
	QTextCodec::setCodecForLocale(codec);

#ifdef NO_LOGIN
    ConnectToDB(SQL_USER, SQL_PSWD, SQL_HOST, SQL_PORT, SQL_DB, "connMain");
#endif

    ui->setupUi(this);

    tabBarEventFilter *tabBarEventFilterObj = new tabBarEventFilter(this);  // Фильтр событий tabBar. В частности, закрытие вкладки по клику средней кнопкой мыши (колёсиком)
    ui->tabWidget->tabBar()->installEventFilter(tabBarEventFilterObj);

    this->resize(1440,800);

    /* Кнопка Ремонты и меню */
    QMenu *workshop_menu = new QMenu();
    QAction *workshop_new = new QAction("Принять", this);
    workshop_menu->addAction(workshop_new);
    QObject::connect(workshop_new,SIGNAL(triggered()),this,SLOT(createTabRepairNew()));
    QAction *workshop_refill = new QAction("Заправка", this);
    workshop_menu->addAction(workshop_refill);
    QAction *workshop_price = new QAction("Прайс-лист", this);
    workshop_menu->addAction(workshop_price);
    QAction *workshop_editor = new QAction("Групповой редактор ремонтов", this);
    workshop_menu->addAction(workshop_editor);
    QToolButton* workshop_button = new QToolButton();
//    QAction *workshop = new QAction("Ремонты", this);
//    workshop_button->addAction(workshop);
    workshop_button->setMenu(workshop_menu);
    workshop_button->setPopupMode(QToolButton::MenuButtonPopup);
    workshop_button->setText("Ремонты");
    workshop_button->setFixedSize(96,48);
    ui->toolBar->addWidget(workshop_button);
    QObject::connect(workshop_button, SIGNAL(clicked()), this, SLOT(createTabRepairs()));

    /* Кнопка Товары и меню */
    QMenu *goods_menu = new QMenu();
    QAction *goods_arrival = new QAction("Приход", this);
    goods_menu->addAction(goods_arrival);
    QAction *goods_sale = new QAction("Продажа", this);
    goods_menu->addAction(goods_sale);
    QAction *store_docs = new QAction("Документы", this);
    goods_menu->addAction(store_docs);
    QAction *purchase_manager = new QAction("Менеджер закупок", this);
    goods_menu->addAction(purchase_manager);
    QAction *store_manager = new QAction("Управление складом", this);
    goods_menu->addAction(store_manager);
    QAction *goods_editor = new QAction("Групповой редактор товаров", this);
    goods_menu->addAction(goods_editor);
    QAction *goods_uploader = new QAction("Выгрузка товаров", this);
    goods_menu->addAction(goods_uploader);
    QAction *device_buyout = new QAction("Выкуп техники", this);
    goods_menu->addAction(device_buyout);
    QAction *stocktaking = new QAction("Переучет", this);
    goods_menu->addAction(stocktaking);
    QToolButton* goods_button = new QToolButton();
    goods_button->setMenu(goods_menu);
    goods_button->setPopupMode(QToolButton::MenuButtonPopup);
    goods_button->setText("Товары");
    goods_button->setFixedSize(96,48);
    ui->toolBar->addWidget(goods_button);

    /* Кнопка Клиенты и меню */
    QMenu *clients_menu = new QMenu();
    QAction *client_new = new QAction("Новый клиент", this);
    clients_menu->addAction(client_new);
    QAction *clients_calls = new QAction("Вызовы", this);
    clients_menu->addAction(clients_calls);
    QAction *clients_sms = new QAction("SMS", this);
    clients_menu->addAction(clients_sms);
//    QAction *clients_editor = new QAction("Групповой редактор клиентов", this);
//    clients_menu->addAction(clients_editor);
    QToolButton* clients_button = new QToolButton();
    clients_button->setMenu(clients_menu);
    clients_button->setPopupMode(QToolButton::MenuButtonPopup);
    clients_button->setText("Клиенты");
    clients_button->setFixedSize(96,48);
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

    get_warehouses_list();
    btnClick();

#ifdef QT_DEBUG
    test_scheduler = new QTimer();
    test_scheduler->setSingleShot(true);
    test_scheduler2 = new QTimer();
    test_scheduler2->setSingleShot(true);
    QObject::connect(test_scheduler, SIGNAL(timeout()), this, SLOT(test_scheduler_handler()));
    QObject::connect(test_scheduler2, SIGNAL(timeout()), this, SLOT(test_scheduler2_handler()));
    test_scheduler->start(200);
#endif
}

MainWindow::~MainWindow()
{
    dbConnection->destroy();
	delete tableConsignmentsModel;
	delete tableGoodsModel;
	delete tree_model;
	delete comboboxSourceModel;
	delete ui;
    p_instance = nullptr;
}

MainWindow* MainWindow::getInstance(QWidget *parent)   // singleton: MainWindow только один объект
{
    if( !p_instance )
      p_instance = new MainWindow(parent);

    return p_instance;
}

void MainWindow::ConnectToDB(const QString &username, const QString &password, const QString &ipaddr, const uint &port, const QString &dbName, const QString &connName )
{
    dbConnection = DBConnection::getInstance(this, username, password, ipaddr, port, dbName, connName);
}

void MainWindow::btnClick()
{
    enum {store_cats_id, store_cats_parent, store_cats_name, store_cats_position};

    if (!(dbConnection->connectOK))
    {
        return;
    }

    QSqlQuery* cat_tree = new QSqlQuery(QSqlDatabase::database("connMain"));
    QStandardItem* parentItem;
    QStandardItem* childItem;
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
            childItem = store_cats[i];
            parent = store_cats[i]->data(Qt::UserRole+2).toInt();
            if(parent)  // если записан parent
            {
                j = 0;
                while (store_cats_ids[j] != parent)
                    j++;
                parentItem = store_cats[j];
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
    if (!(dbConnection->connectOK))
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
    if (!(dbConnection->connectOK))
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

	ui->comboBoxDestWarehouse->setCurrentIndex(1); // По умолчанию складом-приёмником является второй элемент в списке

	delete warehouse_list;
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

void MainWindow::on_tableConsignments_clicked(const QModelIndex &index)
{
	;
}

void MainWindow::readConsignments(const QModelIndex &index, const QString &warehouse_code)
{
    if (!(dbConnection->connectOK))
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
    tabRepairs *subwindow = tabRepairs::getInstance(type);
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
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), tabRepairNew::getInstance(), SLOT(setPrevRepair(int)));
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), this, SLOT(reactivateTabRepairNew(int)));
    }

    ui->tabWidget->setCurrentWidget(subwindow); // Переключаемся на вкладку Ремонты/Выбрать ремонт
}

void MainWindow::createTabRepair(int repair_id)
{
    tabRepair *subwindow = tabRepair::getInstance();
    ui->tabWidget->addTab(subwindow, "Ремонт " + QString::number(repair_id));
    ui->tabWidget->setCurrentWidget(subwindow);
}

void MainWindow::createTabRepairNew()
{
    tabRepairNew *subwindow = tabRepairNew::getInstance();
    if (ui->tabWidget->indexOf(subwindow) == -1) // Если такой вкладки еще нет, то добавляем
        ui->tabWidget->addTab(subwindow, "Приём в ремонт");
    ui->tabWidget->setCurrentWidget(subwindow); // Переключаемся на вкладку Приём в ремонт
    QObject::connect(subwindow,SIGNAL(createTabSelectPrevRepair(int)), this, SLOT(createTabRepairs(int)));
    QObject::connect(subwindow,SIGNAL(createTabSelectExistingClient(int)), this, SLOT(createTabClients(int)));
}

void MainWindow::reactivateTabRepairNew(int)
{
    ui->tabWidget->setCurrentWidget(tabRepairNew::getInstance());
}

void MainWindow::createTabClients(int type)
{
    const QString tabLabels[] = {"Клиенты", "Выбрать клиента"};
    tabClients *subwindow = tabClients::getInstance(type);
    if (ui->tabWidget->indexOf(subwindow) == -1) // Если такой вкладки еще нет, то добавляем
        ui->tabWidget->addTab(subwindow, tabLabels[type]);

    if (type == 0)
    {
        //
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), this, SLOT(createTabClient(int)));
    }
    else
    {
        // Сигнал экземпляра вкладки выбора клиента подключаем к слоту вкладки нового ремонта для заполнения соотв. полей
        // и к слоту MainWindow, в котором происходит переключение на вкладку приёма в ремонт при закрытии вкладки выбора ремонта
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), tabRepairNew::getInstance(), SLOT(fillClientCreds(int)));
        QObject::connect(subwindow,SIGNAL(doubleClicked(int)), this, SLOT(reactivateTabRepairNew(int)));
    }

    ui->tabWidget->setCurrentWidget(subwindow); // Переключаемся на вкладку Ремонты/Выбрать ремонт
    subwindow->lineEditSearchSetFocus();

}

void MainWindow::createTabClient(int)
{

}

void MainWindow::closeTab(int index)
{
    delete ui->tabWidget->widget(index);
}

#ifdef QT_DEBUG
void MainWindow::test_scheduler_handler()  // обработик таймера открытия вкладки
{
//    qDebug() << "test_scheduler_handler(), test_scheduler_counter = " << test_scheduler_counter++;
//    createTabClients(0);
//    test_scheduler2->start(1000);    //  (пере-)запускаем таймер закрытия вкладки

}

void MainWindow::test_scheduler2_handler()  // обработик таймера закрытия вкладки
{
//    qDebug() << "test_scheduler2_handler(), clientTabId = " << ui->tabWidget->indexOf(tabClients::getInstance(0));
//    closeTab(ui->tabWidget->indexOf(tabClients::getInstance(0)));
//    test_scheduler->start(1000);    //  перезапускаем таймер открытия вкладки
}
#endif

