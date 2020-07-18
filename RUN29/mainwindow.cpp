#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置大小、标题
    this->setFixedSize(1280,720);
    this->setWindowTitle("RUN29");

    //游戏widget载入
    GameWidget *gameWidget=new GameWidget(this,this->width(),this->height());
    //title，开始按钮设置
    QLabel *titleLabel=new QLabel(this);
    titleLabel->resize(600,200);
    titleLabel->setPixmap(QPixmap(":/imgs/startTitle.png").scaled(titleLabel->width(),titleLabel->height()));
    titleLabel->move((this->width()-titleLabel->width())*0.5,(this->height()-titleLabel->height())*0.2);
    GameButton *startBtn=new GameButton(":/imgs/startBtn1",":/imgs/startBtn2",":/imgs/startBtn3");
    startBtn->setParent(this);
    startBtn->move((this->width()-startBtn->width())*0.5,(this->height()-startBtn->height())*0.8);
    //infoLabel
    infoBtn= new QPushButton(this);
    infoBtn->setFixedSize(this->width(),this->height());
    infoBtn->setIcon(QPixmap(":/imgs/info.png"));
    infoBtn->setIconSize(QSize(this->width(),this->height()));
    this->setStyleSheet("QPushButton{border:0px;}");
    infoBtn->move(0,0);
    infoBtn->hide();

    connect(startBtn,&QPushButton::clicked,this,[=](){
        animationA=new QPropertyAnimation(startBtn,"geometry");
        animationA->setDuration(300);
        animationA->setEasingCurve(QEasingCurve::OutQuad);
        animationA->setStartValue(QRect(startBtn->x(),startBtn->y(),startBtn->width(),startBtn->height()));
        animationA->setEndValue(QRect(startBtn->x(),750,startBtn->width(),startBtn->height()));
        animationA->start();

        animationB=new QPropertyAnimation(titleLabel,"geometry");
        animationB->setDuration(300);
        animationB->setEasingCurve(QEasingCurve::OutQuad);
        animationB->setStartValue(QRect(titleLabel->x(),titleLabel->y(),titleLabel->width(),titleLabel->height()));
        animationB->setEndValue(QRect(titleLabel->x(),-300,titleLabel->width(),titleLabel->height()));
        animationB->start();

        delay.setSingleShot(true);
        delay.start(300);
    });
    connect(&delay,&QTimer::timeout,[=](){
        titleLabel->hide();
        startBtn->hide();
        infoBtn->show();
    });
    connect(infoBtn,&QPushButton::clicked,[=](){
        infoBtn->hide();
        gameWidget->startOneGame();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

