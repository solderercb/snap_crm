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
    bool isPopupVisible() const;
    void setIsPopupVisible(bool isPopupVisible);
    void toggleIsPopupVisible();
protected:
    QAbstractItemView *listViewWidget = nullptr;
    QLineEdit *lineEditWidget = nullptr;
    QFontMetrics *fontMetrics;
    bool ignoreFocusOut = 0;
    virtual void retranslateKey(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text = QString(), bool autorep = false, ushort count = 1);
private:
    bool wheelEventOn = 1;
    bool m_isPopupVisible = 0;
    QSize szHint;
    QSize minSzHint;
    bool eventFilterComboBox(QEvent *e);
    bool eventFilterLineEdit(QEvent *e);
    bool eventFilterListView(QEvent *e);
    bool isPointInArea(const QPoint &point,  const QRect &area) const;

signals:
private slots:
    void longTextHandler();
};

#endif // SCOMBOBOX_H
