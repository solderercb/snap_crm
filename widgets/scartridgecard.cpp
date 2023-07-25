#include "scartridgecard.h"
#include "ui_scartridgecard.h"

SCartridgeCard::SCartridgeCard(Qt::WindowFlags flags, QWidget *parent) :
    SWidget(parent, flags),
    ui(new Ui::SCartridgeCard)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);

    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &SCartridgeCard::closeForm);
    show();
}

SCartridgeCard::SCartridgeCard(const int id, Qt::WindowFlags flags, QWidget *parent) :
    SCartridgeCard(flags, parent)
{
    m_id = id;
    load(m_id);
}

SCartridgeCard::~SCartridgeCard()
{
    delete ui;
    if(m_vendorsModel)
        delete m_vendorsModel;
    if(m_cardModel)
        delete m_cardModel;
    if(m_materialsModel)
        delete m_materialsModel;
    if(m_photo)
        delete m_photo;
    if(m_cartridgeColors)
        delete m_cartridgeColors;
}

void SCartridgeCard::load(const int id)
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connMain"));

    m_id = id;
    query->exec(QString("SELECT `name`, `maker`, `full_weight`, `toner_weight`, `resource`, `created`, `user`, `notes`, `photo`, `color`, `archive` FROM `cartridge_cards` WHERE `id` = %1;").arg(m_id));
    if(query->first())
    {
        m_deviceMaker = query->value("maker").toInt();
        m_name = query->value("name").toString();
        m_notes = query->value("notes").toString();
        m_fullWeight = query->value("full_weight").toInt();
        m_tonerWeight = query->value("toner_weight").toInt();
        m_resource = query->value("resource").toInt();
        m_color = query->value("color").toInt();
        m_photo = new QByteArray(query->value("photo").toByteArray());
        m_isArchive = query->value("archive").toBool();
    }

    delete query;

    initWidgets();
    updateWidgets();
}

void SCartridgeCard::initModels()
{
    QString query;
    QSqlQuery q(QSqlDatabase::database("connMain"));
    m_materialsFilter.op = FilterList::And;
    m_materialsFilter.fields.append(STableViewBase::initFilterField("card_id", FilterField::Equals, m_id));

    query = QUERY_SEL_CARTRIDGE_CAT_ID;
    q.exec(query);
    if(!q.first())
        return;
    m_deviceClassId = q.value(0).toInt();
    query = QUERY_SEL_DEVICE_MAKERS(q.value(1).toString());
    m_vendorsModel = new SSqlQueryModel();
    m_vendorsModel->setQuery(query, QSqlDatabase::database("connMain"));

    m_cardModel = new SCartridgeCardModel();
    m_cardModel->load(m_id);
    m_materialsModel = new SCartridgeMaterialsModel();
    m_materialsModel->setCardId(m_id);
    m_materialsModel->setSelectStatement(QUERY_SEL_CARTRIDGE_MATERIALS);
    m_materialsModel->setFilter(QString("`card_id` = %1").arg(m_id));
    m_materialsModel->select();
    connect(m_materialsModel, &SCartridgeMaterialsModel::noFurtherMaterialAddition, ui->pushButtonAddMaterial, &QPushButton::setDisabled);
    if(m_id)
    {
        m_materialsModel->setCardId(m_id);
    }
    m_cartridgeColors = colorsList();
}

