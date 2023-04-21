#include "stableviewsalarypayments.h"

STableViewSalaryPayments::STableViewSalaryPayments(QWidget *parent) : STableViewBase(parent)
{
    i_defaultColumnsWidths = {{0, 90},{1, 120},{2, 90},{3, 90},{4, 90},{5, 250}};
    i_defaultHeaderLabels << tr("№ документа") << tr("Дата") << tr("Сумма") << tr("Сотрудник") << tr("Пользователь") << tr("Основание");
    readLayout(SLocalSettings::SalaryPaymentsGrid);
    i_gridLayout->$GridControl.Columns[5].Width_marked = true;  // по умолчанию автоширина столбца с основанием
}

STableViewSalaryPayments::~STableViewSalaryPayments()
{
    saveLayout(SLocalSettings::SalaryPaymentsGrid);
}

XtraSerializer *STableViewSalaryPayments::gridLayout()
{
    return i_gridLayout;
}

void STableViewSalaryPayments::columnResized(int column, int oldWidth, int newWidth)
{
    STableViewBase::columnResized(column, oldWidth, newWidth);
    emit signalColumnResized(column, newWidth);
}
