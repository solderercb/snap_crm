#include "ssettingspageglobal.h"
#include "ui_ssettingspageglobal.h"
#include <ProjectGlobals>
#include <SComSettings>
#include <QPushButton>

SSettingsPageGlobal::SSettingsPageGlobal(QWidget *parent) :
    SSettingsPageBase(parent),
    ui(new Ui::SSettingsPageGlobal)
{
    ui->setupUi(this);

    comSettings->initWidgets();

    updateWidgets();
}

SSettingsPageGlobal::~SSettingsPageGlobal()
{
    comSettings->deleteWidgets();
    delete ui;
}

void SSettingsPageGlobal::saveSettings()
{
    comSettings->save();
}

void SSettingsPageGlobal::updateWidgets()
{
    QFormLayout *layout;
    for(int i = 0; i < comSettings->size(); i++)
    {
        switch (comSettings->propertyGroup(i))
        {
            case 0: layout = ui->formLayoutRegional; break;
            case 1: layout = ui->formLayoutPriceList; break;
            case 2: layout = ui->formLayoutAddons; break;
            case 3: layout = ui->formLayoutRepairs; break;
            case 4: layout = ui->formLayoutRepairsDispatcher; break;
            case 5: layout = ui->formLayoutClients; break;
            case 6: layout = ui->formLayoutTasksTerms; break;
            case 7: layout = ui->formLayoutGoods; break;
            case 8: layout = ui->formLayoutWarranty; break;
            case 9: continue;  // группа 9 обрабатывается отдельно
            case 10: layout = ui->formLayoutDocumentsPrint; break;
            case 11: layout = ui->formLayoutOther; break;
            case 12: continue;  // обрабатывается отдельно
            case 13: continue;  // обрабатывается отдельно
            case 14: layout = ui->formLayoutAdditional; break;
            default: continue;  // параметры с другими кодами групп отображаются на других страницах
        }
        layout->setWidget(i, QFormLayout::LabelRole, comSettings->label(i));
        layout->setWidget(i, QFormLayout::FieldRole, comSettings->widget(i));
    }

    updateWidgetsOfNotificationGroup();
    updateWidgetsOfVoipGroup();
    updateWidgetsOfOnlineStoreGroup();
}

void SSettingsPageGlobal::updateWidgetsOfVoipGroup()
{
    if(comSettings->voipId() <= 0 || comSettings->voipId() == 3)
    {
        ui->groupBoxVoip->setVisible(false);
    }
    else
    {
        ui->groupBoxVoip->setVisible(true);

        QGridLayout *grLayout = ui->gridLayoutVoip;
        int grLayoutRow = 0, grLayoutColumn = 0;
        QMap<int, int> propIds;
        propIds.insert(0, SComSettings::C_voipAsteriskHost);
        propIds.insert(1, SComSettings::C_voipAsteriskPort);
        propIds.insert(2, SComSettings::C_voipAsteriskLogin);
        propIds.insert(3, SComSettings::C_voipAsteriskPassword);
        propIds.insert(4, SComSettings::C_voipAsteriskWebPort);
        propIds.insert(5, SComSettings::C_voipKey);
        propIds.insert(6, SComSettings::C_voipPrefix);
        propIds.insert(7, SComSettings::C_voipSecret);
        propIds.insert(8, SComSettings::C_voipEndpoint);

        QMap<int, int>::const_iterator i = propIds.constBegin();
        while(i != propIds.constEnd())
        {
            grLayout->removeWidget(comSettings->label(i.value()));
            grLayout->removeWidget(comSettings->widget(i.value()));
            i++;
        }

        i = propIds.constBegin();
        while(i!=propIds.constEnd())
        {
            int colSpan = 1;
            switch((comSettings->voipId())*10 + i.key())  // 0 - off, 10 - Zadarma, 20 - Asterisk, 30 - off, 40 - Rostelecom, 50 - Mango telecom, 60 - Megafon
            {
                case 15: colSpan = 3; break;
                case 17: colSpan = 3; break;
                case 20: colSpan = 1; break;
                case 21: colSpan = 2; break;
                case 22: colSpan = 4; break;
                case 23: colSpan = 3; break;
                case 24: colSpan = 4; break;
                case 26: colSpan = 4; break;
                case 45: colSpan = 3; break;
                case 47: colSpan = 3; break;
                case 55: colSpan = 3; break;
                case 57: colSpan = 3; break;
                case 65: colSpan = 3; break;
                case 68: colSpan = 4; break;
                default: i++; continue;
            }

            if(grLayoutColumn == 0 || grLayoutColumn == 2)
            {
                QWidget *lw = comSettings->label(i.value());
                grLayout->addWidget(lw, grLayoutRow, grLayoutColumn++, 1, colSpan);   // label
            }
            QWidget *ew = comSettings->widget(i.value());
            grLayout->addWidget(ew, grLayoutRow, grLayoutColumn++, 1, colSpan);

            if(i.key() == 3 || i.key() == 5 || i.key() == 7)
            {
                dynamic_cast<QLineEdit*>(ew)->setEchoMode(QLineEdit::Password);
                if(!m_extraWidgets.contains(i.value()))
                {
                    QToolButton *tb = new QToolButton();
                    QIcon icon;
                    icon.addFile(QString::fromUtf8(":/icons/light/1F441_32.png"), QSize(), QIcon::Normal, QIcon::Off);
                    tb->setIcon(icon);
                    connect(tb, &QToolButton::clicked, this, &SSettingsPageGlobal::toggleLineEditEchoMode);
                    m_extraWidgets.insert(i.value(), tb);
                }
                QWidget *tb = m_extraWidgets.value(i.value());
                grLayout->addWidget(tb, grLayoutRow, 4, 1, 1);
            }


            if(i.key() > 0)         // виджеты Asterisk::host и Asterisk::port в одну строку, остальные как обычно
            {
                grLayoutColumn = 0;
                grLayoutRow++;
            }
            i++;
        }

        if(!voipTestButton)
        {
            voipTestButton = new QPushButton(ui->groupBoxVoip);
            connect(voipTestButton, &QPushButton::clicked, this, &SSettingsPageGlobal::voipTest);
        }
        voipTestButton->setText((comSettings->voipId() == 2)?tr("Тест"):tr("Баланс"));
        grLayout->addWidget(voipTestButton, grLayoutRow, grLayoutColumn++, 1, 5);
    }
}

