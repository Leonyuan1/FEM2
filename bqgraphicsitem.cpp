#include "bqgraphicsitem.h"
#include <QVector>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QWidgetAction>

BGraphicsItem::BGraphicsItem(QPointF center, QPointF edge, ItemType type)
    : m_center(center), m_edge(edge), m_type(type)
{
    // 初始化画笔的样式
    m_pen_noSelected.setColor(QColor(0, 160, 230)); // 未选中时的画笔颜色为蓝色
    m_pen_noSelected.setWidth(2); // 未选中时的画笔宽度为2
    m_pen_isSelected.setColor(QColor(255, 0, 255)); // 选中时的画笔颜色为紫色
    m_pen_isSelected.setWidth(2); // 选中时的画笔宽度为2

    // 将画笔设置为默认的未选中状态下的画笔
    this->setPen(m_pen_noSelected);

    // 设置图形项的标志，使其可被选中、移动和聚焦
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);
}


void BGraphicsItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event); // 防止编译器警告未使用的参数

    // 设置图形项的画笔为选中状态下的画笔
    this->setPen(m_pen_isSelected);
}

void BGraphicsItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event); // 防止编译器警告未使用的参数

    // 设置图形项的画笔为未选中状态下的画笔
    this->setPen(m_pen_noSelected);
}


//------------------------------------------------------------------------------

BEllipse::BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BGraphicsItem(QPointF(x,y), QPointF(x+width/2,y+height/2), type)
{
    // 创建椭圆图形项的边缘点和中心点，并添加到图形项的点列表中
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge); // 创建边缘点
    point->setParentItem(this); // 设置边缘点的父项为椭圆图形项
    m_pointList.append(point); // 将边缘点添加到点列表中
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center)); // 创建中心点，并添加到点列表中

    // 设置点列表中所有点的颜色为随机颜色
    m_pointList.setRandColor();
}


QRectF BEllipse::boundingRect() const
{
    // 计算椭圆的边界矩形
    // 左上角坐标为椭圆中心点的 x 坐标减去横向半径，y 坐标减去纵向半径
    // 宽度为横向半径的两倍，高度为纵向半径的两倍
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}


void BEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); // 防止编译器警告未使用的参数
    Q_UNUSED(widget); // 防止编译器警告未使用的参数

    // 设置画笔和画刷为图形项的当前设置
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    // 计算椭圆的边界矩形
    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);

    // 绘制椭圆
    painter->drawEllipse(ret);
}


void BEllipse::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // 检查椭圆图形项是否被选中
    if (!this->isSelected())
        return; // 如果未被选中，则直接返回

    // 创建一个上下文菜单
    QMenu* menu = new QMenu();
    // 设置上下文菜单的样式
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    // 创建用于调整椭圆宽度的SpinBox，并连接信号和槽
    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        // 根据SpinBox的值调整椭圆的横向半径，并更新对应的点
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
        } else {
            m_edge.setX(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    // 创建用于调整椭圆高度的SpinBox，并连接信号和槽
    QSpinBox* height_spinBox = new QSpinBox(menu);
    height_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height_spinBox->setRange(0, 1000);
    height_spinBox->setPrefix("h: ");
    height_spinBox->setSuffix(" cm");
    height_spinBox->setSingleStep(1);
    height_spinBox->setValue(2 * abs(m_edge.y()));
    connect(height_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        // 根据SpinBox的值调整椭圆的纵向半径，并更新对应的点
        if (m_edge.y() < 0) {
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    // 创建SpinBox的QWidgetAction，并将其添加到菜单中
    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height_spinBox);
    menu->addAction(height_widgetAction);

    // 在鼠标位置显示上下文菜单
    menu->exec(QCursor::pos());
    delete menu; // 删除上下文菜单

    // 调用基类的contextMenuEvent函数
    QGraphicsItem::contextMenuEvent(event);
}


//------------------------------------------------------------------------------

BCircle::BCircle(qreal x, qreal y, qreal radius, ItemType type)
    : BEllipse(x, y, radius*sqrt(2), radius*sqrt(2), type)
{
    updateRadius(); // 更新半径
}

void BCircle::updateRadius()
{
    // 计算半径
    m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2));
}

QRectF BCircle::boundingRect() const
{
    // 返回圆形的边界矩形
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); // 防止编译器警告未使用的参数
    Q_UNUSED(widget); // 防止编译器警告未使用的参数

    // 设置画笔和画刷为图形项的当前设置
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    // 绘制圆形
    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawEllipse(ret);
}


