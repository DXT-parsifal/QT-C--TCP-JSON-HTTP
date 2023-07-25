#include "online_music.h"
#include "ui_online_music.h"

#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QMovie>

online_music::online_music(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::online_music)
{
    ui->setupUi(this);
    ui->pushButton_video->setText("按下说话");

    ui->label_4->setScaledContents(true);
    ui->label_4->setPixmap(QPixmap(":/res/机器人.png"));

    ui->pushButton_begin->setIcon(QIcon(":/media/res/Player, play.png"));

    this->movie=new QMovie(":/res/online_player.gif");
    ui->label_2->setMovie(movie);
    ui->label_2->setScaledContents(true);

    this->http_online=new QNetworkAccessManager(this);//网络管理对象
    connect(ui->pushButton,&QPushButton::clicked,this,&online_music::slot_btn_request);
    connect(http_online,&QNetworkAccessManager::finished,this,&online_music::slots_http_finshed);

    this->player_online=new QMediaPlayer(this);
    this->playerlist_online=new QMediaPlaylist(this);
    player_online->setPlaylist(playerlist_online);

    connect(ui->pushButton_begin,&QPushButton::clicked,player_online,[=](){
        getState(this->player_online->state());
    });
}

online_music::~online_music()
{
    delete ui;
    delete outFile;//录音时的变量
    delete my_audio;//录音时的变量
}
//音乐ID请求API
void online_music::slot_btn_request()
{
    if(ui->lineEdit->text().isEmpty()){
        return;
    }else{
        QString lineEdit_text=ui->lineEdit->text();
        QString url="http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
        url += lineEdit_text;
        url += "&type=1&offset=0&total=true&limit=2";
        //发送请求
        http_online->get(QNetworkRequest(QUrl(url)));
        playerlist_online->clear();
        movie->stop();//点击播放动图开始动,播放键改变图片
        ui->pushButton_begin->setIcon(QIcon(":/media/res/Player, play.png"));
        player_online->stop();
    }
}

void online_music::slots_http_finshed(QNetworkReply *reply)
{
    if(reply->error()==QNetworkReply::TimeoutError){
        QMessageBox::warning(this,"http:404","请求超时");
        return;
    }
    int stat=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    //读取全部接受到的信息
    QByteArray data=reply->readAll();
    if(stat!=200){//成功获取
        QMessageBox::warning(this,"http警告","请求失败，错误数据:"+data);
        return;
    }

    QJsonDocument document=QJsonDocument::fromJson(data);
    QJsonObject jsonobject=document.object();
    QJsonObject object_result =jsonobject.value("result").toObject();
    QJsonArray array_songs =object_result.value("songs").toArray();
    QJsonObject object_song=array_songs.at(0).toObject();

    //音乐name
    QString music_name=object_song["name"].toString();
    ui->textEdit->append("搜索结果:"+music_name);
    //音乐ID
    QString music_id="http://music.163.com/song/media/outer/url?id=";
    music_id += QString("%1").arg(object_song["id"].toInt());
    QString music_url=music_id;
    music_url += ".mp3";
    qDebug()<<music_url;
    playerlist_online->addMedia(QMediaContent(QUrl(music_url)));
}

void online_music::on_pushButton_begin_clicked()
{
    if(this->player_online->state()==QMediaPlayer::PlayingState){
        this->player_online->pause();
        movie->stop();//点击播放动图开始动,播放键改变图片
    }else{
        this->player_online->play();
        movie->start();//点击播放动图开始动,播放键改变图片
    }
}

void online_music::getState(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::PlayingState){
        ui->pushButton_begin->setIcon(QIcon(":/media/res/Player, pause.png"));
    }else{
        ui->pushButton_begin->setIcon(QIcon(":/media/res/Player, play.png"));
    }
}


//录音开始
void online_music::RecorderStart(QString fileName)
{
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    if(device.isNull())
    {
        QMessageBox::warning(NULL,"QAudioDeviceInfo","录音设备不存在");
        return;
    }
//    设置通道数
    audioFormat.setChannelCount(1);
//    设置编码
    audioFormat.setCodec("audio/pcm");
//    设置采样频率
    audioFormat.setSampleRate(16000);
//    设置位深
    audioFormat.setSampleSize(16);
//    判断设备是否支持该格式
    if(!device.isFormatSupported(audioFormat)){ //当前使用设备是否支持
        audioFormat = device.nearestFormat(audioFormat); //转换为最接近格式
    }
//    创建录音对象
    my_audio = new QAudioInput(audioFormat,this);
    outFile = new QFile(this);
    outFile->setFileName(fileName); //语音原始文件
    outFile->open(QIODevice::WriteOnly);
//     开始录音
    my_audio->start(outFile);
}

