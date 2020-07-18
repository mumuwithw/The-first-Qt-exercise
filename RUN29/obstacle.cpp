#include "obstacle.h"
#include <QDebug>
//////////////////////// Obstacle 障碍物 //////////////////////
Obstacle::Obstacle(int x,int y,int width,int height){
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
}
int Obstacle::getX(){
    return this->x;
}
int Obstacle::getY(){
    return this->y;
}
void Obstacle::setX(int x){
    this->x=x;
}
void Obstacle::setY(int y){
    this->y=y;
}
void Obstacle::executeMove(int dx,int dy){
    this->x+=dx;
    this->y+=dy;
}
int Obstacle::getWidth(){
    return this->width;
}
int Obstacle::getHeight(){
    return this->height;
}
bool Obstacle::isCollision(int px,int py,int pwidth,int pheight){return false;}
void Obstacle::move(){}
bool Obstacle::done(){return false;}
QPixmap Obstacle::getImg(){QPixmap a;return a;}

////////////////////// Wall 墙体 /////////////////////

Wall::Wall(int x,int y,int width,int height)
    :Obstacle(x,y,width,height){
    srand(time(NULL));
    this->img.load(QString(":/imgs/wall%1.png").arg(rand()%2+1));
}
QPixmap Wall::getImg(){
    return this->img;
}
bool Wall::isCollision(int px,int py,int pwidth,int pheight){
    QRect rect1(this->getX(),this->getY(),this->getWidth(),this->getHeight());
    QRect rect2(px,py,pwidth,pheight);
    return rect1.intersects(rect2);
}
void Wall::move(){
    this->executeMove(-5,0);
}
bool Wall::done(){
    if(this->getX()<=-this->getWidth()) return true;
    else return false;
}



////////////////////// Circle 圆形障碍 /////////////////////
Circle::Circle(int x,int y,int width,int height)
    :Obstacle(x,y,width,width){     //此处的xy表示左上角坐标
    this->img.load(":/imgs/circle.png");
    img.scaled(width,width);
    this->r=width/2;
    this->CollisionWidth=width*0.7;
    this->CollisionDX=width*0.15;
}
QPixmap Circle::getImg(){
    QMatrix m;
    degree+=2;
    if(degree>=360) degree=0;
    m.rotate(degree);
    return this->img.transformed(m, Qt::FastTransformation);
}
bool Circle::isCollision(int px,int py,int pwidth,int pheight){
    QRect rect1(this->getX()+CollisionDX,this->getY()+CollisionDX,CollisionWidth,CollisionWidth);
    QRect rect2(px,py,pwidth,pheight);
    return rect1.intersects(rect2);
}
void Circle::move(){
    this->executeMove(-7,0);
}
bool Circle::done(){
    if(this->getX()<=-this->getWidth()) return true;
    else return false;
}

////////////////////// Arrow 箭 /////////////////////
Arrow::Arrow(int x,int y,int width,int height)
:Obstacle(x,y,width,height){
    this->arrowImg.load(":/imgs/arrow.png");
    this->warningImg.load(":/imgs/arrowWarning.png");
    arrowImg.scaled(width,height);
    this->CollisionDX=290;
    this->CollisionWidth=110;
    this->CollisionDY=45;
    this->CollisionHeight=6;
}
QPixmap Arrow::getImg(){
    if(liveFrame<=occurFrame) return this->warningImg;
    else return this->arrowImg;
}
bool Arrow::isCollision(int px,int py,int pwidth,int pheight){
    QRect rect1(this->getX()+CollisionDX,this->getY()+CollisionDY,CollisionWidth,CollisionHeight);
    QRect rect2(px,py,pwidth,pheight);
    return liveFrame>=occurFrame && rect1.intersects(rect2);
}
void Arrow::move(){
    this->liveFrame++;
    if(this->liveFrame==this->occurFrame) this->setX(-400);
    if(this->liveFrame>=this->occurFrame) this->executeMove(40,0);
}
bool Arrow::done(){
    return this->getX()>1500;       //改窗口大小时候注意这里哦
}

