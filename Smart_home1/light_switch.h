#ifndef LIGHT_SWITCH_H
#define LIGHT_SWITCH_H

#include <QWidget>

namespace Ui {
class light_switch;
}

class light_switch : public QWidget
{
    Q_OBJECT

public:
    explicit light_switch(QWidget *parent = nullptr);
    ~light_switch();

    void change_state(int);
signals:
    void state_change(QString );
public:
    int flag;
private:
    Ui::light_switch *ui;
};

#endif // LIGHT_SWITCH_H