// TODO: Это копия кода метода updateWidgetsOfVoipGroup(). Необходимо подумать над созданием универсального метода
void SSettingsPageGlobal::updateWidgetsOfOnlineStoreGroup()
{
    if(comSettings->onlineStoreId() <= 0)
    {
        ui->groupBoxOnlineStore->setVisible(false);
    }
    else
    {
        ui->groupBoxOnlineStore->setVisible(true);

        QGridLayout *grLayout = ui->gridLayoutOnlineStore;
        int grLayoutRow = 0, grLayoutColumn = 0;

        QMap<int, int> propIds;
        propIds.insert(0, SComSettings::C_onlineStoreUrl);
        propIds.insert(1, SComSettings::C_onlineStoreKey);

        QMap<int, int>::const_iterator i = propIds.constBegin();
        while(i != propIds.constEnd())
        {
            grLayout->removeWidget(comSettings->label(i.value()));
            grLayout->removeWidget(comSettings->widget(i.value()));
            i++;
        }

        i = propIds.constBegin();
        while(i!=propIds.constEnd())
        {
            int colSpan = 1;
            switch((comSettings->onlineStoreId())*10 + i.key())  // 0 - off, 10 - OpenCart
            {
                case 10: colSpan = 4; break;
                case 11: colSpan = 3; break;
                default: i++; continue;
            }

            if(grLayoutColumn == 0)
            {
                QWidget *lw = comSettings->label(i.value());
                grLayout->addWidget(lw, grLayoutRow, grLayoutColumn++, 1, colSpan);   // label
            }
            QWidget *ew = comSettings->widget(i.value());
            grLayout->addWidget(ew, grLayoutRow, grLayoutColumn++, 1, colSpan);

            if(i.key() == 1)
            {
                dynamic_cast<QLineEdit*>(ew)->setEchoMode(QLineEdit::Password);
                if(!m_extraWidgets.contains(i.value()))
                {
                    QToolButton *tb = new QToolButton();
                    QIcon icon;
                    icon.addFile(QString::fromUtf8(":/icons/light/1F441_32.png"), QSize(), QIcon::Normal, QIcon::Off);
                    tb->setIcon(icon);
                    connect(tb, &QToolButton::clicked, this, &SSettingsPageGlobal::toggleLineEditEchoMode);
                    m_extraWidgets.insert(i.value(), tb);
                }
                QWidget *tb = m_extraWidgets.value(i.value());
                grLayout->addWidget(tb, grLayoutRow, 4, 1, 1);
            }

            grLayoutColumn = 0;
            grLayoutRow++;

            i++;
        }
    }
}

void SSettingsPageGlobal::updateWidgetsOfNotificationGroup()
{
    QGridLayout *grLayout = ui->formLayoutNotifications;
    int grLayoutRow = 0, grLayoutColumn = 0;
    for(int i = 0; i < comSettings->size(); i++)
    {
        if(comSettings->propertyGroup(i) != 9)
            continue;

        // Отображение настроек данной группы аналогично АСЦ: label checkBox ComboBox
        if(grLayoutColumn == 0)
        {
            QWidget *lw = comSettings->label(i);
            grLayout->addWidget(lw, grLayoutRow, grLayoutColumn++, 1, 1);   // label
        }

        QWidget *ew = comSettings->widget(i);
        QComboBox *cb = dynamic_cast<QComboBox *>(ew);   // TODO: здесь должен быть не простой comboBox, а виджет выбора цвета
        if(cb)
            grLayoutColumn = 2;
        grLayout->addWidget(ew, grLayoutRow, grLayoutColumn++, 1, 1);

        if(cb)
        {
            grLayoutRow++;
            grLayoutColumn = 0;
        }
    }
}

void SSettingsPageGlobal::toggleLineEditEchoMode()
{
    QWidget *sender = dynamic_cast<QWidget*>(QObject::sender());
    QLineEdit *le = dynamic_cast<QLineEdit*>(comSettings->widget(m_extraWidgets.key(sender)));
    if(le->echoMode() == QLineEdit::Password)
        le->setEchoMode(QLineEdit::Normal);
    else
        le->setEchoMode(QLineEdit::Password);
}

void SSettingsPageGlobal::voipTest()
{

}

