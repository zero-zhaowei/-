#include "gamewindow.h"
#include "ui_gamewindow.h"
#include"gamewindow.h"

gamewindow::gamewindow(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::gamewindow)
{
    ui->setupUi(this);
}

gamewindow::~gamewindow()
{
    delete ui;
}
