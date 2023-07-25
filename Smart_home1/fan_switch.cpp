#include "fan_switch.h"
#include "ui_fan_switch.h"

fan_switch::fan_switch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fan_switch)
{
    ui->setupUi(this);
    this->flag=0;//0 关 1 开
    //标签图片自适应
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap(":/res/电风扇关.png"));

    //按钮图片
    ui->pushButton->setIcon(QIcon(":/res/开.png"));

    connect(ui->pushButton,&QPushButton::clicked,this,[=](){
        if(flag==0){
            ui->pushButton->setIcon(QIcon(":/res/关.png"));
            ui->label->setPixmap(QPixmap(":/res/风扇开.png"));
            emit state_change("空调开");
            flag=1;
        }else if(flag==1){
            ui->pushButton->setIcon(QIcon(":/res/开.png"));
            ui->label->setPixmap(QPixmap(":/res/电风扇关.png"));
            emit state_change("空调关");
            flag=0;
        }
    });
}

fan_switch::~fan_switch()
{
    delete ui;
}

void fan_switch::change_state(int change)
{
    if(change==1){
        ui->pushButton->setIcon(QIcon(":/res/关.png"));
        ui->label->setPixmap(QPixmap(":/res/风扇开.png"));
        flag=1;
    }else if(change==0){
        ui->pushButton->setIcon(QIcon(":/res/开.png"));
        ui->label->setPixmap(QPixmap(":/res/电风扇关.png"));
        flag=0;
    }
}
