#include "temperature_humidity.h"
#include "ui_temperature_humidity.h"
#include <QDebug>
temperature_humidity::temperature_humidity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::temperature_humidity)
{
    ui->setupUi(this);
    ui->label_3->setPixmap(QPixmap(":/res/温度湿度气压.png"));
    ui->label_3->setScaledContents(true);
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,[=](){
        emit value_change("温度",ui->lineEdit->text().toDouble());
    });
    connect(ui->lineEdit_2,&QLineEdit::returnPressed,this,[=](){
        emit value_change("湿度",ui->lineEdit_2->text().toDouble());
    });
}

temperature_humidity::~temperature_humidity()
{
    delete ui;
}

void temperature_humidity::change_value_temperature(QString temperature)
{
    ui->lineEdit->setText(temperature);
}

void temperature_humidity::change_value_humidity(QString humidity)
{
    ui->lineEdit_2->setText(humidity);
}
