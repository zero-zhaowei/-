#include "gamemainwidget.h"
#include "ui_gamemainwidget.h"
#include "mainwindow.h"
#include<QPainter>
#include<QString>
#include<QMessageBox>
#include<QRadialGradient>
#include<QLabel>
#include<QBrush>
#include<QLabel>
#include<QSound>
#include<QPushButton>
#include<QMouseEvent>
#include<QDebug>
gamemainwidget::gamemainwidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gamemainwidget)
{
    ui->setupUi(this);
    this->resize(1024,600);
    this->setWindowTitle("游戏窗口");
    init();
    //设置先后手
    connect(ui->pushButton_w,&QPushButton::clicked,[&](){
        //只有第一次点击有效 白棋先手
          if(calu==0)
          {
              roleFlag[1] = (whiteFlag==false)?(whiteFlag=true):true;
              this->update();
              t=0;
              calu++;
          }
          else
          {
              QMessageBox::information(this,"操作不规范","正在游戏，不可选择先后手");
          }

    });
    connect(ui->pushButton_fanhui,&QPushButton::clicked,[=](){
        int ret=0;
        ret=QMessageBox::question(this,"提示!","确认退出吗，退出后将清楚游戏进度");
        if(ret==QMessageBox::Yes){
        this->hide();
        MainWindow *p=new MainWindow;
        p->show();
        }
        else
        {
            return;
        }
    });
    connect(ui->pushButton_b,&QPushButton::clicked,[&](){
        //只有第一次点击有效 黑棋先手
        if(calu==0)
        {
            roleFlag[0] = (blackFlag==false)?(blackFlag=true):true;
            //点击案件更新
            this->update();
            t=1;
            calu++;
        }
        else
        {
            QMessageBox::information(this,"操作不规范","正在游戏，不可选择先后手");
        }
    });
    connect(ui->pushButton_least,&QPushButton::clicked,[=](){
        huiqi();
    });
    connect(ui->pushButton_least,&QPushButton::clicked,[=](){
        restart();
    });

    //初始化棋盘为0 初始中间四颗棋子
    //空0 白1 黑2
}

gamemainwidget::~gamemainwidget()
{
    delete ui;
}

void gamemainwidget::paintEvent(QPaintEvent *event)
{
    //定义一个画家
    QPainter *painter = new QPainter(this);
    //画矩形棋盘外框
    painter->setPen(QPen(Qt::darkGreen,2,Qt::SolidLine));//钢笔工具：颜色，线号，实线
    //painter->setBrush(Qt::gray);//内部颜色填充
    painter->setPen(Qt::darkGreen);//线条颜色
    painter->drawRect(ASIDE,0,SIZE,SIZE);
    //画棋盘网格线
    for(int i = 0;i<8;i++)
    {
        painter->drawLine(ASIDE+WIDTH*i,0,ASIDE+WIDTH*i,SIZE);
    }
    for(int i = 0;i<8;i++)
    {
        painter->drawLine(ASIDE,WIDTH*i,ASIDE+SIZE,WIDTH*i);
    }


    //画棋子
    for(int i = 0;i<8;i++)
    {
        for(int j = 0;j<8;j++)
        {
            if(chess[i][j]==2)
            {
                painter->setBrush(Qt::black);
                painter->drawEllipse(QPoint(ASIDE+WIDTH*i+WIDTH/2,WIDTH*j+WIDTH/2),WIDTH/4,WIDTH/4);
            }
            else if(chess[i][j]==1)
            {
                painter->setBrush(Qt::white);
                painter->drawEllipse(QPoint(ASIDE+WIDTH*i+WIDTH/2,WIDTH*j+WIDTH/2),WIDTH/4,WIDTH/4);
            }
        }
    }
    //显示落子方
    if(roleFlag[0]==true && roleFlag[1]==false)
    {
        painter->setBrush(Qt::black);
        painter->drawEllipse(QPoint(100,250),40,40);
        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(100,350),30,30);
    }
    else if(roleFlag[1]==true && roleFlag[0]==false)
    {
        painter->setBrush(Qt::black);
        painter->drawEllipse(QPoint(100,250),30,30);
        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(100,350),40,40);
    }
    else if(roleFlag[1]==false && roleFlag[0]==false)
    {
        painter->setBrush(Qt::black);
        painter->drawEllipse(QPoint(100,250),30,30);
        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(100,350),30,30);
    }
    //提醒的开关控制
    connect(ui->pushButton_tips,&QPushButton::clicked,[&](){
        if(tips==0)
        {
            tips=1;
        }
        else if(tips==1)
        {
            tips=0;
        }
        this->update();

    });

