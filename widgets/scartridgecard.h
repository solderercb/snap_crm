#ifndef SCARTRIDGECARD_H
#define SCARTRIDGECARD_H

#include <QWidget>
#include "models/scartridgecardmodel.h"
#include "models/scartridgematerialmodel.h"
#include "models/scartridgematerialsmodel.h"
#include "widgets/smodalwidget.h"
#include "widgets/stableviewbase.h"
#include "widgets/scartridgematerialstableitemdelegates.h"
#include "widgets/shortlivednotification.h"

namespace Ui {
class SCartridgeCard;
}

class materialsTable : public STableViewBase
{
    Q_OBJECT
    friend class SCartridgeCard;
signals:
    void tonerWeightChanged(int weight);
public:
    enum Column{ID = 0, Type = 1, Count = 2, Price = 3, PriceWork = 4, Summ = 5, Name = 6, Articul = 7, SalarySumm = 8};
    Q_ENUM(Column)
    enum ColumnWidth{ID_w = 80, Type_w = 80, Count_w = 80, Price_w = 80, PriceWork_w = 80, Summ_w = 80, Name_w = 160, Articul_w = 80, SalarySumm_w = 80};
    Q_ENUM(ColumnWidth)
    explicit materialsTable(QWidget *parent = nullptr);
    ~materialsTable();
    void setModel(QAbstractItemModel *model) override;
    void mouseClickEvent(QMouseEvent *event);
    void clearModel() override;
    void setModelQuery(const QString &query, const QSqlDatabase &database) override;
private:
    SCartridgeMaterialsModel *m_model = nullptr;
    int m_cardId;
    void translateNames();
private slots:
#if QT_VERSION >= 0x060000
    void dataChanged(const QModelIndex&, const QModelIndex&, const QList<int> &roles = QList<int>()) override;
#else
    void dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &roles = QVector<int>()) override;
#endif
    void horizontalHeaderSectionClicked(const int logicalIndex) override;
};

class SCartridgeCard : public SModalWidget
{
    Q_OBJECT
signals:
    void newCardCreated(int id);
    void cardModified(int id);
public:
    enum Color {Black = 0, Cyan, Magenta, Yellow};
    Q_ENUM(Color)
    explicit SCartridgeCard(Qt::WindowFlags flags, QWidget *parent = nullptr);
    explicit SCartridgeCard(const int id, const int vendorIndex, Qt::WindowFlags flags, QWidget *parent = nullptr);
    ~SCartridgeCard();
    void load(const int id);
    void setMaterialsTableFilter(const int id);
    void initModels();
    void configureWidgets();
    void updateWidgets();
    static SStandardItemModel *colorsList();
    bool checkInput();
private:
    Ui::SCartridgeCard *ui;
    int m_id = 0;
    int m_deviceClassId = 0;
    int m_vendorIndex = -1;
    QString m_name;
    int m_fullWeight = 0;
    int m_tonerWeight = 0;
    int m_resource = 0;
    QString m_notes;
    QByteArray *m_photo = nullptr;
    int m_color = 0;
    bool m_isArchive = 0;
    bool m_modelRW = 1;
    SCartridgeCardModel *m_cardModel = nullptr;
    SCartridgeMaterialsModel *m_materialsModel = nullptr;
    SSqlQueryModel* m_vendorsModel = nullptr;
    SStandardItemModel *m_cartridgeColors = nullptr;
    void setModelData();
    QString cartridgeName();
    void translateNames();
private slots:
    void materialSelected(const QModelIndex &index);
    void removeMaterial();
    void setTonerWeight(const int weight);
    bool commit();
    void closeForm();
    void sortMaterials(const int column);
};

#endif // SCARTRIDGECARD_H
