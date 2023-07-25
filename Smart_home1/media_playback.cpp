#include "media_playback.h"
#include "ui_media_playback.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSlider>

media_playback::media_playback(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::media_playback)
{
    ui->setupUi(this);
    //设置背景图片
//    this->setStyleSheet("background-image:url(:/res/media_player.jpg)");
    ui->widget->setStyleSheet("background-color:rgb(46, 149, 111);");

    //初始化对象
    this->player=new QMediaPlayer(this);
    this->playerlist=new QMediaPlaylist(this);

    //设置视频播放输出
    this->player->setVideoOutput(ui->widget_media);
    //播放列表加入歌曲
    playerlist->addMedia(QMediaContent(QUrl("qrc:/media/res/media/画.mp3")));
    playerlist->addMedia(QMediaContent(QUrl("qrc:/media/res/media/不要说话.m4a")));
    playerlist->addMedia(QMediaContent(QUrl("qrc:/media/res/media/桃花落.m4a")));

    //播放器设置播放列表
    player->setPlaylist(playerlist);

    //list列表添加选项
    ui->listWidget->addItem("画.mp3");
    ui->listWidget->addItem("不要说话.m4a");
    ui->listWidget->addItem("桃花落.m4a");

    ui->listWidget->setCurrentRow(playerlist->currentIndex());
    //信号和槽机制实现：开始，下一首，上一首，添加歌曲，删除歌曲
    connect(ui->pushButton_begin,&QPushButton::clicked,player,[=](){
        getState(this->player->state());
    });
    ui->pushButton_begin->setIcon(QIcon(":/media/res/Player, play.png"));
    connect(ui->pushButton_next,&QPushButton::clicked,playerlist,[=](){
        qDebug()<<"当前playerlist播放位置"<<playerlist->currentIndex();
        ui->horizontalSlider->setSliderPosition(0);
        playerlist->next();

    });
    connect(ui->pushButton_prev,&QPushButton::clicked,playerlist,[=](){
        qDebug()<<"当前playerlist播放位置"<<playerlist->currentIndex();
        ui->horizontalSlider->setSliderPosition(0);
        playerlist->previous();
    });
    //播放文件添加
    connect(ui->pushButton_add,&QPushButton::clicked,this,[=](){
        QList<QUrl> urls=QFileDialog::getOpenFileUrls(this,"请选择添加的文件",QUrl("D:\\hqyj\\coding\\C++QT\\QT\\Smart_home1\\res\\media\\"),"*.mp3 *.m4a *.mp4 *.acc");
        for(int i=0;i<urls.size();i++){
            playerlist->addMedia(urls[i]);
            ui->listWidget->addItem(urls[i].fileName());
        }
    });
    //设置播放模式
    void (QComboBox:: *currentindexChanged)(int)=&QComboBox::currentIndexChanged;
    connect(ui->comboBox,currentindexChanged,this,[=](int index){
        switch (index) {
        case 0:
            this->playerlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
            break;
        case 1:
            this->playerlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
            break;
        case 2:
            this->playerlist->setPlaybackMode(QMediaPlaylist::Sequential);
            break;
        case 3:
            this->playerlist->setPlaybackMode(QMediaPlaylist::Loop);
            break;
        case 4:
            this->playerlist->setPlaybackMode(QMediaPlaylist::Random);
            break;
        }
    });

    //音量改变
    connect(ui->verticalSlider,&QSlider::valueChanged,player,&QMediaPlayer::setVolume);

    //listWidget行改变，音乐播放改变，需要判断listWidget的值为0,防止跑飞
    connect(ui->listWidget,&QListWidget::currentRowChanged,playerlist,&QMediaPlaylist::setCurrentIndex);

    //音乐改变，listwidget列表行改变
    void (QListWidget::*setCurrentRow)(int) = &QListWidget::setCurrentRow;
    connect(playerlist,&QMediaPlaylist::currentIndexChanged,ui->listWidget,setCurrentRow);

    connect(ui->pushButton_delete,&QPushButton::clicked,this,[=](){
        int row = ui->listWidget->currentRow();
        //移出播放列表里面的音乐
        if(playerlist->removeMedia(row))
        {
            QMessageBox::information(this,"删除","删除成功");
            ui->listWidget->takeItem(row);
        }
    });    

    //构造函数中添加
    //获取mp3文件时长,durationChanged触发，m4a才能触发这个信号，本地mp3的音乐文件触发不了这个信号(滑块初值被置为0,滑块播放时不移动)，
    connect(player,&QMediaPlayer::durationChanged,this,[=](qint64 duration){
       qDebug()<<"durationChanged";
       ui->horizontalSlider->setMaximum(duration); //设置进度条最大值 也就是歌曲时长 ms
       int secs = duration/1000; //全部秒数
       int mins = secs/60;//分
       secs = secs % 60;//秒
       durationTime = QString::number(mins) + ":" + QString::number(secs);
       ui->label_2->setText(positionTime+"/"+durationTime);
    });

    //获取音频当前播放位置

    connect(player,&QMediaPlayer::positionChanged,this,&media_playback::onPositionChanged);

    //slider值改变，player改变
    //Slider鼠标按下timer.stop(),Slier鼠标移动更新player.position(),Slider的位置，Slider鼠标释放timer.start()。

    connect(ui->horizontalSlider,&QSlider::sliderPressed,this,[=](){
        if(player->state()==QMediaPlayer::PlayingState){
            player->stop();
        }
    });

    connect(ui->horizontalSlider,&QSlider::sliderReleased,this,[=](){
        if(player->duration()!=0){
            player->setPosition(ui->horizontalSlider->value());
        }
        player->play();
    });


    //stackeWidget切换界面
    this->on_music=new online_music(this);
    ui->stackedWidget->addWidget(on_music);
    qDebug()<<"online_music:stackedWidget当前界面index为"<<ui->stackedWidget->currentIndex();
    this->flag=0;//0 本地音乐界面 1 在线音乐界面
    connect(on_music,&online_music::house_state_change,this,&media_playback::house_state_change);//在线语音中转信号
}

