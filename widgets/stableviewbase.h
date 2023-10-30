/*
 *
 */
#ifndef STABLEVIEWBASE_H
#define STABLEVIEWBASE_H

#include <QHeaderView>
#include <QRect>
#include <QScrollBar>
#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QSqlDriver>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include "global.h"
#include "widgets/stableviewgridlayout.h"
#include "stableviewbaseitemdelegates.h"
#include "models/stablebasemodel.h"

typedef struct FilterList FilterList;

struct FilterField
{
    enum Op {Equals = 1<<0, Contains = 1<<1, StartsWith = 1<<2, EndsWith = 1<<3, RegExp = 1<<4, RegularExpression = 1<<5, Wildcard = 1<<6,
             NoOp = 1<<7, Null = 1<<8,
             NotMark = 1<<9 };
    QString column;
    Op operation;
    QVariant value;
    Qt::CaseSensitivity caseSensitivity;
};

struct FilterList
{
    enum Op {And, Or};
    QList<FilterField> fields;
    QList<FilterList> childs;
    bool op = And;
};

class STableViewBase : public QTableView
{
    Q_OBJECT
public:
    enum horizontalHeaderMenuActions{ToggleAutoWidth = 1, FitContent, BestFitAll, SetDefault, Hide, ColumnChooser};
    enum ScrollPos {ScrollPosReset = 0, ScrollPosPreserve = 1};
    enum Selection {SelectionReset = 0, SelectionPreserve = 1};
    explicit STableViewBase(SLocalSettings::SettingsVariant layoutVariant, QWidget *parent = nullptr);
    ~STableViewBase();
    void resizeEvent(QResizeEvent*) override;
    void setModel(QAbstractItemModel *model) override;
    bool eventFilter(QObject *object, QEvent *event) override;
    void setItemDelegate(STableViewBaseItemDelegates *delegate);

    // Часть кода взята из примера https://wiki.qt.io/Sort_and_Filter_a_QSqlQueryModel и доработана
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase::database() );
    void setFilter(const FilterList &filter);
    void filter(const FilterList &filter);
    static FilterField initFilterField(const QString &column, FilterField::Op matchFlag, const QVariant &value, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);
    void setGrouping(const QStringList &grouping);
    void setUniqueIdColumn(int uniqueIdColumn);
    void resetVScrollPos();
    QModelIndexList selectionList();
    void clearSelection();
protected:
    STableBaseModel *m_model = nullptr;
    QFontMetrics *m_fontMetrics;
    SLocalSettings::SettingsVariant m_layoutVariant = SLocalSettings::RepairsGrid;
    XtraSerializer *i_gridLayout;
    QMap<int, int> i_defaultColumnsWidths;
    QList<int> i_defaultMarkedColumns;
    QStringList i_defaultHeaderLabels;
    QMap<int, int> m_autosizedColumns;
    int m_autosizedColumnsSummaryActualWidth = 0;
    int m_autosizedColumnsSummaryDefaultWidth = 0;
    QMenu *horizontalHeaderMenu = nullptr;
    STableViewBaseItemDelegates *i_itemDelegates = nullptr;
    int sizeHintForColumn(int column) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void resizeColumnToContents(int column);
    int columnSizeByContents(int column);
    void resizeColumnsToContents();
    void applyGridlayout();
    void applySorting();
    void initAutosizedColumns();
    void adoptAutosizedColumns();
    virtual void setColumnWidth(int column, int width);
    void setDefaultLayoutParams();
    void setDefaultColumnParams(const int column, const QString &label, const int width);
    void readLayout();
    void initHorizontalHeaderMenu();
    void deleteHorizontalHeaderMenu();
    void resetRowVisibility();
    int calculateVScrollOffset(const int rowScrollBeforeUpdate, const QVariant uniqueId);
    void saveScrollPos(int &vScrollValue, int &hScrollValue, int &topVisibleRow, QVariant &topVisibleRowUniqueId, int &rowCountBeforeUpdate);
    void restoreScrollPos(int &vScrollValue, int &hScrollValue, int &topVisibleRow, QVariant &topVisibleRowUniqueId, int &rowCountBeforeUpdate);
    void saveSelection();
    void restoreSelection();
    bool initHeaders();
private:
    QFile m_layoutSettingsFileName;
    QSqlDatabase m_db;
    QString m_query;
    int m_sortColumn;
    Qt::SortOrder m_sortOrder;
    FilterList *m_filter = nullptr;
    QStringList *m_grouping = nullptr;
    int m_modelColumnsCount = 0;
    int m_uniqueIdColumn = -1;
    QList<QVariant> m_selectionList;
    QTimer *layoutSaveDelay;
    void clearFilter();
    void clearGrouping();
    QString formatFilterGroup(const FilterList &filter);
    QString formatFilterField(const FilterField &field);
    int columnByName(const QString &name);
    void clearSorting();
    int visibleWidth();
public slots:
    void reset() override;
//    void applyLayoutForCategory(const int category);    // это для таблицы товаров, позже будет перенесено в наследующий класс
    virtual void refresh(bool preserveScrollPos = ScrollPosReset, bool preserveSelection = SelectionReset);
protected slots:
    void saveLayout();
    virtual void columnResized(int column, int oldWidth, int newWidth);
    void sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
#if QT_VERSION >= 0x060000
    void dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>()) override;
#else
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>()) override;
#endif
private slots:
    void orderChanged(int logicalIndex, Qt::SortOrder order);
    void horizontalHeaderMenuRequest(const QPoint &pos) const;
    void toggleAutoWidth(bool state);
    void fitContent();
    void bestFitAll();
    void setDefault();
    void hideColumn();
    void showColumnChooser();
};

#endif // STABLEVIEWBASE_H
