#include "role.h"
#include <QString>
#include <QDebug>

//Role::Role(QWidget *parent) : QWidget(parent)
//{

//}

Role::Role(GameWidget *g){
    this->gamewidget=g;
    this->hp=3600;
    this->hpMax=3600;
    this->dashReady=true;
    this->x=100;
    this->y=100;
    this->width=30;
    this->height=40;
    this->curFace=FACE_RIGHT;
    this->maxX=gamewidget->width()-this->width;
    this->maxY=gamewidget->getGroundY()-this->height;
    //LoadImgs...
    //RUN 图片
    for(int i=0;i<5;i++)
    {
        runLeftImg[i].load(QString(":/imgs/run1_%1.png").arg(i+1));
        runRightImg[i].load(QString(":/imgs/run0_%1.png").arg(i+1));
    }
    //STAND 图片
    standLeftImg.load(":/imgs/standLeft.png");
    standRightImg.load(":/imgs/standRight.png");
    //JUMP 图片
    jumpLeftImg.load(":/imgs/jumpLeft.png");
    jumpRightImg.load(":/imgs/jumpRight.png");
    //DASH 图片
    for(int i=0;i<5;i++)
        dashBeforePosImg[i].load(QString(":/imgs/dash0_%1.png").arg(i+1));
    for(int i=0;i<9;i++)
        dashAfterPosImg[i].load(QString(":/imgs/dash1_%1.png").arg(i+1));

    //图片设置
    curRunImg=0;


    curState=STATE_RUN;
    //时钟设置
    //run时每帧时长
    runImgChangeTimer.setInterval(100);
    connect(&runImgChangeTimer,&QTimer::timeout,[=](){
        if(this->hp>0) curRunImg=(curRunImg+1)%3;
    });
    //jump时长
    jumpTimer.setInterval(jumpTime);
    jumpTimer.setSingleShot(true);
    connect(&jumpTimer,&QTimer::timeout,[=](){
        this->curState=STATE_RUN;
        //如果改这里，下方直接stop也要改的
    });
    //dash每帧时长
    dashImgTimer.setInterval(100);
    connect(&dashImgTimer,&QTimer::timeout,[=](){
        if(curDashImg<8)
            curDashImg++;
        else{
            inDashState=false;
            dashImgTimer.stop();
        }
    });
    //dashCD
    dashTimer.setInterval(dashFullCD);
    dashTimer.setSingleShot(true);
    connect(&dashTimer,&QTimer::timeout,[=](){
        dashReady=true;
        dashTimer.setInterval(dashFullCD);
    });
}
void Role::run(){
    runImgChangeTimer.start();
}

QPixmap Role::getImg(){
    QPixmap img;
//    if(isStanding){                       ////不要站立状态了还是
//        if(curFace==FACE_LEFT) img=standLeftImg;
//        else img=standRightImg;
//    }
//    else
    if(y>=maxY)
        if(curFace==FACE_LEFT) img=runLeftImg[curRunImg];
        else img=runRightImg[curRunImg];
    else    //在空中
        if(curFace==FACE_LEFT) img=jumpLeftImg;
        else img=jumpRightImg;
    return img;
}

