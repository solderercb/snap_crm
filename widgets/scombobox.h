#ifndef SCOMBOBOX_H
#define SCOMBOBOX_H

#include <QCoreApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QEvent>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QDebug>
#include "slineedit.h"

class SComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString Buttons READ buttons WRITE setButtons NOTIFY buttonsChanged)
    Q_PROPERTY(bool selectOnTab READ isSelectOnTab WRITE setSelectOnTab)    // выбор выделенного элемента выпадающего списка по Tab
    Q_PROPERTY(bool keepCustomText READ isKeepCustomText WRITE setKeepCustomText)   // сохранить введённый текст при при нажатии Tab/Enter
    Q_PROPERTY(bool keepListShown READ isKeepListShown WRITE setKeepListShown)   // сохранить введённый текст при при нажатии Tab/Enter
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
    virtual void showPopup() override;
    virtual void hidePopup() override;
    QString buttons();
    void setButtons(const QString &buttons);
    void setPlaceholderText(const QString &text);
    virtual void updatePopupGeometry();
    void startPopupShowIntervalTimer();
    void setPopupShowIntervalObserved();
    void setListRowHeight(const int height);
    void setRowHidden(const int row, const bool state = true);
    void hideRow(const int row);
    void setFont(const QFont &font);
    void applyGuiSettings();
protected:
    QAbstractItemView *listViewWidget = nullptr;
    QFontMetrics *fontMetrics;
    SLineEdit *lineEditWidget = nullptr;
    QWidget *popupWidget = nullptr;
    virtual void retranslateKey(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text = QString(), bool autorep = false, ushort count = 1);
    bool isPreserveLineEditFocus() const;
    void setPreserveLineEditFocus(bool state);
    bool isPopupVisible() const;
    void setIsPopupVisible(bool state);
    bool ignorePopupHide() const;
    void setIgnorePopupHide(bool state);
    void setRetranslatedKeyReceiver(QWidget *widget);
    virtual bool eventFilterComboBox(QEvent *e);
    virtual bool eventFilterLineEdit(QEvent *e);
    virtual bool eventFilterListView(QEvent *e);
    bool isKeepListShown();
    void setKeepListShown(const bool state);
private:
    bool wheelEventOn = 1;
    bool m_isPopupVisible = 0;
    QSize szHint;
    QSize minSzHint;
    bool m_preserveLineEditFocus = 0;
    uchar m_customInput = 0;
    bool m_lineEditRO = 1;
    bool m_selectOnTab = 1;
    bool m_keepCustomText = 1;
    bool m_keepListShown = 0;
    bool m_ignorePopupHide = 0;
    QWidget *m_retranslatedKeyReceiver = nullptr;
    bool m_popupShowIntervalObserved = 1;
    int m_listRowHeight = 15;
    bool isPointInArea(const QPoint &point,  const QRect &area) const;
    bool isSelectOnTab();
    void setSelectOnTab(const bool state);
    bool isKeepCustomText();
    void setKeepCustomText(const bool state);
    void applyListViewRowHeight();
protected slots:
    virtual void rowActivated(int);
private slots:
    void longTextHandler();
    void clearButtonPress(int);
};

#endif // SCOMBOBOX_H
