#include "sonwindow.h"
#include "ui_sonwindow.h"

#include<QPushButton>
#include<QPainter>
#include<QMouseEvent>
#include<QBrush>
#include<QPaintEvent>
#include<QPalette>
#include<QMessageBox>
#include<QDebug>
#include<QPainter>
#include"mainwindow.h"
#include <QPaintEvent>

//#include <QMediaPlayer>
#include <string.h>
SonWindow::SonWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SonWindow)
{
    ui->setupUi(this);

   // ui->label_qipan->update();


    gridWidth = 40;
    gridHigh = 40;

    initChess();
    connect(&timer, &QTimer::timeout, this, &SonWindow::machinePlay);

    connect(ui->pushButton_5,&QPushButton::clicked,[=](){
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


    connect(ui->pushButton_re,&QPushButton::clicked,[=](){
        SonWindow::restart();
    });

}


SonWindow::~SonWindow()
{
    delete ui;
}



void SonWindow::paintEvent(QPaintEvent *)
{
    paint=new QPainter;
    paint->begin(this);
    paint->setPen(QPen(Qt::darkGreen,2,Qt::SolidLine));//钢笔工具：颜色，线号，实线
      //画SIZE+1条横线
      for(int i=0;i<9;i++)
        {
          paint->drawLine(x,y+WIDTH*i,x+WIDTH*(SIZE),y+WIDTH*i);//画线函数：x1,y1,x2,y2:画从(x1,y1)到(x2,y2)的线
        }
      //画SIZE+1条竖线
      for(int i=0;i<9;i++)
        {
          paint->drawLine(x+WIDTH*i,y,x+WIDTH*i,y+WIDTH*(SIZE));
        }
      //int num=0;
        num_w=0;
        num_b=0;
      int startX = 0;
      int startY = 0;
      //根据棋盘二维数组的状态画棋子
      for(int i = 0; i < 8; i++){
          for(int j = 0; j < 8; j++){
              startX = x + i*(gridWidth)+10;
              startY = y + j*(gridHigh)+10;
              if(chessStatus[i][j] == Black){


                  //画黑子
                  num_b++;
                  ui->label_bcount->setText(QString::number(num_b));
                  paint->setBrush(QBrush(Qt::black,Qt::SolidPattern));//毛刷：颜色，实图案
                  paint->drawEllipse(startX,startY,WIDTH/2,WIDTH/2);

              }
              else if(chessStatus[i][j] == White){



                  //画白子
                  num_w++;
                  ui->label_wcount->setText(QString::number(num_w));
                  paint->setBrush(QBrush(Qt::white,Qt::SolidPattern));//毛刷：颜色，实图案
                  paint->drawEllipse(startX,startY,WIDTH/2,WIDTH/2);
              }
#if 0
              if (P_flag[i][j]==1)
              {
                  paint->setBrush(QBrush(Qt::red,Qt::SolidPattern));//毛刷：颜色，实图案
                  paint->drawEllipse(startX,startY,WIDTH/10,WIDTH/10);
              }
#endif
          }
      }



      paint->setBrush(QBrush(Qt::black,Qt::SolidPattern));//毛刷：颜色，实图案
      paint->drawEllipse(x+WIDTH,y+WIDTH*SIZE+WIDTH,BSIZEQI,BSIZEQI);
      paint->setBrush(QBrush(Qt::white,Qt::SolidPattern));
       paint->drawEllipse(x+WIDTH*SIZE-2*WIDTH,y+WIDTH*SIZE+WIDTH,WSIZEQI,WSIZEQI);

        paint->end();
}



void SonWindow::mousePressEvent(QMouseEvent *ev)
{

    if((ev->x()>x)&&(ev->x()<(x+WIDTH*8))&&(ev->y()>x)&&(ev->y()<(x+WIDTH*8)))
    {
#if 0
        if(xianhou==false)
        {
            QMessageBox::information(this,"提示","请先选择先后手！");
             return;
        }

#endif
         int chess_x=((ev->x()+20)/40)-1;
         int chess_y=((ev->y()+20)/40)-1;
         tmpX = chess_x;
         tmpY = chess_y;



        countScore();

         //触发定时器
                  this->wtime=this->startTimer(1000);

                  time1=SHI;

                  this->update();


    }



    if(ev->button() == Qt::LeftButton){
        p = ev->globalPos() - this->frameGeometry().topLeft();
    }

    int i = 0, j = 0;
    i = ev->x() -x;
    j = ev->y() - y;

    if(i >= 0 && i <= 8*gridWidth && j >= 0 && j <= 8*gridHigh){
        //点击点在正确的棋盘内
        i = i/gridWidth;
        j = j/gridHigh;

        if( judgeRole(i,j,role) > 0 ){//点击落子成功
            whiteFlag = true;//落子成功则将白子标志位置为true
            changeRole();//改变角色
            update();//更新绘图
        }
    }
}



int SonWindow::judgeRole(int x, int y, ChessFlag currentRole, bool eatChess)
{
    //落子的八个方向
    int dir[8][2] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int tmpX = x,tmpY = y;              //临时保存棋盘坐标位置
    int i = 0;
    eatNum = 0;      //初始化数据

    if(chessStatus[tmpX][tmpY] != Empty){//如果此方格中已经有棋子无效操作，直接返回
        return 0;
    }

    //棋盘的8个方向
    for(i=0; i<8; i++){
        //从鼠标点击点开始
        tmpX = x;
        tmpY = y;
        //确定一个方向：dir[i][0]
        tmpX += dir[i][0];
        tmpY += dir[i][1];

        if(tmpX < GRID_NUMBER && tmpX >= 0 && tmpY < GRID_NUMBER && tmpY >= 0
            && (chessStatus[tmpX][tmpY] != currentRole) && (chessStatus[tmpX][tmpY] != Empty)){

            //如果没有出界并且相邻棋子是对方棋子才有可能吃子
            tmpX += dir[i][0];
            tmpY += dir[i][1];//向前走一步，开始判断该方向还有无自己棋子

            while(tmpX < GRID_NUMBER && tmpX >= 0 && tmpY < GRID_NUMBER && tmpY >= 0){
                if(chessStatus[tmpX][tmpY] == Empty){
                    break;//遇到空位跳出循环，外部if最后一个条件也会不满足，则直接判断下一个方向
                }
                if(chessStatus[tmpX][tmpY] == currentRole){//找到自己的棋子代表可以吃子
                    //能吃子则点击点标记为自己的棋子，update后是自己的棋子，否则点击处不能落子
                    (true == eatChess) ? (chessStatus[x][y] = currentRole) : true;
                    tmpX -= dir[i][0];
                    tmpY -= dir[i][1];//回退一步开始吃子
                    //没有回到开始的位置就继续执行
                    while((tmpX != x) || (tmpY != y)){//没有回退到点击处则继续修改原有棋子状态
                        //若为true则为落子，修改为自己的棋子,如果为false则为测试，不用修改
                        (true == eatChess) ? (chessStatus[tmpX][tmpY] = currentRole) : true;
                        tmpX -= dir[i][0];
                        tmpY -= dir[i][1];//回退一格，继续
                        eatNum++;   //吃子累计
                    }
                    break;//跳出循环，结束该方向的判断
                }
                //没找到自己的棋子就向前（指定方向）走一步，走到0或GRID_NUMBER边界条件时就结束该层if语句
                tmpX += dir[i][0];
                tmpY += dir[i][1];
            }
        }
    }
    return eatNum;
}
void SonWindow::mouseMoveEvent(QMouseEvent *ev)
{
    //如果鼠标事件中包含“按住的是左键”，需要移动窗口（在移动事件处理函数中必然有移动事件）
    if( ev->buttons() & Qt::LeftButton){
        move(ev->globalPos() - p);
    }
}

void SonWindow::timerEvent(QTimerEvent *event)
{

        if(event->timerId()==this->wtime)
        {
            ui->label_w->setText(QString::number(time1--));

            ui->label_b->setText(QString::number(0));

        }


                this->update();


    }





void SonWindow::changeRole()
{
    if(role == Black){
        role = White;
//        ui->label_w->show();
//        ui->label_b->hide();
    }
    else{
        role = Black;
//        ui->label_b->hide();
//        ui->label_w->show();
    }

    if(role == Black){//为黑子时，则由机器下子
        timer.start(1000);//machinePlay();
    }
    //统计个数
    countScore();
}

void SonWindow::countScore()
{
    bNumber = 0;
    wNumber = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            (!(chessStatus[i][j] - Black))
                    ? bNumber++ : ((!(chessStatus[i][j] - White)) ? wNumber++ : true);
        }
    }
    //记分规则：根据落子数与落子占比数来计算得分
    ui->label_wcount->setText(QString::number((wNumber)*(MAX_GRID_NUMBER-bNumber)));
    ui->label_bcount->setText(QString::number((bNumber)*(MAX_GRID_NUMBER-wNumber)));

    //判断能落子的最大值
    int num = 0,max = 0;
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){//判断能吃的位置
            if( num = judgeRole(i, j, role, false) > max){//寻找最多吃子的位置
                max = num;
                P_flag[i][j]=1;

            }
        }
    }

    //最大吃0个子，即不能落子，则直接判断胜负
    if(max == 0){
        if(blackFlag | whiteFlag){
            WHITE_FLAG();
            BLACK_FLAG();
            changeRole();
            update();
        }
        else{
            if(overFlag == false){
                overFlag = true;
                //修改标志位并弹出结果提示信息，在函数返回时另一个gameOver()就不会执行了
                gameOver();
            }
        }
    }
}




