#include "ssupplierstable.h"
#include "modules/purchasemanager/ssuppliersmodel.h"
#include "widgets/sdoublespinbox.h"

STableViewPartRequestSuppliers::STableViewPartRequestSuppliers(QWidget *parent) :
    STableViewBase(SLocalSettings::BuyRequestSuppliersGrid, parent)
{
    readLayout();

    setMouseTracking(true);
}

STableViewPartRequestSuppliers::~STableViewPartRequestSuppliers()
{

}

void STableViewPartRequestSuppliers::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    QRect cellRect = visualRect(index);

    // при наведении указателя на ссылку изменяем его вид
    if (index.isValid() &&
        (index.column() == Column::SupplierUrl ||
          index.column() == Column::ItemUrl) &&
         PSItemDelegate::isUrl(index) &&
         PSItemDelegate::pointInRect(pos, PSItemDelegate::urlRect(cellRect, PSItemDelegate::urlSize(index))) )
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }

    STableViewBase::mouseMoveEvent(event);
}

void STableViewPartRequestSuppliers::clearModel()
{
    // Очищать QSqlRelationalTableModel не нужно
}

void STableViewPartRequestSuppliers::setModelQuery(const QString &query, const QSqlDatabase &database)
{
    Q_UNUSED(query)
    Q_UNUSED(database)

    // для QSqlRelationalTableModel не нужен запрос, он генерируется в методах Qt

    static_cast<QSqlRelationalTableModel*>(m_model)->select();
}

void STableViewPartRequestSuppliers::clearVScrollPos()
{
    // Очистка сохранённых значений положения прокрутки не требуется
}

void STableViewPartRequestSuppliers::refresh(bool preserveScrollPos, bool preserveSelection)
{
    STableViewBase::refresh(preserveScrollPos, preserveSelection);
    restoreVScrollPos();
}

void STableViewPartRequestSuppliers::setDefaultLayoutParams()
{
    STableViewBase::setDefaultLayoutParams();
    i_gridLayout->$GridControl.Columns[Column::Notes].Width_marked = true;  // автоширина по умолчанию
    i_gridLayout->$GridControl.Columns[Column::RequestId].ActualWidth = 0;
    i_gridLayout->$GridControl.Columns[Column::RequestId].Visible = false;
}

void STableViewPartRequestSuppliers::translateNames()
{
    tr("Select");
    tr("Supplier");
    tr("SupplierUrl");
    tr("ItemUrl");
    tr("Moq");
    tr("Price");
    tr("Notes");
}

bool STableViewPartRequestSuppliers::isDecimal(const int column)
{
    return column == Column::Price;
}

PSItemDelegate::PSItemDelegate(QObject *parent) :
    STableViewBaseItemDelegates(parent)
{
}

void PSItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == SPartSuppliersModel::Supplier)
    {
        painter->save();

        QFont f = painter->font();
        QColor penColor = QColor(Qt::darkGray);
        f.setUnderline(false);
        painter->setFont(f);
        if (option.state & QStyle::State_Selected )
        {
            painter->fillRect(option.rect, option.palette.highlight().color());
            painter->setPen(option.palette.highlightedText().color());
            penColor = penColor.lighter(175);
        }
        if ( !(option.state & QStyle::State_Selected) )
            if(index.data(Qt::BackgroundRole).canConvert(QMetaType::QColor))
                painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QColor>());
        painter->drawText(option.rect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
        painter->setPen(penColor);
        drawItemsCount(index, option, painter);
        painter->restore();
        return;
    }

    if(!isUrl(index))
    {
        STableViewBaseItemDelegates::paint(painter, option, index);
        return;
    }

    painter->save();

    QString url = index.data().toString();
    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);

    drawUrl(index, newOption, painter);
    if(index.flags() & Qt::ItemIsEditable)
        drawPixmap(newOption, painter);

    painter->restore();
}

/* Возвращает QRect с учетом кнопки редактирования
*/
QRect PSItemDelegate::textRect(const QRect &delegateRect)
{
    return QRect(delegateRect.x(),
                 delegateRect.y(),
                 delegateRect.width() - PIXMAP_W - PIXMAP_GAP*3,
                 delegateRect.height());
}

