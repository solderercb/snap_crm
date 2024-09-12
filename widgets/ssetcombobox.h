#ifndef SSETCOMBOBOX_H
#define SSETCOMBOBOX_H

//#define CUSTOM_COMPLETER
#define MAJOR 0
#define MINOR 3
#define PATCH 1
#define COMMIT 50

#include <QCoreApplication>
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QCompleter>
#include <QEvent>
#include <QFocusEvent>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QToolButton>
#include <QKeyEvent>
#include <QRegularExpression>
#include "models/ssortfilterproxymodel.h"
#include "widgets/scombobox.h"
//#include <QDebug>
//#include <QTest>
//#include <QListView>

class daughterLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit daughterLineEdit(QWidget *parent = nullptr);
    ~daughterLineEdit();
    void enableDeleteButton(bool enable=true);
    void setMaximumHeight(int);
    void setMaximumWidth(int);
    void resize(const QSize &);
    void resize(int, int);
    QSize sizeHint() const override;
    void setText(const QString &);

signals:
    void buttonClicked(daughterLineEdit*);
    void textChanged(); // это перегруженный сигнал. вызывается в конце функции изменения размера под текст для того, чтобы родительский LineEdit перестроил дочерние

protected:
    void resizeEvent(QResizeEvent *) override;

public slots:
    void home();
    void resizeToText(const QString &text);
    void resizeToText();

private slots:
    void deleteButtonClicked();

private:
    bool eventFilter(QObject *watched, QEvent *event) override;
    bool deleteButtonEnabled = 0;
    void updateTextMargins();
    QSize sz;
    QFontMetrics *fm;
    int frameWidth;
    QSize buttonSize;
    QToolButton *deleteButton;
};

class SSetComboBox : public SComboBox
{
    Q_OBJECT

signals:
    void daughterLineEditAdded();   // для теста

public:
    explicit SSetComboBox(QWidget *parent = nullptr);
    ~SSetComboBox();
    void setModel(QAbstractItemModel*);
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
    void clearEditText();
    void setSeparator(QString);
    void setSeparator(char);
    void setSizePolicy(QSizePolicy);
    void setSizePolicy(QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical);
    QVector<daughterLineEdit*> daughterLineEdits;
    void deleteDaughterLineEditOnKeyPress(int);
    QString version();
    void showPopup() override;
    void hidePopup() override;
#ifdef QT_DEBUG
    void addRandomItem();
#endif

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *) override;
private:
    enum resizeEventTrigger{MinimumHeightUpdated = 1};
    SSortFilterProxyModel *proxyModel;
    QCompleter *p_completer;
    int daughterLineEditFrameSize;
    int daughterLineEditHeight;
    int daughterLineEditFrameHeight;
    int defaultComboBoxHeight;
    int parentLineEditFrameSize;
    QMargins currentDaughterLineEditPosition;
    QString field_separator;
    daughterLineEdit *dLineEdit;
    int eventTrigger = 0;
    QString semicolon_separated_text;
    int rearrangeDaughterLineEdits(int);
    bool eventFilterListView(QEvent *e) override;
public slots:
    void setCurrentIndex(int index);
    void setStyleSheet(const QString& styleSheet);
private slots:
    void comboBoxSetFocus();
    void addItem(const QString &text);
    void rowActivated(int) override;
    void updateProxyModelFilter(const QString &text);
    void daughterLineEditTextChanged();
    void deleteDaughterLineEdit(daughterLineEdit *widget);
    void parentLineEditEditingFinished();
};

#endif // SSETCOMBOBOX_H
