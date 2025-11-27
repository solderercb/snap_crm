#ifndef SCLIENTINPUTFORM_H
#define SCLIENTINPUTFORM_H

#include <SClientInputFormBase>

class SClientModel;

namespace Ui {
class SClientInputForm;
}

class SClientInputForm : public SClientInputFormBase
{
    Q_OBJECT
public:
    explicit SClientInputForm(QWidget *parent = nullptr);
    ~SClientInputForm();
    void commit();
    int checkInput();
    SClientModel* model();
    void setMatchWidgetFilter(int category) override;
#ifdef QT_DEBUG
    void randomFill();
#endif
private:
    Ui::SClientInputForm *ui;
    SClientModel *clientModel;
    int m_client = 0;
    void initWidgets();
    void createClient();
    void setDefaultStyleSheets();
public slots:
    void clearClientCreds(bool keepCoincidenceShown = true);
    void fillClientCreds(int);
    void fillCreds(int) override;
private slots:
    void buttonSelectExistingClientClicked();
    void buttonCreateTabClientClicked();
    void changeClientType();
    void lastNameChanged(const QString &text);
    void firstNameChanged(const QString &text);
    void primaryPhoneEdited(QString);
    void guiFontChanged();
};

#endif // SCLIENTINPUTFORM_H
