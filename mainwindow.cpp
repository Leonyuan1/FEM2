#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("FEM"));
    m_scene.setBackgroundBrush(Qt::gray);
    ui->graphicsView->setScene(&m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_circleBtn_clicked()
{
    // 创建一个圆形对象，并添加到场景中
    BCircle *m_circle = new BCircle(0, 0, 50, BGraphicsItem::ItemType::Circle);
    m_scene.addItem(m_circle);
}

void MainWindow::on_ellipseBtn_clicked()
{
    // 创建一个椭圆对象，并添加到场景中
    BEllipse *m_ellipse = new BEllipse(0, 0, 120, 80, BGraphicsItem::ItemType::Ellipse);
    m_scene.addItem(m_ellipse);
}

void MainWindow::on_squareBtn_clicked()
{
    // 创建一个正方形对象，并添加到场景中
    BSquare *m_square = new BSquare(0, 0, 60, BGraphicsItem::ItemType::Square);
    m_scene.addItem(m_square);
}

void MainWindow::on_rectangleBtn_clicked()
{
    // 创建一个矩形对象，并添加到场景中
    BRectangle *m_rectangle = new BRectangle(0, 0, 80, 60, BGraphicsItem::ItemType::Rectangle);
    m_scene.addItem(m_rectangle);
}

void MainWindow::on_clearBtn_clicked()
{
    // 清空场景中的所有图形对象
    m_scene.clear();
}

void MainWindow::on_saveBtn_clicked()
{
    // 将场景中的图形对象保存到配置文件中
    m_scene.saveItemToConfig();
}

void MainWindow::on_loadBtn_clicked()
{
    // 从配置文件中加载图形对象到场景中
    m_scene.loadItemToScene();
}

void MainWindow::setBtnEnabled(bool enable)
{
    // 设置按钮的启用状态
    ui->circleBtn->setEnabled(enable);
    ui->ellipseBtn->setEnabled(enable);
    ui->squareBtn->setEnabled(enable);
    ui->rectangleBtn->setEnabled(enable);
    ui->clearBtn->setEnabled(enable);
    ui->saveBtn->setEnabled(enable);
    ui->loadBtn->setEnabled(enable);
}
