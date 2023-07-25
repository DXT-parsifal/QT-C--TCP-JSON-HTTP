#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->flag=0;//0 返回false 1 返回true
    ui->btn_home->setEnabled(false);

    this->setWindowTitle("Smart Home");
    this->setWindowIcon(QIcon(":/res/智能家居3.png"));
    this->resize(1000,700);
    //stackeWidget背景设置,切换界面后改变

    qDebug()<<"stackedWidget当前界面index为"<<ui->stackedWidget->currentIndex();

    //全屋控制界面
    this->tabWidget_control=new house_control(this);
    ui->stackedWidget->addWidget(tabWidget_control);
    //媒体播放界面
    this->media_play=new media_playback(this);
    ui->stackedWidget->addWidget(media_play);
    //视频监控界面
    this->video_monitor=new Video_monitor(this);
    ui->stackedWidget->addWidget(video_monitor);

    //home键逻辑
    connect(ui->btn_home,&QPushButton::clicked,this,[=](){
        if(flag==0){
            flag=1;
            ui->btn_home->setEnabled(true);
            ui->stackedWidget->setCurrentIndex(0);
        }else if(flag==1){
            flag=0;
            ui->btn_home->setEnabled(false);
            ui->stackedWidget->setCurrentIndex(0);
        }
    });
    qDebug()<<"stackedWidget的窗口数"<<ui->stackedWidget->count();
    qDebug()<<"stackedWidget的当前窗口数"<<ui->stackedWidget->currentIndex();

    //API天气JSON解析，
    //城市map
    m_city["长沙"]="101250101";
    m_city["北京"]="101010100";
    m_city["上海"]="101020100";
    m_city["深圳"]="101280601";
    m_city["广州"]="101280101";
    this->http=new QNetworkAccessManager(this);//网络管理对象
    //replay由get请求返回
    //初始请求、得到回应、主界面显示
    slot_btn_request();
    if(this->replay->isFinished()){
        slots_http_finshed(replay);
    }

    //发送请求
    connect(ui->pushButton_weather,&QPushButton::clicked,this,&MainWindow::slot_btn_request);
    //收到回应后读取JSON数据
    connect(http,&QNetworkAccessManager::finished,this,&MainWindow::slots_http_finshed);

    connect(tabWidget_control,&house_control::change_video,video_monitor,&Video_monitor::change_state);//接受到要改变状态
    connect(video_monitor,&Video_monitor::state_change,tabWidget_control,&house_control::tochange_video);//要改变其他设备的状态
    connect(media_play,&media_playback::house_state_change,tabWidget_control,&house_control::control);//全屋控制信号转接
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tabWidget_control;
    delete media_play;
    delete video_monitor;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    qDebug()<<"stackedWidget的当前窗口数"<<ui->stackedWidget->currentIndex();
    ui->btn_home->setEnabled(true);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    qDebug()<<"stackedWidget的当前窗口数"<<ui->stackedWidget->currentIndex();
    ui->btn_home->setEnabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    qDebug()<<"stackedWidget的当前窗口数"<<ui->stackedWidget->currentIndex();
    ui->btn_home->setEnabled(true);
}

//天气请求URL get
void MainWindow::slot_btn_request()
{
    //获取comboBox栏的信息
    QString city=ui->comboBox->currentText();
    //组装url信息
    QString url= "http://aider.meizu.com/app/weather/listWeather?cityIds=";
    url += m_city[city];
    //请求连接方式：get/post
    this->replay=this->http->get(QNetworkRequest(QUrl(url)));//请求链接
}
//请求回复读取JSON
void MainWindow::slots_http_finshed(QNetworkReply *reply)
{
    int stat=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    //读取全部接受到的信息
    QByteArray data=reply->readAll();
    if(stat!=200){//成功获取
        QMessageBox::warning(this,"http警告","请求失败，错误数据:"+data);
        return;
    }

    //解析成JSON格式文本
    QJsonDocument document=QJsonDocument::fromJson(data);
    QJsonObject jsonObject=document.object();
    QJsonArray json_value=jsonObject.value("value").toArray();

    QJsonArray json_alarms=json_value.at(0).toObject().value("alarms").toArray();
    QJsonObject object_alarms=json_alarms.at(0).toObject();
    qDebug()<<object_alarms["alarmContent"];

    QJsonArray json_weather = json_value.at(0).toObject().value("weathers").toArray();
    QJsonObject object_weather =json_weather.at(0).toObject();
    qDebug()<<object_weather["weather"].toString();
    QString mes = "城市:" ;
    mes += ui->comboBox->currentText();
    mes += "\n日期:";
    mes += object_weather["date"].toString();
    mes += "\n星期:";
    mes += object_weather["week"].toString();
    mes += "\n白天温度:";
    mes += object_weather["temp_day_c"].toString();
    mes += "摄氏度\n晚上温度:";
    mes += object_weather["temp_night_c"].toString();
    mes += "摄氏度\n天气:";
    mes += object_weather["weather"].toString();

    mes +="\n\n天气预警:";
    mes += object_alarms["alarmContent"].toString();

    //将读取的信息直接set到textEdit中
    ui->textEdit->setText(mes);

}
