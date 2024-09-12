#include "sclientselectform.h"
#include "ui_sclientselectform.h"
#include "global.h"
#include "mainwindow.h"
#include "tabclients.h"

SClientSelectForm::SClientSelectForm(QWidget *parent) :
    SClientInputFormBase(parent),
    ui(new Ui::SClientSelectForm)
{
    ui->setupUi(this);

    m_widgetLastName = ui->lineEditLastName;

    m_widgetId = ui->spinBoxId;
    m_widgetId->setReadOnly(true);
    m_widgetId->setMinimum(0);
    m_widgetId->setMaximum(2147483647);

    ui->doubleSpinBoxBalance->setReadOnly(true);
    ui->doubleSpinBoxBalance->setMinimum(-999999999);
    ui->doubleSpinBoxBalance->setMaximum(999999999);

    connect(ui->lineEditLastName,SIGNAL(buttonClicked(int)),this,SLOT(lineEditClientLastNameButtonClickHandler(int)));
    connect(ui->lineEditLastName, &QLineEdit::textChanged, ui->widgetClientMatch, &SClientMatch::findByName);
    connect(ui->widgetClientMatch,SIGNAL(clientSelected(int)),this,SLOT(fillCreds(int)));
    ui->widgetClientMatch->setClientType(SClientMatch::NameSearchScope::Both);
}

SClientSelectForm::~SClientSelectForm()
{
    delete ui;
}

SClientModel *SClientSelectForm::clientModel()
{
    return m_clientModel;
}

bool SClientSelectForm::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        QWidget *widget = static_cast<QWidget*>(watched);
        int column = m_layoutRefWidgets.key(widget, -1);
        if(column >= 0)
        {
            ui->gridLayout->setColumnMinimumWidth(column, widget->width());
        }
    }

    return false;
}

void SClientSelectForm::setClientModel(SClientModel *model)
{
    m_clientModel = model;
//    connect(m_clientModel, &SClientModel::modelUpdated, this, [=]{update();});

    update();
}

void SClientSelectForm::setLabelText(const QString &text)
{
    ui->labelClient->setText(text);
}

void SClientSelectForm::setLineEditText(const QString &text)
{
    ui->lineEditLastName->setText(text);
}

void SClientSelectForm::setIdVisible(bool state)
{
    m_showId = state;
    m_widgetId->setVisible(m_showId);
    if(!m_showId && m_layoutRefWidgets.contains(1))
    {
        m_layoutRefWidgets.remove(1);
        ui->gridLayout->setColumnMinimumWidth(1, 0);
    }

    if(!m_showId)
    {
        ui->gridLayout->removeWidget(m_widgetId);
        ui->gridLayout->removeWidget(m_widgetLastName);
        ui->gridLayout->addWidget(m_widgetLastName, 0, 1, 1, 2);
    }
    else
    {
        ui->gridLayout->removeWidget(m_widgetLastName);
        ui->gridLayout->addWidget(m_widgetId, 0, 1, 1, 1);
        ui->gridLayout->addWidget(m_widgetLastName, 0, 2, 1, 1);
    }
}

void SClientSelectForm::setBalanceVisible(bool state)
{
    m_showBalance = state;
    ui->labelBalance->setVisible(m_showBalance);
    ui->doubleSpinBoxBalance->setVisible(m_showBalance);

    if(state)
        update();
}

void SClientSelectForm::setQuickFindEnabled(bool state)
{
    ui->widgetClientMatch->setEnabled(state);
}

/* Настройка виджета для связи с макетом родительского виджета.
 * Ширина виджета, указатель на который передан, используется
 * для настройки минимальной ширины столбца.
 * Например, на вкладке проведения кассовой операции виджеты расположены в три столбца.
 * Чтобы дочерние элементы этого виджета располагались по одной линии с виджетами
 * вкладки необходимо вызвать этот метод дважды для 0-го и 1-го столбцов.
*/
void SClientSelectForm::setLayoutRefWidget(int column, QWidget *widget)
{
    if(column < 0 || column >= ui->gridLayout->columnCount())
        Q_ASSERT_X(0, "SClientSelectForm::setLayoutRefWidget(int, QWidget*)", "widget gridLayout column mismatch");

    if(!widget && m_layoutRefWidgets.contains(column))
    {
        m_layoutRefWidgets.remove(column);
        return;
    }

    m_layoutRefWidgets.insert(column, widget);
    widget->installEventFilter(this);
}

void SClientSelectForm::setFont(QFont font)
{
    ui->lineEditLastName->setFont(font);
}

/* Установка текста в LineEdit с предварительной блокировкой сигналов (атомарно)
*/
void SClientSelectForm::setLineEditNameTextAtomic(const QString &text)
{
    bool prevState = m_widgetLastName->signalsBlocked();
    m_widgetLastName->blockSignals(true);
    m_widgetLastName->setText(text);
    m_widgetLastName->blockSignals(prevState);
}

