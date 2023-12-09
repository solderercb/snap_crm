#include "sphone.h"
#include "ui_sphone.h"

SPhone::SPhone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPhone)
{
    ui->setupUi(this);
    guiFontChanged();
    QWidget::setFocusProxy(ui->lineEditPhone);

    ui->comboBoxPhoneMask->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditPhone->setStyleSheet(commonLineEditStyleSheet);

    ui->comboBoxPhoneMask->setModel(clientPhoneTypesModel);
    ui->comboBoxPhoneMask->setModelColumn(0);
    ui->comboBoxPhoneMask->setCurrentIndex(-1);

    connect(ui->comboBoxPhoneMask,SIGNAL(currentIndexChanged(int)),this,SLOT(phoneMaskChanged(int)));
    connect(ui->checkBoxPrimary,SIGNAL(toggled(bool)), this,SLOT(checkBoxTypeToggled(bool)));
    connect(ui->pushButtonAddPhone,SIGNAL(clicked()),this,SIGNAL(buttonAddClicked()));
    connect(ui->pushButtonDelPhone,SIGNAL(clicked()),this,SLOT(buttonDelClicked()));
    connect(ui->pushButtonEditPhone,SIGNAL(clicked()),this,SLOT(buttonEditClicked()));
    connect(ui->lineEditPhone,SIGNAL(textEdited(QString)),this,SLOT(numberEdited(QString)));
    connect(ui->lineEditPhone,SIGNAL(editingFinished()),this,SLOT(numberEditFinished()));
    connect(ui->lineEditNotes,SIGNAL(editingFinished()),this,SLOT(notesEditFinished()));
    connect(ui->checkBoxViber,SIGNAL(clicked(bool)), this,SLOT(checkBoxViberClicked(bool)));
    connect(ui->checkBoxTelegram,SIGNAL(clicked(bool)), this,SLOT(checkBoxTelegramClicked(bool)));
    connect(ui->checkBoxWhatsapp,SIGNAL(clicked(bool)), this,SLOT(checkBoxWhatsappClicked(bool)));
    connect(ui->checkBoxSMS,SIGNAL(clicked(bool)), this,SLOT(checkBoxSMSClicked(bool)));
    connect(userDbData, &SUserSettings::fontSizeChanged, this, &SPhone::guiFontChanged);
}

SPhone::SPhone(SPhoneModel *model, QWidget *parent) :
   SPhone(parent)
{
    setModel(model);
}

SPhone::~SPhone()
{
    delete ui;
}

SPhoneModel *SPhone::model()
{
    return m_phoneModel;
}

void SPhone::setModel(SPhoneModel *model)
{
    connect(model,SIGNAL(modelUpdated()),this,SLOT(modelUpdated()));

    if(!model->isEmpty())
        setReadOnly();

    m_phoneModel = model;
    m_messengers = model->messengers();
    ui->comboBoxPhoneMask->setCurrentIndex((permissions->viewClients || !m_isReadOnly)?model->maskIndex():-1);
    ui->lineEditPhone->setText((permissions->viewClients || !m_isReadOnly)?model->phone():tr("no permissions"));
    ui->checkBoxViber->setChecked(m_messengers&SPhoneModel::Viber);
    ui->checkBoxTelegram->setChecked(m_messengers&SPhoneModel::Telegram);
    ui->checkBoxWhatsapp->setChecked(m_messengers&SPhoneModel::Whatsapp);
    ui->checkBoxSMS->setChecked(model->receiveSMS());
    ui->lineEditNotes->setText((permissions->viewClients || !m_isReadOnly)?model->note():tr("no permissions"));

    m_isPrimary = model->type();
    if(m_isPrimary)
    {
        ui->checkBoxPrimary->setChecked(true);
    }
    updateButtons();
}

void SPhone::setReadOnly(bool state)
{
    m_isReadOnly = state;
    ui->lineEditPhone->setReadOnly(state);
    ui->lineEditNotes->setReadOnly(state);
    ui->comboBoxPhoneMask->setEnabled(!state);
    ui->checkBoxPrimary->setEnabled(!state);
    ui->checkBoxViber->setEnabled(!state);
    ui->checkBoxTelegram->setEnabled(!state);
    ui->checkBoxWhatsapp->setEnabled(!state);
    ui->checkBoxSMS->setEnabled(!state);
}

void SPhone::clear()
{
    ui->lineEditPhone->clear();
    if(m_isPrimary)
        ui->comboBoxPhoneMask->setCurrentIndex(0);
    else
    {
        ui->comboBoxPhoneMask->setCurrentIndex(-1);
        ui->lineEditPhone->setInputMask("");
    }
}

#ifdef QT_DEBUG
void SPhone::testFill(const int mask, const QString &number)
{
    ui->comboBoxPhoneMask->setCurrentIndex(mask);
    ui->lineEditPhone->setText(number);
    if(!ui->lineEditPhone->hasAcceptableInput())
        ui->lineEditPhone->setText(ui->lineEditPhone->displayText().replace('_', '1'));
    numberEditFinished();
}
#endif

QString SPhone::phone()
{
    return ui->lineEditPhone->displayText();
}

QString SPhone::cleanedPhone()
{
    return m_clearPhone;
}

