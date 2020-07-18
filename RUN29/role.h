#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include <QTimer>
#include "gamewidget.h"
class Role : public QWidget
{
private:
    int hp;
    int hpMax;
    int x,y,width,height;
    int maxX,maxY;
    int score=0;
    double difficult=1;

    bool dashReady;
    bool twiceJumpReady=false;
    bool onceJumpDone=false;
    bool isStanding=true;
    int dashFullCD=2000;
    int jumpTime=400;
    double fallSpeed=0;
    double fallSpeedDt=0.5;
    int runSpeed=6;
    int jumpSpeed=20;
    int quickFallSpeed=20;
    int dashDistance=300;

    GameWidget *gamewidget;
    enum {FACE_LEFT,FACE_RIGHT}curFace;
    enum {STATE_RUN,STATE_JUMP,STATE_DASH}curState;
        //目前用途：冲刺时，冲刺结束还是dash状态   跳跃结束是设置为run,但并没有落地    图片更改目前未根据state(也就是说目前state没有用途)
    //RUN 图片    40*50
    int curRunImg=0;
    QPixmap runLeftImg[5];
    QPixmap runRightImg[5];
    //STAND 图片
    QPixmap standLeftImg;
    QPixmap standRightImg;
    //JUMP 图片   40*50
    QPixmap jumpLeftImg,jumpRightImg;
    //DASH 图片   before: 36*50  after:100*70
    bool inDashState=false;
    int curDashImg=8;
    int dashBefoerX,dashBeforeY;
    int dashAfterX,dashAfterY;
    QPixmap dashBeforePosImg[5],dashAfterPosImg[9];

    QTimer runImgChangeTimer,jumpTimer,dashTimer,dashImgTimer;
    int dashRemaining=0;
    Q_OBJECT

public:
//    explicit Role(QWidget *parent = nullptr);
    Role(GameWidget *g);
    void run();
    //HP相关
    void reduceHp();
    void increaseHp(int d);
    int getCurHp();
    int getCurHpPercent();

    QPixmap getImg();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    void move(bool up,bool down,bool left,bool right,bool dash,bool space);
    int getDashCDPercent();
    bool inDash();
    QPixmap getDashBeforeImg();
    QPixmap getDashAfterImg();
    int getDashBeforeX();
    int getDashBeforeY();
    int getDashAfterX();
    int getDashAfterY();
    void addScore(int s);
    int getScore();
    void addDifficult();
    double getDifficult();
    void pauseGame();
    void continueGame();
signals:

};

#endif // ROLE_H