if(tips)
{
    //提示
    for(int X = 0;X<8;X++)
    {
        for(int Y = 0;Y<8;Y++)
        {
            int tmpX = X,tmpY = Y;
            int i = 0;
            for(i = 0;i<8;i++)
            {
                //x,y在相应方向进行加减操作 进行遍历
                tmpX += dir[i][0];
                tmpY += dir[i][1];
                if((tmpX<8 && tmpX>=0 && tmpY<8 && tmpY>=0) && (chess[tmpX][tmpY]==((t+1)%2)+1) && chess[tmpX][tmpY]!=0)
                {
                    tmpX += dir[i][0];
                    tmpY += dir[i][1];
                    while (tmpX<8 && tmpX>=0 && tmpX<8 && tmpX>=0)
                    {
                        if(chess[tmpX][tmpY]==0)
                        {
                            break;
                        }
                        //遇到自己的棋子，当前方向不再遍历
                        if(chess[tmpX][tmpY]==((t%2)+1))
                        {
                            if(chess[X][Y]!=1 && chess[X][Y]!=2)
                            {
                                painter->setBrush(Qt::red);
                                painter->drawEllipse(QPoint(ASIDE+WIDTH*X+WIDTH/2,WIDTH*Y+WIDTH/2),5,5);
                            }
                            break;
                        }
                        //对当前方向遍历
                        tmpX += dir[i][0];
                        tmpY += dir[i][1];
                    }
                }
                //返回点击位置 准备下一次遍历
                tmpX = X;
                tmpY = Y;
            }
        }
    }
}

}

