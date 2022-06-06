#ifndef SSETCOMBOBOX_H
#define SSETCOMBOBOX_H

//#define CUSTOM_COMPLETER
#define MAJOR 0
#define MINOR 2
#define PATCH 0
#define COMMIT 46

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCompleter>
#include <QAbstractItemView>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QSortFilterProxyModel>
#include <QStyle>
#include <QToolButton>
#include <QRegularExpression>

class daughterLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    daughterLineEdit(QWidget *parent = nullptr);
    void enableDeleteButton(bool enable=true);
    void setMaximumHeight(int);
    void setMaximumWidth(int);
    void resize(const QSize &);
    void resize(int, int);
    QSize sizeHint();
    ~daughterLineEdit();

signals:
    void buttonClicked(daughterLineEdit*);
    void textChanged(); // это перегруженный сигнал. вызывается в конце функции изменения размера под текст для того, чтобы родительский LineEdit перестроил дочерние

protected:
    void resizeEvent(QResizeEvent *);

public slots:
    void home();
    void resizeToText();

private slots:
    void deleteButtonClicked();

private:
    bool eventFilter(QObject *watched, QEvent *event);
    bool deleteButtonEnabled = 0;
    void updateTextMargins();
    QSize sz;
    QFontMetrics *fm;
    int frameWidth;
    QSize buttonSize;
    QToolButton *deleteButton;
    QIcon *buttonIcon;
};

/* LineEdit для ComboBox'а. Не охота его выносить в отдельные файлы, т. к. единственная причина его переопределения заключается
 * в отображении выпадающего списка при щелчке мыши */
class CBLineEdit : public QLineEdit
{
    Q_OBJECT

signals:
    void mouseButtonPress();

public:
    explicit CBLineEdit(QWidget *parent = nullptr);
    ~CBLineEdit();

private:
    void resizeEvent(QResizeEvent *);

//protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

};

class viewEventFilter : public QObject
{
    Q_OBJECT
signals:

public:
    viewEventFilter(QObject*);
private:
protected:
    bool eventFilter(QObject*, QEvent*) override;
};

class SSetComboBox : public QComboBox
{
    Q_OBJECT

signals:
    void textChanged(QString);
    void activatedDerived(int);
    void daughterLineEditAdded();

public:
    explicit SSetComboBox(QWidget *parent = nullptr);
    ~SSetComboBox();
    void setModel(QAbstractItemModel*);
    void setEditable(bool);
    int hasHeightForWidth();
    QSize sizeHint();
    QSize minimumSizeHint();
    void setGeometry(int, int, int, int);
    void setGeometry(const QRect &);
    void setRowHeight(int);
    void setMinimumSize(const QSize &);
    void setMinimumHeight(int);
    void resize(const QSize &);
    void resize(int, int);
    void updateLineEditGeometry();
    QString separator();
    QString text();
    QString currentText();
    void setSeparator(QString);
    void setSeparator(char);
    void setSizePolicy(QSizePolicy);
    void setSizePolicy(QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical);
    int isPopupShown;
    QVector<daughterLineEdit*> daughterLineEdits;
    void deleteDaughterLineEditOnKeyPress(int);
    QString version();
    void clearEditText();

public slots:

private:
    enum resizeEventTrigger{
        minimumHeightUpdated = 1
    };

    bool eventFilter(QObject *watched, QEvent *event);
    int rearrangeDaughterLineEdits(int);
    QSortFilterProxyModel *proxyModel;
    QCompleter *p_completer;
    QFontMetrics *fontMetrics;
    int daughterLineEditFrameSize;
    int daughterLineEditHeight;
    int daughterLineEditFrameHeight;
    int defaultComboBoxHeight;
    viewEventFilter *viewEventFilterObj;
    int parentLineEditFrameSize;
    QMargins currentDaughterLineEditPosition;
    QString field_separator;
    daughterLineEdit *dLineEdit;
    int eventTrigger = 0;
    QString semicolon_separated_text;

private slots:
    void comboBoxSetFocus();
    void addItem(const QString &text);
    void activatedHandler(int);
    void updateProxyModelFilter(const QString &text);
    void daughterLineEditTextChanged();
    void deleteDaughterLineEdit(daughterLineEdit *widget);
    void parentLineEditEditingFinished();
    void parentLineEditFocused();
    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
};

#endif // SSETCOMBOBOX_H
