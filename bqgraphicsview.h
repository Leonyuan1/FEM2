#ifndef BQGRAPHICSVIEW_H
#define BQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class BQGraphicsView : public QGraphicsView
{
public:
    BQGraphicsView(QWidget *parent = nullptr);

protected:
    // 重写鼠标按下事件
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // BQGRAPHICSVIEW_H