void SCartridgeCard::initWidgets()
{
    initModels();
    ui->comboBoxVendor->setModel(m_vendorsModel);
    ui->comboBoxVendor->setCurrentIndex(-1);
    ui->tableViewMaterials->setModel(m_materialsModel);
    m_materialsModel->findNextMaterial();   // если в карточке уже заданы все расходные материалы, то кнопка "Добавить" будет отключена
    ui->spinBoxResource->setMinimum(1);
    ui->spinBoxResource->setMaximum(30);
    ui->spinBoxFullWeight->setMinimum(100);
    ui->spinBoxFullWeight->setMaximum(1000);
    ui->spinBoxTonerWeight->setMinimum(10);
    ui->spinBoxTonerWeight->setMaximum(500);
    ui->comboBoxColor->setModel(m_cartridgeColors);
    ui->comboBoxColor->setCurrentIndex(-1);

    connect(ui->tableViewMaterials, &materialsTable::clicked, this, &SCartridgeCard::materialSelected);
    connect(ui->tableViewMaterials, &materialsTable::tonerWeightChanged, this, &SCartridgeCard::setTonerWeight);
    connect(ui->pushButtonAddMaterial, &QPushButton::clicked, m_materialsModel, &SCartridgeMaterialsModel::appendRow);
    connect(ui->pushButtonRemoveMaterial, &QPushButton::clicked, this, &SCartridgeCard::removeMaterial);
    connect(ui->spinBoxTonerWeight, qOverload<int>(&QSpinBox::valueChanged), m_materialsModel, &SCartridgeMaterialsModel::setDefaultTonerWeight);
    connect(ui->pushButtonCreateSave, &QPushButton::clicked, this, &SCartridgeCard::commit);
}

void SCartridgeCard::updateWidgets()
{
    if(m_id)
    {
        ui->pushButtonCreateSave->setText(tr("Сохранить"));
        ui->checkBoxArchive->setVisible(true);
        ui->checkBoxArchive->setChecked(m_isArchive);
        ui->comboBoxVendor->setCurrentIndex(m_vendorsModel->rowByDatabaseID(m_deviceMaker));
        ui->lineEditName->setText(m_name);
        ui->spinBoxResource->setValue(m_resource);
        ui->spinBoxFullWeight->setValue(m_fullWeight);
        ui->spinBoxTonerWeight->setValue(m_tonerWeight);
        ui->plainTextEditNotes->setPlainText(m_notes);
        ui->comboBoxColor->setCurrentIndex(m_color);
        ui->tableViewMaterials->refresh();
        ui->pushButtonRemoveMaterial->setEnabled(false);
    }
    else
    {
        ui->pushButtonCreateSave->setText(tr("Создать"));
        ui->checkBoxArchive->setVisible(false);
    }
}

SStandardItemModel *SCartridgeCard::colorsList()
{
    SStandardItemModel *list = nullptr;
    QMetaEnum colors = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Color"));
    list = SStandardItemModel::modelFromEnum(colors);
    list->setObjectName("cartridgeColorsList");

    return list;
}

void SCartridgeCard::setModelData()
{
    m_cardModel->setArchive(ui->checkBoxArchive->isChecked());
    m_cardModel->setColor(m_cartridgeColors->databaseIDByRow(ui->comboBoxColor->currentIndex()));
    m_cardModel->setFullWeight(ui->spinBoxFullWeight->value());
    m_cardModel->setTonerWeight(ui->spinBoxTonerWeight->value());
    m_cardModel->setVendor(m_vendorsModel->databaseIDByRow(ui->comboBoxVendor->currentIndex()));
    m_cardModel->setName(ui->lineEditName->text());
    m_cardModel->setNotes(ui->plainTextEditNotes->toPlainText());
//    m_cardModel->setPhoto()   // TODO:
    m_cardModel->setResource(ui->spinBoxResource->value());
}

void SCartridgeCard::materialSelected(const QModelIndex &index)
{
    ui->pushButtonRemoveMaterial->setEnabled(index.isValid());
}

void SCartridgeCard::removeMaterial()
{
    int row = ui->tableViewMaterials->currentIndex().row();

    m_materialsModel->removeRow(row);
    ui->tableViewMaterials->hideRow(row);
    ui->pushButtonRemoveMaterial->setEnabled(false);
    ui->pushButtonAddMaterial->setEnabled(true);
    m_materialsModel->findNextMaterial();
}