media_playback::~media_playback()
{
    delete ui;
}

//播放和暂停切换
void media_playback::on_pushButton_begin_clicked()
{
    if(this->player->state()==QMediaPlayer::PlayingState){
        this->player->pause();
    }else{
        this->player->play();
    }
}
void media_playback::getState(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::PlayingState){
        ui->pushButton_begin->setIcon(QIcon(":/media/res/Player, pause.png"));
    }else{
        ui->pushButton_begin->setIcon(QIcon(":/media/res/Player, play.png"));
    }
}

void media_playback::onPositionChanged(qint64 position)
{
    //格式不对做判断
    if(player->duration()!=0){
        ui->horizontalSlider->setSliderPosition(player->position()*ui->horizontalSlider->maximum()/player->duration());
    }else{
        ui->horizontalSlider->setSliderPosition(position);
    }
    int secs = position/1000;
    int mins = secs/60;
    secs = secs % 60;
    positionTime = QString::number(mins) + ":" + QString::number(secs);
    ui->label_2->setText(positionTime+"/"+durationTime);
}

//滑块移动，player值改变


//在线音乐界面切换,在线音乐pushbutton
void media_playback::on_pushButton_4_clicked()
{
    if(0==flag){
        ui->stackedWidget->setCurrentIndex(1);//进到在线音乐界面
        qDebug()<<"online_music:stackedWidget的当前窗口数"<<ui->stackedWidget->currentIndex();
        ui->pushButton_4->setText("本地音乐");
        flag=1;
    }else if(flag==1){
        ui->stackedWidget->setCurrentIndex(0);//返回本地音乐界面
        qDebug()<<"online_music:stackedWidget的当前窗口数"<<ui->stackedWidget->currentIndex();
        ui->pushButton_4->setText("在线音乐");
        flag=0;
    }
}




