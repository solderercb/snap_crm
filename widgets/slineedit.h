#ifndef SLINEEDIT_H
#define SLINEEDIT_H

#include <QLineEdit>
#include <QStringList>

class QFontMetrics;
class QToolButton;
class QSignalMapper;

class SLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString buttons READ buttons WRITE setButtons)

signals:
    void buttonClicked(int buttonId = 0);
    void mouseDoubleClick();
    void keyPress(QKeyEvent *event);

public:
    enum Buttons {Clear, DownArrow, Edit, Search, Print, Apply, Open, Add};
    Q_ENUM(Buttons);
    explicit SLineEdit(QWidget *parent = nullptr);
    ~SLineEdit();
    QString buttons();
    void setButtons(QString buttons);
    void clearButtons();
    void arrangeButtons();
    void resize(const QSize &);
    void resize(int, int);
    void enableExtKeyPressHandler(bool);
    void setText(const QString &text);
    bool isAutoSetCursorPositionToBegin() const;
    void enableAutoSetCursorPositionToBegin(bool state);
private:
    QStringList allowedButtonsList = { "Clear", "DownArrow", "Edit", "Search", "Print", "Apply", "Open", "Add" };
//    QList<QString> buttonIconList = { "🗙", "▼", "🖉", "🔍", "🖶", "🗸", "🗁" };
    // другие варианты unicode иконок кнопок: ✖ или 🗙 или ⌫, ▼, 🖉, 🔍, 🖶 или 🖨, 🗸, 🗁 или 📂
    QList<QString> buttonIconList = { "1F5D9_32.png", "25BC_32.png", "1F589_32.png", "1F50D_32.png", "1F5A8_32.png", "1F5F8_32.png", "1F4C2_32.png", "2795_32.png" };
    QFontMetrics *fontMetrics;
    QStringList *buttonsList;
    int buttonsCount = 0;
    QVector<QToolButton*> lineEditButtons;
    QSignalMapper *signalMapper;
    int frameWidth;
    QSize buttonSize;
    bool m_extKeyPressHandler = 0;
    bool m_autoSetCursorPositionToBegin = 1;
    int m_lastCursorPosition = 0;
protected:
    void resizeEvent(QResizeEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *) override;
    void focusOutEvent(QFocusEvent *e) override;
private slots:
};


#endif // SLINEEDIT_H
