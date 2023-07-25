#ifndef VIDEO_MONITOR_H
#define VIDEO_MONITOR_H

#include <QWidget>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QPixmap>
#include <QFileDialog>

namespace Ui {
class Video_monitor;
}

class Video_monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Video_monitor(QWidget *parent = nullptr);
    ~Video_monitor();

    void change_state(int);
public:
    QCamera *m_Camera;
    QCameraImageCapture *m_imageCapture;
    QPixmap Pixmap;

    int flag;

signals:
    void state_change(int);
private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_image_clicked();

    void displayImage(int,QImage);

    void on_pushButton_save_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Video_monitor *ui;
};

#endif // VIDEO_MONITOR_H
