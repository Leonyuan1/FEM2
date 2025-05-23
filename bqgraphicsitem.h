﻿#ifndef BQGRAPHICSITEM_H
#define BQGRAPHICSITEM_H

#include <QObject>
#include "bpointitem.h"

#define PI 3.1415926535

// 自定义图元 - 基础类
class BGraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    enum ItemType {
        Circle = 0,         // 圆
        Ellipse,            // 椭圆
        Rectangle,          // 矩形
        Square,             // 正方形
    };

    QPointF getCenter() { return m_center; }
    void setCenter(QPointF p) { m_center = p; }

    QPointF getEdge() { return m_edge; }
    void setEdge(QPointF p) { m_edge = p; }

    ItemType getType() { return m_type; }

protected:
    BGraphicsItem(QPointF center, QPointF edge, ItemType type);

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

public:
    QPointF m_center;
    QPointF m_edge;
    ItemType m_type;
    BPointItemList m_pointList;

    QPen m_pen_isSelected;
    QPen m_pen_noSelected;
};

//------------------------------------------------------------------------------

// 椭圆
class BEllipse : public BGraphicsItem
{
    Q_OBJECT

public:
    BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------

// 圆
class BCircle : public BEllipse
{
public:
    BCircle(qreal x, qreal y, qreal radius, ItemType type);

    void updateRadius();

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public:
    qreal m_radius;
};

//------------------------------------------------------------------------------



// 矩形
class BRectangle : public BGraphicsItem
{
public:
    BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------

// 正方形
class BSquare : public BRectangle
{
public:
    BSquare(qreal x, qreal y, qreal width, ItemType type);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------


#endif // BQGRAPHICSITEM_H