void BCircle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // 检查圆形图形项是否被选中
    if (!this->isSelected())
        return; // 如果未被选中，则直接返回

    // 创建一个上下文菜单
    QMenu* menu = new QMenu();
    // 设置上下文菜单的样式
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    // 创建用于调整圆形半径的SpinBox，并连接信号和槽
    QSpinBox* radius_spinBox = new QSpinBox(menu);
    radius_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    radius_spinBox->setRange(0, 1000);
    radius_spinBox->setPrefix("r: ");
    radius_spinBox->setSuffix(" cm");
    radius_spinBox->setSingleStep(1);
    radius_spinBox->setValue(m_radius);
    connect(radius_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        // 更新圆形的半径
        m_radius = v;

        // 根据新的半径调整边缘点的坐标
        if (m_edge.x() < 0) {
            m_edge.setX(m_center.x() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setX(m_center.x() + m_radius * sqrt(2)/2);
        }

        if (m_edge.y() < 0) {
            m_edge.setY(m_center.y() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setY(m_center.y() + m_radius * sqrt(2)/2);
        }

        // 更新对应的点
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    // 创建SpinBox的QWidgetAction，并将其添加到菜单中
    QWidgetAction* radius_widgetAction = new QWidgetAction(menu);
    radius_widgetAction->setDefaultWidget(radius_spinBox);
    menu->addAction(radius_widgetAction);

    // 在鼠标位置显示上下文菜单
    menu->exec(QCursor::pos());
    delete menu; // 删除上下文菜单

    // 调用基类的contextMenuEvent函数
    QGraphicsItem::contextMenuEvent(event);
}


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

BRectangle::BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BGraphicsItem(QPointF(x,y), QPointF(width/2,height/2), type)
{
    // 在构造函数中创建边缘点和中心点，并设置颜色
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BRectangle::boundingRect() const
{
    // 返回矩形的边界矩形
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}

void BRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); // 防止编译器警告未使用的参数
    Q_UNUSED(widget); // 防止编译器警告未使用的参数

    // 设置画笔和画刷为图形项的当前设置
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    // 绘制矩形
    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
    painter->drawRect(ret);
}


void BRectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // 检查矩形图形项是否被选中
    if (!this->isSelected())
        return; // 如果未被选中，则直接返回

    // 创建一个上下文菜单
    QMenu* menu = new QMenu();
    // 设置上下文菜单的样式
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    // 创建用于调整矩形宽度的SpinBox，并连接信号和槽
    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" cm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        // 根据SpinBox的值调整矩形的宽度，并更新对应的点
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
        } else {
            m_edge.setX(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    // 创建用于调整矩形高度的SpinBox，并连接信号和槽
    QSpinBox* height_spinBox = new QSpinBox(menu);
    height_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height_spinBox->setRange(0, 1000);
    height_spinBox->setPrefix("h: ");
    height_spinBox->setSuffix(" mm");
    height_spinBox->setSingleStep(1);
    height_spinBox->setValue(2 * abs(m_edge.y()));
    connect(height_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        // 根据SpinBox的值调整矩形的高度，并更新对应的点
        if (m_edge.y() < 0) {
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    // 创建SpinBox的QWidgetAction，并将其添加到菜单中
    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height_spinBox);
    menu->addAction(height_widgetAction);

    // 在鼠标位置显示上下文菜单
    menu->exec(QCursor::pos());
    delete menu; // 删除上下文菜单

    // 调用基类的contextMenuEvent函数
    QGraphicsItem::contextMenuEvent(event);
}


//------------------------------------------------------------------------------

BSquare::BSquare(qreal x, qreal y, qreal width, ItemType type)
    : BRectangle(x, y, width, width, type) {}

void BSquare::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    // 检查正方形图形项是否被选中
    if (!this->isSelected())
        return; // 如果未被选中，则直接返回

    // 创建一个上下文菜单
    QMenu* menu = new QMenu();
    // 设置上下文菜单的样式
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    // 创建用于调整正方形边长的SpinBox，并连接信号和槽
    QSpinBox* distance_spinBox = new QSpinBox(menu);
    distance_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    distance_spinBox->setRange(0, 1000);
    distance_spinBox->setPrefix("d: ");
    distance_spinBox->setSuffix(" mm");
    distance_spinBox->setSingleStep(1);
    distance_spinBox->setValue(2 * abs(m_edge.x())); // 正方形的边长等于矩形的宽度
    connect(distance_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        // 根据SpinBox的值调整正方形的边长，并更新对应的点
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setX(v/2);
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    // 创建SpinBox的QWidgetAction，并将其添加到菜单中
    QWidgetAction* distance_widgetAction = new QWidgetAction(menu);
    distance_widgetAction->setDefaultWidget(distance_spinBox);
    menu->addAction(distance_widgetAction);

    // 在鼠标位置显示上下文菜单
    menu->exec(QCursor::pos());
    delete menu; // 删除上下文菜单

    // 调用基类的contextMenuEvent函数
    QGraphicsItem::contextMenuEvent(event);
}


//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