void SCartridgeCard::setTonerWeight(const int weight)
{
    ui->spinBoxTonerWeight->blockSignals(true);
    ui->spinBoxTonerWeight->setValue(weight);
    ui->spinBoxTonerWeight->blockSignals(false);
}

bool SCartridgeCard::commit()
{
    QSqlQuery *query = new QSqlQuery(QSqlDatabase::database("connThird"));
    i_queryLog = new SQueryLog();
    bool nErr = 1;
    setModelData();

    QUERY_LOG_START(metaObject()->className());
    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        m_cardModel->commit();
        m_materialsModel->commit();
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Успешно"), tr("Данные сохранены"), QColor(214,239,220), QColor(229,245,234));

#ifdef QT_DEBUG
//        throw Global::ThrowType::Debug; // это для отладки (чтобы сессия всегда завершалась ROLLBACK'OM)
#endif
        QUERY_COMMIT_ROLLBACK(query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if(type == Global::ThrowType::Debug)
        {
            QString err = "DEBUG ROLLBACK";
            QUERY_ROLLBACK_MSG(query, err);
        }
        else if (type == Global::ThrowType::QueryError)
        {
            QUERY_COMMIT_ROLLBACK_MSG(query, nErr);
        }
        else
            QUERY_COMMIT_ROLLBACK(query, nErr);
    }
    QUERY_LOG_STOP;
    delete query;

    return nErr;
}

void SCartridgeCard::translateNames()
{
    tr("Black");
    tr("Cyan");
    tr("Magenta");
    tr("Yellow");
}

// ===============================================================================================================
materialsTable::materialsTable(QWidget *parent) : STableViewBase(parent)
{
    QMetaEnum headers = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Column"));
    QMetaEnum width = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("ColumnWidth"));
    for(int i = 0; i < headers.keyCount(); i++)
    {
        i_defaultHeaderLabels << tr(headers.key(i));
        i_defaultColumnsWidths.insert(headers.value(i), width.value(i));
    }
    readLayout(SLocalSettings::CartridgeMaterialsGrid);
    i_gridLayout->$GridControl.Columns[Column::Name].Width_marked = true;  // по умолчанию автоширина столбца с наименованием

}

materialsTable::~materialsTable()
{
    saveLayout(SLocalSettings::CartridgeMaterialsGrid);
}

void materialsTable::setModel(QAbstractItemModel *model)
{
    m_model = static_cast<SCartridgeMaterialsModel*>(model);
    STableViewBase::setModel(model);
    SCartridgeMaterialsTableItemDelegates *itemDelagates = new SCartridgeMaterialsTableItemDelegates(m_model, this);
    setItemDelegate(itemDelagates);
}

void materialsTable::mouseClickEvent(QMouseEvent *event)
{
    int row = currentIndex().row();
    int column = currentIndex().column();
    switch (column)
    {
        // TODO: implement me!
        default: ;
    }
}

void materialsTable::translateNames()
{
    tr("Type");
    tr("Count");
    tr("Price");
    tr("PriceWork");
    tr("Summ");
    tr("Name");
    tr("Articul");
}

#if QT_VERSION >= 0x060000
void materialsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
#else
void materialsTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
#endif
{
    STableViewBase::dataChanged(topLeft, bottomRight, roles);
    if(!roles.isEmpty() && !roles.contains(Qt::DisplayRole))
        return;

    for(int row = topLeft.row(); row <= bottomRight.row(); row++)
        for(int col = topLeft.column(); col <= bottomRight.column(); col++)
        {
            switch(col)
            {
                case Column::Count: if(model()->index(row, 1).data(Qt::DisplayRole|0x100).toInt() == SCartridgeMaterialModel::Toner) {emit tonerWeightChanged(model()->index(row, col).data(Qt::DisplayRole|0x100).toInt());} break;
                default: /*resizeColumnToContents(col)*/;
            }
        }
}
