#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <vector>
#include <obstacle.h>
class Role;
class GameWidget : public QWidget
{
    Q_OBJECT
private:
    bool pauseState=false;
    Role *r=NULL;
    std::vector<Obstacle*> curObstacle;
    bool gameIsRun=false,GameOver=false;
    bool upKey=false,downKey=false,leftKey=false,rightKey=false,dashKey=false,spaceKey=false;
    QTimer fps_60;
    int groundY;
    int hurtImgAlpha=0;

    QPixmap pauseImg;
    QPixmap dashIconImg;
    QPixmap hurtImg;
    QPixmap groundImg;
    double groundImgSpeed=5;
    double curgroundImgX=0;
    QPixmap bkgImg[3];
    double bkgImgSpeed[3]={0.15,1,0.5};
    double curBkgImgX[3]={0,200,0};
    QPixmap gameOverImg;
    enum obstacleSort{SORT_WALL=1,SORT_CIRCLE,SORT_Arrow};


public:
    explicit GameWidget(QWidget *parent = nullptr,int w=800,int h=600);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void startOneGame();
    int getGroundY();
    void addObstacles();
    void addAObstacle();
    void gameIsOver();
    void pauseGame();
    void continueGame();

signals:

};

#endif // GAMEWIDGET_H
