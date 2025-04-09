#include "sapplication.h"
#include "scartridgecard.h"
#include "ui_scartridgecard.h"

SCartridgeCard::SCartridgeCard(Qt::WindowFlags flags, QWidget *parent) :
    SModalWidget(parent, flags),
    ui(new Ui::SCartridgeCard)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);

    initModels();
    configureWidgets();
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
    if(m_photo)
        delete m_photo;
}

void SCartridgeCard::load(const int id)
{
    m_id = id;

    m_cardModel->load(m_id);
    setMaterialsTableFilter(m_id);
}

void SCartridgeCard::setMaterialsTableFilter(const int id)
{
    m_materialsModel->setCardId(id);

    FilterList list;
    list.op = FilterList::Op::And;

    list.fields.append(STableViewBase::initFilterField("`card_id`", FilterField::Equals, id));
    ui->tableViewMaterials->setFilter(list);
}

void SCartridgeCard::initModels()
{
    bool nErr = 1;
    QString query;
    QSqlQuery q(QSqlDatabase::database("connMain"));

    query = QUERY_SEL_CARTRIDGE_CAT_ID;
    QUERY_EXEC_TH(&q,nErr,query);

    if(!q.first())
        Global::throwError(Global::ThrowType::ResultError, errQuerySelCartridgeCatId);

    m_vendorsModel = new SSqlQueryModel(this);
    m_deviceClassId = q.value(0).toInt();
    query = QUERY_SEL_DEVICE_MAKERS(q.value(1).toString());
    m_vendorsModel->setQuery(query, QSqlDatabase::database("connMain"));

    if(m_vendorsModel->lastError().isValid())
        Global::errorMsg(m_vendorsModel->lastError(), errQuerySelDeviceMakers);
    else if(m_vendorsModel->rowCount() == 0)
        Global::errorMsg(Global::ThrowType::ResultError, errQuerySelDeviceMakers + "; " + tr("список id: \'%1\'").arg(q.value(1).toString()));

    m_cardModel = new SCartridgeCardModel(this);
    m_materialsModel = new SCartridgeMaterialsModel(this);
    m_cartridgeColors = colorsList();
    m_cartridgeColors->setParent(this);

    connect(m_materialsModel, &SCartridgeMaterialsModel::noFurtherMaterialAddition, ui->pushButtonAddMaterial, &QPushButton::setDisabled);
}

void SCartridgeCard::configureWidgets()
{
    m_modelRW = permissions->editCartridgeCards;
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
    ui->tableViewMaterials->setQuery(QUERY_SEL_CARTRIDGE_MATERIALS, QSqlDatabase::database("connMain"));

    connect(ui->tableViewMaterials, &materialsTable::clicked, this, &SCartridgeCard::materialSelected);
    connect(ui->tableViewMaterials, &materialsTable::tonerWeightChanged, this, &SCartridgeCard::setTonerWeight);
    connect(ui->pushButtonAddMaterial, &QPushButton::clicked, m_materialsModel, &SCartridgeMaterialsModel::appendRow);
    connect(ui->pushButtonRemoveMaterial, &QPushButton::clicked, this, &SCartridgeCard::removeMaterial);
    connect(ui->spinBoxTonerWeight, qOverload<int>(&QSpinBox::valueChanged), m_materialsModel, &SCartridgeMaterialsModel::setDefaultTonerWeight);
    connect(ui->pushButtonCreateSave, &QPushButton::clicked, this, &SCartridgeCard::commit);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &SCartridgeCard::closeForm);
    connect(ui->tableViewMaterials->horizontalHeader(), &QHeaderView::sectionClicked, this, &SCartridgeCard::sortMaterials);
}

