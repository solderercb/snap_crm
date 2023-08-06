#include "stableviewsalarypayments.h"

STableViewSalaryPayments::STableViewSalaryPayments(QWidget *parent) :
    STableViewBase(SLocalSettings::SalaryPaymentsGrid, parent)
{
    i_defaultColumnsWidths = {{0, 90},{1, 120},{2, 90},{3, 140},{4, 90},{5, 90},{6, 250}};
    i_defaultHeaderLabels << tr("№ документа") << tr("Дата") << tr("Выдано") << tr("Зачислено на баланс") << tr("Сотрудник") << tr("Пользователь") << tr("Основание");
    readLayout();
    i_gridLayout->$GridControl.Columns[6].Width_marked = true;  // по умолчанию автоширина столбца с основанием
}

STableViewSalaryPayments::~STableViewSalaryPayments()
{
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