void PSItemDelegate::drawUrl(const QModelIndex &index, const QStyleOptionViewItem &option, QPainter *painter) const
{
    QFont font = index.data(Qt::FontRole).value<QFont>();
    font.setUnderline(true);
    if (option.state & QStyle::State_Selected )
    {
        painter->fillRect(option.rect, option.palette.highlight().color());
        painter->setPen(option.palette.highlightedText().color());
    }
    else
    {
        if(index.data(Qt::BackgroundRole).canConvert(QMetaType::QColor))
            painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QColor>());
        painter->setPen(option.palette.link().color());
    }
    painter->setFont(font);
    painter->drawText(textRect(option.rect), Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
}

void PSItemDelegate::drawItemsCount(const QModelIndex &index, const QStyleOptionViewItem &option, QPainter *painter) const
{
    if(m_groupingMode != SPartsRequestsGroupingModel::Supplier)   // отображение кол-ва заявок возле URL поставщика только в режиме групиировки по поставщику
        return;

    QString supplier;
    QModelIndexList match;
    int selected, total;

    supplier = SPartsRequestsGroupingModel::urlFormat(index.siblingAtColumn(SPartSuppliersModel::Columns::SupplierUrl).data().toString());
    if(supplier.isEmpty())
        supplier = index.siblingAtColumn(SPartSuppliersModel::Columns::Supplier).data().toString();

    match = m_subgroupsModel->match(m_subgroupsModel->index(0, SPartsRequestsGroupingModel::Columns::Group), Qt::DisplayRole, supplier);
    if(match.count() == 0)
        return;

    selected = match.begin()->siblingAtColumn(SPartsRequestsGroupingModel::Columns::Selected).data().toInt();
    total = match.begin()->siblingAtColumn(SPartsRequestsGroupingModel::Columns::Total).data().toInt();
    painter->drawText(pixmapRect(option.rect, -2, 2), Qt::AlignCenter, QString("(%1/%2)").arg(selected).arg(total));

}

/* Возвращает QRect для отображения кнопки редактирования
 * index указывает порядковый номер кнопки справа на лево
*/
QRect PSItemDelegate::pixmapRect(const QRect &rect, const int index, const int span) const
{
    int x,y,w,h;
    x = rect.right() + (PIXMAP_W + PIXMAP_GAP) * index;
    y = rect.top()+(rect.height()-PIXMAP_W)/2;
    w = PIXMAP_W*span;
    h = PIXMAP_W;

    return QRect(x,y,w,h);
}

void PSItemDelegate::drawPixmap(const QStyleOptionViewItem &option, QPainter *painter) const
{
    QPixmap pixmap;

    pixmap = QPixmap(":/icons/light/pencil2_16.png");
    painter->drawPixmap(pixmapRect(option.rect), pixmap);
}

/* Проверка являются ли данные ссылкой в Интернет
*/
bool PSItemDelegate::isUrl(const QModelIndex &index)
{
    QString url = index.data().toString();
    if(url.isEmpty() || !url.startsWith("http"))
        return false;

    return true;
}

/* Возвращает длину ссылки в пикселях с учетом шрифта
*/
QSize PSItemDelegate::urlSize(const QModelIndex &index)
{
    QString text = index.data().toString();
    QFont font = index.data(Qt::FontRole).value<QFont>();
    QFontMetrics *fm = new QFontMetrics(font);
    return fm->size(Qt::TextSingleLine, text);
}

/* Возвращает QRect видимой части ссылки
*/
QRect PSItemDelegate::urlRect(const QRect &cell, const QSize &urlSize)
{
    int x, y, w, h;
    x = cell.x();
    y = cell.y();
    w = qMin(urlSize.width(), textRect(cell).width());
    h = cell.height();

    return QRect(x, y, w, h);
}

/* Проверяет попадает ли точка в область
*/
bool PSItemDelegate::pointInRect(const QPoint &point, const QRect &rect)
{
    return point.x() > rect.x() && \
           point.y() > rect.y() && \
           (point.x() < rect.x() + rect.width()) && \
           (point.y() < rect.y() + rect.height());
}

