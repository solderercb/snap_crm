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

class SComboBox : public QComboBox
{
    Q_OBJECT
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
    void setEditable(bool editable);
    void showPopup() override;
    void hidePopup() override;
protected:
    QAbstractItemView *listViewWidget = nullptr;
    QLineEdit *lineEditWidget = nullptr;
    QFontMetrics *fontMetrics;
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
    bool eventFilterComboBox(QEvent *e);
    bool eventFilterLineEdit(QEvent *e);
    bool eventFilterListView(QEvent *e);
    bool isPointInArea(const QPoint &point,  const QRect &area) const;

signals:
private slots:
    void longTextHandler();
};

#endif // SCOMBOBOX_H
