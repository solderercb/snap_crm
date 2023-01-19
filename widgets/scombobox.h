#ifndef SCOMBOBOX_H
#define SCOMBOBOX_H

#include <QComboBox>
#include <QListView>
#include <QDebug>

class SComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SComboBox(QWidget *parent = nullptr);
    ~SComboBox();
    void disableWheelEvent(bool);
    void wheelEvent(QWheelEvent*);
    bool eventFilter(QObject *watched, QEvent *e);
    void resizeEvent(QResizeEvent*);
    int currentDbId();
    void setEditable(bool editable);
    void showPopup();
    bool isPopupVisible() const;
    void setIsPopupVisible(bool isPopupVisible);
    void toggleIsPopupVisible();
    bool isAutoSetCursorPositionToBegin() const;
    void enableAutoSetCursorPositionToBegin(bool state);

private:
    bool wheelEventOn = 1;
    QAbstractItemView *listViewWidget = nullptr;
    QLineEdit *lineEditWidget = nullptr;
    bool m_isPopupVisible = 0;
    bool m_autoSetCursorPositionToBegin = 1;
    QFontMetrics *fontMetrics;
    bool eventFilterComboBox(QEvent *e);
    bool eventFilterLineEdit(QEvent *e);
    bool eventFilterListView(QEvent *e);
    bool isPointInArea(const QPoint &point,  const QRect &area) const;

signals:
private slots:
    void setCursorPositionToBegin();
};

#endif // SCOMBOBOX_H
