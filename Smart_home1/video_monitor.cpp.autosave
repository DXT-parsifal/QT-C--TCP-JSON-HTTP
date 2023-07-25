#include "video_monitor.h"
#include "ui_video_monitor.h"
#include "house_control.h"

Video_monitor::Video_monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Video_monitor)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:rgb(172, 172, 172);");
    ui->label_3->setScaledContents(true);
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();//获取可用摄像头设备列表
    m_Camera = new QCamera(cameras[0]);                             //创建摄像头对象
    m_Camera->setCaptureMode(QCamera::CaptureStillImage);               //设置捕捉模式为静态帧
    m_Camera->setCaptureMode(QCamera::CaptureMode::CaptureViewfinder);  //将采集到取景器中
    m_Camera->setViewfinder(ui->widget);                           //设置数据显示到QVideoWidget控件中
    ui->pushButton_save->setEnabled(false);
    ui->pushButton_image->setEnabled(false);

    this->flag=0;//0 摄像头关闭 1 摄像头开启

    ui->pushButton_3->setEnabled(false);
}

Video_monitor::~Video_monitor()
{
    delete ui;
    delete m_Camera;
    delete m_imageCapture;
}

void Video_monitor::on_pushButton_open_clicked()
{
    if(flag==0){
        m_Camera->start();  //启动摄像头
        flag=1;
        ui->pushButton_image->setEnabled(true);
        m_imageCapture = new QCameraImageCapture(m_Camera); /**实例化媒体记录类对象**/
        m_imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile); //设置拍照将图像捕获到文件中
        /*********** 捕获信号函数 imageCaptured(int,QImage) 拍了一张照,发出该信号 ********/
        connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));
    }
}

void Video_monitor::on_pushButton_close_clicked()
{
    if(flag==1){
        m_Camera->stop();//停止摄像头
        flag=0;
    }
}

void Video_monitor::on_pushButton_image_clicked()
{
    /*********** 按下拍照按钮,获取摄像头帧图片 ********/
    m_Camera->searchAndLock();      //上锁
    m_imageCapture->capture();      //拍照
    m_Camera->unlock();             //解锁
}

void Video_monitor::change_state(int change)
{
    if(change==1){
        m_Camera->start();  //启动摄像头
        flag=1;
        ui->pushButton_image->setEnabled(true);
        m_imageCapture = new QCameraImageCapture(m_Camera); /**实例化媒体记录类对象**/
        m_imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile); //设置拍照将图像捕获到文件中
        /*********** 捕获信号函数 imageCaptured(int,QImage) 拍了一张照,发出该信号 ********/
        connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));

    }else if(change==0){
        m_Camera->stop();//停止摄像头
        flag=0;
    }
    if(flag==1){
        if(change==2){
            //拍照
            m_Camera->searchAndLock();      //上锁
            m_imageCapture->capture();      //拍照
            m_Camera->unlock();             //解锁
        }
    }
}

void Video_monitor::displayImage(int, QImage image)/*** 处理好照片的数据 ***/
{
    Pixmap =QPixmap::fromImage(image);
    ui->label_3->setPixmap(Pixmap);
    qDebug() << "接收到拍摄的照片" << endl;
    ui->pushButton_save->setEnabled(true);
}

void Video_monitor::on_pushButton_save_clicked()
{
    QString path=QFileDialog::getSaveFileName(this,"保存位置","D:\\hqyj\\coding\\C++QT\\QT\\Smart_home1\\CaptureImage\\","*.jpg");
    if(!path.isEmpty()){
        Pixmap.save(path);
    }else{
        qDebug()<<"选择保存的文件";
    }
}
//打开远程摄像头
void Video_monitor::on_pushButton_clicked()
{
    ui->pushButton_3->setEnabled(true);
    emit state_change(1);//1 摄像头打开 0 摄像头关闭 2 摄像头拍照
}
//关闭远程摄像头
void Video_monitor::on_pushButton_2_clicked()
{
    ui->pushButton_3->setEnabled(false);
    emit state_change(0);
}
//远程摄像头拍照
void Video_monitor::on_pushButton_3_clicked()
{
    emit state_change(2);
}
