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
#include "widgets/qtooltipper.h"

typedef struct FilterList FilterList;

struct FilterField
{
    enum Op {NotMark=1, // values representing operation must be even
             Equals=2, Contains=4, StartsWith=6, EndsWith=8, RegExp=10, RegularExpression=12, Wildcard=14, More=16, MoreEq=18, Less=20, LessEq=22,
             NoOp=24, Null=26, InSet=28 };
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
    bool Not = 0;
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
    virtual void setModel(QAbstractItemModel *model) override;
    bool eventFilter(QObject *object, QEvent *event) override;
    void setItemDelegate(STableViewBaseItemDelegates *delegate);

    // Часть кода взята из примера https://wiki.qt.io/Sort_and_Filter_a_QSqlQueryModel и доработана
    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase::database() );
    void setFilter(const FilterList &filter);
    void filter(const FilterList &filter);
    static FilterField initFilterField(const QString &column, FilterField::Op matchFlag, const QVariant &value, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);
    static FilterField initFilterField(const QString &column, int matchFlag, const QVariant &value, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);
    void setGrouping(const QStringList &grouping);
    void setUniqueIdColumn(int uniqueIdColumn);
    void resetVScrollPos();
    QModelIndexList selectionList();
    void clearSelection();
    void setLayoutVariant(const SLocalSettings::SettingsVariant &layoutVariant);
    void enableAutorefresh(const int msec);
    virtual void delayedRefresh(const int msec);
    virtual void refreshPending();
    virtual void refresh(bool preserveScrollPos = ScrollPosReset, bool preserveSelection = SelectionReset);
    void restartAutorefreshTimer();
    Qt::SortOrder sortOrder();
    int sortSection();
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint) override;
    void commitData(QWidget *editor) override;
protected:
    QSqlQueryModel *m_model = nullptr;
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
    int m_sortColumn;
    Qt::SortOrder m_sortOrder;
    int i_vspValue = 0; // vsp and hsp — Vertical Scroll Position and Horisontal Scroll Position
    int i_hspValue = 0;
    int i_vspTopVisibleRow;
    QVariant i_vspTopVisibleRowUniqueId;
    int i_vspOldRowCount;
    QTimer *m_autorefreshTimer = nullptr;
    bool m_delayedRefreshPending = 0;
    int sizeHintForColumn(int column) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void resizeColumnToContents(int column);
    int columnSizeByContents(int column);
    void resizeColumnsToContents();
    void applyGridlayout();
    void initSorting();
    void initAutosizedColumns();
    void adoptAutosizedColumns();
    virtual void setColumnWidth(int column, int width);
    virtual void setDefaultLayoutParams();
    void setColumnLayoutHidden(const int column, const bool state);
    void setDefaultColumnParams(const int column, const QString &label, const int width);
    void readLayout();
    void initHorizontalHeaderMenu();
    void deleteHorizontalHeaderMenu();
    void resetRowVisibility();
    void vScrollCorrection();
    void saveScrollPos();
    virtual void clearVScrollPos();
    virtual void restoreVScrollPos();
    virtual void restoreHScrollPos();
    void saveSelection();
    bool hasSavedSelection();
    void restoreSelection();
    bool initHeaders();
    void setDatabase(const QSqlDatabase &database);
    QString formatFilterGroup(const FilterList &filter);
    QString formatFilterField(const FilterField &field);
    virtual void clearModel();
    virtual void setModelQuery(const QString &query, const QSqlDatabase &database);
    virtual void fetchMore(const QModelIndex &parent);
    virtual void layoutChanged(int,int,int);
    void undoToggleSortIndicator();
    virtual void setSorting(const int logicalIndex, const Qt::SortOrder order);
    virtual void applySorting();
    void keyPressEvent(QKeyEvent *event) override;
    virtual void selectionToArray(const QModelIndexList &selection, QMap<int, QMap<int, QModelIndex>> &array);
    void copyToClipboard(QMap<int, QMap<int, QModelIndex>> &items) const;
private:
    QFile m_layoutSettingsFileName;
    QSqlDatabase m_db;
    QString m_constQuery;
    QString m_query;
    QString m_queryConditions;
    bool m_queryConditionsChanged;
    QStringList *m_grouping = nullptr;
    int m_uniqueIdColumn = -1;
    QList<QVariant> m_selectionList;
    int m_currentIndexRow = -1;
    int m_currentIndexColumn = -1;
    QTimer *m_layoutSaveTimer = nullptr;
    int m_autorefreshTimeout = 0;
    int m_restoreSelectionTrig = 0;
    void clearFilter();
    void clearGrouping();
    int columnByName(const QString &name);
    int visibleWidth();
    void createAutorefreshTimer();
    void deleteAutorefreshTimer();
    void prepareQuery();
public slots:
    void reset() override;
//    void applyLayoutForCategory(const int category);    // это для таблицы товаров, позже будет перенесено в наследующий класс
    virtual void applyGuiSettings();
    void hideColumn(const int col);
    void toggleOrder(int logicalIndex);
protected slots:
    void saveLayout();
    virtual void columnResized(int column, int oldWidth, int newWidth);
    void sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
#if QT_VERSION >= 0x060000
    void dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>()) override;
#else
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>()) override;
#endif
    void verticalScrollbarValueChanged(int value) override;
    void horizontalScrollbarValueChanged(int value) override;
    virtual void vsp_rangeChanged(const int min, const int max);
    virtual void hsp_rangeChanged(const int min, const int max);
    virtual void horizontalHeaderSectionClicked(const int logicalIndex);
    void cutToClipboard();
    void copyToClipboard();
    void pasteFromClipboard();
private slots:
    void autorefreshTable();
    void horizontalHeaderMenuRequest(const QPoint &pos) const;
    void toggleAutoWidth(bool state);
    void fitContent();
    void bestFitAll();
    void setDefault();
    void hideColumn();
    void showColumnChooser();
};

#endif // STABLEVIEWBASE_H
