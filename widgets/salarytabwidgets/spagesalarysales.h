#ifndef SPAGESALARYSALES_H
#define SPAGESALARYSALES_H

#include <QWidget>
#include "spagesalarybase.h"
#include "../stableviewbase.h"

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
    void updateModels();
    void updateWidgets();
    void tableSalesRowSelected(const QModelIndex &current, const QModelIndex &prev);
    void tableSalesRowActivated(const QModelIndex &index);
    void tableSalesRowDoubleClicked(const QModelIndex &index);
    void tableSalePartsRowDoubleClicked(const QModelIndex &index);

private:
    Ui::SPageSalarySales *ui;
};

#endif // SPAGESALARYSALES_H
