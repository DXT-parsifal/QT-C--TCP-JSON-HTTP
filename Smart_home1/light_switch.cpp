#include "light_switch.h"
#include "ui_light_switch.h"
#include <QDebug>
light_switch::light_switch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::light_switch)
{
    ui->setupUi(this);
    this->flag=0;//0 关 1 开
    //标签图片自适应
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap(":/res/灯关.png"));

    //按钮图片
    ui->pushButton->setIcon(QIcon(":/res/开.png"));
    //按键触发
    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        if(flag==0){
            ui->pushButton->setIcon(QIcon(":/res/关.png"));
            ui->label->setPixmap(QPixmap(":/res/灯开.png"));
            emit state_change("灯开");
            flag=1;
        }else if(flag==1){
            ui->pushButton->setIcon(QIcon(":/res/开.png"));
            ui->label->setPixmap(QPixmap(":/res/灯关.png"));
            emit state_change("灯关");
            flag=0;
        }
    });
}

light_switch::~light_switch()
{
    delete ui;
}
//接受信号改变
void light_switch::change_state(int change)
{
    if(change==1){
        ui->pushButton->setIcon(QIcon(":/res/关.png"));
        ui->label->setPixmap(QPixmap(":/res/灯开.png"));
        flag=1;
    }else if(change==0){
        ui->pushButton->setIcon(QIcon(":/res/开.png"));
        ui->label->setPixmap(QPixmap(":/res/灯关.png"));
        flag=0;
    }
//    emit ui->pushButton->clicked();
}