void SonWindow::gameOver()
{
    if(wNumber > bNumber){
        QMessageBox::about(this,"白棋嬴","白棋嬴");
    }
    else if(wNumber < bNumber){
        QMessageBox::about(this,"黑棋嬴","黑棋嬴");
    }
    else if(wNumber == bNumber){
        QMessageBox::about(this,"平局","黑棋嬴");
    }
}

void SonWindow::machinePlay()
{
    //timer.stop();//定时器停止

    int max = 0, num = 0;
    int px = 0, py = 0;

    //判断能落子的最大值
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            //判断能吃的位置,机器为黑子
            if( num = judgeRole(i, j, role, false) > max){//寻找最多吃子的位置
                max = num;
                px = i;
                py = j;
            }
        }
    }
    if(max == 0){//没有可走的了
        if((blackFlag | whiteFlag)){
            WHITE_FLAG();
            BLACK_FLAG();
            changeRole();
            return;
        }
        else{
            if(overFlag == false){
                overFlag = true;
                //修改标志位并弹出结果提示信息，在函数返回时另一个gameOver()就不会执行了
                gameOver();
            }
        }
    }

    //吃子
    if(judgeRole(px, py, Black) > 0){
        blackFlag = true;   //机器落子则将黑子标志位置为true
        changeRole();
        update();
    }
}

