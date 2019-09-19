#ifndef SON_H
#define SON_H

#include <QMainWindow>
#include<QPushButton>

class Son : public QMainWindow
{
    Q_OBJECT
public:
    explicit Son(QWidget *parent = 0);
    void sendslot();

signals:
    void myslot();
public slots:
};

#endif // SON_H