void gamemainwidget::mousePressEvent(QMouseEvent *event)
{


    //悔棋
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            h_flag[i][j]=chess[i][j];

        }
    }
    if(event->button() == Qt::LeftButton)
    {
        //若不选择先后手 不可开始游戏
        while(calu==0)
        {
            QMessageBox::information(this,"落子不规范","请选择先后手");
            return;
        }
        //确定落子方
        flag = (t%2)+1;
        if(flag==2)
        {
            roleFlag[0] = (blackFlag==false)?(blackFlag=true):true;
            roleFlag[1] = (whiteFlag==false)?(whiteFlag=false):false;
        }
        else if(flag==1)
        {
            roleFlag[1] = (whiteFlag==false)?(whiteFlag=true):true;
            roleFlag[0] = (blackFlag==false)?(blackFlag=false):false;
        }
        if(event->x()>=ASIDE && event->x()<=ASIDE+SIZE && event->y()>=0 && event->y()<=SIZE)
        {
            int chess_x=((event->x()-ASIDE)/75);
            int chess_y=(event->y()/75);
            //存储点击位置的坐标
            int tmpX = chess_x;
            int tmpY = chess_y;
            //有棋子则不可落子
            if((tmpX<8 && tmpX>=0 && tmpY<8 && tmpY>=0) && (chess[tmpX][tmpY]==1 || chess[tmpX][tmpY]==2))
            {
                QMessageBox::information(this,"落子不规范","当前区域已落子，请重新落子");
            }
            //落子的算法
            int count = 0;
            int i = 0;
            for(i = 0;i<8;i++)
            {
                //x,y在相应方向进行加减操作 进行遍历
                tmpX += dir[i][0];
                tmpY += dir[i][1];
                if((tmpX<8 && tmpX>=0 && tmpY<8 && tmpY>=0) && chess[tmpX][tmpY]==((t+1)%2)+1)
                {
                    tmpX += dir[i][0];
                    tmpY += dir[i][1];
                    while (tmpX<8 && tmpX>=0 && tmpX<8 && tmpX>=0)
                    {
                        if(chess[tmpX][tmpY]==0)
                        {
                            break;
                        }
                        //遇到自己的棋子，当前方向不再遍历
                        if(chess[tmpX][tmpY]==flag)
                        {
                            if(chess[chess_x][chess_y]!=1 && chess[chess_x][chess_y]!=2)
                            {
                                //吃子
                                chess[chess_x][chess_y]=flag;
                                QSound::play(":/sound/putchess.wav");
                                //落一次子，就算多个方向吃子，也只改变一次flag值，防止同一角色多次吃子
                                count++;
                                if(count==1)
                                {

                                    //更改落子方，+1改变flag值
                                    t++;
                                    z=t;
                                    bool tmp = roleFlag[0];
                                    roleFlag[0] = roleFlag[1];
                                    roleFlag[1] = tmp;
                                }
                                //更新
                                this->update();
                                //回溯遍历 从下一个子开始
                                tmpX -= dir[i][0];
                                tmpY -= dir[i][1];
                                while ((tmpX!=chess_x) || (tmpY!=chess_y))
                                {
                                //落子后，将此次遍历路径上的子反转为自己的子
                                chess[tmpX][tmpY]=flag;
                                tmpX -= dir[i][0];
                                tmpY -= dir[i][1];
                                }
                            }

                            break;
                        }
                        //对当前方向遍历
                        tmpX += dir[i][0];
                        tmpY += dir[i][1];
                    }
                }
                //返回点击位置 准备下一次遍历
                tmpX = chess_x;
                tmpY = chess_y;
            }
        }
        else
        {
            QMessageBox::information(this,"警告","正在游戏，请在棋盘区域落子");
        }
    }
    //判断胜负
    int wcount = 0,bcount = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(chess[i][j]==1)
            {
                wcount++;
            }
            else if(chess[i][j]==2)
            {
                bcount++;
            }
        }
    }
    if(wcount>bcount && (bcount==0 || (wcount+bcount)==64))
    {
        QMessageBox::information(this,"结果","白棋胜！");
    }
    else if(wcount<bcount && (wcount==0 || (wcount+bcount)==64))
    {
        QMessageBox::information(this,"结果","黑棋胜！");
    }
    //实时计数
    ui->label_b->setText(QString::number(bcount));
    ui->label_w->setText(QString::number(wcount));
    //绘图更新
    this->update();
}

void gamemainwidget::init()
{
    for(int i = 0;i<8;i++)
    {
        for(int j = 0;j<8;j++)
        {
            if(i==3 && j==3)
            {
                chess[i][j]=2;
            }
            else if(i==3 && j==4)
            {
                chess[i][j]=1;
            }
            else if(i==4 && j==3)
            {
                chess[i][j]=1;
            }
            else if(i==4 && j==4)
            {
                chess[i][j]=2;
            }
            else
            {
                chess[i][j]=0;
            }
        }
    }

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            h_flag[i][j]=0;

        }
    }
    whiteFlag = true;
     blackFlag = false;

    //先后手
     a = 0;
   z=0;//记录t
     tips = 0;
    calu = 0;
    flag = 0,t = 0;
}

void gamemainwidget::huiqi()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            chess[i][j]=h_flag[i][j];

        }
    }
    t--;
    if(z-t>1)
    {
        QMessageBox::information(this,"提示","只能悔棋一步");
    }
    this->update();
}

void gamemainwidget::restart()
{
    init();
}