/**********************
 * 结束录音并转换格式
**********************/
void online_music::RecorderEnd()
{
//    结束录音
    my_audio->stop();

    outFile->close();
    delete outFile;
    outFile =NULL;

    delete my_audio;
    my_audio = NULL;
}
//按钮按下开始识别
void online_music::on_pushButton_video_pressed()
{
    RecorderStart("./1.pcm");
    ui->pushButton_video->setText("松开识别");
    ui->label_4->setPixmap(QPixmap(":/res/机器人_speak.png"));
}
//按钮松开开始语音识别，并返回文字搜索音乐
void online_music::on_pushButton_video_released()
{
    ui->pushButton_video->setText("按下说话");
    ui->label_4->setPixmap(QPixmap(":/res/机器人.png"));
    RecorderEnd();
    QString text =speechIdentify("./1.pcm");
    ui->textEdit->append("语音识别:"+text);
    audioCtrl(text);
}
//识别结果处理，填入lineEdit，触发搜索按键
void online_music::audioCtrl(QString text)
{

    QString strTmp = text.left(text.size()-1);
    if(strTmp=="厨房灯开"||strTmp=="厨房灯关"||strTmp=="厨房风开"||strTmp=="厨房风关"||
            strTmp=="卫生间灯开"||strTmp=="卫生间灯关"||strTmp=="卫生间风开"||strTmp=="卫生间风关"||
            strTmp=="次卧1灯开"||strTmp=="次卧1灯关"||
            strTmp=="次卧2灯开"||strTmp=="次卧2灯关"||
            strTmp=="大厅灯开"||strTmp=="大厅灯关"||strTmp=="大厅空调开"||strTmp=="大厅空调关"||strTmp.mid(0,4)=="大厅湿度"||strTmp.mid(0,4)=="大厅温度"||
            strTmp=="书房灯开"||strTmp=="书房灯关"||strTmp=="书房空调开"||strTmp=="书房空调关"||
            strTmp=="主卧灯开"||strTmp=="主卧灯关"||strTmp=="主卧空调开"||strTmp=="主卧空调关"||strTmp.mid(0,4)=="主卧温度"||strTmp.mid(0,4)=="主卧湿度"
            ){
        house_state_change(strTmp);//触发全屋控制的信号
    }else{
        ui->lineEdit->setText(strTmp);
        emit ui->pushButton->clicked();
    }
}

//求url组装
bool online_music::post_sync(QString url,QMap<QString,QString>header,QByteArray requestData,QByteArray &replyData)
{
//    发送请求的对象
    QNetworkAccessManager manager;
//    请求 对象
    QNetworkRequest request;
    request.setUrl(url);
    QMapIterator<QString,QString> it(header);
    while (it.hasNext()) {
        it.next();
        request.setRawHeader(it.key().toLatin1() ,it.value().toLatin1());
    }
//设置openssl签名配置,否则在ARM上会报错
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    conf.setProtocol(QSsl::TlsV1_0);
#else
    conf.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(conf);

    QNetworkReply *reply = manager.post(request,requestData);
    QEventLoop l;
    //一旦服务器返回，reply会发出信号
    connect(reply,&QNetworkReply::finished,&l,&QEventLoop::quit);
    l.exec();
    if(reply != nullptr && reply->error() == QNetworkReply::NoError)
    {

        replyData = reply->readAll();
        return true;
    }
    else
    {
        qDebug()<<"request error!";
        return false;
    }
}
//发送请求，百度识别返回结果，送给getJsonValue处理
QString online_music::speechIdentify(QString fileName)
{
//    获取Access Token
    QString tokenUrl =QString(baiduTokenUrl).arg(client_id).arg(client_secret);
    QMap<QString,QString>header;
    header.insert(QString("Content-Type"),QString("audio/pcm;rate=16000"));
    QByteArray requestData;//请求内容
    QByteArray replyData;//url返回内容

   qDebug()<<tokenUrl;
    bool result = post_sync(tokenUrl,header,requestData,replyData);
    if(result) {
        QString key = "access_token";
        QString accessToken =getJsonValue(replyData,key);
        qDebug()<<accessToken;
        //    语音识别
        QString speechUrl = QString(baiduSpeechUrl).arg(QHostInfo::localHostName()).arg(accessToken);
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        requestData = file.readAll();
        file.close();
        replyData.clear();
//        再次发起请求
        result = post_sync(speechUrl,header,requestData,replyData);
         if(result) {
             QString key = "result";
             QString retText =getJsonValue(replyData,key);
             qDebug()<<retText;
             return retText;
         }
         else{
             return NULL;
         }
    }
    else {
        return  "error";
    }
}
//解析返回的语音JOSN格式，得到语音识别的结果
QString online_music::getJsonValue(QByteArray ba,QString key)
{
    QJsonParseError parseError;
    QJsonDocument jsondocument = QJsonDocument::fromJson(ba,&parseError);
    if(parseError.error ==QJsonParseError::NoError)
    {
        if(jsondocument.isObject())
        {
            QJsonObject jsonObject = jsondocument.object();
            if(jsonObject.contains(key)){
                QJsonValue jsonvalue = jsonObject.value(key);
                if(jsonvalue.isString())
                    return jsonvalue.toString();
                else if(jsonvalue.isArray()){
                    QJsonArray arr = jsonvalue.toArray();
                    QJsonValue val =arr.at(0);
                    return val.toString();
                }
            }
        }
    }
    return "";
}


