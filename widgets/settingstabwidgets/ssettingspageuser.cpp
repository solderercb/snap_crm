#include "ssettingspageuser.h"
#include "ui_ssettingspageuser.h"
#include "global.h"

SSettingsPageUser::SSettingsPageUser(QWidget *parent) :
    SSettingsPageBase(parent),
    ui(new Ui::SSettingsPageUser)
{
    ui->setupUi(this);

    userSettings = new SUserSettings();
    userSettings->initWidgets();

    updateWidgets();
}

SSettingsPageUser::~SSettingsPageUser()
{
    userSettings->deleteWidgets();
    delete ui;
}

void SSettingsPageUser::saveSettings()
{
    userSettings->save();  // отключено, т. к. еще не заданы модели данных виджетов ComboBox
}

void SSettingsPageUser::updateWidgets()
{
    QFormLayout *layout;
    for(int i = 0; i < userSettings->count(); i++)
    {
        switch (userSettings->propertyGroup(i))
        {
            case 11: layout = ui->formLayoutInterface; break;
            case 12: layout = ui->formLayoutPrint; break;
            case 13: layout = ui->formLayoutRepairs; break;
            case 14: layout = ui->formLayoutClients; break;
            case 15: layout = ui->formLayoutGoods; break;
            case 16: layout = ui->formLayoutExcelImport; break;
            case 17: layout = ui->formLayoutWindowMode; break;
            default: continue;  // параметры с другими кодами групп отображаются на других страницах
        }
        layout->setWidget(i, QFormLayout::LabelRole, userSettings->widget(i, SPropertyCollection::Label));
        layout->setWidget(i, QFormLayout::FieldRole, userSettings->widget(i, SPropertyCollection::Editor));
    }

}

//void SSettingsPageUser::updateWidgetsOf...Group()
//{
//    if(userSettings->voipId <= 0 || userSettings->voipId == 3)
//    {
//        ui->groupBoxVoip->setVisible(false);
//    }
//    else
//    {
//        ui->groupBoxVoip->setVisible(true);

//        QGridLayout *grLayout = ui->gridLayoutVoip;
//        int grLayoutRow = 0, grLayoutColumn = 0;
//        QMap<int, int> propIds;
//        propIds.insert(0, userSettings->propertyId("voipAsteriskHost"));
//        propIds.insert(1, userSettings->propertyId("voipAsteriskPort"));
//        propIds.insert(2, userSettings->propertyId("voipAsteriskLogin"));
//        propIds.insert(3, userSettings->propertyId("voipAsteriskPassword"));
//        propIds.insert(4, userSettings->propertyId("voipAsteriskWebPort"));
//        propIds.insert(5, userSettings->propertyId("voipKey"));
//        propIds.insert(6, userSettings->propertyId("voipPrefix"));
//        propIds.insert(7, userSettings->propertyId("voipSecret"));
//        propIds.insert(8, userSettings->propertyId("voipEndpoint"));

//        QMap<int, int>::const_iterator i = propIds.constBegin();
//        while(i != propIds.constEnd())
//        {
//            grLayout->removeWidget(userSettings->widget(i.value(), SuserSettings::Label));
//            grLayout->removeWidget(userSettings->widget(i.value(), SuserSettings::Editor));
//            i++;
//        }

//        i = propIds.constBegin();
//        while(i!=propIds.constEnd())
//        {
//            int colSpan = 1;
//            switch((userSettings->voipId)*10 + i.key())  // 0 - off, 10 - Zadarma, 20 - Asterisk, 30 - off, 40 - Rostelecom, 50 - Mango telecom, 60 - Megafon
//            {
//                case 15: colSpan = 3; break;
//                case 17: colSpan = 3; break;
//                case 20: colSpan = 1; break;
//                case 21: colSpan = 2; break;
//                case 22: colSpan = 4; break;
//                case 23: colSpan = 3; break;
//                case 24: colSpan = 4; break;
//                case 26: colSpan = 4; break;
//                case 45: colSpan = 3; break;
//                case 47: colSpan = 3; break;
//                case 55: colSpan = 3; break;
//                case 57: colSpan = 3; break;
//                case 65: colSpan = 3; break;
//                case 68: colSpan = 4; break;
//                default: i++; continue;
//            }

//            if(grLayoutColumn == 0 || grLayoutColumn == 2)
//            {
//                QWidget *lw = userSettings->widget(i.value(), SuserSettings::Label);
//                grLayout->addWidget(lw, grLayoutRow, grLayoutColumn++, 1, colSpan);   // label
//            }
//            QWidget *ew = userSettings->widget(i.value(), SuserSettings::Editor);
//            grLayout->addWidget(ew, grLayoutRow, grLayoutColumn++, 1, colSpan);

//            if(i.key() == 3 || i.key() == 5 || i.key() == 7)
//            {
//                dynamic_cast<QLineEdit*>(ew)->setEchoMode(QLineEdit::Password);
//                if(!m_extraWidgets.contains(i.value()))
//                {
//                    QToolButton *tb = new QToolButton();
//                    QIcon icon;
//                    icon.addFile(QString::fromUtf8(":/icons/light/1F441_32.png"), QSize(), QIcon::Normal, QIcon::Off);
//                    tb->setIcon(icon);
//                    connect(tb, &QToolButton::clicked, this, &SSettingsPageUser::toggleLineEditEchoMode);
//                    m_extraWidgets.insert(i.value(), tb);
//                }
//                QWidget *tb = m_extraWidgets.value(i.value());
//                grLayout->addWidget(tb, grLayoutRow, 4, 1, 1);
//            }


//            if(i.key() > 0)         // виджеты Asterisk::host и Asterisk::port в одну строку, остальные как обычно
//            {
//                grLayoutColumn = 0;
//                grLayoutRow++;
//            }
//            i++;
//        }

//        if(!voipTestButton)
//        {
//            voipTestButton = new QPushButton(ui->groupBoxVoip);
//            connect(voipTestButton, &QPushButton::clicked, this, &SSettingsPageUser::voipTest);
//        }
//        voipTestButton->setText((userSettings->voipId == 2)?tr("Тест"):tr("Баланс"));
//        grLayout->addWidget(voipTestButton, grLayoutRow, grLayoutColumn++, 1, 5);
//    }
//}
