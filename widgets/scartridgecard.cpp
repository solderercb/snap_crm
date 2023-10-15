#include "scartridgecard.h"
#include "ui_scartridgecard.h"

SCartridgeCard::SCartridgeCard(Qt::WindowFlags flags, QWidget *parent) :
    SModalWidget(parent, flags),
    ui(new Ui::SCartridgeCard)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    initWidgets();

    show();
}

SCartridgeCard::SCartridgeCard(const int id, const int vendorIndex, Qt::WindowFlags flags, QWidget *parent) :
    SCartridgeCard(flags, parent)
{
    if(id)
    {
        load(id);
    }
    else
    {
        m_vendorIndex = vendorIndex;
    }

    updateWidgets();
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
    m_id = id;

    m_cardModel->load(m_id);
    m_materialsModel->setCardId(m_id);
    m_materialsModel->select();
}

void SCartridgeCard::initModels()
{
    QString query;
    QSqlQuery q(QSqlDatabase::database("connMain"));

    query = QUERY_SEL_CARTRIDGE_CAT_ID;
    q.exec(query);
    if(!q.first())
        return;
    m_deviceClassId = q.value(0).toInt();
    query = QUERY_SEL_DEVICE_MAKERS(q.value(1).toString());
    m_vendorsModel = new SSqlQueryModel();
    m_vendorsModel->setQuery(query, QSqlDatabase::database("connMain"));

    m_cardModel = new SCartridgeCardModel();
    m_materialsModel = new SCartridgeMaterialsModel();
    connect(m_materialsModel, &SCartridgeMaterialsModel::noFurtherMaterialAddition, ui->pushButtonAddMaterial, &QPushButton::setDisabled);
    m_cartridgeColors = colorsList();
}

void SCartridgeCard::initWidgets()
{
    initModels();
    ui->comboBoxVendor->setModel(m_vendorsModel);
    ui->comboBoxVendor->setCurrentIndex(-1);
    ui->tableViewMaterials->setModel(m_materialsModel);
    m_materialsModel->findNextMaterial();   // если в карточке уже заданы все расходные материалы, то кнопка "Добавить" будет отключена
    ui->spinBoxResource->setMinimum(0);
    ui->spinBoxResource->setMaximum(30);    // см. описание метода SCartridgeCardModel::resource()
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
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &SCartridgeCard::closeForm);
}

void SCartridgeCard::updateWidgets()
{
    if(m_id)
    {
        ui->pushButtonCreateSave->setText(tr("Сохранить"));
        ui->checkBoxArchive->setVisible(true);
        ui->checkBoxArchive->setChecked(m_isArchive);
        ui->comboBoxVendor->setCurrentIndex(m_vendorsModel->rowByDatabaseID(m_cardModel->vendor()));
        ui->lineEditName->setText(m_cardModel->name());
        ui->spinBoxResource->setValue(m_cardModel->resource());
        ui->spinBoxFullWeight->setValue(m_cardModel->fullWeight());
        ui->spinBoxTonerWeight->setValue(m_cardModel->tonerWeight());
        ui->plainTextEditNotes->setPlainText(m_cardModel->notes());
        ui->comboBoxColor->setCurrentIndex(m_cardModel->color());
        ui->tableViewMaterials->refresh();
        ui->pushButtonRemoveMaterial->setEnabled(false);
    }
    else
    {
        ui->comboBoxVendor->setCurrentIndex(m_vendorIndex);
        ui->spinBoxResource->setValue(5);
        ui->pushButtonCreateSave->setText(tr("Создать"));
        ui->checkBoxArchive->setVisible(false);
    }
    if(m_vendorIndex >= 0)
        ui->lineEditName->setFocus();
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
    bool nErr = 1;
    bool isNew = (m_cardModel->id() == 0);
    setModelData();

    QUERY_LOG_START(metaObject()->className());
    try
    {
        QUERY_EXEC(query,nErr)(QUERY_BEGIN);
        m_cardModel->commit();
        if(isNew)
            m_materialsModel->setCardId(m_cardModel->id());
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
    if(!m_id)
    {
        m_id = m_cardModel->id();
        emit newCardCreated(m_id);
    }
    else
    {
        emit cardModified(m_id);
    }
    QUERY_LOG_STOP;
    delete query;

    updateWidgets();
    return nErr;
}

void SCartridgeCard::closeForm()
{
    this->deleteLater();
}

void SCartridgeCard::translateNames()
{
    tr("Black");
    tr("Cyan");
    tr("Magenta");
    tr("Yellow");
}

// ===============================================================================================================
materialsTable::materialsTable(QWidget *parent) :
    STableViewBase(SLocalSettings::CartridgeMaterialsGrid, parent)
{
    QMetaEnum headers = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("Column"));
    QMetaEnum width = staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("ColumnWidth"));
    for(int i = 0; i < headers.keyCount(); i++)
    {
        i_defaultHeaderLabels << tr(headers.key(i));
        i_defaultColumnsWidths.insert(headers.value(i), width.value(i));
    }
    readLayout();
    i_gridLayout->$GridControl.Columns[Column::Name].Width_marked = true;  // по умолчанию автоширина столбца с наименованием

}

materialsTable::~materialsTable()
{
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
    Q_UNUSED(event);
/*    int row = currentIndex().row();
    int column = currentIndex().column();
    switch (column)
    {
        // TODO: implement me!
        default: ;
    }
*/
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
