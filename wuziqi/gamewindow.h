#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include<QPushButton>

namespace Ui {
class gamewindow;
}

class gamewindow : public QPushButton
{
    Q_OBJECT

public:
    explicit gamewindow(QPushButton *parent = 0);
    ~gamewindow();

private:
    Ui::gamewindow *ui;
};

#endif // GAMEWINDOW_H
