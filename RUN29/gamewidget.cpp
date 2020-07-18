#include "gamewidget.h"
#include "role.h"
#include <QDebug>
GameWidget::GameWidget(QWidget *parent,int w,int h) : QWidget(parent){
    srand(time(NULL));
    this->resize(w,h);
    gameIsRun=0;
    this->groundY=h-70;
    //Load Imgs
    bkgImg[0].load(":/imgs/startBkg.png");
    bkgImg[1].load(":/imgs/startBkg2.png");
    bkgImg[2].load(":/imgs/foggy.png");
    groundImg.load(":/imgs/ground.png");
    gameOverImg.load(":/imgs/gameover.png");
    hurtImg.load(":/imgs/hurt.png");
    dashIconImg.load(":/imgs/dashIcon.png");
    pauseImg.load(":/imgs/pause.png");
    fps_60.setInterval(17);
    this->grabKeyboard();
    connect(&fps_60,&QTimer::timeout,[=](){
        if(gameIsRun){
            r->addDifficult();
            r->move(upKey,downKey,leftKey,rightKey,dashKey,spaceKey);
            for(auto i =curObstacle.begin();i!=curObstacle.end();)
            {
                if((*i)->done()) i=curObstacle.erase(i);
                else{
                    if((*i)->isCollision(r->getX()+5,r->getY()+5,r->getWidth()-10,r->getHeight()-10)){
                        r->reduceHp();
                        hurtImgAlpha=255;
                        curObstacle.clear();
                        break;
                    }
                    (*i)->move();
                    i++;
                }
            }
            if(r->getCurHp()<=0) gameIsOver();
            if(!GameOver){
                addObstacles();
                r->increaseHp(1);
            }
        }
        update();
    });
}