void SClientSelectForm::update()
{
    setHandlingButtons(m_lineEditAccessMode);

    if(!m_clientModel || !m_clientId)
    {
        m_widgetId->clear();
        setLineEditNameTextAtomic(QString());
        ui->doubleSpinBoxBalance->clear();
        return;
    }

    m_widgetId->setValue(m_clientId);
    setLineEditNameTextAtomic(m_clientModel->fullShortName());

    if(m_clientModel->balanceEnabled())
        ui->doubleSpinBoxBalance->setValue(m_clientModel->balance());
    else
        ui->doubleSpinBoxBalance->clear();
}

void SClientSelectForm::clearCreds(bool keepCoincidenceShown)
{
    if(m_clientModel)
        m_clientModel->clear();

    m_clientId = 0;
    m_lineEditAccessMode = AccessMode::Full;

    if (!keepCoincidenceShown)
        ui->widgetClientMatch->clear();

    update();
}

void SClientSelectForm::fillCreds(int clientId)
{
    Q_ASSERT_X(m_clientModel, "SClientSelectForm::fillCreds(int)", "model not set");
    if(!clientId)
        return;

    clearCreds();    // очищаем данные клиента, но не прячем таблицу совпадений
    m_clientModel->load(clientId);
    m_clientId = m_clientModel->id();
    m_lineEditAccessMode = AccessMode::SelectClear;

    update();
}

/* Проверка включен ли баланс и вывод диалогового окна для возможности его включить (при наличии соотв. прав).
 * Возвращает 1 в случае ошибки.
*/
int SClientSelectForm::checkBalanceEnabled()
{
    if(!m_clientModel)
        return 0;

    setDefaultStylesheets();

    if(!m_clientModel->balanceEnabled())
    {
        bool tmp = permissions->editClients;    // Редактировать существующих клиентов
        if(tmp)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Баланс клиента"));
            msgBox.setText(tr("Баланс клиента не включен. Включить?"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);

            if(msgBox.exec() == QMessageBox::Yes)
            {
                tmp = m_clientModel->setBalanceEnabled();
            }
            else
                tmp = 0;
        }
        if(!tmp)
        {
            ui->lineEditLastName->setStyleSheet(commonLineEditStyleSheetRed);
            return 1;
        }
    }

    return 0;
}

int SClientSelectForm::checkInput()
{
    int error = 0;
    setDefaultStylesheets();

    if( m_clientId == 0 )
    {
        error = 1;
        ui->lineEditLastName->setStyleSheet(commonLineEditStyleSheetRed);
    }

    return error;
}

void SClientSelectForm::setDefaultStylesheets()
{
    ui->lineEditLastName->setStyleSheet(commonLineEditStyleSheet);
}

void SClientSelectForm::setHandlingButtons(int mode)
{
    bool ro = (mode != AccessMode::Full);
    if(mode == AccessMode::Denied)
    {
        if(m_clientId && permissions->viewClients)
            ui->lineEditLastName->setButtons("Open");
        else
            ui->lineEditLastName->clearButtons();
        ui->widgetClientMatch->hide();
    }
    else if(mode == AccessMode::SelectClear)
    {
        if(m_clientId)
            ui->lineEditLastName->setButtons("Search,Clear");
        else
            ui->lineEditLastName->setButtons("Search");
    }
    else
    {
        if(m_clientId)
            ui->lineEditLastName->setButtons("Clear");
        else
            ui->lineEditLastName->setButtons("Search");
    }
    ui->lineEditLastName->setReadOnly(ro);
    ui->lineEditLastName->updateGeometry();
}


void SClientSelectForm::setMatchWidgetFilter(int category)
{
    ui->widgetClientMatch->setCategory(category);
}

void SClientSelectForm::lineEditClientLastNameButtonClickHandler(int buttonId)
{
    MainWindow *mw = MainWindow::getInstance();
    tabClients *clientsTab = nullptr;
    SWidget *parentTab = findParentTab();
    switch (buttonId)
    {
        case SLineEdit::Clear: clearCreds(0); break;
        case SLineEdit::Search: clientsTab = static_cast<tabClients *>(mw->createTabClients(parentTab)); break;
        case SLineEdit::Open: mw->createTabClient(m_clientModel->id()); break;
    }
    if(clientsTab)
    {
        QObject::connect(clientsTab, &tabClients::doubleClicked,this,&SClientSelectForm::fillCreds);
        QObject::connect(clientsTab, &tabClients::activateCaller, mw, &MainWindow::reactivateCallerTab);
        ui->widgetClientMatch->clear();
        clientsTab->setTabTitle(i_clientsTabTitle);
        clientsTab->setCategory(i_clientsTabDefaultCategory);
        clientsTab->setCallerPtr(parentTab);
    }
}

void SClientSelectForm::guiFontChanged()
{
    QFont font(guiFont());

    ui->labelClient->setFont(font);
    ui->lineEditLastName->setFont(font);
    ui->labelBalance->setFont(font);
    ui->doubleSpinBoxBalance->setFont(font);
    ui->spinBoxId->setFont(font);
}

#ifdef QT_DEBUG
void SClientSelectForm::randomFill()
{
//    fillCreds(33);
//    return;
    fillCreds(randomClientIdFromDB()); // пытаемся заполнить данные уже имеющимся клиентом
//    ui->lineEditLastName->setText("");
}
#endif
