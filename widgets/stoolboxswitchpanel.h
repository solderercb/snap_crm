#ifndef STOOLBOXSWITCHPANEL_H
#define STOOLBOXSWITCHPANEL_H

#include <QWidget>
#include <QIcon>

class QCommandLinkButton;

namespace Ui {
class SToolboxSwitchPanel;
}

class SToolboxSwitchPanel : public QWidget
{
    Q_OBJECT
signals:
    void buttonToggled(int);
public:
    explicit SToolboxSwitchPanel(QWidget *parent = nullptr);
    ~SToolboxSwitchPanel();
    void redraw();
    void addButton(int buttonId = 0);
    void addButton(const QString &text, const QIcon &icon = QIcon(), int buttonId = 0);
    void appendButton(QCommandLinkButton*);
    void deleteButton(int);
    void clearButtons();
    void hideButton(int buttonId);
    void showButton(int buttonId);
    void setButtonVisible(int buttonId, bool state = 1);
    void setButtonText(int buttonId, const QString text);
    void setIcon(int buttonId, QIcon icon);

private:
    Ui::SToolboxSwitchPanel *ui;
    QMap<int, QCommandLinkButton*> m_buttons;
    int m_currentIndex = 0;
private slots:
    void slotButtonToggled(bool);
};

#endif // STOOLBOXSWITCHPANEL_H
