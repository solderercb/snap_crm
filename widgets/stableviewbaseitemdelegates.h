#ifndef STABLEVIEWBASEITEMDELEGATES_H
#define STABLEVIEWBASEITEMDELEGATES_H
#include <QApplication>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QStyleOption>
#include <QPainter>
#include <QProgressBar>
#include "../models/stablebasemodel.h"
#include "../global.h"

#define PIXMAP_W 16
#define PIXMAP_H 16
#define PIXMAP_GAP 2
#define PROGRESSBAR_GREEN   "#228B22"
#define PROGRESSBAR_YELLOW  "#FFFF00"
#define PROGRESSBAR_ORANGE  "#FFA500"
#define PROGRESSBAR_RED     "#E44747"

class STableViewBaseItemDelegates : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum PixmapType {RemoveWork = 1, Work, AddPart, RemovePart, Part};
    STableViewBaseItemDelegates(QObject *parent = nullptr);
    STableViewBaseItemDelegates(STableBaseModel *, QObject *parent = nullptr);
    ~STableViewBaseItemDelegates();

    STableBaseModel *i_tableModel;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    bool event(QEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;
    virtual void setTableModel(QAbstractItemModel *model);
    void setFontMetrics(const QFont &font);
protected:
    // for painting progressbar with stylesheet option, a dummy progress bar is required (ref qBittorent project)
    QProgressBar i_dummyGreenProgressBar;
    QProgressBar i_dummyYellowProgressBar;
    QProgressBar i_dummyOrangeProgressBar;
    QProgressBar i_dummyRedProgressBar;
    QString i_dummyProgressBarStyleSheet;
    QFontMetrics *i_fontMetrics = nullptr;
//    QStyle *i_style;
    void initProgressBarsStyles();
    void paintColorizedProgressBar(const QStyleOptionProgressBar *option, QPainter *painter, const QWidget *widget = nullptr) const;
    QLineEdit* createLineEdit(QWidget*, QAbstractItemModel *) const;
    void setLineEditData(QWidget *editor, const QString&) const;
    void setModelDataFromLineEdit(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QComboBox* createComboBox(QWidget*, QAbstractItemModel *) const;
    void setComboBoxData(QWidget *editor, const QString&) const;
    void setModelDataFromComboBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    QSpinBox* createSpinBox(QWidget*, const QModelIndex &) const;
    void setSpinBoxData(QWidget *editor, const int) const;
    void setModelDataFromSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
    QDoubleSpinBox* createDoubleSpinBox(QWidget*, const QModelIndex&) const;
    void setDoubleSpinBoxData(QWidget *editor, const double) const;
    void setModelDataFromDoubleSpinBox(QWidget*, QAbstractItemModel*, const QModelIndex&) const;
private:
    mutable bool m_autoDropDownList;
private slots:
};

#endif // STABLEVIEWBASEITEMDELEGATES_H
