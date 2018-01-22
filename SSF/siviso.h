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
    int nSensor;
    int tipoSensor;
    int longBTR;
    int longLF;
    int longDEMON;
    int nWords;
    QString catchSend;
    QString compGraf;
    QString catchSensor;
    QString catchCmd;
    PPI *myppi;
    Signal *mysignal;
    bool bToolButton;
    bool bRec;
    bool bPlay;
    bool bSensor;
    bool bAudio;
    bool bPulso;

private slots:
    void leerSocket();
    //void leerSerialDB9();
    void leerSerialUSB();

private slots:

    void on_btOpenPort_clicked();

    void on_tipo_norte_clicked();

    void on_lf_clicked();

    void on_btr_clicked();

    void on_ppi_clicked();

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

    void on_it_valueChanged(int arg1);

    void on_rec_clicked();

    void on_play_clicked();

    void on_toolButton_clicked();

    void on_save_clicked();

    void on_setColorUp_valueChanged(int value);

    void on_setColorDw_valueChanged(int value);

    void on_frecP_valueChanged(int arg1);

    void on_nP_valueChanged(int arg1);

    void on_anchoP_valueChanged(int arg1);

    void deshabilitado(bool value);

    void on_cw_clicked();

    void on_startCom_clicked();

    void on_endCom_clicked();

    void on_sensor0_clicked();

    void on_sensor1_clicked();

    void on_openJars_clicked();

    void on_closeJars_clicked();

    void on_dial_valueChanged(int value);

    void on_ang_valueChanged(int arg1);

    void on_origenOmni_clicked();

    void on_origenManual_clicked();

    void on_dial_sliderReleased();

    void on_ang_editingFinished();

    void on_send_clicked();

    void on_infoSignal_clicked();

    void on_ApagarP_clicked();

    void on_ApagarA_clicked();

    void on_sensorOFF_clicked();

    void on_sensorON_clicked();

    void on_vol_dw_clicked();

    void on_vol_up_clicked();

    void on_demon_clicked();

    void on_chirpUp_clicked();

    void on_chirpDw_clicked();

    void on_chype_clicked();

    void on_chirpFrecUp_valueChanged(int arg1);

    void on_chirpFrecDw_valueChanged(int arg1);

    void on_chirpTime_valueChanged(double arg1);

    void on_chirpFrecUp_editingFinished();

    void on_chirpFrecDw_editingFinished();

    void on_chirpTime_editingFinished();

    void on_prob_deteccion_valueChanged(int arg1);

    void on_dataSim_clicked();

    void on_dist_clicked();

    void on_escala_ppi_valueChanged(int arg1);

private:
    Ui::siviso *ui;

    QUdpSocket *udpsocket;
    QHostAddress direccionSSF;
    QHostAddress direccionApp;
    quint16 puertoSSF;
    quint16 puertoDEMON;
    quint16 puertoBTR;
    quint16 puertoLF;
    quint16 puertoPPI;
    quint16 puertoREC;
    quint16 puertoComSF;
    QSerialPort *serialPortDB9;
    QSerialPort *serialPortUSB;
    QProcess *proceso1;
    QProcess *proceso2;
    QProcess *proceso3;
    QProcess *proceso4;
    QProcess *proceso5;

    // For use the class dbasepostgresql by Misael M Del Valle
    DBasePostgreSQL* myDB;

};

#endif // SIVISO_H
