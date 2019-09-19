#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"sonwindow.h"
#include "gamemainwidget.h"
#include<QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        this->hide();
        SonWindow *son=new SonWindow;
        son->show();

    });
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        this->hide();
        gamemainwidget *p=new gamemainwidget();
        p->show();

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
