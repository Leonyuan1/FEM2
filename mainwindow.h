#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bqgraphicsitem.h"
#include "bqgraphicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 启用或禁用按钮
    void setBtnEnabled(bool enable);

private slots:
    // 按钮点击槽函数
    void on_circleBtn_clicked();
    void on_ellipseBtn_clicked();
    void on_squareBtn_clicked();
    void on_rectangleBtn_clicked();
    void on_clearBtn_clicked();
    void on_saveBtn_clicked();
    void on_loadBtn_clicked();

private:
    Ui::MainWindow *ui;
    BQGraphicsScene m_scene; // 主窗口对应的场景对象
};
#endif // MAINWINDOW_H
