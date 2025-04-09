#include "scomments.h"
#include "ui_scomments.h"

SComments::SComments(QWidget *parent) :
    SWidget(parent),
    ui(new Ui::SComments)
{
    ui->setupUi(this);
    initTableMenu();

    m_query = new QSqlQuery(QSqlDatabase::database("connThird"));
    commentsModel = new SCommentsModel();
    connect(commentsModel, &SCommentsModel::modelReset, this, &SComments::updateTable);

    ui->plainTextEdit->enableExtKeyPressHandler(true);
    connect(ui->plainTextEdit, &SPlainTextEdit::keyPress, this, &SComments::lineEditKeyPressEvent);
    ui->tableViewComments->setModel(commentsModel);
    ui->tableViewComments->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableViewComments->verticalHeader()->hide();
    ui->tableViewComments->horizontalHeader()->hide();
    ui->tableViewComments->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewComments, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menuRequest(QPoint)));
    connect(ui->tableViewComments, SIGNAL(copyText()), this, SLOT(copyToClipboard()));
}

SComments::~SComments()
{
    delete ui;
    delete commentsModel;
    delete tableMenu;
}

void SComments::setMode(const int mode)
{
    commentsModel->setMode(mode);
}

void SComments::load(const int id)
{
//    qDebug().nospace() << "[" << this << "] load()";
    checkParentTab();
    Q_ASSERT_X(commentsModel->mode() != SCommentModel::NotSet, m_parentTab->objectName().toLocal8Bit(), QString("Comments widget: mode not set!").toLocal8Bit());
    commentsModel->load(id);
}

