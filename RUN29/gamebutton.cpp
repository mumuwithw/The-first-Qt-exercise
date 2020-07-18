#include "gamebutton.h"
#include <QDebug>
//GameButton::GameButton(QWidget *parent) : QWidget(parent)
//{

//}

GameButton::GameButton(QString path1,QString path2,QString path3){
    img1.load(path1);
    img2.load(path2);
    img3.load(path3);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setFixedSize(QSize(img1.width(),img1.height()));
    this->setIcon(img1);
    this->setIconSize(QSize(img1.width(),img1.height()));
}
GameButton::~GameButton(){
    delete a;
    delete pos;
}

void GameButton::onClicked(){
    a=new QPropertyAnimation(this,"geometry");
    a->setDuration(1000);
    a->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    a->setEndValue(QRect(this->x(),750,this->width(),this->height()));

    a->start();
}

void GameButton::mousePressEvent(QMouseEvent *e){
    this->move(this->x(),this->y()+25);
    QPushButton::mousePressEvent(e);
}
void GameButton::mouseReleaseEvent(QMouseEvent *e){
    this->move(this->x(),this->y()-25);
    QPushButton::mouseReleaseEvent(e);
}
void GameButton::enterEvent(QEvent *e){
    inTheButton=true;
    this->setFixedSize(QSize(img2.width(),img2.height()));
    this->setIcon(img2);
    this->setIconSize(QSize(img2.width(),img2.height()));
    this->move(this->x()-50,this->y()-50);
}
void GameButton::leaveEvent(QEvent *e){
    if(!inTheButton) return;
    this->setFixedSize(QSize(img1.width(),img1.height()));
    this->setIcon(img1);
    this->setIconSize(QSize(img1.width(),img1.height()));
    this->move(this->x()+50,this->y()+50);
    inTheButton=false;
}
