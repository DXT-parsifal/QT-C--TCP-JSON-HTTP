#ifndef MEDIA_PLAYBACK_H
#define MEDIA_PLAYBACK_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QStringList>
#include "online_music.h"
namespace Ui {
class media_playback;
}

class media_playback : public QWidget
{
    Q_OBJECT

public:
    explicit media_playback(QWidget *parent = nullptr);
    ~media_playback();
public:
    QMediaPlayer *player;
    QMediaPlaylist *playerlist;
    QString durationTime;
    QString positionTime;

    //播放器界面
    online_music *on_music;
    int flag;
signals:
    void house_state_change(QString);
private slots:
    void on_pushButton_begin_clicked();

    void getState(QMediaPlayer::State state);


    //播放文件数据总大小的信号， 它可以获得文件时间长度。
    void onPositionChanged(qint64 position); //当前文件播放位置变化，更新进度显示

    //手动滑块移动，player播放位置随着改变

    //在线界面切换
    void on_pushButton_4_clicked();

private:
    Ui::media_playback *ui;
};

#endif // MEDIA_PLAYBACK_H
