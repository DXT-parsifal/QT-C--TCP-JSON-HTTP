#ifndef TEMPERATURE_HUMIDITY_H
#define TEMPERATURE_HUMIDITY_H

#include <QWidget>

namespace Ui {
class temperature_humidity;
}

class temperature_humidity : public QWidget
{
    Q_OBJECT

public:
    explicit temperature_humidity(QWidget *parent = nullptr);
    ~temperature_humidity();

    void change_value_temperature(QString);
    void change_value_humidity(QString);
signals:
    void value_change(QString,double);

private:
    Ui::temperature_humidity *ui;
};

#endif // TEMPERATURE_HUMIDITY_H
