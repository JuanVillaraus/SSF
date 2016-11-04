/*#ifndef SIVISO_H
#define SIVISO_H

#include <QMainWindow>

namespace Ui {
class siviso;
}

class siviso : public QMainWindow
{
    Q_OBJECT

public:
    explicit siviso(QWidget *parent = 0);
    ~siviso();


*/
#ifndef SIVISO_H
#define SIVISO_H

#include <blanco.h>
#include <ppi.h>
#include <signal.h>
#include <QDir>
#include <QHostAddress>
#include <QUdpSocket>
#include <QSerialPort>
#include <QProcess>

//---------headers for the use of postgres
#include<QtSql/QSql>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlRecord>
#include<QtSql/QSqlField>
#include<QStandardItemModel>
#include"dbasepostgresql.h"
//---------finish declarations of the headers

#include <QMainWindow>
#include "wconfig.h"


#define DAY_STYLE 0
#define DUSK_STYLE 1
#define NIGHT_STYLE 2

namespace Ui {
class siviso;
}

class siviso : public QMainWindow
{
    Q_OBJECT

public:
    explicit siviso(QWidget *parent = 0);
    ~siviso();

    QApplication *m_pApplication;

    void changeStyleSheet(int iStyle);

    int numero;
    int numCatchSend;
    int colorUp;
    int colorDw;
    QString catchSend;
    QString compGraf;
    PPI *myppi;
    Signal *mysignal;
    bool bToolButton;
    bool bRec;
    bool bPlay;

private slots:
    //void on_toolButton_clicked();
    void leerSocket();
    void leerSerialDB9();
    void leerSerialUSB();

private slots:

    void on_btOpenPort_clicked();

    void on_tipo_norte_clicked();

    void on_lf_clicked();

    void on_btr_clicked();

    void on_rng_clicked();

    void on_origen_buque_clicked();

    void on_origen_target_clicked();

    void on_bw_valueChanged(double arg1);

    void on_edo_mar_valueChanged(int arg1);

    void on_prob_falsa_valueChanged(double arg1);

    void on_prob_deteccion_valueChanged(double arg1);

    void on_escala_ppi_valueChanged(double arg1);

    void on_escala_despliegue_tactico_valueChanged(double arg1);

    void on_gan_sen_valueChanged(int arg1);

    void on_frecuencia_valueChanged(int value);

    //void on_pushButton_info_clicked();

    //void on_pushButton_send_clicked();

    void on_it_valueChanged(int arg1);

    void on_rec_clicked();

    void on_play_clicked();

    void on_toolButton_clicked();

    void on_save_clicked();

    void on_setColorUp_valueChanged(int value);

    void on_setColorDw_valueChanged(int value);

private:
    Ui::siviso *ui;

    QUdpSocket *udpsocket;
    QHostAddress direccionSPP;
    QHostAddress direccionApp;
    quint16 puertoSPP;
    quint16 puertoPPI;
    quint16 puertoBTR;
    quint16 puertoLF;
    quint16 puertoREC;
    QSerialPort *serialPortDB9;
    QSerialPort *serialPortUSB;
    QProcess *proceso1;
    QProcess *proceso2;
    QProcess *proceso3;

    // For use the class dbasepostgresql by Misael M Del Valle
    DBasePostgreSQL* myDB;

};

#endif // SIVISO_H
