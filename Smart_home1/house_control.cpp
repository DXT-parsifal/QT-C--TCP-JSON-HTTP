#include "house_control.h"
#include "ui_house_control.h"
#include "video_monitor.h"

house_control::house_control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::house_control)
{
    ui->setupUi(this);
    ui->widget_20->setStyleSheet("background-color:rgb(172, 172, 172);");

    //0~9灯设置大小
    ui->widget->resize(QSize(70,100));
    ui->widget_2->resize(QSize(70,100));
    ui->widget_3->resize(QSize(70,100));
    ui->widget_4->resize(QSize(70,100));
    ui->widget_5->resize(QSize(70,100));
    ui->widget_6->resize(QSize(70,100));
    ui->widget_7->resize(QSize(70,100));
    ui->widget_8->resize(QSize(70,100));
    ui->widget_9->resize(QSize(70,100));
    //10~19风扇设置大小
    ui->widget_10->resize(QSize(70,100));
    ui->widget_11->resize(QSize(70,100));
    ui->widget_12->resize(QSize(70,100));
    ui->widget_13->resize(QSize(70,100));
    ui->widget_14->resize(QSize(70,100));
    ui->widget_15->resize(QSize(70,100));
    ui->widget_16->resize(QSize(70,100));
    ui->widget_17->resize(QSize(70,100));
    ui->widget_18->resize(QSize(90,135));
    ui->widget_19->resize(QSize(90,135));
    //TCP通信，改变参数时
    this->connect_state=0;//连接状态
    this->m_tcp = new QTcpSocket(this);

    //成功连接
    connect(m_tcp,&QTcpSocket::connected,this,[=](){
        ui->textEdit->append("连接服务器成功");
        this->connect_state=1;
    });
    connect(m_tcp,&QTcpSocket::readyRead,this,[=](){
        QByteArray data = m_tcp->readAll();
        ui->textEdit->append("服务器:"+data);//读取服务器发来的信息
        control(data);
    });
    //断开连接
    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
        ui->textEdit->append("已与服务器断开连接");
        this->connect_state=0;
    });

    //风扇空调控制
    connect(ui->widget_15,&fan_switch::state_change,[=](QString str){
        fan_change("主卧",str);
    });
    connect(ui->widget_17,&fan_switch::state_change,[=](QString str){
        fan_change("书房",str);
    });
    connect(ui->widget_16,&fan_switch::state_change,[=](QString str){
        fan_change("大厅",str);
    });
    connect(ui->widget_10,&fan_switch::state_change,[=](QString ){
        int flag=0;//关
        if(0==flag){
            flag=1;//开
            fan_change("厨房","风开");
        }else if(1==flag){
            flag=0;
            fan_change("厨房","风关");
        }
    });
    connect(ui->widget_12,&fan_switch::state_change,[=](QString ){
        int flag=0;//关
        if(0==flag){
            flag=1;//开
            fan_change("卫生间","风开");
        }else if(1==flag){
            flag=0;
            fan_change("卫生间","风关");
        }
    });

    //灯控制
    connect(ui->widget_2,&light_switch::state_change,[=](QString str){
        light_change("厨房",str);
    });
    connect(ui->widget_4,&light_switch::state_change,[=](QString str){
        light_change("卫生间",str);
    });
    connect(ui->widget_5,&light_switch::state_change,[=](QString str){
        light_change("次卧1",str);
    });
    connect(ui->widget_6,&light_switch::state_change,[=](QString str){
        light_change("次卧2",str);
    });
    connect(ui->widget_7,&light_switch::state_change,[=](QString str){
        light_change("主卧",str);
    });
    connect(ui->widget_8,&light_switch::state_change,[=](QString str){
        light_change("大厅",str);
    });
    connect(ui->widget_9,&light_switch::state_change,[=](QString str){
        light_change("书房",str);
    });
    //温湿度改变
    connect(ui->widget_18,&temperature_humidity::value_change,[=](QString type,double value){
        if(type=="温度"){
            if(connect_state==1){
                QString mes="大厅";
                mes += type;
                mes += QString::number(value);
                m_tcp->write(mes.toUtf8().data());
            }
        }else if(type=="湿度"){
            if(connect_state==1){
                QString mes="大厅";
                mes += type;
                mes += QString::number(value);
                m_tcp->write(mes.toUtf8().data());
            }
        }
    });
    connect(ui->widget_19,&temperature_humidity::value_change,[=](QString type,double value){
        if(type=="温度"){
            if(connect_state==1){
                QString mes="主卧";
                mes += type;
                mes += QString::number(value);
                m_tcp->write(mes.toUtf8().data());
            }
        }else if(type=="湿度"){
            if(connect_state==1){
                QString mes="主卧";
                mes += type;
                mes += QString::number(value);
                m_tcp->write(mes.toUtf8().data());
            }
        }
    });


    //改变其他设备的摄像头在主界面中中转信号，connect这个信号，
    connect(this,&house_control::tochange_video,this,[=](int change){
        if(change==1){
            QString mes="摄像头打开";
            m_tcp->write(mes.toUtf8().data());
        }
        if(change==0){
            QString mes="摄像头关闭";
            m_tcp->write(mes.toUtf8().data());
        }
        if(change==2){
            QString mes="摄像头拍照";
            m_tcp->write(mes.toUtf8().data());
        }
    });
}

house_control::~house_control()
{
    delete ui;
    delete m_tcp;
}

void house_control::fan_change(QString room_name,QString sw_str)
{
    if(connect_state==1){
        QString mes=room_name;
        mes += sw_str;
        m_tcp->write(mes.toUtf8().data());
    }
}

