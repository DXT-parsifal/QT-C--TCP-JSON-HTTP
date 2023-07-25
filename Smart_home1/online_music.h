#ifndef ONLINE_MUSIC_H
#define ONLINE_MUSIC_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>
#include <QAudioInput>

namespace Ui {
class online_music;
}

class online_music : public QWidget
{
    Q_OBJECT

public:
    explicit online_music(QWidget *parent = nullptr);
    ~online_music();
public:
    QMovie *movie;
    //网络管理对象
    QNetworkAccessManager *http_online;

    QMediaPlayer *player_online;
    QMediaPlaylist *playerlist_online;

    //语音识别
    void RecorderStart(QString fileName);//开始录音
    void RecorderEnd();//结束录音并转换格式
    QFile *outFile;//录音时的变量
    QAudioInput *my_audio;//录音时的变量
    QAudioFormat audioFormat;//录音时的变量

    bool post_sync(QString url,QMap<QString,QString>header,QByteArray requestData,QByteArray &replyData);

    QString speechIdentify(QString fileName);
    QString getJsonValue(QByteArray ba,QString key);

    void audioCtrl(QString);//语音识别转文字处理
signals:
    void house_state_change(QString);//触发全屋控制的信号
public slots:
    void slot_btn_request();//在线歌曲请求URL get
    void slots_http_finshed(QNetworkReply *reply);//请求回复读取JSON

private slots:
    void on_pushButton_begin_clicked();//播放暂停二和一

    void getState(QMediaPlayer::State state);//播放键改变更改图片
    void on_pushButton_video_pressed();

    void on_pushButton_video_released();

private:
    Ui::online_music *ui;
};

//    获取Access Token
const QString baiduTokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";
const QString client_id = "X0m0YWonD0KX8CIpuHKX14Fv";
const QString client_secret = "gFQP3f8SGKbEdxt38cbKmtp7AIGxx9hS";
//    语音识别url
const QString baiduSpeechUrl = "https://vop.baidu.com/server_api?dev_pid=1537&cuid=%1&token=%2";


#endif // ONLINE_MUSIC_H
