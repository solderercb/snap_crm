#ifndef SCHECKCOMBOBOX_H
#define SCHECKCOMBOBOX_H

#include "qstandarditemmodel.h"
#include "widgets/scombobox.h"
#include <QWidget>
#include <QDebug>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QIdentityProxyModel>
#include <QConcatenateTablesProxyModel>
#include <QListView>
#include <QEvent>
#include <QApplication>

class SCheckComboBoxDataModel : public QConcatenateTablesProxyModel
{
    Q_OBJECT
    friend class SCheckComboBox;
public:
    enum Columns {Name = 0, Id};
    Q_ENUM(Columns)
    explicit SCheckComboBoxDataModel(QObject *parent = nullptr);
    ~SCheckComboBoxDataModel();
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void setSourceModel(QAbstractItemModel* model);
private:
    QAbstractItemModel* m_sourceModel = nullptr;
    QList<QModelIndex> m_checkedItems;
    bool m_showSelectAllRow = 0;
    QStandardItemModel *rowSelectAll = nullptr;
    void setCheckBox(const QModelIndex &index, int state);
    void setCheckBox(const int row, int state);
    void toggleCheckBox(const QModelIndex &index);
    void toggleCheckBox(const int row);
    void setAll(const int state);
    void toggleAll();
    void insertSelectAllRow();
    void removeSelectAllRow();
    void showSelectAllRow(bool state);
    bool isSelectAllRowShown();
};

class SCheckComboBox : public SComboBox
{
    Q_OBJECT
public:
    explicit SCheckComboBox(QWidget *parent = nullptr);
    ~SCheckComboBox();
    void setModel(QAbstractItemModel *model);
    QList<int> checked();
    QList<int> unchecked();
    void setChecked(const QList<int> &list);
    void toggleChecked(const int id);
    void toggleChecked(const QList<int> &list);
    void resetChecked();
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showPopup() override;
    void hidePopup() override;
    void showSelectAllRow(bool state);
private:
    SCheckComboBoxDataModel *m_proxyModel = nullptr;
    bool m_showSelectAllRow = 0;
private slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &botRight, const QVector<int> &roles);
    void rowActivated(int row) override;
};

#endif // SCHECKCOMBOBOX_H