void Role::move(bool up,bool down,bool left,bool right,bool dash,bool space){
    //qDebug()<<"space"<<space<<"up"<<up<<"left"<<left<<"right"<<right<<"dash"<<dash;
    if(this->y==maxY){
        fallSpeed=0;
        twiceJumpReady=true;
    }
    if(up){
        //在地上则可以进行一段跳
        if(this->y==maxY){
            jumpTimer.start();
            onceJumpDone=false;
        }
        //在空中，且一段跳结束，则可以进行二段跳
        if(this->y<maxY && twiceJumpReady && onceJumpDone){
            jumpTimer.start();
            twiceJumpReady=false;
            fallSpeed=0;
        }
        if(jumpTimer.isActive()) this->y-=jumpSpeed*jumpTimer.remainingTime()/jumpTime;
    }
    else{
        jumpTimer.stop();
        onceJumpDone=true;
    }
    if(down){
        this->y+=quickFallSpeed;
    }
    if(left){
        this->x-=runSpeed;
        this->curFace=FACE_LEFT;
    }
    if(right){
        this->x+=runSpeed;
        this->curFace=FACE_RIGHT;
    }
    if(dash && dashReady){
        dashBefoerX=this->x;
        dashBeforeY=this->y;
        if(space){
            jumpTimer.stop();
            if(left){
                this->x-=dashDistance*0.7;
                this->y-=dashDistance*0.7;
            }
            else if(right){
                this->x+=dashDistance*0.7;
                this->y-=dashDistance*0.7;
            }
            else
                this->y-=dashDistance;
        }
        else if(left){
            this->x-=dashDistance;
        }
        else if(right){
            this->x+=dashDistance;
        }
        else if(down){
            this->y=maxY;
        }
        else if(curFace==FACE_LEFT){
            this->x-=dashDistance;
        }
        else if(curFace==FACE_RIGHT){
            this->x+=dashDistance;
        }
        //dash 处理
        this->curState=STATE_DASH;
        inDashState=true;
        curDashImg=0;
        dashImgTimer.start();
        dashAfterX=this->x;
        dashAfterY=this->y;
        dashReady=false;
        fallSpeed=0;
        dashTimer.start();
    }
    //自然掉落
    this->y+=fallSpeed;
    //冲刺后有一小段滞空时间,操作舒适
    if(curDashImg<2 && curDashImg>=0) fallSpeed=0;
    //范围限制
    if(this->x>=maxX) this->x=maxX;
    if(this->x<0) this->x=0;
    if(this->y>=maxY) this->y=maxY;
    if(this->y<0) this->y=0;
    //不按上了就不跳了，所以掉落速度就开始递增了
    if(!jumpTimer.isActive()) fallSpeed+=fallSpeedDt;
    else fallSpeed=0;

    //判断是否在站立不动中
    if(this->y==maxY && !left && !right) isStanding=true;
    else isStanding=false;
    //时间加分
    this->score++;
}

int Role::getDashCDPercent(){
    if(dashReady){
       return 100;
    }
    else if(!dashTimer.isActive()){
        return 100-dashRemaining*100/dashFullCD;
    }
    else return 100-dashTimer.remainingTime()*100/dashFullCD;
}

void Role::reduceHp(){
    this->hp-=1200;
    if(this->hp<=0) this->hp=0;
}
int Role::getCurHp(){
    return this->hp;
}
int Role::getCurHpPercent(){
    return this->hp*100/this->hpMax;
}
void Role::increaseHp(int d){
    this->hp+=d;
    if(this->hp>this->hpMax) this->hp=this->hpMax;
}
bool Role::inDash(){
    return inDashState;
}
QPixmap Role::getDashBeforeImg(){       //form 9 to 5
    return dashBeforePosImg[curDashImg/2];
}
QPixmap Role::getDashAfterImg(){
    return dashAfterPosImg[curDashImg];
}

int Role::getDashBeforeX(){
    return this->dashBefoerX;
}
int Role::getDashBeforeY(){
    return this->dashBeforeY;
}
int Role::getDashAfterX(){
    return this->dashAfterX;
}
int Role::getDashAfterY(){
    return this->dashAfterY;
}

int Role::getX(){
    return this->x;
}

int Role::getY(){
    return this->y;
}
int Role::getWidth(){
    return this->width;
}
int Role::getHeight(){
    return this->height;
}
void Role::addScore(int s){
    this->score+=s;
}
int Role::getScore(){
    return this->score;
}
void Role::addDifficult(){
    this->difficult+=0.0015;
}
double Role::getDifficult(){
    return this->difficult;
}
void Role::pauseGame(){
    dashRemaining=dashTimer.remainingTime();
    if(dashRemaining!=-1) dashTimer.stop();
    dashImgTimer.stop();
    runImgChangeTimer.stop();
    jumpTimer.stop();
}
void Role::continueGame(){
    if(dashRemaining!=-1) dashTimer.start(dashRemaining);
    dashImgTimer.start();
    runImgChangeTimer.start();
}