/* редактирование по одинарному щелчку
*/
bool PSItemDelegate::quickEdit(const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( index.flags() & Qt::ItemIsEditable )
    {
        QAbstractItemView* pView = qobject_cast<QAbstractItemView*>( const_cast<QWidget*>(option.widget) );
        if(pView != nullptr)
        {
            pView->setCurrentIndex(index);
            pView->edit(index);
        }
        return true;
    }
    return false;
}

QDoubleSpinBox *PSItemDelegate::createDoubleSpinBox(QWidget *parent, const QModelIndex &index) const
{
    Q_UNUSED(index)
    SDoubleSpinBox *sb = new SDoubleSpinBox(parent);
    // В таблице ссылок цена может быть с десятичной дробью независимо от настроек
    sb->setDecimals(2);
    sb->setMinimum(0.01);
    sb->setMaximum(999999.99);
    sb->setButtonSymbols(QAbstractSpinBox::NoButtons);

    return sb;
}

bool PSItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QMouseEvent * e = (QMouseEvent *)event;
    if(event->type() == QEvent::MouseButtonRelease && e->button() == Qt::LeftButton)
    {
        if( (index.column() == STableViewPartRequestSuppliers::Column::ItemUrl ||
            index.column() == STableViewPartRequestSuppliers::Column::SupplierUrl) )
        {
            // клик с нажатой клавишей Alt для быстрого поиска
            if(QGuiApplication::queryKeyboardModifiers() & Qt::AltModifier)
                return true;

            if( isUrl(index) )
            {
                // TODO: аналогичный код используется в STableViewBOQItemDelegates::editorEvent(...)
#if QT_VERSION >= 0x060000
                QPoint click = e->position().toPoint();
#else
                QPoint click = e->localPos().toPoint();
#endif
                int clickX = click.x() - option.rect.x();  // коррекция координат курсора для случая когда левый верхний угол яч. уходит за края виджета

                if(PSItemDelegate::pointInRect(e->localPos().toPoint(), PSItemDelegate::urlRect(option.rect, PSItemDelegate::urlSize(index))))
                {
                    QDesktopServices::openUrl(index.data().toUrl());
                    return false;
                }
                else if(clickX < option.rect.width() && clickX > option.rect.width() - PIXMAP_W)
                {
                    return quickEdit(option, index);
                }
                else
                    return false;
            }
        }
        else if(index.column() == STableViewPartRequestSuppliers::Column::Select)
        {
            if( model->flags(index) & (Qt::ItemIsEditable | Qt::ItemIsUserCheckable) )
            {
                model->setData(index, Qt::Checked, Qt::CheckStateRole);
                event->accept();
                return true;
            }
        }
    }

    return STableViewBaseItemDelegates::editorEvent(event, model, option, index);
}

QWidget *PSItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *w;
    switch (index.column())
    {
        case STableViewPartRequestSuppliers::Select: return nullptr;
        case STableViewPartRequestSuppliers::Moq: w = createSpinBox(parent, index); break;
        case STableViewPartRequestSuppliers::Price: w = createDoubleSpinBox(parent, index); break;
        default: w = STableViewBaseItemDelegates::createEditor(parent, option, index);
    }
    return w;
}

void PSItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
        case STableViewPartRequestSuppliers::Moq: setSpinBoxData(editor, index.data().toInt()); break;
        case STableViewPartRequestSuppliers::Price: setDoubleSpinBoxData(editor, index.data().toDouble()); break;
        default: STableViewBaseItemDelegates::setEditorData(editor, index);
    }
}

void PSItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
        case STableViewPartRequestSuppliers::Moq: setModelDataFromSpinBox(editor, model, index); break;
        case STableViewPartRequestSuppliers::Price: setModelDataFromDoubleSpinBox(editor, model, index); break;
        default: STableViewBaseItemDelegates::setModelData(editor, model, index);
    }
}

void PSItemDelegate::setTableModel(SPartSuppliersModel *model)
{
    m_tableModel = model;
}

void PSItemDelegate::setSubgroupsModel(SPartsRequestsGroupingModel *model)
{
    m_subgroupsModel = model;
}

void PSItemDelegate::setGroupingMode(const int mode)
{
    if(!m_subgroupsModel)
        return;

    m_groupingMode = mode;
}
