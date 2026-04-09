#ifndef SCASHBOXWIDGET_H
#define SCASHBOXWIDGET_H

#include <SWidget>
#include <STableViewBase>

class STableCashboxModel;

namespace Ui {
class SCashboxWidget;
}

class SCashboxWidget : public SWidget
{
    Q_OBJECT

public:
    explicit SCashboxWidget(QWidget *parent = nullptr);
    ~SCashboxWidget();
    void setFilter(const FilterList &filter);
    void refresh(bool preserveScrollPos = STableViewBase::ScrollPosPreserve, bool preserveSelection = STableViewBase::SelectionReset);
    void delayedRefresh(const int msec);
    STableCashboxModel* tableModel();
private:
    Ui::SCashboxWidget *ui;
    STableCashboxModel* m_mainTableModel;
    void enableVScrollbarPadding(const bool state = 1);
private slots:
    void updateSummary();
    void tableRowDoubleClicked(const QModelIndex &index);
    void tableColumnResized(int column, int newWidth);
};

#endif // SCASHBOXWIDGET_H
