#include "bqgraphicsscene.h"
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>

BQGraphicsScene::BQGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    is_creating_BPolygon = false;
}

void BQGraphicsScene::startCreate()
{
    // 开始创建多边形
    is_creating_BPolygon = true;
    m_list.clear(); // 清空点列表
}

void BQGraphicsScene::saveItemToConfig()
{
    // 设置保存文件名
    QString fileName = QDir::currentPath() + "/item.ini";
    // 如果文件已存在，则删除
    if ( QFile::exists(fileName) ) {
        QFile(fileName).remove();
    }

    // 创建配置文件对象
    QSettings setting(fileName, QSettings::IniFormat);

    // 获取场景中所有图形项
    QList<QGraphicsItem *> list = this->items();

    // 开始写入图形项信息到配置文件
    setting.beginWriteArray("itemList");
    for ( int i = 0; i < list.size(); ++i )
    {
        setting.setArrayIndex(i);

        // 获取图形项及其类型
        BGraphicsItem *item = static_cast<BGraphicsItem *>(list[i]);
        BGraphicsItem::ItemType type = item->getType();
        setting.setValue("type", (int)type);
        setting.setValue("x", item->x());
        setting.setValue("y", item->y());

        // 根据不同类型的图形项保存额外的信息
        switch (type)
        {
        case BGraphicsItem::ItemType::Circle: {
            BCircle *circle = static_cast<BCircle *>(item);
            setting.setValue("radius", circle->m_radius);
        } break;
        case BGraphicsItem::ItemType::Ellipse: {
            BEllipse *ellipse = static_cast<BEllipse *>(item);
            setting.setValue("edgeX", ellipse->getEdge().x());
            setting.setValue("edgeY", ellipse->getEdge().y());
        } break;
        case BGraphicsItem::ItemType::Rectangle: {
            BRectangle *rectangle = static_cast<BRectangle *>(item);
            setting.setValue("edgeX", rectangle->getEdge().x());
            setting.setValue("edgeY", rectangle->getEdge().y());
        } break;
        case BGraphicsItem::ItemType::Square: {
            BSquare *square = static_cast<BSquare *>(item);
            setting.setValue("edgeX", square->getEdge().x());
        } break;
        default: break;
        }
    }
    setting.endArray();
}

void BQGraphicsScene::loadItemToScene()
{
    // 清空场景
    this->clear();

    // 加载保存的图形项信息
    QString fileName = QDir::currentPath() + "/item.ini";
    QSettings setting(fileName, QSettings::IniFormat);

    // 读取图形项列表大小
    int size = setting.beginReadArray("itemList");
    for ( int i = 0; i < size; ++i )
    {
        setting.setArrayIndex(i);

        // 读取图形项的类型
        BGraphicsItem::ItemType type = (BGraphicsItem::ItemType)setting.value("type").toInt();

        // 根据类型创建相应的图形项并添加到场景中
        switch (type)
        {
        case BGraphicsItem::ItemType::Circle: {
            qreal radius = setting.value("radius").toDouble();
            BCircle *m_circle = new BCircle(0, 0, radius, BGraphicsItem::ItemType::Circle);
            m_circle->setX(setting.value("x").toDouble());
            m_circle->setY(setting.value("y").toDouble());
            addItem(m_circle);
        } break;
        case BGraphicsItem::ItemType::Ellipse: {
            qreal edgeX = setting.value("edgeX").toDouble();
            qreal edgeY = setting.value("edgeY").toDouble();
            BEllipse *m_ellipse = new BEllipse(0, 0, edgeX*2, edgeY*2, BGraphicsItem::ItemType::Ellipse);
            m_ellipse->setX(setting.value("x").toDouble());
            m_ellipse->setY(setting.value("y").toDouble());
            addItem(m_ellipse);
        } break;
        case BGraphicsItem::ItemType::Rectangle: {
            qreal edgeX = setting.value("edgeX").toDouble();
            qreal edgeY = setting.value("edgeY").toDouble();
            BRectangle *m_rectangle = new BRectangle(0, 0, edgeX*2, edgeY*2, BGraphicsItem::ItemType::Rectangle);
            m_rectangle->setX(setting.value("x").toDouble());
            m_rectangle->setY(setting.value("y").toDouble());
            addItem(m_rectangle);
        } break;
        case BGraphicsItem::ItemType::Square: {
            qreal edgeX = setting.value("edgeX").toDouble();
            BSquare *m_square = new BSquare(0, 0, edgeX*2, BGraphicsItem::ItemType::Square);
            m_square->setX(setting.value("x").toDouble());
            m_square->setY(setting.value("y").toDouble());
            addItem(m_square);
        } break;
        default: break;
        }
    }
    setting.endArray();
}

void BQGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_creating_BPolygon) {
        QPointF p(event->scenePos().x(), event->scenePos().y());

        switch ( event->buttons() )
        {
        case Qt::LeftButton: {
            // 在左键点击时向多边形的点列表中添加点，并发射更新点的信号
            m_list.push_back(p);
            emit updatePoint(p, m_list, false);
        } break;
        case Qt::RightButton: {
            if (m_list.size() >= 3) {
                // 在右键点击时，如果点的数量大于等于3，表示多边形创建完成
                emit updatePoint(p, m_list, true); // 更新最后一个点并完成创建
                emit createFinished(); // 发射完成创建的信号
                is_creating_BPolygon = false; // 设置不再创建多边形
                m_list.clear(); // 清空点列表
            }
        } break;
        default: break;
        }
    } else {
        // 如果不在创建多边形状态，则调用基类的鼠标点击事件处理函数
        QGraphicsScene::mousePressEvent(event);
    }
}
