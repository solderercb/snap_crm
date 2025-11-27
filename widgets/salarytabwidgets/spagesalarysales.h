#ifndef SPAGESALARYSALES_H
#define SPAGESALARYSALES_H

#include <QObject>
#include <SSalaryBasePage>

class QWidget;

namespace Ui {
class SPageSalarySales;
}

class SPageSalarySales : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalarySales(QWidget *parent = nullptr);
    ~SPageSalarySales();
private:
    void loadDocumentData(const int id);
public slots:
    void updateModels() override;
    void updateWidgets() override;
    void tableSalesRowSelected(const QModelIndex &current, const QModelIndex &prev);
    void tableSalesRowActivated(const QModelIndex &index);
    void tableSalesRowDoubleClicked(const QModelIndex &index);
    void tableSalePartsRowDoubleClicked(const QModelIndex &index);
public slots:
    void guiFontChanged() override;
private:
    Ui::SPageSalarySales *ui;
};

#endif // SPAGESALARYSALES_H
