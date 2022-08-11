#include "sphone.h"
#include "ui_sphone.h"

SPhone::SPhone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPhone)
{
    ui->setupUi(this);

    ui->comboBoxPhoneMask->setStyleSheet(commonComboBoxStyleSheet);
    ui->lineEditPhone->setStyleSheet(commonLineEditStyleSheet);

    ui->comboBoxPhoneMask->setModel(clientPhoneTypesModel);
    ui->comboBoxPhoneMask->setModelColumn(0);
    ui->comboBoxPhoneMask->setCurrentIndex(-1);

    connect(ui->comboBoxPhoneMask,SIGNAL(currentIndexChanged(int)),this,SLOT(phoneMaskChanged(int)));
    connect(ui->checkBoxPrimary,SIGNAL(toggled(bool)), this,SLOT(checkBoxTypeToggled(bool)));
    connect(ui->pushButtonAddPhone,SIGNAL(clicked()),this,SIGNAL(addPhone()));
    connect(ui->pushButtonDelPhone,SIGNAL(clicked()),this,SLOT(delPhone()));
    connect(ui->lineEditPhone,SIGNAL(textEdited(QString)),this,SLOT(textChanged(QString)));
    connect(ui->lineEditPhone,SIGNAL(editingFinished()),this,SLOT());
    connect(ui->lineEditNotes,SIGNAL(editingFinished()),this,SLOT());
//    ui->pushButtonAddPhone->hide();
    ui->pushButtonDelPhone->hide();
    ui->pushButtonEditPhone->hide();
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

void SPhone::setModel(SPhoneModel *model)
{
    connect(model,SIGNAL(typeChanged(int)),this,SLOT(setCheckBoxPrimaryState(int)));

    m_phoneModel = model;
    ui->comboBoxPhoneMask->setCurrentIndex(model->maskIndex());
    ui->lineEditPhone->setText(model->phone());

    m_isPrimary = model->type();
    if(m_isPrimary)
    {
        ui->checkBoxPrimary->setChecked(true);
    }
    if(!model->isEmpty())
        setReadOnly();
}

void SPhone::setReadOnly(bool state)
{
    ui->lineEditPhone->setReadOnly(state);
    ui->lineEditNotes->setReadOnly(state);
    ui->comboBoxPhoneMask->setEnabled(!state);
    ui->checkBoxPrimary->setEnabled(!state);
    ui->checkBoxViber->setEnabled(!state);
    ui->checkBoxTelegram->setEnabled(!state);
    ui->checkBoxWhatsapp->setEnabled(!state);
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

void SPhone::fillPhone(const QSqlRecord &record)
{
    // TODO: придумать механизм сокрытия части номера, если у пользователя недостаточно прав
    ui->comboBoxPhoneMask->setCurrentText(clientPhoneTypesModel->getDisplayRole(record.value(1).toInt(), 1));    // сначала устанавливаем тип в комбобоксе, чтобы применилась маска к полю
    ui->lineEditPhone->setText(record.value(0).toString());    // теперь устанавливаем номер телефона
}

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
    if (!ui->lineEditPhone->hasAcceptableInput() && comSettings->value("phone_required").toBool())   // если не указано отчество и оно обязятельно (только для физ. лиц)
    {
        ui->lineEditPhone->setStyleSheet(commonLineEditStyleSheetRed);
        return 0;
    }
    return 1;
}

void SPhone::phoneMaskChanged(int index)
{
    m_mask = clientPhoneTypesModel->index(index, 2).data().toString();
    ui->lineEditPhone->setInputMask("");
    ui->lineEditPhone->setInputMask(m_mask + ";_");  // Here ";_" for filling blank characters with underscore
//    m_visibleMask = m_mask.replace(QRegularExpression("[09]"), "_");   // в маске телефона меняем 0 и 9 на подчеркивание; 0 и 9 — это специальные маскировочные символы (см. справку QLineEdit, inputMask)
    m_visibleMask = ui->lineEditPhone->displayText(); // второй вариант получения видимой маски
    if(m_isPrimary)
        emit maskChanged(index);
}

void SPhone::checkBoxTypeToggled(bool checked)
{
    if(checked)
        emit markedPrimary();
}

/*  SLOT: Программная установка типа
 */
void SPhone::setCheckBoxPrimaryState(int type)
{
    ui->checkBoxPrimary->blockSignals(true);
    if(type == SPhoneModel::Primary)
        ui->checkBoxPrimary->setChecked(true);
    else
    {
        ui->checkBoxPrimary->setChecked(false);
    }
    ui->checkBoxPrimary->blockSignals(false);
}

void SPhone::delPhone()
{

}

void SPhone::textChanged(QString text)
{
    ui->lineEditPhone->setStyleSheet(commonLineEditStyleSheet);   // если ранее lineEdit был подсвечен из-за ошибки ввода
    if(m_isPrimary)
        emit inputUpdated(ui->lineEditPhone->displayText());
}

void SPhone::phoneEditFinished()
{
    m_phoneModel->setPhone(ui->lineEditPhone->text());
}

void SPhone::notesEditFinished()
{
    m_phoneModel->setNote(ui->lineEditNotes->text());
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
