#ifndef SCLIENTINPUTFORM_H
#define SCLIENTINPUTFORM_H

#include <QWidget>
#include "models/sclientmodel.h"

namespace Ui {
class SClientInputForm;
}

class SClientInputForm : public QWidget
{
    Q_OBJECT
signals:
    void createTabSelectExistingClient(int, QWidget *);
    void createTabClient(int);
public:
    explicit SClientInputForm(QWidget *parent = nullptr);
    ~SClientInputForm();
    void commit();
    void randomFill();
    int checkInput();
    SClientModel* model();
private:
    Ui::SClientInputForm *ui;
    SClientModel *clientModel;
    int m_client = 0;
    void initWidgets();
    void createClient();
    void setDefaultStyleSheets();
public slots:
    void clearClientCreds(bool hideCoincidence = true);
    void fillClientCreds(int);
private slots:
    void buttonSelectExistingClientClicked();
    void buttonCreateTabClientClicked();
    void changeClientType();
    void primaryPhoneEdited(QString);
};

#endif // SCLIENTINPUTFORM_H
