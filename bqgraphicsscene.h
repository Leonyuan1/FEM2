#ifndef BQGRAPHICSSCENE_H
#define BQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "bpointitem.h"
#include "bqgraphicsitem.h"

class BQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BQGraphicsScene(QObject *parent = nullptr);

    // 开始创建图形
    void startCreate();

    // 将图形项保存到配置文件
    void saveItemToConfig();

    // 从配置文件加载图形项到场景中
    void loadItemToScene();

protected:
    // 鼠标按下事件
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    // 更新点的信号
    void updatePoint(QPointF p, QList<QPointF> list, bool isCenter);

    // 创建完成信号
    void createFinished();

protected:
    QList<QPointF> m_list; // 点的列表
    bool is_creating_BPolygon; // 是否正在创建多边形
};

#endif // BQGRAPHICSSCENE_H
