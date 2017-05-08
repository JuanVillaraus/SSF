#ifndef CONSF_H
#define CONSF_H
#include <QThread>
#include <QString>
#include <QDebug>
#include <QSerialPort>


class conexionSF : public QThread
{

private:
    QString modo;
    int estado;
    QSerialPort serialPortUSB;

public:
    conexionSF();
    ~conexionSF();
    void set_modo(QString modo);
    void set_estado(int estado);
    QString get_modo();
    int get_estado();
    void open_port();
    void set_serialPort(QSerialPort serialPortUSB);

public slots:
    void run();


signals:
    void valueChanged(int estado);


};

#endif // CONSF_H