void SPhone::setPrimary()
{
    m_isPrimary = SPhoneModel::Primary;
    ui->checkBoxPrimary->setChecked(true);
    ui->comboBoxPhoneMask->setCurrentIndex(0);
}

bool SPhone::isValid()
{
    m_isValid = 1;

    if(!permissions->viewClients && m_isReadOnly)  // если не разрешён просмотр клиентов, но модель содержит номер
        return m_isValid;

    if (!ui->lineEditPhone->hasAcceptableInput() && comSettings->isClientPhoneRequired && !m_isReadOnly)
    {
        ui->lineEditPhone->setStyleSheet(commonLineEditStyleSheetRed);
        m_isValid = 0;
    }

    return m_isValid;
}

void SPhone::setButtonVisible(Buttons button, bool state)
{
    switch (button)
    {
        case Buttons::Add   :ui->pushButtonAddPhone->setVisible(state); break;
        case Buttons::Del   :ui->pushButtonDelPhone->setVisible(state); break;
        case Buttons::Edit  : if(!m_phoneModel->isEmpty()) ui->pushButtonEditPhone->setVisible(state); break;
    }
}

void SPhone::updateButtons()
{
    ui->pushButtonAddPhone->setVisible(!m_isReadOnly);
    ui->pushButtonDelPhone->setVisible(!m_isPrimary & !m_isReadOnly);
    ui->pushButtonEditPhone->setVisible(m_isReadOnly & !m_notEditable && permissions->editClients);
}

int SPhone::maskIndex()
{
    return ui->comboBoxPhoneMask->currentIndex();
}

void SPhone::setEditable(const int state)
{
    switch (state)
    {
        case Mode::EditableRW: setReadOnly(0); break;
        case Mode::NotEditable: m_notEditable = 1; Q_FALLTHROUGH();
        case Mode::EditableRO: setReadOnly(1); break;
    }
}

void SPhone::phoneMaskChanged(int index)
{
    QString mask;
    if(index >= 0 || (index == -1 && !m_isReadOnly))
        mask = clientPhoneTypesModel->index(index, 2).data().toString() + ";_";  // Here ";_" for filling blank characters with underscore
    else
        mask = "";

    if(m_isReadOnly)
        ui->lineEditPhone->setReadOnly(false);  // маска не устанавливается в режиме RO, поэтому временно его выключаю
    ui->lineEditPhone->setInputMask("");
    ui->lineEditPhone->setInputMask(mask);
    if(m_isReadOnly)
        ui->lineEditPhone->setReadOnly(true);

    if(!m_isReadOnly)
        m_phoneModel->setMask(index);
}

void SPhone::checkBoxTypeToggled(bool checked)
{
    if(checked)
    {
        setButtonVisible(Buttons::Del, false);
        emit markedPrimary();
    }
}

/*  SLOT
 */
void SPhone::modelUpdated()
{
    ui->checkBoxPrimary->blockSignals(true);

    if(m_phoneModel->type() == SPhoneModel::Primary)
        m_isPrimary = true;
    else
        m_isPrimary = false;
    ui->checkBoxPrimary->setChecked(m_isPrimary);
    setButtonVisible(Buttons::Del, !m_isPrimary);

    ui->checkBoxPrimary->blockSignals(false);
}

void SPhone::buttonDelClicked()
{
    emit delPhone(this);
}

void SPhone::buttonEditClicked()
{
    setReadOnly(!m_isReadOnly);
    updateButtons();
    emit sigEditPhone();
}

void SPhone::numberEdited(QString)
{
    if(!m_isValid)
    {
        ui->lineEditPhone->setStyleSheet(commonLineEditStyleSheet);   // если ранее lineEdit был подсвечен из-за ошибки ввода
        m_isValid = 1;
    }
    if(m_isPrimary)
        emit phoneEdited(ui->lineEditPhone->displayText());
}

void SPhone::numberEditFinished()
{
    if(!m_isReadOnly)
        m_phoneModel->setPhone(ui->lineEditPhone->text());
}

void SPhone::notesEditFinished()
{
    m_phoneModel->setNote(ui->lineEditNotes->text());
}

void SPhone::checkBoxViberClicked(bool state)
{
    m_phoneModel->setMessengers(SPhoneModel::Viber, state);
}

void SPhone::checkBoxTelegramClicked(bool state)
{
    m_phoneModel->setMessengers(SPhoneModel::Telegram, state);
}

void SPhone::checkBoxWhatsappClicked(bool state)
{
    m_phoneModel->setMessengers(SPhoneModel::Whatsapp, state);
}

void SPhone::checkBoxSMSClicked(bool state)
{
    m_phoneModel->setReceiveSMS(state);
}

void SPhone::guiFontChanged()
{
    QFont font;
//    font.setFamily(userLocalData->FontFamily.value);
    font.setPixelSize(userDbData->fontSize);

    ui->lineEditPhone->setFont(font);
    ui->comboBoxPhoneMask->setFont(font);
}

SCheckBox::SCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
}

void SCheckBox::mousePressEvent(QMouseEvent *e)
{
    if(isChecked())
        return;  // режим autoExclusive (убрать галочку кликом нельзя)
    QCheckBox::mousePressEvent(e);
}
