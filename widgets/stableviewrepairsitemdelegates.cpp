#include "stableviewrepairsitemdelegates.h"
#include <QTimeZone>
#include <ProjectGlobals>
#include <SRepairsModel>
#include <SRepairsView>
#include <SComSettings>
#include <QPainter>
#include <SRepairsModel>

STableViewRepairsItemDelegates::STableViewRepairsItemDelegates(QObject *parent) : STableViewBaseItemDelegates(parent)
{
}

STableViewRepairsItemDelegates::~STableViewRepairsItemDelegates()
{
}

void STableViewRepairsItemDelegates::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == STableViewRepairs::Column::Progress)
    {
        paintStatusProgressBar(painter, option, index);
        paintRepairProgressBar(painter, option, index);
    }
    else if(index.column() == STableViewRepairs::Column::ID)
        paintClientInformStatus(painter, option, index);
    else
        STableViewBaseItemDelegates::paint(painter, option, index);
}

// индикатор прогресса текущего статуса
// В АСЦ цвет прогрессбара, при превышении заданного в настройках статусов срока, рассчитывется как-то странно:
// если срок <1ч 16мин (прибл.), то это значение игнорируется и вместо него берётся ~8640сек (2ч 24мин 00сек)
// Далее значение в секундах делится на 4 периода, к каждому периоду добавляется 1..4%
void STableViewRepairsItemDelegates::paintStatusProgressBar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    uint progressStatus = 0;
    uint repairStatus = m_tableModel->unformattedData(index.siblingAtColumn(STableRepairsModel::Columns::Status)).toInt();
    qint64 statusTermSecons = comSettings->repairStatusesVariantCopy()[repairStatus].TermsSec;
    QDateTime statusChanged = m_tableModel->unformattedData(index.siblingAtColumn(STableRepairsModel::Columns::LastStatusChanged)).toDateTime();
    statusChanged.setTimeZone(QTimeZone::utc());
    qint64 secondsSinceStatusChanged = statusChanged.secsTo(QDateTime::currentDateTimeUtc()) + 1;

    if(statusTermSecons <= 0)   // если термин статуса не задан, что шкала прогресса 100%
        statusTermSecons = secondsSinceStatusChanged;

    switch(repairStatus)
    {
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair: break;  // прогресс выданных всегда 0
        default: progressStatus = qMin(100, (int)((100 * secondsSinceStatusChanged)/statusTermSecons));
    }

    QStyleOptionProgressBar progressBarStatus;
    QRect r = option.rect;
    r.setHeight( r.height()/3 );
    progressBarStatus.rect = r;
    progressBarStatus.textAlignment = Qt::AlignCenter;
    progressBarStatus.minimum = 0;
    progressBarStatus.maximum = 100;
    progressBarStatus.progress = progressStatus;
    progressBarStatus.textVisible = false;
    // Qt 6 requires QStyle::State_Horizontal to be set for correctly drawing horizontal progress bar
    progressBarStatus.state = option.state | QStyle::State_Horizontal;

    paintColorizedProgressBar(&progressBarStatus, painter, option.widget);
}

// макс. значение — сумма дней всех терминов в таблице статусов (учитываются только дни, часть с часами, минутами и секундами  не учитывается)
// цвет прогрессбара рассчитываются с учетом часовых поясов: systemTime - systemTimeZone - (db.workshop.in_date + db.config.time_zone)
// из суммы дней вычетается 30 часов;
void STableViewRepairsItemDelegates::paintRepairProgressBar(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    uint progressRepair = 0;
    uint repairStatus = m_tableModel->unformattedData(index.siblingAtColumn(STableRepairsModel::Columns::Status)).toInt();
    QDateTime inDate = m_tableModel->unformattedData(index.siblingAtColumn(STableRepairsModel::Columns::InDate)).toDateTime();
    inDate.setTimeZone(QTimeZone::utc());
    QDateTime currentOrOutDate;
    switch (repairStatus)
    {
        case Global::RepStateIds::Returned:
        case Global::RepStateIds::ReturnedNoRepair:
        case Global::RepStateIds::ReturnedInCredit: currentOrOutDate = m_tableModel->unformattedData(index.siblingAtColumn(STableRepairsModel::Columns::OutDate)).toDateTime(); break;
        default: ;
    }
    if(!currentOrOutDate.isValid())
        currentOrOutDate = QDateTime::currentDateTimeUtc();
    currentOrOutDate.setTimeZone(QTimeZone::utc());
    qint64 secondsSinceInDate = inDate.secsTo(currentOrOutDate);
    qint64 daysSinceInDate = inDate.daysTo(currentOrOutDate);

    if(comSettings->repairStatusesVariantCopy().RepairTermSec)
    {
        progressRepair = qMin(100, (int)((100 * secondsSinceInDate)/comSettings->repairStatusesVariantCopy().RepairTermSec));

        QStyleOptionProgressBar progressBarRepair;
        QRect r2 = option.rect;
        r2.moveTop( r2.top() + option.rect.height()/3 );
        r2.setHeight( r2.height()*2/3 );
        progressBarRepair.rect = r2;
        progressBarRepair.textAlignment = Qt::AlignCenter;
        progressBarRepair.minimum = 0;
        progressBarRepair.maximum = 100;
        progressBarRepair.progress = progressRepair;
        progressBarRepair.text = QString::number(daysSinceInDate) + tr(" дн."); // TODO: просклонять слово
        progressBarRepair.textVisible = true;
        progressBarRepair.state = option.state | QStyle::State_Horizontal;

        paintColorizedProgressBar(&progressBarRepair, painter, option.widget);
    }

}

void STableViewRepairsItemDelegates::paintClientInformStatus(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!i_fontMetrics)
        Q_ASSERT_X(0, objectName().toLocal8Bit(), QString("i_fontMetrics not set").toLocal8Bit());

    STableViewBaseItemDelegates::paint(painter, option, index);
    int status = m_tableModel->unformattedData(index.siblingAtColumn(STableRepairsModel::Columns::InformedStatus)).toInt();

    if(!status)
        return;

    QColor color;
    switch (status)
    {
        case Global::ClientInformStateIds::Informed: color.setRgb(51,190,6); break;
        case Global::ClientInformStateIds::NotResponse:
        case Global::ClientInformStateIds::NotAbailable: color.setRgb(234,73,73); break;
        case Global::ClientInformStateIds::NotInformedOther: color.setRgb(255,165,0); break;
        default: return;
    }
    int w = i_fontMetrics->horizontalAdvance(index.data().toString());
    QPoint pt = option.rect.center();
    pt.setX(option.rect.left() + w + 10);
    QRect rect(0, 0, 4, i_fontMetrics->height()+2);
    rect.moveCenter(pt);

    // TODO: всплывающая подсказка статуса информирования клиента
    painter->save();
    painter->fillRect(rect, QBrush(color));
    painter->restore();
}

void STableViewRepairsItemDelegates::setTableModel(QAbstractItemModel *model)
{
    m_tableModel = static_cast<STableRepairsModel*>(model);
}