void SComments::initTableMenu()
{
    tableMenu = new QMenu(this);
    QAction * copy = new QAction(tr("Копировать"), tableMenu);
    copy->setProperty("type", MenuActions::Copy);
    connect(copy, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    tableMenu->addAction(copy);
    QAction * edit = new QAction(tr("Редактировать"), tableMenu);
    edit->setProperty("type", MenuActions::Edit);
    connect(edit, SIGNAL(triggered()), this, SLOT(edit()));
    tableMenu->addAction(edit);
    QAction * remove = new QAction(tr("Удалить"), tableMenu);
    remove->setProperty("type", MenuActions::Remove);
    connect(remove, SIGNAL(triggered()), this, SLOT(remove()));
    tableMenu->addAction(remove);
}

void SComments::menuRequest(QPoint pos)
{
    int recordEditable = isEditable(ui->tableViewComments->selectionModel()->currentIndex().row());

    const QList<QAction*> actions = tableMenu->actions();
    for(QAction *action : actions)
    {
        switch (action->property("type").toInt())
        {
            case MenuActions::Edit:
            case MenuActions::Remove: action->setVisible(recordEditable); break;
        }
    }
    tableMenu->popup(ui->tableViewComments->viewport()->mapToGlobal(pos));
}

bool SComments::isEditable(const int row)
{
    int timeDiff = commentsModel->created(row).secsTo(QDateTime::currentDateTimeUtc());
    int authorId = commentsModel->userId(row);

    // Удаление и редактирование доступно только в течение некоторого времени после создания (подсмотрено в Telegram).
    // Удалять и редактировать можно только свои комментарии или при наличии особого разрешения
    if((timeDiff < 3600 && authorId == userDbData->id) || permissions->editAnyComment )
        return 1;

    return 0;
}

void SComments::setParentTab(tabCommon *tab)
{
    m_parentTab = tab;
}

tabCommon* SComments::findParentTab()
{
    // TODO: придумать способ получить указатель на родительский объект (tabRepair или др. вкладку, исполующую данных класс).
    // На вкладке tabRepair этого не удалось сделать из-за виджета QScrollArea — метод findChildren этого объекта возвращает список,
    // в котором нет SComments.
    return nullptr;
}

/*  Защита от дурака
*/
void SComments::checkParentTab()
{
    Q_ASSERT_X(m_parentTab, objectName().toLocal8Bit(), QString("Comments widget: parent tab not set!").toLocal8Bit());
}

void SComments::appendLineTermination()
{
    // не уверен, что это когда-то понадобится, но пусть будет...
#ifdef Q_OS_WIN
    QString term = "\r\n";
#elif Q_OS_MAC
    QString term = '\n';
#else
    QString temp = '\r';
#endif
//    ui->plainTextEdit->appendPlainText("");
    ui->plainTextEdit->insertPlainText("\r");
}

void SComments::clearInputField()
{
    ui->plainTextEdit->setProperty("recordId", QVariant());
    ui->plainTextEdit->setProperty("recordRow", QVariant());
    ui->plainTextEdit->clear();
}

void SComments::commit()
{
    bool nErr = 1;

    QUERY_LOG_START(m_parentTab->metaObject()->className());

    try
    {
        QUERY_EXEC_TH(m_query,nErr,QUERY_BEGIN);
        if(!ui->plainTextEdit->property("recordId").isValid())
        {
            commentsModel->add(ui->plainTextEdit->toPlainText());
            userActivityLog->appendRecord(QString("Comment %1").arg(m_parentTab->tabTitle()));
        }
        else
        {
            commentsModel->setText(ui->plainTextEdit->property("recordRow").toInt(), ui->plainTextEdit->toPlainText());
            ui->plainTextEdit->setProperty("recordId", QVariant());
            ui->plainTextEdit->setProperty("recordRow", QVariant());
        }

        ui->plainTextEdit->clear();

        QUERY_COMMIT_ROLLBACK(m_query,nErr);
    }
    catch (Global::ThrowType type)
    {
        nErr = 0;
        if (type != Global::ThrowType::ConnLost)
        {
            QUERY_COMMIT_ROLLBACK(m_query, nErr);
        }
    }

    QUERY_LOG_STOP;
}

void SComments::editLastComment()
{
    if(isEditable(0))
    {
        ui->tableViewComments->selectRow(0);
        edit();
    }
}

void SComments::remove()
{
    bool nErr = 1;

    checkParentTab();
    int row = ui->tableViewComments->selectionModel()->currentIndex().row();
    if(row >= 0){
        if (QMessageBox::warning(this,
                                 tr("Удаление записи"),
                                 tr("Вы уверены, что хотите удалить эту запись?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QUERY_LOG_START(m_parentTab->metaObject()->className());
            try
            {
                QUERY_EXEC_TH(m_query,nErr,QUERY_BEGIN);
                if(ui->plainTextEdit->property("recordId").isValid())
                {
                    ui->plainTextEdit->setProperty("recordId", QVariant());
                    ui->plainTextEdit->setProperty("recordRow", QVariant());
                    ui->plainTextEdit->clear();
                }
                commentsModel->remove(row);
                QUERY_COMMIT_ROLLBACK(m_query,nErr);
            }
            catch (Global::ThrowType type)
            {
                nErr = 0;
                if (type != Global::ThrowType::ConnLost)
                {
                    QUERY_COMMIT_ROLLBACK(m_query, nErr);
                }
            }

            QUERY_LOG_STOP;
        }
    }
}

void SComments::edit()
{
    int row = ui->tableViewComments->selectionModel()->currentIndex().row();
    QString text = commentsModel->text(row);
    int id = commentsModel->recordId(row);
    ui->plainTextEdit->setPlainText(text);
    ui->plainTextEdit->setProperty("recordId", id);
    ui->plainTextEdit->setProperty("recordRow", row);
    ui->plainTextEdit->setFocus();
}

/* Копирование текста комментария в буфер обмена
 * подсмотрено: https://www.medo64.com/2019/12/copy-to-clipboard-in-qt/
*/
void SComments::copyToClipboard(const bool)
{
    // TODO: решить какие данные копировать, если выделена одна строка или несколько. Или сделать два варианта копирования в контекстном меню
    QModelIndexList indexesList =  ui->tableViewComments->selectedIndexes();
    QStringList text;

    for(int i = 0; i < indexesList.count(); i++)
    {
        if(indexesList.at(i).column() == SCommentModel::ColText)
            text.append(indexesList.at(i).data().toString());
    }

    QClipboard* clipboard = QApplication::clipboard();

    clipboard->setText(text.join("\r\n"), QClipboard::Clipboard);

    if (clipboard->supportsSelection())
    {
        clipboard->setText(text.join("\r\n"), QClipboard::Selection);
    }

    #if defined(Q_OS_LINUX)
        QThread::msleep(1); // workaround for copied text not being available...
#endif
}

void SComments::lineEditKeyPressEvent(int key)
{
    checkParentTab();
    switch(key)
    {
        case SPlainTextEdit::PressedKey::Enter: commit(); break;
        case SPlainTextEdit::PressedKey::ShiftEnter: appendLineTermination(); break;
        case SPlainTextEdit::PressedKey::Escape: clearInputField(); break;
        case SPlainTextEdit::PressedKey::Up: editLastComment(); break;
    }
}

void SComments::updateTable()
{
    ui->tableViewComments->resizeRowsToContents();
}

/*********************************************************************************************************************************/
commentsTable::commentsTable(QWidget *parent) :
    QTableView(parent)
{
    setShowGrid(false);
    fontMetrics = new QFontMetrics(this->font());
}

commentsTable::~commentsTable()
{
    delete fontMetrics;
}

void commentsTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    int i, j;
    int colTextWidth = 0;
    int colWidths[] = {120,90,0};

    // при масштабе 125% и ширине столбца по умолчанию дата и время комментария не влезают в одну строку; пересчитываем ширину
    colWidths[0] = fontMetrics->size(Qt::TextSingleLine, QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).width() + 10;
    colTextWidth = this->geometry().width();

    for (i = 0, j = 0; i < model()->columnCount(); i++)
    {
        if(static_cast<SCommentsModel*>(model())->isColumnHidden(i))
            hideColumn(i);
        else
        {
            colTextWidth -= colWidths[j];
            setColumnWidth(i, colWidths[j]);
            j++;
        }
    }
    if (verticalScrollBar()->isVisible())
        setColumnWidth(SCommentModel::Columns::ColText, colTextWidth - verticalScrollBar()->width());
    else
        setColumnWidth(SCommentModel::Columns::ColText, colTextWidth);
    setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
    resizeRowsToContents();
}

void commentsTable::keyPressEvent(QKeyEvent *event)
{
    if( (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C) || \
        (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Insert) )
        emit copyText();
    else
        QTableView::keyPressEvent(event);
}

QModelIndexList commentsTable::selectedIndexes() const
{
    return QTableView::selectedIndexes();
}