void SonWindow::ren()
{

    int max = 0, num = 0;
    int px = 0, py = 0;

    //判断能落子的最大值
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            //判断能吃的位置,机器为黑子
            if( num = judgeRole(i, j, role, false) > max){//寻找最多吃子的位置
                max = num;
                px = i;
                py = j;
            }
        }
    }
    if(max == 0){//没有可走的了
        if((blackFlag | whiteFlag)){
            WHITE_FLAG();
            BLACK_FLAG();
            changeRole();
            return;
        }
        else{
            if(overFlag == false){
                overFlag = true;
                //修改标志位并弹出结果提示信息，在函数返回时另一个gameOver()就不会执行了
                gameOver();
            }
        }
    }
    changeRole();
    update();


}







void SonWindow::initChess()
{
    whiteFlag = blackFlag = true;//一开始双方都能落子
        overFlag = false;   //一开始游戏不结束
        bNumber = wNumber = 0;
        memset(chessStatus,0,sizeof(chessStatus));
        //中间四子：两白两黑
        chessStatus[3][3] = Black;
        chessStatus[4][3] = White;
        chessStatus[3][4] = White;
        chessStatus[4][4] = Black;
        role = White;   //默认白子先下



//        for(int i=0;i<8;i++)
//        {
//            for(int j=0;j<8;j++)
//            {
//                H_flag[i][j][]=0;
//               // qDebug()<<flag[i][j]<<endl;
//                //
//            }
//        }
        ui->label_bcount->setText(QString::number(num_b));
        ui->label_wcount->setText(QString::number(num_w));

}
void SonWindow::huiqi()
{
    //initChess();
    for(int m;m<begin_flag;m++)
    {
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                chessStatus[i][j]=H_flag[i][j][m];

            }
        }
    }

    begin_flag--;
     this->update();
}

void SonWindow::restart()
{
    SonWindow::initChess();

    begin_flag=1;
    num_w=2;
    num_b=2;
    BSIZEQI=30;
    WSIZEQI=30;
    time1=1;
    time2=1;
    xianhou=false;
    this->update();
}

void SonWindow::Bxainhou()
{
    if(xianhou==false)
    {
        xianhou=true;
    }
    else if(xianhou==true){
        xianhou=false;

    }
}
