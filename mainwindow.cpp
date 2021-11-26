#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bottoolbarwidget.h"
#include "tabrepairnew.h"

#define NO_LOGIN

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
    this->resize(960,640);
    ui->tab_store_items->showMaximized();

    /* Кнопка Ремонты и меню */
    QMenu *workshop_menu = new QMenu();
    QAction *workshop_new = new QAction("Принять", this);
    workshop_menu->addAction(workshop_new);
    QAction *workshop_refill = new QAction("Заправка", this);
    workshop_menu->addAction(workshop_refill);
    QAction *workshop_price = new QAction("Прайс-лист", this);
    workshop_menu->addAction(workshop_price);
    QAction *workshop_editor = new QAction("Групповой редактор ремонтов", this);
    workshop_menu->addAction(workshop_editor);
    QToolButton* workshop_button = new QToolButton();
    QObject::connect(workshop_button,SIGNAL(clicked()),this,SLOT(on_workshop_button_triggered()));
    workshop_button->setMenu(workshop_menu);
    workshop_button->setPopupMode(QToolButton::MenuButtonPopup);
    workshop_button->setText("Ремонты");
    workshop_button->setFixedSize(96,48);
    ui->toolBar->addWidget(workshop_button);

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

    ui->dockWidget_3->setTitleBarWidget(new QWidget());
//    QToolBar *tab_toolbar=new QToolBar("toolbar",ui->dockWidgetContents_2);
//    tab_toolbar->setMinimumSize(QSize(0, 48));
//    tab_toolbar->setIconSize(QSize(96, 48));
//    tab_toolbar->setAllowedAreas(Qt::BottomToolBarArea);
//    tab_toolbar->setMovable(1);
//    tab_toolbar->addAction("action1");
//    tab_toolbar->addAction("action2");

    ui->pushButton_9->setStyleSheet("::hover { border: 2px solid #8f8f91; border-radius: 6px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"); // LOL работает

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHeightForWidth(ui->tab_test->sizePolicy().hasHeightForWidth());
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    ui->tab_test->setSizePolicy(sizePolicy);
    QGridLayout* gridLayout_2 = new QGridLayout(ui->tab_test);
    gridLayout_2->setSpacing(1);
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->SetMaximumSize;

    QPushButton* pushButton05 = new QPushButton();
    pushButton05->setText("pushButton05");
    pushButton05->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    gridLayout_2->addWidget(pushButton05, 0, 0, 3, 1);
    QPushButton* pushButton04 = new QPushButton();
    pushButton04->setText("pushButton04");
    gridLayout_2->addWidget(pushButton04, 1, 2, 1, 1);
    QPushButton* pushButton03 = new QPushButton();
    pushButton03->setText("pushButton03");
    gridLayout_2->addWidget(pushButton03, 2, 2, 1, 1);
    QPushButton* pushButton02 = new QPushButton();
    pushButton02->setText("pushButton02");
    gridLayout_2->addWidget(pushButton02, 3, 3, 1, 1);
    QPushButton* pushButton01 = new QPushButton();
    pushButton01->setText("pushButton01");
    gridLayout_2->addWidget(pushButton01, 5, 3, 1, 1);
    BotToolbarWidget* BotToolbarWidget_ = new BotToolbarWidget(ui->tab_workshop);
    gridLayout_2->addWidget(BotToolbarWidget_, 6, 0, 1, 4, Qt::AlignBottom);

    tabRepairNew *subwindow = new tabRepairNew();
    subwindow->setObjectName(QString::fromUtf8("subwindow"));
    ui->mdiArea->addSubWindow(subwindow);

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
}

MainWindow::~MainWindow()
{
    dbConnection->destroy();
	delete tableConsignmentsModel;
	delete tableGoodsModel;
	delete tree_model;
	delete comboboxSourceModel;
	delete ui;
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


void MainWindow::on_workshop_button_triggered()
{
    // create new subwindow
//    QMdiSubWindow *subWindow1 = new QMdiSubWindow(ui->mdiArea);
    // create new widget for the mdi window ( the inner )
    QWidget *myWidget = new QWidget();
    ui->mdiArea->addSubWindow(myWidget);
//    QPushButton* pushButton = new QPushButton(myWidget);
//    subWindow1->show();
    myWidget->show();

}

