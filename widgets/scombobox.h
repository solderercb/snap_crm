#ifndef SCOMBOBOX_H
#define SCOMBOBOX_H

#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QEvent>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QDebug>
#include "slineedit.h"

class SComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString Buttons READ buttons WRITE setButtons NOTIFY buttonsChanged)
    Q_PROPERTY(bool selectOnTab READ isSelectOnTab WRITE setSelectOnTab)    // выбор выделенного элемента выпадающего списка по Tab
    Q_PROPERTY(bool keepCustomText READ isKeepCustomText WRITE setKeepCustomText)   // сохранить введённый текст при при нажатии Tab/Enter
signals:
    void buttonClicked(int buttonId = 0);
    void buttonsChanged();
public:
    explicit SComboBox(QWidget *parent = nullptr);
    ~SComboBox();
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void disableWheelEvent(bool);
    void wheelEvent(QWheelEvent*) override;
    bool eventFilter(QObject *watched, QEvent *e) override;
    void resizeEvent(QResizeEvent*) override;
    int currentDbId();
    void initLineEdit();
    void setEditable(bool editable);
    void showPopup() override;
    void hidePopup() override;
    QString buttons();
    void setButtons(const QString &buttons);
    void setPlaceholderText(const QString &text);
protected:
    QAbstractItemView *listViewWidget = nullptr;
    QFontMetrics *fontMetrics;
    SLineEdit *lineEditWidget = nullptr;
    virtual void retranslateKey(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text = QString(), bool autorep = false, ushort count = 1);
    bool ignoreFocusOut() const;
    void setIgnoreFocusOut(bool state);
    bool considerCursorPosOnHide() const;
    void setConsiderCursorPosOnHide(bool state);
    bool isPopupVisible() const;
    void setIsPopupVisible(bool state);
    void toggleIsPopupVisible();
private:
    bool wheelEventOn = 1;
    bool m_isPopupVisible = 0;
    QSize szHint;
    QSize minSzHint;
    bool m_ignoreFocusOut = 0;
    bool m_considerCursorPosOnHide = 1;
    uchar m_customInput = 0;
    bool m_lineEditRO = 1;
    bool m_selectOnTab = 1;
    bool m_keepCustomText = 1;
    bool eventFilterComboBox(QEvent *e);
    bool eventFilterLineEdit(QEvent *e);
    bool eventFilterListView(QEvent *e);
    bool isPointInArea(const QPoint &point,  const QRect &area) const;
    bool isSelectOnTab();
    void setSelectOnTab(const bool state);
    bool isKeepCustomText();
    void setKeepCustomText(const bool state);
private slots:
    void longTextHandler();
    void clearButtonPress(int);
};

#endif // SCOMBOBOX_H
