#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QPixmap>
#include <QPropertyAnimation>
class GameButton : public QPushButton
{
private:
    QPixmap img1,img2,img3;
    bool inTheButton=false;
    QPropertyAnimation *a=nullptr,*pos=nullptr;
    Q_OBJECT
public:
//    explicit GameButton(QWidget *parent = nullptr);
    GameButton(QString path1,QString path2,QString path3);
    ~GameButton();
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void onClicked();

signals:

};

#endif // GAMEBUTTON_H