void house_control::light_change(QString room_name, QString sw_str)
{
    if(connect_state==1){
        QString mes=room_name;
        mes += sw_str;
        m_tcp->write(mes.toUtf8().data());
    }
}

//接收改变状态信息并做判断
void house_control::control(QString data)
{
    qDebug()<<data;
    //厨房
    if(data.mid(0,2)=="厨房"){
        if(data.mid(2,1)=="灯"){
            if(data.mid(3,1)=="开"){
                //厨房灯开
                ui->widget_2->change_state(1);
            }else if(data.mid(3,1)=="关"){
                //厨房灯关
                ui->widget_2->change_state(0);
            }
        }else if(data.mid(2,1)=="风"){
            if(data.mid(3,1)=="开"){
                //厨房风开
                ui->widget_10->change_state(1);
            }else if(data.mid(3,1)=="关"){
                //厨房风关
                ui->widget_10->change_state(0);
            }
        }
    }
    //书房
    if(data.mid(0,2)=="书房"){
        if(data.mid(2,1)=="灯"){
            if(data.mid(3,1)=="开"){
                //书房灯开
                ui->widget_9->change_state(1);
            }else if(data.mid(3,1)=="关"){
                //书房灯关
                ui->widget_9->change_state(0);
            }
        }else if(data.mid(2,2)=="空调"){
            if(data.mid(4,1)=="开"){
                //书房风开
                ui->widget_17->change_state(1);
            }else if(data.mid(4,1)=="关"){
                //书房风关
                ui->widget_17->change_state(0);
            }
        }
    }
    //大厅
    if(data.mid(0,2)=="大厅"){
        if(data.mid(2,1)=="灯"){
            if(data.mid(3,1)=="开"){
                ui->widget_8->change_state(1);
            }else if(data.mid(3,1)=="关"){
                ui->widget_8->change_state(0);
            }
        }else if(data.mid(2,2)=="空调"){
            if(data.mid(4,1)=="开"){
                ui->widget_16->change_state(1);
            }else if(data.mid(4,1)=="关"){
                ui->widget_16->change_state(0);
            }
        }
    }
    //主卧
    if(data.mid(0,2)=="主卧"){
        if(data.mid(2,1)=="灯"){
            if(data.mid(3,1)=="开"){
                ui->widget_7->change_state(1);
            }else if(data.mid(3,1)=="关"){
                ui->widget_7->change_state(0);
            }
        }else if(data.mid(2,2)=="空调"){
            if(data.mid(4,1)=="开"){
                ui->widget_15->change_state(1);
            }else if(data.mid(4,1)=="关"){
                ui->widget_15->change_state(0);
            }
        }
    }
    //卫生间
    if(data.mid(0,3)=="卫生间"){
        if(data.mid(3,1)=="灯"){
            if(data.mid(4,1)=="开"){
                ui->widget_4->change_state(1);
            }else if(data.mid(4,1)=="关"){
                ui->widget_4->change_state(0);
            }
        }else if(data.mid(3,1)=="风"){
            if(data.mid(4,1)=="开"){
                ui->widget_12->change_state(1);
            }else if(data.mid(4,1)=="关"){
                ui->widget_12->change_state(0);
            }
        }
    }
    //次卧1
    if(data.mid(0,3)=="次卧1"){
        if(data.mid(3,1)=="灯"){
            if(data.mid(4,1)=="开"){
                ui->widget_5->change_state(1);
            }else if(data.mid(4,1)=="关"){
                ui->widget_5->change_state(0);
            }
        }
    }
    //次卧2
    if(data.mid(0,3)=="次卧2"){
        if(data.mid(3,1)=="灯"){
            if(data.mid(4,1)=="开"){
                ui->widget_6->change_state(1);
            }else if(data.mid(4,1)=="关"){
                ui->widget_6->change_state(0);
            }
        }
    }
    //大厅温湿度
    if(data.mid(0,2)=="大厅"){
        if(data.mid(2,2)=="温度"){
            ui->widget_18->change_value_temperature(data.right(5));
        }else if(data.mid(2,2)=="湿度"){
            ui->widget_18->change_value_humidity(data.right(5));
        }
    }
    //主卧
    if(data.mid(0,2)=="主卧"){
        if(data.mid(2,2)=="温度"){
            ui->widget_19->change_value_temperature(data.right(5));
        }else if(data.mid(2,2)=="湿度"){
            ui->widget_19->change_value_humidity(data.right(5));
        }
    }

    //摄像头
    if(data.mid(0,3)=="摄像头"){
        if(data.mid(3,2)=="打开"){
            emit change_video(1);//给主界面cpp发送摄像头改变信号，摄像头对象创建在给cpp中。
        }
        if(data.mid(3,2)=="关闭"){
            emit change_video(0);//给主界面cpp发送摄像头改变信号，摄像头对象创建在给cpp中。
        }
        if(data.mid(3,2)=="拍照"){
            emit change_video(2);//给主界面cpp发送摄像头改变信号，摄像头对象创建在给cpp中。
        }
    }
}

QString house_control::get_port()
{
    return ui->lineEdit_port->text();
}

QString house_control::get_ip()
{
    return ui->lineEdit_ip->text();
}

void house_control::stop()
{
    m_tcp->close();
}

void house_control::on_pushButton_server_connect_clicked()
{
    QString port_str=ui->lineEdit_port->text();
    quint16 port = port_str.toUShort();
    QString adress=ui->lineEdit_ip->text();
    m_tcp->connectToHost(QHostAddress(adress),port);//请求连接服务器
}

void house_control::on_pushButton_disconnect_clicked()
{
    m_tcp->close();
}
