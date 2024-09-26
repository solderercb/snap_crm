#ifndef STABLEVIEWSUMMARYBASE_H
#define STABLEVIEWSUMMARYBASE_H

#include "stableviewbase.h"
#include <QObject>
#include <QWidget>

class STableViewSummaryBase : public STableViewBase
{
    Q_OBJECT
public:
    explicit STableViewSummaryBase(SLocalSettings::SettingsVariant layoutVariant, QWidget *parent = nullptr);
    ~STableViewSummaryBase();
    virtual void setGridLayout(XtraSerializer* layout);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void setTotal(int column, double value, int decimals = comSettings->classicKassa?2:0);
    void setTotal(int column, QVariant value);
    void setData(QModelIndex index, QVariant value);
    void setData(const int row, const int column, QVariant value);
private:
    QStandardItemModel *m_dataModel;
public slots:
    void applyGridlayout();
protected slots:
    void columnResized(int, int, int) override {};
    void applyGuiSettings() override;
};

#endif // STABLEVIEWSUMMARYBASE_H
