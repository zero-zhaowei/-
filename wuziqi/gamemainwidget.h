#ifndef GAMEMAINWIDGET_H
#define GAMEMAINWIDGET_H

#include <QMainWindow>

namespace Ui {
class gamemainwidget;
}

class gamemainwidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit gamemainwidget(QWidget *parent = 0);
    ~gamemainwidget();

    //重写绘图
    void paintEvent(QPaintEvent *event);
    //单击后获取当前坐标
    void mousePressEvent(QMouseEvent *event);
    void init();
    void huiqi();
    void restart();

private:
    Ui::gamemainwidget *ui;
    int ASIDE = 212;
    int SIZE = 600;
    int WIDTH = 75;
    int flag = 0,t = 0;
    //定义数组存放棋盘
    int chess[8][8];
    int h_flag[8][8];
    //定义数组表示八个方向
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    //显示下棋方
    bool whiteFlag = true;
    bool blackFlag = false;
    bool roleFlag[2]={false,false};
    //先后手
    int a = 0;
    int z=0;//记录t
    int tips = 0;
    int calu = 0;
};

#endif // GAMEMAINWIDGET_H
