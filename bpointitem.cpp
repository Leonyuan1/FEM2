#include "bpointitem.h"
#include "bqgraphicsitem.h"

//初始化BPointItem类的成员变量m_point和m_type。
//将图形项的位置设置为传入的点坐标p。
//设置图形项的标志，使其可被选中、移动和聚焦。
//根据传入的点类型type设置图形项的鼠标光标样式。
BPointItem::BPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, PointType type)
    : QAbstractGraphicsShapeItem(parent)
    , m_point(p)
    , m_type(type)
{
    this->setPos(m_point);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);

    switch (type) {
    case Center:
        this->setCursor(Qt::OpenHandCursor);
        break;
    case Edge:
        this->setCursor(Qt::PointingHandCursor);
        break;
    case Special:
        this->setCursor(Qt::PointingHandCursor);
        break;
    default: break;
    }
}

//这段代码是BPointItem类的成员函数boundingRect的实现。这个函数用于返回图形项的边界矩形，即图形项所占据的矩形区域。
QRectF BPointItem::boundingRect() const
{
    return QRectF(-4, -4, 8, 8);
}

//这段代码是BPointItem类的paint函数的实现。这个函数用于绘制图形项的外观。
void BPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    this->setPos(m_point);

    switch (m_type) {
    case Center:
        painter->drawEllipse(-4, -4, 8, 8);
        break;
    case Edge:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    case Special:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    default: break;
    }
}

//这段代码是BPointItem类的mouseMoveEvent函数的实现，用于处理鼠标移动事件。
//实现图形的移动、放缩（通过判断鼠标在的点来决定执行的操作）
void BPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton ) {
        qreal dx = event->scenePos().x() - event->lastScenePos().x();
        qreal dy = event->scenePos().y() - event->lastScenePos().y();

        BGraphicsItem* item = static_cast<BGraphicsItem *>(this->parentItem());
        BGraphicsItem::ItemType itemType = item->getType();

        switch (m_type) {
        case Center: {
            item->moveBy(dx, dy);
            this->scene()->update();
        } break;
        case Edge: {
            if (itemType == BGraphicsItem::ItemType::Circle||itemType == BGraphicsItem::ItemType::Ellipse||itemType == BGraphicsItem::ItemType::Rectangle||itemType == BGraphicsItem::ItemType::Square) {
                m_point = this->mapToParent( event->pos() );
                this->setPos(m_point);
                this->scene()->update();
            }

            switch (itemType) {
            case BGraphicsItem::ItemType::Ellipse: {
                BEllipse *ellipse = dynamic_cast<BEllipse *>(item);
                ellipse->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Circle: {
                BCircle *circle = dynamic_cast<BCircle *>(item);
                circle->setEdge(m_point);
                circle->updateRadius();
            } break;
            case BGraphicsItem::ItemType::Rectangle: {
                BRectangle *rectangle = dynamic_cast<BRectangle *>(item);
                rectangle->setEdge(m_point);
            } break;
            case BGraphicsItem::ItemType::Square: {
                BSquare *square = dynamic_cast<BSquare *>(item);
                qreal ret = m_point.x() > m_point.y() ? m_point.x() : m_point.y();
                m_point.setX(ret);
                m_point.setY(ret);
                square->setEdge(m_point);
            } break;
            default: break;
            }
        } break;
        default: break;
        }
    }
}

//------------------------------------------------------------------------------

void BPointItemList::setRandColor()
{
    // 生成随机颜色并设置给BPointItemList对象中的所有点
    this->setColor(QColor(rand()%256, rand()%256, rand()%256));
}

void BPointItemList::setColor(const QColor color)
{
    for (auto &temp : *this)
    {
        // 遍历BPointItemList对象中的所有点，将它们的画刷颜色设置为指定的颜色
        temp->setBrush(QBrush(color));
    }
}

void BPointItemList::setVisible(bool visible)
{
    for (auto &temp : *this)
    {
        // 遍历BPointItemList对象中的所有点，将它们的可见性设置为指定的值
        temp->setVisible(visible);
    }
}
