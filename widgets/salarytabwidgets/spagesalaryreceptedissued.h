#ifndef SPAGESALARYRECEPTEDISSUED_H
#define SPAGESALARYRECEPTEDISSUED_H

#include <QObject>
#include <SSalaryBasePage>

class QWidget;

namespace Ui {
class SPageSalaryReceptedIssued;
}

class SPageSalaryReceptedIssued : public SPageSalaryBase
{
    Q_OBJECT

public:
    explicit SPageSalaryReceptedIssued(QWidget *parent = nullptr);
    ~SPageSalaryReceptedIssued();
private:
    Ui::SPageSalaryReceptedIssued *ui;
public slots:
    void updateModels() override;
    void updateWidgets() override;
    void tableRepairsRowDoubleClicked(const QModelIndex &index);
private slots:
    void guiFontChanged() override;
};

#endif // SPAGESALARYRECEPTEDISSUED_H
