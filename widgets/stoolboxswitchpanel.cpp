#include "stoolboxswitchpanel.h"
#include "ui_stoolboxswitchpanel.h"

SToolboxSwitchPanel::SToolboxSwitchPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SToolboxSwitchPanel)
{
    ui->setupUi(this);
    m_buttons.insert(0, ui->button0); // Кнопка по-умолчанию "Сводка"
    connect(ui->button0, SIGNAL(toggled(bool)), this, SLOT(slotButtonToggled(bool)));
    ui->button0->setText(tr("Сводка"));
    ui->button0->setIcon(QIcon());
}

SToolboxSwitchPanel::~SToolboxSwitchPanel()
{
    clearButtons(); // сначала добавленные кнопки
    m_buttons.remove(0);
    delete ui;
}

void SToolboxSwitchPanel::redraw()
{
    QLayoutItem *layoutItem;
    QSpacerItem *verticalSpacer;
    QMap<int, QCommandLinkButton*>::const_iterator i = m_buttons.constBegin();

    verticalSpacer = ui->verticalSpacer;
    while(ui->verticalLayout->count() > 1)
    {
        layoutItem = ui->verticalLayout->itemAt(ui->verticalLayout->count() - 1);
        ui->verticalLayout->removeItem(layoutItem);
    }
    while(++i != m_buttons.constEnd())
    {
        ui->verticalLayout->insertWidget(i.key(), i.value());
    }
    ui->verticalLayout->insertSpacerItem(ui->verticalLayout->count(), verticalSpacer);
}

void SToolboxSwitchPanel::addButton(int buttonId)
{
    QCommandLinkButton *button = new QCommandLinkButton(this);
    button->setAutoExclusive(1);
    button->setCheckable(1);
    connect(button, SIGNAL(toggled(bool)), this, SLOT(slotButtonToggled(bool)));
    if(!buttonId)
    {
        appendButton(button);
    }
    else
    {
        Q_ASSERT_X(!m_buttons.contains(buttonId), objectName().toLocal8Bit(), QString("Button with id %1 already exists").arg(buttonId).toLocal8Bit());
        m_buttons.insert(buttonId, button);
    }
}

void SToolboxSwitchPanel::addButton(const QString &text, const QIcon &icon, int buttonId)
{
    QCommandLinkButton *button;
    addButton(buttonId);
    button = m_buttons.value(buttonId);
    button->setText(text);
    button->setIcon(icon);
}

void SToolboxSwitchPanel::appendButton(QCommandLinkButton *button)
{
    int index = m_buttons.size();
    Q_ASSERT_X(!m_buttons.contains(index), objectName().toLocal8Bit(), QString("Button with id %1 already exists").arg(m_buttons.size()).toLocal8Bit());
    m_buttons.insert(index, button);
}

void SToolboxSwitchPanel::deleteButton(int id)
{
    QCommandLinkButton *button;
    if(m_buttons.contains(id))
    {
        button = m_buttons.value(id);
        m_buttons.remove(id);
        button->deleteLater();
    }
}

void SToolboxSwitchPanel::clearButtons()
{
    while(m_buttons.count() > 1)    // Кнопка "Сводка" удаляется в деструкторе формы, а при вызове этого метода не из ~SToolboxSwitchPanel() её удалять не нужно
    {
        deleteButton(m_buttons.lastKey());
    }
}

void SToolboxSwitchPanel::hideButton(int buttonId)
{
    if(!m_buttons.contains(buttonId))
        return;
    m_buttons.value(buttonId)->hide();
}

void SToolboxSwitchPanel::showButton(int buttonId)
{
    if(!m_buttons.contains(buttonId))
        return;
    m_buttons.value(buttonId)->show();
}

void SToolboxSwitchPanel::setButtonVisible(int buttonId, bool state)
{
    if(!m_buttons.contains(buttonId))
        return;
    m_buttons.value(buttonId)->setVisible(state);
}

void SToolboxSwitchPanel::setButtonText(int buttonId, const QString text)
{
    if(!m_buttons.contains(buttonId))
        return;

    m_buttons.value(buttonId)->setText(text);
}

void SToolboxSwitchPanel::setIcon(int buttonId, QIcon icon)
{
    if(!m_buttons.contains(buttonId))
        return;

    m_buttons.value(buttonId)->setIcon(icon);
}

void SToolboxSwitchPanel::slotButtonToggled(bool checked)
{
    if(!checked)
        return;
    QMap<int, QCommandLinkButton*>::const_iterator i = m_buttons.constBegin();
    for(; i != m_buttons.constEnd() && !i.value()->isChecked(); ++i);
    m_currentIndex = i.key();
    emit buttonToggled(m_currentIndex);
}
