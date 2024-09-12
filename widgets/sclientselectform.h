#ifndef SCLIENTSELECTFORM_H
#define SCLIENTSELECTFORM_H

#include "sclientinputformbase.h"
#include "models/sclientmodel.h"
#include "widgets/slineedit.h"
#include "widgets/sdoublespinbox.h"

namespace Ui {
class SClientSelectForm;
}

class SClientSelectForm : public SClientInputFormBase
{
    Q_OBJECT
public:
    enum AccessMode {Full = 0, SelectClear = 1, Denied = 2};
    explicit SClientSelectForm(QWidget *parent = nullptr);
    ~SClientSelectForm();
    SClientModel *clientModel();
    bool eventFilter(QObject *watched, QEvent *event) override;
    void setClientModel(SClientModel *model);
    void setLabelText(const QString &text);
    void setLineEditText(const QString &text);
    void setIdVisible(bool state = true);
    void setBalanceVisible(bool state = true);
    void setQuickFindEnabled(bool state = true);
    void setLayoutRefWidget(int column, QWidget *widget);
    void setFont(QFont font);
    void update();
    void clearCreds(bool keepCoincidenceShown = true);
    int checkBalanceEnabled();
    void setDefaultStylesheets();
    int checkInput();
    void setHandlingButtons(int mode);
    void setMatchWidgetFilter(int category) override;
#ifdef QT_DEBUG
    void randomFill();
#endif
private:
    Ui::SClientSelectForm *ui;
    SClientModel *m_clientModel = nullptr;
    SLineEdit *m_widgetLastName = nullptr;
    QSpinBox *m_widgetId = nullptr;
    int m_clientId = 0;
    bool m_showId = 1;
    bool m_showBalance = 1;
    int m_lineEditAccessMode = AccessMode::Full;
    QMap<int, QWidget*> m_layoutRefWidgets;
    void setLineEditNameTextAtomic(const QString &text);
public slots:
    void fillCreds(int clientId) override;
private slots:
    void lineEditClientLastNameButtonClickHandler(int);
    void guiFontChanged() override;
};

#endif // SCLIENTSELECTFORM_H