void GameWidget::paintEvent(QPaintEvent *event){
    QPainter p(this);
    //背景绘制
    for(int i=0;i<3;i++)
    {
        p.drawPixmap(QRect(0,0,this->width(),this->height())
                     ,bkgImg[i]
                     ,QRect(curBkgImgX[i],0,this->width(),this->height()));
        if(curBkgImgX[i]>bkgImg[i].width()-this->width())
            p.drawPixmap(QRect(bkgImg[i].width()-curBkgImgX[i],0,this->width()-(bkgImg[i].width()-curBkgImgX[i]),this->height())
                         ,bkgImg[i]
                         ,QRect(0,0,this->width()-(bkgImg[i].width()-curBkgImgX[i]),this->height()));
        if(curBkgImgX[i]>=bkgImg[i].width())
                curBkgImgX[i]-=bkgImg[i].width();
        if(gameIsRun) curBkgImgX[i]+=bkgImgSpeed[i];
    }
    //受伤绘制
    if(hurtImgAlpha!=0) {
        QPixmap temp(hurtImg.size());
        temp.fill(Qt::transparent);
        QPainter p2(&temp);
        p2.setCompositionMode(QPainter::CompositionMode_Source);
        p2.drawPixmap(0,0,hurtImg);
        p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        p2.fillRect(temp.rect(),QColor(0,0,0,hurtImgAlpha));
        p.drawPixmap(QRect(0,0,this->width(),this->height()),temp);
        hurtImgAlpha-=3;
    }

    if(gameIsRun || GameOver)   //只是在开始第一局游戏前不走这个if
    {
        //人物绘制
        p.drawPixmap(QRect(r->getX(),r->getY(),r->getWidth(),r->getHeight()),r->getImg());
        //若在冲刺，绘制冲刺图案
        if(r->inDash())
        {
            p.drawPixmap(QRect(r->getDashBeforeX()-10,r->getDashBeforeY()-20,36,50),r->getDashBeforeImg());
            p.drawPixmap(QRect(r->getDashAfterX()-80,r->getDashAfterY()-75,200,140),r->getDashAfterImg().scaled(200,140));
        }
        //血量绘制
        p.drawRect(QRect(1000,50,150,10));
        p.fillRect(QRect(1000,50,r->getCurHpPercent()*150/100,10),Qt::red);
        p.drawLine(1050,50,1050,60);
        p.drawLine(1100,50,1100,60);
        QPen pen1(Qt::black);
        p.setFont(QFont("黑体",20));
        pen1.setColor(Qt::black);
        p.setPen(pen1);
        p.drawText(1000,100,QString("HP:%1%").arg(r->getCurHpPercent()));
        //分数绘制
        p.drawText(500,100,QString("Score:%1").arg(r->getScore()));


        //冲刺CD绘制
        QPen pen2(Qt::DotLine);
        pen2.setWidth(10);
        pen2.setWidth(10);
        pen2.setColor(Qt::gray);
        p.setPen(pen2);
        p.drawLine(QLine(QPoint(50,50),QPoint(250,50)));
        pen2.setColor(Qt::green);
        p.setPen(pen2);
        p.drawLine(QLine(QPoint(50,50),QPoint(50+r->getDashCDPercent()*200/100,50)));
        p.drawPixmap(270,30,dashIconImg);

        //障碍物绘制
        for(auto i=curObstacle.begin();i!=curObstacle.end();i++)
        {
            p.drawPixmap(QRect((*i)->getX(),(*i)->getY(),(*i)->getWidth(),(*i)->getHeight())
                         ,(*i)->getImg());
        }
    }

    //地面绘制
//    p.drawPixmap(QRect(0,this->groundY,this->width(),this->height()-this->groundY),groundImg
//                 ,QRect(0,0,this->width(),this->height()-this->groundY));
    p.drawPixmap(QRect(0,this->groundY,this->width(),this->height()-this->groundY)
                 ,groundImg
                 ,QRect(curgroundImgX,0,this->width(),this->height()-this->groundY));
    if(curgroundImgX>groundImg.width()-this->width())
        p.drawPixmap(QRect(groundImg.width()-curgroundImgX,this->groundY,this->width()-(groundImg.width()-curgroundImgX),this->height()-this->groundY)
                     ,groundImg
                     ,QRect(0,0,this->width()-(groundImg.width()-curgroundImgX),this->height()-this->groundY));
    if(curgroundImgX>=groundImg.width())
            curgroundImgX-=groundImg.width();
    if(gameIsRun) curgroundImgX+=groundImgSpeed;
    //GameIsOver绘制
    if(GameOver){
        p.drawPixmap(QRect(240,60,800,600),gameOverImg);
    }
    //暂停绘制
    if(pauseState){
        p.drawPixmap((this->width()-pauseImg.width())/2,(this->height()-pauseImg.height())/2,pauseImg);
    }
}
void GameWidget::startOneGame(){
    delete(r);
    curObstacle.clear();
    GameOver=false;
    r=new Role(this);
    gameIsRun=true;

    r->run();
    fps_60.start();
    update();
}
void GameWidget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Up)
        this->upKey=true;
    else if(event->key()==Qt::Key_Down)
        this->downKey=true;
    else if(event->key()==Qt::Key_Left)
        this->leftKey=true;
    else if(event->key()==Qt::Key_Right)
        this->rightKey=true;
    else if(event->key()==Qt::Key_Q)
        this->dashKey=true;
    else if(event->key()==Qt::Key_Space)
        this->spaceKey=true;
    else if(GameOver && event->key()==Qt::Key_R)
        startOneGame();
    else return QWidget::keyPressEvent(event);
}
void GameWidget::keyReleaseEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Up)
        this->upKey=false;
    else if(event->key()==Qt::Key_Down)
        this->downKey=false;
    else if(event->key()==Qt::Key_Left)
        this->leftKey=false;
    else if(event->key()==Qt::Key_Right)
        this->rightKey=false;
    else if(event->key()==Qt::Key_Q)
        this->dashKey=false;
    else if(event->key()==Qt::Key_Space)
        this->spaceKey=false;
    else if(gameIsRun && event->key()==Qt::Key_Escape)
        if(pauseState==false) pauseGame();
        else continueGame();
    else return QWidget::keyPressEvent(event);
}

void GameWidget::addObstacles(){
    static int afterLastWallTime;
    enum obstacleSort sort;
    sort=(enum obstacleSort)(rand()%3+1);       //这里改障碍物种类数目哦~
//    sort=SORT_WALL;
    if(sort==SORT_WALL){
        if(rand()%600<=20*r->getDifficult() && afterLastWallTime>=90){
            int x=this->width()+20;
            int y=rand()%(int)(this->groundY*0.3)+this->groundY*0.6;    //(0.1~0.4)
            curObstacle.push_back((Obstacle*)new Wall(x,y,70,this->groundY-y));
            afterLastWallTime=0;
        }
    }else if(sort==SORT_CIRCLE){
        if(rand()%600<=5*r->getDifficult()){
            int x=this->width()+150;
            int y=rand()%this->groundY-50;
            int r=rand()%70+50;
            curObstacle.push_back((Obstacle*)new Circle(x,y,r,r));
        }
    }else if(sort==SORT_Arrow){
        if(rand()%600<=5*r->getDifficult()){
            int x=0;
            int y=rand()%this->groundY*0.6+this->getGroundY()*0.3;
            curObstacle.push_back((Obstacle*)new Arrow(x,y,400,100));
        }
    }
    afterLastWallTime++;
}
void GameWidget::addAObstacle(){

}

void GameWidget::gameIsOver(){
    gameIsRun=false;
    GameOver=true;
}

int GameWidget::getGroundY(){
    return this->groundY;
}
void GameWidget::pauseGame(){
    fps_60.stop();
    r->pauseGame();
    update();
    pauseState=true;
}
void GameWidget::continueGame(){
    fps_60.start();
    r->continueGame();
    pauseState=false;
    update();
}
