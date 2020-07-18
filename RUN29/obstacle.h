#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <QPixmap>
#include <QString>
class gamewidget;
class Obstacle
{
private:
    int x,y;
    int width,height;
public:
    Obstacle(int x,int y,int width,int height);
    virtual bool isCollision(int px,int py,int pwidth,int pheight);
    virtual void move();
    void executeMove(int dx,int dy);
    virtual QPixmap getImg();
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    int getWidth();
    int getHeight();
    virtual bool done();

};

class Wall:public Obstacle
{
private:
    QPixmap img;
public:
    Wall(int x,int y,int width,int height);
    virtual QPixmap getImg();
    virtual bool isCollision(int px,int py,int pwidth,int pheight);
    virtual void move();
    virtual bool done();
};

class Circle:public Obstacle
{
private:
    QPixmap img;
    int r;
    int CollisionDX,CollisionWidth; //碰撞检测
    bool degreeUP=true;             //旋转角度及顺逆时针
    int degree=0;
public:
    Circle(int x,int y,int width,int height);
    virtual QPixmap getImg();
    virtual bool isCollision(int px,int py,int pwidth,int pheight);
    virtual void move();
    virtual bool done();
};

class Arrow:public Obstacle
{
private:
    QPixmap warningImg;
    QPixmap arrowImg;
    int r;
    int CollisionDX,CollisionWidth; //碰撞检测
    int CollisionDY,CollisionHeight; //碰撞检测
    int liveFrame=0,occurFrame=60;
public:
    Arrow(int x,int y,int width,int height);
    virtual QPixmap getImg();
    virtual bool isCollision(int px,int py,int pwidth,int pheight);
    virtual void move();
    virtual bool done();
};
#endif // OBSTACLE_H
