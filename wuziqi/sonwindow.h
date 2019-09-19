#ifndef SONWINDOW_H
#define SONWINDOW_H
#include<QTimer>
#include <QMainWindow>

#include <QWidget>

#include <QTimer>

#include <QFile>
//#include <QAudioFormat>
//#include <QAudioOutput>

#define GRID_NUMBER 8
#define MAX_GRID_NUMBER (GRID_NUMBER * GRID_NUMBER)
#define SHI 15
#define WHITE_FLAG() (role == White) ? (whiteFlag = false) : true
#define BLACK_FLAG() (role == Black) ? (blackFlag = false) : true
namespace Ui {
class SonWindow;
}

class SonWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SonWindow(QWidget *parent = 0);
    ~SonWindow();


    QPainter *paint;
    const int TIME=15;
    const int SIZE=8;
    const int WIDTH=40;
    const int x=20,y=20;
    int begin_flag=1;
    int BSIZEQI=30;
    int WSIZEQI=30;
    int max=0;
    int z=0;
    int time1=SHI;
    int time2=SHI;
    int btime,wtime;
    int num_w=2;
    int num_b=2;
    bool xianhou=false;
    int tmpX=0;
    int tmpY=0;
    int H_flag[8][8][64];//悔棋
     int P_flag[8][8];
    //用枚举标记黑白棋状态
    enum ChessFlag{Empty,Black,White};  //gridStatus
    //棋盘初始化
    void initChess();
    //切换角色
     void changeRole();
    //统计个数
     void countScore();
     void timerEvent(QTimerEvent *event);
void mousePressEvent2(QMouseEvent *ev);
    //吃子规则的算法函数：棋盘坐标(x,y)、棋子当前角色
    //eatChess默认为true即吃子，fasle为检测了否吃子
    //N*N的每一列（行）格数默认为8
     int judgeRole(int x, int y, ChessFlag currentRole, bool eatChess = true);
    //机器下子
     void machinePlay();
    //游戏结束
     void ren();
     void gameOver();
    //启动背景音乐
   //  void startVedio();
     void huiqi();
     void restart();
     void Bxainhou();
protected:
     void paintEvent(QPaintEvent *);//绘图事件：画棋盘，画棋子
     void mousePressEvent(QMouseEvent *);    //鼠标按下事件：获取当前点击的坐标、下子坐标
     void mouseMoveEvent(QMouseEvent *); //鼠标移动事件，用于移动窗口
     void timerEvent(QTimerEvent *event());

private slots:
    // void on_toolButtonQuit_clicked();

   //  void on_toolButtonstart_clicked();
private:
    Ui::SonWindow *ui;
    QPoint p;
    //起点坐标、终点坐标
    QPoint startPoint;
    QPoint endPoint;
    //每一格的高度与宽度
    int gridHigh=40;
    int gridWidth=40;
    int eatNum;
    //定义一个二维数组保存黑白棋的状态
    int chessStatus[8][8];

    //切换状态
    ChessFlag role; //有White和Black两种状态

    QTimer timer;       //定时器，为了保证体验感，不至于机器下子太快
    QFile inputFile;    //音频文件

    //音频调用的类对象
    //QAudioFormat audioFormat;//设置采样格式
   // QAudioOutput *audio;

    bool whiteFlag; //白棋是否落子的标志位，落子则置为true，没落子直接替换角色则置为false
    bool blackFlag; //黑棋是否落子的标志位

    int bNumber;    //最终的黑棋统计
    int wNumber;    //最终的白棋统计

    bool overFlag;  //结束的QMessageBox::about()是否调用过的标志
                    //因为QMessageBox::about()如果不检测调用次数，会影响交互性

   // bool xianhou=false;
};

class qi
{
public:
   // int qiInit(int x,int y,ChessFlag currentRole, bool eatChess);

};


#endif // SONWINDOW_H
