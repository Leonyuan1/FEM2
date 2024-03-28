#include "bqgraphicsview.h"

BQGraphicsView::BQGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    // 隐藏水平/竖直滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 设置场景范围
    setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);

    // 反锯齿
    setRenderHints(QPainter::Antialiasing);
}

void BQGraphicsView::mousePressEvent(QMouseEvent *event)
{
    // 调用基类的鼠标点击事件处理函数
    QGraphicsView::mousePressEvent(event);
}
