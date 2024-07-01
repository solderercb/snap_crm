#ifndef STABLEVIEWBOQITEMDELEGATES_H
#define STABLEVIEWBOQITEMDELEGATES_H
#include <QApplication>
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMouseEvent>
#include <QAbstractItemView>
#include "stableviewbaseitemdelegates.h"
#include "../models/ssaletablemodel.h"

#define PIXMAP_W 16
#define PIXMAP_H 16
#define PIXMAP_GAP 2

class STableViewBOQItemDelegates : public STableViewBaseItemDelegates
{
    Q_OBJECT
signals:
    void addItem();
public:
    enum PixmapType {RemoveWork = 1, Work, AddPart, RemovePart, Part};
    STableViewBOQItemDelegates(QObject *parent = nullptr);
    STableViewBOQItemDelegates(SSaleTableModel*, QObject *parent = nullptr);
    ~STableViewBOQItemDelegates();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void setTableModel(QAbstractItemModel*) override;
private:
    SSaleTableModel *m_tableModel = nullptr;
    mutable bool m_must_open_box;
    QSpinBox* createSpinBox(QWidget*, const QModelIndex &) const;
    QDoubleSpinBox* createDoubleSpinBox(QWidget*, const QModelIndex&) const;
    QRect pixmapRect(const QRect &delegateRect, const PixmapType p) const;
    void drawPixmap(const QRect &delegateRect, PixmapType p, QPainter *painter) const;
    int rowConditionsForPixmap(const QModelIndex&) const;
    void buttonHandler(const int buttonNum, const int row);
};

#endif // STABLEVIEWBOQITEMDELEGATES_H