void SCartridgeCard::updateWidgets()
{
    if(m_id)
    {
        ui->pushButtonCreateSave->setText(tr("Сохранить"));
        ui->pushButtonCreateSave->setEnabled(m_modelRW);
        ui->pushButtonAddMaterial->setEnabled(m_modelRW);
        ui->checkBoxArchive->setVisible(true);
        ui->checkBoxArchive->setChecked(m_isArchive);
        ui->checkBoxArchive->setEnabled(m_modelRW);
        ui->comboBoxVendor->setCurrentIndex(m_vendorsModel->rowByDatabaseID(m_cardModel->vendor()));
        ui->comboBoxVendor->setEnabled(m_modelRW);
        ui->lineEditName->setText(m_cardModel->name());
        ui->lineEditName->setReadOnly(!m_modelRW);
        ui->spinBoxResource->setValue(m_cardModel->resource());
        ui->spinBoxResource->setReadOnly(!m_modelRW);
        ui->spinBoxFullWeight->setValue(m_cardModel->fullWeight());
        ui->spinBoxFullWeight->setReadOnly(!m_modelRW);
        ui->spinBoxTonerWeight->setValue(m_cardModel->tonerWeight());
        ui->spinBoxTonerWeight->setReadOnly(!m_modelRW);
        ui->plainTextEditNotes->setPlainText(m_cardModel->notes());
        ui->plainTextEditNotes->setReadOnly(!m_modelRW);
        ui->comboBoxColor->setCurrentIndex(m_cardModel->color());
        ui->comboBoxColor->setEnabled(m_modelRW);
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

bool SCartridgeCard::checkInput()
{
    try
    {
        for(int i = 0; i < m_materialsModel->rowCount(); i++)
        {
            if(!m_materialsModel->index(i, materialsTable::Column::Count).data().isValid()) throw 1;
            if(!m_materialsModel->index(i, materialsTable::Column::Price).data().isValid()) throw 2;
            if(!m_materialsModel->index(i, materialsTable::Column::PriceWork).data().isValid()) throw 3;
            if(m_materialsModel->index(i, materialsTable::Column::Name).data().toString().isEmpty()) throw 4;
        }

        if(m_materialsModel->rowCount() == 0)
            throw 5;

        if(ui->lineEditName->text().isEmpty())
            throw 6;
    }
    catch (int err)
    {
        QString errMsg;
        const QString notSet = tr("Не указано: ");
        switch (err)
        {
            case 1: errMsg = QString("%1 %2").arg(notSet, materialsTable::tr("Count")); break;
            case 2: errMsg = QString("%1 %2").arg(notSet, materialsTable::tr("Price")); break;
            case 3: errMsg = QString("%1 %2").arg(notSet, materialsTable::tr("PriceWork")); break;
            case 4: errMsg = QString("%1 %2").arg(notSet, materialsTable::tr("Name")); break;
            case 5: errMsg = tr("Таблица материалов пуста"); break;
            case 6: errMsg = tr("Не указана модель картриджа"); break;
            default: errMsg = "";
        }
        shortlivedNotification *newPopup = new shortlivedNotification(this,
                                                                      tr("Ошибка"),
                                                                      errMsg,
                                                                      QColor(255,164,119),
                                                                      QColor(255,199,173));
        return 0;
    }

    return 1;
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
    if(m_modelRW)
        ui->pushButtonRemoveMaterial->setEnabled(index.isValid());
}

void SCartridgeCard::removeMaterial()
{
    int row = ui->tableViewMaterials->currentIndex().row();

    m_materialsModel->removeRow(row);
    ui->tableViewMaterials->hideRow(row);
    ui->pushButtonRemoveMaterial->setEnabled(false);
    ui->pushButtonAddMaterial->setEnabled(m_modelRW);
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
    if(!checkInput())
        return 0;

    QSqlQuery query(QSqlDatabase::database("connThird"));
    bool nErr = 1;
    bool isNew = (m_cardModel->id() == 0);
    setModelData();

    QUERY_LOG_START(metaObject()->className());
    try
    {
        QUERY_EXEC_TH(&query,nErr,QUERY_BEGIN);
        m_cardModel->commit();
        if(isNew)
            m_materialsModel->setCardId(m_cardModel->id());
        m_materialsModel->commit();

#ifdef QT_DEBUG
//        Global::throwDebug();
#endif
        QUERY_COMMIT_ROLLBACK(&query,nErr);

    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if (type != Global::ThrowType::ConnLost)
        {
            QUERY_COMMIT_ROLLBACK(&query, nErr);
        }
    }

    QUERY_LOG_STOP;

    if(nErr)
    {
        if(!m_id)
        {
            m_id = m_cardModel->id();
            setMaterialsTableFilter(m_id);
            emit newCardCreated(m_id);
        }
        else
        {
            emit cardModified(m_id);
        }

        updateWidgets();
        shortlivedNotification *newPopup = new shortlivedNotification(this, tr("Успешно"), tr("Данные сохранены"), QColor(214,239,220), QColor(229,245,234));
    }

    return nErr;
}

void SCartridgeCard::closeForm()
{
    ui->pushButtonCancel->setDisabled(true);
    this->deleteLater();
}

/*  Обработка щелчка по секции заголовка таблицы материалов.
*/
void SCartridgeCard::sortMaterials(const int column)
{
    if(!m_id)
        return;

    if(m_materialsModel->isDirty())
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, cartridgeName(),
                                                                    tr("Для применения сортировки требуется сохранить изменения. Сохранить?\n"),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        switch (resBtn)
        {
            case QMessageBox::Yes:
                if(!commit())
                {
                    ui->tableViewMaterials->undoToggleSortIndicator();
                    return;
                }
                break;
            case QMessageBox::No: break;
            default: ui->tableViewMaterials->undoToggleSortIndicator(); return;
        }
    }

    ui->tableViewMaterials->toggleOrder(column);
}

/* Название картриджа.
 * Метод возвращает объединённые в одну строку брэнд и модель.
*/
QString SCartridgeCard::cartridgeName()
{
    return ui->comboBoxVendor->currentText() + " " + ui->lineEditName->text();
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
    if(permissions->editCartridgeCards)
    {
        SCartridgeMaterialsTableItemDelegates *itemDelagates = new SCartridgeMaterialsTableItemDelegates(m_model, this);
        setItemDelegate(itemDelagates);
    }
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

void materialsTable::clearModel()
{
    m_model->clear();
}

void materialsTable::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    m_model->setSelectStatement(query);
    m_model->select();
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
    tr("SalarySumm");
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

/* Переопределённый слот, вызываемый по клику на секцию заголовка таблицы.
 * В редактируемой модели не сохранённые данные будут утеряны при клике,
 * т. к. при этом выполняется запрос SELECT.
 * Проверка наличия изменений и обработка клика производится в другом слоте;
 * т. к. результат проверки непредсказуем, а индикатор сортировки уже изменён
 * (внутренним образом в Qt), то скрываем его, чтобы не путать пользователя.
*/
void materialsTable::horizontalHeaderSectionClicked(const int logicalIndex)
{
    Q_UNUSED(logicalIndex)

    horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
}
