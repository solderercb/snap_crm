#ifndef SPAGESALARYEXTRA_H
#define SPAGESALARYEXTRA_H

#include <QWidget>
#include <STableViewSummaryBase>
#include <SSalaryBasePage>

namespace Ui {
class SPageSalaryExtra;
}

// Таблица с суммами по столбцам
class STableViewSalaryExtraSummary : public STableViewSummaryBase
{
    Q_OBJECT
public:
    explicit STableViewSalaryExtraSummary(QWidget *parent = nullptr);
    void setGridLayout(XtraSerializer* layout);
};

class SPageSalaryExtra : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalaryExtra(QWidget *parent = nullptr);
    ~SPageSalaryExtra();

private:
    Ui::SPageSalaryExtra *ui;
    void enableVScrollbarPadding(const bool state = 1);
    int checkInput() override;
    QString queryLogFile() override;
    void commit(const int) override;
    void endCommit() override;
public slots:
    void updateModels();
    void updateWidgets();
private slots:
    void tableColumnResized(int column, int newWidth);
};

#endif // SPAGESALARYEXTRA_H
