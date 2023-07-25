#ifndef HOUSE_CONTROL_H
#define HOUSE_CONTROL_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
namespace Ui {
class house_control;
}

class house_control : public QWidget
{
    Q_OBJECT

public:
    explicit house_control(QWidget *parent = nullptr);
    ~house_control();

    void fan_change(QString,QString);
    void light_change(QString,QString);

    void control(QString);

    QString get_port();
    QString get_ip();
    void stop();
public:
    QTcpSocket *m_tcp;
    int connect_state;
signals:
    void change_video(int);
    void tochange_video(int);
private slots:
    void on_pushButton_server_connect_clicked();

    void on_pushButton_disconnect_clicked();

private:
    Ui::house_control *ui;
};

#endif // HOUSE_CONTROL_H
