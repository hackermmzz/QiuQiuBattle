#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("QiuQiuBattle");
    setWindowIcon(QIcon("src/icon/icon.png"));
    resize(800,800);
    Score=ui->score;
    Fps=ui->fps;
    Score->setStyleSheet("color:white");
    Fps->setStyleSheet("color:white");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    //设置opengl渲染控件与窗口同等大小
    auto& openglwidget = ui->canvas;
    openglwidget->move(0, 0);
    openglwidget->resize(width(), height());
    //设置score标签的大小和位置
    const float relativeW=0.2,relativeH=0.02;
    const float alignOff=0.01;
    Score->resize(relativeW*width(),relativeH*height());
    Score->move(alignOff*width(),alignOff*height());
    Fps->resize(relativeW*width(),relativeH*height());
    Fps->move((1-alignOff-relativeW)*width(),alignOff*height());
}
