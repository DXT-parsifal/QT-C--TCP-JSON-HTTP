#ifndef FAN_SWITCH_H
#define FAN_SWITCH_H

#include <QWidget>

namespace Ui {
class fan_switch;
}

class fan_switch : public QWidget
{
    Q_OBJECT

public:
    explicit fan_switch(QWidget *parent = nullptr);
    ~fan_switch();

    void change_state(int);//被动改变
signals:
    void state_change(QString );//主动改变
public:
    int flag;//按钮开关标志

private:
    Ui::fan_switch *ui;
};

#endif // FAN_SWITCH_H
